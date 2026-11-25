#!/usr/bin/env python3

import argparse
from bisect import bisect_right
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
import json
import os
from pathlib import Path, PureWindowsPath
import re
import subprocess
import tempfile
from urllib.request import urlopen

from generate_clang_builtins import prototype_lines, table_lines


ROOT = Path(__file__).resolve().parents[3]
PARSER = ROOT / "crates/slate-parser"
DOCS = "https://learn.microsoft.com/en-us/cpp/intrinsics/"
ALPHABETICAL = "alphabetical-listing-of-intrinsic-functions"
TARGETS = {
    "x86": "i686-pc-windows-msvc",
    "x64": "x86_64-pc-windows-msvc",
    "arm": "thumbv7a-pc-windows-msvc",
    "arm64": "aarch64-pc-windows-msvc",
}
ENTRY = re.compile(
    r"^\[((?:\x60\w+\x60(?:, )?)+)( intrinsic functions)?\]\(([\w-]+)\)", re.M
)
SPELLINGS = {"__mul128": "_mul128"}


def document(page, documents):
    if documents is not None:
        return (documents / f"{page}.md").read_text()
    with urlopen(f"{DOCS}{page}?accept=text/markdown", timeout=30) as response:
        return response.read().decode().replace("\r\n", "\n")


def alphabetical_entries(listing):
    entries = []
    for spellings, family, page in ENTRY.findall(listing):
        for spelling in re.findall(r"\x60(\w+)\x60", spellings):
            entries.append((spelling, bool(family), page))
    if not entries:
        raise ValueError("the alphabetical list contains no intrinsic entries")
    return entries


def documented_name(name):
    return SPELLINGS.get(name, name)


def family_members(base, documentation, declarations):
    suffix = r"(?:8|16|32|64|128)?(?:_(?:acq|rel|nf|np|HLEAcquire|HLERelease))?"
    documented = set(re.findall(rf"\b{re.escape(base)}{suffix}\b", documentation))
    declared = {
        name for name in declarations if re.fullmatch(re.escape(base) + suffix, name)
    }
    return documented | declared | {base}


def oracle(arch, source, *flags):
    environment = dict(os.environ, MSVC_ARCH=arch, WINEDEBUG="-all")
    return subprocess.run(
        [
            str(PARSER / "tools/cl.exe"),
            "/nologo",
            "/TC",
            "/Zc:preprocessor",
            *flags,
            str(source),
        ],
        env=environment,
        capture_output=True,
        text=True,
    )


def compiler_output(result):
    if result.returncode:
        raise RuntimeError(result.stdout + result.stderr)
    return result.stdout


def type_spelling(text):
    text = text.replace("_Bool", "bool")
    text = re.sub(r"\bshort int\b", "short", text)
    return re.sub(
        r"\b(?:long long|long)\b(?! (?:int|long|double)\b)", r"\g<0> int", text
    )


def header_declarations(arch, clang, directory):
    source = directory / f"{arch}.c"
    source.write_text("#include <intrin.h>\nSLATE_MSC_VERSION _MSC_FULL_VER\n")
    text, version = compiler_output(oracle(arch, source, "/E")).rsplit(
        "SLATE_MSC_VERSION ", 1
    )
    result = subprocess.run(
        [
            clang,
            f"--target={TARGETS[arch]}",
            "-x",
            "c",
            "-fms-extensions",
            "-w",
            "-Xclang",
            "-ast-dump=json",
            "-fsyntax-only",
            "-",
        ],
        input=text,
        capture_output=True,
        text=True,
    )
    if result.returncode:
        raise RuntimeError(result.stderr)
    lines = list(re.finditer(r'^#line \d+ "([^"]+)"', text, re.M))
    offsets = [line.start() for line in lines]
    declarations = {}
    for item in json.loads(result.stdout)["inner"]:
        location = item["loc"] if item.get("kind") == "FunctionDecl" else {}
        if "offset" not in location or item.get("isImplicit"):
            continue
        position = bisect_right(offsets, location["offset"])
        if position == 0:
            continue
        header = PureWindowsPath(lines[position - 1][1]).name.lower()
        if not header.endswith(".h"):
            continue
        children = item.get("inner", [])
        result_type, separator, _ = item["type"]["qualType"].partition(" (")
        if not separator:
            continue
        parameters = [
            type_spelling(child["type"]["qualType"])
            for child in children
            if child["kind"] == "ParmVarDecl"
        ]
        result_type = type_spelling(result_type)
        body = any(child["kind"] == "CompoundStmt" for child in children)
        if body or item["name"] not in declarations:
            declarations[item["name"]] = (header, result_type, parameters, body)
    return version.strip(), declarations


