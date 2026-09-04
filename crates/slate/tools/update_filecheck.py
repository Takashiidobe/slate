#!/usr/bin/env python3
import argparse
from collections import Counter
import glob
import os
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

TARGET_FIXTURE_DIRECTORIES = {"bionic", "macos", "msvc"}
FIXTURE_GLOB_ROOTS = (
    "tests/fixtures",
    "tests/fixtures/bionic",
    "tests/fixtures/macos",
    "tests/fixtures/msvc",
    "tests/fixtures.link",
    "tests/fixtures.cfg",
    "tests/fixtures.multi",
    "tests/fixtures.library",
)
SOURCE_ROOT_PATTERN = "__SLATE_FILECHECK_SOURCE_ROOT__"
FIXTURE_STD_OVERRIDES = {
    "gnu_asm_register_variable": "gnu23",
    "c23_typeof_unqual": "gnu23",
}
FIXTURE_CLANG_ARG_OVERRIDES = {
    "goto_temp_cross_state": ["-O2"],
    "ptr_param_field_addr_of_mut": ["-O2"],
    "branch_hint_builtins": ["-O1"],
}


def fixture_std(path):
    return FIXTURE_STD_OVERRIDES.get(path.stem, "c23")


def fixture_clang_args(path):
    return FIXTURE_CLANG_ARG_OVERRIDES.get(path.stem, [])


ANNOTATION_PATTERN = re.compile(
    r"^(?P<indent>\s*)(?:"
    r"//\s*@(?P<line_kind>(?:lowering|rewrite)(?:-not)?)-(?P<line_fn>fn-)?"
    r"(?P<line_boundary>begin|end)\s*"
    r"|/\*\s*@(?P<block_kind>(?:lowering|rewrite)(?:-not)?)-(?P<block_fn>fn-)?"
    r"(?P<block_boundary>begin|end)\s*\*/\s*"
    r")$"
)
GENERATED_MARKER_PATTERN = re.compile(
    r'(?:core|std)::arch::asm!\("# SLATE_FILECHECK_(BEGIN|END)_(\d+)"'
)
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
C_ATTRIBUTE_PATTERN = re.compile(
    r"\b(?:__attribute__|__declspec|__asm__|asm)\s*\("
)
RUST_FN_PATTERN = (
    r'^(?:pub(?:\([^)]*\))?\s+)?(?:unsafe\s+)?(?:extern\s+"[^"]+"\s+)?fn\s+'
)
RUST_STRING_PATTERN = re.compile(
    r'(?:(?:b|c)?r(?P<hash>#{0,16})".*?"(?P=hash)|(?:b|c)?"(?:\\.|[^"\\])*")'
)
UNSTABLE_IDENTIFIER_PATTERNS = (
    (re.compile(r"\b_v[0-9]+\b"), "{{_v[0-9]+}}"),
    (re.compile(r"\barg[0-9]+\b"), "{{arg[0-9]+}}"),
    (
        re.compile(r"\banon_struct[0-9A-Za-z_]*\b"),
        "{{anon_struct[0-9A-Za-z_]*}}",
    ),
    (re.compile(r"(?<![0-9A-Za-z_])anon_[0-9]+\b"), "{{anon_[0-9]+}}"),
    (
        re.compile(r"\b_unnamed_at_[0-9A-Za-z_]+\b"),
        "{{_unnamed_at_[0-9A-Za-z_]+}}",
    ),
    (
        re.compile(r"\b__slate_alloca_frame[0-9]+\b"),
        "{{__slate_alloca_frame[0-9]+}}",
    ),
    (re.compile(r"\b__state[0-9]+\b"), "{{__state[0-9]+}}"),
    (
        re.compile(r"\b__dispatch[0-9]+_l[0-9]+\b"),
        "{{__dispatch[0-9]+_l[0-9]+}}",
    ),
    (
        re.compile(r"\b__dispatch[0-9]+_v[0-9]+\b"),
        "{{__dispatch[0-9]+_v[0-9]+}}",
    ),
    (
        re.compile(r"\b__switch[0-9]+_l[0-9]+\b"),
        "{{__switch[0-9]+_l[0-9]+}}",
    ),
    (re.compile(r"\b__dispatch[0-9]+\b"), "{{__dispatch[0-9]+}}"),
)


