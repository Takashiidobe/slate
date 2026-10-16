#!/usr/bin/env python3
import argparse
import hashlib
import json
import os
import pathlib
import shlex
import sqlite3
import subprocess


ROOT = pathlib.Path(__file__).resolve().parents[2]
HARNESS = ROOT / "tools/fuzz/sqlite_differential.c"


def run(command, env=None):
    print(shlex.join(map(str, command)), flush=True)
    return subprocess.run(command, cwd=ROOT, env=env).returncode


def build(args):
    configuration = json.loads((args.libraries / "build.json").read_text())["configuration"]
    core = configuration["sqlite3.c"]
    flags = [flag for flag in core["flags"] if flag not in {"-O2", "-g"}]
    args.output.mkdir(parents=True, exist_ok=True)
    instrumented = args.output / "sqlite3.o"
    commands = [
        [args.clang, *flags, "-O2", "-g", "-fsanitize=fuzzer-no-link", "-c", core["source"],
         "-o", instrumented],
        [args.clang, "-O2", "-g", "-fsanitize=fuzzer",
         "-I", pathlib.Path(core["source"]).parent, HARNESS, instrumented,
         "-lm", "-lz", "-ldl", "-pthread", "-o", args.output / "sqlite_differential"],
    ]
    for command in commands:
        if run(command):
            raise SystemExit(1)


def seeds(args):
    directory = args.output / "seeds"
    directory.mkdir(parents=True, exist_ok=True)
    count = 0
    for database in sorted((args.corpus / "test").glob("fuzzdata*.db")):
        connection = sqlite3.connect(f"file:{database}?mode=ro", uri=True)
        connection.text_factory = bytes
        for (data,) in connection.execute("SELECT CAST(sqltext AS BLOB) FROM xsql"):
            (directory / hashlib.sha1(data).hexdigest()).write_bytes(data)
            count += 1
        connection.close()
    print(f"{count} seeds in {directory}")


def fuzz(args):
    corpus = args.output / "corpus"
    artifacts = args.output / "artifacts"
    corpus.mkdir(parents=True, exist_ok=True)
    artifacts.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ, SLATE_SQLITE_TRANSLATED=str(args.libraries / "rust/libsqlite3.so"))
    inputs = [corpus, args.output / "seeds"] if not args.inputs else args.inputs
    options = [f"-artifact_prefix={artifacts}/", "-max_len=16384", "-timeout=30",
               "-rss_limit_mb=4096", "-print_final_stats=1"]
    if not args.inputs:
        options += [f"-max_total_time={args.seconds}", f"-fork={args.jobs}",
                    "-ignore_crashes=1"]
    return run([args.output / "sqlite_differential", *options, *args.extra, *inputs], env)


def main():
    parser = argparse.ArgumentParser(description="Differentially fuzz native SQLite against its Slate translation.")
    parser.add_argument("command", choices=["build", "seeds", "fuzz"])
    parser.add_argument("inputs", nargs="*", type=pathlib.Path,
                        help="fuzz: replay these inputs instead of fuzzing")
    parser.add_argument("--corpus", type=pathlib.Path, default=pathlib.Path.home() / "c-corpus/sqlite")
    parser.add_argument("--libraries", type=pathlib.Path, default=ROOT / "target/sqlite-benchmark/fuzz",
                        help="a tools/benchmark/sqlite.py --build-only output directory")
    parser.add_argument("--output", type=pathlib.Path, default=ROOT / "target/sqlite-fuzz")
    parser.add_argument("--clang", default="clang")
    parser.add_argument("--seconds", type=int, default=600)
    parser.add_argument("--jobs", type=int, default=os.cpu_count())
    parser.add_argument("--extra", nargs="*", default=[], help="additional libFuzzer options")
    args = parser.parse_args()
    for name in ["corpus", "libraries", "output"]:
        setattr(args, name, getattr(args, name).expanduser().resolve())
    if args.command == "build":
        build(args)
    elif args.command == "seeds":
        seeds(args)
    else:
        raise SystemExit(fuzz(args))


if __name__ == "__main__":
    main()
