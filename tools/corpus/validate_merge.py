#!/usr/bin/env python3
import argparse
import concurrent.futures
import json
import os
import random
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SLATE = ROOT / "target/test-cache/release/slate"
MATRIX = ROOT / "tools/corpus/matrices/zlib.json"
PROJECT = Path.home() / "c-corpus/zlib"
MSVC_ROOT = Path(os.environ.get("MSVC_WINE_ROOT", Path.home() / ".local/share/msvc-wine/opt/msvc"))
PINNED = "nightly-2026-10-25"
DRIVERS = {"example": ["test/example.c"], "minigzip": ["test/minigzip.c"], "infcover": ["test/infcover.c"],
           "zpipe": ["examples/zpipe.c"]}
LEVELS = [[], ["-1"], ["-9"], ["-h"], ["-r"], ["-f"]]
INPUTS = ["ChangeLog", "zlib.3.pdf", "inflate.c"]
RUN_TIMEOUT = 300


def arm_sysroot():
    output = subprocess.run(["arm-linux-gnueabihf-gcc", "-print-sysroot"], capture_output=True, text=True, check=True)
    return Path(output.stdout.strip()).resolve()


def linux_toolchains():
    sysroot = arm_sysroot()
    return {
        "linux-x86_64": {"cc": ["clang"], "linker": "clang", "runner": []},
        "linux-i686": {"cc": ["clang", "--target=i686-linux-gnu", "-m32"], "linker": "clang",
                       "rustflags": "-C link-arg=--target=i686-linux-gnu -C link-arg=-m32", "runner": []},
        "linux-aarch64": {"cc": ["clang", "--target=aarch64-linux-gnu", "--sysroot=/usr/aarch64-linux-gnu",
                                 "--gcc-toolchain=/usr"],
                          "linker": "aarch64-linux-gnu-gcc",
                          "runner": ["qemu-aarch64-static", "-L", "/usr/aarch64-linux-gnu"]},
        "linux-arm32": {"cc": ["clang", "--target=arm-none-linux-gnueabihf", "-march=armv7-a", f"--sysroot={sysroot}",
                               f"--gcc-toolchain={sysroot.parents[1]}"],
                        "linker": "arm-linux-gnueabihf-gcc", "runner": ["qemu-arm-static", "-L", str(sysroot)]},
    }


def newest(path):
    return sorted(path.iterdir())[-1]


def msvc_env(msvc_arch):
    tools = newest(MSVC_ROOT / "vc/tools/msvc")
    kits_lib = newest(MSVC_ROOT / "kits/10/lib")
    kits_include = newest(MSVC_ROOT / "kits/10/include")
    return {
        "LIB": ";".join(str(p) for p in [tools / "lib" / msvc_arch, kits_lib / "um" / msvc_arch,
                                         kits_lib / "ucrt" / msvc_arch]),
        "INCLUDE": ";".join(str(p) for p in [tools / "include", kits_include / "ucrt", kits_include / "um",
                                             kits_include / "shared"]),
        "WINEDEBUG": "-all",
    }


def windows_toolchains():
    return {
        "windows-x86_64": {"msvc_arch": "x64", "runner": ["wine"]},
        "windows-i686": {"msvc_arch": "x86", "runner": ["wine"]},
    }


def feature_sets(names):
    return [[], *[[name] for name in names], ["lit-mem", "no-gzip"], ["fastest", "no-gzcompress"], list(names)]


def label(features):
    return ("+".join(features) or "default") if len(features) < 4 else "all"


def run(command, **kwargs):
    return subprocess.run(command, capture_output=True, timeout=kwargs.pop("timeout", 1800), **kwargs)


def crate_name(crate):
    return re.search(r'^name = "(.*)"$', (crate / "Cargo.toml").read_text(), re.M)[1]


def cargo_env(target, toolchain, cargo_target_dir):
    env = dict(os.environ, RUSTUP_TOOLCHAIN=PINNED, CARGO_TARGET_DIR=str(cargo_target_dir))
    triple = target["triple"].replace("-", "_")
    upper = triple.upper()
    if "linker" in toolchain:
        env[f"CARGO_TARGET_{upper}_LINKER"] = toolchain["linker"]
    if "rustflags" in toolchain:
        env[f"CARGO_TARGET_{upper}_RUSTFLAGS"] = toolchain["rustflags"]
    if "msvc_arch" in toolchain:
        env.update(msvc_env(toolchain["msvc_arch"]))
        env[f"CARGO_TARGET_{upper}_LINKER"] = "lld-link"
    return env


