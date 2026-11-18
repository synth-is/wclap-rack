import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';

/**
 * sync-bundle-host.mjs — (re)populate bundle-host/, the self-hosted WCLAP bundle
 * root served at /wclap/<engine>/<file> by tools/serve.mjs (tools/wclap-bundles.mjs).
 *
 * Sources, in order:
 *   1. engines/* /build        -> bundle-host/<engine>     (symlink; our own engines)
 *   2. examples/<dir>/<id>.wclap.tar.gz -> bundle-host/<id>  (unpacked, <id>.wclap/ prefix stripped)
 *   3. $OPEN_WEBCLAP_ROOT/build/<port>/<id>.wclap.tar.gz    (open-webclap ports that have been
 *      built locally; default ../open-webclap)             -> bundle-host/<port>
 * ~/wclap-bundles (the production bundles, see kromosynth-burst config/wclap-bundles.env)
 * is a bundle root too, but is NOT written here — only read by the server.
 *
 * bundle-host/ is generated (gitignored); only entries listed in its
 * .sync-manifest.json are removed and recreated, nothing else is touched.
 *
 * Usage: node tools/sync-bundle-host.mjs
 */

const REPO = path.resolve(path.dirname(new URL(import.meta.url).pathname), '..');
const HOST = path.join(REPO, 'bundle-host');
const MANIFEST = path.join(HOST, '.sync-manifest.json');

// ── minimal ustar reader (handles GNU 'L' longnames; pax 'x' headers skipped) ──

