#!/usr/bin/env python3
import argparse
import collections
import json
import pathlib
import platform
import re
import subprocess
import time


def run(command, output, error):
    with output.open("w") as stdout, error.open("w") as stderr:
        subprocess.run(command, stdout=stdout, stderr=stderr, check=True)


def sample_summary(text):
    periods = collections.Counter()
    for line in text.splitlines():
        fields = line.split(maxsplit=2)
        if len(fields) == 3:
            symbol = fields[2].split("+0x", 1)[0]
            periods[symbol] += int(fields[0])
    total = sum(periods.values())
    return [{"symbol": name, "percent": 100 * period / total}
            for name, period in periods.most_common(20)] if total else []


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("native_driver", type=pathlib.Path)
    parser.add_argument("generated_driver", type=pathlib.Path)
    parser.add_argument("output", type=pathlib.Path)
    parser.add_argument("--size", type=int, default=50)
    parser.add_argument("--testsets", nargs="+", default=["main", "cte"])
    parser.add_argument("--frequency", type=int, default=997)
    parser.add_argument("--stats", action="store_true")
    args = parser.parse_args()
    if args.size < 1 or args.frequency < 1:
        parser.error("size and frequency must be positive")
    args.output.mkdir(parents=True, exist_ok=True)
    report = {"platform": platform.platform(), "size": args.size,
              "frequency": args.frequency, "runs": []}
    for testset in args.testsets:
        hashes = {}
        for name, driver in [("native", args.native_driver), ("generated", args.generated_driver)]:
            prefix = args.output / f"{testset}-{name}"
            command = [str(driver.resolve()), "--memdb", "--verify", "--size", str(args.size), "--testset", testset]
            record = ["perf", "record", "-e", "cycles:u", "-F", str(args.frequency), "-o", str(prefix.with_suffix(".data")), "--", *command]
            start = time.perf_counter()
            run(record, prefix.with_suffix(".stdout"), prefix.with_suffix(".stderr"))
            elapsed = time.perf_counter() - start
            verification = re.search(r"^Verification Hash: (.+)$", prefix.with_suffix(".stdout").read_text(), re.MULTILINE)
            if not verification:
                raise RuntimeError(f"{prefix}: missing verification hash")
            hashes[name] = verification[1]
            run(["perf", "report", "--stdio", "--no-children", "--percent-limit", "0.5", "-i", str(prefix.with_suffix(".data"))], prefix.with_suffix(".report"), prefix.with_suffix(".report.stderr"))
            run(["perf", "script", "-i", str(prefix.with_suffix(".data")), "-F", "period,ip,sym,symoff"], prefix.with_suffix(".samples"), prefix.with_suffix(".samples.stderr"))
            row = {"testset": testset, "version": name, "record_command": record,
                   "profile_seconds": elapsed, "verification": verification[1],
                   "symbols": sample_summary(prefix.with_suffix(".samples").read_text())}
            if args.stats:
                stat = ["perf", "stat", "-e", "cycles:u,instructions:u,branches:u,branch-misses:u", "-o", str(prefix.with_suffix(".stat")), "--", *command]
                run(stat, prefix.with_suffix(".stat.stdout"), prefix.with_suffix(".stat.stderr"))
                row["stat_command"] = stat
                stat_hash = re.search(r"^Verification Hash: (.+)$", prefix.with_suffix(".stat.stdout").read_text(), re.MULTILINE)
                if not stat_hash or stat_hash[1] != verification[1]:
                    raise AssertionError(f"{prefix}: counter run verification differs")
            report["runs"].append(row)
            (args.output / "profiles.json").write_text(json.dumps(report, indent=2) + "\n")
            print(f"{testset} {name}: {elapsed:.3f}s under profiling; {row['symbols'][:3]}", flush=True)
        if hashes["native"] != hashes["generated"]:
            raise AssertionError((testset, hashes))
        print(f"PASS {testset} verification", flush=True)


if __name__ == "__main__":
    main()
