#!/usr/bin/env python3
import argparse
import difflib
import os
import re
import subprocess
import tempfile
from pathlib import Path


def _no_color_env() -> dict:
    env = os.environ.copy()
    env.pop("FORCE_COLOR", None)
    env.pop("CLICOLOR_FORCE", None)
    env["NO_COLOR"] = "1"
    return env


DEFINE_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-DEFINES\s+([A-Za-z0-9_-]+)(?:\s+(.*))?$")
BEGIN_RE = re.compile(r"^// SLATE-FILECHECK-BEGIN ([A-Za-z0-9_-]+)$")
ERROR_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-ERROR\s+([A-Za-z0-9_-]+)$")
ISYSTEM_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-ISYSTEM\s+(.*)$")
FLAVOR_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-FLAVOR\s+(\S+)\s*$")
STD_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-STD\s+([A-Za-z0-9_-]+)\s+(\S+)\s*$")
QUOTED_C_INCLUDE_RE = re.compile(r'^\s*#\s*include\s*"([^"/]+\.c)"', re.MULTILINE)


def isystem_paths(source: str) -> list[str]:
    paths = []
    for line in source.splitlines():
        match = ISYSTEM_RE.match(line)
        if match:
            paths.extend(os.path.expanduser(path) for path in match.group(1).split())
    return paths


def flavor_args(source: str) -> list[str]:
    for line in source.splitlines():
        match = FLAVOR_RE.match(line)
        if match:
            return [f"--flavor={match.group(1)}"]
    return []


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


def configuration_defines(source: str, prefix: str) -> list[str]:
    for line in source.splitlines():
        match = DEFINE_RE.match(line)
        if match and match.group(1) == prefix:
            return [item for item in (match.group(2) or "").split() if item]
    return []


def configuration_std_args(source: str, prefix: str) -> list[str]:
    for line in source.splitlines():
        match = STD_RE.match(line)
        if match and match.group(1) == prefix:
            return [f"-std={match.group(2)}"]
    return []


def error_configurations(source: str) -> list[str]:
    return [match.group(1) for line in source.splitlines() if (match := ERROR_RE.match(line))]


def fixture_source(source: str) -> str:
    kept = []
    in_checks = False
    for line in source.splitlines():
        if BEGIN_RE.match(line):
            in_checks = True
        if not in_checks and not line.startswith("// SLATE-FILECHECK-"):
            kept.append(line)
        if line.startswith("// SLATE-FILECHECK-END "):
            in_checks = False
    return "\n".join(kept) + "\n"


def write_isolated_fixture(directory: Path, fixture: Path, source: str) -> Path:
    c_sources = {fixture.name, *QUOTED_C_INCLUDE_RE.findall(fixture_source(source))}
    for sibling in fixture.parent.iterdir():
        destination = directory / sibling.name
        if sibling.name in c_sources:
            destination.write_text(
                fixture_source(source if sibling == fixture else sibling.read_text(errors="surrogateescape")),
                errors="surrogateescape",
            )
        else:
            destination.symlink_to(sibling, target_is_directory=sibling.is_dir())
    return directory / fixture.name


def render(
    repo: Path, fixture: Path, source: str, defines: list[str], isystem: list[str], std_args: list[str]
) -> str:
    with tempfile.TemporaryDirectory(prefix=f".{fixture.stem}.filecheck.") as directory:
        parsed_fixture = write_isolated_fixture(Path(directory), fixture, source)
        command = ["cargo", "run", "--quiet", "--", "parse", str(parsed_fixture)]
        command.extend(f"-D{define.removeprefix('-D')}" for define in defines)
        command.extend(f"-isystem{path}" for path in isystem)
        command.extend(flavor_args(source))
        command.extend(std_args)
        result = subprocess.run(command, cwd=repo, text=True, capture_output=True, env=_no_color_env())
    if result.returncode:
        raise RuntimeError(result.stderr or result.stdout)
    return result.stdout.rstrip("\n")


def render_error(
    repo: Path,
    fixture: Path,
    source: str,
    defines: list[str],
    isystem: list[str],
    std_args: list[str],
) -> list[str]:
    parsed_name = f".{fixture.stem}.filecheck.{os.getpid()}.0.c"
    parsed_fixture = fixture.with_name(parsed_name)
    parsed_fixture.write_text(fixture_source(source), errors="surrogateescape")
    try:
        command = ["cargo", "run", "--quiet", "--", "parse", str(parsed_fixture)]
        command.extend(f"-D{define.removeprefix('-D')}" for define in defines)
        command.extend(f"-isystem{path}" for path in isystem)
        command.extend(flavor_args(source))
        command.extend(std_args)
        result = subprocess.run(command, cwd=repo, text=True, capture_output=True, env=_no_color_env())
    finally:
        parsed_fixture.unlink()
    if result.returncode == 0:
        raise RuntimeError(f"expected {fixture} to fail parsing")
    return [
        line.strip().replace(parsed_name, fixture.name)
        for line in result.stderr.splitlines()
        if line.startswith("Error:")
        or re.match(r"^\s*(?:\d+ │|×|⚠|╭─|·|╰─)", line)
    ]


