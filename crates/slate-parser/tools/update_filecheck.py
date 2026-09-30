#!/usr/bin/env python3
import argparse
import difflib
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path


RENDERER = Path(__file__).resolve().parents[3] / "target/test-cache/release/slate-parser"


def _no_color_env() -> dict:
    env = os.environ.copy()
    env.pop("FORCE_COLOR", None)
    env.pop("CLICOLOR_FORCE", None)
    env["NO_COLOR"] = "1"
    return env


DEFINE_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-DEFINES\s+([A-Za-z0-9_-]+)(?:\s+(.*))?$")
BEGIN_RE = re.compile(r"^// SLATE-FILECHECK-BEGIN ([A-Za-z0-9_-]+)$")
ERROR_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-ERROR\s+([A-Za-z0-9_-]+)$")
WARNING_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-WARNING\s+([A-Za-z0-9_-]+)$")
ISYSTEM_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-ISYSTEM\s+(.*)$")
STD_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-STD\s+([A-Za-z0-9_-]+)\s+(\S+)\s*$")
PREFIX_ARGS_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-PREFIX-ARGS\s+([A-Za-z0-9_-]+)\s+(.*)$")
SHOW_IDS_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-SHOW-IDS\s+([A-Za-z0-9_-]+)\s*$")
QUOTED_C_INCLUDE_RE = re.compile(r'^\s*#\s*include\s*"([^"/]+\.c)"', re.MULTILINE)


def isystem_paths(source: str) -> list[str]:
    paths = []
    for line in source.splitlines():
        match = ISYSTEM_RE.match(line)
        if match:
            paths.extend(os.path.expanduser(path) for path in match.group(1).split())
    return paths


FLAVORS = {"gcc", "clang", "msvc"}
OS_DIRECTORIES = {"linux", "windows", "darwin", "android", "freebsd"}
CANONICAL_TRIPLES = {
    ("linux", "x86_64"): "x86_64-unknown-linux-gnu",
    ("linux", "i686"): "i686-unknown-linux-gnu",
    ("linux", "aarch64"): "aarch64-unknown-linux-gnu",
    ("windows", "x86_64"): "x86_64-pc-windows-msvc",
    ("windows", "i686"): "i686-pc-windows-msvc",
    ("windows", "aarch64"): "aarch64-pc-windows-msvc",
    ("darwin", "x86_64"): "x86_64-apple-darwin",
    ("darwin", "aarch64"): "aarch64-apple-darwin",
    ("android", "x86_64"): "x86_64-linux-android",
    ("android", "aarch64"): "aarch64-linux-android",
    ("freebsd", "x86_64"): "x86_64-unknown-freebsd",
    ("freebsd", "aarch64"): "aarch64-unknown-freebsd",
}
TRIPLE_OS_MARKERS = (
    ("-windows-", "windows"),
    ("-apple-darwin", "darwin"),
    ("-linux-android", "android"),
    ("-freebsd", "freebsd"),
    ("-linux-", "linux"),
)


def placement(fixture: Path) -> tuple[str, str]:
    # must stay in step with fixture_placement in tests/filecheck.rs
    fixtures = Path(__file__).resolve().parent.parent / "tests/fixtures"
    dirs = list(fixture.resolve().relative_to(fixtures).parent.parts)
    if dirs[:1] == ["error"]:
        dirs = dirs[1:]
    elif dirs[:1] == ["suites"] and len(dirs) > 1:
        dirs = dirs[2:]
    shape = "expected [error/ | suites/<name>/]<gcc|clang|msvc>/[<os>/[<arch> | <triple>]]/"
    if not dirs or dirs[0] not in FLAVORS:
        raise ValueError(f"{fixture}: no compiler directory; {shape}")
    if len(dirs) > 3:
        raise ValueError(f"{fixture}: too many directories; {shape}")
    flavor = dirs[0]
    os_name = dirs[1] if len(dirs) > 1 else ("windows" if flavor == "msvc" else "linux")
    if os_name not in OS_DIRECTORIES:
        raise ValueError(f"{fixture}: `{os_name}` is not an OS directory; {shape}")
    leaf = dirs[2] if len(dirs) > 2 else "x86_64"
    triple = CANONICAL_TRIPLES.get((os_name, leaf))
    if triple is None:
        leaf_os = next((name for marker, name in TRIPLE_OS_MARKERS if marker in leaf), None)
        if leaf_os != os_name:
            raise ValueError(f"{fixture}: `{leaf}` is not an arch or {os_name} triple; {shape}")
        triple = leaf
    return flavor, triple


