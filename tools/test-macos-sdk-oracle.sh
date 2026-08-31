#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
OUTPUT="$PROJECT_ROOT/target/macos-oracle/probes/aarch64/macos-11.0"

if [[ -z "${SLATE_MACOS_SDK:-}" ]] && ! command -v xcrun >/dev/null 2>&1; then
    echo "SKIP macOS SDK oracle: set SLATE_MACOS_SDK to an installed MacOSX.sdk"
    exit 0
fi

for mode in predefined header-macros preprocess ast layouts assembly availability symbols; do
    "$SCRIPT_DIR/probe-macos-sdk.sh" --mode "$mode"
done

test -s "$OUTPUT/predefined.txt"
grep -q '#define __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ 110000' "$OUTPUT/predefined.txt"
test -s "$OUTPUT/header-macros.txt"
test -s "$OUTPUT/preprocess.i"
test -s "$OUTPUT/ast.json"
test -s "$OUTPUT/layouts.txt"
test -s "$OUTPUT/assembly.s"
test -s "$OUTPUT/availability.json"
test -s "$OUTPUT/symbols.txt"
grep -q 'slate_macos_export' "$OUTPUT/symbols.txt"
grep -q 'slate_macos_clock' "$OUTPUT/symbols.txt"
grep -q 'slate_macos_sigmask' "$OUTPUT/symbols.txt"
test -s "$OUTPUT/probe.json"

echo "macOS SDK oracle tests passed"
