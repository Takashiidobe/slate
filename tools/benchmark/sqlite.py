#!/usr/bin/env python3
import argparse
import hashlib
import json
import os
import pathlib
import platform
import re
import shlex
import shutil
import subprocess
import time


ROOT = pathlib.Path(__file__).resolve().parents[2]


def run(command, log=None, env=None):
    print(shlex.join(map(str, command)), flush=True)
    start = time.perf_counter()
    if log:
        with log.open("w") as output:
            result = subprocess.run(command, cwd=ROOT, env=env, stdout=output,
                                    stderr=subprocess.STDOUT)
        if result.returncode:
            print("\n".join(log.read_text().splitlines()[-25:]), flush=True)
            result.check_returncode()
    else:
        subprocess.run(command, cwd=ROOT, env=env, check=True)
    return time.perf_counter() - start


def capture(command):
    return subprocess.check_output(command, cwd=ROOT, text=True).strip()


def absolute(directory, value):
    return str((directory / value).resolve())


def cpu_configuration(text):
    fields = ["vendor_id", "cpu family", "model", "model name", "stepping", "flags"]
    rows = [line.split(":", 1) for line in text.splitlines() if ":" in line]
    return {key: sorted({value.strip() for name, value in rows if name.strip() == key})
            for key in fields} | {"logical_cpus": sum(name.strip() == "processor" for name, _ in rows)}


def configuration(database):
    units = {}
    for entry in json.loads(database.read_text()):
        name = pathlib.Path(entry["file"]).name
        if name not in {"sqlite3.c", "shell.c"}:
            continue
        if name in units:
            raise ValueError(f"multiple configurations for {name}")
        directory = pathlib.Path(entry["directory"])
        source = absolute(directory, entry["file"])
        arguments = entry.get("arguments") or shlex.split(entry["command"])
        flags = []
        iterator = iter(arguments[1:])
        for argument in iterator:
            if argument in {"-o", "-MF", "-MT", "-MQ"}:
                next(iterator)
            elif argument in {"-c", "-MD", "-MMD", "-MP", "-fPIC", "-fpic"}:
                continue
            elif argument in {"-I", "-isystem", "-iquote", "-idirafter", "-include", "-imacros"}:
                flags.extend([argument, absolute(directory, next(iterator))])
            elif argument.startswith("-I") and len(argument) > 2:
                flags.append("-I" + absolute(directory, argument[2:]))
            elif not argument.startswith("-") and absolute(directory, argument) == source:
                continue
            else:
                flags.append(argument)
        flags.extend(["-O2", "-g"])
        units[name] = {"source": source, "flags": flags}
    if set(units) != {"sqlite3.c", "shell.c"}:
        raise ValueError("compilation database must contain sqlite3.c and shell.c")
    return units


