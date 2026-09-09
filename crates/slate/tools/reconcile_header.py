#!/usr/bin/env python3
"""Generalized libc-shim header reconciliation for the declaration-matrix
workflow (wiki/concepts/linux-libc-header-matrix-handoff.md).

For a header:
  1. probes all eight glibc/musl x {x86_64,i386,arm,aarch64} oracles via the
     existing `emit_oracle_header_macros` ignored test to find which
     descriptors actually expose the header, and with what macro values;
  2. appends the header to each applicable manifest and runs the real
     declaration-matrix fixture generation + check to get the
     compiler-verified missing-macro list per descriptor (ground truth, not
     a guess from diffing headers);
  3. groups each missing macro by (descriptor-presence pattern, value) and
     derives the narrowest `__SLATE_LIBC_*`/`__SLATE_ARCH_*`/
     `__SLATE_WORDSIZE_*` guard for that pattern;
  4. anchors each group's insertion point after the nearest preceding macro
     that already exists in the shim header, using the real oracle source
     file (from the probe's `definition_file`) to get true declaration
     order;
  5. writes the merged header in place.

A macro whose value differs across descriptors that all lack it is a real
conflict (needs `#if` branching inside the guard, not a flat block) and is
never auto-inserted -- it is reported for manual reconciliation instead.
Only plain, single-line `#define NAME VALUE` macros are handled; anything
else (multi-line continuations, types, decls, structs) is out of scope and
must go through the existing manual per-header workflow.

Usage:
    python3 tools/reconcile_header.py [--jobs=N] <header> [<header> ...]
"""

import json
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
SHIM_INCLUDE = REPO / "libc-shim" / "include"

LIBCS = ["glibc", "musl"]
ARCHES = ["x86_64", "i386", "arm", "aarch64"]
DESCRIPTORS = [f"{libc}-{arch}" for libc in LIBCS for arch in ARCHES]

ENV = dict(os.environ)
ENV.setdefault("SLATE_CLANG", str(Path.home() / "llvm-project/build-cir/bin/clang"))
ENV.setdefault("SLATE_CIR_OPT", str(Path.home() / "llvm-project/build-cir/bin/cir-opt"))


def run(cmd, env=None, cwd=REPO):
    return subprocess.run(
        cmd, cwd=cwd, env=env or ENV, capture_output=True, text=True, shell=False
    )


def header_dir(header):
    return header.replace("/", "_").replace(".", "_")


def probe_key(header):
    return header_dir(header)


def emit_oracle_macros(header, libc, arch):
    env = dict(ENV)
    env["SLATE_LIBC_DECL_LIBC"] = libc
    env["SLATE_LIBC_DECL_ARCH"] = arch
    env["SLATE_LIBC_DECL_HEADER"] = header
    result = run(
        [
            "cargo",
            "nextest",
            "r",
            "--release",
            "--profile",
            "libc",
            "--test",
            "libc_declaration_probe",
            "-E",
            "test(emit_oracle_header_macros)",
            "--run-ignored",
            "ignored-only",
        ],
        env=env,
    )
    if "1 passed" not in result.stdout and "1 passed" not in result.stderr:
        return None
    manifest_path = (
        REPO
        / "target/libc-declaration-probe"
        / f"{libc}-{libc}-{arch}-{probe_key(header)}"
        / "oracle-macros.json"
    )
    if not manifest_path.exists():
        return None
    with open(manifest_path) as f:
        entries = json.load(f)
    return {e["name"]: e for e in entries if not e["private"]}


def add_to_manifest(descriptor, header):
    path = REPO / f"tests/fixtures.libc-static-test/{descriptor}/headers.txt"
    lines = [l.strip() for l in path.read_text().splitlines() if l.strip()]
    if header in lines:
        return False
    with open(path, "a") as f:
        f.write(f"{header}\n")
    return True


def remove_from_manifest(descriptor, header):
    path = REPO / f"tests/fixtures.libc-static-test/{descriptor}/headers.txt"
    lines = [l for l in path.read_text().splitlines() if l.strip() != header]
    path.write_text("\n".join(lines) + "\n")


def generate_fixture(descriptor):
    env = dict(ENV)
    env["SLATE_LIBC_DECL_TARGET"] = descriptor
    return run(
        [
            "cargo",
            "nextest",
            "r",
            "--release",
            "--profile",
            "libc",
            "--test",
            "libc_declaration_matrix_suite",
            "-E",
            "test(generate_declaration_matrix_fixtures)",
            "--run-ignored",
            "ignored-only",
        ],
        env=env,
    )


