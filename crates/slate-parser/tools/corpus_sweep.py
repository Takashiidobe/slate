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

from update_filecheck import placement


DEFINE_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-DEFINES\s+(\S+)(?:\s+(.*))?$")
ERROR_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-ERROR\s+(\S+)\s*$")
STD_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-STD\s+(\S+)\s+(\S+)\s*$")
ISYSTEM_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-ISYSTEM\s+(.*)$")
ARGS_RE = re.compile(r"^\s*//\s*SLATE-FILECHECK-ARGS\s+(.*)$")
BEGIN_RE = re.compile(r"^// SLATE-FILECHECK-BEGIN ")
END_RE = re.compile(r"^// SLATE-FILECHECK-END ")
# a detailed "  × ..." outranks the "Error:   × semantic analysis failed" wrapper,
# and both outrank a #warning that merely precedes the real failure
DIAGNOSTIC_TIERS = (
    re.compile(r"^\s+×\s+(.*)$"),
    re.compile(r"^Error:\s*×\s+(.*)$"),
    re.compile(r"^\s*⚠\s+(.*)$"),
)
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


def jobs(fixtures: Path) -> tuple[list[Job], int]:
    result = []
    expected_errors = 0
    fixture_paths = [fixtures] if fixtures.is_file() else sorted(fixtures.rglob("*.c"))
    for fixture in fixture_paths:
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
        flavor, target = placement(fixture)
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
                    target=target,
                    isystem=isystem,
                    extra_args=extra_args,
                )
            )
    return result, expected_errors


