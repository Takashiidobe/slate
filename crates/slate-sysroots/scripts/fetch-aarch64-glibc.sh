#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
target_dir="$repo_root/sysroots/aarch64-unknown-linux-gnu"
work_dir="$(mktemp -d)"
trap 'rm -rf "$work_dir"' EXIT
base_url="https://deb.debian.org/debian/pool/main/c/cross-toolchain-base"

fetch_and_extract() {
    local filename="$1"
    local sha256="$2"
    local deb="$work_dir/$filename"

    curl --fail --location --silent --show-error "$base_url/$filename" -o "$deb"
    echo "$sha256  $deb" | sha256sum --check --status
    mkdir -p "$work_dir/extract"
    (
        cd "$work_dir/extract"
        ar x "$deb" data.tar.xz
        tar -xJf data.tar.xz -C "$target_dir"
        rm data.tar.xz
    )
}

if [[ -d "$target_dir" ]]; then
    chmod -R u+w "$target_dir"
    rm -rf "$target_dir"
fi
mkdir -p "$target_dir"
fetch_and_extract \
    libc6-arm64-cross_2.36-8cross1_all.deb \
    91936cbbee75771c360ed16513f6e734a25ca5517fe7c3e13dfd7835d7553186
fetch_and_extract \
    libc6-dev-arm64-cross_2.36-8cross1_all.deb \
    5e4cf6abf0e89e89c4b3006bb8b7dc47aa831df25d94df9134929b8b968cacb4
fetch_and_extract \
    linux-libc-dev-arm64-cross_6.1.4-1cross1_all.deb \
    66457f015d16b7d372db6591cff38db0da928362ff2ea1fa75491e28f6904d81

# Make Debian's absolute linker-script paths resolve through this sysroot.
sed -i 's#/usr/aarch64-linux-gnu/lib/##g' \
    "$target_dir/usr/aarch64-linux-gnu/lib/libc.so"

ln -sfn aarch64-linux-gnu/include "$target_dir/usr/include"
mkdir -p "$target_dir/usr/lib" "$target_dir/lib"
ln -sfn ../aarch64-linux-gnu/lib "$target_dir/usr/lib/aarch64-linux-gnu"
ln -sfn ../usr/aarch64-linux-gnu/lib "$target_dir/lib/aarch64-linux-gnu"
ln -sfn usr/aarch64-linux-gnu/lib64 "$target_dir/lib64"
ln -sfn ../usr/aarch64-linux-gnu/lib/ld-linux-aarch64.so.1 "$target_dir/lib/ld-linux-aarch64.so.1"

cat > "$target_dir/SYSROOT-MANIFEST.txt" <<'EOF'
Target: aarch64-unknown-linux-gnu
Libc: glibc 2.36-8 (Debian cross-toolchain-base)
Linux UAPI headers: 6.1.4-1
Source: Debian Bookworm cross-toolchain-base packages
Packages:
  libc6-arm64-cross 2.36-8cross1
  libc6-dev-arm64-cross 2.36-8cross1
  linux-libc-dev-arm64-cross 6.1.4-1cross1
Package layout: Debian triplet-prefixed tree with standard sysroot aliases
EOF

printf 'Fetched sysroot files to %s\n' "$target_dir"
