#!/usr/bin/env python3

import argparse
import re
import subprocess
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent

CONFIGS = {
    "aarch64": {
        "compiler": "aarch64-linux-gnu-gcc",
        "flags": ["-march=armv9-a"],
        "headers": ["arm_neon.h", "arm_acle.h", "arm_fp16.h", "arm_bf16.h"],
    },
    "arm": {
        "compiler": "arm-none-eabi-gcc",
        "flags": ["-march=armv8.2-a+fp16+bf16+simd+crypto", "-mfloat-abi=hard"],
        "headers": ["arm_neon.h", "arm_acle.h", "arm_fp16.h", "arm_bf16.h"],
    },
    "arm-mve": {
        "compiler": "arm-none-eabi-gcc",
        "flags": ["-march=armv8.1-m.main+mve.fp+fp.dp+cdecp0", "-mfloat-abi=hard"],
        "headers": ["arm_mve.h", "arm_cde.h", "arm_acle.h"],
    },
}

TYPE_NAME = re.compile(
    r"\b__(?:builtin_(?:aarch64_simd|neon)_[a-z0-9]+|simd[0-9]+_[a-z0-9_]+|[A-Z][A-Za-z0-9]*_t|mfp8|fp16|bf16)\b|\b[a-z0-9]+x[0-9]+x[2-4]_t\b"
)


def run(command, **kwargs):
    return subprocess.run(command, check=True, text=True, **kwargs)


def dump(name, config, work):
    compiler = config["compiler"]
    plugin_dir = Path(run([compiler, "-print-file-name=plugin"], capture_output=True).stdout.strip())
    include_dir = Path(run([compiler, "-print-file-name=include"], capture_output=True).stdout.strip())
    plugin = work / name / "dump_builtins.so"
    plugin.parent.mkdir()
    run(
        [
            "g++", "-std=c++17", "-shared", "-fPIC", "-fno-rtti",
            f"-I{plugin_dir}/include", str(HERE / "dump_builtins.cc"), "-o", str(plugin),
        ]
    )
    names = set()
    for header in config["headers"] + ["arm_private_neon_types.h", "arm_private_fp8.h", "arm_mve_types.h"]:
        path = include_dir / header
        if path.exists():
            names.update(TYPE_NAME.findall(path.read_text()))
    names_file = work / f"{name}-types.txt"
    names_file.write_text("\n".join(sorted(names)) + "\n")
    stubs = work / f"{name}-stubs"
    stubs.mkdir()
    (stubs / "stdint.h").write_text("#include <stdint-gcc.h>\n")
    source = work / f"{name}.c"
    source.write_text(
        "".join(f"#include <{header}>\n" for header in config["headers"]) + "int slate_dump_marker;\n"
    )
    result = run(
        [
            compiler, f"-idirafter{stubs}", f"-fplugin={plugin}",
            f"-fplugin-arg-dump_builtins-types={names_file}", *config["flags"],
            "-S", "-o", "/dev/null", str(source),
        ],
        capture_output=True,
    )
    return sorted(set(line for line in result.stdout.splitlines() if line.startswith(("fn\t", "type\t", "tuple\t"))))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output-dir", type=Path, default=HERE / "dumps")
    args = parser.parse_args()
    args.output_dir.mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory() as work:
        work = Path(work)
        for name, config in CONFIGS.items():
            lines = dump(name, config, work)
            (args.output_dir / f"{name}.tsv").write_text("\n".join(lines) + "\n")
            print(f"{name}: {len(lines)} rows")


if __name__ == "__main__":
    main()
