#!/usr/bin/env bash
# build-ports.sh — build open-webclap ports and install them into bundle-host/ (our
# self-hosted /wclap route), so the /wclap page shows them "installed" and can load them.
#
# Usage:
#   tools/build-ports.sh                  # every port that has scripts/build-<port>.sh in open-webclap
#   tools/build-ports.sh pearls wet-chorus # just these
#   tools/build-ports.sh --force pearls   # rebuild even if build/<port>/*.wclap.tar.gz exists
#
# Toolchain (the port scripts auto-source ~/emsdk/emsdk_env.sh when em++ is not on PATH):
#   emsdk (tested 3.1.51), cmake, ninja, python3 — all present on this machine.
#   Extra per port: faust-aether/faust-elements need the Faust compiler (~/faust or FAUST=...);
#   faust-elements also needs Rust/cargo; sc-dark needs cargo + wasm32-wasip1 + rust-docs.
#   scripts/fetch-deps.py downloads the pinned upstream sources from GitHub on first run.
#
# Output: open-webclap/build/<port>/<port>.wclap.tar.gz, then sync-bundle-host.mjs unpacks
# it into bundle-host/<port>/ — served at /wclap/<port>/<file> by tools/serve.mjs.
set -euo pipefail

REPO="$(cd "$(dirname "$0")/.." && pwd)"
OW="${OPEN_WEBCLAP_ROOT:-$(cd "$REPO/.." && pwd)/open-webclap}"
# cargo/rustc must be the rustup shims (~/.cargo/bin), not Homebrew's rust: only the rustup
# toolchain has the wasm32-wasip1 std (sc-dark, faust-elements). `rustup target add wasm32-wasip1`.
if [ -x "$HOME/.cargo/bin/cargo" ]; then export PATH="$HOME/.cargo/bin:$PATH"; fi
FORCE=0
PORTS=()
for a in "$@"; do
  case "$a" in
    --force) FORCE=1 ;;
    *) PORTS+=("$a") ;;
  esac
done

if [ ${#PORTS[@]} -eq 0 ]; then
  for s in "$OW"/scripts/build-*.sh; do
    PORTS+=("$(basename "$s" | sed 's/^build-//; s/\.sh$//')")
  done
fi

FAILED=()
for port in "${PORTS[@]}"; do
  script="$OW/scripts/build-$port.sh"
  # the archive name is not always the port id (sc-dark packs polarity-sc-dark.wclap.tar.gz)
  tar="$(ls "$OW"/build/$port/*.wclap.tar.gz 2>/dev/null | head -1)"
  if [ ! -f "$script" ]; then echo "SKIP $port (no $script)"; continue; fi
  if [ -n "$tar" ] && [ "$FORCE" -eq 0 ]; then echo "SKIP $port (already built: $tar — use --force)"; continue; fi
  echo "=== BUILD $port ($script) — log: $REPO/build-$port.log"
  # one port failing must not abort the rest (set -e would stop before the sync)
  if ( cd "$OW" && bash "scripts/build-$port.sh" ) > "$REPO/build-$port.log" 2>&1; then
    tar="$(ls "$OW"/build/$port/*.wclap.tar.gz 2>/dev/null | head -1)"
    if [ -n "$tar" ]; then
      echo "OK   $port -> $tar"
      # sync after EACH port, so bundle-host (and the running server) pick it up immediately
      node "$REPO/tools/sync-bundle-host.mjs" > /dev/null && echo "     synced into bundle-host"
    else
      echo "FAIL $port (script exited 0 but no archive in build/$port/)"; FAILED+=("$port")
    fi
  else
    echo "FAIL $port — last lines of build-$port.log:"; tail -5 "$REPO/build-$port.log" || true
    FAILED+=("$port")
  fi
done

echo "=== failed ports: ${FAILED[*]:-none}"
echo "=== final sync"
node "$REPO/tools/sync-bundle-host.mjs"
echo "=== installed now:"
curl -s http://127.0.0.1:8137/wclap/engines | python3 -c "import json,sys; print(', '.join(e['engine'] for e in json.load(sys.stdin)['engines']))" 2>/dev/null || true
