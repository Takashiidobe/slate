#!/usr/bin/env python3
import argparse
from collections import Counter
import os
import re
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path


TARGET_FIXTURE_DIRECTORIES = {"bionic", "macos", "msvc"}
SOURCE_ROOT_PATTERN = "__SLATE_FILECHECK_SOURCE_ROOT__"
ANNOTATION_PATTERN = re.compile(
    r"^(?P<indent>\s*)(?://\s*@(?P<line_kind>rewrite(?:-not)?)\s*|"
    r"/\*\s*@(?P<block_kind>rewrite(?:-not)?)\s*\*/\s*)$"
)
GENERATED_MARKER_PATTERN = re.compile(
    r'(?:core|std)::arch::asm!\("# SLATE_FILECHECK_(BEGIN|END)_(\d+)"'
)
RUST_STRING_PATTERN = re.compile(
    r'(?:(?:b|c)?r(?P<hash>#{0,16})".*?"(?P=hash)|(?:b|c)?"(?:\\.|[^"\\])*")'
)
UNSTABLE_IDENTIFIER_PATTERNS = (
    (re.compile(r"\b_v[0-9]+\b"), "{{_v[0-9]+}}"),
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
    (re.compile(r"\b__dispatch[0-9]+\b"), "{{__dispatch[0-9]+}}"),
)


def instrument_annotations(source):
    annotations = {}
    active = None
    next_id = 0
    output = []
    for line_number, line in enumerate(source.splitlines(), 1):
        match = ANNOTATION_PATTERN.match(line)
        if not match:
            output.append(line)
            continue
        kind = match.group("line_kind") or match.group("block_kind")
        indent = match.group("indent")
        if active is None:
            marker_id = next_id
            next_id += 1
            annotations[marker_id] = kind
            active = (kind, marker_id, line_number)
            boundary = "BEGIN"
        else:
            active_kind, marker_id, start_line = active
            if kind != active_kind:
                raise ValueError(
                    f"line {line_number}: cannot nest @{kind} inside "
                    f"@{active_kind} opened at line {start_line}"
                )
            active = None
            boundary = "END"
        output.append(
            f'{indent}__asm__ __volatile__("# SLATE_FILECHECK_{boundary}_{marker_id}");'
        )
    if active is not None:
        kind, _, line_number = active
        raise ValueError(f"line {line_number}: unclosed @{kind} annotation")
    suffix = "\n" if source.endswith("\n") else ""
    return "\n".join(output) + suffix, annotations


def extract_generated_regions(rust):
    regions = {}
    active = None
    skip_wrapper_close = False
    for line_number, line in enumerate(rust.splitlines(), 1):
        marker = GENERATED_MARKER_PATTERN.search(line)
        if marker:
            boundary, raw_id = marker.groups()
            marker_id = int(raw_id)
            if boundary == "BEGIN":
                if active is not None:
                    raise RuntimeError(
                        f"generated marker {marker_id} begins inside marker {active[0]}"
                    )
                active = (marker_id, [])
                skip_wrapper_close = True
            else:
                if active is None or active[0] != marker_id:
                    raise RuntimeError(
                        f"generated marker {marker_id} ends without its begin marker"
                    )
                if active[1] and active[1][-1].strip() == "unsafe {":
                    active[1].pop()
                regions[marker_id] = active[1]
                active = None
                skip_wrapper_close = True
            continue
        if skip_wrapper_close:
            if line.strip() != "}":
                raise RuntimeError(
                    f"generated marker wrapper is not closed at Rust line {line_number}"
                )
            skip_wrapper_close = False
            continue
        if active is not None:
            active[1].append(line)
    if active is not None:
        raise RuntimeError(f"generated marker {active[0]} has no end marker")
    return regions


def render_annotation_checks(annotations, rewritten, lowered, prefix):
    rewritten_regions = extract_generated_regions(rewritten)
    lowered_regions = extract_generated_regions(lowered)
    checks = []
    for marker_id, kind in annotations.items():
        regions = rewritten_regions if kind == "rewrite" else lowered_regions
        if marker_id not in regions:
            raise RuntimeError(f"generated Rust is missing annotation marker {marker_id}")
        directive = f"{prefix}-DAG" if kind == "rewrite" else f"{prefix}-NOT"
        patterns = [
            render_check_pattern(line.strip())
            for line in regions[marker_id]
            if line.strip()
        ]
        if kind == "rewrite-not":
            if marker_id not in rewritten_regions:
                raise RuntimeError(
                    f"rewritten Rust is missing annotation marker {marker_id}"
                )
            remaining = Counter(
                render_check_pattern(line.strip())
                for line in rewritten_regions[marker_id]
                if line.strip()
            )
            removed = []
            for pattern in patterns:
                if remaining[pattern]:
                    remaining[pattern] -= 1
                else:
                    removed.append(pattern)
            patterns = removed
        if not patterns:
            raise RuntimeError(f"@{kind} annotation {marker_id} selected no checks")
        checks.extend(f"// {directive}: {pattern}" for pattern in patterns)
    return checks


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
        parts.append(genericize_identifiers(escape_check_pattern(line[offset:string.start()])))
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
    begin = f"// SLATE-FILECHECK-BEGIN {prefix.lower()}"
    end = f"// SLATE-FILECHECK-END {prefix.lower()}"
    pattern = re.compile(
        rf"(?ms)^\s*{re.escape(begin)}\n.*?^\s*{re.escape(end)}\n?"
    )
    return pattern.sub("", source, count=1)


