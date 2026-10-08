import argparse
import hashlib
import json
import os
import platform
import statistics
import subprocess
import threading
import time
from datetime import datetime, timezone
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
TARGET = "x86_64-unknown-linux-gnu"
FLAGS = ["-std=c11", "-fno-common", "-Wall", "-Wno-switch"]


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def measure(command, stderr_path, timeout):
    with open(os.devnull, "wb") as sink, stderr_path.open("wb") as errors:
        start = time.perf_counter()
        process = subprocess.Popen(command, stdout=sink, stderr=errors)
        timer = threading.Timer(timeout, process.kill)
        timer.start()
        try:
            _, status, usage = os.wait4(process.pid, 0)
            elapsed = time.perf_counter() - start
            process.returncode = os.waitstatus_to_exitcode(status)
        finally:
            timer.cancel()
        if process.returncode:
            raise RuntimeError(
                f"IR emission failed ({process.returncode}): {command}\n"
                + stderr_path.read_text(errors="replace")[-4000:]
            )
    return {
        "wall_seconds": elapsed,
        "user_seconds": usage.ru_utime,
        "system_seconds": usage.ru_stime,
        "peak_rss_mib": usage.ru_maxrss / 1024,
        "minor_faults": usage.ru_minflt,
        "major_faults": usage.ru_majflt,
    }


def main():
    parser = argparse.ArgumentParser(
        description="Whole-project chibicc IR emission benchmark"
    )
    parser.add_argument(
        "--project", type=Path, default=Path.home() / "c-corpus/chibicc"
    )
    parser.add_argument(
        "--slate", type=Path, default=ROOT / "target/test-cache/release/slate-parser"
    )
    parser.add_argument(
        "--clang", type=Path, default=Path.home() / "llvm-project/build-cir/bin/clang"
    )
    parser.add_argument(
        "--output", type=Path, default=ROOT / "target/chibicc-ir-shootout"
    )
    parser.add_argument("--runs", type=int, default=9)
    parser.add_argument("--timeout", type=float, default=60)
    parser.add_argument("--profile-tool", choices=["slate", "clang"])
    parser.add_argument("--profile-repeats", type=int, default=20)
    args = parser.parse_args()
    if Path.cwd() != ROOT:
        parser.error("run from the Slate workspace root")
    if args.runs < 1 or args.profile_repeats < 1 or args.timeout <= 0:
        parser.error("runs, profile-repeats, and timeout must be positive")
    project = args.project.resolve()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    sources = sorted(project.glob("*.c"))
    if not sources:
        parser.error(f"no project sources in {project}")
    commands = {"slate": [], "clang": []}
    for source in sources:
        commands["slate"].append(
            [
                str(args.slate.resolve()),
                "ir",
                str(source),
                "--flavor=clang",
                f"-target={TARGET}",
                *FLAGS,
            ]
        )
        commands["clang"].append(
            [
                str(args.clang.resolve()),
                "-emit-cir",
                str(source),
                f"--target={TARGET}",
                "-O0",
                *FLAGS,
                "-o",
                os.devnull,
            ]
        )
    if args.profile_tool:
        for _ in range(args.profile_repeats):
            for command in commands[args.profile_tool]:
                subprocess.run(command, stdout=subprocess.DEVNULL, check=True)
        return

    validated = []
    for tool in commands:
        for source, command in zip(sources, commands[tool]):
            path = output / f"{source.stem}.{tool}.ir"
            check = command[:]
            if tool == "clang":
                check[-1] = str(path)
            with (
                path.open("wb") if tool == "slate" else open(os.devnull, "wb") as stream
            ):
                subprocess.run(
                    check,
                    stdout=stream,
                    stderr=subprocess.PIPE,
                    check=True,
                    timeout=args.timeout,
                )
            if path.stat().st_size == 0:
                raise RuntimeError(f"empty IR: {path}")
            validated.append(
                {
                    "tool": tool,
                    "source": source.name,
                    "bytes": path.stat().st_size,
                    "sha256": digest(path),
                }
            )
        print(f"validated {tool}: {len(sources)} nonempty IR files", flush=True)

    samples = {tool: [] for tool in commands}
    for iteration in range(args.runs):
        order = list(commands) if iteration % 2 == 0 else list(reversed(commands))
        for tool in order:
            start = time.perf_counter()
            rows = [
                {
                    "source": source.name,
                    **measure(command, output / f"{tool}.stderr", args.timeout),
                }
                for source, command in zip(sources, commands[tool])
            ]
            sample = {
                "iteration": iteration + 1,
                "project_wall_seconds": time.perf_counter() - start,
                "peak_rss_mib": max(row["peak_rss_mib"] for row in rows),
                "files": rows,
            }
            for key in (
                "wall_seconds",
                "user_seconds",
                "system_seconds",
                "minor_faults",
                "major_faults",
            ):
                sample[key] = sum(row[key] for row in rows)
            samples[tool].append(sample)
            print(
                f"round {iteration + 1}: {tool} {sample['wall_seconds']:.4f}s, "
                f"{sample['peak_rss_mib']:.1f} MiB",
                flush=True,
            )

    summary = {}
    for tool, rows in samples.items():
        summary[tool] = {
            key: statistics.median(row[key] for row in rows)
            for key in (
                "wall_seconds",
                "project_wall_seconds",
                "user_seconds",
                "system_seconds",
                "peak_rss_mib",
            )
        }
        summary[tool]["wall_min_seconds"] = min(row["wall_seconds"] for row in rows)
        summary[tool]["wall_max_seconds"] = max(row["wall_seconds"] for row in rows)
    metadata = {
        "timestamp": datetime.now(timezone.utc).isoformat(),
        "platform": platform.platform(),
        "cpu": next(
            (
                line.split(":", 1)[1].strip()
                for line in Path("/proc/cpuinfo").read_text().splitlines()
                if line.startswith("model name")
            ),
            "unknown",
        ),
        "cpu_affinity": sorted(os.sched_getaffinity(0)),
        "slate_commit": subprocess.check_output(
            ["git", "rev-parse", "HEAD"], text=True
        ).strip(),
        "project_commit": subprocess.check_output(
            ["git", "-C", str(project), "rev-parse", "HEAD"], text=True
        ).strip(),
        "clang_version": subprocess.check_output(
            [str(args.clang.resolve()), "--version"], text=True
        ).strip(),
        "binary_sha256": {
            "slate": digest(args.slate.resolve()),
            "clang": digest(args.clang.resolve()),
        },
        "source_sha256": {p.name: digest(p) for p in [*sources, project / "chibicc.h"]},
        "method": "sequential complete project; one validation/warmup pass per tool; alternating order; text IR to /dev/null; O0, no debug info; default CIR passes/verifier; each tool uses its default headers; Linux wait4 peak RSS, max across sequential files",
    }
    result = {
        "metadata": metadata,
        "commands": commands,
        "validated_ir": validated,
        "samples": samples,
        "summary": summary,
    }
    (output / "results.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(summary, indent=2))
    print(
        f"Clang/Slate wall ratio: {summary['clang']['wall_seconds'] / summary['slate']['wall_seconds']:.3f}"
    )


if __name__ == "__main__":
    main()
