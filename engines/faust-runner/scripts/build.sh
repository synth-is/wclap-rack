#!/usr/bin/env bash
# Build the faust-runner WCLAP module: engines/faust-runner/build/module.wasm (plus LICENSE, NOTICE,
# BUILD-INFO.txt) and the packed bundle engines/faust-runner/faust-runner.wclap.tar.gz.
# Toolchain: Emscripten 3.1.51 from emsdk (the same as tests/bundles/testfx and engines/sfz-engine).
# Override with EMSDK=/path/to/emsdk. Usage: engines/faust-runner/scripts/build.sh [--clean]
set -euo pipefail

HERE="$(cd "$(dirname "$0")/.." && pwd)"
OUT="$HERE/build"
ARCHIVE="$HERE/faust-runner.wclap.tar.gz"
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

WASM3="$HERE/third_party/wasm3"
# wasm3 v0.5.0 core (no WASI/uvwasi/tracer/libc APIs — the runner's genomes import only env math functions)
WASM3_SRCS=(
    "$WASM3/m3_core.c" "$WASM3/m3_parse.c" "$WASM3/m3_module.c" "$WASM3/m3_compile.c" "$WASM3/m3_exec.c"
    "$WASM3/m3_env.c" "$WASM3/m3_info.c" "$WASM3/m3_code.c" "$WASM3/m3_bind.c" "$WASM3/m3_function.c"
    "$WASM3/m3_emit.c" "$WASM3/m3_optimize.c"
)

mkdir -p "$OUT/wasm3"
# wasm3 is C: compile it separately with a C standard (emcc would otherwise pass -std=c++17).
# NOTE: the wasm3 objects are built with the SYSTEM clang (Apple clang 21, LLVM 21) targeting
# wasm32-unknown-emscripten against emscripten's sysroot — emsdk 3.1.51's own LLVM 18 miscompiles
# wasm3's interpreter when running on wasm32 (wrong values / table traps). The wrapper TU stays on
# emcc. Override the compiler with M3_CC=... ; set M3_CC=emcc to go back to emcc.
SYSROOT="${EMSDK:-$HOME/emsdk}/upstream/emscripten/cache/sysroot"
M3_CC="${M3_CC:-clang}"
for src in "${WASM3_SRCS[@]}"; do
    obj="$OUT/wasm3/$(basename "$src" .c).o"
    if [ "$M3_CC" = "emcc" ]; then
        emcc -std=gnu11 -O3 -fno-exceptions -fno-rtti -DNDEBUG \
            -I"$WASM3" -Dd_m3HasWASI=0 -Dd_m3RecordBacktraces=0 -Dd_m3Use32BitSlots=0 -c "$src" -o "$obj"
    else
        "$M3_CC" --target=wasm32-unknown-emscripten -O2 --sysroot="$SYSROOT" \
            -fno-exceptions -fno-rtti -DNDEBUG \
            -I"$WASM3" -Dd_m3HasWASI=0 -Dd_m3RecordBacktraces=0 -Dd_m3Use32BitSlots=0 -c "$src" -o "$obj"
    fi
done
# Same link discipline as testfx/sfz-engine: standalone wasm, clap_entry + malloc exported, growable
# memory and table, WASI-only imports at the outer level, no exceptions/RTTI, no -ffast-math.
# STACK_SIZE is larger: the embedded wasm3 interpreter compiles genome code into wasm frames.
emcc -std=c++17 -O1 -fno-exceptions -fno-rtti -DNDEBUG \
    -ffile-prefix-map="$HERE=." -fdebug-prefix-map="$HERE=." \
    -I"$HERE/third_party/clap/include" -I"$HERE/third_party/wasm3" -I"$HERE/src" \
    -Dd_m3HasWASI=0 -Dd_m3RecordBacktraces=0 -Dd_m3Use32BitSlots=0 \
    "$HERE/src/faust_runner_clap.cc" "$OUT"/wasm3/*.o \
    -sSTANDALONE_WASM=1 --no-entry \
    -sEXPORTED_FUNCTIONS=_malloc,_free -Wl,--export=clap_entry \
    -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=32MB -sSTACK_SIZE=8MB -sALLOW_TABLE_GROWTH=1 \
    -sERROR_ON_UNDEFINED_SYMBOLS=1 \
    -o "$OUT/module.wasm"

cp "$HERE/LICENSE" "$OUT/LICENSE"
cp "$HERE/NOTICE" "$OUT/NOTICE"
{
    echo "built:      $(date -u +%Y-%m-%dT%H:%M:%SZ) (informational; the module bytes do not depend on it)"
    echo "emcc:       $(emcc --version | head -1)"
    echo "emsdk:      ${EMSDK_USED:-${EMSDK:-$(dirname "$(dirname "$(command -v emcc)")")}}"
    echo "wasm3:      $(grep -m1 -o '28ecb9af[0-9a-f]*' "$WASM3/LICENSE" 2>/dev/null || echo 'see third_party/wasm3 (MIT, commit 6b8bcb1 (tag v0.5.0))')"
    echo "flags:      -std=c++17 -O3 -fno-exceptions -fno-rtti, standalone wasm (see scripts/build.sh)"
    echo "module.wasm sha256: $(shasum -a 256 "$OUT/module.wasm" | cut -d ' ' -f1)"
    echo "sources (sha256):"
    (cd "$HERE" && shasum -a 256 src/faust_runner_clap.cc src/mini_json.h | sed 's/^/  /')
} > "$OUT/BUILD-INFO.txt"

STAGE="$HERE/../.bundle-stage"
rm -rf "$STAGE"
mkdir -p "$STAGE/faust-runner.wclap"
cp "$OUT/module.wasm" "$HERE/LICENSE" "$HERE/NOTICE" "$STAGE/faust-runner.wclap/"
(cd "$STAGE" && tar czf "$ARCHIVE" faust-runner.wclap)
rm -rf "$STAGE"
echo "built $OUT/module.wasm ($(wc -c < "$OUT/module.wasm") bytes)"
echo "bundle packed to $ARCHIVE"
cat "$OUT/BUILD-INFO.txt"