def run_matrix(descriptor):
    env = dict(ENV)
    env["SLATE_LIBC_DECL_TARGET"] = descriptor
    return run(
        [
            "cargo",
            "nextest",
            "r",
            "--release",
            "--profile",
            "libc",
            "--test",
            "libc_declaration_matrix_suite",
            "-E",
            "test(declaration_matrices)",
        ],
        env=env,
    )


def worker_count(jobs, work_items):
    return max(1, min(jobs, work_items))


def discover_headers(headers, jobs):
    tasks = [(header, descriptor) for header in headers for descriptor in DESCRIPTORS]

    def discover(task):
        header, descriptor = task
        libc, arch = descriptor.split("-", 1)
        return header, descriptor, emit_oracle_macros(header, libc, arch)

    oracle = {header: {} for header in headers}
    with ThreadPoolExecutor(max_workers=worker_count(jobs, len(tasks))) as executor:
        for header, descriptor, macros in executor.map(discover, tasks):
            if macros is not None:
                oracle[header][descriptor] = macros
    return oracle


def matrix_output_for(descriptor, headers):
    generate_fixture(descriptor)
    result = run_matrix(descriptor)
    output = result.stdout + result.stderr
    missing = {header: set() for header in headers}
    for header, name in MISSING_MACRO_RE.findall(output):
        if header in missing:
            missing[header].add(name)
    remainder = MISSING_MACRO_RE.sub("", output)
    other = {}
    if result.returncode != 0 and not (
        any(missing.values()) and "error" not in remainder.replace("#error", "")
    ):
        tail = "\n".join(line for line in output.splitlines() if line.strip())[-4000:]
        affected = [
            header
            for header in headers
            if re.search(rf"^\s*{re.escape(header)}:", output, re.M)
        ]
        if not affected:
            affected = headers
        other = {header: [tail] for header in affected}
    return descriptor, missing, other


def run_descriptor_jobs(headers, oracle, jobs):
    descriptor_headers = {
        descriptor: [header for header in headers if descriptor in oracle[header]]
        for descriptor in DESCRIPTORS
    }
    descriptor_headers = {
        descriptor: listed
        for descriptor, listed in descriptor_headers.items()
        if listed
    }
    missing = {header: {} for header in headers}
    other_errors = {header: {} for header in headers}

    def run_descriptor(item):
        descriptor, listed = item
        return matrix_output_for(descriptor, listed)

    with ThreadPoolExecutor(
        max_workers=worker_count(jobs, len(descriptor_headers))
    ) as executor:
        for descriptor, found, errors in executor.map(
            run_descriptor, descriptor_headers.items()
        ):
            for header, names in found.items():
                missing[header][descriptor] = names
            for header, error in errors.items():
                other_errors[header][descriptor] = error
    return missing, other_errors


def validate_descriptors(descriptors, jobs):
    with ThreadPoolExecutor(max_workers=worker_count(jobs, len(descriptors))) as executor:
        results = list(executor.map(run_matrix, descriptors))
    failures = []
    for descriptor, result in zip(descriptors, results):
        if result.returncode:
            output = result.stdout + result.stderr
            failures.append((descriptor, output[-4000:]))
    return failures


MISSING_MACRO_RE = re.compile(r'"([^"]+\.h):([A-Za-z_][A-Za-z0-9_]*) macro is missing')


def missing_macros_for(descriptor, header):
    generate_fixture(descriptor)
    result = run_matrix(descriptor)
    output = result.stdout + result.stderr
    if result.returncode == 0:
        return set(), []
    names = set()
    for hdr, name in MISSING_MACRO_RE.findall(output):
        if hdr == header:
            names.add(name)
    remainder = MISSING_MACRO_RE.sub("", output)
    if names and "error" not in remainder.replace("#error", ""):
        return names, []
    tail = "\n".join(l for l in output.splitlines() if l.strip())[-4000:]
    return names, [tail]


GUARD_ATOMS = {
    "glibc-x86_64": ("__SLATE_LIBC_GLIBC", "__SLATE_ARCH_X86_64"),
    "glibc-i386": ("__SLATE_LIBC_GLIBC", "__SLATE_ARCH_X86"),
    "glibc-arm": ("__SLATE_LIBC_GLIBC", "__SLATE_ARCH_ARM"),
    "glibc-aarch64": ("__SLATE_LIBC_GLIBC", "__SLATE_ARCH_AARCH64"),
    "musl-x86_64": ("__SLATE_LIBC_MUSL", "__SLATE_ARCH_X86_64"),
    "musl-i386": ("__SLATE_LIBC_MUSL", "__SLATE_ARCH_X86"),
    "musl-arm": ("__SLATE_LIBC_MUSL", "__SLATE_ARCH_ARM"),
    "musl-aarch64": ("__SLATE_LIBC_MUSL", "__SLATE_ARCH_AARCH64"),
}

