#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ORACLE_DIR="$PROJECT_ROOT/target/macos-oracle"
FIXTURE="$SCRIPT_DIR/probes/macos-libc.c"
TARGET="arm64-apple-macos11.0"
MODE=""

usage() {
    echo "usage: tools/probe-macos-sdk.sh --mode <predefined|header-macros|preprocess|ast|layouts|assembly|availability|symbols>"
}

while [[ $# -gt 0 ]]; do
    case "$1" in
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

if [[ -z "$MODE" ]]; then
    usage >&2
    exit 2
fi

if [[ -n "${SLATE_MACOS_SDK:-}" ]]; then
    SDK_ROOT="$(cd "$SLATE_MACOS_SDK" && pwd)"
elif command -v xcrun >/dev/null 2>&1; then
    SDK_ROOT="$(xcrun --sdk macosx --show-sdk-path)"
else
    echo "error: set SLATE_MACOS_SDK to an installed MacOSX.sdk" >&2
    exit 1
fi

if [[ ! -f "$SDK_ROOT/usr/include/stdio.h" ]]; then
    echo "error: macOS SDK has no usr/include/stdio.h: $SDK_ROOT" >&2
    exit 1
fi

CLANG="${SLATE_CLANG:-clang}"
LLVM_READOBJ="${SLATE_LLVM_READOBJ:-llvm-readobj}"
OUTPUT_DIR="$ORACLE_DIR/probes/aarch64/macos-11.0"
COMMON=(--target="$TARGET" -isysroot "$SDK_ROOT")

if ! command -v "$CLANG" >/dev/null 2>&1; then
    echo "error: clang is required for macOS SDK probes" >&2
    exit 1
fi

mkdir -p "$OUTPUT_DIR"

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
        "$CLANG" "${COMMON[@]}" -Wunguarded-availability -Xclang -ast-dump=json -fsyntax-only "$FIXTURE" > "$OUTPUT_DIR/availability.json"
        ;;
    symbols)
        if ! command -v "$LLVM_READOBJ" >/dev/null 2>&1; then
            echo "error: llvm-readobj is required for macOS symbol probes" >&2
            exit 1
        fi
        object="$OUTPUT_DIR/macos-libc.o"
        "$CLANG" "${COMMON[@]}" -c -O0 "$FIXTURE" -o "$object"
        "$LLVM_READOBJ" --symbols "$object" > "$OUTPUT_DIR/symbols.txt"
        ;;
    *)
        echo "error: unsupported macOS SDK probe mode: $MODE" >&2
        usage >&2
        exit 2
        ;;
esac

printf '{"architecture":"aarch64","deployment_target":"11.0","clang_target":"%s","sdk_root":"%s"}\n' \
    "$TARGET" "$SDK_ROOT" > "$OUTPUT_DIR/probe.json"
echo "macOS SDK $MODE probe: $OUTPUT_DIR"
