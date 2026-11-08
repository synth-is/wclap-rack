# ports-rack — a rack built from real open-webclap ports

WetChorus (bucket-brigade chorus, MIT) → Pearls (granular delay/reverb after
Mutable Instruments' Beads, MIT), both ported in
[open-webclap](https://github.com/andremichelle/open-webclap). The archives are
copied from `open-webclap/site/public/bundled/`; `MANIFEST.json` pins each
`module.wasm` by sha256.

```sh
node tools/validate.mjs --bundle examples/ports-rack
node tools/render.mjs  --bundle examples/ports-rack --seconds 2 --out /tmp/ports-rack.wav
node tools/pack-rack.mjs examples/ports-rack --out examples/ports-rack/ports-rack.wclap-rack.tar.gz
# then in the browser test host:
#   serve the repo root, open /test-host/?bundle=/examples/ports-rack/ports-rack.wclap-rack.tar.gz
```

The plugin ids and parameter ids/names were dumped with the reference host's
`describeBundle` (see tools/render.mjs output); `params` keys in rack.json are
the numeric clap param ids as strings.
