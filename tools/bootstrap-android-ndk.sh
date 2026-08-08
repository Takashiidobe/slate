#!/usr/bin/env bash
set -euo pipefail

NDK_RELEASE="r27d"
NDK_REVISION="27.3.13750724"
NDK_ARCHIVE="android-ndk-r27d-linux.zip"
NDK_ARCHIVE_SIZE="663956036"
NDK_ARCHIVE_SHA1="22105e410cf29afcf163760cc95522b9fb981121"
NDK_ARCHIVE_SHA256="601246087a682d1944e1e16dd85bc6e49560fe8b6d61255be2829178c8ed15d9"
NDK_ARCHIVE_URL="https://dl.google.com/android/repository/android-ndk-r27d-linux.zip"
API_LEVEL="21"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ORACLE_DIR="$PROJECT_ROOT/target/android-ndk-oracle"
CACHE_DIR="$PROJECT_ROOT/target/android-ndk-cache"
MANAGED_NDK="$ORACLE_DIR/ndk"
ACCEPT_LICENSE=0

usage() {
    echo "usage: tools/bootstrap-android-ndk.sh [--accept-license]"
    echo "       SLATE_ANDROID_NDK=/path/to/ndk tools/bootstrap-android-ndk.sh"
}

for arg in "$@"; do
    case "$arg" in
        --accept-license)
            ACCEPT_LICENSE=1
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "error: unknown argument: $arg" >&2
            usage >&2
            exit 2
            ;;
    esac
done

if [[ "${ANDROID_NDK_ACCEPT_LICENSE:-}" == "1" ]]; then
    ACCEPT_LICENSE=1
fi

sha1_file() {
    if command -v sha1sum >/dev/null 2>&1; then
        sha1sum "$1" | awk '{print $1}'
        return
    fi
    if command -v shasum >/dev/null 2>&1; then
        shasum -a 1 "$1" | awk '{print $1}'
        return
    fi
    echo "error: sha1sum or shasum is required" >&2
    return 1
}

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

validate_ndk() {
    local ndk_root="$1"
    local revision
    if [[ ! -f "$ndk_root/source.properties" ]]; then
        echo "error: Android NDK source.properties is missing: $ndk_root" >&2
        return 1
    fi
    revision="$(sed -n 's/^Pkg\.Revision[[:space:]]*=[[:space:]]*//p' "$ndk_root/source.properties")"
    if [[ "$revision" != "$NDK_REVISION" ]]; then
        echo "error: Android NDK revision is $revision, expected $NDK_REVISION" >&2
        return 1
    fi
    local host_tag
    host_tag="$(find "$ndk_root/toolchains/llvm/prebuilt" -mindepth 1 -maxdepth 1 -type d -exec basename {} \; | head -1)"
    if [[ -z "$host_tag" ]]; then
        echo "error: Android NDK has no LLVM host toolchain: $ndk_root" >&2
        return 1
    fi
    local sysroot="$ndk_root/toolchains/llvm/prebuilt/$host_tag/sysroot"
    local clang="$ndk_root/toolchains/llvm/prebuilt/$host_tag/bin/clang"
    if [[ ! -x "$clang" || ! -f "$sysroot/usr/include/stdio.h" ]]; then
        echo "error: Android NDK LLVM sysroot is incomplete: $ndk_root" >&2
        return 1
    fi
    for triple in aarch64-linux-android x86_64-linux-android; do
        if [[ ! -d "$sysroot/usr/lib/$triple/$API_LEVEL" ]]; then
            echo "error: Android API $API_LEVEL libraries are missing for $triple" >&2
            return 1
        fi
    done
    printf '%s\n' "$host_tag"
}

mkdir -p "$ORACLE_DIR" "$CACHE_DIR"

if [[ -n "${SLATE_ANDROID_NDK:-}" ]]; then
    NDK_ROOT="$(cd "$SLATE_ANDROID_NDK" && pwd)"
    ACQUISITION="external"