def build_staticlib(crate, target, toolchain, features, out):
    env = cargo_env(target, toolchain, out / "cargo")
    command = ["cargo", "rustc", "--release", "--lib", "--crate-type", "staticlib", "--target", target["triple"],
               "--no-default-features", "--features", ",".join(features), "--", "--print", "native-static-libs",
               "-Awarnings"]
    result = run(command, cwd=crate, env=env, text=True)
    if result.returncode:
        return None, None, result.stderr[-4000:]
    native = re.search(r"native-static-libs: (.*)", result.stderr)
    libs = native[1].split() if native else []
    name = crate_name(crate)
    built = out / "cargo" / target["triple"] / "release"
    path = next(p for p in [built / f"lib{name}.a", built / f"{name}.lib"] if p.exists())
    return path, libs, ""


def defines_for(matrix, features):
    by_name = {feature["name"]: feature["defines"] for feature in matrix["features"]}
    return [define for name in features for define in by_name[name]]


def compile_args(matrix, target, features):
    return [*target.get("args", []), *matrix["args"], *[f"-D{define}" for define in defines_for(matrix, features)]]


def absolute_includes(args):
    return [f"-I{PROJECT / arg[2:]}" if arg.startswith("-I") else arg for arg in args]


def link_unix(toolchain, args, sources, extra, output):
    command = [*toolchain["cc"], "-O2", "-w", *absolute_includes(args), *sources, *extra, "-lm", "-o", str(output)]
    return run(command, text=True)


def link_msvc(toolchain, args, sources, extra, output, native):
    cl = MSVC_ROOT / "bin" / toolchain["msvc_arch"] / "cl"
    env = dict(os.environ, **msvc_env(toolchain["msvc_arch"]))
    work = output.parent / f"{output.stem}.obj"
    work.mkdir(parents=True, exist_ok=True)
    return run([str(cl), "/nologo", "/O2", "/w", "/MD", "/Zc:preprocessor", *msvc_flags(args),
                *[str(source) for source in sources], f"/Fe:{wine_path(output)}", f"/Fo:{wine_path(work)}\\",
                *[wine_path(path) for path in extra], "/link", *native], cwd=work, env=env, text=True)


def wine_path(path):
    return "z:" + str(path).replace("/", "\\")


def msvc_flags(args):
    flags = [arg.replace("-I", "/I", 1).replace("-D", "/D", 1) for arg in absolute_includes(args)]
    return [*flags, "/std:clatest"] if "-DNO_vsnprintf" in args else flags


def compile_library(toolchain, args, sources, directory):
    directory.mkdir(parents=True, exist_ok=True)
    if "msvc_arch" in toolchain:
        cl = MSVC_ROOT / "bin" / toolchain["msvc_arch"] / "cl"
        env = dict(os.environ, **msvc_env(toolchain["msvc_arch"]))
        result = run([str(cl), "/nologo", "/c", "/O2", "/w", "/MD", "/Zc:preprocessor", *msvc_flags(args),
                      *[str(source) for source in sources], f"/Fo:{directory}/"], cwd=directory, env=env, text=True)
        return result, sorted(directory.glob("*.obj"))
    result = run([*toolchain["cc"], "-c", "-O2", "-w", *absolute_includes(args), *[str(s) for s in sources]],
                 cwd=directory, text=True)
    return result, sorted(directory.glob("*.o"))


def link(toolchain, args, sources, extra, output, native):
    if "msvc_arch" in toolchain:
        return link_msvc(toolchain, args, sources, extra, output, native)
    return link_unix(toolchain, args, sources, extra, output)


def make_inputs(directory):
    directory.mkdir(parents=True, exist_ok=True)
    names = []
    for name in INPUTS:
        shutil.copy(PROJECT / name, directory / name)
        names.append(name)
    rng = random.Random(0x5a7e)
    (directory / "empty").write_bytes(b"")
    (directory / "noise").write_bytes(rng.randbytes(1 << 18))
    (directory / "runs").write_bytes(b"".join(bytes([rng.randrange(4)]) * rng.randrange(1, 400) for _ in range(4000)))
    return [*names, "empty", "noise", "runs"]


