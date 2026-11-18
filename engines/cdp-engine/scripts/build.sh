#!/usr/bin/env bash
# Build the cdp-engine WCLAP module: engines/cdp-engine/build/module.wasm (plus LICENSE, NOTICE,
# BUILD-INFO.txt, catalog.json) and the packed bundle engines/cdp-engine/cdp-engine.wclap.tar.gz.
# Toolchain: Emscripten 3.1.51 from emsdk (the same as tests/bundles/testfx and sfz-engine). Override with EMSDK=/path/to/emsdk.
# Usage: engines/cdp-engine/scripts/build.sh [--clean]
set -euo pipefail

HERE="$(cd "$(dirname "$0")/.." && pwd)"
OUT="$HERE/build"
ARCHIVE="$HERE/cdp-engine.wclap.tar.gz"
WANT_EMCC="3.1.51"

if [ "${1:-}" = "--clean" ]; then rm -rf "$OUT" "$ARCHIVE"; fi

if ! command -v emcc >/dev/null 2>&1 || [ "$(emcc --version | head -1 | sed -E 's/.* ([0-9]+\.[0-9]+\.[0-9]+).*/\1/')" != "$WANT_EMCC" ]; then
    for env in "${EMSDK:+$EMSDK/emsdk_env.sh}" "$HOME/emsdk/emsdk_env.sh"; do
        if [ -n "$env" ] && [ -f "$env" ]; then
            EMSDK_QUIET=1 source "$env" >/dev/null 2>&1
            EMSDK_USED="$(dirname "$env")"
            break
        fi
    done
fi
GOT_EMCC="$(emcc --version | head -1 | sed -E 's/.* ([0-9]+\.[0-9]+\.[0-9]+).*/\1/')"
if [ "$GOT_EMCC" != "$WANT_EMCC" ]; then
    echo "need Emscripten $WANT_EMCC, found $GOT_EMCC (set EMSDK to an emsdk with $WANT_EMCC activated)" >&2
    exit 1
fi

SRCS=("$HERE/src/cdp_engine_clap.cc")

mkdir -p "$OUT"
# Same link discipline as tests/bundles/testfx / sfz-engine: standalone wasm, clap_entry +
# malloc/free/cdp_deliver exported (the host calls cdp_deliver when an async wclap.cdp/1
# render completes), growable memory and table, no exceptions/RTTI, no -ffast-math.
# The wclap.cdp/1 render_request import comes from the import_module/import_name attributes
# in the source; hosts without the bridge answer ENOSYS and notes stay silent.
emcc -std=c++17 -O3 -fno-exceptions -fno-rtti -DNDEBUG \
    -ffile-prefix-map="$HERE=." -fdebug-prefix-map="$HERE=." \
    -I"$HERE/third_party/clap/include" \
    "${SRCS[@]}" \
    -sSTANDALONE_WASM=1 --no-entry \
    -sEXPORTED_FUNCTIONS=_malloc,_free,_cdp_deliver -Wl,--export=clap_entry \
    -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=16MB -sSTACK_SIZE=256KB -sALLOW_TABLE_GROWTH=1 \
    -sERROR_ON_UNDEFINED_SYMBOLS=0 \
    -o "$OUT/module.wasm"

# The wclap.cdp/1 render_request import is intentional (import_module attribute in the
# source); emscripten's undefined-symbol check still flags it, so ERROR_ON_UNDEFINED_SYMBOLS
# is off — verify the import section explicitly instead, so a real typo can't slip through.
node -e '
const fs = require("fs");
WebAssembly.compile(fs.readFileSync(process.argv[1])).then((m) => {
    const imps = WebAssembly.Module.imports(m).filter((i) => i.kind === "function");
    const want = imps.filter((i) => i.module === "wclap.cdp/1");
    if (want.length !== 1 || want[0].name !== "render_request")
        throw new Error("expected exactly one wclap.cdp/1.render_request import, got " + JSON.stringify(imps));
    const wasi = imps.filter((i) => i.module === "wasi_snapshot_preview1").map((i) => i.name).sort();
    const other = imps.filter((i) => i.module !== "wasi_snapshot_preview1" && i.module !== "wclap.cdp/1" &&
        !(i.module === "env" && i.name === "emscripten_notify_memory_growth"));  // same as sfz-engine/testfx
    if (other.length) throw new Error("unexpected imports: " + JSON.stringify(other));
    const exps = WebAssembly.Module.exports(m).map((e) => e.name);
    for (const e of ["clap_entry", "malloc", "free", "cdp_deliver"])  // EXPORTED_FUNCTIONS=_malloc exports as "malloc"
        if (!exps.includes(e)) throw new Error("missing export " + e);
    console.log("import check ok: wclap.cdp/1.render_request + " + wasi.length + " wasi imports; exports ok");
}).catch((e) => { console.error(e.message); process.exit(1); });
' "$OUT/module.wasm"

cp "$HERE/LICENSE" "$OUT/LICENSE"
cp "$HERE/NOTICE" "$OUT/NOTICE"
cp "$HERE/catalog.json" "$OUT/catalog.json"
mkdir -p "$OUT/ui"
cp "$HERE/ui/index.html" "$OUT/ui/index.html"
# the editor page fetches the catalog relative to itself
cp "$HERE/catalog.json" "$OUT/ui/catalog.json"
{
    echo "built:      $(date -u +%Y-%m-%dT%H:%M:%SZ) (informational; the module bytes do not depend on it)"
    echo "emcc:       $(emcc --version | head -1)"
    echo "emsdk:      ${EMSDK_USED:-${EMSDK:-$(dirname "$(dirname "$(command -v emcc)")")}}"
    echo "flags:      -std=c++17 -O3 -fno-exceptions -fno-rtti, standalone wasm (see scripts/build.sh)"
    echo "module.wasm sha256: $(shasum -a 256 "$OUT/module.wasm" | cut -d ' ' -f1)"
    echo "sources (sha256):"
    (cd "$HERE" && shasum -a 256 src/cdp_engine_clap.cc | sed 's/^/  /')
    echo "vendored:   third_party/cdp-wasm (cdp-wasm $(node -p "require('$HERE/third_party/cdp-wasm/package.json').version")) — see NOTICE"
} > "$OUT/BUILD-INFO.txt"

# pack the WCLAP bundle archive a rack bundle's MANIFEST references
STAGE="$HERE/../.bundle-stage"
rm -rf "$STAGE"
mkdir -p "$STAGE/cdp-engine.wclap"
cp "$OUT/module.wasm" "$HERE/LICENSE" "$HERE/NOTICE" "$OUT/BUILD-INFO.txt" "$HERE/catalog.json" "$STAGE/cdp-engine.wclap/"
cp -r "$OUT/ui" "$STAGE/cdp-engine.wclap/ui"
(cd "$STAGE" && tar czf "$ARCHIVE" cdp-engine.wclap)
rm -rf "$STAGE"
echo "built $OUT/module.wasm ($(wc -c < "$OUT/module.wasm") bytes)"
echo "bundle packed to $ARCHIVE"
cat "$OUT/BUILD-INFO.txt"
