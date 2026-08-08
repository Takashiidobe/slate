#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ORACLE_DIR="$PROJECT_ROOT/target/android-ndk-oracle"
FIXTURE="$SCRIPT_DIR/probes/android-libc.c"
ARCH=""
API_LEVEL=""
MODE=""

usage() {
    echo "usage: tools/probe-android-ndk.sh --arch <aarch64|x86_64> --api <level> --mode <predefined|header-macros|preprocess|ast|layouts|assembly|availability|symbols>"
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --arch)
            ARCH="${2:-}"
            shift 2
            ;;
        --api)
            API_LEVEL="${2:-}"
            shift 2
            ;;
        --mode)
            MODE="${2:-}"
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "error: unknown argument: $1" >&2
            usage >&2
            exit 2
            ;;
    esac
done

if [[ -z "$ARCH" || -z "$API_LEVEL" || -z "$MODE" ]]; then
    usage >&2
    exit 2
fi
if [[ ! "$API_LEVEL" =~ ^[0-9]+$ ]] || ((API_LEVEL < 21)); then
    echo "error: API level must be an integer at least 21" >&2
    exit 2
fi

case "$ARCH" in
    aarch64)
        TARGET="aarch64-linux-android$API_LEVEL"
        LIBRARY_TRIPLE="aarch64-linux-android"
        ;;
    x86_64)
        TARGET="x86_64-linux-android$API_LEVEL"
        LIBRARY_TRIPLE="x86_64-linux-android"
        ;;
    *)
        echo "error: unsupported Android oracle architecture: $ARCH" >&2
        exit 2
        ;;
esac

if [[ -n "${SLATE_ANDROID_NDK:-}" ]]; then
    NDK_ROOT="$(cd "$SLATE_ANDROID_NDK" && pwd)"
elif [[ -f "$ORACLE_DIR/ndk-root.txt" ]]; then
    NDK_ROOT="$(sed -n '1p' "$ORACLE_DIR/ndk-root.txt")"
else
    echo "error: run tools/bootstrap-android-ndk.sh first or set SLATE_ANDROID_NDK" >&2
    exit 1
fi

HOST_TAG="$(find "$NDK_ROOT/toolchains/llvm/prebuilt" -mindepth 1 -maxdepth 1 -type d -exec basename {} \; | head -1)"
TOOLCHAIN="$NDK_ROOT/toolchains/llvm/prebuilt/$HOST_TAG"
SYSROOT="$TOOLCHAIN/sysroot"
CLANG="$TOOLCHAIN/bin/clang"
LLVM_READOBJ="$TOOLCHAIN/bin/llvm-readobj"
LLVM_READELF="$TOOLCHAIN/bin/llvm-readelf"
LIBRARY_ROOT="$SYSROOT/usr/lib/$LIBRARY_TRIPLE/$API_LEVEL"
OUTPUT_DIR="$ORACLE_DIR/probes/$ARCH/api-$API_LEVEL"

if [[ ! -x "$CLANG" || ! -d "$LIBRARY_ROOT" ]]; then
    echo "error: NDK does not provide $TARGET under $NDK_ROOT" >&2
    exit 1
fi

mkdir -p "$OUTPUT_DIR"
COMMON=(--target="$TARGET" --sysroot="$SYSROOT")

case "$MODE" in
    predefined)
        "$CLANG" "${COMMON[@]}" -dM -E -x c /dev/null > "$OUTPUT_DIR/predefined.txt"
        ;;
    header-macros)
        "$CLANG" "${COMMON[@]}" -dM -E "$FIXTURE" > "$OUTPUT_DIR/header-macros.txt"
        ;;
    preprocess)
        "$CLANG" "${COMMON[@]}" -E -P "$FIXTURE" > "$OUTPUT_DIR/preprocess.i"
        ;;
    ast)
        "$CLANG" "${COMMON[@]}" -Xclang -ast-dump=json -fsyntax-only "$FIXTURE" > "$OUTPUT_DIR/ast.json"
        ;;
    layouts)
        "$CLANG" "${COMMON[@]}" -Xclang -fdump-record-layouts-complete -fsyntax-only "$FIXTURE" > "$OUTPUT_DIR/layouts.txt" 2>&1
        ;;
    assembly)
        "$CLANG" "${COMMON[@]}" -S -O0 "$FIXTURE" -o "$OUTPUT_DIR/assembly.s"
        ;;
    availability)
        "$CLANG" "${COMMON[@]}" -D_FORTIFY_SOURCE=2 -O2 -Xclang -ast-dump=json -fsyntax-only "$FIXTURE" > "$OUTPUT_DIR/availability.json"
        ;;
    symbols)
        if [[ ! -x "$LLVM_READOBJ" || ! -x "$LLVM_READELF" ]]; then
            echo "error: NDK llvm-readobj and llvm-readelf are required for symbol probes" >&2
            exit 1
        fi
        object="$OUTPUT_DIR/android-libc.o"
        "$CLANG" "${COMMON[@]}" -c -O0 "$FIXTURE" -o "$object"
        {
            "$LLVM_READOBJ" --symbols "$object"
            "$LLVM_READELF" --dyn-symbols "$LIBRARY_ROOT/libc.so"
        } > "$OUTPUT_DIR/symbols.txt"
        ;;
    *)
        echo "error: unsupported Android NDK probe mode: $MODE" >&2
        usage >&2
        exit 2
        ;;
esac

printf '{"architecture":"%s","api_level":%s,"clang_target":"%s","library_triple":"%s","ndk_root":"%s"}\n' \
    "$ARCH" "$API_LEVEL" "$TARGET" "$LIBRARY_TRIPLE" "$NDK_ROOT" > "$OUTPUT_DIR/probe.json"
echo "Android NDK $MODE probe: $OUTPUT_DIR"