def execute(runner, binary, args, cwd, env, stdin=None):
    try:
        result = run([*runner, str(binary), *args], cwd=cwd, env=env, timeout=RUN_TIMEOUT, input=stdin)
    except subprocess.TimeoutExpired:
        return {"code": "timeout", "stdout": b""}
    return {"code": result.returncode, "stdout": result.stdout, "stderr": result.stderr}


def run_env(toolchain):
    env = dict(os.environ)
    if "msvc_arch" in toolchain:
        env["WINEDEBUG"] = "-all"
    return env


def compare_programs(toolchain, binaries, inputs, work):
    env = run_env(toolchain)
    runner = toolchain["runner"]
    outcome = {"checks": 0, "failures": [], "c_failures": []}

    def check(name, args, rust_dir, c_dir, stdin=None):
        rust = execute(runner, binaries["rust"][name], args, rust_dir, env, stdin)
        native = execute(runner, binaries["c"][name], args, c_dir, env, stdin)
        outcome["checks"] += 1
        if native["code"] != 0:
            outcome["c_failures"].append(f"{name} {' '.join(args)}: C exit {native['code']}")
        if rust["code"] != native["code"] or rust["stdout"] != native["stdout"]:
            outcome["failures"].append(f"{name} {' '.join(args)}: exit {rust['code']} vs C {native['code']}, "
                                       f"stdout {len(rust['stdout'])} vs {len(native['stdout'])} bytes, "
                                       f"stderr {rust.get('stderr', b'')[-200:]!r}")
        return rust, native

    for program in ["example", "infcover"]:
        if program in binaries["rust"] and program in binaries["c"]:
            rust_dir = work / f"{program}-cwd-rust"
            c_dir = work / f"{program}-cwd-c"
            rust_dir.mkdir(exist_ok=True)
            c_dir.mkdir(exist_ok=True)
            check(program, [], rust_dir, c_dir)

    data = work / "data"
    for level in LEVELS:
        for name in inputs:
            rust, native = check("minigzip", [*level, "-c", name], data, data)
            if rust["code"] != 0 or native["code"] != 0:
                continue
            for producer, stream in [("rust", rust["stdout"]), ("c", native["stdout"])]:
                compressed = f"{name}.{producer}.gz"
                (data / compressed).write_bytes(stream)
                check("minigzip", ["-d", "-c", compressed], data, data)
                (data / compressed).unlink()
    for name in inputs:
        plain = (data / name).read_bytes()
        rust, native = check("zpipe", [], data, data, plain)
        if rust["code"] != 0 or native["code"] != 0:
            continue
        check("zpipe", ["-d"], data, data, rust["stdout"])
        check("zpipe", ["-d"], data, data, native["stdout"])
    return outcome


def validate_run(crate, matrix, target, toolchain, features, out, control=False):
    work = out / label(features)
    work.mkdir(parents=True, exist_ok=True)
    report = {"target": target["name"], "features": label(features)}
    lib, native, error = build_staticlib(crate, target, toolchain, features, out)
    if lib is None:
        return {**report, "status": "rust build failed", "detail": error}
    kept = work / lib.name
    shutil.copy(lib, kept)
    args = compile_args(matrix, target, [*features, "fastest"] if control else features)
    sources = [PROJECT / source for source in matrix["sources"]]
    binaries = {"rust": {}, "c": {}}
    notes = []
    suffix = ".exe" if "msvc_arch" in toolchain else ""
    library, objects = compile_library(toolchain, args, sources, work / "c-objects")
    if library.returncode:
        return {**report, "status": "C library failed", "detail": library.stdout[-3000:] + library.stderr[-3000:]}
    for program, driver in DRIVERS.items():
        driver_paths = [PROJECT / path for path in driver]
        driver_args = [*args, "-DZLIB_BUILD"] if program == "example" else [a for a in args if a != "-DZLIB_BUILD"]
        native_build = link(toolchain, driver_args, driver_paths, objects, work / f"{program}-c{suffix}", [])
        if native_build.returncode:
            notes.append(f"{program}: C build failed: {native_build.stdout[-300:]}{native_build.stderr[-300:]}")
            continue
        rust_build = link(toolchain, driver_args, driver_paths, [kept], work / f"{program}-rust{suffix}", native)
        if rust_build.returncode:
            return {**report, "status": "link failed", "detail": f"{program}\n{rust_build.stdout[-3000:]}"
                                                                 f"{rust_build.stderr[-3000:]}"}
        binaries["c"][program] = work / f"{program}-c{suffix}"
        binaries["rust"][program] = work / f"{program}-rust{suffix}"
    if "minigzip" not in binaries["rust"]:
        return {**report, "status": "no minigzip", "detail": "; ".join(notes)}
    inputs = make_inputs(work / "data")
    outcome = compare_programs(toolchain, binaries, inputs, work)
    status = "pass" if not outcome["failures"] else "mismatch"
    for program in binaries:
        for path in binaries[program].values():
            path.unlink(missing_ok=True)
    kept.unlink(missing_ok=True)
    return {**report, "status": status, "checks": outcome["checks"], "failures": outcome["failures"],
            "c_failures": outcome["c_failures"], "notes": notes}


