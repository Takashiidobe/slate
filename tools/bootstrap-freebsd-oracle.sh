#!/usr/bin/env bash
set -euo pipefail

FREEBSD_RELEASE="15.1-RELEASE"
FREEBSD_VERSION="15.1"
FREEBSD_VERSION_NUMBER="1501000"
SOURCE_TAG="release/15.1.0"
SOURCE_COMMIT="96841ea08dcfa84b954a32dc5ae1a26c28966cf4"
AMD64_BASE_SIZE="164624792"
AMD64_BASE_SHA256="3768988b151c20f965679062b065c63a977d6bbb9f47fd83695ec2c40790c18f"
ARM64_BASE_SIZE="155595600"
ARM64_BASE_SHA256="5b7a46a0abfbe23a1d4454b5600e2efcfce16b705bc7e9c851c37470c035ef98"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ORACLE_DIR="$PROJECT_ROOT/target/freebsd-oracle"
CACHE_DIR="$PROJECT_ROOT/target/freebsd-cache"
SYSROOTS_DIR="$ORACLE_DIR/sysroots"

sha256_file() {
    if command -v sha256sum >/dev/null 2>&1; then
        sha256sum "$1" | awk '{print $1}'
        return
    fi
    if command -v shasum >/dev/null 2>&1; then
        shasum -a 256 "$1" | awk '{print $1}'
        return
    fi
    echo "error: sha256sum or shasum is required" >&2
    return 1
}

archive_url() {
    printf 'https://download.freebsd.org/releases/%s/%s/base.txz\n' "$1" "$FREEBSD_RELEASE"
}

archive_size() {
    case "$1" in
        amd64) printf '%s\n' "$AMD64_BASE_SIZE" ;;
        arm64) printf '%s\n' "$ARM64_BASE_SIZE" ;;
    esac
}

archive_sha256() {
    case "$1" in
        amd64) printf '%s\n' "$AMD64_BASE_SHA256" ;;
        arm64) printf '%s\n' "$ARM64_BASE_SHA256" ;;
    esac
}

validate_sysroot() {
    local architecture="$1"
    local sysroot="$2"
    local expected_machine
    local actual_machine
    for path in usr/include/stdio.h usr/include/sys/event.h usr/include/sys/param.h lib/libc.so.7; do
        if [[ ! -e "$sysroot/$path" ]]; then
            echo "error: FreeBSD $architecture sysroot is missing $path: $sysroot" >&2
            return 1
        fi
    done
    if ! grep -Eq "^#define[[:space:]]+__FreeBSD_version[[:space:]]+$FREEBSD_VERSION_NUMBER$" "$sysroot/usr/include/sys/param.h"; then
        echo "error: FreeBSD $architecture sysroot does not match $FREEBSD_RELEASE" >&2
        return 1
    fi
    case "$architecture" in
        amd64) expected_machine="Advanced Micro Devices X86-64" ;;
        arm64) expected_machine="AArch64" ;;
    esac
    if command -v llvm-readelf >/dev/null 2>&1; then
        actual_machine="$(llvm-readelf --file-header "$sysroot/lib/libc.so.7" | sed -n 's/^[[:space:]]*Machine:[[:space:]]*//p')"
    elif command -v readelf >/dev/null 2>&1; then
        actual_machine="$(readelf --file-header "$sysroot/lib/libc.so.7" | sed -n 's/^[[:space:]]*Machine:[[:space:]]*//p')"
    else
        echo "error: llvm-readelf or readelf is required to validate FreeBSD libc" >&2
        return 1
    fi
    if [[ "$actual_machine" != "$expected_machine" ]]; then
        echo "error: FreeBSD $architecture libc reports machine $actual_machine, expected $expected_machine" >&2
        return 1
    fi
}

download_archive() {
    local architecture="$1"
    local archive="$CACHE_DIR/$FREEBSD_RELEASE-$architecture-base.txz"
    local partial="$archive.partial"
    local expected_size
    local expected_sha256
    local actual_size
    local actual_sha256
    expected_size="$(archive_size "$architecture")"
    expected_sha256="$(archive_sha256 "$architecture")"
    if [[ ! -f "$archive" ]]; then
        if ! command -v curl >/dev/null 2>&1; then
            echo "error: curl is required to download FreeBSD $FREEBSD_RELEASE" >&2
            return 1
        fi
        curl --fail --location --continue-at - --output "$partial" "$(archive_url "$architecture")"
        mv "$partial" "$archive"
    fi
    actual_size="$(wc -c < "$archive" | tr -d ' ')"
    actual_sha256="$(sha256_file "$archive")"
    if [[ "$actual_size" != "$expected_size" || "$actual_sha256" != "$expected_sha256" ]]; then
        echo "error: cached FreeBSD $architecture base.txz failed size or checksum verification: $archive" >&2
        echo "remove that file and rerun" >&2
        return 1
    fi
    printf '%s\n' "$archive"
}

