#!/usr/bin/env python3
import argparse
import difflib
import glob
import os
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

CRATE = Path(__file__).resolve().parent.parent
WORKSPACE = CRATE.parent.parent
TESTS = CRATE / "tests"
FIXTURES = TESTS / "fixtures"
RELEASE = TESTS / "fixtures.release"
PLAN = TESTS / "fixtures.plan"
LOWERED_NAMES = TESTS / "fixtures.lowered-names"
PLAN_SUITES = {PLAN: "plan", LOWERED_NAMES: "lowered-names"}
WORK = CRATE / "target" / "difftest-generated"

DEFAULT_TARGET = "x86_64-unknown-linux-gnu"
ARCH_TARGETS = {
    "x86_64": "x86_64-unknown-linux-gnu",
    "i686": "i686-unknown-linux-gnu",
    "aarch64": "aarch64-unknown-linux-gnu",
    "arm": "armv7-unknown-linux-gnueabihf",
}
WINDOWS_SUFFIX = "-pc-windows-msvc"
TEST_COMMAND = ["--test", "differential", "-E", "test(=generated_differential)"]
RELEASE_TEST_COMMAND = ["--test", "differential", "-E", "test(=release_build_differential)"]
PLAN_TEST_COMMAND = ["--test", "project_plan", "-E", "test(=plan_filecheck)"]
LOWERED_NAMES_TEST_COMMAND = ["--test", "lowered_names", "-E", "test(=lowered_names_filecheck)"]

