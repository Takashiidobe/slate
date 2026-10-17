#!/usr/bin/env python3
import argparse
import os
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path
from typing import NamedTuple

CRATE = Path(__file__).resolve().parent.parent
WORKSPACE = CRATE.parent.parent
TESTS = CRATE / "tests"


class Suite(NamedTuple):
    directory: str
    subdirectories: tuple
    work: str
    binary: str
    test: str
    selector: str


SUITES = (
    Suite("fixtures", ("", "x86_64"), "difftest-generated/supported", "differential", "generated_differential", "SLATE_DIFF_FIXTURE"),
    Suite("fixtures", ("i686",), "difftest-generated/i686", "differential", "i686_differential", "SLATE_DIFF_FIXTURE"),
)

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
GENERATED_IDENTIFIER = re.compile(
    r"""\b(?:
        __v[0-9]+ | __t[0-9]+ | _v[0-9]+ | arg[0-9]+
        | anon_struct\w* | anon_[0-9]+ | _unnamed_at_\w+
        | __slate_(?:break|continue|slot|storage|compound|anon)_[0-9]+
        | __slate_alloca_frame[0-9]+ | __state[0-9]+
        | __dispatch[0-9]+(?:_[lv][0-9]+)? | __switch[0-9]+_l[0-9]+
        | __join_[0-9]+ | __SlateRecord[0-9]+
    )\b""",
    re.X,
)
RENAMED_IDENTIFIER = re.compile(r"\b([A-Za-z_]\w*?)_[0-9]+\b")
CANDIDATE_IDENTIFIER = re.compile(f"{GENERATED_IDENTIFIER.pattern}|{RENAMED_IDENTIFIER.pattern}", re.X)
C_IDENTIFIER = re.compile(r"\b[A-Za-z_]\w*\b")
C_COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)


class FixtureError(Exception):
    pass


def fixture_suite(path):
    relative = path.resolve().relative_to(TESTS)
    directory, subdirectory = relative.parts[0], "/".join(relative.parts[1:-1])
    for suite in SUITES:
        if suite.directory == directory and subdirectory in suite.subdirectories:
            return suite
    raise FixtureError(f"{relative}: not in a single-file suite")


def generated_rust(suite, path):
    return CRATE / "target" / suite.work / f"{path.stem}.generated.rs"


def runtime_ok(rust):
    return rust.with_name(rust.name.removesuffix(".generated.rs") + ".runtime-ok")


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


class Variables:
    def __init__(self, c_identifiers):
        self.c_identifiers = c_identifiers
        self.names = {}
        self.counts = defaultdict(int)

    def unstable(self, identifier):
        if identifier in self.c_identifiers:
            return False
        if GENERATED_IDENTIFIER.fullmatch(identifier):
            return True
        renamed = RENAMED_IDENTIFIER.fullmatch(identifier)
        return bool(renamed) and renamed.group(1) in self.c_identifiers

    def substitute(self, match):
        identifier = match.group(0)
        if not self.unstable(identifier):
            return identifier
        if identifier in self.names:
            return f"[[{self.names[identifier]}]]"
        base = re.sub(r"\W", "_", identifier.rstrip("0123456789").strip("_")).upper() or "ID"
        name = f"{base}_{self.counts[base]}"
        self.counts[base] += 1
        self.names[identifier] = name
        return f"[[{name}:{re.sub(r'[0-9]+', '[0-9]+', identifier)}]]"

    def anonymous(self, match):
        identifier = match.group(0)
        if not self.unstable(identifier):
            return identifier
        return "{{" + re.sub(r"[0-9]+", "[0-9]+", identifier) + "}}"

    def pattern(self, line, label=False):
        substitute = self.anonymous if label else self.substitute
        parts = []
        offset = 0
        for string in RUST_STRING_PATTERN.finditer(line):
            parts.append(CANDIDATE_IDENTIFIER.sub(substitute, escape(line[offset : string.start()])))
            parts.append(escape(string.group()))
            offset = string.end()
        parts.append(CANDIDATE_IDENTIFIER.sub(substitute, escape(line[offset:])))
        return "".join(parts).replace(f"{CRATE}{os.sep}", "{{.*}}")


def c_identifiers(source):
    return set(C_IDENTIFIER.findall(C_COMMENT.sub(" ", source)))


def body_checks(lines, variables):
    return [
        f"// CHECK-NEXT: {variables.pattern(line)}" if line.strip() else "// CHECK-EMPTY:"
        for line in lines
    ]


