#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ORACLE_DIR="$PROJECT_ROOT/target/freebsd-oracle"
SHIM_SOURCE="$PROJECT_ROOT/libc-shim/include"
REPORT_DIR="$ORACLE_DIR/comparison"
DIFF_DIR="$REPORT_DIR/diffs"
AMD64_SYSROOT="${SLATE_FREEBSD_AMD64_SYSROOT:-}"
ARM64_SYSROOT="${SLATE_FREEBSD_ARM64_SYSROOT:-}"

if [[ -z "$AMD64_SYSROOT" && -f "$ORACLE_DIR/sysroot-roots.tsv" ]]; then
    AMD64_SYSROOT="$(awk -F '\t' '$1 == "amd64" {print $2}' "$ORACLE_DIR/sysroot-roots.tsv")"
fi
if [[ -z "$ARM64_SYSROOT" && -f "$ORACLE_DIR/sysroot-roots.tsv" ]]; then
    ARM64_SYSROOT="$(awk -F '\t' '$1 == "arm64" {print $2}' "$ORACLE_DIR/sysroot-roots.tsv")"
fi
if [[ ! -d "$AMD64_SYSROOT/usr/include" || ! -d "$ARM64_SYSROOT/usr/include" ]]; then
    echo "error: run tools/bootstrap-freebsd-oracle.sh first" >&2
    exit 1
fi

mkdir -p "$DIFF_DIR/amd64" "$DIFF_DIR/arm64"
(cd "$SHIM_SOURCE" && find . -type f -name '*.h' | sed 's#^\./##' | sort) > "$REPORT_DIR/shim-headers.txt"
(cd "$AMD64_SYSROOT/usr/include" && find . -type f -name '*.h' | sed 's#^\./##' | sort) > "$REPORT_DIR/freebsd-amd64-headers.txt"
(cd "$ARM64_SYSROOT/usr/include" && find . -type f -name '*.h' | sed 's#^\./##' | sort) > "$REPORT_DIR/freebsd-arm64-headers.txt"

printf 'header\tamd64\tarm64\n' > "$REPORT_DIR/header-map.tsv"
while IFS= read -r header; do
    amd64_state=missing
    arm64_state=missing
    if [[ -f "$AMD64_SYSROOT/usr/include/$header" ]]; then
        amd64_state=present
        mkdir -p "$DIFF_DIR/amd64/$(dirname "$header")"
        diff -u "$AMD64_SYSROOT/usr/include/$header" "$SHIM_SOURCE/$header" > "$DIFF_DIR/amd64/$header.diff" || true
    fi
    if [[ -f "$ARM64_SYSROOT/usr/include/$header" ]]; then
        arm64_state=present
        mkdir -p "$DIFF_DIR/arm64/$(dirname "$header")"
        diff -u "$ARM64_SYSROOT/usr/include/$header" "$SHIM_SOURCE/$header" > "$DIFF_DIR/arm64/$header.diff" || true
    fi
    printf '%s\t%s\t%s\n' "$header" "$amd64_state" "$arm64_state" >> "$REPORT_DIR/header-map.tsv"
done < "$REPORT_DIR/shim-headers.txt"

comm -12 "$REPORT_DIR/freebsd-amd64-headers.txt" "$REPORT_DIR/shim-headers.txt" > "$REPORT_DIR/amd64-overlap.txt"
comm -12 "$REPORT_DIR/freebsd-arm64-headers.txt" "$REPORT_DIR/shim-headers.txt" > "$REPORT_DIR/arm64-overlap.txt"

{
    printf 'shim headers: %s\n' "$(wc -l < "$REPORT_DIR/shim-headers.txt")"
    printf 'FreeBSD amd64 headers: %s\n' "$(wc -l < "$REPORT_DIR/freebsd-amd64-headers.txt")"
    printf 'FreeBSD arm64 headers: %s\n' "$(wc -l < "$REPORT_DIR/freebsd-arm64-headers.txt")"
    printf 'amd64/shim overlap: %s\n' "$(wc -l < "$REPORT_DIR/amd64-overlap.txt")"
    printf 'arm64/shim overlap: %s\n' "$(wc -l < "$REPORT_DIR/arm64-overlap.txt")"
} > "$REPORT_DIR/summary.txt"

cat "$REPORT_DIR/summary.txt"
echo "header map: $REPORT_DIR/header-map.tsv"
echo "raw diffs: $DIFF_DIR"
