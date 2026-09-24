#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ndk_root="${ANDROID_NDK_HOME:-${ANDROID_NDK_ROOT:-}}"
if [[ -z "$ndk_root" ]]; then
    printf 'error: set ANDROID_NDK_HOME or ANDROID_NDK_ROOT to an installed Android NDK r27d\n' >&2
    exit 1
fi
ndk_root="$(cd "$ndk_root" && pwd)"
ndk_sysroot="$ndk_root/toolchains/llvm/prebuilt/linux-x86_64/sysroot"
ndk_revision='27.3.13750724'

if ! rg -q "^Pkg.Revision = $ndk_revision$" "$ndk_root/source.properties"; then
    printf 'error: expected Android NDK r27d (%s) at %s\n' "$ndk_revision" "$ndk_root" >&2
    exit 1
fi
if [[ ! -d "$ndk_sysroot/usr/include" || ! -f "$ndk_root/NOTICE" ]]; then
    printf 'error: incomplete Android NDK sysroot at %s\n' "$ndk_root" >&2
    exit 1
fi

fetch_target() {
    local triple="$1"
    local arch_header="$2"
    local target_dir="$repo_root/sysroots/$triple"

    if [[ -d "$target_dir" ]]; then
        chmod -R u+w "$target_dir"
        rm -rf "$target_dir"
    fi
    mkdir -p "$target_dir/usr/include" "$target_dir/licenses"
    cp -a "$ndk_sysroot/usr/include/." "$target_dir/usr/include/"

    for other_arch in aarch64-linux-android arm-linux-androideabi i686-linux-android \
        riscv64-linux-android x86_64-linux-android; do
        if [[ "$other_arch" != "$arch_header" ]]; then
            rm -rf "$target_dir/usr/include/$other_arch"
        fi
    done

    cp "$ndk_root/NOTICE" "$target_dir/licenses/ANDROID-NDK-NOTICE"
    cat > "$target_dir/SYSROOT-MANIFEST.txt" <<EOF
Target: $triple
Operating system: Android (Bionic)
NDK: r27d ($ndk_revision)
Android API level: 21
Contents: C system headers only; link libraries and runtime files omitted
Source package: https://dl.google.com/android/repository/android-ndk-r27d-linux.zip
Source SHA-1: 22105e410cf29afcf163760cc95522b9fb981121
EOF
    printf 'Prepared Bionic headers at %s\n' "$target_dir"
}

fetch_target x86_64-linux-android x86_64-linux-android
fetch_target aarch64-linux-android aarch64-linux-android
