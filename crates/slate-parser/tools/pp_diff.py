#!/usr/bin/env python3
"""Diff `slate-parser pp` against `clang -E -P`, token by token.

usage: pp_diff.py [--corpus [PROJECT ...]] [--compdb compile_commands.json ...]
                  [--file source.c [-- ARGS ...]] [--report target/pp-diff.md] [--jobs N]

Both outputs are re-tokenized and compared ignoring whitespace and line breaks.
Clang runs with -nostdinc over slate's own compiler headers and sysroot, so both
sides read the same headers. Compile commands keep -D/-U/-I/-isystem/-iquote/
-include/-std. Needs target/test-cache/release/slate-parser.
"""
import argparse
import concurrent.futures
import json
import os
import re
import shlex
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SLATE = ROOT.parents[1] / "target/test-cache/release/slate-parser"
TARGET = "x86_64-unknown-linux-gnu"
# slate defaults every flavor to gnu23 (slate-parser-6x05.6); pin clang's own default instead
CLANG_DEFAULT_STANDARD = "-std=gnu17"
CONTEXT = 3

TOKEN = re.compile(
    r"""
    //.*|/\*.*?\*/
  | (?:u8|[uUL])?"(?:\\.|[^"\\\n])*"
  | (?:u8|[uUL])?'(?:\\.|[^'\\\n])*'
  | \.?[0-9](?:[eEpP][+-]|[0-9A-Za-z_.']|\\.)*
  | [A-Za-z_$][A-Za-z_$0-9]*
  | \.\.\.|<<=|>>=|->|\+\+|--|<<|>>|<=|>=|==|!=|&&|\|\||[-+*/%&|^!=<>]=|\#\#
  | \S
    """,
    re.X,
)

# clang -E consumes these pragmas instead of printing them
CONSUMED_PRAGMA = re.compile(
    r"\s*#\s*pragma\s+(?:once|push_macro|pop_macro|region|endregion"
    r"|GCC\s+(?:system_header|poison)|clang\s+(?:deprecated|final|diagnostic))\b"
)

KEEP_JOINED = ("-D", "-U", "-I", "-std=", "-isystem", "-iquote", "-idirafter", "-include", "-imacros")
KEEP_SEPARATE = {"-D", "-U", "-I", "-isystem", "-iquote", "-idirafter", "-include", "-imacros"}
PATH_FLAGS = {"-I", "-isystem", "-iquote", "-idirafter", "-include", "-imacros"}
DROP_SEPARATE = {"-mllvm", "-Xclang"}
SEMANTIC_FLAGS = (
    "-funsigned-char", "-fsigned-char", "-fshort-wchar", "-fms-extensions", "-fms-anonymous-structs",
    "-fexperimental-late-parse-attributes", "-fstrict-flex-arrays=", "-ffreestanding", "-nostdinc", "-fno-builtin",
    "-fpic", "-fPIC", "-fpie", "-fPIE", "-fno-pic", "-fno-PIC", "-fno-pie", "-fno-PIE",
    "-fstack-protector", "-fno-stack-protector", "-fcf-protection", "-mcmodel=",
)
TARGET_FEATURE = re.compile(r"-march=.+|-m(?!llvm$)[a-z0-9][a-z0-9.-]*")


@dataclass
class Job:
    project: str
    source: str
    directory: str
    args: list[str]


@dataclass
class Outcome:
    job: Job
    status: str
    detail: str = ""


def data_dir() -> Path:
    base = os.environ.get("XDG_DATA_HOME") or str(Path.home() / ".local/share")
    return Path(base) / "slate"


def system_includes() -> list[str]:
    sysroots = Path(os.environ.get("SLATE_SYSROOTS", data_dir() / "sysroots"))
    headers = Path(os.environ.get("SLATE_COMPILER_HEADERS", data_dir() / "compiler-headers"))
    clang = sorted(
        headers.glob("clang-*/include"),
        key=lambda path: [int(part) for part in re.findall(r"\d+", path.parent.name)],
    )
    root = sysroots / TARGET
    candidates = [*clang[-1:], root / "SDK/usr/include", root / "usr/include", root / "include"]
    candidates.append(root / "usr/x86_64-linux-gnu/include")
    return [f"-isystem{path}" for path in candidates if path.is_dir()]


def absolute(directory: str, path: str) -> str:
    return path if os.path.isabs(path) else str(Path(directory) / path)


def kept_args(argv: list[str], directory: str) -> list[str]:
    kept, index = [], 1
    while index < len(argv):
        arg = argv[index]
        if arg in KEEP_SEPARATE and index + 1 < len(argv):
            value = argv[index + 1]
            if arg in PATH_FLAGS:
                value = absolute(directory, value)
            kept += [arg, value]
            index += 2
            continue
        if arg in DROP_SEPARATE:
            index += 2
            continue
        flag = next((flag for flag in ("-isystem", "-iquote", "-idirafter", "-I") if arg.startswith(flag)), None)
        if flag and len(arg) > len(flag):
            arg = flag + absolute(directory, arg[len(flag):])
        if arg.startswith(KEEP_JOINED) or arg.startswith(SEMANTIC_FLAGS) or TARGET_FEATURE.fullmatch(arg):
            kept.append(arg)
        index += 1
    return kept


def compdb_jobs(path: Path, project: str) -> list[Job]:
    jobs, seen = [], set()
    for entry in json.loads(path.read_text()):
        source = absolute(entry["directory"], entry["file"])
        if not source.endswith(".c") or source in seen:
            continue
        seen.add(source)
        argv = entry.get("arguments") or shlex.split(entry["command"])
        jobs.append(Job(project, source, entry["directory"], kept_args(argv, entry["directory"])))
    return jobs