ALL8 = frozenset(DESCRIPTORS)
GLIBC4 = frozenset(d for d in DESCRIPTORS if d.startswith("glibc"))
MUSL4 = frozenset(d for d in DESCRIPTORS if d.startswith("musl"))
WORD32 = frozenset(d for d in DESCRIPTORS if d.endswith("i386") or d.endswith("arm"))
WORD64 = frozenset(d for d in DESCRIPTORS if d.endswith("x86_64") or d.endswith("aarch64"))


ARCH_MACRO = {
    "x86_64": "__SLATE_ARCH_X86_64",
    "i386": "__SLATE_ARCH_X86",
    "arm": "__SLATE_ARCH_ARM",
    "aarch64": "__SLATE_ARCH_AARCH64",
}


def arch_of(descriptor):
    return descriptor.split("-", 1)[1]


def guard_for(pattern):
    pattern = frozenset(pattern)
    if pattern == ALL8:
        return None
    if pattern == GLIBC4:
        return "defined(__SLATE_LIBC_GLIBC)"
    if pattern == MUSL4:
        return "defined(__SLATE_LIBC_MUSL)"
    if pattern == WORD32:
        return "defined(__SLATE_WORDSIZE_32)"
    if pattern == WORD64:
        return "defined(__SLATE_WORDSIZE_64)"
    if pattern == GLIBC4 & WORD32:
        return "defined(__SLATE_LIBC_GLIBC) && defined(__SLATE_WORDSIZE_32)"
    if pattern == GLIBC4 & WORD64:
        return "defined(__SLATE_LIBC_GLIBC) && defined(__SLATE_WORDSIZE_64)"
    if pattern == MUSL4 & WORD32:
        return "defined(__SLATE_LIBC_MUSL) && defined(__SLATE_WORDSIZE_32)"
    if pattern == MUSL4 & WORD64:
        return "defined(__SLATE_LIBC_MUSL) && defined(__SLATE_WORDSIZE_64)"
    for libc4, libc_macro in ((GLIBC4, "__SLATE_LIBC_GLIBC"), (MUSL4, "__SLATE_LIBC_MUSL")):
        if pattern <= libc4 and len(pattern) > 1:
            archs = sorted({arch_of(d) for d in pattern})
            arch_defs = " || ".join(f"defined({ARCH_MACRO[a]})" for a in archs)
            return f"defined({libc_macro}) && ({arch_defs})"
    clauses = []
    for d in sorted(pattern):
        libc_macro, arch_macro = GUARD_ATOMS[d]
        clauses.append(f"(defined({libc_macro}) && defined({arch_macro}))")
    return " || ".join(clauses)


def clean_value(replacement):
    return replacement.strip()


def find_definition_order(path, names_of_interest):
    order = {}
    if not path or not os.path.exists(path):
        return order
    for idx, line in enumerate(open(path, errors="replace")):
        m = re.match(r"#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)", line)
        if m and m.group(1) in names_of_interest and m.group(1) not in order:
            order[m.group(1)] = idx
    return order