def strip_attribute_groups(text):
    text = re.sub(r"\[\[.*?\]\]", " ", text, flags=re.DOTALL)
    while True:
        match = C_ATTRIBUTE_PATTERN.search(text)
        if not match:
            return text
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


def c_function_definition_name(lines):
    head = strip_attribute_groups("\n".join(lines).split("{", 1)[0])
    for match in re.finditer(r"\b([^\W\d]\w*)\s*\(", head):
        if match.group(1) not in C_DECLARATOR_KEYWORDS:
            return match.group(1)
    return None


def instrument_annotations(source, first_id=0):
    annotations = {}
    fn_targets = {}
    active = []
    next_id = first_id
    output = []
    lines = source.splitlines()
    for line_number, line in enumerate(lines, 1):
        match = ANNOTATION_PATTERN.match(line)
        if not match:
            output.append(line)
            continue
        kind = match.group("line_kind") or match.group("block_kind")
        is_fn = bool(match.group("line_fn") or match.group("block_fn"))
        annotation_boundary = match.group("line_boundary") or match.group(
            "block_boundary"
        )
        indent = match.group("indent")
        if annotation_boundary == "end":
            if not active:
                raise ValueError(f"line {line_number}: unmatched @{kind}-end")
            active_kind, marker_id, start_line, active_fn = active[-1]
            if active_kind != kind:
                if any(open_kind == kind for open_kind, _, _, _ in active):
                    raise ValueError(
                        f"line {line_number}: @{kind}-end crosses "
                        f"@{active_kind}-begin opened at line {start_line}"
                    )
                raise ValueError(
                    f"line {line_number}: @{kind}-end does not match "
                    f"@{active_kind}-begin opened at line {start_line}"
                )
            if active_fn != is_fn:
                raise ValueError(
                    f"line {line_number}: @{kind} boundary style mismatches "
                    f"the -begin opened at line {start_line}"
                )
            active.pop()
            boundary = "END"
            if is_fn:
                name = c_function_definition_name(lines[start_line : line_number - 1])
                if name is None:
                    raise ValueError(
                        f"line {start_line}: @{kind}-fn-begin does not wrap a "
                        "function definition"
                    )
                fn_targets[marker_id] = name
        else:
            if any(open_kind == kind for open_kind, _, _, _ in active):
                raise ValueError(f"line {line_number}: @{kind}-begin is already open")
            marker_id = next_id
            next_id += 1
            annotations[marker_id] = kind
            active.append((kind, marker_id, line_number, is_fn))
            boundary = "BEGIN"
        if is_fn:
            output.append("")
        else:
            output.append(
                f'{indent}__asm__ __volatile__("# SLATE_FILECHECK_{boundary}_{marker_id}");'
            )
    if active:
        kind, _, line_number, _ = active[-1]
        raise ValueError(f"line {line_number}: unclosed @{kind}-begin annotation")
    suffix = "\n" if source.endswith("\n") else ""
    return "\n".join(output) + suffix, annotations, fn_targets


def extract_fn_item(lines, name):
    pattern = re.compile(RUST_FN_PATTERN + re.escape(name) + r"\s*[(<]")
    for index, line in enumerate(lines):
        if not pattern.match(line):
            continue
        start = index
        while start > 0 and lines[start - 1].lstrip().startswith(("#[", "///")):
            start -= 1
        for end in range(index, len(lines)):
            if lines[end] == "}":
                return lines[start : end + 1]
        break
    return None


