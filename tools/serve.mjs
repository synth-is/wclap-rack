import http from 'node:http';
import fs from 'node:fs';
import path from 'node:path';
import { execFileSync } from 'node:child_process';
import { bundleRoots, handleWclapRequest } from './wclap-bundles.mjs';
// serve.mjs — no-store static server for the test host (Chrome otherwise heuristically caches
// modules across edits). Usage: node tools/serve.mjs [port] [root]
//
// Besides the repo root it also serves the SELF-HOSTED WCLAP bundle routes
// (tools/wclap-bundles.mjs, same shapes as the burst service):
//   GET /wclap/engines          installed engine bundles across the bundle roots
//   GET /wclap/<engine>/<file>  one file from an engine's bundle (module.wasm, ui/…)
// Bundle roots: WCLAP_BUNDLE_ROOTS env (colon-separated), default
// "<repo>/bundle-host:~/wclap-bundles" — see tools/sync-bundle-host.mjs.
const PORT = Number(process.argv[2] ?? 8137);
const ROOT = process.argv[3] ?? path.resolve(path.dirname(new URL(import.meta.url).pathname), '..');
const MIME = { '.html': 'text/html', '.js': 'text/javascript', '.mjs': 'text/javascript', '.json': 'application/json', '.gz': 'application/gzip', '.png': 'image/png', '.css': 'text/css' };
const WCLAP_ROOTS = bundleRoots(process.env, ROOT);
http.createServer(async (req, res) => {
  try {
    const pathname = decodeURIComponent(new URL(req.url, 'http://x').pathname);
    // /proxy/synthis/<path> — dev-only CORS proxy to api.synth.is (the API answers browser
    // preflights from unknown origins with 500, so the test host fetches through here instead).
    if (pathname.startsWith('/proxy/synthis/')) {
      const target = 'https://api.synth.is/' + pathname.slice('/proxy/synthis/'.length) +
        (new URL(req.url, 'http://x').search ?? '');
      const upstream = await fetch(target);
      const buf = Buffer.from(await upstream.arrayBuffer());
      res.writeHead(upstream.status, {
        'content-type': upstream.headers.get('content-type') ?? 'application/json',
        'cache-control': 'no-store',
        'access-control-allow-origin': '*',
      });
      res.end(buf);
      return;
    }
    // /wclap/… — self-hosted engine bundles (our own platform, not an external studio)
    if (pathname.startsWith('/wclap/')) {
      const out = handleWclapRequest(pathname, WCLAP_ROOTS);
      if (out) {
        res.writeHead(out.status, out.headers);
        res.end(out.body);
        return;
      }
    }
    // /wclap-faust/<soundId>/<file> — the faust2wasm pair (genome.json / genome.wasm / genome.dsp)
    // the faust-runner WCLAP runs, compiled ON DEMAND per Synth.is faust sound by
    // tools/compile-faust-sound.mjs (needs the Faust compiler) and cached under
    // <first bundle root>/wclap-faust/<soundId>/ — this is what makes the faust-runner
    // WCLAP usable in the page for ANY faust sound, not only ones with a delivered rack.
    if (pathname.startsWith('/wclap-faust/')) {
      const parts = pathname.split('/').filter(Boolean); // ['wclap-faust', soundId, file]
      if (parts.length === 3 && ['genome.json', 'genome.wasm', 'genome.dsp'].includes(parts[2])) {
        const dir = path.join(WCLAP_ROOTS[0], 'wclap-faust', parts[1]);
        const file = path.join(dir, parts[2]);
        if (!fs.existsSync(file)) {
          try {
            execFileSync(process.execPath,
              [path.join(ROOT, 'tools', 'compile-faust-sound.mjs'), parts[1], dir],
              { stdio: 'inherit', timeout: 180000 });
          } catch { res.writeHead(502); res.end('faust compile failed'); return; }
        }
        if (!fs.existsSync(file)) { res.writeHead(404); res.end('nf'); return; }
        res.writeHead(200, {
          'content-type': parts[2].endsWith('.wasm') ? 'application/wasm'
            : parts[2].endsWith('.json') ? 'application/json' : 'text/plain; charset=utf-8',
          'cache-control': 'no-store',
        });
        res.end(fs.readFileSync(file));
        return;
      }
    }
    const p = path.join(ROOT, pathname);
    const file = fs.statSync(p).isDirectory() ? path.join(p, 'index.html') : p;
    const data = fs.readFileSync(file);
    res.writeHead(200, { 'content-type': MIME[path.extname(file)] ?? 'application/octet-stream', 'cache-control': 'no-store' });
    res.end(data);
  } catch { res.writeHead(404); res.end('nf'); }
}).listen(PORT, '127.0.0.1', () => console.log(`serving ${ROOT} on http://127.0.0.1:${PORT} (no-store, /proxy/synthis/ -> api.synth.is, /wclap/ -> [${WCLAP_ROOTS.join(', ')}])`));
