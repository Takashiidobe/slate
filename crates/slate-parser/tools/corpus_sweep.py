#!/usr/bin/env python3
import argparse
import concurrent.futures
import json
import os
import re
import subprocess
from collections import defaultdict
from dataclasses import dataclass, asdict
from pathlib import Path


DEFINE_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-DEFINES\s+(\S+)(?:\s+(.*))?$")
ERROR_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-ERROR\s+(\S+)\s*$")
STD_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-STD\s+(\S+)\s+(\S+)\s*$")
ISYSTEM_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-ISYSTEM\s+(.*)$")
FLAVOR_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-FLAVOR\s+(\S+)\s*$")
ARGS_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-ARGS\s+(.*)$")
BEGIN_RE = re.compile(r"^// SLATE-FILECHECK-BEGIN ")
END_RE = re.compile(r"^// SLATE-FILECHECK-END ")
DIAGNOSTIC_RE = re.compile(r"^\s*[×⚠]\s+(.*)$")
LOCATION_RE = re.compile(r"\b[^\s:]+\.c:\d+(?::\d+)?\b")
NUMBER_RE = re.compile(r"\b\d+\b")


@dataclass(frozen=True)
class Job:
    fixture: str
    prefix: str
    defines: tuple[str, ...]
    standard: str | None
    flavor: str | None
    target: str | None
    isystem: tuple[str, ...]
    extra_args: tuple[str, ...]


@dataclass
class Result:
    fixture: str
    prefix: str
    tool: str
    accepted: bool
    root: str
    stderr: str
    warnings: list[str]


def fixture_source(source: str) -> str:
    lines = []
    in_checks = False
    for line in source.splitlines():
        if BEGIN_RE.match(line):
            in_checks = True
        if not in_checks and not line.startswith("// SLATE-FILECHECK-"):
            lines.append(line)
        if END_RE.match(line):
            in_checks = False
    return "\n".join(lines) + "\n"


def target_for(fixture: Path) -> str | None:
    if fixture.parent.parent.name == "sema":
        return fixture.parent.name
    return None


def jobs(fixtures: Path) -> tuple[list[Job], int]:
    result = []
    expected_errors = 0
    for fixture in sorted(fixtures.rglob("*.c")):
        source = fixture.read_text(errors="surrogateescape")
        errors = {match.group(1) for line in source.splitlines() if (match := ERROR_RE.match(line))}
        standards = {
            match.group(1): match.group(2)
            for line in source.splitlines()
            if (match := STD_RE.match(line))
        }
        isystem = tuple(
            os.path.expanduser(path)
            for line in source.splitlines()
            if (match := ISYSTEM_RE.match(line))
            for path in match.group(1).split()
        )
        flavor = next(
            (match.group(1) for line in source.splitlines() if (match := FLAVOR_RE.match(line))),
            None,
        )
        extra_args = tuple(
            arg
            for line in source.splitlines()
            if (match := ARGS_RE.match(line))
            for arg in match.group(1).split()
            if not arg.startswith("--dump-ir")
            and arg not in {"--show-ids", "--show-spans", "--show-metadata", "--compact-ir"}
        )
        for line in source.splitlines():
            match = DEFINE_RE.match(line)
            if not match:
                continue
            prefix = match.group(1)
            if prefix in errors:
                expected_errors += 1
                continue
            result.append(
                Job(
                    fixture=str(fixture),
                    prefix=prefix,
                    defines=tuple((match.group(2) or "").split()),
                    standard=standards.get(prefix),
                    flavor=flavor,
                    target=target_for(fixture),
                    isystem=isystem,
                    extra_args=extra_args,
                )
            )
    return result, expected_errors


def root_diagnostic(stderr: str) -> str:
    lines = stderr.splitlines()
    diagnostic = next(
        (match.group(1) for line in lines if (match := DIAGNOSTIC_RE.match(line))),
        "",
    )
    if not diagnostic:
        diagnostic = next(
            (line.split(" error: ", 1)[1] for line in lines if " error: " in line),
            "",
        )
    if not diagnostic:
        diagnostic = next(
            (line.removeprefix("Error: ") for line in lines if line.startswith("Error: ")),
            lines[-1] if lines else "no diagnostic",
        )
    diagnostic = LOCATION_RE.sub("<file>", diagnostic)
    diagnostic = NUMBER_RE.sub("<n>", diagnostic)
    return diagnostic.strip()


