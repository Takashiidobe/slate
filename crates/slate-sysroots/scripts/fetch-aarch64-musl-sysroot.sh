#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
target_dir="$repo_root/sysroots/aarch64-unknown-linux-musl"
work_dir="$(mktemp -d)"
trap 'rm -rf "$work_dir"' EXIT

archive="$work_dir/aarch64-unknown-linux-musl.tar.xz"
archive_url="https://github.com/cross-tools/musl-cross/releases/download/20260430/aarch64-unknown-linux-musl.tar.xz"
archive_sha256="9303385fc29f8197004f641f96382196d56e4ec6dd975bff8ddd66641378628d"

curl --fail --location --silent --show-error "$archive_url" -o "$archive"
echo "$archive_sha256  $archive" | sha256sum --check --status

if [[ -d "$target_dir" ]]; then
    chmod -R u+w "$target_dir"
    rm -rf "$target_dir"
fi
mkdir -p "$target_dir"
tar -xJf "$archive" \
    --directory "$target_dir" \
    --strip-components=3 \
    --no-same-owner \
    --no-same-permissions \
    aarch64-unknown-linux-musl/aarch64-unknown-linux-musl/sysroot

chmod -R u+w "$target_dir/lib"
find "$target_dir/lib" -mindepth 1 ! -name ld-musl-aarch64.so.1 -exec rm -rf -- {} +
tar -xJf "$archive" \
    --directory "$target_dir" \
    --strip-components=2 \
    --no-same-owner \
    --no-same-permissions \
    aarch64-unknown-linux-musl/share/licenses/musl/COPYRIGHT \
    aarch64-unknown-linux-musl/share/licenses/linux/COPYING

cat > "$target_dir/SYSROOT-MANIFEST.txt" <<'EOF'
Target: aarch64-unknown-linux-musl
Libc: musl (cross-tools/musl-cross release 20260430)
Source archive: aarch64-unknown-linux-musl.tar.xz
Source URL: https://github.com/cross-tools/musl-cross/releases/download/20260430/aarch64-unknown-linux-musl.tar.xz
Source SHA-256: 9303385fc29f8197004f641f96382196d56e4ec6dd975bff8ddd66641378628d
Extracted contents: libc headers, libraries, startup files, loader, and license notices; compiler, binutils, and GCC runtime omitted
EOF

printf 'Fetched sysroot files to %s\n' "$target_dir"
