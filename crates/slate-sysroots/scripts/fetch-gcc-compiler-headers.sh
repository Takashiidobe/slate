#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
    printf 'usage: %s <gcc-version>\n' "$0" >&2
    exit 2
fi

version="$1"
if [[ ! "$version" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    printf 'error: expected a numeric GCC release version, got: %s\n' "$version" >&2
    exit 2
fi

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="$repo_root/compiler-headers/gcc-$version"
if [[ -e "$output" ]]; then
    printf 'error: output already exists: %s\n' "$output" >&2
    exit 1
fi

work_dir="$(mktemp -d)"
trap 'rm -rf "$work_dir"' EXIT
archive="$work_dir/gcc.tar.xz"
source_url="https://ftp.gnu.org/gnu/gcc/gcc-$version/gcc-$version.tar.xz"
curl --fail --location --silent --show-error "$source_url" -o "$archive"
tar -xJf "$archive" --directory "$work_dir" \
    "gcc-$version/gcc/ginclude" \
    "gcc-$version/COPYING3" \
    "gcc-$version/COPYING.RUNTIME"
mkdir -p "$output/include"
cp -a "$work_dir/gcc-$version/gcc/ginclude/." "$output/include/"
cp "$work_dir/gcc-$version/COPYING3" "$output/"
cp "$work_dir/gcc-$version/COPYING.RUNTIME" "$output/"
cat > "$output/COMPILER-HEADERS-MANIFEST.txt" <<EOF
Compiler: GCC
Version: $version
Source URL: $source_url
Source path: gcc/ginclude
Scope: generic compiler headers only; target-specific and generated headers omitted
EOF
printf 'Fetched GCC %s generic headers to %s\n' "$version" "$output"