def validate_target_runs(crate, matrix, target, toolchain, sets, out, control):
    out.mkdir(parents=True, exist_ok=True)
    return [validate_run(crate, matrix, target, toolchain, features, out, control) for features in sets]


def installed_targets(toolchain):
    output = run(["rustup", f"+{toolchain}", "target", "list", "--installed"], text=True)
    return set(output.stdout.split())


def check_command(triple, pinned, default):
    if triple in pinned:
        return PINNED, []
    if triple in default:
        return "nightly", []
    return PINNED, ["-Zbuild-std"]


def system_includes(triple):
    root = Path.home() / ".local/share/slate/sysroots" / triple
    resource = Path(run(["clang", "-print-resource-dir"], text=True).stdout.strip()) / "include"
    includes = [resource]
    if "windows" in triple:
        includes.append(root / "crt/include")
        includes += sorted(path for path in (root / "sdk/include").iterdir() if path.is_dir())
        return includes
    usr = root / ("SDK/usr/include" if "apple" in triple else "usr/include")
    arch = triple.split("-")[0]
    prefixes = {"armv7": "arm", "i686": "i386"}
    includes += sorted(path for path in usr.iterdir() if path.is_dir()
                       and path.name.startswith((arch, prefixes.get(arch, arch))) and (path / "asm").is_dir())
    includes.append(usr)
    return includes


def field_name(text):
    text = re.sub(r"(\[[^\]]*\])+$", "", text.strip())
    match = re.search(r"([A-Za-z_]\w*)$", text)
    return match[1] if match else None


def record_name(text):
    unnamed = re.search(r"\(.*?at (.*?):(\d+):\d+\)", text)
    if unnamed:
        stem = Path(unnamed[1]).name.replace(".", "_").replace("-", "_")
        return f"__SlateRecord_{stem}_{unnamed[2]}"
    return text.split()[-1]


def parse_layouts(dump):
    records = {}
    for block in dump.split("*** Dumping AST Record Layout")[1:]:
        lines = block.splitlines()[1:]
        if not lines or "|" not in lines[0]:
            continue
        head = lines[0].split("|", 1)[1].strip()
        kind = head.split()[0]
        name = record_name(head)
        fields = {}
        size = None
        for line in lines[1:]:
            if "|" not in line:
                break
            offset, text = line.split("|", 1)
            found = re.search(r"\[sizeof=(\d+),.*?\balign=(\d+)", text)
            if found:
                size = (int(found[1]), int(found[2]))
                break
            offset = offset.strip()
            if not offset.isdigit() or not re.match(r"  \S", text[1:] if text.startswith(" ") else text):
                continue
            if "unnamed" in text:
                continue
            field = field_name(text)
            if field:
                fields[field] = int(offset)
        if size:
            records[name] = {"kind": kind, "size": size, "fields": fields}
    return records


def clang_layouts(matrix, target, features, source):
    triple = target["triple"]
    clang_triple = {"armv7-unknown-linux-gnueabihf": "armv7-unknown-linux-gnueabihf"}.get(triple, triple)
    extra = ["-fno-blocks"] if "apple" in triple else []
    includes = [flag for path in system_includes(triple) for flag in ("-isystem", str(path))]
    result = run(["clang", f"--target={clang_triple}", "-nostdinc", *includes, "-fsyntax-only", "-w", *extra,
                  "-Xclang", "-fdump-record-layouts-complete", *absolute_includes(compile_args(matrix, target, features)),
                  str(PROJECT / source)], text=True)
    return result, parse_layouts(result.stdout)


RUST_KEYWORDS = {"type", "match", "ref", "move", "fn", "impl", "loop", "mod", "use", "where", "in", "box", "final"}


