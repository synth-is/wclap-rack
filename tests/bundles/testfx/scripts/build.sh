#!/usr/bin/env bash
# Build the testfx fixture as a standalone WCLAP module: tests/bundles/testfx/build/module.wasm
# (plus LICENSE, NOTICE and BUILD-INFO.txt next to it). The build products are not committed
# (engines/testfx/.gitignore); the module is also copied to the test fixture
# tests/fixtures/testfx/module.wasm.
#
# Toolchain: Emscripten 3.1.51 from emsdk (the same as engines/dx7 and the Surge XT WCLAP).
# Override the location with EMSDK=/path/to/emsdk.
#
# Usage: engines/testfx/scripts/build.sh [--clean]
set -euo pipefail

HERE="$(cd "$(dirname "$0")/.." && pwd)"
OUT="$HERE/build"
ARCHIVE="$HERE/../testfx.wclap.tar.gz"
WANT_EMCC="3.1.51"

if [ "${1:-}" = "--clean" ]; then rm -rf "$HERE/build"; fi

if ! command -v emcc >/dev/null 2>&1 || [ "$(emcc --version | head -1 | sed -E 's/.* ([0-9]+\.[0-9]+\.[0-9]+).*/\1/')" != "$WANT_EMCC" ]; then
    for env in "${EMSDK:+$EMSDK/emsdk_env.sh}" "$HOME/wclap-native/emsdk/emsdk_env.sh" "$HOME/emsdk/emsdk_env.sh"; do
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

SRCS=("$HERE/src/testfx_clap.cc")

mkdir -p "$OUT"
# Link options follow engines/dx7/scripts/build.sh (which follows juce_webclap.cmake's
# JUCE_WEBCLAP_DSP_LINK_OPTIONS): standalone wasm, clap_entry + malloc exported, growable memory and table.
# No exceptions or RTTI, no -ffast-math (flags that change float results are not used).
emcc -std=c++17 -O3 -fno-exceptions -fno-rtti -DNDEBUG \
    -ffile-prefix-map="$HERE=." -fdebug-prefix-map="$HERE=." \
    -I"$HERE/third_party/clap/include" \
    "${SRCS[@]}" \
    -sSTANDALONE_WASM=1 --no-entry \
    -sEXPORTED_FUNCTIONS=_malloc,_free -Wl,--export=clap_entry \
    -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=4MB -sSTACK_SIZE=256KB -sALLOW_TABLE_GROWTH=1 \
    -sERROR_ON_UNDEFINED_SYMBOLS=1 \
    -o "$OUT/module.wasm"

cp "$HERE/LICENSE" "$OUT/LICENSE"
cp "$HERE/NOTICE" "$OUT/NOTICE"
{
    echo "built:      $(date -u +%Y-%m-%dT%H:%M:%SZ) (informational; the module bytes do not depend on it)"
    echo "emcc:       $(emcc --version | head -1)"
    echo "emsdk:      ${EMSDK_USED:-${EMSDK:-$(dirname "$(dirname "$(command -v emcc)")")}}"
    echo "flags:      -std=c++17 -O3 -fno-exceptions -fno-rtti, standalone wasm (see scripts/build.sh)"
    echo "module.wasm sha256: $(shasum -a 256 "$OUT/module.wasm" | cut -d' ' -f1)"
    echo "sources (sha256):"
    (cd "$HERE" && shasum -a 256 src/testfx_clap.cc | sed 's/^/  /')
} > "$OUT/BUILD-INFO.txt"

# pack the WCLAP bundle archive the rack bundle's MANIFEST references
mkdir -p "$HERE/../bundle/testfx.wclap"
cp "$OUT/module.wasm" "$HERE/LICENSE" "$HERE/NOTICE" "$HERE/../bundle/testfx.wclap/"
(cd "$HERE/../bundle" && tar czf "$ARCHIVE" testfx.wclap)
rm -rf "$HERE/../bundle"
echo "built $OUT/module.wasm ($(wc -c < "$OUT/module.wasm") bytes)"
echo "bundle packed to $ARCHIVE"
cat "$OUT/BUILD-INFO.txt"
