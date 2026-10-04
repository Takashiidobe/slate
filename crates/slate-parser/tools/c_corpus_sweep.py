#!/usr/bin/env python3
"""Sweep ~/c-corpus through `slate-parser ir` with the selected compiler as oracle.

usage: c_corpus_sweep.py [PROJECT ...] [--flavor clang|gcc|msvc] [--jobs N] [--trophies]

Reads <corpus>/<project>/build-<flavor>/compile_commands.json, written by
tools/c_corpus_setup.py. Needs target/test-cache/release/slate-parser.
"""
import argparse
import collections
import concurrent.futures
import json
import os
import re
import shlex
import subprocess
import sys
import time
from dataclasses import asdict, dataclass
from pathlib import Path

import pp_diff

ROOT = Path(__file__).resolve().parents[1]
SLATE = ROOT.parents[1] / "target/test-cache/release/slate-parser"
PROVIDERS = {"zlib": ["", "build-{flavor}"]}
HOST_INCLUDE_DIRS = {"cpython": ["/usr/include"], "linux": ["/usr/include"],"nginx": ["/usr/include"], "pcre2": ["/usr/include"], "postgres": ["/usr/include"], "redis": ["/usr/include"]}
PROJECTS = {
    "cJSON": "https://github.com/DaveGamble/cJSON",
    "c-ares": "https://c-ares.org",
    "cglm": "https://github.com/recp/cglm",
    "cpython": "https://www.python.org",
    "chibicc": "https://github.com/rui314/chibicc",
    "curl": "https://curl.se",
    "giflib": "https://giflib.sourceforge.net",
    "jq": "https://jqlang.org",
    "libdeflate": "https://github.com/ebiggers/libdeflate",
    "libevent": "https://libevent.org",
    "libexpat": "https://libexpat.github.io",
    "libpng": "https://www.libpng.org/pub/png/libpng.html",
    "libsodium": "https://libsodium.org",
    "libuv": "https://libuv.org",
    "libyaml": "https://github.com/yaml/libyaml",
    "lmdb": "https://www.symas.com/lmdb",
    "lua": "https://www.lua.org",
    "lz4": "https://github.com/lz4/lz4",
    "mbedtls": "https://github.com/Mbed-TLS/mbedtls",
    "mimalloc": "https://github.com/microsoft/mimalloc",
    "musl": "https://musl.libc.org",
    "nginx": "https://nginx.org",
    "oniguruma": "https://github.com/kkos/oniguruma",
    "pcre2": "https://github.com/PCRE2Project/pcre2",
    "postgres": "https://www.postgresql.org",
    "linux": "https://kernel.org",
    "quickjs": "https://bellard.org/quickjs/",
    "redis": "https://redis.io",
    "sqlite": "https://sqlite.org",
    "stb": "https://github.com/nothings/stb",
    "tinycc": "https://bellard.org/tcc/",
    "utf8proc": "https://github.com/JuliaStrings/utf8proc",
    "xxHash": "https://github.com/Cyan4973/xxHash",
    "yyjson": "https://github.com/ibireme/yyjson",
    "zlib": "https://zlib.net",
    "zstd": "https://github.com/facebook/zstd",
}
DISPLAY = {"cpython": "CPython", "sqlite": "SQLite", "lua": "Lua", "pcre2": "PCRE2", "quickjs": "QuickJS", "lz4": "LZ4", "tinycc": "TinyCC", "mbedtls": "Mbed TLS", "redis": "Redis", "postgres": "PostgreSQL", "linux": "Linux"}
DETAIL = re.compile(r"^\s*(?:Error:\s*)?×\s+(.*)$")
DEPENDENCY_OUTPUT = {"-M", "-MM", "-MD", "-MMD", "-MP", "-MG"}
DEPENDENCY_OUTPUT_VALUES = ("-MF", "-MT", "-MQ", "-MJ")
STATUSES = ("ok", "internal", "unimplemented", "rejected", "timeout", "missing-dependency", "oracle-rejects")


@dataclass
class Job:
    project: str
    source: str
    directory: str
    args: list[str]


@dataclass
class Result:
    project: str
    source: str
    status: str
    detail: str
    seconds: float