def warnings(stderr: str, tool: str) -> list[str]:
    if tool == "slate-ir":
        return sorted({line.strip()[2:] for line in stderr.splitlines() if line.strip().startswith("-W")})
    result = set()
    for line in stderr.splitlines():
        if " warning:" not in line:
            continue
        option = re.search(r"\[-W([^]]+)\]", line)
        result.add(option.group(1) if option else re.sub(r"^.* warning: ", "", line).strip())
    return sorted(result)


def common_args(job: Job) -> list[str]:
    args = [f"-D{define.removeprefix('-D')}" for define in job.defines]
    args.extend(f"-isystem{path}" for path in job.isystem)
    if job.standard:
        args.append(f"-std={job.standard}")
    if job.target:
        args.append(f"--target={job.target}")
    return args


def slate_libc_defines(job: Job) -> list[str]:
    if not any("slate/libc-shim" in path for path in job.isystem):
        return []
    target = job.target or "x86_64-unknown-linux-gnu"
    fields = target.split("-")
    arch = fields[0]
    vendor = fields[1] if len(fields) > 1 else "unknown"
    kernel = fields[2] if len(fields) > 2 else "linux"
    environment = fields[3] if len(fields) > 3 else "gnu"
    arch_names = {
        "x86_64": "X86_64",
        "i686": "X86",
        "aarch64": "AARCH64",
        "arm": "ARM",
        "riscv64": "RISCV64",
        "riscv32": "RISCV32",
    }
    result = [
        f"-D__SLATE_ARCH_{arch_names.get(arch, arch.upper())}=1",
        f"-D__SLATE_VENDOR_{vendor.upper()}=1",
        f"-D__SLATE_KERNEL_{kernel.upper()}=1",
        f"-D__SLATE_WORDSIZE_{'64' if arch in {'x86_64', 'aarch64', 'riscv64'} else '32'}=1",
        "-D__SLATE_ENDIAN_LITTLE=1",
    ]
    if kernel in {"linux", "freebsd"}:
        result.append("-D__SLATE_OBJ_ELF=1")
    elif kernel == "windows":
        result.append("-D__SLATE_OBJ_COFF=1")
    elif kernel == "darwin":
        result.append("-D__SLATE_OBJ_MACHO=1")
    libc = "GLIBC" if environment == "gnu" else environment.upper()
    result.append(f"-D__SLATE_LIBC_{libc}=1")
    if libc == "GLIBC":
        result.append("-D__SLATE_GLIBC_MINOR__=0")
    return result


def run_ir(job: Job, parser: str) -> Result:
    args = [parser, "ir", job.fixture, *common_args(job)]
    if job.flavor:
        args.append(f"--flavor={job.flavor}")
    args.extend(job.extra_args)
    completed = subprocess.run(args, text=True, capture_output=True)
    return Result(
        fixture=job.fixture,
        prefix=job.prefix,
        tool="slate-ir",
        accepted=completed.returncode == 0,
        root="" if completed.returncode == 0 else root_diagnostic(completed.stderr),
        stderr=completed.stderr[-4000:],
        warnings=warnings(completed.stderr, "slate-ir"),
    )


def run_clang(job: Job, clang: str) -> Result:
    args = [clang, "-fsyntax-only", job.fixture, *common_args(job), *slate_libc_defines(job)]
    if job.flavor == "msvc":
        args.extend(["-fms-extensions", "-fms-compatibility"])
    args.extend(arg for arg in job.extra_args if arg.startswith("-W"))
    completed = subprocess.run(args, text=True, capture_output=True)
    return Result(
        fixture=job.fixture,
        prefix=job.prefix,
        tool="clang",
        accepted=completed.returncode == 0,
        root="" if completed.returncode == 0 else root_diagnostic(completed.stderr),
        stderr=completed.stderr[-4000:],
        warnings=warnings(completed.stderr, "clang"),
    )


