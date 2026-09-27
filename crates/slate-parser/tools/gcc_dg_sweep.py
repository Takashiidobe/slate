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
SLATE = Path(__file__).resolve().parent.parent / "target/release/slate-parser"

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
    gcc_rc, gcc_err = run(["gcc", "-fsyntax-only", *std, *common, *[o for o in unsupported if o not in ("-fopenmp",)], "-I", str(path.parent), str(path)], path.parent)
    slate_rc, slate_err = run([str(SLATE), "parse", str(path), "--flavor=gcc", *std, *common, "-I" + str(path.parent), "--dump-ir"], path.parent)
    slate_err = slate_err.replace(str(path), rel.name)
    return {
        "file": str(rel),
        "kind": kind,
        "std": std[0],
        "flags": common,
        "unsupported": unsupported,
        "selectors": selectors,
        "requires": requires,
        "dg_error": "dg-error" in text,
        "gcc": gcc_rc,
        "gcc_err": gcc_err[-1500:],
        "slate": slate_rc,
        "slate_err": slate_err[-3000:],
    }


def status(result):
    if result["slate"] == "timeout":
        return "slate-timeout"
    if result["gcc"] == 0:
        return "both-accept" if result["slate"] == 0 else "gcc-only-accepts"
    return "slate-only-accepts" if result["slate"] == 0 else "both-reject"


def main():
    global ROOT
    parser = argparse.ArgumentParser(description="Compare slate-parser --flavor=gcc against gcc -fsyntax-only over gcc.dg")
    parser.add_argument("out", type=Path, help="JSON-lines result file")
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--jobs", type=int, default=os.cpu_count())
    args = parser.parse_args()
    ROOT = args.root
    files = sorted(p for p in ROOT.rglob("*.c") if p.relative_to(ROOT).parts[0] not in SKIP_DIRS)
    counts = collections.Counter()
    with ThreadPoolExecutor(max_workers=args.jobs) as pool, args.out.open("w") as out:
        for result in pool.map(one, files):
            counts[status(result)] += 1
            out.write(json.dumps(result) + "\n")
    print(dict(counts))


main()
