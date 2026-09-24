#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 2 ]]; then
    printf 'usage: %s <msvc-version-label> <target>\n' "$0" >&2
    exit 2
fi

version="$1"
target="$2"
case "$target" in
    x86_64-pc-windows-msvc|i686-pc-windows-msvc|aarch64-pc-windows-msvc) ;;
    *) printf 'error: unsupported MSVC target: %s\n' "$target" >&2; exit 2 ;;
esac
if [[ ! "$version" =~ ^[A-Za-z0-9._+-]+$ ]]; then
    printf 'error: invalid MSVC version label: %s\n' "$version" >&2
    exit 2
fi

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
sysroot="$repo_root/sysroots/$target"
if [[ ! -d "$sysroot/crt/include" ]]; then
    "$repo_root/scripts/fetch-windows-msvc-sysroot.sh" "$target"
fi

output="$repo_root/compiler-headers/msvc-$version/$target"
if [[ -e "$output" ]]; then
    printf 'error: output already exists: %s\n' "$output" >&2
    exit 1
fi
mkdir -p "$output"
cp -a "$sysroot/crt/include" "$output/"
cat > "$output/COMPILER-HEADERS-MANIFEST.txt" <<EOF
Compiler: Microsoft Visual C++
Version label: $version
Target: $target
Source: Microsoft CRT compiler-support headers acquired with xwin
Source sysroot: $sysroot
License: Microsoft license applies; do not redistribute this bundle
EOF
printf 'Copied MSVC CRT compiler-support headers to %s\n' "$output"
