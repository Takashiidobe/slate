#!/usr/bin/env python3
"""Time slate's interprocedural passes on generated C of growing size.

Run from the workspace root:

    python3 tools/benchmark/interproc_scaling.py
    python3 tools/benchmark/interproc_scaling.py --shapes fanout,scc_ring --sizes 100,200,400,800
    python3 tools/benchmark/interproc_scaling.py --json target/interproc-scaling.json

Each shape stresses one axis interprocedural cost depends on. For every
shape and N the script translates the generated file with
SLATE_INTERPROC_TIMING=1, keeps the fastest of --runs samples per phase,
and prints per-pass time split into extract (facts read from the Rust AST),
propagate (call-graph fixpoint), apply (rewrite), and mixed (passes that
interleave analysis and rewrite). The exponent column is the least-squares
slope of log(ms) over log(N) across the largest four sizes: 1 is linear,
2 is quadratic.
"""

import argparse
import json
import math
import os
import re
import subprocess
import sys
import tempfile
import time
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SLATE = ROOT / "target/test-cache/release/slate"


def chain_ptr(n):
    out = []
    out.append(f"static int f{n}(int *p, int n) {{ return n > 0 ? p[0] : 0; }}")
    for i in reversed(range(n)):
        out.append(
            f"static int f{i}(int *p, int n) {{\n"
            f"    int *q = p;\n"
            f"    q[0] += {i};\n"
            f"    q++;\n"
            f"    return f{i + 1}(q, n - 1);\n"
            f"}}"
        )
    out.append(f"int main(void) {{ static int a[{n + 1}]; return f0(a, {n + 1}); }}")
    return "\n".join(out), []


def chain_ptr_wrapv(n):
    src, _ = chain_ptr(n)
    return src, ["-fwrapv"]


def chain_kill_leaf(n):
    out = ["#include <stdlib.h>", "static int *sink;"]
    out.append(f"static int f{n}(int *p) {{ sink = p; return 0; }}")
    for i in reversed(range(n)):
        out.append(f"static int f{i}(int *p) {{ return p[0] + f{i + 1}(p); }}")
    out.append("int main(void) { int a[1] = {0}; return f0(a); }")
    return "\n".join(out), []


def chain_kill_root(n):
    out = ["#include <stddef.h>", "static int g;"]
    out.append(f"static int f{n}(int *p) {{ return p ? *p : 0; }}")
    for i in reversed(range(n)):
        out.append(f"static int f{i}(int *p) {{ return f{i + 1}(p); }}")
    out.append("int main(int argc, char **argv) { (void)argv; return f0(argc > 1 ? &g : NULL); }")
    return "\n".join(out), []


def scc_ring(n):
    out = [f"static int f{i}(int *p, int d);" for i in range(n)]
    for i in range(n):
        out.append(
            f"static int f{i}(int *p, int d) {{\n"
            f"    if (d <= 0) return p[0];\n"
            f"    p[0] += 1;\n"
            f"    return f{(i + 1) % n}(p, d - 1);\n"
            f"}}"
        )
    out.append("int main(void) { int a[1] = {0}; return f0(a, 3); }")
    return "\n".join(out), []


def fanout(n):
    out = ["static int hub(int *p, int k) { return p[k]; }"]
    for i in range(n):
        out.append(
            f"static int c{i}(int *p) {{ int *q = p + 1; return hub(q, {i % 4}); }}"
        )
    calls = " + ".join(f"c{i}(a)" for i in range(n))
    out.append(f"int main(void) {{ int a[8] = {{0}}; return {calls}; }}")
    return "\n".join(out), []


def str_chain(n):
    out = ["#include <string.h>"]
    out.append(f"static int s{n}(const char *s) {{ return (int)strlen(s) + s[0]; }}")
    for i in reversed(range(n)):
        out.append(
            f"static int s{i}(const char *s) {{ const char *t = s; return s{i + 1}(t); }}"
        )
    out.append('int main(void) { return s0("hello"); }')
    return "\n".join(out), []


def buf_chain(n):
    out = []
    out.append(
        f"static int b{n}(int *p, int len) {{ int t = 0; for (int i = 0; i < len; i++) t += p[i]; return t; }}"
    )
    for i in reversed(range(n)):
        out.append(f"static int b{i}(int *p, int len) {{ return b{i + 1}(p, len); }}")
    out.append("int main(void) { int a[16] = {0}; return b0(a, 16); }")
    return "\n".join(out), []


def wide_locals(n):
    body = ["    int *q0 = p;"]
    for i in range(1, n + 1):
        body.append(f"    int *q{i} = q{i - 1} + 1;")
        body.append(f"    t += q{i}[0];")
    out = [
        "static int w(int *p) {",
        "    int t = 0;",
        *body,
        "    return t;",
        "}",
        f"int main(void) {{ static int a[{n + 2}]; return w(a); }}",
    ]
    return "\n".join(out), []


