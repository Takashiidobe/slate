#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
clang_bin="${CLANG:-clang}"
sysroot="$repo_root/sysroots/x86_64-pc-windows-msvc"
resource_dir="$("$clang_bin" -print-resource-dir)"
work_dir="$(mktemp -d)"
trap 'rm -rf "$work_dir"' EXIT

if [[ ! -f "$sysroot/SYSROOT-MANIFEST.txt" ]]; then
    printf 'error: MSVC sysroot not found; run scripts/fetch-windows-msvc-sysroot.sh first\n' >&2
    exit 1
fi

cat > "$work_dir/smoke.c" <<'EOF'
#include <stdio.h>
#include <windows.h>

#ifndef _MSC_VER
#error expected the MSVC compatibility target
#endif

#ifndef _WIN64
#error expected a 64-bit Windows target
#endif

int main(void) {
    SYSTEM_INFO info;
    return (int)(sizeof(info) == 0);
}
EOF

"$clang_bin" \
    --target=x86_64-pc-windows-msvc \
    -fms-extensions \
    -fms-compatibility \
    -nostdinc \
    -isystem "$resource_dir/include" \
    -isystem "$sysroot/crt/include" \
    -isystem "$sysroot/sdk/include/ucrt" \
    -isystem "$sysroot/sdk/include/shared" \
    -isystem "$sysroot/sdk/include/um" \
    -isystem "$sysroot/sdk/include/winrt" \
    -isystem "$sysroot/sdk/include/cppwinrt" \
    -H -fsyntax-only "$work_dir/smoke.c" 2>"$work_dir/includes.txt"

for expected in \
    "$sysroot/sdk/include/ucrt/stdio.h" \
    "$sysroot/sdk/include/um/windows.h"; do
    if ! rg -F "$expected" "$work_dir/includes.txt" >/dev/null; then
        cat "$work_dir/includes.txt" >&2
        printf 'error: expected header was not loaded: %s\n' "$expected" >&2
        exit 1
    fi
done

printf 'Clang parsed MSVC CRT and Windows SDK headers for x86_64\n'