def has_handwritten_block(source, prefix):
    outside = remove_generated_block(source, prefix)
    directive = re.compile(rf"^\s*//\s*{prefix.upper()}(?:-[A-Z0-9_-]+)?:", re.M)
    return bool(directive.search(outside))


def insert_generated_blocks(source, blocks):
    lines = source.splitlines()
    directive = re.compile(r"^\s*//\s*(?:COMMON|LOWERING|REWRITES)(?:-[A-Z0-9_-]+)?:")
    index = next((index for index, line in enumerate(lines) if directive.match(line)), len(lines))
    lines[index:index] = ["\n\n".join(blocks), ""]
    return "\n".join(lines).rstrip("\n") + "\n"


def translate_output(path, command, environment=None):
    env = os.environ.copy()
    if environment:
        env.update(environment)
    clang_args = shlex.split(env.get("SLATE_CLANG_ARGS", ""))
    env["SLATE_CLANG_ARGS"] = shlex.join([*clang_args, "-std=c23"])
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
        rust = translate_output(temporary, command, environment)
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


def update_path(path, profiles, in_place, target_mode):
    if path.parent.name in TARGET_FIXTURE_DIRECTORIES and not target_mode:
        print(f"skip: {path}: target fixture requires --target", file=sys.stderr)
        return
    source = path.read_text()
    instrumented, annotations = instrument_annotations(source)
    if target_mode:
        source = remove_generated_block(remove_generated_block(source, "LOWERING"), "REWRITES")
    updated = source
    blocks = []
    for profile, command, environment in profiles:
        if annotations and not profile.startswith("REWRITES"):
            updated = remove_generated_block(updated, profile)
            continue
        if has_handwritten_block(updated, profile):
            print(
                f"skip: {path}: handwritten {profile} directives present; leaving block untouched",
                file=sys.stderr,
            )
            continue
        updated = remove_generated_block(updated, profile)
        if annotations:
            rewritten = normalize_source_paths(
                translate_instrumented(path, instrumented, command, environment), path
            )
            lowered_command = [*command[:-1], "translate-lowered"]
            lowered = normalize_source_paths(
                translate_instrumented(
                    path, instrumented, lowered_command, environment
                ),
                path,
            )
            checks = render_annotation_checks(
                annotations, rewritten, lowered, profile
            )
        else:
            rust = normalize_source_paths(
                translate_output(path, command, environment), path
            )
            checks = render_checks(rust, profile)
        if not checks:
            raise RuntimeError(f"{path}: generated Rust contains no functions")
        blocks.append(generated_block(checks, profile))
    if blocks:
        updated = insert_generated_blocks(updated, blocks)
    if in_place:
        path.write_text(updated)
    else:
        sys.stdout.write(updated)


def main(argv):
    parser = argparse.ArgumentParser(description="generate stable Slate FileCheck scaffolding")
    parser.add_argument("paths", nargs="+", type=Path)
    parser.add_argument("--profile", choices=("lowering", "rewrites", "both"), default="both")
    parser.add_argument("--in-place", action="store_true")
    parser.add_argument("--slate", default="cargo run --quiet --")
    parser.add_argument(
        "--target",
        action="append",
        metavar="PREFIX=TRIPLE",
        help="generate target-qualified checks; may be repeated",
    )
    args = parser.parse_args(argv)
    slate = args.slate.split()
    profiles = []
    targets = []
    for target in args.target or []:
        prefix, separator, triple = target.partition("=")
        if not separator or not prefix or not triple:
            parser.error(f"--target must be PREFIX=TRIPLE, got {target!r}")
        targets.append((prefix, target_environment(triple)))
    if args.profile in ("lowering", "both"):
        for prefix, environment in targets or [("", {})]:
            profiles.append((f"LOWERING-{prefix}".rstrip("-"), [*slate, "translate-lowered"], environment))
    if args.profile in ("rewrites", "both"):
        for prefix, environment in targets or [("", {})]:
            profiles.append((f"REWRITES-{prefix}".rstrip("-"), [*slate, "translate"], environment))
    for path in args.paths:
        update_path(path, profiles, args.in_place, bool(targets))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main(sys.argv[1:]))
    except (RuntimeError, ValueError) as error:
        print(f"error: {error}", file=sys.stderr)
        raise SystemExit(1)