def extract_generated_regions(rust, fn_targets=None):
    if not rust:
        return {}
    lines = rust.splitlines()
    regions = {}
    for marker_id, name in (fn_targets or {}).items():
        item = extract_fn_item(lines, name)
        if item is None:
            raise RuntimeError(f"generated Rust has no definition of fn {name}")
        regions[marker_id] = item
    active = []
    skip_wrapper_close = False
    for line_number, line in enumerate(lines, 1):
        marker = GENERATED_MARKER_PATTERN.search(line)
        if marker:
            boundary, raw_id = marker.groups()
            marker_id = int(raw_id)
            for _, captured in active:
                if captured and captured[-1].strip() == "unsafe {":
                    captured.pop()
            if boundary == "BEGIN":
                active.append((marker_id, []))
                skip_wrapper_close = True
            else:
                if not active or active[-1][0] != marker_id:
                    raise RuntimeError(
                        f"generated marker {marker_id} ends without its begin marker"
                    )
                _, captured = active.pop()
                regions[marker_id] = captured
                skip_wrapper_close = True
            continue
        if skip_wrapper_close:
            if line.strip() != "}":
                raise RuntimeError(
                    f"generated marker wrapper is not closed at Rust line {line_number}"
                )
            skip_wrapper_close = False
            continue
        for _, captured in active:
            captured.append(line)
    if active:
        raise RuntimeError(f"generated marker {active[-1][0]} has no end marker")
    return regions


def render_annotation_checks(annotations, fn_targets, rewritten, lowered, prefix):
    rewritten_regions = extract_generated_regions(rewritten, fn_targets)
    lowered_regions = extract_generated_regions(lowered, fn_targets)
    regions_by_profile = {
        "lowering": lowered_regions,
        "rewrite": rewritten_regions,
    }
    profile = prefix.partition("-")[0].lower().removesuffix("s")
    opposite = "rewrite" if profile == "lowering" else "lowering"
    checks = []
    for marker_id, kind in annotations.items():
        if kind.removesuffix("-not") != profile:
            continue
        negative = kind.endswith("-not")
        selected_profile = opposite if negative else profile
        regions = regions_by_profile[selected_profile]
        if marker_id not in regions:
            raise RuntimeError(
                f"generated Rust is missing annotation marker {marker_id}"
            )
        directive = f"{prefix}-NOT" if negative else f"{prefix}-DAG"
        entries = dedented_patterns(regions[marker_id])
        if negative:
            baseline = regions_by_profile[profile]
            if marker_id not in baseline:
                raise RuntimeError(
                    f"{profile} Rust is missing annotation marker {marker_id}"
                )
            remaining = Counter(
                pattern for _, pattern in dedented_patterns(baseline[marker_id])
            )
            removed = []
            for indent, pattern in entries:
                if remaining[pattern]:
                    remaining[pattern] -= 1
                else:
                    removed.append((indent, pattern))
            entries = removed
        if not entries:
            raise RuntimeError(f"@{kind} annotation {marker_id} selected no checks")
        checks.extend(
            f"// {directive}: {indent}{pattern}" for indent, pattern in entries
        )
    return checks


def dedented_patterns(lines):
    body = [line for line in lines if line.strip()]
    if not body:
        return []
    base = min(len(line) - len(line.lstrip()) for line in body)
    return [
        (
            line[base : len(line) - len(line.lstrip())],
            render_check_pattern(line.strip()),
        )
        for line in body
    ]


def render_checks(rust, prefix):
    checks = []
    for index, line in enumerate(rust.splitlines()):
        if line:
            directive = prefix if index == 0 else f"{prefix}-NEXT"
            checks.append(f"// {directive}: {render_check_pattern(line)}")
        else:
            checks.append(f"// {prefix}-EMPTY:")
    return checks


def escape_check_pattern(line):
    escaped = (
        line.replace("{{", "{{[{][{]}}")
        .replace("[[", "{{\\[\\[}}")
        .replace("\x00", "{{\\x00}}")
    )
    return escaped.replace(SOURCE_ROOT_PATTERN, "{{.*}}")


def render_check_pattern(line):
    parts = []
    offset = 0
    for string in RUST_STRING_PATTERN.finditer(line):
        parts.append(
            genericize_identifiers(escape_check_pattern(line[offset : string.start()]))
        )
        parts.append(escape_check_pattern(string.group()))
        offset = string.end()
    parts.append(genericize_identifiers(escape_check_pattern(line[offset:])))
    return "".join(parts)