def assertions(records, defined, control):
    lines = []
    for name, record in sorted(records.items()):
        if name not in defined:
            continue
        size, align = record["size"]
        size += control
        lines.append(f'const _: () = assert!(::core::mem::size_of::<{name}>() == {size}, "layout|{name}|sizeof|{size}");')
        lines.append(f'const _: () = assert!(::core::mem::align_of::<{name}>() == {align}, "layout|{name}|align|{align}");')
        if record["kind"] == "union":
            continue
        for field, offset in record["fields"].items():
            rust = f"r#{field}" if field in RUST_KEYWORDS else field
            lines.append(f'const _: () = assert!(::core::mem::offset_of!({name}, {rust}) == {offset}, '
                         f'"layout|{name}|{field}|{offset}");')
    return lines


def check_layouts(crate, matrix, target, features, copy, target_dir, tool, control):
    triple = target["triple"]
    report = {"target": target["name"], "features": label(features)}
    if copy.exists():
        shutil.rmtree(copy)
    shutil.copytree(crate, copy, ignore=shutil.ignore_patterns("target"))
    total = 0
    disabled = {}
    units = {}
    for source in matrix["sources"]:
        unit = Path(source).stem
        result, records = clang_layouts(matrix, target, features, source)
        if result.returncode and not records:
            return {**report, "status": "clang failed", "detail": f"{source}: {result.stderr[-1500:]}"}
        path = copy / "src" / f"{unit}.rs"
        defined = set(re.findall(r"^\s*(?:pub\s+)?(?:struct|union)\s+(\w+)", path.read_text(), re.M))
        units[unit] = (path, path.read_text(), assertions(records, defined, control))
    (toolchain, extra) = tool
    env = dict(os.environ, RUSTUP_TOOLCHAIN=toolchain, CARGO_TARGET_DIR=str(target_dir))
    command = ["cargo", "check", *extra, "--release", "--lib", "--target", triple, "--no-default-features", "--features",
               ",".join(features), "--message-format", "short"]
    for attempt in range(6):
        total = 0
        for unit, (path, text, lines) in units.items():
            kept = [line for line in lines if not any(f"<{name}>" in line or f"!({name}," in line
                                                      for name in disabled.get(unit, ()))]
            total += len(kept)
            path.write_text(text + "\n" + "\n".join(kept) + "\n")
        result = run(command, cwd=copy, env=env, text=True)
        missing = re.findall(r"src/(\w+)\.rs:\d+:\d+: error\[E0(?:412|425|433)\]: cannot find type `(\w+)`", result.stderr)
        if not missing:
            break
        for unit, name in missing:
            disabled.setdefault(unit, set()).add(name)
    failures = sorted(set(re.findall(r"layout\|(\w+)\|(\w+)\|(\d+)", result.stderr)))
    no_field = sorted(set(re.findall(r"src/(\w+)\.rs:\d+:\d+: error\[E0609\]: (no field .*)", result.stderr)))
    other = [line for line in result.stderr.splitlines() if ": error" in line and "layout|" not in line
             and "E0609" not in line and "aborting" not in line and "evaluation" not in line]
    status = "pass" if result.returncode == 0 else ("mismatch" if failures or no_field else "check failed")
    return {**report, "status": status, "assertions": total, "failures": failures, "no_field": no_field,
            "toolchain": toolchain + (" build-std" if extra else ""), "other": other[:12],
            "tail": result.stderr[-1500:] if status == "check failed" else ""}


def validate_target_layouts(crate, matrix, target, sets, out, tool, control):
    out.mkdir(parents=True, exist_ok=True)
    return [check_layouts(crate, matrix, target, features, out / "crate", out / "cargo", tool, control)
            for features in sets]


def layouts_main(args, matrix, targets, sets):
    names = args.targets.split(",") if args.targets else list(targets)
    pinned = installed_targets(PINNED)
    default = installed_targets("nightly")
    with concurrent.futures.ThreadPoolExecutor(args.jobs) as pool:
        futures = [pool.submit(validate_target_layouts, args.crate, matrix, targets[name], sets, args.work / name,
                               check_command(targets[name]["triple"], pinned, default), args.control)
                   for name in names]
        results = [entry for future in futures for entry in future.result()]
    (args.work / "layouts.json").write_text(json.dumps(results, indent=1, default=str))
    failed = 0
    for entry in results:
        print(f"{entry['target']:20} {entry['features']:40} {entry['status']:14} {entry.get('toolchain', ''):22} "
              f"asserts={entry.get('assertions', 0)} bad={len(entry.get('failures', []))} "
              f"nofield={len(entry.get('no_field', []))}")
        for failure in [*entry.get("failures", []), *entry.get("no_field", []), *entry.get("other", [])][:8]:
            print(f"    {failure}")
        if entry.get("detail") or entry.get("tail"):
            print("    " + (entry.get("detail") or entry["tail"]).replace("\n", "\n    "))
        failed += entry["status"] != "pass"
    print(f"{len(results) - failed}/{len(results)} passed")
    sys.exit(1 if failed else 0)


