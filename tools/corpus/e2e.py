#!/usr/bin/env python3
import argparse
import collections
import concurrent.futures
import csv
import json
import os
import re
import shlex
import shutil
import socket
import statistics
import subprocess
import sys
import tempfile
import time
from collections.abc import Callable
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "crates/slate-parser/tools"))
import pp_diff

SLATE = ROOT / "target/test-cache/release/slate"
SETUP = ROOT / "crates/slate-parser/tools/c_corpus_setup.py"
STAGES = ("native", "barriers", "translate", "check", "build", "bench", "test")
COMPILERS = {"clang": "clang", "gcc": "gcc"}


@dataclass
class Recipe:
    target: str
    make_dir: str
    objects: list[str]
    archives: list[str]
    libs: str
    ldflags: list[str]
    host_includes: list[str] = field(default_factory=list)
    links: dict[str, str] = field(default_factory=dict)
    test: list[str] = field(default_factory=list)
    test_copy: list[str] = field(default_factory=list)
    test_build: list[list[str]] = field(default_factory=list)
    test_dir: str = ""
    test_patches: dict[str, list[tuple[str, str]]] = field(default_factory=dict)
    benchmark: Callable | None = None


@dataclass
class Differential:
    tool: str
    args: list[str] = field(default_factory=list)
    stdin: bool = False


@dataclass
class CMakeLibrary:
    library: str
    variants: list[str] = field(default_factory=list)
    tools: list[str] = field(default_factory=list)
    differential: list[Differential] = field(default_factory=list)
    inputs: list[str] = field(default_factory=list)
    host_includes: list[str] = field(default_factory=list)
    translate_tests: bool = False
    benchmark: Callable | None = None


RUST_NATIVE_LIBS = ["-lgcc_s", "-lutil", "-lrt", "-lpthread", "-lm", "-ldl", "-lc"]


def log(message):
    print(message, flush=True)


def run(command, output, cwd=ROOT, env=None, timeout=None, stdin=None):
    log(f"$ {shlex.join(map(str, command))}")
    start = time.perf_counter()
    with output.open("w") as handle:
        try:
            result = subprocess.run(command, cwd=cwd, env=env, stdin=stdin, stdout=handle,
                                    stderr=subprocess.STDOUT, timeout=timeout)
            code = result.returncode
        except subprocess.TimeoutExpired:
            code = "timeout"
    seconds = round(time.perf_counter() - start, 1)
    if code:
        log("\n".join(output.read_text(errors="replace").splitlines()[-30:]))
    return code, seconds


def make_variables(project_dir, recipe, compiler, names):
    output = subprocess.run(
        ["make", "-pn", f"CC={compiler}", recipe.target],
        cwd=project_dir / recipe.make_dir, capture_output=True, text=True,
    ).stdout
    values = {}
    for line in output.splitlines():
        match = re.match(r"^(\w+) :?= (.*)$", line)
        if match and match[1] in names and match[1] not in values:
            values[match[1]] = match[2].strip()
    missing = [name for name in names if name not in values]
    if missing:
        raise SystemExit(f"make -pn did not print {', '.join(missing)}")
    return values


def native_check(project_dir, recipe, variables):
    make_dir = project_dir / recipe.make_dir
    inputs = [*recipe.archives, *(word for word in variables[recipe.libs].split() if word.endswith(".a"))]
    missing = [path for path in inputs if not (make_dir / path).exists()]
    if not (make_dir / recipe.target).exists():
        missing.append(recipe.target)
    return missing


def free_port():
    with socket.socket() as probe:
        probe.bind(("127.0.0.1", 0))
        return probe.getsockname()[1]


