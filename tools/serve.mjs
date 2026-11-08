import http from 'node:http';
import fs from 'node:fs';
import path from 'node:path';
// serve.mjs — no-store static server for the test host (Chrome otherwise heuristically caches
// modules across edits). Usage: node tools/serve.mjs [port] [root]
const PORT = Number(process.argv[2] ?? 8137);
const ROOT = process.argv[3] ?? path.resolve(path.dirname(new URL(import.meta.url).pathname), '..');
const MIME = { '.html': 'text/html', '.js': 'text/javascript', '.mjs': 'text/javascript', '.json': 'application/json', '.gz': 'application/gzip', '.png': 'image/png', '.css': 'text/css' };
http.createServer((req, res) => {
  try {
    const p = path.join(ROOT, decodeURIComponent(new URL(req.url, 'http://x').pathname));
    const file = fs.statSync(p).isDirectory() ? path.join(p, 'index.html') : p;
    const data = fs.readFileSync(file);
    res.writeHead(200, { 'content-type': MIME[path.extname(file)] ?? 'application/octet-stream', 'cache-control': 'no-store' });
    res.end(data);
  } catch { res.writeHead(404); res.end('nf'); }
}).listen(PORT, '127.0.0.1', () => console.log(`serving ${ROOT} on http://127.0.0.1:${PORT} (no-store)`));
