#!/usr/bin/env python3
import argparse
import os
import re
import subprocess
import sys
from pathlib import Path

CRATE = Path(__file__).resolve().parent.parent
WORKSPACE = CRATE.parent.parent
TESTS = CRATE / "tests"
GENERATED = CRATE / "target" / "difftest-generated"

BEGIN = "// SLATE-FILECHECK-BEGIN"
END = "// SLATE-FILECHECK-END"
BLOCK_PATTERN = re.compile(
    rf"(?ms)^{re.escape(BEGIN)}[^\n]*\n.*?^{re.escape(END)}[^\n]*(?:\n|$)"
)
MARKER_PATTERN = re.compile(r"^\s*//\s*@check-fn-(begin|end)\s*$")
STALE_MARKER_PATTERN = re.compile(r"^\s*//\s*@[a-z-]+-(begin|end)\s*$")
C_DECLARATOR_KEYWORDS = frozenset(
    (
        "if",
        "while",
        "for",
        "switch",
        "return",
        "sizeof",
        "typeof",
        "__typeof__",
        "__extension__",
        "_Alignas",
        "alignof",
        "_Alignof",
        "static_assert",
        "_Static_assert",
    )
)
C_ATTRIBUTE_PATTERN = re.compile(r"\b(?:__attribute__|__declspec|__asm__|asm)\s*\(")
RUST_FN_PREFIX = r'^(?:pub(?:\([^)]*\))?\s+)?(?:unsafe\s+)?(?:extern\s+"[^"]+"\s+)?fn\s+'
RUST_STRING_PATTERN = re.compile(
    r'(?:(?:b|c)?r(?P<hash>#{0,16})".*?"(?P=hash)|(?:b|c)?"(?:\\.|[^"\\])*")'
)
UNSTABLE_IDENTIFIERS = (
    (re.compile(r"\b__v[0-9]+\b"), "{{__v[0-9]+}}"),
    (re.compile(r"\barg[0-9]+\b"), "{{arg[0-9]+}}"),
    (re.compile(r"\banon_struct[0-9A-Za-z_]*\b"), "{{anon_struct[0-9A-Za-z_]*}}"),
    (re.compile(r"(?<![0-9A-Za-z_])anon_[0-9]+\b"), "{{anon_[0-9]+}}"),
    (re.compile(r"\b(__slate_[A-Za-z_]*?_)[0-9]+\b"), r"\1{{[0-9]+}}"),
)


class FixtureError(Exception):
    pass


def harness_location(path):
    relative = path.resolve().relative_to(TESTS)
    bucket, *rest = relative.parts
    if bucket == "fixtures" and rest[:-1] in ([], ["x86_64"]):
        return "supported", "generated_differential"
    if bucket == "fixtures" and rest[:-1] == ["i686"]:
        return "i686", "i686_differential"
    if bucket == "fixtures.unsupported":
        return "unsupported", "fixtures_unsupported_tests_still_fail"
    raise FixtureError(f"{relative}: only single-file differential fixtures are supported")


def strip_attribute_groups(text):
    text = re.sub(r"\[\[.*?\]\]", " ", text, flags=re.DOTALL)
    while match := C_ATTRIBUTE_PATTERN.search(text):
        depth = 0
        for index in range(match.end() - 1, len(text)):
            if text[index] == "(":
                depth += 1
            elif text[index] == ")":
                depth -= 1
                if depth == 0:
                    text = text[: match.start()] + " " + text[index + 1 :]
                    break
        else:
            return text
    return text


def c_function_name(lines):
    body = [line for line in lines if not line.lstrip().startswith("#")]
    head = strip_attribute_groups("\n".join(body).split("{", 1)[0])
    for match in re.finditer(r"\b([^\W\d]\w*)\s*\(", head):
        if match.group(1) not in C_DECLARATOR_KEYWORDS:
            return match.group(1)
    return None


def checked_functions(source):
    names = []
    start = None
    lines = source.splitlines()
    for number, line in enumerate(lines, 1):
        match = MARKER_PATTERN.match(line)
        if not match:
            if STALE_MARKER_PATTERN.match(line):
                raise FixtureError(f"line {number}: unsupported marker `{line.strip()}`")
            continue
        if match.group(1) == "begin":
            if start is not None:
                raise FixtureError(f"line {number}: @check-fn-begin is already open")
            start = number
            continue
        if start is None:
            raise FixtureError(f"line {number}: unmatched @check-fn-end")
        name = c_function_name(lines[start : number - 1])
        if name is None:
            raise FixtureError(f"line {start}: @check-fn-begin does not wrap a function")
        names.append(name)
        start = None
    if start is not None:
        raise FixtureError(f"line {start}: unclosed @check-fn-begin")
    return names


