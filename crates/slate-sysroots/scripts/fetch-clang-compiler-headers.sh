#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
    printf 'usage: %s <llvm-version>\n' "$0" >&2
    exit 2
fi

version="$1"
if [[ ! "$version" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    printf 'error: expected a numeric LLVM release version, got: %s\n' "$version" >&2
    exit 2
fi

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="$repo_root/compiler-headers/clang-$version"
if [[ -e "$output" ]]; then
    printf 'error: output already exists: %s\n' "$output" >&2
    exit 1
fi

work_dir="$(mktemp -d)"
trap 'rm -rf "$work_dir"' EXIT
source_url="https://github.com/llvm/llvm-project.git"
git clone --quiet --depth 1 --filter=blob:none --sparse --branch "llvmorg-$version" \
    "$source_url" "$work_dir/llvm-project"
git -C "$work_dir/llvm-project" sparse-checkout set --no-cone \
    '/clang/lib/Headers/' '/LICENSE.TXT' '/NOTICE.TXT'
mkdir -p "$output/include"
cp -a "$work_dir/llvm-project/clang/lib/Headers/." "$output/include/"
cp "$work_dir/llvm-project/LICENSE.TXT" "$output/"
if [[ -f "$work_dir/llvm-project/NOTICE.TXT" ]]; then
    cp "$work_dir/llvm-project/NOTICE.TXT" "$output/"
fi
cat > "$output/COMPILER-HEADERS-MANIFEST.txt" <<EOF
Compiler: upstream Clang
Version: $version
Source repository: $source_url
Source tag: llvmorg-$version
Source path: clang/lib/Headers
EOF
printf 'Fetched Clang %s resource headers to %s\n' "$version" "$output"
