#!/usr/bin/env python3
import argparse
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
PROBE = ROOT / "tools/libc-abi-probe.py"
SHIM = ROOT / "libc-shim/include"


TARGETS = {
    "x86_64": {
        "musl_target": "x86_64-linux-musl",
        "glibc_target": "x86_64-linux-gnu",
        "arch_define": "__SLATE_ARCH_X86_64",
        "wordsize_define": "__SLATE_WORDSIZE_64",
        "compiler_args": [],
        "musl_linker": "musl-clang",
        "qemu": None,
    },
    "i386": {
        "musl_target": "i686-linux-musl",
        "glibc_target": "i686-linux-gnu",
        "arch_define": "__SLATE_ARCH_X86",
        "wordsize_define": "__SLATE_WORDSIZE_32",
        "compiler_args": ["-m32"],
        "musl_linker": "musl-clang",
        "qemu": "qemu-i386-static",
    },
    "arm": {
        "musl_target": "arm-linux-musleabihf",
        "glibc_target": "armv7-linux-gnueabihf",
        "arch_define": "__SLATE_ARCH_ARM",
        "wordsize_define": "__SLATE_WORDSIZE_32",
        "compiler_args": [],
        "musl_linker": "musl-gcc",
        "qemu": "qemu-arm-static",
    },
    "aarch64": {
        "musl_target": "aarch64-linux-musl",
        "glibc_target": "aarch64-linux-gnu",
        "arch_define": "__SLATE_ARCH_AARCH64",
        "wordsize_define": "__SLATE_WORDSIZE_64",
        "compiler_args": [],
        "musl_linker": "musl-gcc",
        "qemu": "qemu-aarch64-static",
    },
}


def env_path(name, default=None):
    value = os.environ.get(name, default)
    return Path(value).expanduser() if value else None


def require_file(path, description):
    if not path or not path.is_file():
        raise RuntimeError(f"{description} is missing: {path}")
    return path


def require_executable(value, description):
    path = Path(value).expanduser()
    if path.is_file() and os.access(path, os.X_OK):
        return path
    found = shutil.which(str(value))
    if found:
        return Path(found)
    raise RuntimeError(f"{description} is missing: {value}")


def require_dir(path, description):
    if not path or not path.is_dir():
        raise RuntimeError(f"{description} is missing: {path}")
    return path


def qemu_for(name):
    override = os.environ.get(f"SLATE_LIBC_QEMU_{name.upper()}")
    if override:
        return require_file(Path(override), f"QEMU for {name}")
    command = TARGETS[name]["qemu"]
    if not command:
        return None
    found = shutil.which(command)
    if not found:
        raise RuntimeError(f"QEMU for {name} is unavailable; set SLATE_LIBC_QEMU_{name.upper()}")
    return Path(found)


def slate_defines(name, libc):
    target = TARGETS[name]
    defines = [
        "-D_SLATE_LIBC",
        f"-D{target['arch_define']}",
        f"-D{target['wordsize_define']}",
        "-D__SLATE_VENDOR_UNKNOWN",
        "-D__SLATE_KERNEL_LINUX",
        "-D__SLATE_OBJ_ELF",
        "-D__SLATE_ENDIAN_LITTLE",
        f"-D__SLATE_LIBC_{libc.upper()}",
    ]
    if libc == "glibc":
        defines.append(
            f"-D__SLATE_GLIBC_MINOR__={os.environ.get('SLATE_GLIBC_MINOR', '43')}"
        )
    return defines