def placement_args(fixture: Path) -> list[str]:
    flavor, triple = placement(fixture)
    return [f"--flavor={flavor}", f"--target={triple}"]


def configurations(source: str) -> list[tuple[str, list[str]]]:
    found = []
    for line in source.splitlines():
        match = DEFINE_RE.match(line)
        if match:
            defines = [item for item in (match.group(2) or "").split() if item]
            found.append((match.group(1), defines))
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


def configuration_show_ids_args(source: str, prefix: str) -> list[str]:
    for line in source.splitlines():
        match = SHOW_IDS_RE.match(line)
        if match and match.group(1) == prefix:
            return ["--show-ids"]
    return []


def configuration_prefix_args(source: str, prefix: str) -> list[str]:
    return [
        arg
        for line in source.splitlines()
        if (match := PREFIX_ARGS_RE.match(line)) and match.group(1) == prefix
        for arg in match.group(2).split()
    ]


def fixture_args(source: str) -> list[str]:
    return [
        arg
        for line in source.splitlines()
        if line.strip().startswith("// SLATE-FILECHECK-ARGS ")
        for arg in line.strip().removeprefix("// SLATE-FILECHECK-ARGS ").split()
    ]


def error_configurations(source: str) -> list[str]:
    return [match.group(1) for line in source.splitlines() if (match := ERROR_RE.match(line))]


def warning_configurations(source: str) -> list[str]:
    return [match.group(1) for line in source.splitlines() if (match := WARNING_RE.match(line))]


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
    repo: Path,
    fixture: Path,
    source: str,
    defines: list[str],
    isystem: list[str],
    std_args: list[str],
    extra_args: list[str] = (),
) -> str:
    with tempfile.TemporaryDirectory(prefix=f".{fixture.stem}.filecheck.") as directory:
        parsed_fixture = write_isolated_fixture(Path(directory), fixture, source)
        example = next((line.strip().removeprefix("// SLATE-FILECHECK-EXAMPLE ").strip()
                        for line in source.splitlines()
                        if line.strip().startswith("// SLATE-FILECHECK-EXAMPLE ")), None)
        command = (["cargo", "run", "--release", "--quiet", "--example", example, "--"] if example
                   else [str(RENDERER), "parse", str(parsed_fixture)])
        command.extend(f"-D{define.removeprefix('-D')}" for define in defines)
        command.extend(f"-isystem{path}" for path in isystem)
        command.extend(std_args)
        command.extend(extra_args)
        command.extend(placement_args(fixture))
        filecheck_args = fixture_args(source)
        if not any(arg.startswith("--dump-ir") for arg in filecheck_args):
            command.append("--dump-ir")
        for line in source.splitlines():
            if line.strip().startswith("// SLATE-FILECHECK-ARGS "):
                command.extend(line.strip().removeprefix("// SLATE-FILECHECK-ARGS ").split())
        result = subprocess.run(command, cwd=repo, text=True, capture_output=True, env=_no_color_env())
    if result.returncode:
        raise RuntimeError(result.stderr or result.stdout)
    return result.stdout.rstrip("\n")


def render_warnings(
    repo: Path,
    fixture: Path,
    source: str,
    defines: list[str],
    isystem: list[str],
    std_args: list[str],
    extra_args: list[str] = (),
) -> list[str]:
    parsed_name = f".{fixture.stem}.filecheck.{os.getpid()}.0.c"
    parsed_fixture = fixture.with_name(parsed_name)
    parsed_fixture.write_text(fixture_source(source), errors="surrogateescape")
    try:
        command = [str(RENDERER), "parse", str(parsed_fixture)]
        command.extend(f"-D{define.removeprefix('-D')}" for define in defines)
        command.extend(f"-isystem{path}" for path in isystem)
        command.extend(std_args)
        command.extend(extra_args)
        command.extend(placement_args(fixture))
        filecheck_args = fixture_args(source)
        if not any(arg.startswith("--dump-ir") for arg in filecheck_args):
            command.append("--dump-ir")
        for line in source.splitlines():
            if line.strip().startswith("// SLATE-FILECHECK-ARGS "):
                command.extend(line.strip().removeprefix("// SLATE-FILECHECK-ARGS ").split())
        result = subprocess.run(command, cwd=repo, text=True, capture_output=True, env=_no_color_env())
    finally:
        parsed_fixture.unlink()
    if result.returncode:
        raise RuntimeError(f"expected {fixture} to parse successfully:\n{result.stderr}")
    return [
        line.strip().replace(parsed_name, fixture.name)
        for line in result.stderr.splitlines()
        if re.match(r"^\s*(?:-W[a-z0-9-]+$|\d+ │|│|×|⚠|╭─|·|╰─)", line)
    ]


