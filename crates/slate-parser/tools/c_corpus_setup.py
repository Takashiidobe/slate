#!/usr/bin/env python3
import argparse
import os
import shutil
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

FLAVORS = ("gcc", "msvc", "clang")
MSVC_BIN = (
    Path(os.environ.get("MSVC_WINE_ROOT", Path.home() / ".local/share/msvc-wine/opt/msvc"))
    / "bin"
    / os.environ.get("MSVC_ARCH", "x64")
)
JOBS = str(os.cpu_count() or 8)


@dataclass
class CMake:
    source: str = "."
    options: list[str] = field(default_factory=list)
    msvc_options: list[str] | None = field(default_factory=list)
    python_requirements: list[str] = field(default_factory=list)

    def flavors(self):
        return FLAVORS if self.msvc_options is not None else ("gcc", "clang")


@dataclass
class Configure:
    configure: list[str]
    make: list[str] = field(default_factory=list)
    bootstrap: list[str] | None = None

    def flavors(self):
        return ("gcc", "clang")


@dataclass
class InTree:
    make: list[str] = field(default_factory=list)
    clean: list[str] = field(default_factory=lambda: ["make", "clean"])
    configure: list[str] | None = None

    def flavors(self):
        return ("gcc", "clang")


@dataclass
class Nmake:
    unix: Configure
    makefile: str
    options: list[str] = field(default_factory=list)

    def flavors(self):
        return FLAVORS


MSVC_CURL = [
    "-DCURL_USE_SCHANNEL=ON",
    "-DCURL_USE_OPENSSL=OFF",
    "-DCURL_USE_LIBPSL=OFF",
    "-DCURL_USE_LIBSSH2=OFF",
    "-DUSE_LIBIDN2=OFF",
    "-DUSE_NGHTTP2=OFF",
    "-DCURL_ZLIB=OFF",
    "-DCURL_BROTLI=OFF",
    "-DCURL_ZSTD=OFF",
    "-DBUILD_LIBCURL_DOCS=OFF",
    "-DBUILD_MISC_DOCS=OFF",
    "-DENABLE_CURL_MANUAL=OFF",
]

RECIPES = {
    "cJSON": CMake(options=["-DENABLE_CJSON_TEST=On"]),
    "curl": CMake(msvc_options=MSVC_CURL),
    "libexpat": CMake(source="expat"),
    "libpng": CMake(
        options=["-DZLIB_INCLUDE_DIR={corpus}/zlib", "-DZLIB_LIBRARY={corpus}/zlib/build-{flavor}/{zlib}"]
    ),
    "libuv": CMake(options=["-DLIBUV_BUILD_TESTS=ON", "-DLIBUV_BUILD_BENCH=ON"]),
    "libyaml": CMake(),
    "lz4": CMake(source="build/cmake"),
    "mbedtls": CMake(
        options=["-DENABLE_TESTING=ON", "-DENABLE_PROGRAMS=ON", "-DMBEDTLS_FATAL_WARNINGS=OFF"],
        python_requirements=["scripts/basic.requirements.txt", "tf-psa-crypto/scripts/basic.requirements.txt"],
    ),
    "pcre2": CMake(options=["-DPCRE2_BUILD_TESTS=ON"]),
    "utf8proc": CMake(),
    "yyjson": CMake(),
    "zlib": CMake(),
    "zstd": CMake(source="build/cmake"),
    "mimalloc": CMake(),
    "libdeflate": CMake(options=["-DLIBDEFLATE_BUILD_TESTS=ON"]),
    "xxHash": CMake(source="build/cmake", options=["-DDISPATCH=ON"]),
    "oniguruma": CMake(),
    "cglm": CMake(options=["-DCGLM_USE_TEST=ON"]),
    "libevent": CMake(options=["-DEVENT__DISABLE_OPENSSL=ON", "-DEVENT__DISABLE_MBEDTLS=ON"]),
    "c-ares": CMake(),
    "sqlite": Nmake(
        unix=Configure(["{source}/configure", "CC={cc}"], ["all", "testfixture"]),
        makefile="Makefile.msc",
        options=["TOP=..", "USE_AMALGAMATION=0"],
    ),
    "musl": Configure(["{source}/configure", "CC={cc}"]),
    "tinycc": Configure(["{source}/configure", "--cc={cc}"]),
    "cpython": Configure(["{source}/configure", "CC={cc}"]),
    "libsodium": Configure(["{source}/configure", "CC={cc}"], ["check"], bootstrap=["./autogen.sh", "-s"]),
    "postgres": Configure(["{source}/configure", "CC={cc}"], ["world-bin"]),
    "linux": Configure(
        ["make", "-C", "{source}", "O={build}", "CC={cc}", "HOSTCC={cc}", "defconfig"],
        ["CC={cc}", "HOSTCC={cc}"],
    ),
    "jq": Configure(
        ["{source}/configure", "CC={cc}", "--with-oniguruma=builtin", "--disable-docs"],
        bootstrap=["autoreconf", "-i"],
    ),
    "nginx": InTree(
        configure=["auto/configure", "--with-cc={cc}", "--builddir={build}/objs"],
        clean=["rm", "-rf", "{build}/objs"],
    ),
    "redis": InTree(make=["CC={cc}"], clean=["make", "distclean"]),
    "lua": InTree(make=["CC={cc}"]),
    "quickjs": InTree(make=["CC={cc}"]),
    "chibicc": InTree(make=["CC={cc}"]),
    "giflib": InTree(make=["CC={cc}", "MAKE=true"]),
    "lmdb": InTree(make=["-C", "libraries/liblmdb", "CC={cc}"], clean=["make", "-C", "libraries/liblmdb", "clean"]),
    "stb": InTree(make=["-i", "-C", "tests", "CC={cc}"], clean=["true"]),
}


