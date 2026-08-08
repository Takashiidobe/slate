#!/usr/bin/env bash
set -euo pipefail

MACOS_RELEASE="11.0.1"
DEPLOYMENT_TARGET="11.0"
DISTRIBUTION_TAG="macos-1101"
DISTRIBUTION_COMMIT="0d630ff4a33e8c13df2e6b0ccfcc0d78c1360c90"
LIBC_TAG="Libc-1439.40.11"
LIBC_COMMIT="768d166d42689471e1e8fd1ddde5eee25db02381"
XNU_TAG="xnu-7195.50.7.100.1"
XNU_COMMIT="bb611c8fecc755a0d8e56e2fa51513527c5b7a0e"
TARGET_TRIPLE="arm64-apple-macos11.0"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ORACLE_DIR="$PROJECT_ROOT/target/macos-oracle"
SOURCES_DIR="$ORACLE_DIR/sources"

clone_source() {
    local name="$1"
    local repository="$2"
    local tag="$3"
    local commit="$4"
    local destination="$SOURCES_DIR/$name"

    if [[ -d "$destination/.git" ]]; then
        local actual
        actual="$(git -C "$destination" rev-parse HEAD)"
        if [[ "$actual" != "$commit" ]]; then
            echo "error: $destination is at $actual, expected $commit; remove that directory and rerun" >&2
            return 1
        fi
        return
    fi
    if [[ -e "$destination" ]]; then
        echo "error: $destination exists but is not a Git checkout" >&2
        return 1
    fi
    git clone --quiet --depth 1 --branch "$tag" "$repository" "$destination"
    local actual
    actual="$(git -C "$destination" rev-parse HEAD)"
    if [[ "$actual" != "$commit" ]]; then
        echo "error: $name resolved to $actual, expected $commit" >&2
        return 1
    fi
}

discover_sdk() {
    if [[ -n "${SLATE_MACOS_SDK:-}" ]]; then
        printf '%s\n' "$SLATE_MACOS_SDK"
        return
    fi
    if command -v xcrun >/dev/null 2>&1; then
        xcrun --sdk macosx --show-sdk-path
        return
    fi
    return 1
}

mkdir -p "$SOURCES_DIR"
clone_source distribution https://github.com/apple-oss-distributions/distribution-macOS.git "$DISTRIBUTION_TAG" "$DISTRIBUTION_COMMIT"
clone_source Libc https://github.com/apple-oss-distributions/Libc.git "$LIBC_TAG" "$LIBC_COMMIT"
clone_source xnu https://github.com/apple-oss-distributions/xnu.git "$XNU_TAG" "$XNU_COMMIT"

SDK_ROOT="$(discover_sdk || true)"
if [[ -n "$SDK_ROOT" ]]; then
    if [[ ! -d "$SDK_ROOT/usr/include" ]]; then
        echo "error: macOS SDK has no usr/include directory: $SDK_ROOT" >&2
        exit 1
    fi
    for header in assert.h errno.h stdio.h stdlib.h string.h sys/types.h; do
        if [[ ! -f "$SDK_ROOT/usr/include/$header" ]]; then
            echo "error: macOS SDK is missing usr/include/$header: $SDK_ROOT" >&2
            exit 1
        fi
    done
fi

SDK_JSON="null"
if [[ -n "$SDK_ROOT" ]]; then
    SDK_JSON="\"$SDK_ROOT\""
fi
cat > "$ORACLE_DIR/oracle.json" <<EOF
{
  "macos_release": "$MACOS_RELEASE",
  "deployment_target": "$DEPLOYMENT_TARGET",
  "clang_target": "$TARGET_TRIPLE",
  "distribution": {"tag": "$DISTRIBUTION_TAG", "commit": "$DISTRIBUTION_COMMIT"},
  "libc": {"tag": "$LIBC_TAG", "commit": "$LIBC_COMMIT"},
  "xnu": {"tag": "$XNU_TAG", "commit": "$XNU_COMMIT"},
  "sdk_root": $SDK_JSON
}
EOF

if [[ -n "$SDK_ROOT" ]]; then
    {
        printf 'root=%s\n' "$SDK_ROOT"
        if command -v xcrun >/dev/null 2>&1; then
            printf 'version=%s\n' "$(xcrun --sdk macosx --show-sdk-version)"
            printf 'build=%s\n' "$(xcrun --sdk macosx --show-sdk-build-version)"
        elif command -v plutil >/dev/null 2>&1 && [[ -f "$SDK_ROOT/SDKSettings.plist" ]]; then
            printf 'version=%s\n' "$(plutil -extract Version raw "$SDK_ROOT/SDKSettings.plist")"
            if [[ -f "$SDK_ROOT/System/Library/CoreServices/SystemVersion.plist" ]]; then
                printf 'build=%s\n' "$(plutil -extract ProductBuildVersion raw "$SDK_ROOT/System/Library/CoreServices/SystemVersion.plist")"
            else
                printf 'build=unavailable\n'
            fi
        else
            printf 'version=inspect SDKSettings.json or SDKSettings.plist\n'
            printf 'build=unavailable without xcrun or plutil\n'
        fi
    } > "$ORACLE_DIR/sdk-identity.txt"
fi

SLATE_MACOS_SDK="$SDK_ROOT" "$SCRIPT_DIR/compare-macos-libc.sh"

echo "macOS open-source oracle ready: $ORACLE_DIR"
if [[ -z "$SDK_ROOT" ]]; then
    echo "SDK probes unavailable: set SLATE_MACOS_SDK to an installed MacOSX.sdk and rerun"
else
    echo "macOS SDK: $SDK_ROOT"
fi