def render_error(
    repo: Path,
    fixture: Path,
    source: str,
    defines: list[str],
    isystem: list[str],
    std_args: list[str],
    extra_args: list[str] = (),
) -> list[str]:
    parsed_name = f".{fixture.stem}.filecheck.{os.getpid()}.0.c"
    parsed_fixture = fixture.with_name(parsed_name)
    parsed_fixture.write_text(fixture_source(source), errors="surrogateescape")
    try:
        command = [str(RENDERER), "parse", str(parsed_fixture)]
        command.extend(f"-D{define.removeprefix('-D')}" for define in defines)
        command.extend(f"-isystem{path}" for path in isystem)
        command.extend(std_args)
        command.extend(extra_args)
        command.extend(placement_args(fixture))
        for line in source.splitlines():
            if line.strip().startswith("// SLATE-FILECHECK-ARGS "):
                command.extend(line.strip().removeprefix("// SLATE-FILECHECK-ARGS ").split())
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
SYSTEM_KIND_RE = re.compile(r"^// [^:]+:(\s*)kind: System,$")
PROVENANCE_LINE_RE = re.compile(r"^(// [^:]+:(\s*)line: )[0-9]+(,)$")
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


def loosen_system_provenance(block: list[str]) -> list[str]:
    """Match a system header's provenance line number by regex. Editing such a
    header shifts every line below it, which would otherwise restamp every
    fixture that reads it. User-file provenance stays exact: those line numbers
    live in the fixture itself and a wrong one is a real defect."""
    result: list[str] = []
    for line in block:
        system = SYSTEM_KIND_RE.match(result[-1]) if result else None
        if system:
            loosened = PROVENANCE_LINE_RE.match(line)
            if loosened and loosened.group(2) == system.group(1):
                line = f"{loosened.group(1)}{{{{[0-9]+}}}}{loosened.group(3)}"
        result.append(line)
    return result


TAG_LABEL_RE = re.compile(r"^(// [^:]+: tag\[)[0-9]+(\].*)$")
DECL_LABEL_RE = re.compile(r"^(// [^:]+: decl\[)[0-9]+(\].*)$")
ID_OPEN_RE = re.compile(r"^// [^:]+:\s*(?:[a-z_]+: )?(TagId|FileId)\($")
ID_VALUE_RE = re.compile(r"^(// [^:]+:\s*)([0-9]+)(,)$")
ID_VARIABLE_PREFIXES = {"TagId": "TAG", "FileId": "FILE", "NodeId": "NODE"}
SPAN_FILE_RE = re.compile(r"(?<=\b)(spelling|expansion)=([0-9]+)(?=:)")
NODE_ID_RE = re.compile(r"(?<![\w\[])#([0-9]+)\b")