def function_checks(lines, identifiers):
    variables = Variables(identifiers)
    if len(lines) == 1:
        return [f"// CHECK: {{{{^}}}}{variables.pattern(lines[0])}"]
    checks = [f"// CHECK-LABEL: {{{{^}}}}{variables.pattern(lines[0], label=True)}"]
    checks.extend(body_checks(lines[1:-1], variables))
    checks.append("// CHECK: {{^}}}")
    return checks


def file_checks(lines, identifiers):
    variables = Variables(identifiers)
    first, *rest = lines
    return [f"// CHECK: {{{{^}}}}{variables.pattern(first)}", *body_checks(rest, variables), "// CHECK-NOT: {{.}}"]


def update_source(source, rust):
    names = checked_functions(source)
    stripped = BLOCK_PATTERN.sub("", source).rstrip("\n")
    identifiers = c_identifiers(stripped)
    lines = rust.rstrip("\n").splitlines()
    if names:
        checks = [check for name in names for check in function_checks(rust_function(lines, name), identifiers)]
    else:
        checks = file_checks(lines, identifiers)
    return f"{stripped}\n\n{BEGIN}\n" + "\n".join(checks) + f"\n{END}\n"


def run_harness(fixtures_by_suite):
    environment = os.environ.copy()
    names_by_selector = defaultdict(set)
    for suite, fixtures in fixtures_by_suite.items():
        names_by_selector[suite.selector].update(path.stem for path in fixtures)
    for selector, names in names_by_selector.items():
        if len(names) == 1:
            environment[selector] = next(iter(names))
    command = ["cargo", "nextest", "r", "--release", "--profile", "slate", "--no-fail-fast"]
    for binary in sorted({suite.binary for suite in fixtures_by_suite}):
        command.extend(["--test", binary])
    command.extend(["-E", " | ".join(sorted(f"test(={suite.test})" for suite in fixtures_by_suite))])
    subprocess.run(command, cwd=WORKSPACE, env=environment, capture_output=True, check=False)


def suite_fixtures():
    return sorted(
        path
        for suite in SUITES
        for subdirectory in suite.subdirectories
        for path in (TESTS / suite.directory / subdirectory).glob("*.c")
    )


def resolve_fixture(argument):
    path = Path(argument)
    if path.suffix == ".c" or path.exists():
        return path.resolve()
    matches = [fixture for fixture in suite_fixtures() if fixture.stem == argument]
    if len(matches) != 1:
        found = ", ".join(str(match.relative_to(TESTS)) for match in matches) or "no fixture"
        raise FixtureError(f"{argument}: name matches {found}; pass a path")
    return matches[0]


def main(argv):
    parser = argparse.ArgumentParser(
        description="Regenerate the FileCheck blocks of tests/fixtures fixtures: the whole generated Rust, or only the @check-fn-wrapped functions."
    )
    parser.add_argument(
        "fixtures",
        nargs="*",
        help="fixture paths, or names as a failing test run lists them; default: every fixture",
    )
    parser.add_argument(
        "--no-run",
        action="store_true",
        help="reuse the Rust the last test run wrote instead of rerunning the harness",
    )
    arguments = parser.parse_args(argv)
    generated = {}
    fixtures_by_suite = defaultdict(list)
    failures = 0
    for argument in arguments.fixtures or suite_fixtures():
        try:
            path = resolve_fixture(argument)
            suite = fixture_suite(path)
        except FixtureError as error:
            print(f"error: {error}", file=sys.stderr)
            failures += 1
            continue
        generated[path] = generated_rust(suite, path)
        fixtures_by_suite[suite].append(path)
    if not arguments.no_run and generated:
        for rust in generated.values():
            rust.unlink(missing_ok=True)
            runtime_ok(rust).unlink(missing_ok=True)
        run_harness(fixtures_by_suite)
    for path, rust in generated.items():
        name = path.relative_to(TESTS)
        if not rust.is_file():
            print(f"error: {name}: translation failed; no {rust.name}", file=sys.stderr)
            failures += 1
            continue
        if not runtime_ok(rust).is_file():
            print(f"error: {name}: Rust did not build or did not match C; checks left unchanged", file=sys.stderr)
            failures += 1
            continue
        try:
            source = path.read_text(errors="surrogateescape")
            updated = update_source(source, rust.read_text())
        except FixtureError as error:
            print(f"error: {name}: {error}", file=sys.stderr)
            failures += 1
            continue
        if updated != source:
            path.write_text(updated, errors="surrogateescape")
            print(f"updated {name}")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
