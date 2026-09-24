#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
clang_bin="${CLANG:-clang}"
resource_dir="$("$clang_bin" -print-resource-dir)"
work_dir="$(mktemp -d)"
trap 'rm -rf "$work_dir"' EXIT

cat > "$work_dir/smoke.c" <<'EOF'
#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>

int main(void) {
    return __FreeBSD_version > 0 ? 0 : 1;
}
EOF

for target in x86_64-unknown-freebsd aarch64-unknown-freebsd; do
    sysroot="$repo_root/sysroots/$target"
    if [[ ! -f "$sysroot/SYSROOT-MANIFEST.txt" ]]; then
        printf 'error: FreeBSD headers not found; run scripts/fetch-freebsd-15-headers.sh for each architecture first\n' >&2
        exit 1
    fi

    case "$target" in
        x86_64-*) clang_target=x86_64-unknown-freebsd15.1 ;;
        aarch64-*) clang_target=aarch64-unknown-freebsd15.1 ;;
    esac

    "$clang_bin" \
        --target="$clang_target" \
        --sysroot="$sysroot" \
        -nostdinc -isystem "$resource_dir/include" -isystem "$sysroot/usr/include" \
        -H -fsyntax-only "$work_dir/smoke.c" 2>"$work_dir/includes.txt"
    if ! rg -F "$sysroot/usr/include/stdio.h" "$work_dir/includes.txt" >/dev/null; then
        cat "$work_dir/includes.txt" >&2
        printf 'error: stdio.h was not loaded from %s\n' "$target" >&2
        exit 1
    fi
    printf 'Clang parsed FreeBSD 15.1 headers for %s\n' "$target"
done
