#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
clang_bin="${CLANG:-clang}"
resource_dir="$("$clang_bin" -print-resource-dir)"
work_dir="$(mktemp -d)"
trap 'rm -rf "$work_dir"' EXIT

cat > "$work_dir/smoke.c" <<'EOF'
#include <android/api-level.h>
#include <stdio.h>
#include <sys/types.h>

int main(void) {
    return __ANDROID_API__ >= 21 ? 0 : 1;
}
EOF

for target in x86_64-linux-android aarch64-linux-android; do
    sysroot="$repo_root/sysroots/$target"
    if [[ ! -f "$sysroot/SYSROOT-MANIFEST.txt" ]]; then
        printf 'error: Bionic headers not found; run scripts/fetch-android-bionic-headers.sh first\n' >&2
        exit 1
    fi

    "$clang_bin" \
        --target="${target}21" \
        --sysroot="$sysroot" \
        -nostdinc \
        -isystem "$resource_dir/include" \
        -isystem "$sysroot/usr/include/$target" \
        -isystem "$sysroot/usr/include" \
        -H -fsyntax-only "$work_dir/smoke.c" 2>"$work_dir/includes.txt"
    if ! rg -F "$sysroot/usr/include/stdio.h" "$work_dir/includes.txt" >/dev/null; then
        cat "$work_dir/includes.txt" >&2
        printf 'error: stdio.h was not loaded from %s\n' "$target" >&2
        exit 1
    fi
    printf 'Clang parsed Android API 21 Bionic headers for %s\n' "$target"
done