def genericize_identifiers(pattern):
    for identifier, replacement in UNSTABLE_IDENTIFIER_PATTERNS:
        pattern = identifier.sub(replacement, pattern)
    return pattern


def generated_block(checks, prefix):
    begin = f"// SLATE-FILECHECK-BEGIN {prefix.lower()}"
    end = f"// SLATE-FILECHECK-END {prefix.lower()}"
    return "\n".join([begin, *checks, end])


def remove_generated_block(source, prefix):
    profile = re.escape(prefix.lower())
    pattern = re.compile(
        rf"(?ms)^\s*//[^\n]*SLATE-FILECHECK-BEGIN\s+{profile}(?=\s|$)[^\n]*(?:\n|$)"
        rf".*?^\s*//[^\n]*SLATE-FILECHECK-END\s+{profile}(?=\s|$)[^\n]*(?:\n|$)"
    )
    return pattern.sub("", source)


def has_handwritten_block(source, prefix):
    outside = remove_generated_block(source, prefix)
    directive = re.compile(rf"^\s*//\s*{prefix.upper()}(?:-[A-Z0-9_-]+)?:", re.M)
    return bool(directive.search(outside))


def insert_generated_blocks(source, blocks):
    source = source.rstrip("\n")
    separator = "\n\n" if source else ""
    blocks = "\n\n".join(blocks)
    return f"{source}{separator}{blocks}\n"


def translate_output(path, command, environment=None, fixture_path=None):
    fixture_path = fixture_path or path
    env = os.environ.copy()
    if environment:
        env.update(environment)
    clang_args = shlex.split(env.get("SLATE_CLANG_ARGS", ""))
    env["SLATE_CLANG_ARGS"] = shlex.join(
        [
            *clang_args,
            f"-std={fixture_std(fixture_path)}",
            *fixture_clang_args(fixture_path),
        ]
    )
    result = subprocess.run(
        [*command, str(path.resolve())],
        check=False,
        text=True,
        capture_output=True,
        env=env,
    )
    if result.returncode:
        raise RuntimeError(
            f"{path}: translation failed ({result.returncode})\n{result.stderr}"
        )
    return result.stdout


def translate_instrumented(path, source, command, environment=None):
    temporary = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="w",
            suffix=path.suffix,
            prefix=f".{path.stem}.filecheck-",
            dir=path.parent,
            delete=False,
        ) as file:
            file.write(source)
            temporary = Path(file.name)
        rust = translate_output(temporary, command, environment, fixture_path=path)
        return rust.replace(str(temporary.resolve()), str(path.resolve()))
    finally:
        if temporary is not None:
            temporary.unlink(missing_ok=True)


def normalize_source_paths(rust, path):
    root = Path(__file__).resolve().parent.parent
    try:
        path.resolve().relative_to(root)
    except ValueError:
        return rust
    return rust.replace(f"{root}{os.sep}", SOURCE_ROOT_PATTERN)


def target_environment(triple):
    environment = {"SLATE_TARGET": triple}
    args = []
    if "android" in triple:
        api = "21"
        arch = "AARCH64" if triple.startswith("aarch64") else "X86_64"
        environment["SLATE_ANDROID_API"] = api
        args.extend([f"-D__SLATE_ANDROID_API__={api}", f"-DEXPECT_{arch}"])
    elif "apple-darwin" in triple:
        args.append("-mmacosx-version-min=11.0")
    if args:
        inherited = shlex.split(os.environ.get("SLATE_CLANG_ARGS", ""))
        environment["SLATE_CLANG_ARGS"] = shlex.join([*inherited, *args])
    return environment