def root_diagnostic(stderr: str) -> str:
    lines = stderr.splitlines()
    diagnostic = ""
    for pattern in DIAGNOSTIC_TIERS:
        diagnostic = next(
            (match.group(1) for line in lines if (match := pattern.match(line))),
            "",
        )
        if diagnostic:
            break
    if not diagnostic:
        diagnostic = next(
            (line.split(" error: ", 1)[1] for line in lines if " error: " in line),
            "",
        )
    if not diagnostic:
        diagnostic = next(
            (match.group(1) for line in lines if (match := re.search(r"\berror\s+C\d+:\s*(.*)", line, re.IGNORECASE))),
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
        if tool == "msvc":
            match = re.search(r"\bwarning\s+(C\d+):\s*(.*)", line, re.IGNORECASE)
            if match:
                result.add(match.group(1).upper())
        elif " warning:" in line:
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


def sysroot_include_paths(job: Job) -> list[str]:
    # mirrors SysrootLayout in src/target_registry.rs and sysroot::include_paths_at
    target = job.target or "x86_64-unknown-linux-gnu"
    sysroots = Path(os.environ.get(
        "SLATE_SYSROOTS",
        str(Path(__file__).resolve().parents[1] / "../slate-sysroots/sysroots"),
    ))
    root = sysroots / target
    if job.flavor == "msvc":
        candidates = [
            root / "crt/include",
            root / "sdk/include/ucrt",
            root / "sdk/include/shared",
            root / "sdk/include/um",
            root / "sdk/include/winrt",
            root / "sdk/include/cppwinrt",
        ]
    else:
        candidates = [root / "SDK/usr/include", root / "usr/include", root / "include"]
        if target.endswith("-unknown-linux-gnu"):
            arch = target.removesuffix("-unknown-linux-gnu")
            candidates.append(root / "usr" / f"{arch}-linux-gnu/include")
    return [str(path) for path in candidates if path.is_dir()]


def external_common_args(job: Job) -> list[str]:
    args = common_args(job)
    args.extend(f"-isystem{path}" for path in sysroot_include_paths(job))
    return args


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


def run_gcc(job: Job, gcc: str) -> Result:
    gcc_job = Job(
        fixture=job.fixture,
        prefix=job.prefix,
        defines=job.defines,
        standard=job.standard,
        flavor=job.flavor,
        target=None,
        isystem=job.isystem,
        extra_args=job.extra_args,
    )
    args = [gcc, "-fsyntax-only", job.fixture, *external_common_args(gcc_job)]
    args.extend(arg for arg in job.extra_args if arg.startswith("-W"))
    completed = subprocess.run(args, text=True, capture_output=True)
    return Result(
        fixture=job.fixture,
        prefix=job.prefix,
        tool="gcc",
        accepted=completed.returncode == 0,
        root="" if completed.returncode == 0 else root_diagnostic(completed.stderr),
        stderr=completed.stderr[-4000:],
        warnings=warnings(completed.stderr, "gcc"),
    )


def run_clang(job: Job, clang: str) -> Result:
    args = [clang, "-fsyntax-only", job.fixture, *external_common_args(job)]
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


def msvc_args(job: Job, msvc: str) -> list[str]:
    args = [msvc, "/nologo", "/Zs", "/TC"]
    args.extend(f"/D{define.removeprefix('-D')}" for define in job.defines)
    args.extend(
        f"/I{path}"
        for path in [*job.isystem, *sysroot_include_paths(job)]
        if "/lib/clang/" not in path
    )
    standard = job.standard.removeprefix("gnu") if job.standard else None
    if standard in {"c11", "c17"}:
        args.append(f"/std:{standard}")
    elif standard in {"c89", "c99", "c23"}:
        args.append("/std:c17")
    return args


def run_msvc_batch(batch: list[Job], msvc: str) -> list[Result]:
    args = msvc_args(batch[0], msvc)
    args.extend(str(Path(job.fixture).resolve()) for job in batch)
    completed = subprocess.run(args, text=True, capture_output=True)
    output = completed.stdout + completed.stderr
    if completed.returncode == 0:
        return [Result(job.fixture, job.prefix, "msvc", True, "", output[-4000:], warnings(output, "msvc")) for job in batch]
    failures: dict[str, list[str]] = defaultdict(list)
    for line in output.splitlines():
        match = re.search(r"^(.+?\.c)\(\d+\):\s*error\s+C\d+:\s*.*$", line, re.IGNORECASE)
        if match:
            location = match.group(1).replace("\\", "/").casefold()
            for job in batch:
                if location.endswith(job.fixture.replace("\\", "/").casefold()):
                    failures[job.fixture].append(line)
                    break
    if len(batch) > 1 and not failures:
        middle = len(batch) // 2
        return run_msvc_batch(batch[:middle], msvc) + run_msvc_batch(batch[middle:], msvc)
    if len(batch) > 1 and len(failures) < len(batch):
        known = [
            Result(job.fixture, job.prefix, "msvc", False, root_diagnostic("\n".join(failures[job.fixture])), "\n".join(failures[job.fixture])[-4000:], warnings(output, "msvc"))
            for job in batch if job.fixture in failures
        ]
        unresolved = [job for job in batch if job.fixture not in failures]
        return known + run_msvc_batch(unresolved, msvc)
    if len(batch) > 1:
        return [
            Result(job.fixture, job.prefix, "msvc", False, root_diagnostic("\n".join(failures[job.fixture])), "\n".join(failures[job.fixture])[-4000:], warnings(output, "msvc"))
            for job in batch
        ]
    job = batch[0]
    return [
        Result(
            fixture=job.fixture,
            prefix=job.prefix,
            tool="msvc",
            accepted=False,
            root=root_diagnostic(output),
            stderr=output[-4000:],
            warnings=warnings(output, "msvc"),
        )
    ]


def msvc_batches(corpus: list[Job], msvc: str, batch_size: int = 32) -> list[list[Job]]:
    groups: dict[tuple[str, ...], list[Job]] = defaultdict(list)
    for job in corpus:
        groups[tuple(msvc_args(job, msvc))].append(job)
    return [
        group[start : start + batch_size]
        for group in groups.values()
        for start in range(0, len(group), batch_size)
    ]


def slate_failure_kind(root: str) -> str:
    if "not implemented: " in root:
        return "unimplemented"
    if "internal error: " in root:
        return "internal"
    return "rejected"


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
    for tool in ("gcc", "msvc"):
        if all((fixture, prefix, candidate) in indexed for fixture, prefix in pairs for candidate in ("slate-ir", tool)):
            lines.append(
                f"- Slate IR rejects / {tool} accepts: "
                + str(sum(not indexed[(fixture, prefix, "slate-ir")].accepted and indexed[(fixture, prefix, tool)].accepted for fixture, prefix in pairs))
            )
            lines.append(
                f"- Slate IR accepts / {tool} rejects: "
                + str(sum(indexed[(fixture, prefix, "slate-ir")].accepted and not indexed[(fixture, prefix, tool)].accepted for fixture, prefix in pairs))
            )
        if all((fixture, prefix, candidate) in indexed for fixture, prefix in pairs for candidate in ("clang", tool)):
            lines.append(
                f"- Clang rejects / {tool} accepts: "
                + str(sum(not indexed[(fixture, prefix, "clang")].accepted and indexed[(fixture, prefix, tool)].accepted for fixture, prefix in pairs))
            )
            lines.append(
                f"- Clang accepts / {tool} rejects: "
                + str(sum(indexed[(fixture, prefix, "clang")].accepted and not indexed[(fixture, prefix, tool)].accepted for fixture, prefix in pairs))
            )
    kinds: dict[str, int] = defaultdict(int)
    for result in results:
        if result.tool == "slate-ir" and not result.accepted:
            kinds[slate_failure_kind(result.root)] += 1
    if kinds:
        lines.append(
            "- Slate IR failures by kind: "
            + ", ".join(f"{kind} {kinds[kind]}" for kind in ("rejected", "unimplemented", "internal"))
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
        kind = f" [{slate_failure_kind(root)}]" if tool == "slate-ir" else ""
        lines.extend(["", f"## {tool}{kind}: {root} ({len(members)})", ""])
        lines.extend(f"- `{member.fixture}` ({member.prefix})" for member in members)
    path.write_text("\n".join(lines) + "\n")


def version(command: str, tool: str) -> str:
    if tool == "msvc":
        completed = subprocess.run([command], text=True, capture_output=True)
        lines = (completed.stdout + completed.stderr).splitlines()
        return next((line for line in lines if line.startswith("Microsoft (R)")), lines[0] if lines else "unknown")
    completed = subprocess.run([command, "--version"], text=True, capture_output=True)
    return completed.stdout.splitlines()[0] if completed.stdout else "unknown"


def git_revision() -> str:
    completed = subprocess.run(["git", "rev-parse", "--short", "HEAD"], text=True, capture_output=True)
    return completed.stdout.strip() or "unknown"


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--fixtures", type=Path, default=Path("tests/fixtures"))
    parser.add_argument("--slate-parser", default="target/test-cache/release/slate-parser")
    parser.add_argument("--clang", default="clang")
    parser.add_argument("--gcc", default="gcc")
    parser.add_argument("--msvc", default="tools/cl.exe")
    parser.add_argument("--tool", choices=("ir", "clang", "gcc", "msvc", "both", "all"), default="both")
    parser.add_argument("--jobs", type=int, default=min(12, os.cpu_count() or 1))
    parser.add_argument("--report", type=Path, default=Path("target/corpus-sweep.md"))
    parser.add_argument("--json", type=Path, default=Path("target/corpus-sweep.json"))
    args = parser.parse_args()
    corpus, expected_errors = jobs(args.fixtures)
    runners = []
    msvc_requested = args.tool in {"msvc", "all"}
    if args.tool in {"ir", "both"}:
        runners.append((run_ir, args.slate_parser))
    if args.tool in {"clang", "both", "all"}:
        runners.append((run_clang, args.clang))
    if args.tool in {"gcc", "both", "all"}:
        runners.append((run_gcc, args.gcc))
    work = [(runner, command, job) for runner, command in runners for job in corpus]
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        results = list(pool.map(lambda item: item[0](item[2], item[1]), work))
    if msvc_requested:
        batches = msvc_batches(corpus, args.msvc)
        for batch in batches:
            results.extend(run_msvc_batch(batch, args.msvc))
    versions = {"slate-parser": f"git {git_revision()} ({args.slate_parser})"}
    if args.tool in {"clang", "both", "all"}:
        versions["clang"] = version(args.clang, "clang")
    if args.tool in {"gcc", "both", "all"}:
        versions["gcc"] = version(args.gcc, "gcc")
    if args.tool in {"msvc", "all"}:
        versions["msvc"] = version(args.msvc, "msvc")
        versions["msvc language modes"] = "C11/C17; C89/C99/C23 configurations use C17"
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.json.parent.mkdir(parents=True, exist_ok=True)
    write_report(args.report, results, expected_errors, versions)
    args.json.write_text(json.dumps([asdict(result) for result in results], indent=2) + "\n")


if __name__ == "__main__":
    main()
