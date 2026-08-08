#!/usr/bin/env bash
set -euo pipefail

XWIN_VERSION="0.9.0"
XWIN_MANIFEST_VERSION="17.14.37516.0"
XWIN_MANIFEST_SHA256="0d58265796664d7b69bd7b00498feea9efe0742a565dec42f3408bc3170588e9"
XWIN_MANIFEST_URL="https://download.visualstudio.microsoft.com/download/pr/f7f5ecbc-83ca-4cf0-bdb2-aaf70efb6d97/0d58265796664d7b69bd7b00498feea9efe0742a565dec42f3408bc3170588e9/VisualStudio.vsman"
CRT_VERSION="14.44.17.14"
SDK_VERSION="10.0.26100"
ARCH="x86_64"
TARGET_TRIPLE="x86_64-pc-windows-msvc"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
TOOLS_DIR="$PROJECT_ROOT/target/msvc-tools"
CARGO_HOME_DIR="$TOOLS_DIR/cargo-home"
XWIN_BIN="$TOOLS_DIR/bin/xwin"
CACHE_DIR="$PROJECT_ROOT/target/msvc-xwin-cache"
SYSROOT_DIR="$PROJECT_ROOT/target/msvc-sysroot"
MANIFEST_PATH="$CACHE_DIR/slate-manifest-$XWIN_MANIFEST_VERSION.json"
VERSION_STAMP="xwin=$XWIN_VERSION manifest=$XWIN_MANIFEST_VERSION crt=$CRT_VERSION sdk=$SDK_VERSION arch=$ARCH"
ACCEPT_LICENSE=0

usage() {
    echo "usage: tools/bootstrap-msvc-sysroot.sh --accept-license"
    echo "       XWIN_ACCEPT_LICENSE=1 tools/bootstrap-msvc-sysroot.sh"
}

for arg in "$@"; do
    case "$arg" in
        --accept-license)
            ACCEPT_LICENSE=1
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "error: unknown argument: $arg" >&2
            usage >&2
            exit 2
            ;;
    esac
done

if [[ "${XWIN_ACCEPT_LICENSE:-}" == "1" ]]; then
    ACCEPT_LICENSE=1
fi

if [[ "$ACCEPT_LICENSE" != "1" ]]; then
    echo "error: Microsoft license acceptance is required" >&2
    echo "review https://go.microsoft.com/fwlink/?LinkId=2086102, then rerun with --accept-license or XWIN_ACCEPT_LICENSE=1" >&2
    exit 2
fi

if ! command -v cargo >/dev/null 2>&1; then
    echo "error: cargo is required to install pinned xwin $XWIN_VERSION" >&2
    echo "install Rust and Cargo from https://rustup.rs, then rerun this script" >&2
    exit 1
fi

mkdir -p "$CARGO_HOME_DIR" "$CACHE_DIR"

if [[ ! -x "$XWIN_BIN" ]] || [[ "$($XWIN_BIN --version 2>/dev/null || true)" != "xwin $XWIN_VERSION" ]]; then
    if ! CARGO_HOME="$CARGO_HOME_DIR" cargo install xwin --version "=$XWIN_VERSION" --locked --root "$TOOLS_DIR" --force; then
        echo "error: failed to install pinned xwin $XWIN_VERSION under $TOOLS_DIR" >&2
        echo "check Cargo's network access and compiler setup, then rerun this script" >&2
        exit 1
    fi
fi

printf '{"channelItems":[{"id":"Microsoft.VisualStudio.Manifests.VisualStudio","version":"%s","type":"Manifest","payloads":[{"fileName":"VisualStudio.vsman","sha256":"%s","size":30444099,"url":"%s"}]}]}\n' \
    "$XWIN_MANIFEST_VERSION" "$XWIN_MANIFEST_SHA256" "$XWIN_MANIFEST_URL" > "$MANIFEST_PATH"

