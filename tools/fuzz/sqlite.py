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
FEATURES = [
    "SQLITE_ENABLE_FTS3", "SQLITE_ENABLE_FTS3_PARENTHESIS", "SQLITE_ENABLE_FTS4",
    "SQLITE_ENABLE_FTS5", "SQLITE_ENABLE_RTREE", "SQLITE_ENABLE_GEOPOLY",
    "SQLITE_ENABLE_DBSTAT_VTAB", "SQLITE_ENABLE_DBPAGE_VTAB", "SQLITE_ENABLE_BYTECODE_VTAB",
    "SQLITE_ENABLE_STMTVTAB", "SQLITE_ENABLE_CARRAY", "SQLITE_ENABLE_OFFSET_SQL_FUNC",
    "SQLITE_ENABLE_STAT4", "SQLITE_ENABLE_ORDERED_SET_AGGREGATES",
]


def run(command, env=None, cwd=ROOT):
    print(shlex.join(map(str, command)), flush=True)
    return subprocess.run(command, cwd=cwd, env=env).returncode


def libraries(args):
    entries = json.loads((args.corpus / "build-clang/compile_commands.json").read_text())
    defines = [f"-D{feature}" for feature in FEATURES]
    for entry in entries:
        if pathlib.Path(entry["file"]).name != "sqlite3.c":
            continue
        if "arguments" in entry:
            entry["arguments"] = [*entry["arguments"], *defines]
        else:
            entry["command"] = f"{entry['command']} {shlex.join(defines)}"
    args.output.mkdir(parents=True, exist_ok=True)
    database = args.output / "compile_commands.json"
    database.write_text(json.dumps(entries, indent=2) + "\n")
    if run(["python3", ROOT / "tools/benchmark/sqlite.py", args.corpus, "--build-only",
            "--compile-commands", database, "--output", args.libraries.parent,
            "--label", args.libraries.name]):
        raise SystemExit(1)


def encode_image(image):
    lines = []
    skipped = False
    for offset in range(0, len(image), 16):
        row = image[offset:offset + 16]
        if not any(row) and offset + 16 < len(image):
            skipped = True
            continue
        prefix = f"[{offset:x}]" if skipped else ""
        lines.append(prefix + row.hex())
        skipped = False
    return "\n".join(lines).encode()


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
        scripts = [data for (data,) in connection.execute("SELECT CAST(sqltext AS BLOB) FROM xsql")]
        images = [data for (data,) in connection.execute("SELECT CAST(dbcontent AS BLOB) FROM db")]
        connection.close()
        if images:
            scripts = [encode_image(image) + b"\n--\n" + script
                       for image in images for script in scripts]
        for data in scripts:
            (directory / hashlib.sha1(data).hexdigest()).write_bytes(data)
            count += 1
    print(f"{count} seeds in {directory}")


def fuzz(args):
    corpus = args.output / "corpus"
    artifacts = args.output / "artifacts"
    divergences = args.output / "divergences"
    for directory in [corpus, artifacts, divergences]:
        directory.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ, SLATE_SQLITE_TRANSLATED=str(args.libraries / "rust/libsqlite3.so"))
    inputs = [corpus, args.output / "seeds"] if not args.inputs else args.inputs
    options = [f"-artifact_prefix={artifacts}/", "-max_len=65536", "-timeout=30",
               "-rss_limit_mb=4096", "-print_final_stats=1"]
    if not args.inputs:
        env["SLATE_SQLITE_DIVERGENCES"] = str(divergences)
        options += [f"-max_total_time={args.seconds}", f"-jobs={args.jobs}",
                    f"-workers={args.jobs}"]
    return run([args.output / "sqlite_differential", *options, *args.extra, *inputs], env,
               cwd=args.output)


def main():
    parser = argparse.ArgumentParser(description="Differentially fuzz native SQLite against its Slate translation.")
    parser.add_argument("command", choices=["libraries", "build", "seeds", "fuzz"])
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
    if args.command == "libraries":
        libraries(args)
    elif args.command == "build":
        build(args)
    elif args.command == "seeds":
        seeds(args)
    else:
        raise SystemExit(fuzz(args))


if __name__ == "__main__":
    main()
