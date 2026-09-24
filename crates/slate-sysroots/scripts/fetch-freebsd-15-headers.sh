#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
    printf 'usage: %s amd64|aarch64\n' "$0" >&2
    exit 2
fi

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
work_dir="$(mktemp -d)"
trap 'rm -rf "$work_dir"' EXIT

case "$1" in
    amd64)
        target="x86_64-unknown-freebsd"
        release_arch="amd64/amd64"
        archive_sha256="3768988b151c20f965679062b065c63a977d6bbb9f47fd83695ec2c40790c18f"
        ;;
    aarch64)
        target="aarch64-unknown-freebsd"
        release_arch="arm64/aarch64"
        archive_sha256="5b7a46a0abfbe23a1d4454b5600e2efcfce16b705bc7e9c851c37470c035ef98"
        ;;
    *)
        printf 'error: unsupported FreeBSD architecture: %s\n' "$1" >&2
        exit 2
        ;;
esac

target_dir="$repo_root/sysroots/$target"
archive="$work_dir/base.txz"
archive_url="https://download.freebsd.org/releases/$release_arch/15.1-RELEASE/base.txz"

curl --fail --location --silent --show-error "$archive_url" -o "$archive"
echo "$archive_sha256  $archive" | sha256sum --check --status

if [[ -d "$target_dir" ]]; then
    chmod -R u+w "$target_dir"
    rm -rf "$target_dir"
fi
mkdir -p "$target_dir"
tar -xJf "$archive" \
    --directory "$target_dir" \
    --no-same-owner \
    --no-same-permissions \
    ./COPYRIGHT ./usr/include

cat > "$target_dir/SYSROOT-MANIFEST.txt" <<EOF
Target: $target
Operating system: FreeBSD 15.1-RELEASE
Contents: headers only, from the FreeBSD base set
Source URL: $archive_url
Source SHA-256: $archive_sha256
EOF

printf 'Fetched FreeBSD 15.1 headers to %s\n' "$target_dir"