def write_report(path: Path, results: list[Result], expected_errors: int, versions: dict[str, str]) -> None:
    groups: dict[tuple[str, str], list[Result]] = defaultdict(list)
    for result in results:
        if not result.accepted:
            groups[(result.tool, result.root)].append(result)
    accepted = defaultdict(int)
    total = defaultdict(int)
    for result in results:
        total[result.tool] += 1
        accepted[result.tool] += int(result.accepted)
    lines = ["# Corpus sweep", ""]
    for tool in sorted(total):
        lines.append(f"- {tool}: {accepted[tool]}/{total[tool]} accepted")
    indexed = {(result.fixture, result.prefix, result.tool): result for result in results}
    pairs = {(result.fixture, result.prefix) for result in results}
    if all((fixture, prefix, tool) in indexed for fixture, prefix in pairs for tool in ("slate-ir", "clang")):
        lines.append(
            "- Slate IR rejects / Clang accepts: "
            + str(sum(not indexed[(fixture, prefix, "slate-ir")].accepted and indexed[(fixture, prefix, "clang")].accepted for fixture, prefix in pairs))
        )
        lines.append(
            "- Slate IR accepts / Clang rejects: "
            + str(sum(indexed[(fixture, prefix, "slate-ir")].accepted and not indexed[(fixture, prefix, "clang")].accepted for fixture, prefix in pairs))
        )
    lines.append(f"- expected-error configurations skipped: {expected_errors}")
    for tool, version in sorted(versions.items()):
        lines.append(f"- {tool} version: `{version}`")
    warning_diffs = []
    for fixture, prefix in sorted({(result.fixture, result.prefix) for result in results}):
        slate = indexed.get((fixture, prefix, "slate-ir"))
        clang = indexed.get((fixture, prefix, "clang"))
        if slate and clang and slate.accepted and clang.accepted and slate.warnings != clang.warnings:
            warning_diffs.append((fixture, prefix, slate.warnings, clang.warnings))
    if warning_diffs:
        lines.extend(["", f"## Warning differences ({len(warning_diffs)})", ""])
        for fixture, prefix, slate, clang in warning_diffs:
            lines.append(
                f"- `{fixture}` ({prefix}): slate={','.join(slate) or '-'}; clang={','.join(clang) or '-'}"
            )
    for (tool, root), members in sorted(groups.items(), key=lambda item: (-len(item[1]), item[0])):
        lines.extend(["", f"## {tool}: {root} ({len(members)})", ""])
        lines.extend(f"- `{member.fixture}` ({member.prefix})" for member in members)
    path.write_text("\n".join(lines) + "\n")


def version(command: str) -> str:
    completed = subprocess.run([command, "--version"], text=True, capture_output=True)
    return completed.stdout.splitlines()[0] if completed.stdout else "unknown"


def git_revision() -> str:
    completed = subprocess.run(["git", "rev-parse", "--short", "HEAD"], text=True, capture_output=True)
    return completed.stdout.strip() or "unknown"


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--fixtures", type=Path, default=Path("tests/fixtures"))
    parser.add_argument("--slate-parser", default="target/release/slate-parser")
    parser.add_argument("--clang", default="clang")
    parser.add_argument("--tool", choices=("ir", "clang", "both"), default="both")
    parser.add_argument("--jobs", type=int, default=min(12, os.cpu_count() or 1))
    parser.add_argument("--report", type=Path, default=Path("target/corpus-sweep.md"))
    parser.add_argument("--json", type=Path, default=Path("target/corpus-sweep.json"))
    args = parser.parse_args()
    corpus, expected_errors = jobs(args.fixtures)
    runners = []
    if args.tool in {"ir", "both"}:
        runners.append((run_ir, args.slate_parser))
    if args.tool in {"clang", "both"}:
        runners.append((run_clang, args.clang))
    work = [(runner, command, job) for runner, command in runners for job in corpus]
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        results = list(pool.map(lambda item: item[0](item[2], item[1]), work))
    versions = {"slate-parser": f"git {git_revision()} ({args.slate_parser})"}
    if args.tool in {"clang", "both"}:
        versions["clang"] = version(args.clang)
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.json.parent.mkdir(parents=True, exist_ok=True)
    write_report(args.report, results, expected_errors, versions)
    args.json.write_text(json.dumps([asdict(result) for result in results], indent=2) + "\n")


if __name__ == "__main__":
    main()
