#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
sysroot="$repo_root/sysroots/x86_64-unknown-linux-gnu"
clang_bin="${CLANG:-clang}"
loader="$sysroot/usr/x86_64-linux-gnu/lib/ld-linux-x86-64.so.2"
resource_dir="$("$clang_bin" -print-resource-dir)"
work_dir="$(mktemp -d)"
trap 'rm -rf "$work_dir"' EXIT

if ! command -v "$clang_bin" >/dev/null 2>&1; then
    printf 'error: clang not found: %s\n' "$clang_bin" >&2
    exit 1
fi

if [[ ! -f "$sysroot/SYSROOT-MANIFEST.txt" || ! -x "$loader" ]]; then
    printf 'error: glibc sysroot not found; run scripts/fetch-x86_64-glibc.sh first\n' >&2
    exit 1
fi

cat > "$work_dir/smoke.c" <<'EOF'
#include <stdio.h>

int main(void) {
    printf("glibc %d.%d\n", __GLIBC__, __GLIBC_MINOR__);
    return 0;
}
EOF

"$clang_bin" \
    --target=x86_64-unknown-linux-gnu \
    --sysroot="$sysroot" \
    -nostdinc -isystem "$resource_dir/include" -isystem "$sysroot/usr/include" \
    -H -fsyntax-only "$work_dir/smoke.c" 2>"$work_dir/includes.txt"

if ! rg -F "$sysroot/usr/include/stdio.h" "$work_dir/includes.txt" >/dev/null; then
    cat "$work_dir/includes.txt" >&2
    printf 'error: stdio.h was not loaded from the target sysroot\n' >&2
    exit 1
fi

"$clang_bin" \
    --target=x86_64-unknown-linux-gnu \
    --sysroot="$sysroot" \
    -nostdinc -isystem "$resource_dir/include" -isystem "$sysroot/usr/include" \
    "$work_dir/smoke.c" -o "$work_dir/smoke"

output="$("$loader" --library-path "$sysroot/usr/x86_64-linux-gnu/lib" "$work_dir/smoke")"
if [[ "$output" != 'glibc 2.36' ]]; then
    printf 'error: unexpected program output: %s\n' "$output" >&2
    exit 1
fi

printf 'Clang compiled and ran against %s\n' "$sysroot"