def rust_function(lines, c_name):
    rust_name = "__slate_main" if c_name == "main" else c_name
    pattern = re.compile(RUST_FN_PREFIX + re.escape(rust_name) + r"\s*[(<]")
    for index, line in enumerate(lines):
        if not pattern.match(line):
            continue
        if line.endswith("{}"):
            return lines[index : index + 1]
        for end in range(index + 1, len(lines)):
            if lines[end] == "}":
                return lines[index : end + 1]
        break
    raise FixtureError(f"generated Rust has no top-level fn {rust_name}")


def escape(text):
    return text.replace("{{", "{{[{][{]}}").replace("[[", "{{\\[\\[}}")


def check_pattern(line):
    parts = []
    offset = 0
    for string in RUST_STRING_PATTERN.finditer(line):
        parts.append(genericize(escape(line[offset : string.start()])))
        parts.append(escape(string.group()))
        offset = string.end()
    parts.append(genericize(escape(line[offset:])))
    return "".join(parts)


def genericize(pattern):
    for identifier, replacement in UNSTABLE_IDENTIFIERS:
        pattern = identifier.sub(replacement, pattern)
    return pattern


def function_checks(lines):
    if len(lines) == 1:
        return [f"// CHECK: {{{{^}}}}{check_pattern(lines[0])}"]
    checks = [f"// CHECK-LABEL: {{{{^}}}}{check_pattern(lines[0])}"]
    for line in lines[1:-1]:
        checks.append(f"// CHECK-NEXT: {check_pattern(line)}" if line.strip() else "// CHECK-EMPTY:")
    checks.append("// CHECK: {{^}}}")
    return checks


def update_source(source, rust):
    names = checked_functions(source)
    stripped = BLOCK_PATTERN.sub("", source).rstrip("\n")
    if not names:
        return stripped + "\n"
    lines = rust.splitlines()
    checks = []
    for name in names:
        checks.extend(function_checks(rust_function(lines, name)))
    return f"{stripped}\n\n{BEGIN}\n" + "\n".join(checks) + f"\n{END}\n"


def run_harness(fixtures):
    tests = sorted({harness_location(path)[1] for path in fixtures})
    environment = os.environ.copy()
    if len(fixtures) == 1:
        environment["SLATE_DIFF_FIXTURE"] = fixtures[0].stem
    command = [
        "cargo",
        "nextest",
        "r",
        "--release",
        "--profile",
        "slate",
        "--no-fail-fast",
        "--test",
        "differential",
        "-E",
        " | ".join(f"test(={test})" for test in tests),
    ]
    subprocess.run(command, cwd=WORKSPACE, env=environment, capture_output=True, check=False)


def annotated_fixtures():
    roots = [TESTS / "fixtures", TESTS / "fixtures" / "x86_64", TESTS / "fixtures" / "i686"]
    return sorted(
        path
        for root in roots
        for path in root.glob("*.c")
        if "@check-fn-" in path.read_text() or BEGIN in path.read_text()
    )


def main(argv):
    parser = argparse.ArgumentParser(
        description="Regenerate CHECK blocks for @check-fn-annotated differential fixtures."
    )
    parser.add_argument("fixtures", nargs="*", type=Path)
    parser.add_argument(
        "--no-run",
        action="store_true",
        help="reuse the Rust the last test run wrote instead of rerunning the harness",
    )
    arguments = parser.parse_args(argv)
    fixtures = [path.resolve() for path in arguments.fixtures] or annotated_fixtures()
    generated = {}
    failures = 0
    for path in fixtures:
        try:
            group, _ = harness_location(path)
        except FixtureError as error:
            print(f"error: {error}", file=sys.stderr)
            failures += 1
            continue
        generated[path] = GENERATED / group / f"{path.stem}.generated.rs"
    if not arguments.no_run and generated:
        for rust in generated.values():
            rust.unlink(missing_ok=True)
        run_harness(list(generated))
    for path, rust in generated.items():
        name = path.relative_to(TESTS)
        if not rust.is_file():
            print(f"error: {name}: translation failed; no {rust.name}", file=sys.stderr)
            failures += 1
            continue
        try:
            source = path.read_text()
            updated = update_source(source, rust.read_text())
        except FixtureError as error:
            print(f"error: {name}: {error}", file=sys.stderr)
            failures += 1
            continue
        if updated != source:
            path.write_text(updated)
            print(f"updated {name}")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
