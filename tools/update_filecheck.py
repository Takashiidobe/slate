#!/usr/bin/env python3
import argparse
import os
import re
import shlex
import subprocess
import sys
from pathlib import Path


TARGET_FIXTURE_DIRECTORIES = {"bionic", "macos", "msvc"}
SOURCE_ROOT_PATTERN = "__SLATE_FILECHECK_SOURCE_ROOT__"


def render_checks(rust, prefix):
    checks = []
    for index, line in enumerate(rust.splitlines()):
        if line:
            directive = prefix if index == 0 else f"{prefix}-NEXT"
            checks.append(f"// {directive}: {escape_check_pattern(line)}")
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
    if target_mode:
        source = remove_generated_block(remove_generated_block(source, "LOWERING"), "REWRITES")
    updated = source
    blocks = []
    for profile, command, environment in profiles:
        updated = remove_generated_block(updated, profile)
        rust = normalize_source_paths(translate_output(path, command, environment), path)
        checks = render_checks(rust, profile)
        if not checks:
            raise RuntimeError(f"{path}: generated Rust contains no functions")
        blocks.append(generated_block(checks, profile))
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
    except RuntimeError as error:
        print(f"error: {error}", file=sys.stderr)
        raise SystemExit(1)
