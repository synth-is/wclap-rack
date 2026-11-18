import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';

/**
 * wclap-bundles.mjs — the self-hosted WCLAP bundle routes for tools/serve.mjs,
 * mirroring the burst service's public bundle routes (kromosynth-burst
 * server.mjs: GET /wclap/engines, GET /wclap/<engine>/<file>) so the web app can
 * load engine bundles from OUR OWN host instead of an external studio.
 *
 * Bundle roots: WCLAP_BUNDLE_ROOTS (colon-separated), default
 *   <repo>/bundle-host : ~/wclap-bundles
 * bundle-host/ is populated by tools/sync-bundle-host.mjs (engines builds,
 * examples tars, open-webclap build tars — gitignored).
 * ~/wclap-bundles is the production layout (kromosynth-burst
 * config/wclap-bundles.env): <root>/<engine>/<file>.wclap/ is the unpacked
 * bundle, so an engine dir may itself contain *.wclap sub-bundles.
 *
 * Layout per engine, either way: a directory containing module.wasm (plus
 * ui/, LICENSE, … when the bundle ships them).
 */

/** Port ids whose bundle dir is named after the engine substrate instead. */
export const ENGINE_ALIASES = Object.freeze({ dexed: 'dx7', 'six-sines': 'sixsines' });

const WCLAP_TYPES = {
  '.wasm': 'application/wasm', '.js': 'text/javascript; charset=utf-8',
  '.mjs': 'text/javascript; charset=utf-8', '.html': 'text/html; charset=utf-8',
  '.json': 'application/json', '.css': 'text/css; charset=utf-8',
  '.png': 'image/png', '.svg': 'image/svg+xml', '.ico': 'image/x-icon',
  '.woff': 'font/woff', '.woff2': 'font/woff2', '.ttf': 'font/ttf',
  '.txt': 'text/plain; charset=utf-8', '.md': 'text/plain; charset=utf-8',
  '.map': 'application/json', '.gif': 'image/gif', '.jpg': 'image/jpeg',
  '.wav': 'audio/wav', '.mp3': 'audio/mpeg', '.sfz': 'text/plain; charset=utf-8',
  '.tar.gz': 'application/gzip',
};

/** The bundle roots this server searches, in order (first hit wins). */
export function bundleRoots(env = process.env, repoRoot = path.resolve(path.dirname(new URL(import.meta.url).pathname), '..')) {
  const raw = env.WCLAP_BUNDLE_ROOTS
    || [path.join(repoRoot, 'bundle-host'), path.join(os.homedir(), 'wclap-bundles')].join(':');
  return raw.split(':').map((p) => p.trim()).filter(Boolean).map((p) => path.resolve(p));
}

const isBundleDir = (d) => {
  try { return fs.statSync(d).isDirectory() && fs.existsSync(path.join(d, 'module.wasm')); }
  catch { return false; }
};

/**
 * Resolve one engine to its bundle directory across all roots. Accepts the
 * engine/substrate name (surge-xt, dx7, cppn-engine, …) and the port-id
 * aliases (dexed -> dx7, six-sines -> sixsines). Returns null when no root
 * has a module.wasm for it.
 */
export function resolveEngineDir(engine, roots) {
  for (const name of [engine, ENGINE_ALIASES[engine]].filter(Boolean)) {
    for (const root of roots) {
      const direct = path.join(root, name);
      if (isBundleDir(direct)) return direct;
      // ~/wclap-bundles layout: <root>/<engine>/<file>.wclap/ is the bundle
      try {
        if (fs.statSync(direct).isDirectory()) {
          for (const sub of fs.readdirSync(direct).sort()) {
            const d = path.join(direct, sub);
            if (sub.endsWith('.wclap') && isBundleDir(d)) return d;
          }
        }
      } catch { /* next root */ }
    }
  }
  return null;
}

/** All files under dir, as /-separated relative paths (cap 1000, sorted). */
function walkFiles(dir, cap = 1000) {
  const out = [];
  const walk = (d, prefix) => {
    let ents = [];
    try { ents = fs.readdirSync(d, { withFileTypes: true }); } catch { return; }
    for (const e of ents.sort((a, b) => a.name.localeCompare(b.name))) {
      if (out.length >= cap) return;
      const p = path.join(d, e.name);
      const rel = prefix ? `${prefix}/${e.name}` : e.name;
      if (e.isDirectory()) walk(p, rel);
      else if (e.isFile() || e.isSymbolicLink()) out.push(rel);
    }
  };
  walk(dir, '');
  return out;
}