materialize_sysroot() {
    local architecture="$1"
    local archive="$2"
    local sysroot="$SYSROOTS_DIR/$architecture"
    local staging="$SYSROOTS_DIR/$architecture.tmp.$$"
    if [[ -d "$sysroot" ]]; then
        validate_sysroot "$architecture" "$sysroot"
        printf '%s\n' "$sysroot"
        return
    fi
    trap 'rm -rf "$staging"' RETURN
    mkdir -p "$staging"
    tar -xJf "$archive" -C "$staging" ./lib ./usr/include ./usr/lib
    printf '%s\n' "$FREEBSD_RELEASE" > "$staging/.slate-freebsd-release"
    validate_sysroot "$architecture" "$staging"
    mv "$staging" "$sysroot"
    trap - RETURN
    printf '%s\n' "$sysroot"
}

mkdir -p "$ORACLE_DIR" "$CACHE_DIR" "$SYSROOTS_DIR"

if [[ -n "${SLATE_FREEBSD_AMD64_SYSROOT:-}" || -n "${SLATE_FREEBSD_ARM64_SYSROOT:-}" ]]; then
    if [[ -z "${SLATE_FREEBSD_AMD64_SYSROOT:-}" || -z "${SLATE_FREEBSD_ARM64_SYSROOT:-}" ]]; then
        echo "error: set both SLATE_FREEBSD_AMD64_SYSROOT and SLATE_FREEBSD_ARM64_SYSROOT" >&2
        exit 2
    fi
    AMD64_SYSROOT="$(cd "$SLATE_FREEBSD_AMD64_SYSROOT" && pwd)"
    ARM64_SYSROOT="$(cd "$SLATE_FREEBSD_ARM64_SYSROOT" && pwd)"
    ACQUISITION="external"
else
    AMD64_ARCHIVE="$(download_archive amd64)"
    ARM64_ARCHIVE="$(download_archive arm64)"
    AMD64_SYSROOT="$(materialize_sysroot amd64 "$AMD64_ARCHIVE")"
    ARM64_SYSROOT="$(materialize_sysroot arm64 "$ARM64_ARCHIVE")"
    ACQUISITION="downloaded"
fi

validate_sysroot amd64 "$AMD64_SYSROOT"
validate_sysroot arm64 "$ARM64_SYSROOT"

printf 'amd64\t%s\narm64\t%s\n' "$AMD64_SYSROOT" "$ARM64_SYSROOT" > "$ORACLE_DIR/sysroot-roots.tsv"
cat > "$ORACLE_DIR/oracle.json" <<EOF
{
  "freebsd_release": "$FREEBSD_RELEASE",
  "freebsd_version": "$FREEBSD_VERSION",
  "freebsd_version_number": $FREEBSD_VERSION_NUMBER,
  "source_tag": "$SOURCE_TAG",
  "source_commit": "$SOURCE_COMMIT",
  "acquisition": "$ACQUISITION",
  "architectures": {
    "aarch64": {
      "distribution_architecture": "arm64",
      "clang_target": "aarch64-unknown-freebsd$FREEBSD_VERSION",
      "rust_target": "aarch64-unknown-freebsd",
      "base_url": "$(archive_url arm64)",
      "base_size": $ARM64_BASE_SIZE,
      "base_sha256": "$ARM64_BASE_SHA256",
      "sysroot": "$ARM64_SYSROOT"
    },
    "x86_64": {
      "distribution_architecture": "amd64",
      "clang_target": "x86_64-unknown-freebsd$FREEBSD_VERSION",
      "rust_target": "x86_64-unknown-freebsd",
      "base_url": "$(archive_url amd64)",
      "base_size": $AMD64_BASE_SIZE,
      "base_sha256": "$AMD64_BASE_SHA256",
      "sysroot": "$AMD64_SYSROOT"
    }
  }
}
EOF

SLATE_FREEBSD_AMD64_SYSROOT="$AMD64_SYSROOT" \
SLATE_FREEBSD_ARM64_SYSROOT="$ARM64_SYSROOT" \
    "$SCRIPT_DIR/compare-freebsd-libc.sh"

echo "FreeBSD libc oracle ready: $ORACLE_DIR"
echo "Release baseline: $FREEBSD_RELEASE"