function untarGz(buf) {
  const tar = zlib.gunzipSync(buf);
  const files = [];
  let off = 0;
  let longname = null;
  while (off + 512 <= tar.length) {
    const block = tar.subarray(off, off + 512);
    if (block.every((b) => b === 0)) break;
    const nameRaw = block.subarray(0, 100).toString('utf8').replace(/\0.*$/, '');
    const prefix = block.subarray(345, 500).toString('utf8').replace(/\0.*$/, '');
    const size = parseInt(block.subarray(124, 136).toString('utf8').replace(/\0.*$/, '').trim() || '0', 8);
    const type = String.fromCharCode(block[156]);
    const data = tar.subarray(off + 512, off + 512 + size);
    off += 512 + Math.ceil(size / 512) * 512;
    if (type === 'L') { longname = data.toString('utf8').replace(/\0.*$/, ''); continue; }
    if (type === 'x' || type === 'g') continue; // pax headers: short names only here
    const name = (longname || (prefix ? `${prefix}/${nameRaw}` : nameRaw)).replace(/^\.\//, '');
    longname = null;
    if (!name || type === '5') { if (type === '5') files.push({ name, dir: true }); continue; }
    files.push({ name, data });
  }
  return files;
}

function unpackTarInto(tarGzPath, destDir) {
  const files = untarGz(fs.readFileSync(tarGzPath));
  // strip the top-level "<id>.wclap/" component the packers add
  const stripped = files.map((f) => ({ ...f, name: f.name.replace(/^[^/]+\.wclap\//, '') }))
    .filter((f) => f.name && f.name !== '.');
  for (const f of stripped) {
    const p = path.join(destDir, f.name);
    if (!p.startsWith(destDir + path.sep) && p !== destDir) throw new Error(`bad path in ${tarGzPath}: ${f.name}`);
    if (f.dir) { fs.mkdirSync(p, { recursive: true }); continue; }
    fs.mkdirSync(path.dirname(p), { recursive: true });
    fs.writeFileSync(p, f.data);
  }
  return stripped.length;
}

function* walk(dir, depth = 3) {
  let ents = [];
  try { ents = fs.readdirSync(dir, { withFileTypes: true }); } catch { return; }
  for (const e of ents) {
    const p = path.join(dir, e.name);
    if (e.isDirectory() && depth > 0) yield* walk(p, depth - 1);
    else if (e.isFile()) yield p;
  }
}

function main() {
  fs.mkdirSync(HOST, { recursive: true });
  // remove only what a previous sync created
  let prev = [];
  try { prev = JSON.parse(fs.readFileSync(MANIFEST, 'utf8')).entries || []; } catch { /* first run */ }
  for (const e of prev) {
    const p = path.join(HOST, e.name);
    if (p.startsWith(HOST + path.sep)) fs.rmSync(p, { recursive: true, force: true });
  }

  const created = [];
  const linkOrCopy = (name, target) => {
    const p = path.join(HOST, name);
    try { fs.symlinkSync(target, p, 'dir'); }
    catch { fs.cpSync(target, p, { recursive: true }); }
    created.push({ name, kind: 'symlink', source: target });
  };

  // 1. our own engine builds
  const enginesDir = path.join(REPO, 'engines');
  for (const e of fs.readdirSync(enginesDir, { withFileTypes: true })) {
    if (!e.isDirectory()) continue;
    const build = path.join(enginesDir, e.name, 'build');
    if (fs.existsSync(path.join(build, 'module.wasm'))) {
      linkOrCopy(e.name, path.relative(HOST, build));
      console.log(`  ${e.name}  <- engines/${e.name}/build`);
    }
  }

  // 2. example port bundles shipped in the repo (wet-chorus, …)
  for (const p of walk(path.join(REPO, 'examples'), 2)) {
    if (!p.endsWith('.wclap.tar.gz')) continue;
    const id = path.basename(p).replace(/\.wclap\.tar\.gz$/, '');
    const dest = path.join(HOST, id);
    fs.mkdirSync(dest, { recursive: true });
    const n = unpackTarInto(p, dest);
    created.push({ name: id, kind: 'tar', source: path.relative(REPO, p) });
    console.log(`  ${id}  <- ${path.relative(REPO, p)} (${n} files)`);
  }

  // 3. locally built open-webclap ports
  const owRoot = process.env.OPEN_WEBCLAP_ROOT || path.resolve(REPO, '..', 'open-webclap');
  const owBuild = path.join(owRoot, 'build');
  for (const p of walk(owBuild, 2)) {
    if (!p.endsWith('.wclap.tar.gz')) continue;
    const id = path.basename(p).replace(/\.wclap\.tar\.gz$/, '');
    const dest = path.join(HOST, id);
    fs.mkdirSync(dest, { recursive: true });
    const n = unpackTarInto(p, dest);
    created.push({ name: id, kind: 'tar', source: path.relative(REPO, p) });
    console.log(`  ${id}  <- ${path.relative(REPO, p)} (${n} files)`);
  }

  // 4. kromosynth-burst engine builds (engines/<engine>/build/<engine>.wclap/ dirs, e.g. dx7
  //    from engines/dx7/scripts/build.sh) — symlinked as bundle-host/<engine>
  const burstRoot = process.env.KROMOSYNTH_BURST_ROOT || path.resolve(REPO, '..', '..', 'synth.is', 'kromosynth-burst');
  for (const p of walk(path.join(burstRoot, 'engines'), 4)) {
    if (path.basename(p) !== 'module.wasm') continue;
    const bundleDir = path.dirname(p);                    // …/engines/<engine>/build/<engine>.wclap
    if (!path.basename(bundleDir).endsWith('.wclap')) continue;
    const engine = path.basename(path.dirname(path.dirname(bundleDir))); // <engine>
    linkOrCopy(engine, path.relative(HOST, bundleDir));
    console.log(`  ${engine}  <- ${path.relative(REPO, bundleDir)}`);
  }

  // 5. dist/<id>.wclap.tar.gz — downloaded release archives (e.g. from our own
  // synth-is/open-webclap GitHub releases once CI publishes built bundles).
  // This is how the remaining open-webclap ports become "installed" on our host
  // without building them here.
  for (const p of walk(path.join(REPO, 'dist'), 1)) {
    if (!p.endsWith('.wclap.tar.gz')) continue;
    const id = path.basename(p).replace(/\.wclap\.tar\.gz$/, '');
    const dest = path.join(HOST, id);
    fs.mkdirSync(dest, { recursive: true });
    const n = unpackTarInto(p, dest);
    created.push({ name: id, kind: 'dist', source: path.relative(REPO, p) });
    console.log(`  ${id}  <- ${path.relative(REPO, p)} (${n} files)`);
  }

  fs.writeFileSync(MANIFEST, JSON.stringify({ updated: new Date().toISOString(), entries: created }, null, 2));
  console.log(`bundle-host: ${created.length} engines at ${HOST}`);
}

main();