def pragma_intrinsics(arch, names, directory):
    source = directory / f"{arch}-pragmas.c"
    source.write_text(
        "#include <intrin.h>\n" + "".join(f"#pragma intrinsic({n})\n" for n in names)
    )
    result = oracle(arch, source, "/W4", "/Zs")
    rejected = set(
        re.findall(r"warning C4163: '(\w+)'", compiler_output(result) + result.stderr)
    )
    return set(names) - rejected


def lowering(slate, arch, name, result_type, parameters, directory):
    if slate is None:
        return "unprobed"
    source = directory / f"{arch}-{name}.c"
    declarations = ", ".join(parameters) or "void"
    arguments = ", ".join(f"arg{i}" for i in range(len(parameters)))
    bindings = ", ".join(f"{ty} arg{i}" for i, ty in enumerate(parameters)) or "void"
    prefix = "" if result_type == "void" else "return "
    source.write_text(
        f"#include <intrin.h>\n{result_type} {name}({declarations});\n"
        f"{result_type} slate_probe({bindings}) {{ {prefix}{name}({arguments}); }}\n"
    )
    args = [
        "--flavor=msvc",
        f"--target={TARGETS[arch]}",
        "/Zc:preprocessor",
        str(source),
    ]
    result = subprocess.run(
        [str(slate), "lowering-barriers", *args], capture_output=True, text=True
    )
    if "builtin without a lowering" in result.stdout:
        return "missing"
    if result.returncode:
        return "other barrier" if result.stdout.strip() else "probe failed"
    result = subprocess.run(
        [str(slate), "translate", *args], capture_output=True, text=True
    )
    if result.returncode:
        return "probe failed"
    declaration = rf"(?m)^\s*fn {re.escape(name)}\([^;\n]*;"
    if re.search(declaration, result.stdout) and re.search(
        rf"\b{re.escape(name)}\(", re.sub(declaration, "", result.stdout)
    ):
        return "extern call"
    return "lowered"