def build(args, directory):
    units = configuration(args.compile_commands)
    revision = capture(["git", "rev-parse", "HEAD"])
    (directory / "source.patch").write_text(capture(["git", "diff", "HEAD", "--", "crates/slate", "crates/slate-parser"]) + "\n")
    native = directory / "clang"
    generated = directory / "rust"
    native.mkdir(exist_ok=True)
    timings = {}
    timings["slate"] = run(["cargo", "build", "--release", "-p", "slate"], directory / "slate-build.log")
    commands = [{"directory": str(ROOT), "file": unit["source"],
                 "arguments": [args.clang, *unit["flags"], "-D__PIC__=2", "-D__pic__=2",
                               "-c", unit["source"]]} for unit in units.values()]
    database = directory / "compile_commands.json"
    database.write_text(json.dumps(commands, indent=2) + "\n")
    timings["translation"] = run([
        str(ROOT / "target/test-cache/release/slate"), "translate-project",
        "--compile-commands", str(database), str(args.corpus), str(generated),
    ], directory / "translation.log")
    core, shell = units["sqlite3.c"], units["shell.c"]
    timings["clang_core"] = run([
        args.clang, *core["flags"], "-fPIC", "-c", core["source"],
        "-o", str(native / "sqlite3.o"),
    ], directory / "clang-core.log")
    timings["clang_library"] = run([
        args.clang, "-shared", str(native / "sqlite3.o"), "-lm", "-lz", "-ldl", "-pthread",
        "-o", str(native / "libsqlite3.so"),
    ], directory / "clang-library.log")
    timings["clang_shell"] = run([
        args.clang, *shell["flags"], "-fPIC", shell["source"], str(native / "sqlite3.o"),
        "-lz", "-lreadline", "-lncurses", "-lm", "-ldl", "-pthread",
        "-o", str(native / "sqlite3"),
    ], directory / "clang-shell.log")
    manifest = generated / "Cargo.toml"
    manifest.write_text(manifest.read_text() + '\n[lib]\nname = "sqlite_generated"\ncrate-type = ["cdylib"]\n')
    main = (generated / "src/main.rs").read_text()
    features = "\n".join(re.findall(r"^#!\[feature\([^\n]+\)\]", main, re.MULTILINE))
    (generated / "src/lib.rs").write_text(features + '\n#[path = "sqlite3.rs"]\nmod __slate_unit_sqlite3;\n')
    cargo_env = dict(os.environ, CARGO_TARGET_DIR=str(generated / "target"))
    timings["rust_library"] = run([
        "cargo", "rustc", "--manifest-path", str(manifest), "--release", "--lib",
        "--", "-A", "warnings", "-l", "m", "-l", "z", "-l", "dl",
    ], directory / "rust-library.log", cargo_env)
    timings["rust_shell"] = run([
        "cargo", "rustc", "--manifest-path", str(manifest), "--release", "--bin", generated.name,
        "--", "-A", "warnings", "-l", "z", "-l", "readline", "-l", "ncurses", "-l", "m", "-l", "dl",
    ], directory / "rust-shell.log", cargo_env)
    shutil.copy2(generated / "target/release/libsqlite_generated.so", generated / "libsqlite3.so")
    shutil.copy2(generated / "target/release" / generated.name, generated / "sqlite3")
    source = (generated / "src/sqlite3.rs").read_text()
    vdbe = re.search(r'\bfn sqlite3VdbeExec\(', source)
    next_function = re.search(r'^.*\bfn \w+\(', source[vdbe.end():], re.MULTILINE) if vdbe else None
    body = source[vdbe.start():vdbe.end() + next_function.start()] if next_function else ""
    metrics = {"vdbe_states": len(re.findall(r"^\s*\d+ =>", body, re.MULTILINE)),
               "vdbe_dispatch_transfers": body.count("continue '__slate_dispatch"),
               "vdbe_rust_bytes": len(body.encode())}
    for name, path in [("clang", native), ("rust", generated)]:
        symbols = capture(["nm", "-S", "-C", "--defined-only", str(path / "libsqlite3.so")])
        symbol = next((line for line in symbols.splitlines()
                       if line.split()[-1].rsplit("::", 1)[-1] == "sqlite3VdbeExec"), None)
        metrics[name + "_vdbe_code_bytes"] = int(symbol.split()[1], 16) if symbol else None
        if symbol:
            address, size = [int(value, 16) for value in symbol.split()[:2]]
            run(["objdump", "-d", f"--start-address={address}", f"--stop-address={address + size}",
                 str(path / "libsqlite3.so")], path / "vdbe.asm")
    metadata = {
        "git_commit": revision,
        "clang": capture([args.clang, "--version"]), "rustc": capture(["rustc", "-Vv"]),
        "platform": platform.platform(), "cpu": cpu_configuration(pathlib.Path("/proc/cpuinfo").read_text()),
        "sqlite_version": (args.corpus / "VERSION").read_text().strip(),
        "sqlite_sha256": hashlib.sha256(pathlib.Path(core["source"]).read_bytes()).hexdigest(),
        "driver_sha256": hashlib.sha256((args.corpus / "test/speedtest1.c").read_bytes()).hexdigest(),
        "configuration": units, "build_seconds": timings, "metrics": metrics,
    }
    (directory / "build.json").write_text(json.dumps(metadata, indent=2) + "\n")


