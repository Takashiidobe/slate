#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ORACLE_DIR="$PROJECT_ROOT/target/android-ndk-oracle"

ANDROID_NDK_ACCEPT_LICENSE=1 "$SCRIPT_DIR/bootstrap-android-ndk.sh"
ANDROID_NDK_ACCEPT_LICENSE=1 "$SCRIPT_DIR/bootstrap-android-ndk.sh"

test -f "$ORACLE_DIR/oracle.json"
grep -q '"ndk_revision": "27.3.13750724"' "$ORACLE_DIR/oracle.json"
grep -q '"api_level": 21' "$ORACLE_DIR/oracle.json"

if env -u ANDROID_NDK_ACCEPT_LICENSE -u SLATE_ANDROID_NDK "$SCRIPT_DIR/bootstrap-android-ndk.sh" >/dev/null 2>&1; then
    echo "bootstrap accepted an automatic download without license acceptance" >&2
    exit 1
fi
if "$SCRIPT_DIR/probe-android-ndk.sh" --arch riscv64 --api 21 --mode predefined >/dev/null 2>&1; then
    echo "probe accepted an unsupported architecture" >&2
    exit 1
fi
if "$SCRIPT_DIR/probe-android-ndk.sh" --arch aarch64 --api 20 --mode predefined >/dev/null 2>&1; then
    echo "probe accepted an API below the baseline" >&2
    exit 1
fi

for arch in aarch64 x86_64; do
    for mode in predefined header-macros preprocess ast layouts assembly availability symbols; do
        "$SCRIPT_DIR/probe-android-ndk.sh" --arch "$arch" --api 21 --mode "$mode"
    done
    output="$ORACLE_DIR/probes/$arch/api-21"
    test -s "$output/predefined.txt"
    grep -q '__ANDROID__' "$output/predefined.txt"
    grep -q '#define __ANDROID_API__ __ANDROID_MIN_SDK_VERSION__' "$output/predefined.txt"
    grep -q '#define __ANDROID_MIN_SDK_VERSION__ 21' "$output/predefined.txt"
    test -s "$output/header-macros.txt"
    test -s "$output/preprocess.i"
    test -s "$output/ast.json"
    test -s "$output/layouts.txt"
    test -s "$output/assembly.s"
    test -s "$output/availability.json"
    test -s "$output/symbols.txt"
    grep -q 'slate_android_export' "$output/symbols.txt"
done

echo "Android NDK oracle tests passed"