FILECHECK_LITERAL_RE = re.compile(r"\{\{|\}\}|\[\[")
TEMP_FILECHECK_PATH_RE = re.compile(r'(["])[^"]*\.filecheck\.[^"]*(["])')
FILECHECK_LITERAL_ESCAPES = {
    "{{": "{{\\{\\{}}",
    "}}": "{{[}][}]}}",
    "[[": "{{\\[\\[}}",
}


def escape_filecheck_literal(line: str) -> str:
    line = FILECHECK_LITERAL_RE.sub(
        lambda match: FILECHECK_LITERAL_ESCAPES[match.group(0)], line
    )
    return TEMP_FILECHECK_PATH_RE.sub(r"\1{{.*}}\2", line)


CODE_UNITS_OPEN_RE = re.compile(r"^(\s*)code_units: \[$")
PIECES_OPEN_RE = re.compile(r"^(\s*)pieces: \[$")
QUOTED_LINE_RE = re.compile(r'^\s*"((?:[^"\\]|\\.)*)",?$')


def redact_code_units(lines: list[str]) -> list[tuple[str, bool, str | None]]:
    """Loosen a code_units list into a forward scan when its decoded text (the
    following pieces block) embeds the per-run temp fixture path: that path's
    byte count varies run to run (e.g. pid digit count), so neither an exact
    per-element CHECK-NEXT chain nor a single collapsed line can match it."""
    result: list[tuple[str, bool, str | None]] = []
    i = 0
    n = len(lines)
    while i < n:
        open_match = CODE_UNITS_OPEN_RE.match(lines[i])
        if not open_match:
            result.append((lines[i], True, None))
            i += 1
            continue
        indent = open_match.group(1)
        start = i
        close = i + 1
        while close < n and lines[close] != f"{indent}],":
            close += 1
        if close == n:
            result.extend((line, True, None) for line in lines[start:])
            break
        tainted = False
        pieces_match = close + 1 < n and PIECES_OPEN_RE.match(lines[close + 1])
        if pieces_match and pieces_match.group(1) == indent:
            j = close + 2
            while j < n and lines[j] != f"{indent}],":
                quoted = QUOTED_LINE_RE.match(lines[j])
                if quoted and ".filecheck." in quoted.group(1):
                    tainted = True
                j += 1
        if tainted:
            result.append((lines[start], True, None))
            result.append((f"{indent}],", True, "plain"))
        else:
            result.extend((line, True, None) for line in lines[start : close + 1])
        i = close + 1
    return result


def generated_blocks(repo: Path, fixture: Path, source: str) -> str:
    isystem = isystem_paths(source)
    blocks = []
    for prefix in error_configurations(source):
        output = render_error(
            repo,
            fixture,
            source,
            configuration_defines(source, prefix),
            isystem,
            configuration_std_args(source, prefix),
        )
        block = [f"// SLATE-FILECHECK-BEGIN {prefix}"]
        block.extend(f"// {prefix}: {escape_filecheck_literal(line)}" for line in output)
        block.append(f"// SLATE-FILECHECK-END {prefix}")
        blocks.extend(block)
    if error_configurations(source):
        return "\n".join(blocks)
    for prefix, defines in configurations(source):
        output = render(repo, fixture, source, defines, isystem, configuration_std_args(source, prefix))
        lines = redact_code_units(output.splitlines())
        block = [f"// SLATE-FILECHECK-BEGIN {prefix}"]
        for index, (line, escape, force) in enumerate(lines):
            directive = prefix if force == "plain" or index == 0 else f"{prefix}-NEXT"
            text = escape_filecheck_literal(line) if escape else line
            block.append(f"// {directive}: {text}")
        block.append(f"// SLATE-FILECHECK-END {prefix}")
        blocks.extend(block)
    return "\n".join(blocks)


def replace_blocks(source: str, generated: str, prefixes: list[str]) -> str:
    check_re = re.compile(
        r"^// (" + "|".join(re.escape(prefix) for prefix in prefixes) + r")(?:-NEXT)?:"
    ) if prefixes else None
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
        if check_re and check_re.match(lines[index]):
            index += 1
            continue
        kept.append(lines[index])
        index += 1
    while kept and not kept[-1].strip():
        kept.pop()
    return "\n".join(kept) + "\n\n" + generated + "\n"


def update(repo: Path, fixture: Path) -> tuple[str, str]:
    source = fixture.read_text(errors="surrogateescape")
    prefixes = error_configurations(source) or [prefix for prefix, _ in configurations(source)]
    updated = replace_blocks(source, generated_blocks(repo, fixture, source), prefixes)
    return source, updated


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--in-place", action="store_true")
    parser.add_argument("fixtures", nargs="*", type=Path)
    args = parser.parse_args()
    repo = Path(__file__).resolve().parent.parent
    fixtures = args.fixtures or sorted((repo / "tests/fixtures").rglob("*.c"))
    changed = False
    for fixture_arg in fixtures:
        fixture = (Path.cwd() / fixture_arg).resolve() if not fixture_arg.is_absolute() else fixture_arg
        source, updated = update(repo, fixture)
        if source == updated:
            continue
        changed = True
        if args.in_place:
            fixture.write_text(updated, errors="surrogateescape")
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