def compiler(flavor: str) -> str:
    return {"gcc": "gcc", "clang": "clang", "msvc": "cl"}[flavor]


def environment(flavor: str) -> dict[str, str]:
    env = dict(os.environ)
    if flavor == "msvc":
        env["PATH"] = f"{MSVC_BIN}:{env['PATH']}"
        env["WINEDEBUG"] = "-all"
    else:
        env["CC"] = compiler(flavor)
    return env


def expand(args: list[str], values: dict[str, str]) -> list[str]:
    return [arg.format(**values) for arg in args]


def run(argv: list[str], cwd: Path, env: dict[str, str], log, required: bool = True) -> bool:
    log.write(f"$ {' '.join(argv)}  (in {cwd})\n")
    log.flush()
    status = subprocess.run(argv, cwd=cwd, env=env, stdout=log, stderr=subprocess.STDOUT).returncode
    if status and required:
        raise RuntimeError(f"{argv[0]} exited {status}")
    return status == 0


def bear(output: Path, argv: list[str]) -> list[str]:
    return ["bear", "--output", str(output), "--", *argv]


def setup(corpus: Path, project: str, flavor: str) -> str:
    recipe = RECIPES[project]
    root = corpus / project
    build = root / f"build-{flavor}"
    if build.exists():
        shutil.rmtree(build)
    build.mkdir()
    database = build / "compile_commands.json"
    env = environment(flavor)
    values = {
        "cc": compiler(flavor),
        "corpus": str(corpus),
        "source": str(root),
        "build": str(build),
        "flavor": flavor,
        "zlib": "libz.lib" if flavor == "msvc" else "libz.a",
    }
    with open(root / f"build-{flavor}.log", "w") as log:
        match recipe:
            case CMake():
                options = recipe.options + (recipe.msvc_options or []) * (flavor == "msvc")
                if recipe.python_requirements:
                    python = python_environment(corpus, [root / r for r in recipe.python_requirements], log)
                    options = [*options, f"-DPython3_EXECUTABLE={python}"]
                configure = [
                    "cmake", "-S", str(root / recipe.source), "-B", str(build), "-G", "Ninja",
                    "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
                    f"-DCMAKE_C_COMPILER={compiler(flavor)}",
                    *(["-DCMAKE_SYSTEM_NAME=Windows"] if flavor == "msvc" else []),
                    *expand(options, values),
                ]
                run(configure, root, env, log)
                built = run(["cmake", "--build", str(build), "--", "-k", "0", "-j", JOBS], root, env, log, required=False)
            case Configure():
                if recipe.bootstrap:
                    run(expand(recipe.bootstrap, values), root, env, log)
                run(expand(recipe.configure, values), build, env, log)
                built = run(bear(database, ["make", "-j", JOBS, *expand(recipe.make, values)]), build, env, log, required=False)
            case Nmake() if flavor == "msvc":
                argv = ["nmake", "/nologo", "/i", "/f", f"..\\{recipe.makefile}", *expand(recipe.options, values)]
                built = run(argv, build, env, log, required=False)
                write_nmake_database(build, root / f"build-{flavor}.log", database)
            case Nmake():
                run(expand(recipe.unix.configure, values), build, env, log)
                built = run(bear(database, ["make", "-j", JOBS, *recipe.unix.make]), build, env, log, required=False)
            case InTree():
                run(expand(recipe.clean, values), root, env, log, required=False)
                if recipe.configure:
                    run(expand(recipe.configure, values), root, env, log)
                argv = ["make", "-j", JOBS, *expand(recipe.make, values)]
                built = run(bear(database, argv), root, env, log, required=False)
    if not database.exists():
        raise RuntimeError("no compile_commands.json written")
    return "ok" if built else "database written, build incomplete"


def python_environment(corpus: Path, requirements: list[Path], log) -> Path:
    venv = corpus / ".venv"
    if not venv.exists():
        run([sys.executable, "-m", "venv", str(venv)], corpus, dict(os.environ), log)
    python = venv / "bin" / "python"
    install = [str(python), "-m", "pip", "install", "-q"]
    for requirement in requirements:
        install += ["-r", str(requirement)]
    run(install, corpus, dict(os.environ), log)
    return python


def write_nmake_database(build: Path, log: Path, database: Path) -> None:
    import json
    import shlex

    entries = []
    for line in log.read_text(errors="replace").splitlines():
        words = shlex.split(line.strip(), posix=False) if line.strip().lower().startswith("cl ") else []
        sources = [w for w in words if w.lower().endswith(".c") and not w.startswith("/")]
        for source in sources:
            path = source.replace("\\", "/")
            path = path[2:] if path[:2].upper() == "Z:" else os.path.normpath(build / path)
            entries.append({"directory": str(build), "file": path, "arguments": words})
    database.write_text(json.dumps(entries, indent=2))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--corpus", type=Path, default=Path(os.environ.get("SLATE_CORPUS", Path.home() / "c-corpus")))
    parser.add_argument("--flavor", choices=FLAVORS, action="append")
    parser.add_argument("projects", nargs="*")
    args = parser.parse_args()
    failed = False
    for project in args.projects or sorted(RECIPES):
        recipe = RECIPES[project]
        for flavor in FLAVORS:
            if flavor not in recipe.flavors() or (args.flavor and flavor not in args.flavor):
                continue
            try:
                status = setup(args.corpus, project, flavor)
            except (RuntimeError, OSError) as error:
                status, failed = f"FAILED: {error}", True
            print(f"{project:10} {flavor:6} {status}", flush=True)
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
