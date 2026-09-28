#!/usr/bin/env python3
import argparse
import collections
import json
import os
import re
import shlex
import subprocess
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path.home() / "gcc-sparse/gcc/testsuite/gcc.dg"
REPO = Path(__file__).resolve().parent.parent
SLATE = REPO / "target/release/slate-parser"
FIXTURES = REPO / "tests/fixtures/suites/gcc-dg"
FLAVORS = ("gcc", "clang")

SKIP_DIRS = {"rtl", "plugin", "pch", "lto", "Foundation.framework", "html-output", "sarif-output", "compat", "simulate-thread", "tree-prof"}
SLATE_F = {"wrapv", "trapv", "strict-overflow", "rounding-math", "trapping-math", "gnu89-inline", "common"}

DG_OPTIONS = re.compile(r"\{\s*dg-(?:additional-)?options\s+\"([^\"]*)\"(.*?)\}")
DG_DO = re.compile(r"\{\s*dg-do\s+(\w+)(.*?)\}")
DG_TARGET_REQ = re.compile(r"dg-require-effective-target\s+(\S+)")


def parse_directives(text):
    options = []
    selectors = []
    for match in DG_OPTIONS.finditer(text):
        tail = match.group(2)
        if "target" in tail and not re.search(r"x86_64|i\?86|\*-\*-\*|linux|lp64", tail):
            continue
        try:
            options += shlex.split(match.group(1), posix=True)
        except ValueError:
            options += match.group(1).split()
    do = DG_DO.search(text)
    kind = do.group(1) if do else "compile"
    if do and "target" in do.group(2):
        selectors.append(do.group(2).strip())
    return kind, options, selectors, DG_TARGET_REQ.findall(text)


def split_flags(options):
    std = []
    common = []
    unsupported = []
    for opt in options:
        if opt.startswith("-std="):
            std = [opt]
        elif opt == "-ansi":
            std = ["-std=c89"]
        elif opt.startswith(("-D", "-U", "-Wno-")):
            common.append(opt)
        elif opt.startswith("-f"):
            name = opt[2:].removeprefix("no-")
            if name in SLATE_F:
                common.append(opt)
            elif name in ("ms-extensions", "plan9-extensions", "signed-char", "unsigned-char", "short-enums", "short-wchar", "permissive", "freestanding", "hosted", "no-builtin", "builtin", "dollars-in-identifiers", "extended-identifiers", "input-charset", "exec-charset", "wide-exec-charset", "openmp", "openacc", "cilkplus", "gnu-tm", "short-double", "pack-struct", "allow-parameterless-variadic-functions") or name.startswith(("pack-struct", "exec-charset", "input-charset", "wide-exec-charset", "openmp", "openacc", "strict-flex-arrays", "sso-struct")):
                unsupported.append(opt)
        elif opt.startswith("-m") and opt in ("-m32", "-mx32", "-m16"):
            unsupported.append(opt)
    return std, common, unsupported


def run(cmd, cwd):
    try:
        proc = subprocess.run(cmd, cwd=cwd, capture_output=True, timeout=20, env={**os.environ, "NO_COLOR": "1"})
        return proc.returncode, proc.stderr.decode(errors="replace")
    except subprocess.TimeoutExpired:
        return "timeout", ""


ANSI_DIRS = {"cpp", "autopar", "fixed-point", "ipa", "tls", "weak", "tree-ssa", "tm", "debug", "vxworks"}


def default_options(rel):
    top = rel.parts[0] if len(rel.parts) > 1 else ""
    if top == "" or top in ANSI_DIRS or rel.parts[:2] == ("torture", "tls"):
        return ["-ansi"]
    if top == "dfp":
        return ["-std=gnu99"]
    return []


def one(path):
    rel = path.relative_to(ROOT)
    text = path.read_text(errors="replace")
    kind, options, selectors, requires = parse_directives(text)
    if not DG_OPTIONS.search(text):
        options = default_options(rel)
    std, common, unsupported = split_flags(options)
    std = std or ["-std=gnu23"]
    oracle_flags = [*std, *common, *[o for o in unsupported if o not in ("-fopenmp",)], "-I", str(path.parent), str(path)]
    result = {
        "file": str(rel),
        "kind": kind,
        "std": std[0],
        "flags": common,
        "unsupported": unsupported,
        "selectors": selectors,
        "requires": requires,
        "dg_error": "dg-error" in text,
    }
    for flavor in FLAVORS:
        rc, err = run([flavor, "-fsyntax-only", *oracle_flags], path.parent)
        result[flavor] = rc
        result[f"{flavor}_err"] = err[-1500:]
        rc, err = run([str(SLATE), "parse", str(path), f"--flavor={flavor}", *std, *common, "-I" + str(path.parent), "--dump-ir"], path.parent)
        result[f"slate_{flavor}"] = rc
        result[f"slate_{flavor}_err"] = err.replace(str(path), rel.name)[-3000:]
    return result