def default_targets_for_path(path):
    parts = path.resolve().parts
    if "bionic" in parts or path.parent.name == "bionic":
        return [
            ("BIONIC-AARCH64", target_environment("aarch64-linux-android")),
            ("BIONIC-X86_64", target_environment("x86_64-linux-android")),
        ]
    if "macos" in parts or path.parent.name == "macos":
        return [
            ("MACOS", target_environment("aarch64-apple-darwin")),
        ]
    if "msvc" in parts or path.parent.name == "msvc":
        return [
            ("MSVC", target_environment("x86_64-pc-windows-msvc")),
        ]
    if ("fixtures.link" in parts and "long_double" in parts) or (
        path.parent.name == "long_double" and "fixtures.link" in str(path)
    ):
        return [
            ("X86_64-GNU", target_environment("x86_64-unknown-linux-gnu")),
        ]
    return []


def update_path(path, profiles, in_place, target_mode):
    if not path.is_file():
        raise ValueError(f"path is not a file: {path}")
    source = path.read_text()
    instrumented, annotations, fn_targets = instrument_annotations(source)
    if target_mode:
        source = remove_generated_block(
            remove_generated_block(source, "LOWERING"), "REWRITES"
        )
    updated = source
    blocks = []
    annotation_outputs = {}
    for profile, command, environment in profiles:
        annotation_profile = profile.partition("-")[0].lower().removesuffix("s")
        has_profile_annotations = any(
            kind.removesuffix("-not") == annotation_profile
            for kind in annotations.values()
        )
        if annotations and not has_profile_annotations:
            updated = remove_generated_block(updated, profile)
            continue
        if has_handwritten_block(updated, profile):
            print(
                f"skip: {path}: handwritten {profile} directives present; leaving block untouched",
                file=sys.stderr,
            )
            continue
        updated = remove_generated_block(updated, profile)
        try:
            if annotations:
                cache_key = (tuple(command[:-1]), tuple(sorted(environment.items())))
                if cache_key not in annotation_outputs:
                    lowered_command = [*command[:-1], "translate-lowered"]
                    rewritten_command = [*command[:-1], "translate"]
                    annotation_outputs[cache_key] = (
                        normalize_source_paths(
                            translate_instrumented(
                                path, instrumented, rewritten_command, environment
                            ),
                            path,
                        ),
                        normalize_source_paths(
                            translate_instrumented(
                                path, instrumented, lowered_command, environment
                            ),
                            path,
                        ),
                    )
                rewritten, lowered = annotation_outputs[cache_key]
                checks = render_annotation_checks(
                    annotations, fn_targets, rewritten, lowered, profile
                )
            else:
                rust = normalize_source_paths(
                    translate_output(path, command, environment), path
                )
                checks = render_checks(rust, profile)
        except RuntimeError as error:
            if target_mode:
                print(
                    f"skip: {path}: {profile} translation failed; leaving block untouched",
                    file=sys.stderr,
                )
                return
            raise
        if not checks:
            raise RuntimeError(f"{path}: generated Rust contains no functions")
        blocks.append(generated_block(checks, profile))
    if blocks:
        updated = insert_generated_blocks(updated, blocks)
    if in_place:
        path.write_text(updated)
    else:
        sys.stdout.write(updated)


def project_sources(project, library):
    source_dir = project / "src" if library else project
    return sorted(source_dir.glob("*.c"))


def run_project_translation(project, crate_dir, slate, raw, library, environment):
    env = os.environ.copy()
    env.update(environment)
    clang_args = shlex.split(env.get("SLATE_CLANG_ARGS", ""))
    env["SLATE_CLANG_ARGS"] = shlex.join([*clang_args, "-std=gnu23"])
    env.pop("SLATE_RAW_LOWER", None)
    if raw:
        env["SLATE_RAW_LOWER"] = "1"
    command = [*slate, "translate-project"]
    if library:
        command.append("--lib")
    result = subprocess.run(
        [*command, str(project), str(crate_dir)],
        check=False,
        text=True,
        capture_output=True,
        env=env,
    )
    if result.returncode:
        raise RuntimeError(
            f"{project}: project translation failed ({result.returncode})\n{result.stderr}"
        )
    return {
        path.name: path.read_text() for path in sorted((crate_dir / "src").glob("*.rs"))
    }