def codegen_target(crate, target, sets, out, tool):
    (toolchain, extra) = tool
    env = dict(os.environ, RUSTUP_TOOLCHAIN=toolchain, CARGO_TARGET_DIR=str(out))
    results = []
    for features in sets:
        result = run(["cargo", "build", *extra, "--release", "--lib", "--target", target["triple"],
                      "--no-default-features", "--features", ",".join(features), "--message-format", "short"],
                     cwd=crate, env=env, text=True)
        errors = [line for line in result.stderr.splitlines() if "error" in line and "warning" not in line]
        results.append({"target": target["name"], "features": label(features),
                        "status": "pass" if result.returncode == 0 else "build failed",
                        "toolchain": toolchain + (" build-std" if extra else ""), "errors": errors[:8]})
    return results


def codegen_main(args, matrix, targets, sets):
    names = args.targets.split(",") if args.targets else list(targets)
    pinned = installed_targets(PINNED)
    default = installed_targets("nightly")
    with concurrent.futures.ThreadPoolExecutor(args.jobs) as pool:
        futures = [pool.submit(codegen_target, args.crate, targets[name], sets, args.work / name,
                               check_command(targets[name]["triple"], pinned, default)) for name in names]
        results = [entry for future in futures for entry in future.result()]
    (args.work / "codegen.json").write_text(json.dumps(results, indent=1))
    failed = [entry for entry in results if entry["status"] != "pass"]
    for entry in failed:
        print(entry["target"], entry["features"], *entry["errors"], sep="\n    ")
    print(f"{len(results) - len(failed)}/{len(results)} passed")
    sys.exit(1 if failed else 0)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--crate", type=Path, required=True)
    parser.add_argument("--work", type=Path, required=True)
    parser.add_argument("--targets", default="")
    parser.add_argument("--features", default="")
    parser.add_argument("--jobs", type=int, default=4)
    parser.add_argument("--layouts", action="store_true")
    parser.add_argument("--control", action="store_true")
    parser.add_argument("--codegen", action="store_true")
    args = parser.parse_args()
    matrix = json.loads(MATRIX.read_text())
    targets = {target["name"]: target for target in matrix["targets"]}
    if args.codegen:
        names = [feature["name"] for feature in matrix["features"]]
        codegen_main(args, matrix, targets, [[]] if args.features == "default" else feature_sets(names))
    if args.layouts:
        names = [feature["name"] for feature in matrix["features"]]
        layouts_main(args, matrix, targets, [[]] if args.features == "default" else feature_sets(names))
    toolchains = {**linux_toolchains(), **windows_toolchains()}
    names = args.targets.split(",") if args.targets else list(toolchains)
    all_features = [feature["name"] for feature in matrix["features"]]
    sets = [all_features] if args.features == "all" else feature_sets(all_features)
    if args.features == "default":
        sets = [[]]
    with concurrent.futures.ThreadPoolExecutor(args.jobs) as pool:
        futures = [pool.submit(validate_target_runs, args.crate, matrix, targets[name], toolchains[name], sets,
                               args.work / name, args.control) for name in names]
        results = [entry for future in futures for entry in future.result()]
    (args.work / "runs.json").write_text(json.dumps(results, indent=1, default=str))
    failed = 0
    for entry in results:
        print(f"{entry['target']:16} {entry['features']:40} {entry['status']:18} checks={entry.get('checks', 0)} "
              f"c_nonzero={len(entry.get('c_failures', []))} mismatches={len(entry.get('failures', []))}")
        for failure in entry.get("failures", [])[:4]:
            print(f"    {failure}")
        if entry.get("detail"):
            print("    " + entry["detail"].replace("\n", "\n    "))
        for note in entry.get("notes", []):
            print(f"    note: {note}")
        failed += entry["status"] != "pass"
    print(f"{len(results) - failed}/{len(results)} passed")
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
