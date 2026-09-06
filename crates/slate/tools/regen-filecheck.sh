#!/usr/bin/env bash
set -uo pipefail

root_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
case "${1:-}" in
    lowering|failed-lowering) profile=lowering ;;
    rewrites|failed-rewrites) profile=rewrites ;;
    *)
        printf 'usage: %s {lowering|rewrites}\n' "$0" >&2
        exit 2
        ;;
esac

log_file=$(mktemp)
trap 'rm -f "$log_file"' EXIT

set +e
(cd "$root_dir" && cargo nextest r --release --no-fail-fast --no-capture --profile "$profile") 2>&1 | tee "$log_file"
test_status=${PIPESTATUS[0]}
set -e

# Two check-file shapes reach the log: single-fixture runs write
# .../filecheck/<fixture>/checks-N.txt, project runs (cross-TU and library)
# write .../<fixture>/filecheck/<module>/checks-N.txt, where the fixture is
# the directory holding `filecheck`, not the module under it.
mapfile -t fixture_targets < <(
    grep -oE '[^ "]*/filecheck/[^/]+/checks-[0-9]+\.txt' "$log_file" |
        awk '
            /\/cross-tu\// {
                sub(/\/filecheck\/[^\/]+\/checks-[0-9]+\.txt$/, "")
                sub(/.*\//, "")
                print "project:" $0
                next
            }
            {
                sub(/\/checks-[0-9]+\.txt$/, "")
                sub(/.*\//, "")
                print "fixture:" $0
            }
        ' |
        sort -u
)

if ((${#fixture_targets[@]} == 0)); then
    printf 'no FileCheck failures found for %s\n' "$profile"
    exit "$test_status"
fi

unresolved=()
for fixture_target in "${fixture_targets[@]}"; do
    kind=${fixture_target%%:*}
    fixture_name=${fixture_target#*:}
    if [[ $kind == project ]]; then
        fixture_paths=()
        for group in fixtures.multi fixtures.library; do
            candidate="$root_dir/tests/$group/$fixture_name"
            [[ -d $candidate ]] && fixture_paths+=("$candidate")
        done
    else
        mapfile -t fixture_paths < <(
            rg --files "$root_dir/tests" |
                awk -F/ -v name="${fixture_name}.c" '$NF == name'
        )
    fi
    if ((${#fixture_paths[@]} != 1)); then
        printf 'skipping %s: expected one fixture, found %s\n' \
            "$fixture_name" "${#fixture_paths[@]}" >&2
        unresolved+=("$fixture_name")
        continue
    fi
    printf 'regenerating %s (%s)\n' "${fixture_paths[0]}" "$profile"
    python3 "$root_dir/tools/update_filecheck.py" \
        --profile "$profile" --in-place "${fixture_paths[0]}"
done

if ((${#unresolved[@]} > 0)); then
    printf 'could not regenerate: %s\n' "${unresolved[*]}" >&2
fi

exit "$test_status"