def loosen_ids(block: list[str]) -> list[str]:
    """Match tag ids, file ids, and decl indices by regex. Tag ids and decl
    indices count every declaration in the translation unit, system headers
    included, so adding one declaration to a header restamps every fixture that
    reads it; file ids are stamped in the order sources are interned, so adding
    a predefine layer restamps every fixture that reports provenance. A tag id
    or file id is also a cross-reference, so each distinct id binds a FileCheck
    numeric variable on first sight and reuses it afterwards: a declaration
    still has to name the tag it named before and come from the same header it
    came from before, only the absolute number stops mattering. The tag[N] and
    decl[N] labels are rendered from the same field the block already checks,
    so they carry nothing a variable would preserve."""
    variables: dict[tuple[str, str], tuple[str, int]] = {}
    index = 0

    def reference(kind: str, number: str) -> str:
        key = (kind, number)
        if key in variables:
            name, defined_at = variables[key]
            # FileCheck rejects a numeric variable used in the directive that defines it
            return "{{[0-9]+}}" if defined_at == index else f"[[#{name}]]"
        prefix = ID_VARIABLE_PREFIXES[kind]
        name = f"{prefix}{sum(1 for other in variables if other[0] == kind)}"
        variables[key] = (name, index)
        return f"[[#{name}:]]"

    result: list[str] = []
    for index, line in enumerate(block):
        line = SPAN_FILE_RE.sub(
            lambda span: f"{span.group(1)}={reference('FileId', span.group(2))}", line
        )
        line = NODE_ID_RE.sub(lambda node: f"#{reference('NodeId', node.group(1))}", line)
        label = TAG_LABEL_RE.match(line) or DECL_LABEL_RE.match(line)
        if label:
            result.append(f"{label.group(1)}{{{{[0-9]+}}}}{label.group(2)}")
            continue
        opener = ID_OPEN_RE.match(result[-1]) if result else None
        if opener:
            value = ID_VALUE_RE.match(line)
            if value:
                line = f"{value.group(1)}{reference(opener.group(1), value.group(2))}{value.group(3)}"
        result.append(line)
    return result


IR_CHECK_RE = re.compile(r"^(// [^:]+: )(.*)$")
IR_QUOTED_RE = re.compile(r'"(?:[^"\\]|\\.)*"')
IR_TYPE_REF_RE = re.compile(r"@type([0-9]+)\b(?!\()")
IR_BINDING_RE = re.compile(r"(?<![\w%])%([0-9]+)\b(?: \.str([0-9]+)(?=:))?")
IR_TYPE_DEF_RE = re.compile(r"^\s*type @type([0-9]+) (?:([A-Za-z_][A-Za-z0-9_]*) )?=")
IR_BINDING_NAME_RES = (
    re.compile(r"(?<![\w%])%([0-9]+) @([A-Za-z_][A-Za-z0-9_]*)\("),
    re.compile(r"(?<![\w%])%([0-9]+) ([A-Za-z_][A-Za-z0-9_]*)(?::| =)"),
    re.compile(r"(?<![\w%])%([0-9]+) \.(str)[0-9]+:"),
)


def loosen_ir_ids(block: list[str]) -> list[str]:
    """Match IR type ids (@typeN) and bindings (%N) by FileCheck string
    variables. Both are numbered over the whole module, system headers
    included, so one more header declaration restamps every later id. Each id
    is still a cross-reference: every use binds or reuses the variable named
    after the entity's declared name, and the type or binding's one definition
    line must carry the same number, so only the absolute number stops
    mattering. Quoted strings and asm templates are left alone because their
    %N are operand indices, not bindings."""
    if len(block) < 2 or not block[1].endswith(": module {"):
        return block
    names: dict[tuple[str, str], str] = {}
    taken: set[str] = set()
    ordinals = {"TYPE": 0, "VALUE": 0}

    def claim(kind: str, number: str, name: str | None) -> None:
        if (kind, number) in names:
            return
        if name is None:
            variable = f"{kind}{ordinals[kind]}"
            ordinals[kind] += 1
        else:
            variable = f"{kind}_{name}"
            suffix = 2
            while variable in taken:
                variable = f"{kind}_{name}_{suffix}"
                suffix += 1
        taken.add(variable)
        names[(kind, number)] = variable

    def outside_quotes(text: str) -> str:
        return IR_QUOTED_RE.sub(lambda quoted: " " * len(quoted.group(0)), text)

    for line in block:
        check = IR_CHECK_RE.match(line)
        if not check or check.group(2).lstrip().startswith("template:"):
            continue
        text = outside_quotes(check.group(2))
        definition = IR_TYPE_DEF_RE.match(text)
        if definition:
            claim("TYPE", definition.group(1), definition.group(2))
        for pattern in IR_BINDING_NAME_RES:
            for binding in pattern.finditer(text):
                claim("VALUE", binding.group(1), binding.group(2))

    defined: set[str] = set()

    def reference(kind: str, number: str) -> str:
        claim(kind, number, None)
        variable = names[(kind, number)]
        if variable in defined:
            return f"[[{variable}]]"
        defined.add(variable)
        return f"[[{variable}:[0-9]+]]"

    result = []
    for line in block:
        check = IR_CHECK_RE.match(line)
        if not check or check.group(2).lstrip().startswith("template:"):
            result.append(line)
            continue
        text = check.group(2)
        pieces = []
        last = 0
        for quoted in IR_QUOTED_RE.finditer(text):
            pieces.append((text[last:quoted.start()], True))
            pieces.append((quoted.group(0), False))
            last = quoted.end()
        pieces.append((text[last:], True))
        def binding(match: re.Match[str]) -> str:
            number, string = match.group(1), match.group(2)
            rewritten = f"%{reference('VALUE', number)}"
            if string is None:
                return rewritten
            # a string literal global is named after its own binding
            if string == number:
                return f"{rewritten} .str[[{names[('VALUE', number)]}]]"
            return f"{rewritten} .str{string}"

        rewritten = "".join(
            IR_BINDING_RE.sub(
                binding,
                IR_TYPE_REF_RE.sub(lambda ty: f"@type{reference('TYPE', ty.group(1))}", piece),
            )
            if loosen
            else piece
            for piece, loosen in pieces
        )
        result.append(check.group(1) + rewritten)
    return result