def config(name, libc, slate_clang, musl_root):
    target = TARGETS[name]
    if libc == "musl":
        sysroot = require_dir(musl_root / name, f"musl {name} sysroot")
        compiler = slate_clang
        linker = require_file(
            sysroot / "bin" / target["musl_linker"],
            f"musl {name} linker",
        )
        linker_args = ["-static"]
        linker_post_args = ["-Wl,--start-group", "-lc", "-lgcc", "-Wl,--end-group"]
        if name == "aarch64":
            linker = require_executable(
                os.environ.get("SLATE_MUSL_AARCH64_LINKER", "aarch64-linux-gnu-gcc"),
                "AArch64 musl linker",
            )
            linker_args = [
                "-static",
                "-nostdlib",
                "-nostartfiles",
                "-nodefaultlibs",
                str(sysroot / "lib/crt1.o"),
                str(sysroot / "lib/crti.o"),
                "-L" + str(sysroot / "lib"),
            ]
            linker_post_args = [
                "-Wl,--start-group",
                "-lc",
                "-lgcc",
                "-lgcc_eh",
                "-Wl,--end-group",
                str(sysroot / "lib/crtn.o"),
            ]
        target_triple = target["musl_target"]
    else:
        sysroot_names = {
            "x86_64": ["SLATE_GLIBC_SYSROOT_X86_64"],
            "i386": ["SLATE_GLIBC_SYSROOT_X86", "SLATE_I686_SYSROOT"],
            "arm": ["SLATE_GLIBC_SYSROOT_ARM", "SLATE_ARM_SYSROOT"],
            "aarch64": ["SLATE_GLIBC_SYSROOT_AARCH64", "SLATE_AARCH64_SYSROOT"],
        }
        sysroot_value = next(
            (os.environ[name] for name in sysroot_names[name] if os.environ.get(name)),
            None,
        )
        sysroot = require_dir(
            env_path(sysroot_names[name][0], sysroot_value),
            f"glibc {name} sysroot; set {' or '.join(sysroot_names[name])}",
        )
        compiler = slate_clang
        target_triple = target["glibc_target"]
        linker_names = {
            "x86_64": ["SLATE_GLIBC_LINKER_X86_64"],
            "i386": ["SLATE_GLIBC_LINKER_X86", "SLATE_I686_LINKER"],
            "arm": ["SLATE_GLIBC_LINKER_ARM", "SLATE_ARM_LINKER"],
            "aarch64": ["SLATE_GLIBC_LINKER_AARCH64", "SLATE_AARCH64_LINKER"],
        }
        linker_env = next(
            (os.environ[name] for name in linker_names[name] if os.environ.get(name)),
            None,
        )
        if linker_env:
            linker = require_executable(linker_env, f"glibc {name} linker")
            linker_args = [f"--sysroot={sysroot}"]
            linker_post_args = []
        elif name == "x86_64":
            linker = slate_clang
            linker_args = [f"--target={target_triple}", f"--sysroot={sysroot}", "-static"]
            linker_post_args = []
        else:
            raise RuntimeError(
                f"glibc {name} linker is not configured; set SLATE_GLIBC_LINKER_{name.upper()}"
            )

    runner = qemu_for(name)
    runner_args = ["-L", str(sysroot)] if runner else []
    if linker.name == "clang" and not any(arg.startswith("--target=") for arg in linker_args):
        linker_args.insert(0, f"--target={target_triple}")
    if linker.name == "clang" and not any(arg.startswith("--sysroot=") for arg in linker_args):
        linker_args.insert(1, f"--sysroot={sysroot}")
    return {
        "name": name,
        "libc": libc,
        "target": target_triple,
        "sysroot": sysroot,
        "compiler": compiler,
        "compiler_args": target["compiler_args"],
        "linker": linker,
        "linker_args": linker_args,
        "linker_post_args": linker_post_args,
        "runner": runner,
        "runner_args": runner_args,
        "defines": slate_defines(name, libc),
    }


def run_probe(configuration, shim, output):
    command = [
        sys.executable,
        str(PROBE),
        "run",
        "--compiler",
        str(configuration["compiler"]),
        "--target",
        configuration["target"],
        "--sysroot",
        str(configuration["sysroot"]),
        "--linker",
        str(configuration["linker"]),
        "--output",
        str(output),
    ]
    for value in configuration["compiler_args"]:
        command.append(f"--compiler-arg={value}")
    for value in configuration["linker_args"]:
        command.append(f"--linker-arg={value}")
    for value in configuration["linker_post_args"]:
        command.append(f"--linker-post-arg={value}")
    if shim:
        command.extend(["--shim", str(SHIM)])
        for value in configuration["defines"]:
            command.append(f"--define={value}")
    if configuration["runner"]:
        command.extend(["--runner", str(configuration["runner"])])
        for value in configuration["runner_args"]:
            command.extend([f"--runner-arg={value}"])
    result = subprocess.run(command, text=True, capture_output=True)
    if result.returncode:
        detail = (result.stdout + result.stderr).strip()
        raise RuntimeError(detail or "probe execution failed")