def jobs(corpus: Path, projects: list[str], flavor: str) -> list[Job]:
    found = []
    for project in projects:
        # clang demotes an -I directory that is also -idirafter to a system directory
        extra = [
            f"-idirafter{corpus / provider / sub.format(flavor=flavor)}"
            for provider, subs in PROVIDERS.items()
            if provider != project
            for sub in subs
        ]
        extra += [f"-idirafter{directory}" for directory in HOST_INCLUDE_DIRS.get(project, [])]
        database = corpus / project / f"build-{flavor}" / "compile_commands.json"
        if not database.exists():
            print(f"{project}: no {database.relative_to(corpus)}; run tools/c_corpus_setup.py", file=sys.stderr)
            continue
        seen = set()
        for entry in json.loads(database.read_text()):
            source = pp_diff.absolute(entry["directory"], entry["file"])
            if not source.endswith(".c") or source in seen or not os.path.exists(source):
                continue
            seen.add(source)
            argv = entry.get("arguments") or shlex.split(entry["command"])
            if flavor != "msvc":
                args = [arg for arg in argv[1:] if arg.startswith("-") or pp_diff.absolute(entry["directory"], arg) != source]
            else:
                args = pp_diff.kept_args(argv, entry["directory"])
                path_flags = ("-isystem", "-iquote", "-idirafter", "-I", "-include", "-imacros")
                normalized = []
                for arg in args:
                    flag = next((flag for flag in path_flags if arg.startswith(flag)), None)
                    normalized.append(flag + arg[len(flag):].replace("\\", "/") if flag else arg)
                args = normalized
            if flavor != "msvc" and not any(arg.startswith("-std=") for arg in args):
                args.append(pp_diff.CLANG_DEFAULT_STANDARD)
            found.append(Job(project, source, entry["directory"], [*args, *extra]))
    return found


def classify(stderr: str) -> tuple[str, str]:
    details = [m.group(1).strip() for line in stderr.splitlines() if (m := DETAIL.match(line))]
    detail = next((d for d in details if d != "semantic analysis failed"), details[0] if details else stderr.strip()[:200])
    if detail.startswith("header not found"):
        return "missing-dependency", detail
    if detail.startswith("internal error"):
        return "internal", detail
    if detail.startswith("not implemented"):
        return "unimplemented", detail
    return "rejected", detail


def without_dependency_output(args: list[str]) -> list[str]:
    kept, index = [], 0
    while index < len(args):
        arg = args[index]
        if arg in DEPENDENCY_OUTPUT_VALUES:
            index += 2
            continue
        if arg not in DEPENDENCY_OUTPUT and not arg.startswith(("-Wp,-M", *DEPENDENCY_OUTPUT_VALUES)):
            kept.append(arg)
        index += 1
    return kept


def oracle_args(job: Job, flavor: str) -> list[str]:
    if flavor != "msvc":
        return [flavor, *without_dependency_output(job.args), "-fsyntax-only", "-w", job.source]
    args = []
    for arg in job.args:
        if arg.startswith(("-D", "-U")):
            args.append("/" + arg[1:])
        elif (flag := next((f for f in ("-isystem", "-iquote", "-idirafter", "-I") if arg.startswith(f)), None)):
            path = arg[len(flag):]
            args.append("/I" + path)
    return [str(ROOT / "tools/cl.exe"), "/nologo", "/Zs", "/std:c17", *args, job.source]


def run(job: Job, flavor: str, timeout: int) -> Result:
    start = time.monotonic()

    def result(status: str, detail: str = "") -> Result:
        return Result(job.project, job.source, status, detail[:200], round(time.monotonic() - start, 3))

    oracle = subprocess.run(
        oracle_args(job, flavor),
        cwd=job.directory, capture_output=True, text=True, errors="replace",
    )
    if oracle.returncode:
        output = oracle.stdout + oracle.stderr
        first = next((line for line in output.splitlines() if "error:" in line or " error " in line or "fatal error C" in line), output[:200])
        return result("oracle-rejects", first.split("error:", 1)[-1].strip())
    try:
        slate_args = [str(SLATE), "ir", job.source, f"--flavor={flavor}", *job.args]
        if flavor == "msvc":
            slate_args.extend(("--target=x86_64-pc-windows-msvc", "-std=c17"))
        elif not any(arg.startswith("-std=") for arg in job.args):
            slate_args.append(pp_diff.CLANG_DEFAULT_STANDARD)
        slate = subprocess.run(
            slate_args,
            cwd=job.directory, capture_output=True, text=True, errors="replace", timeout=timeout,
        )
    except subprocess.TimeoutExpired:
        return result("timeout", f"over {timeout}s")
    if slate.returncode:
        return result(*classify(slate.stderr))
    return result("ok")


