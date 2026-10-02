#!/usr/bin/env python3
import argparse
import json
import pathlib
import re
import statistics
import subprocess
import time


def build_driver(source, headers, library, output, clang="clang"):
    subprocess.run([
        clang, "-O2", "-g", f"-I{headers}", str(source), str(library),
        f"-Wl,-rpath,{library.parent}", "-lm", "-lz", "-ldl", "-pthread",
        "-o", str(output),
    ], check=True)


def sample(driver, testset, size):
    command = [str(driver), "--memdb", "--verify", "--size", str(size), "--testset", testset]
    start = time.perf_counter()
    result = subprocess.run(command, capture_output=True, text=True, timeout=180, check=True)
    elapsed = time.perf_counter() - start
    verification = re.search(r"^Verification Hash: (.+)$", result.stdout, re.MULTILINE)
    total = re.search(r"^\s+TOTAL\.+\s+([\d.]+)s$", result.stdout, re.MULTILINE)
    if not verification or not total:
        raise RuntimeError(result.stdout + result.stderr)
    return {"wall_seconds": elapsed, "sqlite_seconds": float(total[1]), "verification": verification[1]}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("corpus", type=pathlib.Path)
    parser.add_argument("native_library", type=pathlib.Path)
    parser.add_argument("generated_library", type=pathlib.Path)
    parser.add_argument("output", type=pathlib.Path)
    parser.add_argument("--size", type=int, default=20)
    parser.add_argument("--runs", type=int, default=5)
    parser.add_argument("--testsets", nargs="+", default=["main", "cte", "json", "fp"])
    parser.add_argument("--clang", default="clang")
    args = parser.parse_args()
    if args.runs < 1 or args.size < 1:
        parser.error("runs and size must be positive")
    args.output.mkdir(parents=True, exist_ok=True)
    binaries = {}
    for name, library in [("native", args.native_library), ("generated", args.generated_library)]:
        binaries[name] = (args.output / f"speedtest1-{name}").resolve()
        build_driver(args.corpus / "test/speedtest1.c", args.corpus / "build-clang", library.resolve(), binaries[name], args.clang)
    report = {"size": args.size, "runs": args.runs, "mode": "memory", "testsets": {}}
    for testset in args.testsets:
        samples = {name: [] for name in binaries}
        warmups = {name: sample(binary, testset, args.size) for name, binary in binaries.items()}
        if warmups["native"]["verification"] != warmups["generated"]["verification"]:
            raise AssertionError((testset, warmups))
        for run in range(args.runs):
            order = list(binaries) if run % 2 == 0 else list(reversed(binaries))
            for name in order:
                result = sample(binaries[name], testset, args.size)
                if result["verification"] != warmups["native"]["verification"]:
                    raise AssertionError((testset, name, result))
                samples[name].append(result)
        medians = {name: statistics.median(row["wall_seconds"] for row in rows) for name, rows in samples.items()}
        spread = {name: {"min": min(row["wall_seconds"] for row in rows),
                         "max": max(row["wall_seconds"] for row in rows),
                         "stdev": statistics.pstdev(row["wall_seconds"] for row in rows)}
                  for name, rows in samples.items()}
        ratio = medians["generated"] / medians["native"]
        report["testsets"][testset] = {"samples": samples, "median_wall_seconds": medians,
                                      "spread_wall_seconds": spread, "generated_over_native": ratio}
        (args.output / "runtime.json").write_text(json.dumps(report, indent=2) + "\n")
        print(f"{testset}: native {medians['native']:.3f}s, generated {medians['generated']:.3f}s, ratio {ratio:.2f}x; verification matched", flush=True)


if __name__ == "__main__":
    main()
