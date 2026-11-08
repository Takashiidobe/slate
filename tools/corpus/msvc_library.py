#!/usr/bin/env python3
import argparse
import json
import os
import re
import shutil
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SLATE = ROOT / "target/test-cache/release/slate"
MSVC_ROOT = Path(os.environ.get("MSVC_WINE_ROOT", Path.home() / ".local/share/msvc-wine/opt/msvc"))
ARCHES = {"x86_64": "x64", "i686": "x86"}


@dataclass
class MsvcLibrary:
    sources: list[str]
    drivers: list[str]
    includes: list[str] = field(default_factory=list)
    options: list[str] = field(default_factory=list)
    system_libs: list[str] = field(default_factory=list)


RECIPES = {
    "minhook": MsvcLibrary(
        sources=["src/*.c", "src/hde/*.c"],
        drivers=["msvc/minhook_driver.c"],
        includes=["include"],
        options=["/O2", "/MT", "/Zc:preprocessor", "/DWIN32", "/DNDEBUG"],
    ),
}


def run(command, log, cwd=ROOT, env=None):
    print(f"$ {' '.join(map(str, command))}", flush=True)
    with log.open("w") as handle:
        code = subprocess.run(command, cwd=cwd, env=env, stdout=handle, stderr=subprocess.STDOUT).returncode
    if code:
        print("\n".join(log.read_text(errors="replace").splitlines()[-30:]))
        raise SystemExit(f"failed ({code}), see {log}")


def msvc_tool(arch, tool):
    return MSVC_ROOT / "bin" / ARCHES[arch] / tool


def newest(directory):
    return sorted(directory.iterdir())[-1]


def cargo_env(arch):
    msvc_arch = ARCHES[arch]
    tools = newest(MSVC_ROOT / "vc/tools/msvc")
    kits_lib = newest(MSVC_ROOT / "kits/10/lib")
    kits_include = newest(MSVC_ROOT / "kits/10/include")
    triple = f"{arch}_pc_windows_msvc"
    return dict(
        os.environ,
        LIB=";".join(map(str, [tools / "lib" / msvc_arch, kits_lib / "um" / msvc_arch, kits_lib / "ucrt" / msvc_arch])),
        INCLUDE=";".join(map(str, [tools / "include", kits_include / "ucrt", kits_include / "um",
                                    kits_include / "shared"])),
        **{f"CC_{triple}": "clang-cl", f"AR_{triple}": "llvm-lib"},
        RUSTFLAGS=" ".join(filter(None, [os.environ.get("RUSTFLAGS"), "-A warnings", "-C target-feature=+crt-static",
                                         "--print native-static-libs"])),
    )


def wine_env():
    return dict(os.environ, WINEDEBUG="-all")


def check_arch(name, recipe, project_dir, arch, out):
    out.mkdir(parents=True, exist_ok=True)
    sources = sorted(path for pattern in recipe.sources for path in project_dir.glob(pattern))
    includes = [f"/I{project_dir / include}" for include in recipe.includes]
    cl, lib = msvc_tool(arch, "cl"), msvc_tool(arch, "lib")

    native_objects = out / "native-obj"
    native_objects.mkdir(exist_ok=True)
    run([cl, "/nologo", "/c", *recipe.options, *includes, f"/Fo{native_objects}/", *sources],
        out / "native-compile.log", env=wine_env())
    native_lib = out / f"{name}.native.lib"
    run([lib, "/nologo", f"/OUT:{native_lib}", *sorted(native_objects.glob("*.obj"))], out / "native-lib.log",
        env=wine_env())

    database = out / "compile_commands.json"
    database.write_text(json.dumps([
        {"directory": str(project_dir), "file": str(source),
         "arguments": ["cl.exe", f"--target={arch}-pc-windows-msvc", "/nologo", "/c", *recipe.options, *includes,
                       str(source)]}
        for source in sources
    ], indent=1) + "\n")
    crate = out / "rust"
    if crate.exists():
        shutil.rmtree(crate)
    run([SLATE, "translate-project", "--crate-type", "staticlib", "--compile-commands", database, project_dir, crate],
        out / "translate.log")
    triple = f"{arch}-pc-windows-msvc"
    env = cargo_env(arch) | {"CARGO_TARGET_DIR": str(out / "cargo-target")}
    run(["cargo", "build", "--release", "--target", triple], out / "build.log", cwd=crate, env=env)
    translated_lib = next((out / "cargo-target" / triple / "release").glob("*.lib"))
    native_static = re.findall(r"native-static-libs: (.*)", (out / "build.log").read_text())
    rust_libs = [word for word in native_static[-1].split() if word.endswith(".lib")] if native_static else []

    mismatches = []
    for driver in recipe.drivers:
        driver_path = ROOT / "tools/corpus" / driver
        stem = driver_path.stem
        results = {}
        for leg, archive, extra in (("native", native_lib, []), ("translated", translated_lib, rust_libs)):
            exe = out / f"{stem}.{leg}.exe"
            objects = out / f"{stem}.{leg}-obj"
            objects.mkdir(exist_ok=True)
            run([cl, "/nologo", *recipe.options, *includes, f"/Fo{objects}/", f"/Fe{exe}", driver_path, archive,
                 *recipe.system_libs, *dict.fromkeys(extra)], out / f"{stem}.{leg}.link.log", env=wine_env())
            proc = subprocess.run(["wine", exe], cwd=out, env=wine_env(), capture_output=True, timeout=300)
            (out / f"{stem}.{leg}.out").write_bytes(proc.stdout)
            results[leg] = (proc.returncode, proc.stdout)
        if results["native"] != results["translated"]:
            mismatches.append(stem)
            print(f"MISMATCH {name} {arch} {stem}: native exit {results['native'][0]}, "
                  f"translated exit {results['translated'][0]}; diff {out}/{stem}.native.out {out}/{stem}.translated.out")
        else:
            lines = results["native"][1].decode(errors="replace").count("\n")
            print(f"OK {name} {arch} {stem}: exit {results['native'][0]}, {lines} identical lines")
    return mismatches


def main():
    parser = argparse.ArgumentParser(description="translate an MSVC C library and diff drivers against the native lib")
    parser.add_argument("project", choices=sorted(RECIPES))
    parser.add_argument("--arch", choices=sorted(ARCHES), action="append")
    parser.add_argument("--corpus", type=Path, default=Path(os.environ.get("SLATE_CORPUS", Path.home() / "c-corpus")))
    parser.add_argument("--out", type=Path)
    args = parser.parse_args()

    recipe = RECIPES[args.project]
    project_dir = (args.corpus / args.project).resolve()
    out = (args.out or ROOT / "target/corpus" / args.project / "msvc").resolve()
    subprocess.run(["cargo", "build", "--release", "-p", "slate-c2rust"], cwd=ROOT, check=True)
    mismatches = []
    for arch in args.arch or sorted(ARCHES, reverse=True):
        mismatches += [f"{arch}/{stem}" for stem in check_arch(args.project, recipe, project_dir, arch, out / arch)]
    return 1 if mismatches else 0


if __name__ == "__main__":
    sys.exit(main())
