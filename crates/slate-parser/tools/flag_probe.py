#!/usr/bin/env python3
"""Compare how gcc/clang and slate-parser treat compiler flags.

usage: flag_probe.py FLAG [FLAG ...] [--flavor gcc|clang ...] [--no-variants] [-- EXTRA ARGS]

For each flag and flavor it reports whether the oracle and slate-parser
accept it, and whether the flag changes the same predefined macros on both
sides. A FLAG with a space is one flag with a separate value ("-mllvm -foo").
Unless --no-variants, `-fX`/`-mX` also probe their `-fno-X`/`-mno-X` opposite
and `-fX=V` also probes a bogus value, to show whether values are validated.
EXTRA ARGS go to every command (`-m32`, `-std=c11`); `-std=gnu17` is added
when absent. Exits 1 if any line is DIFF. Needs target/test-cache/release/slate-parser.
"""
import argparse
import concurrent.futures
import re
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SLATE = ROOT.parents[1] / "target/test-cache/release/slate-parser"
DEFAULT_STANDARD = "-std=gnu17"
BOGUS = "slate-probe-bogus"
DEFINE = re.compile(r"#define (\w+)(.*)")


def variants(flag: str) -> list[str]:
    if " " in flag:
        return [flag]
    if "=" in flag:
        return [flag, flag.split("=", 1)[0] + "=" + BOGUS]
    for prefix in ("-f", "-m"):
        if flag.startswith(prefix + "no-"):
            return [flag, prefix + flag[len(prefix) + 3 :]]
        if flag.startswith(prefix) and len(flag) > 2:
            return [flag, prefix + "no-" + flag[len(prefix) :]]
    return [flag]


def first_line(text: str, needle: str) -> str:
    line = next((line for line in text.splitlines() if needle in line), text.strip().splitlines()[0] if text.strip() else "?")
    line = re.sub(r"\s+", " ", line.replace("×", "")).strip()
    return line.removeprefix("Error: ")[:110]


def oracle_status(flavor: str, args: list[str], empty: Path) -> tuple[bool, str]:
    run = subprocess.run([flavor, "-fsyntax-only", "-x", "c", *args, str(empty)], capture_output=True, text=True)
    if run.returncode:
        return False, "rejected: " + first_line(run.stderr, "error")
    if "warning:" in run.stderr:
        return True, "accepted, warns: " + first_line(run.stderr, "warning:").split("warning:", 1)[-1].strip()
    return True, "accepted"


def slate_status(flavor: str, args: list[str], empty: Path) -> tuple[bool, str]:
    run = subprocess.run([str(SLATE), "ir", str(empty), f"--flavor={flavor}", *args], capture_output=True, text=True)
    if run.returncode:
        return False, "rejected: " + first_line(run.stderr, "×")
    return True, "accepted"


def oracle_macros(flavor: str, args: list[str]) -> dict[str, str]:
    run = subprocess.run([flavor, "-dM", "-E", "-x", "c", *args, "/dev/null"], capture_output=True, text=True)
    return {m.group(1): m.group(2).strip() for line in run.stdout.splitlines() if (m := DEFINE.match(line))}


def probe_source(names: list[str]) -> str:
    return "".join(
        f'#ifdef {name}\nslate_probe "{name}" = {name} ;\n#else\nslate_probe "{name}" undefined ;\n#endif\n'
        for name in names
    )


def expanded(command: list[str]) -> dict[str, str] | None:
    run = subprocess.run(command, capture_output=True, text=True)
    if run.returncode:
        return None
    values = {}
    for line in run.stdout.splitlines():
        match = re.match(r'\s*slate_probe "(\w+)" (.*?)\s*;\s*$', line)
        if match:
            values[match.group(1)] = re.sub(r"\s+", " ", match.group(2))
    return values


def delta(before: dict[str, str], after: dict[str, str]) -> dict[str, str]:
    return {name: after[name] for name in after if before.get(name) != after[name]}


def describe(changes: dict[str, str]) -> str:
    if not changes:
        return "none"
    return " ".join(f"-{name}" if value == "undefined" else f"{name}{value[1:]}" for name, value in sorted(changes.items()))


def probe(flavor: str, flag: str, extra: list[str], scratch: Path) -> list[tuple[str, bool]]:
    flag_args = shlex.split(flag)
    args = [*extra, *flag_args]
    empty = scratch / "empty.c"
    oracle_ok, oracle = oracle_status(flavor, args, empty)
    slate_ok, slate = slate_status(flavor, args, empty)
    lines = [(f"{flavor:5} {flag:42} oracle {oracle:44} slate {slate}", oracle_ok == slate_ok)]
    if not oracle_ok:
        return lines
    names = sorted(set(oracle_macros(flavor, extra)) | set(oracle_macros(flavor, args)))
    source = scratch / f"probe-{abs(hash((flavor, flag)))}.c"
    source.write_text(probe_source(names))
    oracle_before = expanded([flavor, "-E", "-P", *extra, str(source)])
    oracle_after = expanded([flavor, "-E", "-P", *args, str(source)])
    oracle_delta = delta(oracle_before or {}, oracle_after or {})
    if not slate_ok:
        if oracle_delta:
            lines.append((f"{'':5} {'':42} macros oracle {describe(oracle_delta)}", False))
        return lines
    slate_before = expanded([str(SLATE), "pp", str(source), f"--flavor={flavor}", *extra])
    slate_after = expanded([str(SLATE), "pp", str(source), f"--flavor={flavor}", *args])
    if slate_before is None or slate_after is None:
        lines.append((f"{'':5} {'':42} macros slate pp failed", False))
        return lines
    slate_delta = delta(slate_before, slate_after)
    if oracle_delta or slate_delta:
        same = oracle_delta == slate_delta
        detail = f"oracle {describe(oracle_delta)}" if same else f"oracle {describe(oracle_delta)} | slate {describe(slate_delta)}"
        lines.append((f"{'':5} {'':42} macros {detail}", same))
    return lines


def main() -> int:
    argv = sys.argv[1:]
    extra = argv[argv.index("--") + 1 :] if "--" in argv else []
    argv = argv[: argv.index("--")] if "--" in argv else argv
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("flags", nargs="+")
    parser.add_argument("--flavor", action="append", choices=("gcc", "clang"))
    parser.add_argument("--no-variants", action="store_true")
    parser.add_argument("--jobs", type=int, default=16)
    args, unknown = parser.parse_known_args(argv)
    flags = [*args.flags, *unknown]
    if not any(arg.startswith("-std=") for arg in extra):
        extra = [*extra, DEFAULT_STANDARD]
    flavors = args.flavor or ["clang", "gcc"]
    work = [
        (flavor, variant)
        for flag in flags
        for variant in ([flag] if args.no_variants else variants(flag))
        for flavor in flavors
    ]
    with tempfile.TemporaryDirectory() as directory:
        scratch = Path(directory)
        (scratch / "empty.c").write_text("int slate_probe_unit;\n")
        with concurrent.futures.ThreadPoolExecutor(args.jobs) as pool:
            results = list(pool.map(lambda job: probe(*job, extra, scratch), work))
    diff = False
    for lines in results:
        for text, same in lines:
            print(f"{'ok  ' if same else 'DIFF'} {text}")
            diff |= not same
    return 1 if diff else 0


if __name__ == "__main__":
    sys.exit(main())