/**
 * GET /wclap/engines — one row per engine found in any root:
 *   [{ engine, installed, hasUi, files }]
 * `engine` is the root-level dir name (substrate id or port id). `installed`
 * is always true here (only bundles with module.wasm are listed); the burst
 * service also lists not-installed engines with a `why` — this static host
 * simply has nothing to say about those. `files` lists the bundle's files
 * relative to its root, so a client can fetch the ui/ payload without
 * unpacking the archive (cap 1000 entries).
 */
export function listEngines(roots) {
  const map = new Map(); // engine -> { hasUi, files }
  for (const root of roots) {
    let entries = [];
    try { entries = fs.readdirSync(root, { withFileTypes: true }); } catch { continue; }
    for (const ent of entries) {
      if (!ent.isDirectory() && !ent.isSymbolicLink()) continue;
      const name = ent.name;
      if (name.startsWith('.')) continue;
      const dir = resolveEngineDir(name, [root]);
      if (!dir) continue;
      const prev = map.get(name) || { hasUi: false, files: [] };
      const files = walkFiles(dir);
      map.set(name, {
        hasUi: prev.hasUi || files.includes('ui/index.html') || files.includes('index.html'),
        files: [...new Set([...prev.files, ...files])],
      });
    }
  }
  return [...map.entries()].map(([engine, v]) => ({ engine, installed: true, hasUi: v.hasUi, files: v.files }));
}

/**
 * GET /wclap/<engine>/<file> — serve one file from the engine's bundle dir.
 * Returns { status, headers, body } or null when the route does not match.
 * Path traversal is refused; only files inside the resolved bundle dir are read.
 */
export function serveBundleFile(pathname, roots) {
  const parts = pathname.slice('/wclap/'.length).split('/');
  const engine = parts[0];
  if (!engine) return null;
  let rel;
  try { rel = parts.slice(1).map((s) => decodeURIComponent(s)).join('/'); }
  catch { return { status: 400, headers: { 'content-type': 'application/json' }, body: JSON.stringify({ ok: false, error: 'bad path' }) }; }
  if (!rel || rel.split('/').includes('..')) {
    return { status: 400, headers: { 'content-type': 'application/json' }, body: JSON.stringify({ ok: false, error: 'bad path' }) };
  }
  const dir = resolveEngineDir(engine, roots);
  if (!dir) {
    return { status: 404, headers: { 'content-type': 'application/json' }, body: JSON.stringify({ ok: false, error: `no bundle installed for engine "${engine}"` }) };
  }
  const file = path.resolve(dir, rel);
  if (file !== dir && !file.startsWith(dir + path.sep)) {
    return { status: 400, headers: { 'content-type': 'application/json' }, body: JSON.stringify({ ok: false, error: 'bad path' }) };
  }
  let st;
  try { st = fs.statSync(file); } catch { st = null; }
  if (!st || !st.isFile()) {
    return { status: 404, headers: { 'content-type': 'application/json' }, body: JSON.stringify({ ok: false, error: `no such file in the ${engine} bundle: ${rel}` }) };
  }
  const ext = path.extname(file).toLowerCase();
  return {
    status: 200,
    headers: {
      'content-type': WCLAP_TYPES[ext] || 'application/octet-stream',
      'content-length': st.size,
      // Bundle files are NOT content-hashed: a rebuilt engine is served as the same module.wasm, so the
      // browser must ask again every time (an `immutable` week kept a page on an old engine build).
      'cache-control': 'no-cache',
      'last-modified': st.mtime.toUTCString(),
      etag: `"${st.size.toString(16)}-${Math.floor(st.mtimeMs).toString(16)}"`,
      'access-control-allow-origin': '*',
    },
    body: fs.readFileSync(file),
  };
}

/**
 * Handle a /wclap/… request. Returns { status, headers, body } or null when
 * the pathname is not a /wclap route (the caller falls through to static).
 */
export function handleWclapRequest(pathname, roots) {
  if (pathname === '/wclap/engines') {
    return {
      status: 200,
      headers: { 'content-type': 'application/json', 'access-control-allow-origin': '*', 'cache-control': 'no-store' },
      body: JSON.stringify({ ok: true, engines: listEngines(roots) }),
    };
  }
  if (pathname.startsWith('/wclap/')) return serveBundleFile(pathname, roots);
  return null;
}
