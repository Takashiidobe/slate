#!/usr/bin/env bash
set -euo pipefail

if [[ $# -gt 1 ]]; then
    printf 'usage: %s [apple-clang-version]\n' "$0" >&2
    exit 2
fi
if [[ "$(uname -s)" != Darwin ]]; then
    printf 'error: Apple Clang headers must be acquired on macOS from Xcode or Command Line Tools\n' >&2
    exit 1
fi

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
clang_bin="$(xcrun --find clang)"
resource_dir="$("$clang_bin" -print-resource-dir)"
clang_version="$("$clang_bin" --version | sed -n 's/.*Apple clang version \([^ ]*\).*/\1/p' | head -n 1)"
build_version="$("$clang_bin" --version | sed -n 's/.*(clang-\([^)]*\)).*/\1/p' | head -n 1)"
version="${1:-$clang_version${build_version:+-clang-$build_version}}"
if [[ -z "$version" || ! -d "$resource_dir/include" ]]; then
    printf 'error: could not determine Apple Clang version or resource headers from %s\n' "$clang_bin" >&2
    exit 1
fi
if [[ ! "$version" =~ ^[A-Za-z0-9._+-]+$ ]]; then
    printf 'error: invalid Apple Clang version label: %s\n' "$version" >&2
    exit 2
fi

output="$repo_root/compiler-headers/apple-clang-$version"
if [[ -e "$output" ]]; then
    printf 'error: output already exists: %s\n' "$output" >&2
    exit 1
fi
mkdir -p "$output"
cp -a "$resource_dir/include" "$output/"
cat > "$output/COMPILER-HEADERS-MANIFEST.txt" <<EOF
Compiler: Apple Clang
Version: $version
Compiler path: $clang_bin
Resource directory: $resource_dir
Acquisition: copied from locally installed Xcode or Command Line Tools
Distribution: local use only; Apple toolchain license applies
EOF
printf 'Copied Apple Clang %s resource headers to %s\n' "$version" "$output"
