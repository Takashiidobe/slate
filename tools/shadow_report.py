#!/usr/bin/env python3
"""Compare an IR-side analysis with its Rust-side pass over C fixtures.

Run from the workspace root after building release:

    python3 tools/shadow_report.py crates/slate/tests/fixtures
    python3 tools/shadow_report.py --pass pointer_lattice crates/slate/tests/fixtures.gcc-torture
    python3 tools/shadow_report.py --project ~/c-corpus/sqlite/compile_commands.json
    python3 tools/shadow_report.py --json target/shadow.json crates/slate/tests/fixtures

Each input is translated with SLATE_SHADOW=<pass> and, like the differential
harness, -std=c23 unless its dg-options set a standard. The pass prints one
`slate-shadow` line per binding whose decisions differ (or exist on one side
only) and a summary line. The report groups differences by pass, kind and
(rust -> ir) decision, with example sites, so each group can be triaged as an
improvement (add a fixture) or a bug before the IR port switches.
"""

import argparse
import json
import os
import re
import shlex
import subprocess
import sys
import tempfile
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SLATE = ROOT / "target/test-cache/release/slate"
LINE = re.compile(
    r"^slate-shadow pass=(\S+) kind=(\S+) unit=(\d+) fn=(\S+) binding=(\S+) rust=(\S+) ir=(\S+)$"
)
SUMMARY = re.compile(r"^slate-shadow pass=(\S+) summary (.*)$")
DG_OPTIONS = re.compile(r'dg-options\s+"([^"]*)"')


def fixture_args(path):
    match = DG_OPTIONS.search(path.read_text(errors="replace"))
    args = shlex.split(match[1]) if match else []
    if not any(arg.startswith("-std=") for arg in args):
        args.insert(0, "-std=c23")
    return args


def inputs(paths):
    for path in map(Path, paths):
        if path.is_dir():
            yield from sorted(path.glob("*.c"))
        else:
            yield path


def run(command, passes):
    env = dict(os.environ, SLATE_SHADOW=passes)
    proc = subprocess.run(command, env=env, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, text=True)
    return proc.returncode, proc.stderr


def translate(path, passes):
    return path, *run([str(SLATE), "translate", *fixture_args(path), str(path)], passes)


def parse(site, stderr, rows, totals):
    for line in stderr.splitlines():
        if m := LINE.match(line):
            pass_, kind, _unit, function, binding, rust, ir = m.groups()
            rows.append({"site": str(site), "pass": pass_, "kind": kind, "fn": function,
                         "binding": binding, "rust": rust, "ir": ir})
        elif m := SUMMARY.match(line):
            for field in m[2].split():
                kind, count = field.split("=")
                totals[m[1]][kind] += int(count)


def report(rows, totals, failed, examples):
    for pass_, counts in sorted(totals.items()):
        print(f"## {pass_}: " + ", ".join(f"{k} {v}" for k, v in sorted(counts.items())))
        groups = defaultdict(list)
        for row in rows:
            if row["pass"] == pass_:
                groups[(row["kind"], row["rust"], row["ir"])].append(row)
        for (kind, rust, ir), members in sorted(groups.items(), key=lambda g: -len(g[1])):
            print(f"  {len(members):>6}  {kind:<9} {rust} -> {ir}")
            for row in members[:examples]:
                print(f"          {row['site']}: {row['fn']}::{row['binding']}")
    if failed:
        print(f"\ntranslate failed on {len(failed)} inputs (not compared):")
        for site in failed:
            print(f"  {site}")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("paths", nargs="*", help="C files or directories of C files (not recursive)")
    parser.add_argument("--pass", dest="passes", default="all", help="comma-separated passes, or all")
    parser.add_argument("--project", type=Path, help="compile_commands.json to run translate-project on")
    parser.add_argument("--jobs", type=int, default=os.cpu_count())
    parser.add_argument("--examples", type=int, default=3)
    parser.add_argument("--json", type=Path)
    args = parser.parse_args()
    if not args.paths and not args.project:
        parser.error("give C paths or --project")

    rows, totals, failed = [], defaultdict(Counter), []
    with ThreadPoolExecutor(args.jobs) as pool:
        for path, code, stderr in pool.map(lambda p: translate(p, args.passes), inputs(args.paths)):
            if code != 0:
                failed.append(str(path))
            parse(path, stderr, rows, totals)
    if args.project:
        with tempfile.TemporaryDirectory() as out:
            code, stderr = run([str(SLATE), "translate-project", "--compile-commands", str(args.project),
                                str(args.project.parent), out], args.passes)
        if code != 0:
            failed.append(str(args.project))
        parse(args.project, stderr, rows, totals)

    report(rows, totals, failed, args.examples)
    if args.json:
        args.json.write_text(json.dumps({"differences": rows, "totals": totals, "failed": failed}, indent=2) + "\n")


if __name__ == "__main__":
    sys.exit(main())
