import argparse
import json
import os
import platform
import resource
import shlex
import signal
import statistics
import subprocess
import threading
import time
from datetime import datetime, timezone
from pathlib import Path

from ir_shootout import ROOT, digest


def run(command, output, error, timeout, memory_mib):
    def limits():
        resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
        if memory_mib:
            size = memory_mib * 1024 * 1024
            resource.setrlimit(resource.RLIMIT_AS, (size, size))

    with output.open("wb") as stream, error.open("wb") as errors:
        start = time.perf_counter()
        process = subprocess.Popen(
            command,
            stdout=stream,
            stderr=errors,
            preexec_fn=limits,
            start_new_session=True,
        )
        expired = threading.Event()

        def kill():
            expired.set()
            try:
                os.killpg(process.pid, signal.SIGKILL)
            except ProcessLookupError:
                pass

        timer = threading.Timer(timeout, kill)
        timer.start()
        try:
            _, status, usage = os.wait4(process.pid, 0)
            process.returncode = os.waitstatus_to_exitcode(status)
        except BaseException:
            try:
                os.killpg(process.pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
            _, status, _ = os.wait4(process.pid, 0)
            process.returncode = os.waitstatus_to_exitcode(status)
            raise
        finally:
            timer.cancel()
            timer.join()
        return {
            "exit_code": process.returncode,
            "timeout": expired.is_set(),
            "wall_seconds": time.perf_counter() - start,
            "user_seconds": usage.ru_utime,
            "system_seconds": usage.ru_stime,
            "peak_rss_mib": usage.ru_maxrss / 1024,
        }


def flags(row):
    arguments = row.get("arguments") or shlex.split(row["command"])
    result = []
    skip = False
    for arg in arguments[1:]:
        if skip:
            skip = False
            continue
        if arg in ("-o", "-MF", "-MT", "-MQ"):
            skip = True
        elif arg == "-c" or arg == row["file"]:
            continue
        elif arg.startswith(("-O", "-g", "-W")) or arg in ("-MD", "-MMD"):
            continue
        elif arg == "-I/usr/include":
            result.append("-idirafter/usr/include")
        else:
            result.append(arg)
    return ["-std=gnu17", *result]


def main():
    parser = argparse.ArgumentParser(
        description="Bounded whole-production curl IR shootout"
    )
    parser.add_argument("--project", type=Path, default=Path.home() / "c-corpus/curl")
    parser.add_argument(
        "--clang", type=Path, default=Path.home() / "llvm-project/build-cir/bin/clang"
    )
    parser.add_argument(
        "--slate", type=Path, default=ROOT / "target/test-cache/release/slate-parser"
    )
    parser.add_argument("--output", type=Path, default=ROOT / "target/curl-ir-shootout")
    parser.add_argument("--runs", type=int, default=7)
    parser.add_argument("--timeout", type=float, default=30)
    parser.add_argument("--memory-mib", type=int, default=2048)
    args = parser.parse_args()
    if Path.cwd() != ROOT:
        parser.error("run from the workspace root")
    if args.runs < 1 or args.timeout <= 0 or args.memory_mib < 1:
        parser.error("runs, timeout, and memory-mib must be positive")
    project = args.project.resolve()
    build = project / "build-clang"
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    database = json.loads((build / "compile_commands.json").read_text())
    rows = sorted(
        (
            r
            for r in database
            if any(
                f"CMakeFiles/{t}.dir/" in r["output"]
                for t in ("libcurl_shared", "curl")
            )
        ),
        key=lambda r: r["file"],
    )
    if not rows:
        parser.error("no curl production entries")
    commands = {"slate": [], "cir": [], "clang": []}
    for row in rows:
        source = str(Path(row["file"]).resolve())
        common = [*flags(row), "--target=x86_64-unknown-linux-gnu"]
        commands["slate"].append(
            [str(args.slate.resolve()), "ir", source, "--flavor=clang", *common]
        )
        for tool, mode in (("cir", "-emit-cir"), ("clang", "-emit-llvm")):
            commands[tool].append(
                [str(args.clang.resolve()), mode, "-O0", source, *common, "-o", "-"]
                + (["-S"] if tool == "clang" else [])
            )
    result = {
        "timestamp": datetime.now(timezone.utc).isoformat(),
        "platform": platform.platform(),
        "cpu_affinity": sorted(os.sched_getaffinity(0)),
        "project": str(project),
        "translation_units": len(rows),
        "source_lines": sum(
            len(Path(r["file"]).read_bytes().splitlines()) for r in rows
        ),
        "commands": commands,
        "validation": [],
        "samples": {},
        "limits": {
            "timeout_seconds": args.timeout,
            "cir_address_space_mib": args.memory_mib,
        },
        "method": "complete production libcurl_shared and curl executable TUs from compile database; O0/no debug; warnings removed; host /usr/include moved to idirafter for both; tool default sysroots; sequential text IR emission; CIR default verifier/passes",
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
            "cir": digest(args.clang.resolve()),
        },
        "input_sha256": {
            str(path.relative_to(project)): digest(path)
            for path in [
                *(Path(row["file"]) for row in rows),
                build / "compile_commands.json",
                build / "lib/curl_config.h",
            ]
        },
    }

    def save():
        (output / "results.json").write_text(json.dumps(result, indent=2) + "\n")

    print(
        f"Production workload: {len(rows)} TUs, {result['source_lines']} C lines",
        flush=True,
    )
    for index, row in enumerate(rows):
        for tool in ("cir", "slate"):
            ir = output / f"{index}.{tool}.ir"
            stderr = output / f"{index}.{tool}.stderr"
            sample = run(
                commands[tool][index],
                ir,
                stderr,
                args.timeout,
                args.memory_mib if tool == "cir" else 0,
            )
            sample.update(
                source=row["file"],
                tool=tool,
                ir_bytes=ir.stat().st_size,
                stderr=stderr.read_text(errors="replace"),
            )
            result["validation"].append(sample)
            if sample["exit_code"] or not sample["ir_bytes"]:
                result["status"] = "validation_failed"
                normal = run(
                    commands["clang"][index],
                    output / "oracle.ll",
                    output / "oracle.stderr",
                    args.timeout,
                    0,
                )
                normal["stderr"] = (output / "oracle.stderr").read_text(
                    errors="replace"
                )
                result["normal_clang_on_failure"] = normal
                save()
                print(json.dumps(sample, indent=2), flush=True)
                print("Normal Clang:", normal, flush=True)
                return
            sample["ir_sha256"] = digest(ir)
        if (index + 1) % 25 == 0:
            print(f"Validated {index + 1}/{len(rows)} TUs with both tools", flush=True)
    save()
    for iteration in range(args.runs):
        for tool in ("slate", "cir") if iteration % 2 == 0 else ("cir", "slate"):
            samples = []
            for command in commands[tool]:
                sample = run(
                    command,
                    Path(os.devnull),
                    output / "timing.stderr",
                    args.timeout,
                    args.memory_mib if tool == "cir" else 0,
                )
                if sample["exit_code"]:
                    result["status"] = "timing_failed"
                    result["timing_failure"] = {"command": command, **sample}
                    save()
                    raise RuntimeError("timed emission failed")
                samples.append(sample)
            total = {
                key: sum(s[key] for s in samples)
                for key in ("wall_seconds", "user_seconds", "system_seconds")
            }
            total["peak_rss_mib"] = max(s["peak_rss_mib"] for s in samples)
            result["samples"].setdefault(tool, []).append(total)
            print(f"Round {iteration + 1} {tool}: {total}", flush=True)
            save()
    result["summary"] = {
        tool: {key: statistics.median(s[key] for s in samples) for key in samples[0]}
        for tool, samples in result["samples"].items()
    }
    result["status"] = "complete"
    save()
    print(json.dumps(result["summary"], indent=2))


if __name__ == "__main__":
    main()
