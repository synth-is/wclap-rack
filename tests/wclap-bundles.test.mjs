import { test } from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { bundleRoots, resolveEngineDir, listEngines, serveBundleFile, handleWclapRequest } from '../tools/wclap-bundles.mjs';

function tmpRoot() {
  return fs.mkdtempSync(path.join(os.tmpdir(), 'wclap-bundles-test-'));
}

function mkBundle(dir, { ui = false } = {}) {
  fs.mkdirSync(dir, { recursive: true });
  fs.writeFileSync(path.join(dir, 'module.wasm'), Buffer.from([0, 97, 115, 109]));
  if (ui) {
    fs.mkdirSync(path.join(dir, 'ui'), { recursive: true });
    fs.writeFileSync(path.join(dir, 'ui', 'index.html'), '<html></html>');
  }
}

test('listEngines finds direct bundle dirs and *.wclap sub-bundles, with hasUi', () => {
  const root = tmpRoot();
  mkBundle(path.join(root, 'cppn-engine'), { ui: true });
  mkBundle(path.join(root, 'wet-chorus'));
  mkBundle(path.join(root, 'dx7', 'dx7-abd0fb5f-native-exnref-c6249221.wclap'), { ui: true });
  fs.mkdirSync(path.join(root, 'not-a-bundle'), { recursive: true });
  const engines = listEngines([root]);
  const byName = Object.fromEntries(engines.map((e) => [e.engine, e]));
  assert.equal(byName['cppn-engine'].installed, true);
  assert.equal(byName['cppn-engine'].hasUi, true);
  assert.deepEqual(byName['cppn-engine'].files, ['module.wasm', 'ui/index.html']);
  assert.equal(byName['wet-chorus'].hasUi, false);
  assert.deepEqual(byName['wet-chorus'].files, ['module.wasm']);
  assert.equal(byName['dx7'].hasUi, true); // *.wclap sub-bundle layout
  assert.ok(byName['dx7'].files.includes('module.wasm'));
  assert.equal(byName['not-a-bundle'], undefined);
});

test('resolveEngineDir follows port-id aliases (dexed -> dx7)', () => {
  const root = tmpRoot();
  mkBundle(path.join(root, 'dx7', 'dx7-x.wclap'));
  assert.equal(resolveEngineDir('dx7', [root]), path.join(root, 'dx7', 'dx7-x.wclap'));
  assert.equal(resolveEngineDir('dexed', [root]), path.join(root, 'dx7', 'dx7-x.wclap'));
  assert.equal(resolveEngineDir('nope', [root]), null);
});

test('serveBundleFile serves module.wasm with wasm content type and refuses traversal', () => {
  const root = tmpRoot();
  const dir = path.join(root, 'cppn-engine');
  mkBundle(dir, { ui: true });
  const ok = serveBundleFile('/wclap/cppn-engine/module.wasm', [root]);
  assert.equal(ok.status, 200);
  assert.equal(ok.headers['content-type'], 'application/wasm');
  assert.equal(ok.headers['cache-control'], 'no-cache', 'module.wasm is not content-hashed: revalidated every time');
  assert.ok(ok.headers.etag, 'with an etag');
  assert.deepEqual([...ok.body], [0, 97, 115, 109]);
  const ui = serveBundleFile('/wclap/cppn-engine/ui/index.html', [root]);
  assert.equal(ui.status, 200);
  assert.equal(ui.headers['content-type'], 'text/html; charset=utf-8');
  assert.equal(serveBundleFile('/wclap/cppn-engine/../engines/cppn-engine/build/module.wasm', [root]).status, 400);
  assert.equal(serveBundleFile('/wclap/cppn-engine/nope.wasm', [root]).status, 404);
  assert.equal(serveBundleFile('/wclap/unknown/module.wasm', [root]).status, 404);
});

test('handleWclapRequest answers /wclap/engines and ignores other paths', () => {
  const root = tmpRoot();
  mkBundle(path.join(root, 'sfz-engine'));
  const engines = handleWclapRequest('/wclap/engines', [root]);
  assert.equal(engines.status, 200);
  const body = JSON.parse(engines.body);
  assert.equal(body.ok, true);
  assert.equal(body.engines.length, 1);
  assert.equal(body.engines[0].engine, 'sfz-engine');
  assert.equal(body.engines[0].installed, true);
  assert.deepEqual(body.engines[0].files, ['module.wasm']);
  assert.equal(handleWclapRequest('/engines/cppn-engine/build/module.wasm', [root]), null);
});

test('bundleRoots defaults to <repo>/bundle-host + ~/wclap-bundles, env override wins', () => {
  const repo = path.resolve(path.dirname(new URL(import.meta.url).pathname), '..');
  const def = bundleRoots({}, repo);
  assert.deepEqual(def, [path.join(repo, 'bundle-host'), path.join(os.homedir(), 'wclap-bundles')]);
  const custom = bundleRoots({ WCLAP_BUNDLE_ROOTS: '/a:/b' }, repo);
  assert.deepEqual(custom, ['/a', '/b']);
});
