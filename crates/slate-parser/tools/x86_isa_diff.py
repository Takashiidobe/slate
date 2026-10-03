#!/usr/bin/env python3

import argparse
import concurrent.futures
import json
import re
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SLATE = ROOT.parents[1] / "target/test-cache/release/slate-parser"
TARGET_MACROS = re.compile(r"__FLT16_\w+|__GCC_HAVE_SYNC_COMPARE_AND_SWAP_[1248]")
RICH_CPU = "diamondrapids"


def clang_macros(clang, triple, args):
    result = subprocess.run(
        [clang, f"--target={triple}", *args, "-dM", "-E", "-x", "c", "/dev/null"],
        capture_output=True,
        text=True,
    )
    if result.returncode:
        return None
    names = (line.split()[1] for line in result.stdout.splitlines() if line.startswith("#define "))
    return {name for name in names if not TARGET_MACROS.fullmatch(name)}


def slate_macros(slate, probe, triple, args):
    result = subprocess.run(
        [str(slate), "pp", str(probe), "--flavor=clang", f"-target={triple}", *args],
        capture_output=True,
        text=True,
    )
    if result.returncode:
        return None
    return set(re.findall(r"\bpresent(\w+)", result.stdout))


def x86_flags(llvm_tblgen, llvm_project):
    with tempfile.NamedTemporaryFile(suffix=".json") as out:
        includes = ["llvm/include", "clang/include", "clang/include/clang/Options"]
        subprocess.run(
            [str(llvm_tblgen), "--dump-json", *[f"-I{llvm_project / i}" for i in includes],
             str(llvm_project / "clang/include/clang/Options/Options.td"), "-o", out.name],
            check=True,
        )
        options = json.loads(Path(out.name).read_text())
    return sorted(
        options[name]["Name"][1:]
        for name in options["!instanceof"]["Option"]
        if (group := options[name].get("Group")) and group["def"] == "m_x86_Features_Group"
        and not options[name]["Name"].startswith("mno-")
        and not options[name]["Name"].endswith("=")
    )


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--llvm-tblgen", type=Path, required=True)
    parser.add_argument("--llvm-project", type=Path, required=True)
    parser.add_argument("--clang", default="clang")
    parser.add_argument("--slate", type=Path, default=SLATE)
    parser.add_argument("--report", type=Path, default=ROOT / "target/x86-isa-diff.json")
    args = parser.parse_args()

    flags = x86_flags(args.llvm_tblgen, args.llvm_project)
    cpus = sorted(
        line.split()[0]
        for line in subprocess.run(
            [args.clang, "--target=x86_64-unknown-linux-gnu", "--print-supported-cpus"],
            capture_output=True, text=True,
        ).stderr.splitlines() + subprocess.run(
            [args.clang, "--target=x86_64-unknown-linux-gnu", "--print-supported-cpus"],
            capture_output=True, text=True,
        ).stdout.splitlines()
        if line.startswith("\t") and line.split()
    )
    cases = []
    for triple in ("x86_64-unknown-linux-gnu", "i686-unknown-linux-gnu"):
        cases += [(triple, [f"-march={cpu}"]) for cpu in cpus]
        cases += [(triple, [f"-m{flag}"]) for flag in flags]
    cases += [("x86_64-unknown-linux-gnu", [f"-march={RICH_CPU}", f"-mno-{flag}"]) for flag in flags]

    with concurrent.futures.ThreadPoolExecutor(32) as pool:
        expected = list(pool.map(lambda case: clang_macros(args.clang, *case), cases))
        accepted = [macros for macros in expected if macros is not None]
        varying = set.union(*accepted) - set.intersection(*accepted)
        with tempfile.TemporaryDirectory() as scratch:
            probe = Path(scratch) / "probe.c"
            probe.write_text("".join(f"#ifdef {m}\npresent{m}\n#endif\n" for m in sorted(varying)))
            actual = list(pool.map(lambda case: slate_macros(args.slate, probe, *case), cases))

    failures, unsupported = [], []
    for (triple, flags_used), want, got in zip(cases, expected, actual):
        case = f"{triple} {' '.join(flags_used)}"
        if want is None and got is None:
            continue
        if want is None:
            failures.append({"case": case, "problem": "slate accepts what clang rejects"})
        elif got is None:
            unsupported.append(case)
        else:
            want &= varying
            if want != got:
                failures.append({"case": case, "missing": sorted(want - got), "extra": sorted(got - want)})
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps({"failures": failures, "unsupported": unsupported}, indent=1) + "\n")
    print(f"{len(cases)} cases, {len(failures)} mismatches, {len(unsupported)} rejected by slate; report: {args.report}")
    raise SystemExit(1 if failures else 0)


if __name__ == "__main__":
    main()
