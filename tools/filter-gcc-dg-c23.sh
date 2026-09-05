#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 2 ]]; then
    printf 'usage: %s <gcc.dg dir> <empty output dir>\n' "$0" >&2
    exit 2
fi

gcc_dg_source=$(realpath "$1")
gcc_dg_output=$(realpath -m "$2")
script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

if [[ ! -d "$gcc_dg_source" ]]; then
    printf 'missing gcc.dg directory: %s\n' "$gcc_dg_source" >&2
    exit 2
fi

staging=$(mktemp -d /tmp/slate-gcc-dg-c23-staging.XXXXXX)
trap 'rm -rf -- "$staging"' EXIT

# Only c23-*.c cases declared `dg-do run` are candidates: the rest of
# gcc.dg/c23-*.c is `dg-do compile` (checks accept/reject diagnostics, not
# runtime behavior) and has nothing to differential-test. `-aux.c` helper
# files and cases with `dg-additional-sources` are multi-TU tests; copying
# them alone (matching filter-gcc-torture.sh's single-file convention) makes
# them fail to link, which the compile-and-run gate below already rejects.
find "$gcc_dg_source" -maxdepth 1 -name 'c23-*.c' -print0 \
    | xargs -0 grep -l 'dg-do run' \
    | xargs -I{} cp {} "$staging/"

find "$staging" -maxdepth 1 -name 'c23-*.c' | wc -l | xargs printf 'candidates: %s\n' >&2

"$script_dir/filter-gcc-torture.sh" "$staging" "$gcc_dg_output"
