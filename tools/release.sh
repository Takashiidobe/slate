#!/usr/bin/env bash
set -euo pipefail

usage() {
  printf 'Usage: %s major|minor|patch\n' "${0##*/}" >&2
  exit 2
}

[[ $# -eq 1 ]] || usage
case "$1" in
  major|minor|patch) bump=$1 ;;
  *) usage ;;
esac

root=$(git rev-parse --show-toplevel)
python3 - "$root" "$bump" <<'PY'
import re
import sys
import tomllib
from pathlib import Path

root = Path(sys.argv[1])
bump = sys.argv[2]
packages = {
    "slate-parser": root / "crates/slate-parser/Cargo.toml",
    "slate-sysroots": root / "crates/slate-sysroots/Cargo.toml",
    "slate-c2rust": root / "crates/slate/Cargo.toml",
}

versions = {
    name: tomllib.loads(path.read_text())["package"]["version"]
    for name, path in packages.items()
}
if len(set(versions.values())) != 1:
    raise SystemExit(f"published package versions differ: {versions}")

version = versions["slate-c2rust"]
match = re.fullmatch(r"(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)", version)
if match is None:
    raise SystemExit(f"expected a stable x.y.z version, got {version!r}")
major, minor, patch = map(int, match.groups())
if bump == "major":
    major, minor, patch = major + 1, 0, 0
elif bump == "minor":
    minor, patch = minor + 1, 0
else:
    patch += 1
next_version = f"{major}.{minor}.{patch}"

def replace_table_value(text, table, pattern, value):
    sections = list(re.finditer(r"(?m)^\[([^\]]+)\]\s*$", text))
    for index, section in enumerate(sections):
        if section.group(1) != table:
            continue
        end = sections[index + 1].start() if index + 1 < len(sections) else len(text)
        body = text[section.end():end]
        updated, count = re.subn(pattern, value, body, count=1, flags=re.MULTILINE)
        if count != 1:
            raise SystemExit(f"could not update {table} in Cargo.toml")
        return text[:section.end()] + updated + text[end:]
    raise SystemExit(f"missing [{table}] in Cargo.toml")

for path in packages.values():
    text = path.read_text()
    text = replace_table_value(
        text,
        "package",
        rf'^(version\s*=\s*)"{re.escape(version)}"\s*$',
        rf'\g<1>"{next_version}"',
    )
    path.write_text(text)

workspace = root / "Cargo.toml"
text = workspace.read_text()
for name in ("slate-parser", "slate-sysroots"):
    text, count = re.subn(
        rf'(?m)^({re.escape(name)}\s*=\s*\{{\s*version\s*=\s*)"{re.escape(version)}"',
        rf'\g<1>"{next_version}"',
        text,
        count=1,
    )
    if count != 1:
        raise SystemExit(f"could not update workspace dependency {name}")
workspace.write_text(text)

print(f"Bumped published packages from {version} to {next_version}")
PY

cd "$root"
cargo metadata --no-deps --format-version 1 >/dev/null
cargo update --workspace
printf 'Updated package manifests and Cargo.lock. Changes are uncommitted.\n'
