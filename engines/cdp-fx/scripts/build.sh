#!/usr/bin/env bash
# Build the cdp-fx WCLAP module (passthrough placeholder for an offline CDP effect stage):
# engines/cdp-fx/build/module.wasm (+ LICENSE, NOTICE, BUILD-INFO.txt) and the packed bundle
# engines/cdp-fx/cdp-fx.wclap.tar.gz. Toolchain: Emscripten 3.1.51 from emsdk (same as
# cdp-engine). Override with EMSDK=/path/to/emsdk. Usage: engines/cdp-fx/scripts/build.sh [--clean]
set -euo pipefail

HERE="$(cd "$(dirname "$0")/.." && pwd)"
OUT="$HERE/build"
ARCHIVE="$HERE/cdp-fx.wclap.tar.gz"
CLAP_INC="$HERE/../cdp-engine/third_party/clap/include"
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

mkdir -p "$OUT"
# Same link discipline as cdp-engine / sfz-engine: standalone wasm, clap_entry + malloc/free
# exported, growable memory, no exceptions/RTTI. No host imports: this module never calls out.
emcc -std=c++17 -O3 -fno-exceptions -fno-rtti -DNDEBUG \
    -ffile-prefix-map="$HERE=." -fdebug-prefix-map="$HERE=." \
    -I"$CLAP_INC" \
    "$HERE/src/cdp_fx_clap.cc" \
    -sSTANDALONE_WASM=1 --no-entry \
    -sEXPORTED_FUNCTIONS=_malloc,_free -Wl,--export=clap_entry \
    -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=4MB -sSTACK_SIZE=128KB -sALLOW_TABLE_GROWTH=1 \
    -o "$OUT/module.wasm"

node -e '
const fs = require("fs");
WebAssembly.compile(fs.readFileSync(process.argv[1])).then((m) => {
    const imps = WebAssembly.Module.imports(m).filter((i) => i.kind === "function");
    const wasi = imps.filter((i) => i.module === "wasi_snapshot_preview1").map((i) => i.name).sort();
    const other = imps.filter((i) => i.module !== "wasi_snapshot_preview1" &&
        !(i.module === "env" && i.name === "emscripten_notify_memory_growth"));
    if (other.length) throw new Error("unexpected imports: " + JSON.stringify(other));
    const exps = WebAssembly.Module.exports(m).map((e) => e.name);
    for (const e of ["clap_entry", "malloc", "free"])
        if (!exps.includes(e)) throw new Error("missing export " + e);
    console.log("import check ok: " + wasi.length + " wasi imports, no host imports; exports ok");
}).catch((e) => { console.error(e.message); process.exit(1); });
' "$OUT/module.wasm"

cp "$HERE/LICENSE" "$OUT/LICENSE"
cp "$HERE/NOTICE" "$OUT/NOTICE"
# the editor page fetches the effect catalog relative to itself
mkdir -p "$OUT/ui"
cp "$HERE/ui/index.html" "$OUT/ui/index.html"
cp "$HERE/../cdp-engine/catalog.json" "$OUT/ui/catalog.json"
{
    echo "built:      $(date -u +%Y-%m-%dT%H:%M:%SZ) (informational; the module bytes do not depend on it)"
    echo "emcc:       $(emcc --version | head -1)"
    echo "emsdk:      ${EMSDK_USED:-${EMSDK:-$(dirname "$(dirname "$(command -v emcc)")")}}"
    echo "flags:      -std=c++17 -O3 -fno-exceptions -fno-rtti, standalone wasm (see scripts/build.sh)"
    echo "module.wasm sha256: $(shasum -a 256 "$OUT/module.wasm" | cut -d' ' -f1)"
    echo "sources (sha256):"
    (cd "$HERE" && shasum -a 256 src/cdp_fx_clap.cc | sed 's/^/  /')
} > "$OUT/BUILD-INFO.txt"

STAGE="$HERE/../.bundle-stage-fx"
rm -rf "$STAGE"
mkdir -p "$STAGE/cdp-fx.wclap"
cp "$OUT/module.wasm" "$HERE/LICENSE" "$HERE/NOTICE" "$OUT/BUILD-INFO.txt" "$STAGE/cdp-fx.wclap/"
cp -r "$OUT/ui" "$STAGE/cdp-fx.wclap/ui"
(cd "$STAGE" && tar czf "$ARCHIVE" cdp-fx.wclap)
rm -rf "$STAGE"
echo "built $OUT/module.wasm ($(wc -c < "$OUT/module.wasm") bytes)"
echo "bundle packed to $ARCHIVE"
cat "$OUT/BUILD-INFO.txt"
