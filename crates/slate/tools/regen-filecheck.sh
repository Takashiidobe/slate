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

mapfile -t fixture_names < <(
    sed -nE 's#.*filecheck/([^/]+)/checks-[0-9]+\.txt.*#\1#p' "$log_file" |
        sort -u
)

if ((${#fixture_names[@]} == 0)); then
    printf 'no FileCheck failures found for %s\n' "$profile"
    exit "$test_status"
fi

for fixture_name in "${fixture_names[@]}"; do
    mapfile -t fixture_paths < <(
        rg --files "$root_dir/tests" |
            awk -F/ -v name="${fixture_name}.c" '$NF == name'
    )
    if ((${#fixture_paths[@]} != 1)); then
        printf 'skipping %s: expected one fixture, found %s\n' \
            "$fixture_name" "${#fixture_paths[@]}" >&2
        continue
    fi
    printf 'regenerating %s (%s)\n' "${fixture_paths[0]}" "$profile"
    python3 "$root_dir/tools/update_filecheck.py" \
        --profile "$profile" --in-place "${fixture_paths[0]}"
done

exit "$test_status"
