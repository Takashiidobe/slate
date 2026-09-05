#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 2 ]]; then
    printf 'usage: %s <gcc.dg dir> <empty output dir>\n' "$0" >&2
    exit 2
fi

source_dir=$(realpath "$1")
output_dir=$(realpath -m "$2")
clang_bin=${SLATE_CLANG:-$HOME/llvm-project/build-cir/bin/clang}
jobs=${SLATE_GCC_DG_FILTER_JOBS:-8}
timeout_seconds=${SLATE_GCC_DG_FILTER_TIMEOUT:-5}
work_dir=$(mktemp -d /tmp/slate-gcc-dg-filter.XXXXXX)

if [[ ! -d "$source_dir" ]]; then
    printf 'missing gcc.dg directory: %s\n' "$source_dir" >&2
    exit 2
fi
if [[ -e "$output_dir" ]] && find "$output_dir" -mindepth 1 -print -quit | grep -q .; then
    printf 'output directory is not empty: %s\n' "$output_dir" >&2
    exit 2
fi

mkdir -p "$output_dir" "$work_dir/status" "$work_dir/bin"
trap 'rm -rf -- "$work_dir"' EXIT

case_flags() {
    local source=$1
    awk '
        /dg-options|dg-additional-options/ {
            if (match($0, /"[^"]*"/)) {
                value = substr($0, RSTART + 1, RLENGTH - 2)
                print value
            } else {
                value = $0
                sub(/^.*dg-(options|additional-options)[[:space:]]*/, "", value)
                gsub(/[{}]/, " ", value)
                print value
            }
        }
    ' "$source" | tr '\n' ' '
}

has_optimization_flags() {
    local source=$1
    case_flags "$source" | grep -Eq '(^|[[:space:]])-O([0-9s]|fast|g|z)?([[:space:]]|$)'
}

has_run_directive() {
    grep -Eq 'dg-do[[:space:]]+run([[:space:]}]|$)' "$1"
}

has_extra_sources() {
    grep -Eq 'dg-(additional-sources|additional-source)' "$1"
}

inline_local_includes() {
    local source=$1
    local line include candidate
    while IFS= read -r line || [[ -n "$line" ]]; do
        if [[ "$line" =~ ^[[:space:]]*#[[:space:]]*include[[:space:]]+\"([^\"]+)\" ]]; then
            include=${BASH_REMATCH[1]}
            candidate=$(realpath -m "$(dirname "$source")/$include")
            if [[ -f "$candidate" ]]; then
                inline_local_includes "$candidate"
                continue
            fi
        fi
        printf '%s\n' "$line"
    done <"$source"
}

run_case() {
    local source=$1
    local relative=${source#"$source_dir/"}
    local name=${relative//\//__}
    local stem=${name%.c}
    local status="$work_dir/status/$stem.tsv"
    local clang_binary="$work_dir/bin/$stem.clang"
    local flags_text
    local -a flags
    local clang_compile clang_run

    if ! has_run_directive "$source"; then
        printf '%s\tskipped-no-run\n' "$relative" >"$status"
        return
    fi
    if has_optimization_flags "$source"; then
        printf '%s\tskipped-optimization\n' "$relative" >"$status"
        return
    fi
    if has_extra_sources "$source"; then
        printf '%s\tskipped-multi-source\n' "$relative" >"$status"
        return
    fi

    flags_text=$(case_flags "$source")
    read -r -a flags <<<"$flags_text"

    set +e
    timeout 30 "$clang_bin" -O0 "${flags[@]}" -o "$clang_binary" "$source" -lm -latomic >/dev/null 2>&1
    clang_compile=$?
    if [[ $clang_compile -ne 0 ]]; then
        printf '%s\tcompile-failed:%s\n' "$relative" "$clang_compile" >"$status"
        set -e
        return
    fi
    timeout "$timeout_seconds" "$clang_binary" >/dev/null 2>&1
    clang_run=$?
    set -e

    if [[ $clang_run -eq 0 ]]; then
        inline_local_includes "$source" >"$output_dir/$name"
        printf '%s\tadmitted:%s\n' "$relative" "$clang_run" >"$status"
    else
        printf '%s\trun-failed:%s\n' "$relative" "$clang_run" >"$status"
    fi
}

export source_dir output_dir clang_bin timeout_seconds work_dir
export -f case_flags has_optimization_flags has_run_directive has_extra_sources inline_local_includes run_case

find "$source_dir" -type f -name '*.c' -print0 | sort -z |
    xargs -0 -r -P "$jobs" -n 1 bash -c 'run_case "$1"' _ 2>/dev/null

find "$work_dir/status" -type f -name '*.tsv' -exec cat {} + | sort >"$output_dir/FILTER.tsv"
awk -F '\t' '{ counts[$2]++ } END { for (status in counts) print status, counts[status] }' \
    "$output_dir/FILTER.tsv" | sort
