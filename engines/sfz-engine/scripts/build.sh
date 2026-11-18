#!/usr/bin/env bash
# Build the sfz-engine WCLAP module: engines/sfz-engine/build/module.wasm (plus LICENSE, NOTICE,
# BUILD-INFO.txt) and the packed bundle engines/sfz-engine/sfz-engine.wclap.tar.gz.
# Toolchain: Emscripten 3.1.51 from emsdk (the same as tests/bundles/testfx). Override with EMSDK=/path/to/emsdk.
# Usage: engines/sfz-engine/scripts/build.sh [--clean]
set -euo pipefail

HERE="$(cd "$(dirname "$0")/.." && pwd)"
OUT="$HERE/build"
ARCHIVE="$HERE/sfz-engine.wclap.tar.gz"
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

SRCS=("$HERE/src/sfz_engine_clap.cc")

mkdir -p "$OUT"
# Same link discipline as tests/bundles/testfx/scripts/build.sh: standalone wasm, clap_entry + malloc
# exported, growable memory and table, WASI-only imports, no exceptions/RTTI, no -ffast-math.
# INITIAL_MEMORY is larger than testfx's: activate() loads the delivered samples into memory.
emcc -std=c++17 -O3 -fno-exceptions -fno-rtti -DNDEBUG \
    -ffile-prefix-map="$HERE=." -fdebug-prefix-map="$HERE=." \
    -I"$HERE/third_party/clap/include" \
    "${SRCS[@]}" \
    -sSTANDALONE_WASM=1 --no-entry \
    -sEXPORTED_FUNCTIONS=_malloc,_free -Wl,--export=clap_entry \
    -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=16MB -sSTACK_SIZE=256KB -sALLOW_TABLE_GROWTH=1 \
    -sERROR_ON_UNDEFINED_SYMBOLS=1 \
    -o "$OUT/module.wasm"

cp "$HERE/LICENSE" "$OUT/LICENSE"
cp "$HERE/NOTICE" "$OUT/NOTICE"
{
    echo "built:      $(date -u +%Y-%m-%dT%H:%M:%SZ) (informational; the module bytes do not depend on it)"
    echo "emcc:       $(emcc --version | head -1)"
    echo "emsdk:      ${EMSDK_USED:-${EMSDK:-$(dirname "$(dirname "$(command -v emcc)")")}}"
    echo "flags:      -std=c++17 -O3 -fno-exceptions -fno-rtti, standalone wasm (see scripts/build.sh)"
    echo "module.wasm sha256: $(shasum -a 256 "$OUT/module.wasm" | cut -d ' ' -f1)"
    echo "sources (sha256):"
    (cd "$HERE" && shasum -a 256 src/sfz_engine_clap.cc | sed 's/^/  /')
} > "$OUT/BUILD-INFO.txt"

# pack the WCLAP bundle archive a rack bundle's MANIFEST references
STAGE="$HERE/../.bundle-stage"
rm -rf "$STAGE"
mkdir -p "$STAGE/sfz-engine.wclap"
cp "$OUT/module.wasm" "$HERE/LICENSE" "$HERE/NOTICE" "$STAGE/sfz-engine.wclap/"
(cd "$STAGE" && tar czf "$ARCHIVE" sfz-engine.wclap)
rm -rf "$STAGE"
echo "built $OUT/module.wasm ($(wc -c < "$OUT/module.wasm") bytes)"
echo "bundle packed to $ARCHIVE"
cat "$OUT/BUILD-INFO.txt"