def compare(directory, baseline):
    builds = [json.loads((path / "build.json").read_text()) for path in [baseline, directory]]
    for key in ["sqlite_sha256", "driver_sha256", "configuration", "clang", "rustc", "platform", "cpu"]:
        if builds[0][key] != builds[1][key]:
            raise ValueError(f"baseline {key} differs; rebuild matching configurations")
    reports = [json.loads((path / "runtime/runtime.json").read_text()) for path in [baseline, directory]]
    for key in ["size", "mode", "runs"]:
        if reports[0][key] != reports[1][key]:
            raise ValueError(f"baseline benchmark {key} differs")
    comparison = {"baseline": str(baseline), "current": str(directory), "testsets": {},
                  "metrics": {"before": builds[0]["metrics"], "after": builds[1]["metrics"]}}
    for name, after in reports[1]["testsets"].items():
        if name not in reports[0]["testsets"]:
            raise ValueError(f"baseline has no {name} benchmark")
        before = reports[0]["testsets"][name]
        if before["samples"]["native"][0]["verification"] != after["samples"]["native"][0]["verification"]:
            raise ValueError(f"{name}: baseline verification differs")
        speedup = before["median_wall_seconds"]["generated"] / after["median_wall_seconds"]["generated"]
        comparison["testsets"][name] = {"generated_speedup": speedup,
                                        "before": before["median_wall_seconds"],
                                        "after": after["median_wall_seconds"]}
        print(f"{name}: generated {speedup:.2f}x faster than {baseline.name}")
    (directory / "comparison.json").write_text(json.dumps(comparison, indent=2) + "\n")


def main():
    parser = argparse.ArgumentParser(description="Build and compare Clang and Slate SQLite from the workspace root.")
    parser.add_argument("corpus", nargs="?", type=pathlib.Path, default=pathlib.Path.home() / "c-corpus/sqlite")
    parser.add_argument("--compile-commands", type=pathlib.Path)
    parser.add_argument("--output", type=pathlib.Path, default=ROOT / "target/sqlite-benchmark")
    parser.add_argument("--label", default="current")
    parser.add_argument("--clang", default="clang")
    parser.add_argument("--size", type=int, default=50)
    parser.add_argument("--runs", type=int, default=5)
    parser.add_argument("--testsets", nargs="+", default=["main", "cte", "json"])
    parser.add_argument("--skip-build", action="store_true")
    parser.add_argument("--build-only", action="store_true")
    parser.add_argument("--profile", action="store_true")
    parser.add_argument("--compare", help="compare against an earlier label in the same output directory")
    args = parser.parse_args()
    if pathlib.Path.cwd().resolve() != ROOT:
        parser.error(f"run from {ROOT}")
    if args.size < 1 or args.runs < 1:
        parser.error("size and runs must be positive")
    if not re.fullmatch(r"[A-Za-z0-9_-]+", args.label):
        parser.error("label must contain only letters, digits, underscores or hyphens")
    if args.compare and (not re.fullmatch(r"[A-Za-z0-9_-]+", args.compare) or args.compare == args.label):
        parser.error("compare must name a different valid label")
    args.corpus = args.corpus.expanduser().resolve()
    args.compile_commands = (args.compile_commands or args.corpus / "build-clang/compile_commands.json").expanduser().resolve()
    directory = args.output.expanduser().resolve() / args.label
    directory.mkdir(parents=True, exist_ok=True)
    try:
        if not args.skip_build:
            build(args, directory)
        if args.build_only:
            return
        native, generated = directory / "clang", directory / "rust"
        run(["python3", str(ROOT / "tools/sqlite_runtime_diff.py"), str(native / "sqlite3"),
             str(generated / "sqlite3"), "--native-library", str(native / "libsqlite3.so"),
             "--generated-library", str(generated / "libsqlite3.so")])
        run(["python3", str(ROOT / "tools/sqlite_benchmark.py"), str(args.corpus),
             str(native / "libsqlite3.so"), str(generated / "libsqlite3.so"), str(directory / "runtime"),
             "--size", str(args.size), "--runs", str(args.runs), "--clang", args.clang, "--testsets", *args.testsets])
        if args.profile:
            run(["python3", str(ROOT / "tools/sqlite_profile.py"), str(directory / "runtime/speedtest1-native"),
                 str(directory / "runtime/speedtest1-generated"), str(directory / "profile"),
                 "--size", str(args.size), "--testsets", *args.testsets, "--stats"])
        if args.compare:
            compare(directory, directory.parent / args.compare)
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        parser.exit(1, f"{error}\nbuild logs and results: {directory}\n")
    print(f"Results: {directory}")


if __name__ == "__main__":
    main()
