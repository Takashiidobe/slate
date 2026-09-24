#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
target="${1:-}"
sdk_path="${2:-${OSX_CROSS_SDK:-}}"

case "$target" in
    x86_64-apple-darwin|aarch64-apple-darwin) ;;
    *) printf 'usage: %s <x86_64-apple-darwin|aarch64-apple-darwin> [sdk-path]\n' "$0" >&2; exit 2 ;;
esac

if [[ -z "$sdk_path" && "$(uname -s)" == Darwin ]]; then
    sdk_path="$(xcrun --sdk macosx --show-sdk-path)"
fi
if [[ -z "$sdk_path" ]]; then
    printf 'error: provide an SDK path from an OSXCross installation with OSX_CROSS_SDK or as argument\n' >&2
    exit 1
fi
if [[ ! -d "$sdk_path/usr/include" ]]; then
    printf 'error: not a macOS SDK root (missing usr/include): %s\n' "$sdk_path" >&2
    exit 1
fi

sdk_path="$(cd "$sdk_path" && pwd -P)"
output="$repo_root/sysroots/$target"
if [[ -e "$output" || -L "$output" ]]; then
    printf 'error: output already exists: %s\n' "$output" >&2
    exit 1
fi

mkdir -p "$output"
ln -s "$sdk_path" "$output/SDK"
cat > "$output/SYSROOT-MANIFEST.txt" <<EOF
Target: $target
Operating system: macOS
SDK path: $sdk_path
SDK version: $(basename "$sdk_path")
Source: user-provided Xcode, Command Line Tools, or OSXCross SDK
Contents: SDK referenced by symlink; not copied or distributed by Slate
EOF
printf 'Linked macOS SDK for %s at %s/SDK\n' "$target" "$output"
