#!/usr/bin/env python3
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "crates/slate/src/frontend/lowerer/sve_table.rs"
GCC_DUMP = ROOT / "crates/slate-parser/tools/gcc_plugin/dumps/aarch64.tsv"
FN = re.compile(r"^pub (?:unsafe )?fn (sv\w+)\s*(?:<([^>]*)>)?\s*\(([^)]*)\)", re.MULTILINE)
IMMEDIATES = {"i32": "I32", "svpattern": "Pattern", "svprfop": "Prefetch"}
TYPE = re.compile(r"\(pub, (?:\w+, )?(?:(sv\w+_t), \d+|sv\w+_t, \d, (sv\w+_t))\)")


def core_arch() -> Path:
    sysroot = subprocess.run(
        ["rustc", "--print", "sysroot"], cwd=ROOT, capture_output=True, text=True, check=True
    ).stdout.strip()
    return Path(sysroot) / "lib/rustlib/src/rust/library/stdarch/crates/core_arch/src/aarch64"


def gcc_arity() -> dict[str, int]:
    arity = {}
    for line in GCC_DUMP.read_text().splitlines():
        fields = line.split("\t")
        if fields[0] != "fn" or not fields[1].startswith("sv"):
            continue
        params = fields[2][fields[2].index("(") + 1 : fields[2].rindex(")")].strip()
        arity[fields[1]] = 0 if params in ("", "void") else params.count(",") + 1
    return arity


def stdarch() -> dict[str, tuple[list[str], int, int]]:
    found = {}
    for module in ("sve", "sve2"):
        for match in FN.finditer((core_arch() / module / "generated.rs").read_text()):
            name, generics, params = match.groups()
            immediates, inferred = [], 0
            for generic in filter(None, (g.strip() for g in (generics or "").split(","))):
                if generic.startswith("const "):
                    immediates.append(generic.split(":")[1].strip())
                else:
                    inferred += 1
            count = len([p for p in params.split(",") if p.strip()])
            found[name] = (immediates, inferred, count)
    return found


def main() -> int:
    arity = gcc_arity()
    lines = [
        "use super::sve::{SveImmediate::*, SveIntrinsic};",
        "",
        "pub(super) static SVE_INTRINSICS: &[SveIntrinsic] = &[",
    ]
    kept = skipped = 0
    for name, (immediates, inferred, count) in sorted(stdarch().items()):
        kinds = [IMMEDIATES.get(kind) for kind in immediates]
        if name not in arity or None in kinds or arity[name] != count + len(kinds):
            skipped += 1
            continue
        kept += 1
        lines.append(
            f'    SveIntrinsic {{ name: "{name}", immediates: &[{", ".join(kinds)}], inferred: {inferred} }},'
        )
    lines.append("];")
    types = sorted(
        {a or b for a, b in TYPE.findall((core_arch() / "sve" / "mod.rs").read_text())}
    )
    lines += ["", "pub(super) static SVE_TYPES: &[&str] = &["]
    lines += [f'    "{name}",' for name in types]
    lines.append("];")
    OUT.write_text("\n".join(lines) + "\n")
    print(f"{kept} sv intrinsics, {skipped} skipped, {len(types)} types -> {OUT.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