def reconcile(header, oracle=None, missing_by_descriptor=None, other_errors=None,
              add_manifest=True):
    print(f"=== {header} ===")
    if oracle is None:
        oracle = discover_headers([header], 1)[header]
    if not oracle:
        print(f"  header not found in any of the 8 oracles; skipping")
        return

    applicable = sorted(oracle.keys())
    print(f"  present in: {', '.join(applicable)}")

    if add_manifest:
        for d in applicable:
            add_to_manifest(d, header)

    shim_path = SHIM_INCLUDE / header
    if not shim_path.exists():
        print(f"  ERROR: {shim_path} does not exist in the shim; create it first")
        return
    shim_text = shim_path.read_text()
    shim_names = set(re.findall(r"^#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)", shim_text, re.M))

    if missing_by_descriptor is None:
        missing_by_descriptor = {}
        other_errors = {}
        for d in applicable:
            names, other = missing_macros_for(d, header)
            missing_by_descriptor[d] = names
            if other:
                other_errors[d] = other

    all_missing = set()
    for names in missing_by_descriptor.values():
        all_missing |= names
    if not all_missing and not other_errors:
        print(f"  already reconciled across all applicable descriptors")
        return

    name_pattern = {}
    name_value = {}
    conflicts = {}
    for name in all_missing:
        pattern = frozenset(d for d in applicable if name in missing_by_descriptor[d])
        values = {}
        for d in pattern:
            entry = oracle[d].get(name)
            if entry is None:
                continue
            values.setdefault(clean_value(entry["replacement"]), []).append(d)
        if len(values) > 1:
            conflicts[name] = values
            continue
        if not values:
            continue
        value = next(iter(values))
        if value == name:
            conflicts[name] = {f"<self-referential, backing enum value unknown>": pattern}
            continue
        name_pattern[name] = pattern
        name_value[name] = value

    groups = {}
    for name, pattern in name_pattern.items():
        groups.setdefault(pattern, []).append(name)

    insertions = {}
    for pattern, names in groups.items():
        rep_descriptor = sorted(pattern)[0]
        rep_macros = oracle[rep_descriptor]
        files_needed = {rep_macros[n]["definition_file"] for n in names if n in rep_macros}
        order = {}
        for path in files_needed:
            order.update(find_definition_order(path, set(names)))
        ordered_names = sorted(names, key=lambda n: order.get(n, 1 << 30))

        anchor = None
        for path in files_needed:
            full_order = {}
            for idx, line in enumerate(open(path, errors="replace")):
                m = re.match(r"#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)", line)
                if m:
                    full_order.setdefault(m.group(1), idx)
            if ordered_names and ordered_names[0] in full_order:
                target_idx = full_order[ordered_names[0]]
                best = None
                for nm, idx in full_order.items():
                    if idx < target_idx and nm in shim_names:
                        if best is None or idx > full_order[best]:
                            best = nm
                if best is not None:
                    anchor = best
            break

        guard = guard_for(pattern)
        body = []
        for n in ordered_names:
            v = name_value[n]
            body.append(f"#define {n} {v}" if v else f"#define {n}")
        insertions.setdefault(anchor, []).append((guard, body))

    lines = shim_text.split("\n")
    anchor_line_idx = {}
    for idx, line in enumerate(lines):
        m = re.match(r"#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)", line)
        if m:
            anchor_line_idx[m.group(1)] = idx

    plain_insert_at_end = []
    keyed_insertions = {}
    for anchor, blocks in insertions.items():
        if anchor is None or anchor not in anchor_line_idx:
            plain_insert_at_end.extend(blocks)
        else:
            keyed_insertions.setdefault(anchor_line_idx[anchor], []).extend(blocks)

    def render(blocks):
        out = []
        for guard, body in blocks:
            if guard is None:
                out.extend(body)
            else:
                out.append(f"#if {guard}")
                out.extend(body)
                out.append("#endif")
        return out

    for idx in sorted(keyed_insertions.keys(), reverse=True):
        block_text = render(keyed_insertions[idx])
        lines[idx + 1 : idx + 1] = block_text

    if plain_insert_at_end:
        end_idx = None
        for idx in range(len(lines) - 1, -1, -1):
            if lines[idx].strip() == "#endif":
                end_idx = idx
                break
        block_text = render(plain_insert_at_end)
        if end_idx is not None:
            lines[end_idx:end_idx] = block_text + [""]
        else:
            lines.extend(block_text)

    shim_path.write_text("\n".join(lines))

    inserted_count = sum(len(names) for names in groups.values())
    print(f"  inserted {inserted_count} macros across {len(groups)} guard group(s)")
    if conflicts:
        print(f"  CONFLICTS (value differs across descriptors that both lack it, not auto-inserted):")
        for name, values in conflicts.items():
            print(f"    {name}: {values}")
    if other_errors:
        print(f"  NON-MACRO ERRORS (needs manual fixup):")
        for d, errs in other_errors.items():
            for e in errs[:5]:
                print(f"    [{d}] {e}")


def reconcile_batch(headers, jobs):
    oracle = discover_headers(headers, jobs)
    applicable_headers = []
    for header in headers:
        if not oracle[header]:
            print(f"=== {header} ===")
            print("  header not found in any of the 8 oracles; skipping")
            continue
        applicable_headers.append(header)
        for descriptor in sorted(oracle[header]):
            add_to_manifest(descriptor, header)

    if not applicable_headers:
        return

    missing, other_errors = run_descriptor_jobs(applicable_headers, oracle, jobs)
    for header in applicable_headers:
        reconcile(
            header,
            oracle=oracle[header],
            missing_by_descriptor=missing[header],
            other_errors=other_errors[header],
            add_manifest=False,
        )

    descriptors = sorted(
        {
            descriptor
            for header in applicable_headers
            for descriptor in oracle[header]
        }
    )
    failures = validate_descriptors(descriptors, jobs)
    if failures:
        print("FINAL MATRIX ERRORS (needs manual fixup):")
        for descriptor, output in failures:
            print(f"  [{descriptor}] {output}")
    else:
        print("final declaration matrices pass across all applicable descriptors")


def main(argv):
    jobs = min(8, len(DESCRIPTORS))
    headers = []
    for arg in argv[1:]:
        if arg.startswith("--jobs="):
            jobs = int(arg.split("=", 1)[1])
        elif not arg.startswith("--"):
            headers.append(arg)
    if not headers:
        print(__doc__, file=sys.stderr)
        return 2
    reconcile_batch(headers, jobs)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
