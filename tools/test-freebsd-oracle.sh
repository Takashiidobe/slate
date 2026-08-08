#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ORACLE_DIR="$PROJECT_ROOT/target/freebsd-oracle"
RELEASE="15.1-RELEASE"

"$SCRIPT_DIR/bootstrap-freebsd-oracle.sh"
"$SCRIPT_DIR/bootstrap-freebsd-oracle.sh"

test -f "$ORACLE_DIR/oracle.json"
grep -q '"freebsd_release": "15.1-RELEASE"' "$ORACLE_DIR/oracle.json"
grep -q '"source_commit": "96841ea08dcfa84b954a32dc5ae1a26c28966cf4"' "$ORACLE_DIR/oracle.json"
grep -q '"clang_target": "aarch64-unknown-freebsd15.1"' "$ORACLE_DIR/oracle.json"
grep -q '"clang_target": "x86_64-unknown-freebsd15.1"' "$ORACLE_DIR/oracle.json"
test -s "$ORACLE_DIR/comparison/summary.txt"
test -s "$ORACLE_DIR/comparison/header-map.tsv"

if "$SCRIPT_DIR/probe-freebsd-libc.sh" --arch riscv64 --mode predefined >/dev/null 2>&1; then
    echo "probe accepted an unsupported architecture" >&2
    exit 1
fi

for arch in aarch64 x86_64; do
    for mode in predefined header-macros preprocess ast layouts assembly availability symbols; do
        "$SCRIPT_DIR/probe-freebsd-libc.sh" --arch "$arch" --mode "$mode"
    done
    output="$ORACLE_DIR/probes/$arch/$RELEASE"
    test -s "$output/predefined.txt"
    grep -q '#define __FreeBSD__ 15' "$output/predefined.txt"
    test -s "$output/header-macros.txt"
    grep -q '#define __FreeBSD_version 1501000' "$output/header-macros.txt"
    test -s "$output/preprocess.i"
    test -s "$output/ast.json"
    test -s "$output/layouts.txt"
    test -s "$output/assembly.s"
    test -s "$output/availability.json"
    test -s "$output/symbols.txt"
    grep -q 'slate_freebsd_export' "$output/symbols.txt"
    grep -q 'clock_gettime' "$output/symbols.txt"
done

echo "FreeBSD oracle tests passed"
