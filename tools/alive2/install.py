import argparse
import json
import shutil
import subprocess
from pathlib import Path


REVISION = "01a5ec45c8152995755f7331827407a9de19f262"
REPOSITORY = "https://github.com/AliveToolkit/alive2.git"
ROOT = Path(__file__).resolve().parents[2]
COMPONENT_LINK = "  llvm_map_components_to_libnames(llvm_libs support core irreader bitwriter analysis passes transformutils codegen targetparser AllTargetsCodeGens AllTargetsDescs AllTargetsInfos)"


def run(command):
    subprocess.run([str(arg) for arg in command], cwd=ROOT, check=True)


def read(command):
    return subprocess.check_output(
        [str(arg) for arg in command], cwd=ROOT, text=True
    ).strip()


def main():
    parser = argparse.ArgumentParser(
        description="Build standalone alive-tv against LLVM 23"
    )
    parser.add_argument("--prefix", type=Path, default=Path.home() / ".local")
    parser.add_argument("--llvm-config", default="llvm-config")
    parser.add_argument("--jobs", type=int, default=4)
    args = parser.parse_args()
    if Path.cwd() != ROOT:
        parser.error("run from the workspace root")
    if args.jobs < 1:
        parser.error("--jobs must be positive")
    llvm = read([args.llvm_config, "--version"])
    if llvm.split(".")[0] != "23":
        parser.error(f"this Alive2 revision targets LLVM 23, found {llvm}")
    prefix = args.prefix.resolve()
    source = prefix / "share/alive2"
    if not source.exists():
        source.parent.mkdir(parents=True, exist_ok=True)
        run(["git", "clone", "--no-checkout", REPOSITORY, source])
        run(["git", "-C", source, "checkout", "--detach", REVISION])
    if read(["git", "-C", source, "rev-parse", "HEAD"]) != REVISION:
        parser.error(f"{source} is not at the pinned revision; use another --prefix")
    cmake = source / "CMakeLists.txt"
    original = read(["git", "-C", source, "show", "HEAD:CMakeLists.txt"]) + "\n"
    if COMPONENT_LINK not in original:
        parser.error("pinned CMake layout changed")
    shared = original.replace(COMPONENT_LINK, "  set(llvm_libs LLVM)")
    if cmake.read_text() not in (original, shared):
        parser.error(f"{cmake} has unrelated changes")
    cmake.write_text(shared)
    build = source / "build"
    run(
        [
            "cmake",
            "-S",
            source,
            "-B",
            build,
            "-G",
            "Ninja",
            "-DCMAKE_BUILD_TYPE=Release",
            "-DBUILD_TV=1",
            f"-DLLVM_DIR={read([args.llvm_config, '--cmakedir'])}",
            "-DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld",
        ]
    )
    run(["cmake", "--build", build, "--target", "alive-tv", "-j", args.jobs])
    binary = prefix / "bin/alive-tv"
    binary.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(build / "alive-tv", binary)
    run([binary, "--version"])
    (source / "installation.json").write_text(
        json.dumps(
            {
                "revision": REVISION,
                "llvm": llvm,
                "binary": str(binary),
                "cmake_adjustment": "link shared LLVM instead of component archives",
            },
            indent=2,
        )
        + "\n"
    )
    print(f"installed {binary}")


if __name__ == "__main__":
    main()
