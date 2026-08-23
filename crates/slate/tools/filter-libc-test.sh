#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 2 ]]; then
    printf 'usage: %s <libc-test checkout> <empty output dir>\n' "$0" >&2
    exit 2
fi

libc_test_source=$(realpath "$1")
libc_test_output=$(realpath -m "$2")
libc_test_clang=${SLATE_CLANG:-$HOME/llvm-project/build-cir/bin/clang}
libc_test_jobs=${SLATE_LIBC_TEST_JOBS:-8}
libc_test_timeout=${SLATE_LIBC_TEST_TIMEOUT:-5}

if [[ ! -d "$libc_test_source/src/api" || ! -d "$libc_test_source/src/functional" ]]; then
    printf 'missing src/api or src/functional under: %s\n' "$libc_test_source" >&2
    exit 2
fi

if [[ -e "$libc_test_output" ]] && find "$libc_test_output" -mindepth 1 -print -quit | grep -q .; then
    printf 'output directory is not empty: %s\n' "$libc_test_output" >&2
    exit 2
fi

mkdir -p "$libc_test_output/api" "$libc_test_output/functional"
libc_test_work=$(mktemp -d /tmp/slate-libc-test-filter.XXXXXX)
mkdir -p "$libc_test_work/status"

cleanup_libc_test_filter() {
    if [[ "$libc_test_work" == /tmp/slate-libc-test-filter.* ]]; then
        rm -rf -- "$libc_test_work"
    fi
}
trap cleanup_libc_test_filter EXIT

# api/*.c: compile-only declaration/type conformance checks (upstream builds
# them with `-c -pedantic-errors -Werror -Wno-unused -D_XOPEN_SOURCE=700` and
# never runs them -- api/main.c is an empty `int main(){}` stub the upstream
# Makefile links every api/*.o against so the directory produces a binary; it
# carries no test content and is dropped here). No admission gate: every file
# is a self-contained snippet, and shim conformance itself is what the
# libc_test_api_suite nextest bucket checks per (arch, libc) target.
for f in "$libc_test_source"/src/api/*.c; do
    name=$(basename "$f")
    [[ "$name" == main.c ]] && continue
    cp "$f" "$libc_test_output/api/$name"
done

# functional/*.c: differential runtime tests. Upstream splits each case into
# main.c (the test) plus src/common/{test.h,print.c} (t_status/t_printf, the
# only common/ helper any admitted case below needs). Slate's single-file
# `translate` path is simpler and faster than cross-TU translate-project (no
# per-case Cargo crate, no subprocess per case), so merge test.h + print.c +
# main.c into one self-contained <name>.c instead of vendoring three files.
# `#define _GNU_SOURCE 1` goes first, ahead of any merged content, so it wins
# regardless of where in main.c's own text a conditional
# `#ifndef _GNU_SOURCE` guard would otherwise have appeared. Tests that need a
# different common/*.c helper (rand.c, vmfill.c, fdfill.c, ...), argv, threads,
# or path.c's argv0-relative t_pathrel (dlopen.c itself) don't link against
# this reduced harness and are rejected here rather than vendored
# half-working. Cases that dlopen() a companion .so by a hardcoded
# "src/functional/<dso>.so" path (tls_align_dlopen, tls_init_dlopen) get that
# .so built and staged below instead.

# A <name>.mk with an empty `$(N).BINS:=` and a non-empty `$(N).LIBS:=` is a
# companion shared library upstream builds for a sibling *_dlopen case
# (tls_align_dso.c, tls_init_dso.c, dlopen_dso.c), not a standalone test --
# it has no main and must never be probed as its own case.
is_dso_only_case() {
    local mk="$libc_test_source/src/functional/$1.mk"
    [[ -f "$mk" ]] || return 1
    grep -qE '^\$\(N\)\.BINS:=[[:space:]]*$' "$mk" && grep -qE '^\$\(N\)\.LIBS:=' "$mk"
}

# *_dlopen.mk declares its companion .so as a build prerequisite, e.g.
#   $(B)/$(N).err: $(B)/$(D)/tls_align_dso.so
# Pull the "<stem>.so" stem out of that line so the case below can build and
# stage the real shared object.
dso_stem_for_case() {
    local mk="$libc_test_source/src/functional/$1.mk"
    [[ -f "$mk" ]] || return 0
    grep -oE '\$\(D\)/[A-Za-z0-9_]+\.so' "$mk" | head -n1 | sed -E 's#.*/##; s/\.so$//'
}

