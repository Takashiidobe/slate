#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
xwin_bin="${XWIN:-xwin}"
target="${1:-x86_64-pc-windows-msvc}"
case "$target" in
    x86_64-pc-windows-msvc) arch=x86_64 ;;
    i686-pc-windows-msvc) arch=x86 ;;
    aarch64-pc-windows-msvc) arch=aarch64 ;;
    *) printf 'error: unsupported MSVC target: %s\n' "$target" >&2; exit 2 ;;
esac

if ! command -v "$xwin_bin" >/dev/null 2>&1; then
    printf 'error: xwin is required; install it with `cargo install xwin --locked` or set XWIN\n' >&2
    exit 1
fi

output="$repo_root/sysroots/$target"
if [[ -e "$output" || -L "$output" ]]; then
    printf 'error: output already exists: %s\n' "$output" >&2
    exit 1
fi

mkdir -p "$repo_root/sysroots/.xwin-cache"
"$xwin_bin" --arch "$arch" \
    --cache-dir "$repo_root/sysroots/.xwin-cache" \
    splat --output "$output"

cat > "$output/SYSROOT-MANIFEST.txt" <<EOF
Target: $target
Operating system: Windows
ABI: MSVC
Source: Microsoft CRT and Windows SDK, assembled by xwin
License acceptance: handled interactively by xwin
Contents: headers and libraries; local use only, not a Slate-distributed artifact
EOF
printf 'Prepared local MSVC SDK and CRT at %s\n' "$output"
