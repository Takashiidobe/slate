#!/usr/bin/env bash
set -euo pipefail

FREEBSD_RELEASE="15.1-RELEASE"
FREEBSD_VERSION="15.1"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ORACLE_DIR="$PROJECT_ROOT/target/freebsd-oracle"
FIXTURE="$SCRIPT_DIR/probes/freebsd-libc.c"
ARCH=""
MODE=""

usage() {
    echo "usage: tools/probe-freebsd-libc.sh --arch <aarch64|x86_64> --mode <predefined|header-macros|preprocess|ast|layouts|assembly|availability|symbols>"
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --arch)
            ARCH="${2:-}"
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

if [[ -z "$ARCH" || -z "$MODE" ]]; then
    usage >&2
    exit 2
fi

case "$ARCH" in
    aarch64)
        TARGET="aarch64-unknown-freebsd$FREEBSD_VERSION"
        DISTRIBUTION_ARCH="arm64"
        SYSROOT="${SLATE_FREEBSD_ARM64_SYSROOT:-$ORACLE_DIR/sysroots/arm64}"
        ;;
    x86_64)
        TARGET="x86_64-unknown-freebsd$FREEBSD_VERSION"
        DISTRIBUTION_ARCH="amd64"
        SYSROOT="${SLATE_FREEBSD_AMD64_SYSROOT:-$ORACLE_DIR/sysroots/amd64}"
        ;;
    *)
        echo "error: unsupported FreeBSD oracle architecture: $ARCH" >&2
        exit 2
        ;;
esac

CLANG="${SLATE_FREEBSD_CLANG:-clang}"
LLVM_READOBJ="${SLATE_FREEBSD_LLVM_READOBJ:-llvm-readobj}"
LLVM_READELF="${SLATE_FREEBSD_LLVM_READELF:-llvm-readelf}"
OUTPUT_DIR="$ORACLE_DIR/probes/$ARCH/$FREEBSD_RELEASE"

if ! command -v "$CLANG" >/dev/null 2>&1; then
    echo "error: clang is required for FreeBSD libc probes" >&2
    exit 1
fi
if [[ ! -f "$SYSROOT/usr/include/stdio.h" || ! -f "$SYSROOT/lib/libc.so.7" ]]; then
    echo "error: run tools/bootstrap-freebsd-oracle.sh first or set the matching SLATE_FREEBSD_*_SYSROOT" >&2
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
        if ! command -v "$LLVM_READOBJ" >/dev/null 2>&1 || ! command -v "$LLVM_READELF" >/dev/null 2>&1; then
            echo "error: llvm-readobj and llvm-readelf are required for symbol probes" >&2
            exit 1
        fi
        object="$OUTPUT_DIR/freebsd-libc.o"
        "$CLANG" "${COMMON[@]}" -c -O0 "$FIXTURE" -o "$object"
        {
            "$LLVM_READOBJ" --symbols "$object"
            "$LLVM_READELF" --dyn-symbols "$SYSROOT/lib/libc.so.7"
        } > "$OUTPUT_DIR/symbols.txt"
        ;;
    *)
        echo "error: unsupported FreeBSD libc probe mode: $MODE" >&2
        usage >&2
        exit 2
        ;;
esac

printf '{"architecture":"%s","distribution_architecture":"%s","freebsd_release":"%s","clang_target":"%s","sysroot":"%s"}\n' \
    "$ARCH" "$DISTRIBUTION_ARCH" "$FREEBSD_RELEASE" "$TARGET" "$SYSROOT" > "$OUTPUT_DIR/probe.json"
echo "FreeBSD libc $MODE probe: $OUTPUT_DIR"
