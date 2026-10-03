#!/usr/bin/env python3

import argparse
import concurrent.futures
import json
import re
import subprocess
import tempfile
from pathlib import Path

TARGET_64 = "x86_64-unknown-linux-gnu"
TARGET_32 = "i686-unknown-linux-gnu"

EXTRA_IMPLIES = {
    "amx-avx512": ["avx10.2"],
}
SGX_CPUS = [
    "alderlake", "arrowlake", "arrowlake-s", "cannonlake", "clearwaterforest", "diamondrapids",
    "emeraldrapids", "gracemont", "graniterapids", "graniterapids-d", "icelake-client",
    "icelake-server", "lunarlake", "meteorlake", "novalake", "pantherlake", "raptorlake",
    "sapphirerapids", "sierraforest", "skylake", "tigerlake", "tremont",
]
CPU_OVERRIDES = {
    **{cpu: (["sgx"], []) for cpu in SGX_CPUS},
    "tigerlake": (["sgx", "kl", "widekl"], []),
    "amdfam10": (["mmx"], []),
    "knl": (["invpcid"], []),
    "knm": (["invpcid"], []),
    "k8-sse3": ([], ["cx16"]),
}

MATH_MACROS = {"__SSE_MATH__", "__SSE2_MATH__"}
NAMED_CPUS = {
    "PENTIUM4": "pentium4",
    "X86_64": "x86-64",
    "X86_64_V2": "x86-64-v2",
    "X86_64_V3": "x86-64-v3",
    "X86_64_V4": "x86-64-v4",
}
TARGET_MACROS = re.compile(r"__FLT16_\w+|__GCC_HAVE_SYNC_COMPARE_AND_SWAP_[1248]")


def tablegen(llvm_tblgen, llvm_project, source, includes):
    with tempfile.NamedTemporaryFile(suffix=".json") as out:
        flags = [f"-I{llvm_project / include}" for include in includes]
        subprocess.run(
            [llvm_tblgen, "--dump-json", *flags, str(llvm_project / source), "-o", out.name],
            check=True,
        )
        return json.loads(Path(out.name).read_text())


def defined_macros(clang, target, args):
    result = subprocess.run(
        [clang, f"--target={target}", *args, "-dM", "-E", "-x", "c", "/dev/null"],
        capture_output=True,
        text=True,
    )
    if result.returncode:
        return None
    names = (line.split()[1] for line in result.stdout.splitlines() if line.startswith("#define "))
    return {name for name in names if not TARGET_MACROS.fullmatch(name)}