def compare(oracle, candidate):
    left = json.loads(oracle.read_text())
    right = json.loads(candidate.read_text())
    if left.get("probe") != right.get("probe"):
        raise RuntimeError("probe names differ")
    if left.get("source_sha256") != right.get("source_sha256"):
        raise RuntimeError("probe sources differ")
    left_records = left["records"]
    right_records = right["records"]
    return [
        (name, left_records.get(name), right_records.get(name))
        for name in sorted(set(left_records) | set(right_records))
    ]


def parser():
    result = argparse.ArgumentParser()
    result.add_argument("--libc", choices=["musl", "glibc", "all"], default="all")
    result.add_argument("--arch", choices=sorted(TARGETS), action="append")
    result.add_argument(
        "--output-root",
        type=Path,
        default=ROOT / "target/libc-abi-matrix",
    )
    result.add_argument("--list", action="store_true")
    return result


def main():
    args = parser().parse_args()
    if args.list:
        for libc in (["musl", "glibc"] if args.libc == "all" else [args.libc]):
            for name in args.arch or sorted(TARGETS):
                print(f"{libc}/{name}")
        return

    slate_clang = require_file(
        env_path("SLATE_CLANG", Path.home() / "llvm-project/build-cir/bin/clang"),
        "SLATE_CLANG",
    )
    musl_root = env_path("SLATE_MUSL_SYSROOT_ROOT", Path.home() / "toolchains/slate-musl")
    names = args.arch or sorted(TARGETS)
    libcs = ["musl", "glibc"] if args.libc == "all" else [args.libc]
    results = []
    failures = []
    record_passes = 0
    record_failures = 0
    for libc in libcs:
        for name in names:
            label = f"{libc}/{name}"
            try:
                configuration = config(name, libc, slate_clang, musl_root)
                directory = args.output_root / libc / name
                directory.mkdir(parents=True, exist_ok=True)
                oracle = directory / "oracle.json"
                candidate = directory / "shim.json"
                run_probe(configuration, False, oracle)
                run_probe(configuration, True, candidate)
                records = compare(oracle, candidate)
                mismatches = [
                    (name, expected, actual)
                    for name, expected, actual in records
                    if expected != actual
                ]
                record_passes += len(records) - len(mismatches)
                record_failures += len(mismatches)
                if mismatches:
                    detail = "\n".join(
                        f"{name}: oracle={expected!r} candidate={actual!r}"
                        for name, expected, actual in mismatches
                    )
                    failures.append((label, detail))
                    print(f"FAIL {label}")
                else:
                    results.append(label)
                    print(f"PASS {label}")
                for name, expected, actual in records:
                    if expected == actual:
                        print(f"  PASS {name}")
                    else:
                        print(
                            f"  FAIL {name}: oracle={expected!r} "
                            f"candidate={actual!r}"
                        )
            except (OSError, RuntimeError) as error:
                failures.append((label, str(error)))
                print(f"FAIL {label} (toolchain)", file=sys.stderr)
                for line in str(error).splitlines():
                    print(f"  {line}", file=sys.stderr)

    manifest = {
        "schema": 1,
        "compiler": str(slate_clang),
        "musl_root": str(musl_root),
        "passed": results,
        "failed": {label: error for label, error in failures},
    }
    args.output_root.mkdir(parents=True, exist_ok=True)
    (args.output_root / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(
        f"SUMMARY targets_passed={len(results)} targets_failed={len(failures)} "
        f"records_passed={record_passes} records_failed={record_failures}"
    )
    if failures:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
