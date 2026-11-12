#!/usr/bin/env bash
# Build the cppn-engine WCLAP module: engines/cppn-engine/build/module.wasm (plus LICENSE, NOTICE,
# BUILD-INFO.txt) and the packed bundle engines/cppn-engine/cppn-engine.wclap.tar.gz.
# Toolchain: Emscripten 3.1.51 from emsdk (the same as tests/bundles/testfx and engines/sfz-engine).
# Override with EMSDK=/path/to/emsdk. Usage: engines/cppn-engine/scripts/build.sh [--clean]
set -euo pipefail

HERE="$(cd "$(dirname "$0")/.." && pwd)"
OUT="$HERE/build"
ARCHIVE="$HERE/cppn-engine.wclap.tar.gz"
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

SRCS=("$HERE/src/cppn_engine_clap.cc")

mkdir -p "$OUT"
# Same link discipline as engines/sfz-engine/scripts/build.sh: standalone wasm, clap_entry + malloc
# exported, growable memory and table, WASI-only imports, no exceptions/RTTI, no -ffast-math.
emcc -std=c++17 -O3 -fno-exceptions -fno-rtti -DNDEBUG \
    -ffile-prefix-map="$HERE=." -fdebug-prefix-map="$HERE=." \
    -I"$HERE/third_party/clap/include" \
    "${SRCS[@]}" \
    -sSTANDALONE_WASM=1 --no-entry \
    -sEXPORTED_FUNCTIONS=_malloc,_free -Wl,--export=clap_entry \
    -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=16MB -sSTACK_SIZE=512KB -sALLOW_TABLE_GROWTH=1 \
    -sERROR_ON_UNDEFINED_SYMBOLS=1 \
    -o "$OUT/module.wasm"

cp "$HERE/LICENSE" "$OUT/LICENSE"
cp "$HERE/NOTICE" "$OUT/NOTICE"
rm -rf "$OUT/ui" && cp -R "$HERE/ui" "$OUT/ui"   # served next to module.wasm by hosts that load the build dir
{
    echo "built:      $(date -u +%Y-%m-%dT%H:%M:%SZ) (informational; the module bytes do not depend on it)"
    echo "emcc:       $(emcc --version | head -1)"
    echo "emsdk:      ${EMSDK_USED:-${EMSDK:-$(dirname "$(dirname "$(command -v emcc)")")}}"
    echo "flags:      -std=c++17 -O3 -fno-exceptions -fno-rtti, standalone wasm (see scripts/build.sh)"
    echo "module.wasm sha256: $(shasum -a 256 "$OUT/module.wasm" | cut -d ' ' -f1)"
    echo "sources (sha256):"
    (cd "$HERE" && shasum -a 256 src/cppn_engine_clap.cc src/cppn_core.h src/cppn_graph.h src/faust_nodes.h src/faust_templates/all.h src/mini_json.h | sed 's/^/  /')
} > "$OUT/BUILD-INFO.txt"

# pack the WCLAP bundle archive a rack bundle's MANIFEST references
STAGE="$HERE/../.bundle-stage"
rm -rf "$STAGE"
mkdir -p "$STAGE/cppn-engine.wclap"
cp "$OUT/module.wasm" "$HERE/LICENSE" "$HERE/NOTICE" "$STAGE/cppn-engine.wclap/"
cp -R "$HERE/ui" "$STAGE/cppn-engine.wclap/ui"   # the editor page (clap.webview get_uri = ui/index.html)
(cd "$STAGE" && tar czf "$ARCHIVE" cppn-engine.wclap)
rm -rf "$STAGE"
echo "built $OUT/module.wasm ($(wc -c < "$OUT/module.wasm") bytes)"
echo "bundle packed to $ARCHIVE"
cat "$OUT/BUILD-INFO.txt"
