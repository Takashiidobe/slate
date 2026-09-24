#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
sysroot="$repo_root/sysroots/aarch64-unknown-linux-musl"
clang_bin="${CLANG:-clang}"
lld_bin="${LLD:-ld.lld}"
qemu_bin="${QEMU_AARCH64:-}"
if [[ -z "$qemu_bin" ]]; then
    if command -v qemu-aarch64 >/dev/null 2>&1; then
        qemu_bin=qemu-aarch64
    else
        qemu_bin=qemu-aarch64-static
    fi
fi
target_libdir="$sysroot/usr/lib"
loader="$sysroot/lib/ld-musl-aarch64.so.1"
resource_dir="$("$clang_bin" -print-resource-dir)"
work_dir="$(mktemp -d)"
trap 'rm -rf "$work_dir"' EXIT

for tool in "$clang_bin" "$lld_bin" "$qemu_bin"; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        printf 'error: required tool not found: %s\n' "$tool" >&2
        exit 1
    fi
done

if [[ ! -f "$sysroot/SYSROOT-MANIFEST.txt" || ! -x "$loader" ]]; then
    printf 'error: AArch64 musl sysroot not found; run scripts/fetch-aarch64-musl-sysroot.sh first\n' >&2
    exit 1
fi

cat > "$work_dir/smoke.c" <<'EOF'
#include <stdio.h>

int main(void) {
    puts("aarch64 musl sysroot");
    return 0;
}
EOF

"$clang_bin" \
    --target=aarch64-unknown-linux-musl \
    --sysroot="$sysroot" \
    -nostdinc -isystem "$resource_dir/include" -isystem "$sysroot/usr/include" \
    -H -fsyntax-only "$work_dir/smoke.c" 2>"$work_dir/includes.txt"
if ! rg -F "$sysroot/usr/include/stdio.h" "$work_dir/includes.txt" >/dev/null; then
    cat "$work_dir/includes.txt" >&2
    printf 'error: stdio.h was not loaded from the target sysroot\n' >&2
    exit 1
fi

"$clang_bin" --target=aarch64-unknown-linux-musl --sysroot="$sysroot" \
    -nostdinc -isystem "$resource_dir/include" -isystem "$sysroot/usr/include" \
    -c "$work_dir/smoke.c" -o "$work_dir/smoke.o"
"$lld_bin" --sysroot="$sysroot" -m aarch64linux --pie --no-dynamic-linker \
    -o "$work_dir/smoke" \
    "$target_libdir/Scrt1.o" "$target_libdir/crti.o" "$work_dir/smoke.o" \
    -L"$target_libdir" -lc "$target_libdir/crtn.o"

output="$("$qemu_bin" -L "$sysroot" "$loader" \
    --library-path "$target_libdir" "$work_dir/smoke")"
if [[ "$output" != 'aarch64 musl sysroot' ]]; then
    printf 'error: unexpected program output: %s\n' "$output" >&2
    exit 1
fi

printf 'Clang compiled and ran against %s\n' "$sysroot"
