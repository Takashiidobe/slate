import os
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TRIPLE = "x86_64-unknown-linux-gnu"
MULTIARCH = "x86_64-linux-gnu"
DEFINES = [
    "-D__SLATE_ARCH_X86_64",
    "-D__SLATE_VENDOR_UNKNOWN",
    "-D__SLATE_KERNEL_LINUX",
    "-D__SLATE_LIBC_GLIBC",
    "-D__SLATE_OBJ_ELF",
    "-D__SLATE_WORDSIZE_64",
    "-D__SLATE_ENDIAN_LITTLE",
]
TARGETS = {
    "crates/slate/compile_flags.txt": ["-std=c23", *DEFINES[:4], "-D__SLATE_GLIBC_MINOR__=43", *DEFINES[4:]],
    "crates/slate/tests/fixtures.chibicc/compile_flags.txt": ["-std=gnu11", *DEFINES],
}


def data_dir():
    base = os.environ.get("XDG_DATA_HOME") or Path.home() / ".local/share"
    return Path(base) / "slate"


def env_dir(var, default):
    value = os.environ.get(var)
    return Path(value) if value else default


def latest_clang_headers(root):
    def version(path):
        return [int(part) for part in re.findall(r"\d+", path.name)]

    profiles = [p for p in root.glob("clang-*") if (p / "include").is_dir()]
    if not profiles:
        raise SystemExit(f"no clang-* compiler headers under {root}")
    return max(profiles, key=version) / "include"


def include_dirs():
    headers = latest_clang_headers(env_dir("SLATE_COMPILER_HEADERS", data_dir() / "compiler-headers"))
    sysroot = env_dir("SLATE_SYSROOTS", data_dir() / "sysroots") / TRIPLE
    candidates = [
        sysroot / "SDK/usr/include",
        sysroot / "usr/include",
        sysroot / "include",
        sysroot / "usr" / MULTIARCH / "include",
    ]
    dirs = [headers, *(c for c in candidates if c.is_dir())]
    if len(dirs) == 1:
        raise SystemExit(f"no sysroot at {sysroot}; run: cargo run -p slate-c2rust -- sysroot install {TRIPLE}")
    return dirs


def main():
    dirs = include_dirs()
    isystem = [flag for d in dirs for flag in ("-isystem", str(d))]
    for rel, flags in TARGETS.items():
        path = ROOT / rel
        path.write_text("\n".join([flags[0], "-nostdinc", *isystem, *flags[1:]]) + "\n")
        print(f"wrote {rel}")


main()