def module_with_marker(modules, marker_id, fn_targets):
    name = fn_targets.get(marker_id)
    if name is None:
        needle = f"SLATE_FILECHECK_BEGIN_{marker_id}"
        matches = [rust for rust in modules.values() if needle in rust]
    else:
        matches = [
            rust
            for rust in modules.values()
            if extract_fn_item(rust.splitlines(), name) is not None
        ]
    if len(matches) != 1:
        raise RuntimeError(
            f"generated project has {len(matches)} modules containing marker {marker_id}"
        )
    return matches[0]


def update_project(project, profiles, in_place, slate, library):
    source_entries = []
    next_id = 0
    for path in project_sources(project, library):
        source = path.read_text()
        instrumented, annotations, fn_targets = instrument_annotations(source, next_id)
        next_id += len(annotations)
        if annotations:
            source_entries.append(
                (path, source, instrumented, annotations, fn_targets)
            )
    if not source_entries:
        raise RuntimeError(f"{project}: project contains no FileCheck annotations")

    requested = {profile.partition("-")[0] for profile, _, _ in profiles}
    kinds = {
        kind
        for _, _, _, annotations, _ in source_entries
        for kind in annotations.values()
    }
    needs_both = any(kind.endswith("-not") for kind in kinds)
    needs_lowering = needs_both or "LOWERING" in requested
    needs_rewrites = needs_both or "REWRITES" in requested
    environment = profiles[0][2]

    with tempfile.TemporaryDirectory(
        prefix=f".{project.name}.filecheck-", dir=project.parent
    ) as temporary:
        temporary = Path(temporary)
        instrumented_project = temporary / project.name
        shutil.copytree(project, instrumented_project)
        for path, _, instrumented, _, _ in source_entries:
            relative = path.relative_to(project)
            (instrumented_project / relative).write_text(instrumented)
        lowered_modules = (
            run_project_translation(
                instrumented_project,
                temporary / "lowered",
                slate,
                True,
                library,
                environment,
            )
            if needs_lowering
            else {}
        )
        rewritten_modules = (
            run_project_translation(
                instrumented_project,
                temporary / "rewritten",
                slate,
                False,
                library,
                environment,
            )
            if needs_rewrites
            else {}
        )

        for path, source, _, annotations, fn_targets in source_entries:
            marker_id = next(iter(annotations))
            lowered = (
                module_with_marker(lowered_modules, marker_id, fn_targets)
                if needs_lowering
                else ""
            )
            rewritten = (
                module_with_marker(rewritten_modules, marker_id, fn_targets)
                if needs_rewrites
                else ""
            )
            lowered = normalize_source_paths(lowered, path)
            rewritten = normalize_source_paths(rewritten, path)
            updated = source
            blocks = []
            for profile, _, _ in profiles:
                annotation_profile = profile.partition("-")[0].lower().removesuffix("s")
                if not any(
                    kind.removesuffix("-not") == annotation_profile
                    for kind in annotations.values()
                ):
                    updated = remove_generated_block(updated, profile)
                    continue
                if has_handwritten_block(updated, profile):
                    print(
                        f"skip: {path}: handwritten {profile} directives present; leaving block untouched",
                        file=sys.stderr,
                    )
                    continue
                updated = remove_generated_block(updated, profile)
                checks = render_annotation_checks(
                    annotations, fn_targets, rewritten, lowered, profile
                )
                blocks.append(generated_block(checks, profile))
            if blocks:
                updated = insert_generated_blocks(updated, blocks)
            if in_place:
                path.write_text(updated)
            else:
                sys.stdout.write(updated)


def make_profiles(slate, targets, profile_name):
    profiles = []
    if profile_name in ("lowering", "both"):
        for prefix, environment in targets or [("", {})]:
            profiles.append(
                (
                    f"LOWERING-{prefix}".rstrip("-"),
                    [*slate, "translate-lowered"],
                    environment,
                )
            )
    if profile_name in ("rewrites", "both"):
        for prefix, environment in targets or [("", {})]:
            profiles.append(
                (f"REWRITES-{prefix}".rstrip("-"), [*slate, "translate"], environment)
            )
    return profiles