def variant(spelling):
    name = "".join(part[:1].upper() + part[1:] for part in re.split(r"[.-]", spelling))
    return name if name[:1].isalpha() else f"X{name}"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--llvm-tblgen", type=Path, required=True)
    parser.add_argument("--llvm-project", type=Path, required=True)
    parser.add_argument("--clang", default="clang")
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    x86 = tablegen(args.llvm_tblgen, args.llvm_project, "llvm/lib/Target/X86/X86.td",
                   ["llvm/include", "llvm/lib/Target/X86"])
    options = tablegen(args.llvm_tblgen, args.llvm_project, "clang/include/clang/Options/Options.td",
                       ["llvm/include", "clang/include", "clang/include/clang/Options"])

    records = {
        name: x86[name]
        for name in x86["!instanceof"]["SubtargetFeature"]
        if not name.startswith("Tuning")
    }
    by_spelling = {record["Name"]: name for name, record in records.items()}
    flags = sorted(
        {
            options[name]["Name"][1:]
            for name in options["!instanceof"]["Option"]
            if (group := options[name].get("Group")) and group["def"] == "m_x86_Features_Group"
            and not options[name]["Name"].startswith("mno-")
            and not options[name]["Name"].endswith("=")
        }
    )
    features = [flag for flag in flags if flag in by_spelling]
    skipped = [flag for flag in flags if flag not in by_spelling]
    if len(features) > 128:
        raise SystemExit(f"{len(features)} features do not fit the u128 feature set")
    index = {feature: position for position, feature in enumerate(features)}
    variants = [variant(feature) for feature in features]
    if len(set(variants)) != len(variants):
        raise SystemExit("feature variant names collide")

    def implied(feature):
        record = records[by_spelling[feature]]
        direct = [records[i["def"]]["Name"] for i in record["Implies"] if i["def"] in records]
        return sorted({name for name in direct + EXTRA_IMPLIES.get(feature, []) if name in index})

    def closure(spellings):
        seen, pending = set(), list(spellings)
        while pending:
            spelling = pending.pop()
            if spelling in seen or spelling not in by_spelling:
                continue
            seen.add(spelling)
            record = records[by_spelling[spelling]]
            pending += [records[i["def"]]["Name"] for i in record["Implies"] if i["def"] in records]
            pending += EXTRA_IMPLIES.get(spelling, [])
        return seen & set(index)

    processors = {x86[name]["Name"]: x86[name] for name in x86["!instanceof"]["Processor"]}

    with concurrent.futures.ThreadPoolExecutor(32) as pool:
        base_64 = defined_macros(args.clang, TARGET_64, ["-march=x86-64"])
        base_32 = defined_macros(args.clang, TARGET_32, ["-march=i386"])
        probes_64 = dict(zip(features, pool.map(
            lambda f: defined_macros(args.clang, TARGET_64, ["-march=x86-64", f"-m{f}"]), features)))
        probes_32 = dict(zip(features, pool.map(
            lambda f: defined_macros(args.clang, TARGET_32, ["-march=i386", f"-m{f}"]), features)))
        cpus_64 = dict(zip(processors, pool.map(
            lambda c: defined_macros(args.clang, TARGET_64, [f"-march={c}"]), processors)))
        cpus_32 = dict(zip(processors, pool.map(
            lambda c: defined_macros(args.clang, TARGET_32, [f"-march={c}"]), processors)))

    deltas = {}
    for feature in features:
        delta = (probes_64[feature] or set()) - base_64
        if not delta and probes_32[feature] is not None:
            delta = probes_32[feature] - base_32
        deltas[feature] = delta - MATH_MACROS
    owned = {
        feature: sorted(
            delta - set().union(*[other for name, other in deltas.items() if name != feature and other < delta])
        )
        for feature, delta in deltas.items()
    }
    feature_macros = set().union(*map(set, owned.values())) | MATH_MACROS

    common_64 = set.intersection(*[m - feature_macros for m in cpus_64.values() if m is not None])
    common_32 = set.intersection(*[m - feature_macros for m in cpus_32.values() if m is not None])
    cpus = []
    for name, record in sorted(processors.items()):
        long_mode = cpus_64[name] is not None
        macros = cpus_64[name] - common_64 if long_mode else (cpus_32[name] or set()) - common_32
        if not long_mode and cpus_32[name] is None:
            print(f"clang rejects -march={name}; skipped")
            continue
        listed = [x86[f["def"]]["Name"] for f in record["Features"] if f["def"] in records]
        added, removed = CPU_OVERRIDES.get(name, ([], []))
        enabled = closure(listed + added) - set(removed)
        cpus.append((name, long_mode, enabled, macros - feature_macros))
    unknown = set(CPU_OVERRIDES) - set(processors)
    if unknown:
        raise SystemExit(f"CPU_OVERRIDES names unknown CPUs: {', '.join(sorted(unknown))}")
    long_mode_only = [feature for feature in features if probes_32[feature] is None]

    llvm_commit = subprocess.run(
        ["git", "-C", str(args.llvm_project), "rev-parse", "--short=12", "HEAD"],
        capture_output=True, text=True, check=True,
    ).stdout.strip()
    clang_version = subprocess.run([args.clang, "--version"], capture_output=True, text=True,
                                   check=True).stdout.splitlines()[0]

    def bits(spellings):
        return f"0x{sum(1 << index[s] for s in spellings):032x}"

    lines = [
        f"// generated by tools/generate_x86_isa.py from llvm-project {llvm_commit} ({clang_version});",
        "// do not edit",
        "",
        "#[derive(Debug, Clone, Copy, PartialEq, Eq)]",
        "pub enum X86Feature {",
        *[f"    {name}," for name in variants],
        "}",
        "",
        f"pub(super) const ALL_FEATURES: [X86Feature; {len(features)}] = [",
        *[f"    X86Feature::{name}," for name in variants],
        "];",
        "",
        "impl X86Feature {",
        "    pub(super) fn spelling(self) -> &'static str {",
        "        match self {",
        *[f'            Self::{name} => "{feature}",' for name, feature in zip(variants, features)],
        "        }",
        "    }",
        "",
        "    pub(super) fn macros(self) -> &'static [&'static str] {",
        "        match self {",
        *[
            f"            Self::{name} => &[{', '.join(f'"{m}"' for m in owned[feature])}],"
            for name, feature in zip(variants, features)
        ],
        "        }",
        "    }",
        "",
        "    pub(super) fn clang_implies(self) -> u128 {",
        "        match self {",
        *[f"            Self::{name} => {bits(implied(feature))}," for name, feature in zip(variants, features)],
        "        }",
        "    }",
        "",
        "    pub(super) fn long_mode_only(self) -> bool {",
        "        matches!(",
        "            self,",
        "            " + " | ".join(f"Self::{variants[index[f]]}" for f in long_mode_only),
        "        )",
        "    }",
        "}",
        "",
        "pub(super) struct X86Cpu {",
        "    pub(super) name: &'static str,",
        "    pub(super) long_mode: bool,",
        "    pub(super) features: u128,",
        "    pub(super) macros: &'static [&'static str],",
        "}",
        "",
        f"pub(super) const CPUS: [X86Cpu; {len(cpus)}] = [",
    ]
    for name, long_mode, enabled, macros in cpus:
        listed = ", ".join(f'"{m}"' for m in sorted(macros))
        lines += [
            "    X86Cpu {",
            f'        name: "{name}",',
            f"        long_mode: {str(long_mode).lower()},",
            f"        features: {bits(enabled)},",
            f"        macros: &[{listed}],",
            "    },",
        ]
    lines += ["];", ""]
    positions = {name: position for position, (name, *_) in enumerate(cpus)}
    for constant, name in NAMED_CPUS.items():
        if name not in positions:
            raise SystemExit(f"-march={name} is missing from the CPU table")
        lines.append(f"pub(super) const {constant}: usize = {positions[name]};")
    args.output.write_text("\n".join(lines) + "\n")
    print(f"{len(features)} features, {len(cpus)} cpus; flags without an X86.td feature: {', '.join(skipped)}")


if __name__ == "__main__":
    main()