STAMP_PATH="$SYSROOT_DIR/.slate-version"
CURRENT_STAMP="$(sed -n '1p' "$STAMP_PATH" 2>/dev/null || true)"

if [[ "$CURRENT_STAMP" != "$VERSION_STAMP" ]] || [[ ! -d "$SYSROOT_DIR/crt/include" ]] || [[ ! -d "$SYSROOT_DIR/sdk/include/ucrt" ]]; then
    STAGING_DIR="$PROJECT_ROOT/target/msvc-sysroot.tmp.$$"
    OLD_DIR="$PROJECT_ROOT/target/msvc-sysroot.old.$$"
    trap 'rm -rf "$STAGING_DIR" "$OLD_DIR"' EXIT
    rm -rf "$STAGING_DIR" "$OLD_DIR"

    if ! "$XWIN_BIN" \
        --accept-license \
        --manifest "$MANIFEST_PATH" \
        --crt-version "$CRT_VERSION" \
        --sdk-version "$SDK_VERSION" \
        --arch "$ARCH" \
        --variant desktop \
        --cache-dir "$CACHE_DIR" \
        splat --output "$STAGING_DIR"; then
        echo "error: xwin failed to materialize the pinned MSVC reference sysroot" >&2
        echo "check network access and the xwin/manifest/CRT/SDK pins near the top of this script" >&2
        exit 1
    fi

    rm -rf \
        "$STAGING_DIR/crt/lib" \
        "$STAGING_DIR/sdk/lib" \
        "$STAGING_DIR/sdk/include/um" \
        "$STAGING_DIR/sdk/include/winrt" \
        "$STAGING_DIR/sdk/include/cppwinrt"
    printf '%s\n' "$VERSION_STAMP" > "$STAGING_DIR/.slate-version"

    if [[ -e "$SYSROOT_DIR" ]]; then
        mv "$SYSROOT_DIR" "$OLD_DIR"
    fi
    mv "$STAGING_DIR" "$SYSROOT_DIR"
    rm -rf "$OLD_DIR"
    trap - EXIT
fi

CRT_INCLUDE="$SYSROOT_DIR/crt/include"
UCRT_INCLUDE="$SYSROOT_DIR/sdk/include/ucrt"
INCLUDE_ROOTS="$SYSROOT_DIR/include-roots.json"
printf '{"target":"%s","crt":"%s","ucrt":"%s"}\n' \
    "$TARGET_TRIPLE" "$CRT_INCLUDE" "$UCRT_INCLUDE" > "$INCLUDE_ROOTS"

CLANG_BIN="${SLATE_CLANG:-${HOME}/llvm-project/build-cir/bin/clang}"
if [[ ! -x "$CLANG_BIN" ]]; then
    echo "error: CIR-enabled Clang not found or not executable: $CLANG_BIN" >&2
    echo "set SLATE_CLANG to the configured CIR-enabled clang and rerun this script" >&2
    exit 1
fi

PROBE="$(mktemp "$PROJECT_ROOT/target/msvc-header-probe.XXXXXX.c")"
trap 'rm -f "$PROBE"' EXIT
printf '#include <vcruntime.h>\n#include <stdio.h>\nint probe(void) { return EOF; }\n' > "$PROBE"

if ! "$CLANG_BIN" \
    --target="$TARGET_TRIPLE" \
    -fclangir \
    -fsyntax-only \
    -nostdlibinc \
    -isystem "$CRT_INCLUDE" \
    -isystem "$UCRT_INCLUDE" \
    "$PROBE"; then
    echo "error: CIR Clang could not parse representative pinned CRT and UCRT headers" >&2
    echo "clang: $CLANG_BIN" >&2
    echo "include roots: $INCLUDE_ROOTS" >&2
    exit 1
fi

rm -f "$PROBE"
trap - EXIT
echo "MSVC reference sysroot ready: $SYSROOT_DIR"
echo "include roots: $INCLUDE_ROOTS"
