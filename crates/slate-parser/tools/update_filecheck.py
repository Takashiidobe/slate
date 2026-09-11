#!/usr/bin/env python3
import argparse
import difflib
import re
import subprocess
from pathlib import Path


DEFINE_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-DEFINES\s+([A-Za-z0-9_-]+)(?:\s+(.*))?$")
BEGIN_RE = re.compile(r"^// SLATE-FILECHECK-BEGIN ([A-Za-z0-9_-]+)$")
CHECK_RE = re.compile(r"^// ([A-Za-z0-9_-]+)(?:-NEXT)?:")


def configurations(source: str) -> list[tuple[str, list[str]]]:
    found = []
    for line in source.splitlines():
        match = DEFINE_RE.match(line)
        if match:
            defines = [item for item in (match.group(2) or "").split() if item]
            found.append((match.group(1), defines))
    if not found:
        raise ValueError("fixture has no SLATE-FILECHECK-DEFINES directives")
    return found


def render(repo: Path, fixture: Path, defines: list[str]) -> str:
    command = ["cargo", "run", "--quiet", "--", "filecheck", str(fixture)]
    command.extend(f"-D{define.removeprefix('-D')}" for define in defines)
    result = subprocess.run(command, cwd=repo, text=True, capture_output=True)
    if result.returncode:
        raise RuntimeError(result.stderr or result.stdout)
    return result.stdout.rstrip("\n")


def generated_blocks(repo: Path, fixture: Path, source: str) -> str:
    blocks = []
    for prefix, defines in configurations(source):
        output = render(repo, fixture, defines)
        lines = output.splitlines()
        block = [f"// SLATE-FILECHECK-BEGIN {prefix}"]
        for index, line in enumerate(lines):
            directive = prefix if index == 0 else f"{prefix}-NEXT"
            block.append(f"// {directive}: {line}")
        block.append(f"// SLATE-FILECHECK-END {prefix}")
        blocks.extend(block)
    return "\n".join(blocks)


def replace_blocks(source: str, generated: str) -> str:
    lines = source.splitlines()
    kept = []
    index = 0
    while index < len(lines):
        begin = BEGIN_RE.match(lines[index])
        if begin:
            prefix = begin.group(1)
            index += 1
            while index < len(lines) and lines[index] != f"// SLATE-FILECHECK-END {prefix}":
                index += 1
            if index == len(lines):
                raise ValueError(f"unterminated FileCheck block for {prefix}")
            index += 1
            continue
        if CHECK_RE.match(lines[index]):
            index += 1
            continue
        kept.append(lines[index])
        index += 1
    while kept and not kept[-1].strip():
        kept.pop()
    return "\n".join(kept) + "\n\n" + generated + "\n"


def update(repo: Path, fixture: Path) -> tuple[str, str]:
    source = fixture.read_text()
    updated = replace_blocks(source, generated_blocks(repo, fixture, source))
    return source, updated


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--in-place", action="store_true")
    parser.add_argument("fixtures", nargs="*", type=Path)
    args = parser.parse_args()
    repo = Path(__file__).resolve().parent.parent
    fixtures = args.fixtures or sorted((repo / "tests/fixtures").glob("*.c"))
    changed = False
    for fixture_arg in fixtures:
        fixture = (Path.cwd() / fixture_arg).resolve() if not fixture_arg.is_absolute() else fixture_arg
        source, updated = update(repo, fixture)
        if source == updated:
            continue
        changed = True
        if args.in_place:
            fixture.write_text(updated)
        else:
            print("".join(difflib.unified_diff(
                source.splitlines(keepends=True),
                updated.splitlines(keepends=True),
                fromfile=str(fixture),
                tofile=str(fixture),
            )), end="")
    return 1 if changed and not args.in_place else 0


if __name__ == "__main__":
    raise SystemExit(main())