def resolve_paths(raw_args):
    resolved = []
    seen = set()
    for raw in raw_args:
        literal = Path(raw)
        if literal.exists():
            matches = [literal]
        else:
            matches = sorted(Path(p) for p in glob.glob(raw, recursive=True))
            if not matches and "/" not in raw:
                for root in FIXTURE_GLOB_ROOTS:
                    root_path = Path(root)
                    if not root_path.is_dir():
                        continue
                    matches.extend(sorted(root_path.glob(raw)))
                    matches.extend(sorted(root_path.glob(f"{raw}.c")))
        if not matches:
            raise SystemExit(f"error: no fixtures matched {raw!r}")
        for match in matches:
            if match not in seen:
                seen.add(match)
                resolved.append(match)
    return resolved


def infer_mode(path, project_flag, library_flag):
    if project_flag:
        return "project"
    if library_flag:
        return "library"
    parts = path.parts
    if "fixtures.multi" in parts:
        return "project"
    if "fixtures.library" in parts:
        return "library"
    return "plain"


def main(argv):
    parser = argparse.ArgumentParser(
        description="generate stable Slate FileCheck scaffolding"
    )
    parser.add_argument(
        "paths", nargs="+", help="fixture paths, or glob patterns against them"
    )
    parser.add_argument(
        "--profile", choices=("lowering", "rewrites", "both"), default="both"
    )
    parser.add_argument("--in-place", action="store_true")
    parser.add_argument("--slate", default="cargo run --quiet --")
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--project", action="store_true")
    mode.add_argument("--library-project", action="store_true")
    parser.add_argument(
        "--target",
        action="append",
        metavar="PREFIX=TRIPLE",
        help="generate target-qualified checks; may be repeated",
    )
    args = parser.parse_args(argv)
    if (args.project or args.library_project) and args.target:
        parser.error("project FileCheck generation does not support --target")
    slate = args.slate.split()
    explicit_targets = []
    for target in args.target or []:
        prefix, separator, triple = target.partition("=")
        if not separator or not prefix or not triple:
            parser.error(f"--target must be PREFIX=TRIPLE, got {target!r}")
        explicit_targets.append((prefix, target_environment(triple)))

    for path in resolve_paths(args.paths):
        path_mode = infer_mode(path, args.project, args.library_project)
        if path_mode in ("project", "library"):
            library = path_mode == "library"
            if not path.is_dir():
                if path.is_file():
                    path = path.parent
                    if library and path.name == "src":
                        path = path.parent
                else:
                    parser.error(f"project path does not exist: {path}")
            sub_projects = (
                [
                    p
                    for p in sorted(path.iterdir())
                    if p.is_dir() and project_sources(p, library)
                ]
                if (
                    path.name.startswith("fixtures.multi")
                    or path.name.startswith("fixtures.library")
                )
                else [path]
            )
            profiles = make_profiles(slate, explicit_targets, args.profile)
            for proj in sub_projects:
                try:
                    update_project(
                        proj,
                        profiles,
                        args.in_place,
                        slate,
                        library,
                    )
                except RuntimeError as error:
                    print(f"skip: {proj}: {error}", file=sys.stderr)
        elif path.is_dir():
            c_files = sorted(path.glob("*.c"))
            if not c_files:
                c_files = sorted(path.rglob("*.c"))
            for c_file in c_files:
                targets = (
                    explicit_targets
                    if explicit_targets
                    else default_targets_for_path(c_file)
                )
                profiles = make_profiles(slate, targets, args.profile)
                update_path(c_file, profiles, args.in_place, bool(targets))
        else:
            targets = (
                explicit_targets if explicit_targets else default_targets_for_path(path)
            )
            profiles = make_profiles(slate, targets, args.profile)
            update_path(path, profiles, args.in_place, bool(targets))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main(sys.argv[1:]))
    except (RuntimeError, ValueError) as error:
        print(f"error: {error}", file=sys.stderr)
        raise SystemExit(1)