else
    if [[ "$(uname -s)" != "Linux" || "$(uname -m)" != "x86_64" ]]; then
        echo "error: automatic NDK acquisition supports only Linux x86_64" >&2
        echo "install NDK $NDK_REVISION and set SLATE_ANDROID_NDK on this host" >&2
        exit 1
    fi
    if [[ "$ACCEPT_LICENSE" != "1" ]]; then
        echo "error: Android NDK license acceptance is required for automatic download" >&2
        echo "review https://developer.android.com/ndk/downloads, then rerun with --accept-license or ANDROID_NDK_ACCEPT_LICENSE=1" >&2
        exit 2
    fi
    ARCHIVE_PATH="$CACHE_DIR/$NDK_ARCHIVE"
    if [[ -f "$ARCHIVE_PATH" ]]; then
        actual_size="$(wc -c < "$ARCHIVE_PATH" | tr -d ' ')"
        actual_sha1="$(sha1_file "$ARCHIVE_PATH")"
        actual_sha256="$(sha256_file "$ARCHIVE_PATH")"
        if [[ "$actual_size" != "$NDK_ARCHIVE_SIZE" || "$actual_sha1" != "$NDK_ARCHIVE_SHA1" || "$actual_sha256" != "$NDK_ARCHIVE_SHA256" ]]; then
            echo "error: cached NDK archive failed size or checksum verification: $ARCHIVE_PATH" >&2
            echo "remove that file and rerun" >&2
            exit 1
        fi
    else
        if ! command -v curl >/dev/null 2>&1; then
            echo "error: curl is required to download Android NDK $NDK_RELEASE" >&2
            exit 1
        fi
        DOWNLOAD_PATH="$CACHE_DIR/$NDK_ARCHIVE.partial"
        if ! curl --fail --location --continue-at - --output "$DOWNLOAD_PATH" "$NDK_ARCHIVE_URL"; then
            echo "error: failed to download $NDK_ARCHIVE_URL" >&2
            exit 1
        fi
        actual_size="$(wc -c < "$DOWNLOAD_PATH" | tr -d ' ')"
        actual_sha1="$(sha1_file "$DOWNLOAD_PATH")"
        actual_sha256="$(sha256_file "$DOWNLOAD_PATH")"
        if [[ "$actual_size" != "$NDK_ARCHIVE_SIZE" || "$actual_sha1" != "$NDK_ARCHIVE_SHA1" || "$actual_sha256" != "$NDK_ARCHIVE_SHA256" ]]; then
            echo "error: downloaded NDK archive failed size or checksum verification" >&2
            exit 1
        fi
        mv "$DOWNLOAD_PATH" "$ARCHIVE_PATH"
    fi
    if [[ ! -d "$MANAGED_NDK" ]]; then
        if ! command -v unzip >/dev/null 2>&1; then
            echo "error: unzip is required to materialize Android NDK $NDK_RELEASE" >&2
            exit 1
        fi
        STAGING_ROOT="$ORACLE_DIR/ndk.tmp.$$"
        trap 'rm -rf "$STAGING_ROOT"' EXIT
        mkdir -p "$STAGING_ROOT"
        unzip -q "$ARCHIVE_PATH" -d "$STAGING_ROOT"
        mv "$STAGING_ROOT/android-ndk-$NDK_RELEASE" "$MANAGED_NDK"
        rmdir "$STAGING_ROOT"
        trap - EXIT
    fi
    NDK_ROOT="$MANAGED_NDK"
    ACQUISITION="downloaded"
fi

HOST_TAG="$(validate_ndk "$NDK_ROOT")"
TOOLCHAIN="$NDK_ROOT/toolchains/llvm/prebuilt/$HOST_TAG"
SYSROOT="$TOOLCHAIN/sysroot"
HOST_PACKAGE="$NDK_ARCHIVE"
if [[ "$ACQUISITION" == "external" ]]; then
    HOST_PACKAGE="external-$HOST_TAG"
fi

printf '%s\n' "$NDK_ROOT" > "$ORACLE_DIR/ndk-root.txt"
cat > "$ORACLE_DIR/oracle.json" <<EOF
{
  "ndk_release": "$NDK_RELEASE",
  "ndk_revision": "$NDK_REVISION",
  "host_package": "$HOST_PACKAGE",
  "download_pin": {"package": "$NDK_ARCHIVE", "size": $NDK_ARCHIVE_SIZE, "sha1": "$NDK_ARCHIVE_SHA1", "sha256": "$NDK_ARCHIVE_SHA256"},
  "host_tag": "$HOST_TAG",
  "acquisition": "$ACQUISITION",
  "api_level": $API_LEVEL,
  "architectures": {
    "aarch64": {"clang_target": "aarch64-linux-android$API_LEVEL", "library_triple": "aarch64-linux-android"},
    "x86_64": {"clang_target": "x86_64-linux-android$API_LEVEL", "library_triple": "x86_64-linux-android"}
  },
  "ndk_root": "$NDK_ROOT",
  "sysroot": "$SYSROOT",
  "include_root": "$SYSROOT/usr/include",
  "metadata_root": "$NDK_ROOT/meta"
}
EOF

echo "Android NDK oracle ready: $ORACLE_DIR"
echo "NDK root: $NDK_ROOT"
echo "API baseline: $API_LEVEL"