def fields(n):
    members = "\n".join(f"    int *m{i};" for i in range(n))
    out = [f"struct S {{\n{members}\n}};"]
    for i in range(n):
        out.append(
            f"static int g{i}(struct S *s, int *p) {{ s->m{i} = p; return s->m{i}[0]; }}"
        )
    calls = " + ".join(f"g{i}(&s, a)" for i in range(n))
    out.append(f"int main(void) {{ struct S s; int a[1] = {{0}}; return {calls}; }}")
    return "\n".join(out), []


def rev_alias(n, elem, use):
    names = [f"t{i}" for i in range(1, n + 1)]
    decls = "\n".join(f"    {elem} *{t} = 0;" for t in names)
    assigns = "\n".join(
        f"        {names[i]} = {names[i - 1] if i else 'p'};" for i in reversed(range(n))
    )
    return (
        f"{decls}\n"
        f"    for (int k = 0; k < {n}; k++) {{\n{assigns}\n    }}\n"
        f"    {use.format(t=names[-1])}"
    )


def rev_alias_str(n):
    body = rev_alias(n, "const char", "return (int)strlen({t});")
    src = (
        "#include <string.h>\n"
        f"static int use(const char *p) {{\n{body}\n}}\n"
        'int main(void) { return use("hello"); }'
    )
    return src, []


def rev_alias_ptr(n):
    body = rev_alias(n, "int", "{t}[0] = 1; return {t}[0];")
    src = (
        f"static int use(int *p) {{\n{body}\n}}\n"
        "int main(void) { int a[1] = {0}; return use(a); }"
    )
    return src, []


def hub_kill(n):
    out = ["static int hub(int *p) { return p[0]; }"]
    for i in range(n):
        out.append(f"static int d{i}(int *p) {{ p[0] = {i}; return 0; }}")
        out.append(f"static int c{i}(int *p) {{ return hub(p) + d{i}(p); }}")
    calls = " + ".join(f"c{i}(a)" for i in range(n))
    out.append(f"int main(void) {{ int a[1] = {{0}}; return {calls}; }}")
    return "\n".join(out), []


def str_hub_kill(n):
    out = ["#include <string.h>", "static int hub(const char *s) { return (int)strlen(s); }"]
    for i in range(n):
        out.append(f"static int d{i}(const char *s) {{ return s[0] == {i % 128}; }}")
        out.append(f"static int c{i}(const char *s) {{ return hub(s) + d{i}(s); }}")
    out.append("static char buf[8];")
    calls = " + ".join(f"c{i}({'buf' if i == n - 1 else chr(34) + 'x' + chr(34)})" for i in range(n))
    out.append(f"int main(void) {{ return {calls}; }}")
    return "\n".join(out), []


SHAPES = {
    "chain_ptr": chain_ptr,
    "chain_ptr_wrapv": chain_ptr_wrapv,
    "chain_kill_leaf": chain_kill_leaf,
    "chain_kill_root": chain_kill_root,
    "scc_ring": scc_ring,
    "fanout": fanout,
    "str_chain": str_chain,
    "buf_chain": buf_chain,
    "wide_locals": wide_locals,
    "fields": fields,
    "rev_alias_str": rev_alias_str,
    "rev_alias_ptr": rev_alias_ptr,
    "hub_kill": hub_kill,
    "str_hub_kill": str_hub_kill,
}

TIMING = re.compile(r"^interproc-timing pass=(\S+) phase=(\S+) ms=([0-9.]+)$")
COUNT = re.compile(r"^interproc-count pass=(\S+) name=(\S+) value=(\d+)$")


def category(phase):
    return phase.split(".", 1)[0]


def translate_once(path, flags):
    env = dict(os.environ, SLATE_INTERPROC_TIMING="1")
    start = time.perf_counter()
    proc = subprocess.run(
        [str(SLATE), "translate", *flags, str(path)],
        env=env,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.PIPE,
        text=True,
    )
    wall = (time.perf_counter() - start) * 1e3
    if proc.returncode != 0:
        raise RuntimeError(f"translate failed on {path}:\n{proc.stderr[-2000:]}")
    phases = defaultdict(float)
    counts = {}
    for line in proc.stderr.splitlines():
        if m := TIMING.match(line):
            phases[(m[1], category(m[2]))] += float(m[3])
        elif m := COUNT.match(line):
            counts[(m[1], m[2])] = int(m[3])
    return wall, phases, counts


def sqlite_phases(corpus, workdir):
    sys.path.insert(0, str(Path(__file__).parent))
    import sqlite

    units = sqlite.configuration(corpus / "build-clang/compile_commands.json")
    database = Path(workdir) / "sqlite_compile_commands.json"
    database.write_text(json.dumps([
        {"directory": str(ROOT), "file": unit["source"],
         "arguments": ["clang", *unit["flags"], "-D__PIC__=2", "-D__pic__=2", "-c", unit["source"]]}
        for unit in units.values()
    ]))
    env = dict(os.environ, SLATE_INTERPROC_TIMING="1")
    start = time.perf_counter()
    proc = subprocess.run(
        [str(SLATE), "translate-project", "--compile-commands", str(database), str(corpus),
         str(Path(workdir) / "sqlite-rust")],
        env=env, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, text=True,
    )
    wall = (time.perf_counter() - start) * 1e3
    if proc.returncode != 0:
        raise RuntimeError(f"sqlite translation failed:\n{proc.stderr[-2000:]}")
    print(f"\n## sqlite (translate-project wall {wall / 1e3:.1f} s)")
    totals = defaultdict(float)
    for line in proc.stderr.splitlines():
        if m := TIMING.match(line):
            print(f"{m[1]:<16} {m[2]:<22} {float(m[3]):>9.1f} ms")
            if m[1] != "interproc":
                totals[category(m[2])] += float(m[3])
        elif m := COUNT.match(line):
            print(f"{m[1]:<16} #{m[2]:<21} {m[3]:>9}")
    print("by category: " + ", ".join(f"{c} {ms:.0f} ms" for c, ms in sorted(totals.items())))


