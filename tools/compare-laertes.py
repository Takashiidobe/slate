#!/usr/bin/env python3
import argparse
import collections
import json
import pathlib
import re
import shlex
import subprocess
import tempfile


ROOT = pathlib.Path(__file__).resolve().parents[1]
SLATE = ROOT / "target/test-cache/release/slate"
FUNCTION = re.compile(r"\bfn\s+([A-Za-z_][A-Za-z_0-9]*)\s*(?:<[^{};]*>)?\s*\(")
RAW_POINTER = re.compile(r"\*(?:const|mut)\b")


def function_parameters(source):
    functions = collections.defaultdict(list)
    for match in FUNCTION.finditer(source):
        open_paren = match.end() - 1
        depth = 1
        end = open_paren + 1
        while end < len(source) and depth:
            if source[end] == "(":
                depth += 1
            elif source[end] == ")":
                depth -= 1
            end += 1
        if depth:
            continue
        params = source[open_paren + 1 : end - 1]
        start = 0
        nesting = []
        raw_params = []
        for index, char in enumerate(params):
            if char in "([{<":
                nesting.append({"(": ")", "[": "]", "{": "}", "<": ">"}[char])
            elif nesting and char == nesting[-1]:
                nesting.pop()
            elif char == "," and not nesting:
                parameter = params[start:index].strip()
                if RAW_POINTER.search(parameter):
                    raw_params.append(parameter)
                start = index + 1
        parameter = params[start:].strip()
        if RAW_POINTER.search(parameter):
            raw_params.append(parameter)
        line = source.count("\n", 0, match.start()) + 1
        functions[match.group(1)].append((line, raw_params))
    return functions


def scan_rust(directory):
    functions = collections.defaultdict(list)
    for path in sorted(directory.rglob("*.rs")):
        if ".git" in path.parts or path.parent.name == "target":
            continue
        try:
            source = path.read_text(errors="replace")
        except OSError:
            continue
        for name, declarations in function_parameters(source).items():
            for line, params in declarations:
                functions[name].append((path, line, params))
    return functions


def pointer_count(functions, names):
    return sum(len(params) for name in names for _, _, params in functions.get(name, []))


def slate_binary(path):
    if path.exists():
        return path
    subprocess.run(["cargo", "build", "--release", "--bin", "slate"], cwd=ROOT, check=True)
    if not path.exists():
        raise FileNotFoundError(f"Slate binary not found after build: {path}")
    return path


def compare(args):
    project = args.project.resolve()
    database = args.compile_commands.resolve()
    laertes_dir = args.laertes_dir.resolve()
    if not project.is_dir():
        raise ValueError(f"project directory does not exist: {project}")
    if not database.is_file():
        raise ValueError(f"compile_commands.json does not exist: {database}")
    if not laertes_dir.is_dir():
        raise ValueError(f"Laertes Rust directory does not exist: {laertes_dir}")
    entries = json.loads(database.read_text())
    if not isinstance(entries, list) or not entries:
        raise ValueError(f"compile database has no commands: {database}")

    binary = slate_binary(args.slate_binary.resolve())
    if args.slate_output:
        output = args.slate_output.resolve()
        output.parent.mkdir(parents=True, exist_ok=True)
        if output.exists() and any(output.iterdir()):
            raise ValueError(f"Slate output directory is not empty: {output}")
        run_translation(binary, args, project, database, output)
        report(laertes_dir, output / "src", database, output)
    else:
        with tempfile.TemporaryDirectory(prefix=f"slate-{project.name}-") as temporary:
            output = pathlib.Path(temporary) / "slate-output"
            run_translation(binary, args, project, database, output)
            report(laertes_dir, output / "src", database, output)


def run_translation(binary, args, project, database, output):
    command = [str(binary), "translate-project"]
    if args.flavor:
        command.extend(["--flavor", args.flavor])
    command.extend(["--compile-commands", str(database), str(project), str(output)])
    print("$ " + shlex.join(command), flush=True)
    subprocess.run(command, cwd=ROOT, check=True)


def report(laertes_dir, slate_dir, database, output):
    laertes = scan_rust(laertes_dir)
    slate = scan_rust(slate_dir)
    common = sorted(laertes.keys() & slate.keys())
    laertes_only = sorted(laertes.keys() - slate.keys())
    slate_only = sorted(slate.keys() - laertes.keys())
    before = pointer_count(laertes, common)
    after = pointer_count(slate, common)

    print(f"compile database: {database}")
    print(f"Slate output: {output}")
    print(f"matched functions: {len(common)}")
    print(f"Laertes raw-pointer parameters in matched functions: {before}")
    print(f"Slate raw-pointer parameters in matched functions:   {after}")
    print(f"Slate minus Laertes: {after - before:+d}")
    print("\nfunction                         Laertes  Slate")
    print("-------------------------------- -------  -----")
    for name in common:
        left = pointer_count(laertes, [name])
        right = pointer_count(slate, [name])
        print(f"{name:32} {left:7}  {right:5}")
        for label, functions in (("Laertes", laertes), ("Slate", slate)):
            for path, line, params in functions[name]:
                if params:
                    rendered = "; ".join(params)
                    print(f"  {label} {path}:{line}: {rendered}")
    if laertes_only:
        print("\nLaertes-only functions: " + ", ".join(laertes_only))
    if slate_only:
        print("Slate-only functions: " + ", ".join(slate_only))
    print("\nCounts are syntactic raw-pointer parameters; they do not establish rewrite soundness.")


def main():
    parser = argparse.ArgumentParser(
        description="Translate a C project with Slate and compare raw-pointer parameters with Laertes Rust output."
    )
    parser.add_argument("project", type=pathlib.Path, help="C project directory")
    parser.add_argument("--compile-commands", required=True, type=pathlib.Path,
                        help="compile_commands.json used for both project context and Slate translation")
    parser.add_argument("--laertes-dir", required=True, type=pathlib.Path,
                        help="directory containing Laertes-transformed Rust source")
    parser.add_argument("--slate-output", type=pathlib.Path,
                        help="keep Slate's generated crate here; defaults to a temporary directory")
    parser.add_argument("--slate-binary", type=pathlib.Path, default=SLATE)
    parser.add_argument("--flavor", choices=("clang", "gcc", "msvc"))
    args = parser.parse_args()
    try:
        compare(args)
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        parser.exit(1, f"compare-laertes: {error}\n")


if __name__ == "__main__":
    main()