def generate(args, directory):
    listing = document(ALPHABETICAL, args.documents)
    entries = alphabetical_entries(listing)
    family_pages = sorted({page for _, family, page in entries if family})
    with ThreadPoolExecutor() as pool:
        docs = dict(
            zip(
                family_pages,
                pool.map(lambda page: document(page, args.documents), family_pages),
            )
        )
        targets = dict(
            zip(
                args.arch,
                pool.map(
                    lambda arch: header_declarations(arch, args.clang, directory),
                    args.arch,
                ),
            )
        )
    declared_names = set().union(
        *(declarations for _, declarations in targets.values())
    )
    candidates = defaultdict(set)
    for spelling, family, page in entries:
        names = (
            family_members(spelling, docs[page], declared_names)
            if family
            else {spelling}
        )
        for name in names:
            candidates[documented_name(name)].add((spelling, page))

    report: dict[str, dict[str, dict[str, str]]] = {
        name: {} for name in sorted(candidates)
    }
    tables = {}
    lowerings = {}
    for arch in TARGETS:
        if arch not in targets:
            for row in report.values():
                row[arch] = {"recognition": "unverified target"}
            continue
        _, headers = targets[arch]
        probes = pragma_intrinsics(
            arch, sorted(candidates.keys() & headers.keys()), directory
        )
        rows = []
        for name, row in report.items():
            if name not in headers:
                row[arch] = {"recognition": "no declaration"}
                continue
            header, result_type, parameters, body = headers[name]
            signature = f"{result_type}({', '.join(parameters)})"
            status = "builtin" if name in probes else "not intrinsic by pragma"
            if body:
                status = "header definition"
            row[arch] = {
                "recognition": status,
                "header": header,
                "signature": signature,
            }
            if status == "builtin":
                rows.append(
                    (
                        name,
                        name,
                        signature,
                        "Builtin",
                        [],
                        "ALL_MS_LANGUAGES",
                        "intrin.h",
                        None,
                    )
                )
                lowerings[(arch, name)] = (result_type, parameters)
        tables[arch] = rows

    versions = {arch: version for arch, (version, _) in targets.items()}
    lines = [
        "// generated by tools/generate_msvc_builtins.py",
        f"// _MSC_FULL_VER: {versions}",
    ]
    prototypes = {}
    for i, signature in enumerate(
        sorted({row[2] for rows in tables.values() for row in rows})
    ):
        prototypes[signature] = f"MSVC_PROTOTYPE_{i:03}"
        lines.extend(prototype_lines(prototypes[signature], signature))
        lines.append("")
    typedefs = sorted(
        set(re.findall(r'BuiltinType::Opaque\("(\w+)"\)', "\n".join(lines)))
    )
    lines.append("pub(super) static MSVC_BUILTIN_TYPEDEFS: &[&str] = &[")
    lines.extend(f"    {json.dumps(name)}," for name in typedefs)
    lines.extend(["];", ""])
    for arch in TARGETS:
        lines.extend(
            table_lines(
                f"MSVC_{arch.upper()}_BUILTINS", tables.get(arch, []), prototypes
            )
        )
    args.output.write_text("\n".join(lines) + "\n")

    def check(item):
        (arch, name), (result_type, parameters) = item
        return (
            arch,
            name,
            lowering(args.slate, arch, name, result_type, parameters, directory),
        )

    with ThreadPoolExecutor(max_workers=8) as pool:
        for arch, name, status in pool.map(check, lowerings.items()):
            report[name][arch]["lowering"] = status
    revisions = {}
    for page, text in [(ALPHABETICAL, listing), *docs.items()]:
        revision = re.search(r"^git_commit_id: (\w+)", text, re.M)
        revisions[page] = revision[1] if revision else None
    provenance = {
        "listing": DOCS + ALPHABETICAL,
        "revisions": revisions,
        "_MSC_FULL_VER": versions,
    }
    args.inventory.write_text(
        json.dumps(provenance)
        + "\n"
        + "".join(
            json.dumps(
                {"name": name, "sources": sorted(candidates[name]), "targets": row}
            )
            + "\n"
            for name, row in report.items()
        )
    )


def main():
    parser = argparse.ArgumentParser(
        description="Generate MSVC builtins from the alphabetical listing"
    )
    parser.add_argument(
        "--documents", type=Path, help="directory of Microsoft Learn Markdown snapshots"
    )
    parser.add_argument("--clang", default="clang")
    parser.add_argument(
        "--arch", choices=TARGETS, action="append", help="default: x86, x64, arm64"
    )
    parser.add_argument(
        "--slate", type=Path, help="slate binary used to measure lowering coverage"
    )
    parser.add_argument(
        "--output", type=Path, default=PARSER / "src/sema/msvc_builtins.rs"
    )
    parser.add_argument(
        "--inventory",
        type=Path,
        default=PARSER / "tools/msvc_intrinsics_alphabetical.jsonl",
    )
    args = parser.parse_args()
    args.arch = args.arch or ["x86", "x64", "arm64"]
    with tempfile.TemporaryDirectory(prefix="slate-msvc-builtins-") as temporary:
        generate(args, Path(temporary))


if __name__ == "__main__":
    main()
