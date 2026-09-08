#!/usr/bin/env python3
import argparse
import hashlib
import json
import subprocess
import sys
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
SOURCE = ROOT / "tools/probes/linux-libc-abi.c"


def command(args):
    result = subprocess.run(args, text=True, capture_output=True)
    if result.returncode:
        sys.stderr.write(f"command failed: {' '.join(args)}\n")
        sys.stderr.write(result.stdout)
        sys.stderr.write(result.stderr)
        raise SystemExit(result.returncode)
    return result.stdout


def parse_probe(output):
    records = {}
    for line_number, line in enumerate(output.splitlines(), 1):
        fields = line.split("\t")
        if len(fields) != 3:
            raise ValueError(f"line {line_number}: expected three tab-separated fields")
        kind, name, value = fields
        key = f"{kind}:{name}"
        if key in records:
            raise ValueError(f"line {line_number}: duplicate record {key}")
        try:
            value = int(value, 10)
        except ValueError as error:
            raise ValueError(f"line {line_number}: non-numeric value {value!r}") from error
        records[key] = {"kind": kind, "name": name, "value": value}
    if records.pop("schema:version", None) != {
        "kind": "schema",
        "name": "version",
        "value": 1,
    }:
        raise ValueError("missing or unsupported probe schema")
    return records


def probe(args):
    compile_args = [args.compiler, *args.compiler_arg]
    if args.target:
        compile_args.append(f"--target={args.target}")
    if args.sysroot:
        compile_args.append(f"--sysroot={args.sysroot}")
    compile_args.extend(["-std=gnu23", "-O0"])
    if args.shim:
        compile_args.extend(["-nostdlibinc", "-isystem", str(args.shim)])
    compile_args.extend(args.define)
    compile_args.extend(args.extra_arg)

    with tempfile.TemporaryDirectory(prefix="slate-libc-probe-") as directory:
        executable = Path(directory) / "probe"
        command(compile_args + [str(SOURCE), "-o", str(executable)])
        run_args = [*args.runner, *args.runner_arg, str(executable)]
        output = command(run_args)

    result = {
        "schema": 1,
        "probe": SOURCE.name,
        "source_sha256": hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
        "mode": "shim" if args.shim else "oracle",
        "compiler": args.compiler,
        "compiler_args": args.compiler_arg,
        "target": args.target,
        "sysroot": args.sysroot,
        "defines": args.define,
        "records": parse_probe(output),
    }
    return result


def compare(left, right):
    if left.get("probe") != right.get("probe"):
        raise ValueError("probe names differ")
    if left.get("source_sha256") != right.get("source_sha256"):
        raise ValueError("probe sources differ")
    left_records = left["records"]
    right_records = right["records"]
    mismatches = []
    for name in sorted(set(left_records) | set(right_records)):
        if name not in left_records:
            mismatches.append((name, None, right_records[name]))
        elif name not in right_records:
            mismatches.append((name, left_records[name], None))
        elif left_records[name] != right_records[name]:
            mismatches.append((name, left_records[name], right_records[name]))
    return mismatches


def load(path):
    with path.open() as stream:
        value = json.load(stream)
    if value.get("schema") != 1 or "records" not in value:
        raise ValueError(f"unsupported probe result: {path}")
    return value


def parser():
    result = argparse.ArgumentParser()
    subparsers = result.add_subparsers(dest="command", required=True)

    run = subparsers.add_parser("run")
    run.add_argument("--compiler", required=True)
    run.add_argument("--compiler-arg", action="append", default=[])
    run.add_argument("--target", required=True)
    run.add_argument("--sysroot", required=True)
    run.add_argument("--shim", type=Path)
    run.add_argument("--define", action="append", default=[])
    run.add_argument("--extra-arg", action="append", default=[])
    run.add_argument("--runner", action="append", default=[])
    run.add_argument("--runner-arg", action="append", default=[])
    run.add_argument("--output", type=Path)

    compare_parser = subparsers.add_parser("compare")
    compare_parser.add_argument("oracle", type=Path)
    compare_parser.add_argument("candidate", type=Path)

    return result


def main():
    args = parser().parse_args()
    if args.command == "run":
        result = probe(args)
        rendered = json.dumps(result, indent=2, sort_keys=True) + "\n"
        if args.output:
            args.output.parent.mkdir(parents=True, exist_ok=True)
            args.output.write_text(rendered)
        else:
            sys.stdout.write(rendered)
        return

    try:
        oracle = load(args.oracle)
        candidate = load(args.candidate)
        mismatches = compare(oracle, candidate)
    except (OSError, ValueError, KeyError) as error:
        print(f"probe comparison failed: {error}", file=sys.stderr)
        raise SystemExit(2) from error
    if not mismatches:
        print("probe results match")
        return
    for name, expected, actual in mismatches:
        print(f"{name}: oracle={expected!r} candidate={actual!r}")
    raise SystemExit(1)


if __name__ == "__main__":
    main()
