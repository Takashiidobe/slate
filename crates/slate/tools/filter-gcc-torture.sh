#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 2 ]]; then
    printf 'usage: %s <gcc execute dir> <empty output dir>\n' "$0" >&2
    exit 2
fi

gcc_torture_source=$(realpath "$1")
gcc_torture_output=$(realpath -m "$2")
gcc_torture_clang=${SLATE_CLANG:-$HOME/llvm-project/build-cir/bin/clang}
gcc_torture_jobs=${SLATE_GCC_TORTURE_JOBS:-8}
gcc_torture_timeout=${SLATE_GCC_TORTURE_TIMEOUT:-5}

if [[ ! -d "$gcc_torture_source" ]]; then
    printf 'missing execute directory: %s\n' "$gcc_torture_source" >&2
    exit 2
fi

if [[ -e "$gcc_torture_output" ]] && find "$gcc_torture_output" -mindepth 1 -print -quit | grep -q .; then
    printf 'output directory is not empty: %s\n' "$gcc_torture_output" >&2
    exit 2
fi

mkdir -p "$gcc_torture_output"
gcc_torture_work=$(mktemp -d /tmp/slate-gcc-torture-filter.XXXXXX)
mkdir -p "$gcc_torture_work/status"

cleanup_gcc_torture_filter() {
    if [[ "$gcc_torture_work" == /tmp/slate-gcc-torture-filter.* ]]; then
        rm -rf -- "$gcc_torture_work"
    fi
}

trap cleanup_gcc_torture_filter EXIT

filter_gcc_torture_case() {
    local source=$1
    local relative=${source#"$gcc_torture_source/"}
    local name=${relative//\//__}
    local destination="$gcc_torture_output/$name"
    local binary="$gcc_torture_work/${name%.c}"
    local status="$gcc_torture_work/status/${name%.c}.tsv"
    local result

    cp "$source" "$destination"
    set +e
    timeout 30 "$gcc_torture_clang" -O0 -std=c23 -o "$binary" "$destination" -lm >/dev/null 2>&1
    result=$?
    set -e
    if [[ $result -ne 0 ]]; then
        rm -f "$destination"
        printf '%s\tcompile-failed\n' "$relative" >"$status"
        return
    fi

    set +e
    timeout "$gcc_torture_timeout" "$binary" >/dev/null 2>&1
    result=$?
    set -e
    if [[ $result -eq 0 ]]; then
        printf '%s\tadmitted\n' "$relative" >"$status"
    elif [[ $result -eq 124 ]]; then
        rm -f "$destination"
        printf '%s\trun-timeout\n' "$relative" >"$status"
    else
        rm -f "$destination"
        printf '%s\trun-failed:%s\n' "$relative" "$result" >"$status"
    fi
}

export gcc_torture_source gcc_torture_output gcc_torture_clang gcc_torture_timeout gcc_torture_work
export -f filter_gcc_torture_case

find "$gcc_torture_source" -type f -name '*.c' -print0 \
    | sort -z \
    | xargs -0 -r -P "$gcc_torture_jobs" -n 1 bash -c 'filter_gcc_torture_case "$1"' _

find "$gcc_torture_work/status" -type f -name '*.tsv' -exec sh -c 'cat "$@"' _ {} + \
    | sort >"$gcc_torture_output/FILTER.tsv"

awk -F '\t' '{ counts[$2]++ } END { for (status in counts) print status, counts[status] }' \
    "$gcc_torture_output/FILTER.tsv" \
    | sort