def corpus_jobs(corpus: Path, projects: list[str]) -> list[Job]:
    jobs, seen = [], set()
    for compdb in sorted(corpus.glob("**/compile_commands.json")):
        project = compdb.relative_to(corpus).parts[0]
        if projects and project not in projects:
            continue
        for job in compdb_jobs(compdb, project):
            if job.source not in seen:
                seen.add(job.source)
                jobs.append(job)
    return jobs


def tokens(text: str) -> list[tuple[str, int]]:
    result = []
    for number, line in enumerate(text.splitlines(), 1):
        if CONSUMED_PRAGMA.match(line):
            continue
        # clang -E -P keeps `//` comments from system headers when the standard lacks them
        result.extend(
            (match.group(), number) for match in TOKEN.finditer(line) if not match.group().startswith(("//", "/*"))
        )
    return result


def first_line(stderr: str) -> str:
    for line in stderr.splitlines():
        line = re.sub(r"\s+", " ", line.replace("×", "")).strip()
        if line and "preprocessing failed" not in line:
            return line[:160]
    return "?"


def run(job: Job, includes: list[str]) -> Outcome:
    if not any(arg.startswith("-std=") for arg in job.args):
        job.args = [*job.args, CLANG_DEFAULT_STANDARD]
    clang = subprocess.run(
        ["clang", f"--target={TARGET}", "-nostdinc", *job.args, *includes, "-E", "-P", "-w", job.source],
        cwd=job.directory, capture_output=True, text=True, errors="replace",
    )
    if clang.returncode:
        return Outcome(job, "clang-rejects", first_line(clang.stderr))
    try:
        slate = subprocess.run(
            [str(SLATE), "pp", job.source, "--flavor=clang", *job.args],
            cwd=job.directory, capture_output=True, text=True, errors="replace", timeout=120,
        )
    except subprocess.TimeoutExpired:
        return Outcome(job, "slate-error", "timeout")
    if slate.returncode:
        return Outcome(job, "slate-error", first_line(slate.stderr))
    theirs, ours = tokens(clang.stdout), tokens(slate.stdout)
    for index, (their, our) in enumerate(zip(theirs, ours)):
        if their[0] != our[0]:
            return Outcome(job, "diff", describe(theirs, ours, index))
    if len(theirs) != len(ours):
        return Outcome(job, "diff", describe(theirs, ours, min(len(theirs), len(ours))))
    return Outcome(job, "same")


def describe(theirs: list[tuple[str, int]], ours: list[tuple[str, int]], index: int) -> str:
    def window(stream: list[tuple[str, int]]) -> str:
        before = " ".join(token for token, _ in stream[max(0, index - CONTEXT):index])
        after = " ".join(token for token, _ in stream[index:index + CONTEXT + 1]) or "<eof>"
        line = stream[index][1] if index < len(stream) else "eof"
        return f"`{before} ⟦{after}⟧` (line {line})"

    return f"clang {window(theirs)} vs slate {window(ours)}"


def report(outcomes: list[Outcome], path: Path) -> str:
    projects: dict[str, list[Outcome]] = {}
    for outcome in outcomes:
        projects.setdefault(outcome.job.project, []).append(outcome)
    statuses = ["same", "diff", "slate-error", "clang-rejects"]
    lines = ["# pp diff", "", "| Project | " + " | ".join(statuses) + " |", "| --- |" + " --- |" * len(statuses)]
    for project, results in sorted(projects.items()):
        counts = [sum(result.status == status for result in results) for status in statuses]
        lines.append(f"| {project} | " + " | ".join(map(str, counts)) + " |")
    summary = "\n".join(lines)
    for project, results in sorted(projects.items()):
        failures = [result for result in results if result.status in ("diff", "slate-error")]
        if not failures:
            continue
        lines += ["", f"## {project}", ""]
        for result in sorted(failures, key=lambda result: result.job.source):
            lines.append(f"- {result.status} `{result.job.source}`: {result.detail}")
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines) + "\n")
    return summary


def main() -> None:
    argv = sys.argv[1:]
    extra = []
    if "--" in argv:
        split = argv.index("--")
        argv, extra = argv[:split], argv[split + 1:]
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--corpus", nargs="*", metavar="PROJECT")
    parser.add_argument("--corpus-root", type=Path, default=Path.home() / "c-corpus")
    parser.add_argument("--compdb", type=Path, action="append", default=[])
    parser.add_argument("--file", action="append", default=[])
    parser.add_argument("--report", type=Path, default=ROOT / "target/pp-diff.md")
    parser.add_argument("--jobs", type=int, default=os.cpu_count())
    options = parser.parse_args(argv)

    jobs = []
    if options.corpus is not None:
        jobs += corpus_jobs(options.corpus_root, options.corpus)
    for compdb in options.compdb:
        jobs += compdb_jobs(compdb, compdb.resolve().parent.name)
    for source in options.file:
        jobs.append(Job("files", str(Path(source).resolve()), os.getcwd(), extra))
    if not jobs:
        parser.error("nothing to diff: pass --corpus, --compdb or --file")

    includes = system_includes()
    with concurrent.futures.ThreadPoolExecutor(options.jobs) as pool:
        outcomes = list(pool.map(lambda job: run(job, includes), jobs))
    print(report(outcomes, options.report))
    if len(outcomes) == 1 and outcomes[0].detail:
        print(outcomes[0].detail)
    print(f"\nwrote {options.report}")


if __name__ == "__main__":
    main()