filter_libc_test_functional_case() {
    local source=$1
    local name
    name=$(basename "$source" .c)
    local merged="$libc_test_output/functional/$name.c"
    local binary="$libc_test_work/$name"
    local status="$libc_test_work/status/$name.tsv"
    local result

    {
        printf '#define _GNU_SOURCE 1\n\n'
        grep -v '^#include "test.h"$' "$libc_test_source/src/common/test.h"
        printf '\n'
        grep -v '^#include "test.h"$' "$libc_test_source/src/common/print.c"
        printf '\n'
        grep -v '^#include "test.h"$' "$source"
    } >"$merged"

    set +e
    timeout 30 "$libc_test_clang" -O0 -std=c23 -D_POSIX_C_SOURCE=200809L -o "$binary" \
        "$merged" -lm -lrt >/dev/null 2>&1
    result=$?
    set -e
    if [[ $result -ne 0 ]]; then
        rm -f "$merged"
        printf 'functional/%s\tcompile-failed\n' "$name" >"$status"
        return
    fi

    # Build and stage the companion .so, if this case dlopen()s one, into a
    # scratch dir shaped like the "src/functional/<dso>.so" path the test
    # source hardcodes, so the native probe actually exercises real dynamic
    # loading instead of a dlopen() failure that happens to match on both
    # sides.
    local dso_stem dso_source run_dir=""
    dso_stem=$(dso_stem_for_case "$name")
    if [[ -n "$dso_stem" ]]; then
        dso_source="$libc_test_source/src/functional/$dso_stem.c"
        if [[ -f "$dso_source" ]]; then
            run_dir="$libc_test_work/rundir/$name"
            mkdir -p "$run_dir/src/functional"
            set +e
            "$libc_test_clang" -O0 -std=c23 -shared -fPIC -o \
                "$run_dir/src/functional/$dso_stem.so" "$dso_source" >/dev/null 2>&1
            result=$?
            set -e
            [[ $result -eq 0 ]] || run_dir=""
        fi
    fi

    set +e
    if [[ -n "$run_dir" ]]; then
        (cd "$run_dir" && timeout "$libc_test_timeout" "$binary" >/dev/null 2>&1)
    else
        timeout "$libc_test_timeout" "$binary" >/dev/null 2>&1
    fi
    result=$?
    set -e
    if [[ $result -eq 124 ]]; then
        rm -f "$merged"
        printf 'functional/%s\trun-timeout\n' "$name" >"$status"
    elif [[ $result -ge 128 ]]; then
        rm -f "$merged"
        printf 'functional/%s\trun-crashed:%s\n' "$name" "$result" >"$status"
    else
        printf 'functional/%s\tadmitted\n' "$name" >"$status"
        if [[ -n "$run_dir" ]]; then
            cp "$run_dir/src/functional/$dso_stem.so" "$libc_test_output/functional/$dso_stem.so"
        fi
    fi
}

export libc_test_source libc_test_output libc_test_clang libc_test_timeout libc_test_work
export -f is_dso_only_case dso_stem_for_case filter_libc_test_functional_case

find "$libc_test_source/src/functional" -maxdepth 1 -type f -name '*.c' -print0 \
    | sort -z \
    | xargs -0 -r -P "$libc_test_jobs" -n 1 bash -c '
        name=$(basename "$1" .c)
        is_dso_only_case "$name" && exit 0
        filter_libc_test_functional_case "$1"
    ' _

find "$libc_test_work/status" -type f -name '*.tsv' -exec sh -c 'cat "$@"' _ {} + \
    | sort >"$libc_test_output/FILTER.tsv"
for f in "$libc_test_output"/api/*.c; do
    printf 'api/%s\tadmitted\n' "$(basename "$f")"
done >>"$libc_test_output/FILTER.tsv.api"
sort "$libc_test_output/FILTER.tsv.api" >>"$libc_test_output/FILTER.tsv"
rm -f "$libc_test_output/FILTER.tsv.api"

awk -F '\t' '{ counts[$2]++ } END { for (status in counts) print status, counts[status] }' \
    "$libc_test_output/FILTER.tsv" \
    | sort