def status(result, flavor):
    oracle, slate = result[flavor], result[f"slate_{flavor}"]
    if slate == "timeout":
        return "slate-timeout"
    if oracle == 0:
        return "both-accept" if slate == 0 else f"{flavor}-only-accepts"
    return "slate-only-accepts" if slate == 0 else "both-reject"


# directories whose tests check optimizer or tooling output rather than front-end behavior
MIGRATE_SKIP_DIRS = {"vect", "tree-ssa", "graphite", "ipa", "guality", "debug", "analyzer", "asan", "ubsan", "hwasan", "tsan", "gomp", "goacc", "tm", "torture", "lto", "profile-update", "tree-prof"}
MIGRATE_SKIP_FILES = {"cpp/include3.c", "cpp/embed-6.c", "cpp/embed-7.c"}
MAX_IR_LINES = 4000


def migration_blocker(result, text):
    rel = Path(result["file"])
    if rel.parts[0] in MIGRATE_SKIP_DIRS or result["file"] in MIGRATE_SKIP_FILES:
        return "directory"
    if rel.name.startswith("auto-init-"):
        return "auto-init"
    if re.search(r'#\s*include\s*"', text) or "dg-additional-sources" in text or "dg-additional-files" in text:
        return "needs-other-files"
    if re.search(r"#\s*embed\s+__FILE__", text) or "__DATE__" in text or "__TIME__" in text or "SOURCE_DATE_EPOCH" in text:
        return "unstable-output"
    if any(flag.startswith("-U") or (flag.startswith("-D") and " " in flag) for flag in result["flags"]):
        return "flags"
    return None


def fixture_text(result, text):
    defines = [flag[2:] for flag in result["flags"] if flag.startswith("-D")]
    args = [flag for flag in result["flags"] if flag.startswith("-f")]
    lines = [f"// SLATE-FILECHECK-STD DEFAULT {result['std'].removeprefix('-std=')}"]
    if args:
        lines.append(f"// SLATE-FILECHECK-ARGS {' '.join(args)}")
    lines.append("// SLATE-FILECHECK-DEFINES DEFAULT" + "".join(f" {d}" for d in defines))
    return text.rstrip("\n") + "\n\n" + "\n".join(lines) + "\n\n// SLATE-FILECHECK-BEGIN DEFAULT\n// SLATE-FILECHECK-END DEFAULT\n"


def migrate(results, flavor):
    dest = FIXTURES / flavor / "linux" / "x86_64"
    written = collections.Counter()
    for result in results:
        if status(result, flavor) != "both-accept":
            continue
        source = ROOT / result["file"]
        text = source.read_text(errors="replace")
        blocker = migration_blocker(result, text)
        if blocker is None:
            ir = subprocess.run([str(SLATE), "parse", str(source), f"--flavor={flavor}", result["std"], *result["flags"], "--dump-ir"], cwd=source.parent, capture_output=True, timeout=20)
            if ir.stdout.count(b"\n") > MAX_IR_LINES:
                blocker = "large-output"
        if blocker is not None:
            written[f"skipped-{blocker}"] += 1
            continue
        target = dest / "__".join(Path(result["file"]).parts)
        if target.exists():
            written["existing"] += 1
            continue
        target.write_text(fixture_text(result, text))
        written["written"] += 1
    return written


def main():
    global ROOT
    parser = argparse.ArgumentParser(description="Compare slate-parser --flavor=gcc|clang against gcc and clang -fsyntax-only over gcc.dg")
    parser.add_argument("out", type=Path, help="JSON-lines result file")
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--jobs", type=int, default=os.cpu_count())
    parser.add_argument("--migrate", choices=FLAVORS, help="write fixtures for tests this flavor's oracle and slate both accept, reusing the results in OUT instead of sweeping")
    args = parser.parse_args()
    ROOT = args.root
    if args.migrate:
        results = [json.loads(line) for line in args.out.open()]
        print(dict(migrate(results, args.migrate)))
        return
    files = sorted(p for p in ROOT.rglob("*.c") if p.relative_to(ROOT).parts[0] not in SKIP_DIRS)
    counts = {flavor: collections.Counter() for flavor in FLAVORS}
    with ThreadPoolExecutor(max_workers=args.jobs) as pool, args.out.open("w") as out:
        for result in pool.map(one, files):
            for flavor in FLAVORS:
                counts[flavor][status(result, flavor)] += 1
            out.write(json.dumps(result) + "\n")
    for flavor in FLAVORS:
        print(flavor, dict(counts[flavor]))


main()