BEGIN = "// SLATE-FILECHECK-BEGIN"
END = "// SLATE-FILECHECK-END"
BLOCK_PATTERN = re.compile(
    rf"(?ms)^{re.escape(BEGIN)}[^\n]*\n.*?^{re.escape(END)}[^\n]*(?:\n|$)"
)
HEADER_PATTERN = re.compile(rf"(?m)^{re.escape(BEGIN)}([^\n]*)$")
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
        | __join_[0-9]+
    )\b""",
    re.X,
)
RENAMED_IDENTIFIER = re.compile(r"\b([A-Za-z_]\w*?)_[0-9]+\b")
CANDIDATE_IDENTIFIER = re.compile(f"{GENERATED_IDENTIFIER.pattern}|{RENAMED_IDENTIFIER.pattern}", re.X)
C_IDENTIFIER = re.compile(r"\b[A-Za-z_]\w*\b")
C_COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)


class FixtureError(Exception):
    pass


def run_target(target):
    if target.endswith(WINDOWS_SUFFIX):
        return ARCH_TARGETS[target.removesuffix(WINDOWS_SUFFIX)]
    return target


def is_msvc(path):
    return not is_release(path) and not is_plan(path) and path.resolve().relative_to(FIXTURES).parts[0] == "msvc"


def fixture_arch(path):
    if is_release(path) or is_plan(path):
        return None
    relative = path.resolve().relative_to(FIXTURES)
    if relative.parts[0] in ("gcc", "msvc"):
        relative = relative.relative_to(relative.parts[0])
    if len(relative.parts) == 1:
        return None
    if len(relative.parts) == 2 and relative.parts[0] in ARCH_TARGETS:
        return relative.parts[0]
    raise FixtureError(f"{path}: not a tests/fixtures fixture")


def check_target(path, target):
    arch = fixture_arch(path)
    if arch is not None and target.split("-", 1)[0] != ARCH_TARGETS[arch].split("-", 1)[0]:
        raise FixtureError(f"{path.relative_to(TESTS)}: {arch} fixtures only run for {arch} targets, not {target}")


def is_release(path):
    return RELEASE in path.resolve().parents


def is_plan(path):
    return path.resolve().parent in PLAN_SUITES


def release_project(path):
    return path.stem if path.resolve().parent == RELEASE else path.resolve().parent.name


def generated_rust(target, path):
    if is_plan(path):
        return WORK / PLAN_SUITES[path.resolve().parent] / target / f"{path.stem}.generated.rs"
    if is_release(path):
        src = WORK / "release" / release_project(path) / "src"
        unit = src / f"{'__slate_unit_main' if path.stem == 'main' else path.stem}.rs"
        return unit if unit.is_file() else src / "main.rs"
    if target.endswith(WINDOWS_SUFFIX):
        return WORK / f"{run_target(target)}-windows" / f"{path.stem}.generated.rs"
    return WORK / target / f"{path.stem}.generated.rs"


def runtime_ok(rust):
    if rust.parent.name == "src":
        return rust.parent.parent / "runtime-ok"
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


def block_targets(source):
    match = HEADER_PATTERN.search(source)
    return match.group(1).split() if match else []


def rust_function(lines, c_name, target):
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
    raise FixtureError(f"generated Rust for {target} has no top-level fn {rust_name}")


def escape(text):
    return text.replace("{{", "{{[{][{]}}").replace("[[", "{{\\[\\[}}")


class Variables:
    def __init__(self, c_identifiers):
        self.c_identifiers = c_identifiers
        self.names = {}
        self.counts = defaultdict(int)
        self.defined = defaultdict(set)

    def unstable(self, identifier):
        if identifier in self.c_identifiers:
            return False
        if GENERATED_IDENTIFIER.fullmatch(identifier):
            return True
        renamed = RENAMED_IDENTIFIER.fullmatch(identifier)
        return bool(renamed) and renamed.group(1) in self.c_identifiers

    def name(self, identifier):
        if identifier not in self.names:
            base = re.sub(r"\W", "_", identifier.rstrip("0123456789").strip("_")).upper() or "ID"
            self.names[identifier] = f"{base}_{self.counts[base]}"
            self.counts[base] += 1
        return self.names[identifier]

    def substitute(self, lane):
        def replace(match):
            identifier = match.group(0)
            if not self.unstable(identifier):
                return identifier
            name = self.name(identifier)
            if identifier in self.defined[lane]:
                return f"[[{name}]]"
            self.defined[lane].add(identifier)
            return f"[[{name}:{re.sub(r'[0-9]+', '[0-9]+', identifier)}]]"

        return replace

    def anonymous(self, match):
        identifier = match.group(0)
        if not self.unstable(identifier):
            return identifier
        return "{{" + re.sub(r"[0-9]+", "[0-9]+", identifier) + "}}"

    def pattern(self, line, lane, label=False):
        substitute = self.anonymous if label else self.substitute(lane)
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


def merge(lines_by_target):
    targets = list(lines_by_target)
    merged = [(line, {targets[0]}) for line in lines_by_target[targets[0]]]
    for count, target in enumerate(targets[1:], 1):
        seen = set(targets[:count])
        anchors = [index for index, (_, lanes) in enumerate(merged) if lanes >= seen]
        lines = lines_by_target[target]
        matcher = difflib.SequenceMatcher(None, [merged[index][0] for index in anchors], lines, autojunk=False)
        inserts = defaultdict(list)
        for tag, i1, i2, j1, j2 in matcher.get_opcodes():
            if tag == "equal":
                for offset in range(i2 - i1):
                    merged[anchors[i1 + offset]][1].add(target)
            elif j1 < j2:
                position = anchors[i2] if i2 < len(anchors) else len(merged)
                inserts[position].extend(lines[j1:j2])
        rebuilt = []
        for index in range(len(merged) + 1):
            rebuilt.extend((line, {target}) for line in inserts.get(index, ()))
            if index < len(merged):
                rebuilt.append(merged[index])
        merged = rebuilt
    return merged


def file_kind(position, length, line):
    if position == 0:
        return "", True
    return ("-EMPTY", False) if not line.strip() else ("-NEXT", False)


def function_kind(position, length, line):
    if length == 1:
        return "", True
    if position == 0:
        return "-LABEL", True
    if position == length - 1:
        return "", True
    return ("-EMPTY", False) if not line.strip() else ("-NEXT", False)


def render(lines_by_target, identifiers, kind):
    targets = list(lines_by_target)
    variables = Variables(identifiers)
    lengths = {target: len(lines) for target, lines in lines_by_target.items()}
    positions = defaultdict(int)
    checks = []
    for line, lanes in merge(lines_by_target):
        rendered = {}
        for lane in targets:
            if lane not in lanes:
                continue
            suffix, anchored = kind(positions[lane], lengths[lane], line)
            positions[lane] += 1
            if suffix == "-EMPTY":
                rendered[lane] = (suffix, "")
            elif kind is function_kind and anchored and suffix == "" and line == "}":
                rendered[lane] = ("", "{{^}}}")
            else:
                pattern = variables.pattern(line, lane, label=suffix == "-LABEL")
                rendered[lane] = (suffix, f"{{{{^}}}}{pattern}" if anchored else pattern)
        if len(rendered) == len(targets) and len(set(rendered.values())) == 1:
            checks.append(directive("CHECK", *next(iter(rendered.values()))))
        else:
            checks.extend(directive(lane.upper(), *value) for lane, value in rendered.items())
    return checks


def directive(prefix, suffix, pattern):
    return f"// {prefix}{suffix}: {pattern}".rstrip() if pattern else f"// {prefix}{suffix}:"


def update_source(source, outputs):
    names = checked_functions(source)
    stripped = BLOCK_PATTERN.sub("", source).rstrip("\n")
    identifiers = c_identifiers(stripped)
    lines_by_target = {target: rust.rstrip("\n").splitlines() for target, rust in outputs.items()}
    if names:
        checks = [
            check
            for name in names
            for check in render(
                {target: rust_function(lines, name, target) for target, lines in lines_by_target.items()},
                identifiers,
                function_kind,
            )
        ]
    else:
        checks = [*render(lines_by_target, identifiers, file_kind), "// CHECK-NOT: {{.}}"]
    header = " ".join([BEGIN, *outputs])
    return f"{stripped}\n\n{header}\n" + "\n".join(checks) + f"\n{END}\n"


def run_harness(targets, fixtures):
    generated = [path for path in fixtures if not is_release(path) and not is_plan(path)]
    release = [path for path in fixtures if is_release(path)]
    plan = [path for path in fixtures if path.resolve().parent == PLAN]
    lowered_names = [path for path in fixtures if path.resolve().parent == LOWERED_NAMES]
    for names, test_command in (
        ({path.stem for path in generated}, TEST_COMMAND),
        ({release_project(path) for path in release}, RELEASE_TEST_COMMAND),
        ({path.stem for path in plan}, PLAN_TEST_COMMAND),
        ({path.stem for path in lowered_names}, LOWERED_NAMES_TEST_COMMAND),
    ):
        if not names:
            continue
        environment = os.environ.copy()
        environment["SLATE_TEST_TARGETS"] = ",".join(dict.fromkeys(map(run_target, targets)))
        if len(names) == 1:
            environment["SLATE_DIFF_FIXTURE"] = next(iter(names))
        command = ["cargo", "nextest", "r", "--release", "--profile", "slate", "--no-fail-fast", *test_command]
        subprocess.run(command, cwd=WORKSPACE, env=environment, capture_output=True, check=False)


def all_fixtures():
    generated = (
        path
        for flavor in ("", "gcc", "msvc")
        for subdirectory in ("", *ARCH_TARGETS)
        for path in (FIXTURES / flavor / subdirectory).glob("*.c")
    )
    release = (
        path
        for path in RELEASE.rglob("*.c")
        if BEGIN in path.read_text(errors="surrogateescape")
    )
    return sorted([*generated, *release, *PLAN.glob("*.c"), *LOWERED_NAMES.glob("*.c")])


def expand_glob(argument):
    if not any(char in argument for char in "*?["):
        return [argument]
    path = Path(argument)
    pattern = str(path.resolve())
    deep = str(path.resolve().parent / "**" / path.name)
    known = {str(fixture) for fixture in all_fixtures()}
    matches = sorted(
        match
        for candidate in (pattern, deep)
        for match in glob.glob(candidate, recursive=True)
        if match in known
    )
    return list(dict.fromkeys(matches)) or [argument]


def resolve_fixture(argument):
    path = Path(argument)
    if path.suffix == ".c" or path.exists():
        return path.resolve(), None
    target, _, name = argument.rpartition("/")
    matches = [fixture for fixture in all_fixtures() if fixture.stem == name]
    if len(matches) != 1:
        found = ", ".join(str(match.relative_to(TESTS)) for match in matches) or "no fixture"
        raise FixtureError(f"{argument}: name matches {found}; pass a path")
    return matches[0], target or None


def fixture_targets(path, requested, extra):
    if requested:
        targets = list(requested)
    else:
        targets = block_targets(path.read_text(errors="surrogateescape"))
        if not targets:
            arch = fixture_arch(path)
            target = ARCH_TARGETS[arch] if arch else DEFAULT_TARGET
            if is_msvc(path):
                target = target.split("-", 1)[0] + WINDOWS_SUFFIX
            targets = [target]
    if extra and extra not in targets:
        targets.append(extra)
    for target in targets:
        check_target(path, target)
    return targets


def main(argv):
    parser = argparse.ArgumentParser(
        description="Regenerate the FileCheck blocks of tests/fixtures, tests/fixtures.release, tests/fixtures.plan and tests/fixtures.lowered-names fixtures: the whole generated output, or only the @check-fn-wrapped functions."
    )
    parser.add_argument(
        "fixtures",
        nargs="*",
        help="fixture paths (globs also match subdirectories), or names (optionally TARGET/NAME) as a failing test run lists them; default: every fixture",
    )
    parser.add_argument(
        "--target",
        action="append",
        default=[],
        help="target triple the block covers; repeatable; default: the targets the block already lists, else x86_64-unknown-linux-gnu or the fixture directory's architecture",
    )
    parser.add_argument(
        "--no-run",
        action="store_true",
        help="reuse the Rust the last test run wrote instead of rerunning the harness",
    )
    arguments = parser.parse_args(argv)
    targets_by_fixture = {}
    failures = 0
    requested = [
        expanded
        for argument in arguments.fixtures
        for expanded in expand_glob(argument)
    ]
    for argument in requested or [str(path) for path in all_fixtures()]:
        try:
            path, extra = resolve_fixture(argument)
            targets = fixture_targets(path, arguments.target, extra)
        except FixtureError as error:
            print(f"error: {error}", file=sys.stderr)
            failures += 1
            continue
        targets_by_fixture.setdefault(path, [])
        targets_by_fixture[path].extend(target for target in targets if target not in targets_by_fixture[path])
    if not arguments.no_run and targets_by_fixture:
        targets = sorted({target for targets in targets_by_fixture.values() for target in targets})
        for path, fixture_targets_ in targets_by_fixture.items():
            for target in fixture_targets_:
                rust = generated_rust(target, path)
                rust.unlink(missing_ok=True)
                runtime_ok(rust).unlink(missing_ok=True)
        run_harness(targets, targets_by_fixture)
    for path, targets in targets_by_fixture.items():
        name = path.relative_to(TESTS)
        outputs = {}
        for target in targets:
            rust = generated_rust(target, path)
            if not rust.is_file():
                print(f"error: {name}: translation for {target} failed; no {rust.name}", file=sys.stderr)
                break
            if not runtime_ok(rust).is_file():
                print(f"error: {name}: Rust for {target} did not build or did not match C; checks left unchanged", file=sys.stderr)
                break
            outputs[target] = rust.read_text()
        else:
            try:
                source = path.read_text(errors="surrogateescape")
                updated = update_source(source, outputs)
            except FixtureError as error:
                print(f"error: {name}: {error}", file=sys.stderr)
                failures += 1
                continue
            if updated != source:
                path.write_text(updated, errors="surrogateescape")
                print(f"updated {name}")
            continue
        failures += 1
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
