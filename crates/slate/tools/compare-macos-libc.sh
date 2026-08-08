#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ORACLE_DIR="$PROJECT_ROOT/target/macos-oracle"
LIBC_SOURCE="$ORACLE_DIR/sources/Libc/include"
XNU_SOURCE="$ORACLE_DIR/sources/xnu/bsd"
SHIM_SOURCE="$PROJECT_ROOT/libc-shim/include"
REPORT_DIR="$ORACLE_DIR/comparison"
DIFF_DIR="$REPORT_DIR/diffs"
SDK_ROOT="${SLATE_MACOS_SDK:-}"

if [[ ! -d "$LIBC_SOURCE" || ! -d "$XNU_SOURCE" ]]; then
    echo "error: run tools/bootstrap-macos-oracle.sh first" >&2
    exit 1
fi

mkdir -p "$DIFF_DIR/libc" "$DIFF_DIR/xnu" "$DIFF_DIR/sdk"
(cd "$SHIM_SOURCE" && find . -type f -name '*.h' | sed 's#^\./##' | sort) > "$REPORT_DIR/shim-headers.txt"
(cd "$LIBC_SOURCE" && find . -type f -name '*.h' | sed 's#^\./##' | sort) > "$REPORT_DIR/apple-libc-headers.txt"
(cd "$XNU_SOURCE" && find . -type f -name '*.h' | sed 's#^\./##' | sort) > "$REPORT_DIR/apple-xnu-bsd-headers.txt"

printf 'header\tlibc_source\txnu_source\tsdk\n' > "$REPORT_DIR/header-map.tsv"
while IFS= read -r header; do
    libc_state=missing
    xnu_state=missing
    sdk_state=unavailable
    if [[ -f "$LIBC_SOURCE/$header" ]]; then
        libc_state=present
        mkdir -p "$DIFF_DIR/libc/$(dirname "$header")"
        diff -u "$LIBC_SOURCE/$header" "$SHIM_SOURCE/$header" > "$DIFF_DIR/libc/$header.diff" || true
    fi
    if [[ -f "$XNU_SOURCE/$header" ]]; then
        xnu_state=present
        mkdir -p "$DIFF_DIR/xnu/$(dirname "$header")"
        diff -u "$XNU_SOURCE/$header" "$SHIM_SOURCE/$header" > "$DIFF_DIR/xnu/$header.diff" || true
    fi
    if [[ -n "$SDK_ROOT" ]]; then
        sdk_state=missing
        if [[ -f "$SDK_ROOT/usr/include/$header" ]]; then
            sdk_state=present
            mkdir -p "$DIFF_DIR/sdk/$(dirname "$header")"
            diff -u "$SDK_ROOT/usr/include/$header" "$SHIM_SOURCE/$header" > "$DIFF_DIR/sdk/$header.diff" || true
        fi
    fi
    printf '%s\t%s\t%s\t%s\n' "$header" "$libc_state" "$xnu_state" "$sdk_state" >> "$REPORT_DIR/header-map.tsv"
done < "$REPORT_DIR/shim-headers.txt"

comm -12 "$REPORT_DIR/apple-libc-headers.txt" "$REPORT_DIR/shim-headers.txt" > "$REPORT_DIR/libc-overlap.txt"
comm -23 "$REPORT_DIR/apple-libc-headers.txt" "$REPORT_DIR/shim-headers.txt" > "$REPORT_DIR/libc-source-only.txt"

{
    printf 'shim headers: %s\n' "$(wc -l < "$REPORT_DIR/shim-headers.txt")"
    printf 'Apple Libc source headers: %s\n' "$(wc -l < "$REPORT_DIR/apple-libc-headers.txt")"
    printf 'direct Libc/shim overlap: %s\n' "$(wc -l < "$REPORT_DIR/libc-overlap.txt")"
    printf 'Apple Libc source-only headers: %s\n' "$(wc -l < "$REPORT_DIR/libc-source-only.txt")"
    printf 'XNU BSD source headers: %s\n' "$(wc -l < "$REPORT_DIR/apple-xnu-bsd-headers.txt")"
    if [[ -n "$SDK_ROOT" ]]; then
        printf 'SDK root: %s\n' "$SDK_ROOT"
    else
        printf 'SDK root: unavailable\n'
    fi
} > "$REPORT_DIR/summary.txt"

cat "$REPORT_DIR/summary.txt"
echo "header map: $REPORT_DIR/header-map.tsv"
echo "raw diffs: $DIFF_DIR"
