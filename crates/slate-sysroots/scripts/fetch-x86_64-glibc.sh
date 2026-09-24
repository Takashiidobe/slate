#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
target_dir="$repo_root/sysroots/x86_64-unknown-linux-gnu"
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

mkdir -p "$target_dir"
fetch_and_extract \
    libc6-amd64-cross_2.36-8cross1_all.deb \
    e410f3d2da35bccf757976d38e6a309d4d94d25c0dfde565a9662e3f75951b2c
fetch_and_extract \
    libc6-dev-amd64-cross_2.36-8cross1_all.deb \
    197b497ed91056e6c4093b0972296199260424f24943d7108a8124cc7de750d6
fetch_and_extract \
    linux-libc-dev-amd64-cross_6.1.4-1cross1_all.deb \
    b7ec9f95d20bd4669119201ec6a1ac372189fc222d35f21631bb79b723519293

# Debian's cross packages use a triplet-prefixed layout; expose standard sysroot
# paths so Clang can consume the result with only --sysroot.
ln -sfn x86_64-linux-gnu/include "$target_dir/usr/include"
mkdir -p "$target_dir/usr/lib"
ln -sfn ../../x86_64-linux-gnu/lib "$target_dir/usr/lib/x86_64-linux-gnu"
mkdir -p "$target_dir/lib"
ln -sfn ../usr/x86_64-linux-gnu/lib "$target_dir/lib/x86_64-linux-gnu"
ln -sfn usr/x86_64-linux-gnu/lib64 "$target_dir/lib64"

cat > "$target_dir/SYSROOT-MANIFEST.txt" <<'EOF'
Target: x86_64-unknown-linux-gnu
Libc: glibc 2.36-8 (Debian cross-toolchain-base)
Linux UAPI headers: 6.1.4-1
Source: Debian Bookworm cross-toolchain-base packages
Package layout: Debian triplet-prefixed tree with standard sysroot aliases
Packages:
  libc6-amd64-cross 2.36-8cross1
  libc6-dev-amd64-cross 2.36-8cross1
  linux-libc-dev-amd64-cross 6.1.4-1cross1
EOF

printf 'Fetched sysroot files to %s\n' "$target_dir"