CODE_UNITS_OPEN_RE = re.compile(r"^(\s*)code_units: \[$")
PIECES_OPEN_RE = re.compile(r"^(\s*)pieces: \[$")
QUOTED_LINE_RE = re.compile(r'^\s*"((?:[^"\\]|\\.)*)",?$')
IR_CODE_UNITS_RE = re.compile(r"code_units<array<i8, ([0-9]+)>>\((\[[0-9, ]*\])\)")


def redact_code_units(lines: list[str]) -> list[tuple[str, bool, str | None]]:
    """Loosen a code_units list into a forward scan when its decoded text (the
    following pieces block) embeds the per-run temp fixture path: that path's
    byte count varies run to run (e.g. pid digit count), so neither an exact
    per-element CHECK-NEXT chain nor a single collapsed line can match it."""
    result: list[tuple[str, bool, str | None]] = []
    dynamic_file_globals: set[str] = set()
    i = 0
    n = len(lines)
    while i < n:
        unit_match = IR_CODE_UNITS_RE.search(lines[i])
        dynamic_file = False
        if unit_match:
            values = [int(value) for value in unit_match.group(2).strip("[]").split(",") if value.strip()]
            dynamic_file = b".filecheck." in bytes(values)
            global_match = re.search(r"global %([0-9]+) \.str[0-9]+:", lines[i])
            if dynamic_file and global_match:
                dynamic_file_globals.add(global_match.group(1))
        def redact_ir_code_units(match: re.Match[str]) -> str:
            values = [int(value) for value in match.group(2).strip("[]").split(",") if value.strip()]
            if b".filecheck." not in bytes(values):
                return match.group(0)
            return r"code_units<array<i8, {{[0-9]+}}>>({{\[[0-9, ]+\]}})"

        redacted, count = IR_CODE_UNITS_RE.subn(redact_ir_code_units, lines[i])
        if count and redacted != lines[i]:
            redacted = re.sub(r"array<i8, [0-9]+>", "array<i8, {{[0-9]+}}>", redacted)
            result.append((redacted, False, None))
            i += 1
            continue
        if dynamic_file_globals:
            for global_id in dynamic_file_globals:
                line, count = re.subn(
                    rf"length=Some\([0-9]+\)>\(%{global_id}\)",
                    f"length=Some({{{{[0-9]+}}}})>(%{global_id})",
                    lines[i],
                )
                if count:
                    break
            if count:
                result.append((line, False, None))
                i += 1
                continue
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
    errors = error_configurations(source)
    warnings = warning_configurations(source)
    config_names = {prefix for prefix, _ in configurations(source)}
    error_only = bool(errors) and not set(errors).issubset(config_names)
    for prefix in errors:
        output = render_error(
            repo,
            fixture,
            source,
            configuration_defines(source, prefix),
            isystem,
            configuration_std_args(source, prefix),
            configuration_show_ids_args(source, prefix) + configuration_prefix_args(source, prefix),
        )
        block = [f"// SLATE-FILECHECK-BEGIN {prefix}"]
        block.extend(f"// {prefix}: {escape_filecheck_literal(line)}" for line in output)
        block.append(f"// SLATE-FILECHECK-END {prefix}")
        blocks.extend(block)
    if error_only:
        return "\n".join(blocks)
    for prefix in warnings:
        output = render_warnings(
            repo,
            fixture,
            source,
            configuration_defines(source, prefix),
            isystem,
            configuration_std_args(source, prefix),
            configuration_show_ids_args(source, prefix) + configuration_prefix_args(source, prefix),
        )
        block = [f"// SLATE-FILECHECK-BEGIN {prefix}"]
        block.extend(f"// {prefix}: {escape_filecheck_literal(line)}" for line in output)
        block.append(f"// SLATE-FILECHECK-END {prefix}")
        blocks.extend(block)
    ir_errors = [
        line.strip().removeprefix("// SLATE-FILECHECK-IR-ERROR ")
        for line in source.splitlines()
        if line.strip().startswith("// SLATE-FILECHECK-IR-ERROR ")
    ]
    for prefix in ir_errors:
        output = render_error(
            repo,
            fixture,
            source,
            configuration_defines(source, prefix),
            isystem,
            configuration_std_args(source, prefix),
            ["--dump-ir"] + configuration_prefix_args(source, prefix),
        )
        blocks.extend([f"// SLATE-FILECHECK-BEGIN {prefix}"])
        blocks.extend(f"// {prefix}: {escape_filecheck_literal(line)}" for line in output)
        blocks.append(f"// SLATE-FILECHECK-END {prefix}")
    for prefix, defines in configurations(source):
        if prefix in errors or prefix in ir_errors:
            continue
        output = render(
            repo,
            fixture,
            source,
            defines,
            isystem,
            configuration_std_args(source, prefix),
            configuration_show_ids_args(source, prefix) + configuration_prefix_args(source, prefix),
        )
        check_prefix = f"IR-{prefix}" if prefix in warnings else prefix
        lines = redact_code_units(output.splitlines())
        block = [f"// SLATE-FILECHECK-BEGIN {check_prefix}"]
        for index, (line, escape, force) in enumerate(lines):
            directive = check_prefix if force == "plain" or index == 0 else f"{check_prefix}-NEXT"
            text = escape_filecheck_literal(line) if escape else line
            block.append(f"// {directive}: {text}")
        block = loosen_ir_ids(loosen_ids(loosen_system_provenance(block)))
        block.append(f"// SLATE-FILECHECK-END {check_prefix}")
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
    prefixes = sorted(
        {prefix for prefix, _ in configurations(source)}
        | set(error_configurations(source))
        | set(warning_configurations(source))
        | {f"IR-{prefix}" for prefix in warning_configurations(source)}
        | {
            line.strip().removeprefix("// SLATE-FILECHECK-IR-ERROR ")
            for line in source.splitlines()
            if line.strip().startswith("// SLATE-FILECHECK-IR-ERROR ")
        }
    )
    updated = replace_blocks(source, generated_blocks(repo, fixture, source), prefixes)
    return source, updated


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--in-place", action="store_true")
    parser.add_argument("--no-fail-fast", action="store_true")
    parser.add_argument("fixtures", nargs="*", type=Path)
    args = parser.parse_args()
    repo = Path(__file__).resolve().parent.parent
    fixtures = args.fixtures or sorted(
        path for path in (repo / "tests/fixtures").rglob("*.c") if not path.name.startswith(".")
    )
    subprocess.run(["cargo", "build", "--release", "--quiet", "--bin", "slate-parser"], cwd=repo, check=True)
    changed = False
    failures = []
    for fixture_arg in fixtures:
        fixture = (Path.cwd() / fixture_arg).resolve() if not fixture_arg.is_absolute() else fixture_arg
        try:
            source, updated = update(repo, fixture)
        except Exception as error:
            if not args.no_fail_fast:
                raise
            failures.append(fixture)
            print(f"FAIL {fixture}\n{error}", file=sys.stderr)
            continue
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
    if failures:
        print(f"\n{len(failures)} fixture(s) failed:", file=sys.stderr)
        for fixture in failures:
            print(f"  {fixture}", file=sys.stderr)
        return 1
    return 1 if changed and not args.in_place else 0


if __name__ == "__main__":
    raise SystemExit(main())