def measure(shape, n, runs, workdir):
    src, flags = SHAPES[shape](n)
    path = Path(workdir) / f"{shape}_{n}.c"
    path.write_text(src + "\n")
    best_wall = math.inf
    best = {}
    counts = {}
    for _ in range(runs):
        wall, phases, counts = translate_once(path, flags)
        best_wall = min(best_wall, wall)
        for key, ms in phases.items():
            best[key] = min(best.get(key, math.inf), ms)
    return best_wall, best, counts


def slope(points):
    pts = [(math.log(n), math.log(ms)) for n, ms in points if ms > 0]
    if len(pts) < 2:
        return None
    mx = sum(x for x, _ in pts) / len(pts)
    my = sum(y for _, y in pts) / len(pts)
    den = sum((x - mx) ** 2 for x, _ in pts)
    return sum((x - mx) * (y - my) for x, y in pts) / den if den else None


def report(shape, sizes, rows):
    keys = sorted({k for _, phases, _ in rows for k in phases})
    print(f"\n## {shape}")
    header = ["pass", "phase", *[f"N={n}" for n in sizes], "exp"]
    table = [header]
    walls = [wall for wall, _, _ in rows]
    table.append(["translate", "wall", *[f"{w:.1f}" for w in walls], fmt_exp(slope(list(zip(sizes, walls))[-4:]))])
    for key in keys:
        series = [phases.get(key, 0.0) for _, phases, _ in rows]
        if max(series) < 0.05:
            continue
        exp = slope(list(zip(sizes, series))[-4:]) if series[-1] >= 0.5 else None
        table.append([key[0], key[1], *[f"{v:.2f}" for v in series], fmt_exp(exp)])
    count_keys = sorted({k for _, _, counts in rows for k in counts})
    for key in count_keys:
        series = [counts.get(key, 0) for _, _, counts in rows]
        exp = slope(list(zip(sizes, series))[-4:]) if series[-1] > 0 else None
        table.append([key[0], f"#{key[1]}", *[str(v) for v in series], fmt_exp(exp)])
    widths = [max(len(r[i]) for r in table) for i in range(len(header))]
    for row in table:
        print("  ".join(cell.ljust(w) if i < 2 else cell.rjust(w) for i, (cell, w) in enumerate(zip(row, widths))))


def fmt_exp(value):
    return "-" if value is None else f"{value:.2f}"


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--shapes", default=",".join(SHAPES))
    parser.add_argument("--sizes", default="50,100,200,400,800")
    parser.add_argument("--runs", type=int, default=3)
    parser.add_argument("--json", type=Path)
    parser.add_argument("--keep", type=Path, help="write generated C here instead of a temp dir")
    parser.add_argument("--no-build", action="store_true")
    parser.add_argument("--sqlite", type=Path, nargs="?", const=Path.home() / "c-corpus/sqlite",
                        help="also time translate-project on the sqlite corpus (default ~/c-corpus/sqlite)")
    args = parser.parse_args()

    shapes = [s for s in args.shapes.split(",") if s]
    unknown = [s for s in shapes if s not in SHAPES]
    if unknown:
        parser.error(f"unknown shapes: {', '.join(unknown)}; known: {', '.join(SHAPES)}")
    sizes = [int(n) for n in args.sizes.split(",")]

    if not args.no_build:
        subprocess.run(["cargo", "build", "--release", "-p", "slate-c2rust"], cwd=ROOT, check=True)

    results = {}
    with tempfile.TemporaryDirectory() as tmp:
        workdir = args.keep or Path(tmp)
        workdir.mkdir(parents=True, exist_ok=True)
        for shape in shapes:
            rows = []
            for n in sizes:
                rows.append(measure(shape, n, args.runs, workdir))
                print(f"{shape} N={n}: {rows[-1][0]:.1f} ms", file=sys.stderr)
            report(shape, sizes, rows)
            results[shape] = [
                {
                    "n": n,
                    "wall_ms": wall,
                    "phases": {f"{p}.{c}": ms for (p, c), ms in phases.items()},
                    "counts": {f"{p}.{c}": v for (p, c), v in counts.items()},
                }
                for n, (wall, phases, counts) in zip(sizes, rows)
            ]
        if args.sqlite:
            sqlite_phases(args.sqlite, workdir)
    if args.json:
        args.json.write_text(json.dumps(results, indent=2) + "\n")


if __name__ == "__main__":
    main()