def revision(corpus: Path, project: str) -> str:
    version_tags = [f"--match={pattern}" for pattern in ("v[0-9]*", "[0-9]*", "R_[0-9]*", "version-[0-9]*")]
    described = subprocess.run(
        ["git", "-C", str(corpus / project), "describe", "--tags", "--always", *version_tags],
        capture_output=True,
        text=True,
    )
    return described.stdout.strip() or "?"


def passing(counts: collections.Counter) -> bool:
    return counts["ok"] > 0 and counts["ok"] == sum(counts.values())


def trophies(corpus: Path, table: dict[str, collections.Counter]) -> str:
    winners = sorted(
        (p for p, counts in table.items() if passing(counts)),
        key=lambda p: (-table[p]["ok"], p.lower()),
    )
    rows = [("Project", "Revision")] + [
        (f"[{DISPLAY.get(p, p)}]({PROJECTS.get(p, '')})", f"`{revision(corpus, p)}`") for p in winners
    ]
    widths = [max(len(row[i]) for row in rows) for i in range(2)]
    lines = ["| " + " | ".join(cell.ljust(w) for cell, w in zip(row, widths)) + " |" for row in rows]
    lines.insert(1, "| " + " | ".join("-" * w for w in widths) + " |")
    return "\n".join(lines)


def report(results: list[Result], table: dict[str, collections.Counter], flavor: str) -> str:
    lines = [f"# c-corpus sweep ({flavor})", "", "| Project | " + " | ".join(STATUSES) + " | Full |", "|" + " --- |" * (len(STATUSES) + 2)]
    for project, counts in sorted(table.items(), key=lambda kv: kv[0].lower()):
        cells = " | ".join(str(counts[s]) for s in STATUSES)
        lines.append(f"| {project} | {cells} | {'yes' if passing(counts) else ''} |")
    clusters = collections.defaultdict(list)
    for r in results:
        if r.status != "ok":
            clusters[(r.status, re.sub(r"\d+", "N", r.detail))].append(r.source)
    lines += ["", "## Failure clusters", ""]
    for (status, detail), sources in sorted(clusters.items(), key=lambda kv: (-len(kv[1]), kv[0])):
        lines.append(f"- {len(sources)} {status}: {detail}\n  - e.g. `{sources[0]}`")
    slow = sorted(results, key=lambda r: -r.seconds)[:5]
    lines += ["", "## Slowest", ""] + [f"- {r.seconds:.1f}s `{r.source}`" for r in slow]
    return "\n".join(lines) + "\n"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("projects", nargs="*")
    parser.add_argument("--corpus", type=Path, default=Path(os.environ.get("SLATE_CORPUS", Path.home() / "c-corpus")))
    parser.add_argument("--flavor", default="clang", choices=("clang", "gcc", "msvc"))
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 8)
    parser.add_argument("--timeout", type=int, default=300)
    parser.add_argument("--report", type=Path, default=ROOT / "target/c-corpus-sweep.md")
    parser.add_argument("--json", type=Path, default=ROOT / "target/c-corpus-sweep.json")
    parser.add_argument("--trophies", action="store_true")
    args = parser.parse_args()
    projects = args.projects or sorted(
        p.name for p in args.corpus.iterdir() if p.is_dir() and not p.name.startswith(".")
    )
    work = jobs(args.corpus, projects, args.flavor)
    with concurrent.futures.ThreadPoolExecutor(args.jobs) as pool:
        results = list(pool.map(lambda job: run(job, args.flavor, args.timeout), work))
    table = collections.defaultdict(collections.Counter)
    for r in results:
        table[r.project][r.status] += 1
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(report(results, table, args.flavor))
    args.json.write_text(json.dumps([asdict(r) for r in results], indent=1))
    for project, counts in sorted(table.items(), key=lambda kv: kv[0].lower()):
        print(f"{project:10} " + " ".join(f"{s}={counts[s]}" for s in STATUSES if counts[s]))
    print(f"report: {args.report}")
    if args.trophies:
        print(trophies(args.corpus, table))
    return 0


if __name__ == "__main__":
    sys.exit(main())