def redis_sample(project_dir, binary, args):
    tools = project_dir / "src"
    port = str(free_port())
    with tempfile.TemporaryDirectory() as directory:
        server = subprocess.Popen([binary, "--port", port, "--save", "", "--appendonly", "no", "--dir", directory],
                                  stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        try:
            for _ in range(100):
                ping = subprocess.run([tools / "redis-cli", "-p", port, "ping"], capture_output=True, text=True)
                if ping.stdout.strip() == "PONG":
                    break
                time.sleep(0.1)
            else:
                raise RuntimeError(f"{binary} did not answer PING")
            result = subprocess.run(
                [tools / "redis-benchmark", "-p", port, "--csv", "-n", str(args.bench_requests),
                 "-c", "50", "-P", "16", "-t", args.bench_tests],
                capture_output=True, text=True, check=True,
            )
        finally:
            server.terminate()
            server.wait(timeout=30)
    rows = list(csv.reader(result.stdout.splitlines()))
    return {row[0]: float(row[1]) for row in rows[1:] if len(row) > 1}


def redis_benchmark(project_dir, binary, out, args):
    native = project_dir / "src/redis-server"
    samples = {"native": [], "translated": []}
    redis_sample(project_dir, native, args)
    redis_sample(project_dir, binary, args)
    for _ in range(args.bench_runs):
        samples["native"].append(redis_sample(project_dir, native, args))
        samples["translated"].append(redis_sample(project_dir, binary, args))
    tests = list(samples["native"][0])
    medians = {kind: {test: statistics.median(run[test] for run in runs) for test in tests}
               for kind, runs in samples.items()}
    ratios = {test: round(medians["translated"][test] / medians["native"][test], 3) for test in tests}
    (out / "bench.json").write_text(json.dumps({"unit": "requests/s", "samples": samples,
                                                "medians": medians, "ratios": ratios}, indent=1) + "\n")
    lines = ["| test | native rps | translated rps | translated / native |", "| --- | ---: | ---: | ---: |"]
    lines += [f"| {test} | {medians['native'][test]:.0f} | {medians['translated'][test]:.0f} | {ratios[test]:.3f} |"
              for test in tests]
    (out / "bench.md").write_text("\n".join(lines) + "\n")
    log("\n".join(lines))
    return {"geomean_ratio": round(statistics.geometric_mean(ratios.values()), 3), "report": "bench.md"}


LUA_BENCHMARKS = {
    "fib": "local function fib(n) if n < 2 then return n end return fib(n - 1) + fib(n - 2) end "
           "assert(fib(32) == 2178309)",
    "table": "local t = {} for i = 1, 3000000 do t[i] = i * 2 end local s = 0 "
             "for k, v in ipairs(t) do s = s + v end assert(s > 0)",
    "string": "local parts = {} for i = 1, 300000 do parts[#parts + 1] = string.format('%d:%x', i, i) end "
              "local text = table.concat(parts, ',') local n = 0 "
              "for word in text:gmatch('[^,]+') do n = n + #word:upper() end assert(n > 0)",
    "sort": "math.randomseed(42) local t = {} for i = 1, 1000000 do t[i] = math.random() end "
            "table.sort(t) for i = 2, #t do assert(t[i - 1] <= t[i]) end",
    "closure": "local acc = 0 for i = 1, 3000000 do local f = function(x) return x + i end acc = f(acc) % 1000003 end "
               "assert(acc >= 0)",
}


def lua_sample(binary, script):
    start = time.perf_counter()
    subprocess.run([binary, "-e", script], check=True, capture_output=True)
    return time.perf_counter() - start


def lua_benchmark(project_dir, binary, out, args):
    native = project_dir / "lua"
    samples = {kind: {name: [] for name in LUA_BENCHMARKS} for kind in ("native", "translated")}
    for _ in range(args.bench_runs):
        for name, script in LUA_BENCHMARKS.items():
            samples["native"][name].append(lua_sample(native, script))
            samples["translated"][name].append(lua_sample(binary, script))
    medians = {kind: {name: statistics.median(times) for name, times in runs.items()}
               for kind, runs in samples.items()}
    ratios = {name: round(medians["translated"][name] / medians["native"][name], 3) for name in LUA_BENCHMARKS}
    (out / "bench.json").write_text(json.dumps({"unit": "seconds", "samples": samples,
                                                "medians": medians, "ratios": ratios}, indent=1) + "\n")
    lines = ["| script | native s | translated s | translated / native |", "| --- | ---: | ---: | ---: |"]
    lines += [f"| {name} | {medians['native'][name]:.3f} | {medians['translated'][name]:.3f} | {ratios[name]:.3f} |"
              for name in LUA_BENCHMARKS]
    (out / "bench.md").write_text("\n".join(lines) + "\n")
    log("\n".join(lines))
    return {"geomean_ratio": round(statistics.geometric_mean(ratios.values()), 3), "report": "bench.md"}


def timed_filter(command, source, destination):
    with open(source, "rb") as stdin, open(destination, "wb") as stdout:
        start = time.perf_counter()
        subprocess.run(command, stdin=stdin, stdout=stdout, check=True)
        return time.perf_counter() - start


def zlib_benchmark(project_dir, build_dir, sandbox, out, args):
    work = out / "bench"
    work.mkdir(exist_ok=True)
    data = b"".join(path.read_bytes() for path in sorted(project_dir.glob("*.[ch]")) + [project_dir / "zlib.3.pdf"])
    plain = work / "input"
    plain.write_bytes(data * max(1, (64 << 20) // len(data)))
    binaries = {"native": build_dir / "test/minigzip", "translated": sandbox / "test/minigzip"}
    operations = {"compress": ([], plain, ".gz"), "compress -9": (["-9"], plain, ".9.gz"),
                  "decompress": (["-d"], work / "native.gz", ".out")}
    samples = {kind: {name: [] for name in operations} for kind in binaries}
    for _ in range(args.bench_runs):
        for name, (flags, source, suffix) in operations.items():
            for kind, binary in binaries.items():
                samples[kind][name].append(timed_filter([binary, *flags], source, work / f"{kind}{suffix}"))
    for suffix in (".gz", ".9.gz", ".out"):
        if (work / f"native{suffix}").read_bytes() != (work / f"translated{suffix}").read_bytes():
            raise RuntimeError(f"native{suffix} and translated{suffix} differ")
    if (work / "native.out").read_bytes() != plain.read_bytes():
        raise RuntimeError("decompressed output differs from the input")
    medians = {kind: {name: statistics.median(times) for name, times in runs.items()}
               for kind, runs in samples.items()}
    ratios = {name: round(medians["translated"][name] / medians["native"][name], 3) for name in operations}
    (out / "bench.json").write_text(json.dumps({"unit": "seconds", "input_bytes": plain.stat().st_size,
                                                "samples": samples, "medians": medians, "ratios": ratios},
                                               indent=1) + "\n")
    lines = ["| operation | native s | translated s | translated / native |", "| --- | ---: | ---: | ---: |"]
    lines += [f"| {name} | {medians['native'][name]:.3f} | {medians['translated'][name]:.3f} | {ratios[name]:.3f} |"
              for name in operations]
    (out / "bench.md").write_text("\n".join(lines) + "\n")
    log("\n".join(lines))
    shutil.rmtree(work)
    return {"geomean_ratio": round(statistics.geometric_mean(ratios.values()), 3), "report": "bench.md"}


def native_arguments(argv, entry):
    kept, skip = [], False
    for argument in argv:
        if skip:
            skip = False
        elif argument in {"-o", "-MF", "-MT", "-MQ"}:
            skip = True
        elif argument in {"-c", "-M", "-MM", "-MD", "-MMD", "-MP"} or argument == entry["file"]:
            continue
        else:
            kept.append(argument)
    return kept


def native_compile_seconds(units, out, jobs):
    objects = out / "native-objects"
    objects.mkdir(exist_ok=True)

    def compile_unit(unit):
        output = objects / (Path(unit["file"]).stem + ".o")
        result = subprocess.run([*unit["native"], "-c", unit["file"], "-o", output],
                                cwd=unit["directory"], capture_output=True, text=True)
        return unit["file"], result.returncode, result.stderr

    start = time.perf_counter()
    with concurrent.futures.ThreadPoolExecutor(jobs) as pool:
        failures = [(path, stderr) for path, code, stderr in pool.map(compile_unit, units) if code]
    seconds = round(time.perf_counter() - start, 1)
    shutil.rmtree(objects)
    return seconds, failures


def translation_unit(entry, source, recipe, flavor):
    argv = entry.get("arguments") or shlex.split(entry["command"])
    args = pp_diff.kept_args(argv, entry["directory"])
    if not any(arg.startswith("-std=") for arg in args):
        args.append(pp_diff.CLANG_DEFAULT_STANDARD)
    args += [f"-idirafter{directory}" for directory in recipe.host_includes]
    return {
        "directory": entry["directory"],
        "file": str(source),
        "arguments": [COMPILERS[flavor], *args, "-c", str(source)],
        "native": native_arguments(argv, entry),
    }


def translation_units(project_dir, recipe, database, variables, flavor):
    objects = [word for name in recipe.objects for word in variables[name].split()]
    wanted = {Path(obj).stem: obj for obj in objects}
    found = {}
    for entry in json.loads(database.read_text()):
        source = Path(pp_diff.absolute(entry["directory"], entry["file"])).resolve()
        if source.suffix != ".c" or source.stem not in wanted or source.stem in found or not source.exists():
            continue
        found[source.stem] = translation_unit(entry, source, recipe, flavor)
    missing = sorted(set(wanted) - set(found))
    if missing:
        raise SystemExit(f"no compile command for {', '.join(missing)}")
    return [found[Path(obj).stem] for obj in objects]


def ninja_tool(build_dir, *args):
    return subprocess.run(["ninja", "-C", build_dir, "-t", *args], capture_output=True, text=True,
                          check=True).stdout


def target_objects(build_dir, target):
    objects, reading = [], False
    target = (build_dir / target).resolve().relative_to(build_dir.resolve())
    for line in ninja_tool(build_dir, "query", str(target)).splitlines():
        line = line.strip()
        if line.startswith("input:"):
            reading = True
        elif line.startswith("outputs:"):
            break
        elif reading and line.endswith(".o") and not line.startswith("|"):
            objects.append((build_dir / line).resolve())
    if not objects:
        raise SystemExit(f"ninja -t query {target} listed no objects")
    return objects


def link_command(build_dir, executable):
    segments, segment = [], []
    for token in shlex.split(ninja_tool(build_dir, "commands", executable).strip().splitlines()[-1]):
        if token == "&&":
            segments.append(segment)
            segment = []
        else:
            segment.append(token)
    segments.append(segment)
    return next(segment for segment in segments if "-o" in segment)


def units_for_objects(objects, recipe, database, flavor):
    found = {}
    for entry in json.loads(database.read_text()):
        output = entry.get("output")
        if not output:
            continue
        output = Path(pp_diff.absolute(entry["directory"], output)).resolve()
        source = Path(pp_diff.absolute(entry["directory"], entry["file"])).resolve()
        if output in objects and source.suffix == ".c":
            found[output] = translation_unit(entry, source, recipe, flavor)
    missing = [str(obj) for obj in objects if obj not in found]
    if missing:
        raise SystemExit(f"no compile command for {', '.join(missing)}")
    return [found[obj] for obj in objects]


def library_units(build_dir, recipe, database, flavor):
    return units_for_objects(target_objects(build_dir, recipe.library), recipe, database, flavor)


def executable_units(build_dir, executable, recipe, database, flavor, shared_library):
    objects = target_objects(build_dir, executable)
    command = link_command(build_dir, executable)
    library = (build_dir / recipe.library).resolve()
    libraries = []
    for token in command[1:]:
        path = (build_dir / token).resolve()
        if path == library:
            libraries += [str(shared_library), f"-Wl,-rpath,{shared_library.parent}"]
        elif path.suffix in {".a", ".so"} or ".so." in path.name:
            if path.is_relative_to(build_dir.resolve()) and path.exists():
                objects += target_objects(build_dir, path)
        elif token.startswith("-l"):
            libraries.append(token)
    return units_for_objects(objects, recipe, database, flavor), libraries


def ctest_cases(build_dir):
    listing = subprocess.run(["ctest", "--show-only=json-v1"], cwd=build_dir, capture_output=True, text=True,
                             check=True).stdout
    cases = []
    for test in json.loads(listing)["tests"]:
        properties = {item["name"]: item["value"] for item in test.get("properties", [])}
        cases.append({"name": test["name"], "command": test["command"],
                      "cwd": properties.get("WORKING_DIRECTORY", str(build_dir))})
    return cases


def relink(build_dir, executable, libraries, archive, destination):
    libraries = {(build_dir / library).resolve() for library in libraries}
    command = link_command(build_dir, executable)
    linked, index = [], 0
    while index < len(command):
        token = command[index]
        if token == "-Xlinker" and command[index + 1].startswith("--dependency-file"):
            index += 2
            continue
        if token == "-o":
            output = destination / command[index + 1]
            output.parent.mkdir(parents=True, exist_ok=True)
            linked += ["-o", str(output)]
            index += 2
            continue
        linked.append(str(archive) if (build_dir / token).resolve() in libraries else token)
        index += 1
    result = subprocess.run([*linked, *RUST_NATIVE_LIBS], cwd=build_dir, capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(f"relinking {executable} failed:\n{result.stderr[-2000:]}")
    return output


def translate_executable(project_dir, build_dir, executable, recipe, database, flavor, destination, profile,
                         shared_library):
    units, libraries = executable_units(build_dir, executable, recipe, database, flavor, shared_library)
    work = destination / "translated" / executable.replace("/", "_")
    if work.exists():
        shutil.rmtree(work)
    work.mkdir(parents=True)
    commands = work / "compile_commands.json"
    commands.write_text(json.dumps([{key: value for key, value in unit.items() if key != "native"}
                                    for unit in units], indent=1) + "\n")
    crate = work / "crate"
    code, _ = run([SLATE, "translate-project", "--compile-commands", commands, project_dir, crate],
                  work / "translate.log")
    if code:
        raise RuntimeError(f"translating {executable} failed; see {work / 'translate.log'}")
    target_dir = destination / "translated" / "cargo-target"
    flags = " ".join(f"-C link-arg={library}" for library in libraries)
    env = dict(os.environ, CARGO_TARGET_DIR=str(target_dir),
               RUSTFLAGS=" ".join(filter(None, [os.environ.get("RUSTFLAGS"), "-A warnings", flags])))
    code, _ = run(["cargo", "build", "--profile", profile], work / "build.log", cwd=crate, env=env)
    if code:
        raise RuntimeError(f"building {executable} failed; see {work / 'build.log'}")
    package = re.search(r'^name = "([^"]+)"', (crate / "Cargo.toml").read_text(), re.MULTILINE)[1]
    binary = destination / executable
    binary.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(target_dir / ("debug" if profile == "dev" else profile) / package, binary)
    return binary


def run_case(command, cwd, stdin=None, timeout=300, executable=None):
    start = time.perf_counter()
    try:
        with open(stdin, "rb") if stdin else open(os.devnull, "rb") as handle:
            result = subprocess.run(command, cwd=cwd, stdin=handle, capture_output=True, timeout=timeout,
                                    executable=executable)
        code, stdout = result.returncode, result.stdout
    except subprocess.TimeoutExpired:
        code, stdout = "timeout", b""
    return code, stdout, time.perf_counter() - start


def library_tests(project_dir, build_dir, recipe, archive, database, args, out):
    sandbox = out / "test-tree"
    if sandbox.exists():
        shutil.rmtree(sandbox)
    cases, skipped = [], []
    for case in ctest_cases(build_dir):
        in_build = Path(case["command"][0]).resolve().is_relative_to(build_dir.resolve())
        (cases if in_build else skipped).append(case)
    executables = {str(Path(case["command"][0]).resolve().relative_to(build_dir.resolve())) for case in cases}
    executables |= set(recipe.tools)
    if recipe.translate_tests:
        shared_library = archive.with_suffix(".so")
        relinked = {name: translate_executable(project_dir, build_dir, name, recipe, database, args.mode,
                                               sandbox, args.profile, shared_library)
                    for name in sorted(executables)}
    else:
        relinked = {name: relink(build_dir, name, [recipe.library, *recipe.variants], archive, sandbox)
                    for name in sorted(executables)}
    rows, failures, seconds = [], 0, {"native": 0.0, "translated": 0.0}
    for case in cases:
        name = str(Path(case["command"][0]).resolve().relative_to(build_dir.resolve()))
        native = run_case(case["command"], case["cwd"])
        translated = run_case([str(relinked[name]), *case["command"][1:]], case["cwd"])
        seconds["native"] += native[2]
        seconds["translated"] += translated[2]
        result = "FAIL" if translated[0] != 0 else "ok" if translated[1] == native[1] else "STDOUT DIFFERS"
        failures += result != "ok"
        rows.append(f"| ctest {case['name']} | {native[0]} | {translated[0]} | {result} |")
    rows += [f"| ctest {case['name']} | | | skipped: runs {Path(case['command'][0]).name} |" for case in skipped]
    inputs = sorted(path for pattern in recipe.inputs for path in project_dir.glob(pattern))
    mismatches = 0
    for differential in recipe.differential:
        for path in inputs:
            args = [arg.replace("{input}", str(path)) for arg in differential.args]
            stdin = path if differential.stdin else None
            command = [differential.tool, *args]
            native = run_case(command, build_dir, stdin, 60, build_dir / differential.tool)
            translated = run_case(command, build_dir, stdin, 60, relinked[differential.tool])
            seconds["native"] += native[2]
            seconds["translated"] += translated[2]
            same = native[:2] == translated[:2]
            mismatches += not same
            rows.append(f"| {differential.tool} {os.path.relpath(path, project_dir)} | {native[0]} | "
                        f"{translated[0]} | {'ok' if same else 'MISMATCH'} |")
    lines = ["| case | native exit | translated exit | result |", "| --- | --- | --- | --- |", *rows]
    (out / "test.md").write_text("\n".join(lines) + "\n")
    log("\n".join(lines))
    return {"ctest": len(cases), "ctest_failures": failures, "ctest_skipped": len(skipped),
            "differential": len(rows) - len(cases) - len(skipped), "mismatches": mismatches, "report": "test.md",
            "native_seconds": round(seconds["native"], 2), "seconds": round(seconds["translated"], 2),
            "test_over_native": round(seconds["translated"] / seconds["native"], 2) if seconds["native"] else None}


def barrier_rows(entry):
    args = entry["arguments"][1:-2]
    result = subprocess.run([SLATE, "lowering-barriers", *args, entry["file"]],
                            cwd=entry["directory"], capture_output=True, text=True)
    rows = []
    for line in result.stdout.splitlines():
        parts = line.split("\t")
        if len(parts) == 2 and parts[1] == "ok":
            rows.append({"scope": parts[0], "site": "", "kind": "ok", "detail": ""})
        elif len(parts) >= 4:
            rows.append({"scope": parts[0], "site": parts[1], "kind": parts[2], "detail": parts[3]})
    if result.returncode and not rows:
        error = result.stderr.strip().splitlines()
        rows.append({"scope": "<tu>", "site": "", "kind": "error",
                     "detail": error[-1] if error else f"exit {result.returncode}"})
    return entry["file"], rows


def barrier_report(units, project_dir, out, jobs):
    with concurrent.futures.ThreadPoolExecutor(jobs) as pool:
        results = dict(pool.map(barrier_rows, units))
    rows = [{"tu": os.path.relpath(tu, project_dir), **row} for tu, tu_rows in results.items() for row in tu_rows]
    (out / "barriers.json").write_text(json.dumps(rows, indent=1) + "\n")
    blocked = [row for row in rows if row["kind"] != "ok"]
    functions = sum(row["scope"] not in ("<module>", "<tu>") and not row["scope"].startswith("<declaration ")
                    for row in rows)
    kinds = collections.defaultdict(lambda: [0, set()])
    for row in blocked:
        key = f"{row['kind']}: {re.sub(r'%[0-9]+', '%N', row['detail'])[:120]}"
        kinds[key][0] += 1
        kinds[key][1].add(row["tu"])
    lines = [
        "# Lowering barriers",
        "",
        f"- TUs: {len(results)}, with barriers: {len({row['tu'] for row in blocked})}",
        f"- functions and globals: {len(rows)} rows, {functions} functions, {len(blocked)} blocked",
        "",
        "| count | TUs | barrier |",
        "| ---: | ---: | --- |",
        *(f"| {count} | {len(tus)} | `{key}` |"
          for key, (count, tus) in sorted(kinds.items(), key=lambda item: -item[1][0])),
        "",
        "| TU | scope | site | barrier |",
        "| --- | --- | --- | --- |",
        *(f"| {row['tu']} | {row['scope']} | {row['site']} | `{row['detail']}` |" for row in blocked),
    ]
    (out / "barriers.md").write_text("\n".join(lines) + "\n")
    return len(blocked)


def link_flags(project_dir, recipe, variables):
    make_dir = project_dir / recipe.make_dir
    flags = [str((make_dir / archive).resolve()) for archive in recipe.archives]
    for word in variables[recipe.libs].split():
        flags.append(str((make_dir / word).resolve()) if word.endswith(".a") else word)
    flags += recipe.ldflags
    return " ".join(f"-C link-arg={flag}" for flag in flags)


def test_sandbox(project_dir, recipe, binary, out, jobs, name="test-tree"):
    for index, command in enumerate(recipe.test_build):
        code, _ = run([*command, f"-j{jobs}"], out / f"test-build-{index}.log", cwd=project_dir)
        if code:
            raise RuntimeError(f"{shlex.join(command)} failed; see test-build-{index}.log")
    sandbox = out / name
    if sandbox.exists():
        shutil.rmtree(sandbox)
    sandbox.mkdir()
    for name in recipe.test_copy:
        source = project_dir / name
        if source.is_dir():
            shutil.copytree(source, sandbox / name, symlinks=True,
                            ignore=shutil.ignore_patterns("tmp", "*.o", "*.xo"))
        elif source.exists():
            shutil.copy2(source, sandbox / name)
    for name, replacements in recipe.test_patches.items():
        path = sandbox / name
        text = path.read_text()
        for old, new in replacements:
            if old not in text:
                raise RuntimeError(f"test patch for {name} no longer applies: {old!r}")
            text = text.replace(old, new)
        path.write_text(text)
    for link, kind in recipe.links.items():
        path = sandbox / link
        path.parent.mkdir(parents=True, exist_ok=True)
        path.symlink_to(binary if kind == "target" and binary else (project_dir / link).resolve())
    return sandbox


RECIPES = {
    "redis": Recipe(
        target="redis-server",
        make_dir="src",
        objects=["REDIS_SERVER_OBJ", "REDIS_VEC_SETS_OBJ"],
        archives=[
            "../deps/hiredis/libhiredis.a",
            "../deps/lua/src/liblua.a",
            "../deps/hdr_histogram/libhdrhistogram.a",
            "../deps/fpconv/libfpconv.a",
            "../deps/xxhash/libxxhash.a",
            "../deps/tre/libtre.a",
        ],
        libs="FINAL_LIBS",
        ldflags=["-rdynamic"],
        host_includes=["/usr/include"],
        links={
            "src/redis-server": "target",
            "src/redis-check-rdb": "target",
            "src/redis-check-aof": "target",
            "src/redis-cli": "native",
            "src/redis-benchmark": "native",
        },
        test=["./runtest", "--clients", "8"],
        test_copy=["runtest", "tests", "redis.conf", "sentinel.conf", "utils"],
        test_build=[["make", "-C", "tests/modules"]],
        benchmark=redis_benchmark,
    ),
    "lua": Recipe(
        target="lua",
        make_dir=".",
        objects=["LUA_O", "CORE_O", "AUX_O", "LIB_O"],
        archives=[],
        libs="LIBS",
        ldflags=["-Wl,-E", "-ldl"],
        links={"lua": "target"},
        test=["../lua", "all.lua"],
        test_copy=["testes"],
        test_build=[["make", "-C", "testes/libs"]],
        test_dir="testes",
        test_patches={"testes/files.lua": [("    {\"sh -c 'kill -s HUP $$'\", \"exit\"},\n", "")]},
        benchmark=lua_benchmark,
    ),
    "cJSON": CMakeLibrary(library="libcjson.so", translate_tests=True),
    "zlib": CMakeLibrary(
        library="libz.a",
        variants=["libz.so"],
        tools=["test/minigzip"],
        differential=[Differential("test/minigzip", [*level, "-c", "{input}"])
                      for level in ([], ["-1"], ["-9"], ["-h"], ["-r"], ["-f"])],
        inputs=["*.c", "*.h", "ChangeLog", "FAQ", "doc/*", "zlib.3.pdf"],
        benchmark=zlib_benchmark,
    ),
    "libyaml": CMakeLibrary(
        library="libyaml.a",
        tools=["run-scanner", "run-parser", "run-loader", "run-emitter", "run-dumper", "run-parser-test-suite",
               "example-reformatter", "example-reformatter-alt", "example-deconstructor",
               "example-deconstructor-alt"],
        differential=[
            *(Differential(tool, ["{input}"]) for tool in
              ["run-scanner", "run-parser", "run-loader", "run-emitter", "run-dumper", "run-parser-test-suite"]),
            *(Differential(tool, stdin=True) for tool in
              ["example-reformatter", "example-reformatter-alt", "example-deconstructor",
               "example-deconstructor-alt"]),
        ],
        inputs=["examples/*.yaml", "regression-inputs/*"],
    ),
}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("project", choices=sorted(RECIPES))
    parser.add_argument("--mode", choices=sorted(COMPILERS), default="clang")
    parser.add_argument("--corpus", type=Path, default=Path(os.environ.get("SLATE_CORPUS", Path.home() / "c-corpus")))
    parser.add_argument("--out", type=Path)
    parser.add_argument("--setup", action="store_true", help="rebuild build-<mode> with c_corpus_setup.py first")
    parser.add_argument("--until", choices=STAGES, default=STAGES[-1])
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 8)
    parser.add_argument("--profile", default="release")
    parser.add_argument("--test-timeout", type=int, default=7200)
    parser.add_argument("--bench-runs", type=int, default=3, help="alternating native/translated samples; 0 skips")
    parser.add_argument("--bench-requests", type=int, default=200000)
    parser.add_argument("--bench-tests", default="set,get,incr,lpush,rpush,lpop,rpop,sadd,hset,spop,zadd,zpopmin,lrange_100,mset")
    parser.add_argument("--no-native-test", dest="native_test", action="store_false",
                        help="skip timing the test command against the native binary")
    parser.add_argument("test_args", nargs="*", help="extra arguments for the project's test command")
    args = parser.parse_args()

    recipe = RECIPES[args.project]
    project_dir = args.corpus / args.project
    out = (args.out or ROOT / "target/corpus" / args.project / args.mode).resolve()
    out.mkdir(parents=True, exist_ok=True)
    summary = {"project": args.project, "mode": args.mode, "stages": {}}

    def finish(stage, status, **details):
        summary["stages"][stage] = {"status": status, **details}
        (out / "summary.json").write_text(json.dumps(summary, indent=2) + "\n")
        log(f"== {stage}: {status} {json.dumps(details) if details else ''}")
        return status == "ok" and STAGES.index(stage) < STAGES.index(args.until)

    library = isinstance(recipe, CMakeLibrary)
    code, seconds = run(["cargo", "build", "--release", "-p", "slate-c2rust"], out / "slate-build.log")
    if code:
        raise SystemExit("slate build failed")
    if args.setup:
        code, seconds = run([sys.executable, SETUP, args.project, "--flavor", args.mode],
                            out / "setup.log", timeout=3600)
        if code:
            finish("native", "failed", step="setup", log="setup.log", seconds=seconds)
            return 1
    database = project_dir / f"build-{args.mode}" / "compile_commands.json"
    if not database.exists():
        finish("native", "failed", error=f"missing {database}; run with --setup")
        return 1
    build_dir = database.parent
    if library:
        variables = None
        missing = [] if (build_dir / recipe.library).exists() else [recipe.library]
        units = library_units(build_dir, recipe, database, args.mode)
    else:
        variables = make_variables(project_dir, recipe, COMPILERS[args.mode], [*recipe.objects, recipe.libs])
        missing = native_check(project_dir, recipe, variables)
        units = translation_units(project_dir, recipe, database, variables, args.mode)
    commands = out / "compile_commands.json"
    commands.write_text(json.dumps([{key: value for key, value in unit.items() if key != "native"}
                                    for unit in units], indent=1) + "\n")
    if not finish("native", "failed" if missing else "ok", missing=missing, units=len(units)):
        return 1 if missing else 0

    blocked = barrier_report(units, project_dir, out, args.jobs)
    if not finish("barriers", "ok", blocked=blocked, report="barriers.md"):
        return 0

    crate = out / (args.project if library else recipe.target)
    if crate.exists():
        shutil.rmtree(crate)
    crate_type = ["--crate-type", "staticlib,cdylib" if recipe.translate_tests else "staticlib"] if library else []
    code, seconds = run([SLATE, "translate-project", *crate_type, "--compile-commands", commands, project_dir,
                         crate], out / "translate.log")
    native_seconds, native_failures = native_compile_seconds(units, out, args.jobs)
    timing = {"translate_seconds": seconds, "native_compile_seconds": native_seconds,
              "translate_over_compile": round(seconds / native_seconds, 2) if native_seconds else None,
              "native_compile_failures": [path for path, _ in native_failures]}
    if not finish("translate", "failed" if code else "ok", log="translate.log", **timing):
        return 1 if code else 0

    env = dict(os.environ, CARGO_TARGET_DIR=str(out / "cargo-target"))
    code, _ = run(["cargo", "check", "--profile", args.profile, "--message-format=short"],
                  out / "check.log", cwd=crate, env=env)
    errors = sum(line.startswith("error") for line in (out / "check.log").read_text(errors="replace").splitlines())
    if not finish("check", "failed" if code else "ok", errors=errors, log="check.log"):
        return 1 if code else 0

    env["RUSTFLAGS"] = " ".join(filter(None, [os.environ.get("RUSTFLAGS"), "-A warnings",
                                              None if library else link_flags(project_dir, recipe, variables)]))
    code, _ = run(["cargo", "build", "--profile", args.profile], out / "build.log", cwd=crate, env=env)
    profile_dir = "debug" if args.profile == "dev" else args.profile
    package = re.search(r'^name = "([^"]+)"', (crate / "Cargo.toml").read_text(), re.MULTILINE)[1]
    binary = out / "cargo-target" / profile_dir / package
    if library:
        binary = next((out / "cargo-target" / profile_dir).glob("*.a"), binary)
    if not finish("build", "failed" if code else "ok", log="build.log", binary=str(binary)):
        return 1 if code else 0

    if library:
        try:
            results = library_tests(project_dir, build_dir, recipe, binary, database, args, out)
        except (RuntimeError, subprocess.SubprocessError) as error:
            finish("test", "failed", error=str(error))
            return 1
        failed = results["ctest_failures"] or results["mismatches"]
        finish("test", "failed" if failed else "ok", **results)
        if failed or not recipe.benchmark or not args.bench_runs:
            return 1 if failed else 0
        try:
            bench = recipe.benchmark(project_dir, build_dir, out / "test-tree", out, args)
        except (RuntimeError, subprocess.SubprocessError) as error:
            finish("bench", "failed", error=str(error))
            return 1
        finish("bench", "ok", **bench)
        return 0

    if recipe.benchmark and args.bench_runs:
        try:
            bench = recipe.benchmark(project_dir, binary, out, args)
        except (RuntimeError, subprocess.SubprocessError) as error:
            finish("bench", "failed", error=str(error))
            return 1
        if not finish("bench", "ok", **bench):
            return 0

    try:
        sandbox = test_sandbox(project_dir, recipe, binary, out, args.jobs)
    except RuntimeError as error:
        finish("test", "failed", error=str(error))
        return 1
    code, seconds = run([*recipe.test, *args.test_args], out / "test.log", cwd=sandbox / recipe.test_dir,
                        timeout=args.test_timeout, stdin=subprocess.PIPE)
    timing = {}
    if args.native_test:
        native_sandbox = test_sandbox(project_dir, recipe, None, out, args.jobs, "test-tree-native")
        native_code, native_seconds = run([*recipe.test, *args.test_args], out / "test-native.log",
                                          cwd=native_sandbox / recipe.test_dir, timeout=args.test_timeout,
                                          stdin=subprocess.PIPE)
        timing = {"native_seconds": native_seconds, "native_exit": native_code,
                  "native_log": "test-native.log",
                  "test_over_native": round(seconds / native_seconds, 2) if native_seconds else None}
    finish("test", "failed" if code else "ok", seconds=seconds, log="test.log", exit=code, **timing)
    return 1 if code else 0


if __name__ == "__main__":
    sys.exit(main())
