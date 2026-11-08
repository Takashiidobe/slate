#!/usr/bin/env python3
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "crates/slate/src/msvc_simd_table.rs"
X86_FILES = [
    "sse", "sse2", "sse3", "ssse3", "sse41", "sse42", "aes", "pclmulqdq",
    "sha", "avx", "avx2", "fma", "f16c",
]
X86_64_FILES = ["sse", "sse2", "sse41", "sse42", "avx"]
FN = re.compile(r"^pub (?:const )?(?:unsafe )?fn (_mm\w*)\s*(?:<[^>]*>)?\s*\(([^)]*)\)", re.MULTILINE)


def core_arch() -> Path:
    sysroot = subprocess.run(
        ["rustc", "--print", "sysroot"], cwd=ROOT, capture_output=True, text=True, check=True
    ).stdout.strip()
    return Path(sysroot) / "lib/rustlib/src/rust/library/stdarch/crates/core_arch/src"


def names(directory: Path, files: list[str]) -> set[str]:
    found = set()
    for stem in files:
        text = (directory / f"{stem}.rs").read_text()
        text = text.split("#[cfg(test)]\nmod tests")[0]
        for match in FN.finditer(text):
            attrs = re.search(r"((?:^#\[.*\]\n)*)\Z", text[: match.start()], re.MULTILINE)
            if "#[unstable(" not in attrs.group(1) and "&" not in match.group(2):
                found.add(match.group(1))
    return found


def main() -> int:
    src = core_arch()
    shared = names(src / "x86", X86_FILES)
    x64_only = names(src / "x86_64", X86_64_FILES) - shared
    lines = ["const MSVC_SIMD: &[Symbol] = &["]
    for name in sorted(shared):
        lines.append(f'    msvc_intrinsic("{name}", "core::arch::x86::{name}", MSVC_IA32),')
        lines.append(f'    msvc_intrinsic("{name}", "core::arch::x86_64::{name}", MSVC_X64),')
    for name in sorted(x64_only):
        lines.append(f'    msvc_intrinsic("{name}", "core::arch::x86_64::{name}", MSVC_X64),')
    lines.append("];")
    OUT.write_text("\n".join(lines) + "\n")
    print(f"{len(shared)} shared, {len(x64_only)} x86_64-only -> {OUT.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
