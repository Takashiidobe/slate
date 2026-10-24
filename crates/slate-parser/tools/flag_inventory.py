#!/usr/bin/env python3
"""Inventory the compiler flags gcc, clang and cl.exe accept, and what slate does with them.

usage: flag_inventory.py [--llvm DIR] [--corpus DIR] [--no-msvc] [--jobs N] [--out FILE] [--json FILE]

Sources:
  clang  `llvm-tblgen -dump-json` of clang/include/clang/Options/Options.td. A cc1
         option is classified by its marshalling KeyPath (LangOpts, CodeGenOpts,
         TargetOpts, ...), with LangOptions.def's Benign/Compatible/NotCompatible.
         A driver-only flag is resolved with `clang -###` to the cc1 arguments it
         adds, which are classified the same way.
  gcc    `gcc -Q --help=<class>` of the installed gcc (c, target, optimizers,
         warnings, common).
  msvc   `cl.exe /?`, mapped to a meaning through clang-cl's aliases.

For every flag accepted by an oracle on x86_64-linux it records the predefined
macros the flag changes (`-dM -E`), whether slate-parser accepts it for that
flavor (and whether it is a CODEGEN_ONLY_FLAGS row), and how often the corpus
compile databases pass it. Writes target/flag-inventory.md and .json.
Per-flag detail (macro values on both sides): tools/flag_probe.py.
"""
import argparse
import collections
import concurrent.futures
import json
import os
import re
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
WORKSPACE = ROOT.parents[1]
SLATE = WORKSPACE / "target/test-cache/release/slate-parser"
CL = ROOT / "tools/cl.exe"
DEFINE = re.compile(r"#define (\w+)(.*)")
NOT_C = re.compile(
    r"objc|obj-c|cuda|hip|opencl|cl-|sycl|hlsl|cxx|c\+\+|modules?\b|module-|gpu|offload|openmp-target|"
    r"rtti|exceptions-model|coroutines|concepts|char8|sized-dealloc|aligned-alloc|new-|delete-|elide|"
    r"access-control|threadsafe-statics|template|constexpr|vtable|virtual|pch|apinotes|swift|xray|"
    r"profile|coverage|instrument|sanitize-coverage|debug|dwarf|^g|record-command|crash|time-trace|"
    r"save-temps|diagnostics|color|caret|show-|message-length|spell|tabstop|macro-backtrace|"
    r"template-backtrace|print-|verbose|emit-|ast|plugin|pass-plugin|experimental-new-pm|lto-|"
    r"thinlto|fatal-warnings|ms-runtime-lib|ms-omit|cs-profile|basic-block|split-dwarf|unique-|"
    r"sycl|dxc|hipstdpar|openacc|zvector|altivec|apple|darwin|ios|macos|watchos|tvos|xros|driverkit|"
    r"objc|fortran|flang|ropi|rwpi|aligned-new|constant-|cfstrings|cf-runtime|raw-string|operator-names|"
    r"assume-nothrow|complete-member-pointers|application-extension|fuchsia|experimental-library|"
    r"ignore-exceptions|atomic-(fine|remote|ignore)|unsafe-fp-atomics|retain-comments|parse-all-comments|"
    r"max-tokens|rewrite-|driver-only|=help|linker-version|define-target-os-macros|visibility-ms-compat|"
    r"openmp-extensions|clang-abi-compat|double-square-bracket|pointer-field-protection|gnu-keywords|"
    r"modulemap|lvi-|seses|speculative-load|kernel$|iamcu",
    re.I,
)
SEMANTIC_KEYS = ("LangOpts", "TargetOpts", "PreprocessorOpts", "HeaderSearchOpts")
SEMANTIC_GCC = ("c", "target")
GCC_SEMANTIC_OPTIMIZERS = {
    "-fsingle-precision-constant", "-ffloat-store", "-fpcc-struct-return", "-freg-struct-return",
    "-fstrict-volatile-bitfields", "-fwrapv-pointer", "-fpack-struct", "-fpack-struct=", "-ffp-contract=",
    "-fexcess-precision=", "-fleading-underscore", "-fzero-init-padding-bits=", "-fcx-fortran-rules",
    "-fcx-limited-range", "-fsigned-zeros", "-ftrapping-math", "-frounding-math", "-fsignaling-nans",
    "-fwrapv", "-ftrapv", "-fshort-enums", "-fdelete-null-pointer-checks", "-fstrict-aliasing",
    "-fisolate-erroneous-paths-dereference", "-ftrivial-auto-var-init=",
}
VALUE_GUESSES = (
    "1", "4", "none", "default", "all", "on", "fast", "utf-8", "0", "sse", "sysv", "precise", "standard",
    "strict", "c11", "x86-64", "full", "source", "ignore", "short",
)


def run(command: list[str], **kwargs) -> subprocess.CompletedProcess:
    return subprocess.run(command, capture_output=True, text=True, errors="replace", **kwargs)


def tblgen_options(llvm: Path) -> dict:
    out = Path(tempfile.mkstemp(suffix=".json")[1])
    options_dir = llvm / "clang/include/clang/Options"
    result = run(
        [
            "llvm-tblgen", "-dump-json",
            "-I", str(options_dir), "-I", str(llvm / "clang/include"), "-I", str(llvm / "llvm/include"),
            str(options_dir / "Options.td"), "-o", str(out),
        ]
    )
    if result.returncode:
        sys.exit(f"llvm-tblgen failed: {result.stderr.strip()[:400]}")
    records = json.loads(out.read_text())
    out.unlink()
    return records


def lang_compatibility(llvm: Path) -> dict[str, str]:
    text = (llvm / "clang/include/clang/Basic/LangOptions.def").read_text()
    plain = re.compile(r"^(?:VALUE_)?LANGOPT\(\s*(\w+)\s*,[^,]*,[^,]*,\s*(\w+)", re.M)
    enum = re.compile(r"^ENUM_LANGOPT\(\s*(\w+)\s*,[^,]*,[^,]*,[^,]*,\s*(\w+)", re.M)
    return dict(plain.findall(text) + enum.findall(text))


def def_name(value) -> str:
    if isinstance(value, dict):
        return value.get("def") or value.get("printable") or ""
    return value or ""


def visibility(record: dict) -> set[str]:
    return {def_name(entry) for entry in record.get("Visibility") or []}


def clang_records(records: dict) -> tuple[list[dict], dict[str, dict], list[dict]]:
    driver, cc1, cl = [], {}, []
    for name in records["!instanceof"]["Option"]:
        record = records[name]
        kind = def_name(record.get("Kind"))
        if kind in ("KIND_INPUT", "KIND_UNKNOWN", "KIND_GROUP"):
            continue
        prefixes = record.get("Prefixes") or []
        spelling = record.get("Name", "")
        entry = {
            "id": name,
            "name": spelling,
            "kind": kind.removeprefix("KIND_").lower(),
            "group": def_name(record.get("Group")),
            "keypath": record.get("KeyPath") or "",
            "alias": def_name(record.get("Alias")),
            "alias_args": record.get("AliasArgs") or [],
            "values": record.get("Values") or "",
            "help": (record.get("HelpText") or "").replace("\n", " "),
            "prefixes": prefixes,
        }
        vis = visibility(record)
        if "CC1Option" in vis and "-" in prefixes:
            cc1.setdefault("-" + spelling, entry)
        if "CLOption" in vis and "/" in prefixes:
            cl.append(entry)
        if "DefaultVis" in vis and "-" in prefixes and not (vis & {"FlangOnlyOption"}):
            driver.append(entry)
    return driver, cc1, cl


def is_c_candidate(entry: dict) -> bool:
    name = entry["name"]
    if not name or not (name.startswith(("f", "m")) or name in ("undef", "traditional-cpp", "trigraphs", "ansi")):
        return False
    if entry["group"].startswith(("W_", "R_", "clang_ignored_legacy", "m_hexagon", "m_mips", "m_ppc", "m_riscv",
                                  "m_wasm", "m_ve", "m_loongarch", "m_sparc", "m_systemz", "m_amdgpu",
                                  "m_libc", "m_hip", "m_cuda", "m_ve_Features")):
        return False
    if entry["kind"] in ("separate", "remaining_args", "comma_joined", "multi_arg", "remaining_args_joined"):
        return False
    return not NOT_C.search(name)


def value_candidates(entry: dict) -> list[str]:
    if entry["kind"] == "flag":
        return [""]
    explicit = [value for value in entry["values"].split(",") if value]
    return explicit[:2] + [value for value in VALUE_GUESSES if value not in explicit]


def cc1_tokens(stderr: str) -> list[str] | None:
    for line in stderr.splitlines():
        if '"-cc1"' in line:
            return shlex.split(line)[2:]
    return None


def resolve_cc1(tokens: list[str], cc1: dict[str, dict]) -> list[dict]:
    hits = []
    joined = sorted((s for s, e in cc1.items() if e["kind"] in ("joined", "joined_or_separate")), key=len, reverse=True)
    for token in tokens:
        if not token.startswith("-"):
            continue
        entry = cc1.get(token) or next((cc1[s] for s in joined if token.startswith(s)), None)
        if entry and entry["name"] not in ("x", "fsyntax-only", "o", "mllvm", "target-feature", "main-file-name"):
            hits.append(entry)
    return hits


def keypath_class(keypath: str, compat: dict[str, str]) -> str:
    if not keypath:
        return ""
    head, _, field = keypath.partition("->")
    field = re.split(r"[.\[]", field)[0]
    if head == "LangOpts":
        return f"LangOpts:{compat.get(field, '?')}"
    return head


def probe_clang(entry: dict, cc1: dict[str, dict], compat: dict[str, str], scratch: Path, baseline: list[str],
                macros0: dict[str, str]) -> dict:
    empty = scratch / "empty.c"
    spelling = "-" + entry["name"]
    for value in value_candidates(entry):
        flag = spelling + value
        result = run(["clang", "-###", "-c", "-x", "c", flag, str(empty), "-o", "/dev/null"])
        if "error:" in result.stderr or result.returncode:
            continue
        tokens = cc1_tokens(result.stderr) or []
        added = list((collections.Counter(tokens) - collections.Counter(baseline)).elements())
        hits = resolve_cc1(added, cc1)
        classes = sorted({keypath_class(hit["keypath"], compat) or f"cc1:{hit['name']}" for hit in hits})
        if "-target-feature" in added:
            classes.append("TargetOpts:feature")
        if "-mllvm" in added:
            classes.append("CodeGenOpts:mllvm")
        unused = "argument unused" in result.stderr or "optimization flag" in result.stderr
        macros = oracle_macros("clang", [flag], scratch)
        if macros is None:
            continue
        return {
            "flag": flag,
            "accepted": True,
            "classes": classes,
            "cc1": added[:6],
            "unused": unused and not added,
            "ignored": entry["group"].startswith("clang_ignored"),
            "macros": macro_delta(macros0, macros),
        }
    return {"flag": spelling, "accepted": False}


def oracle_macros(cc: str, flags: list[str], scratch: Path) -> dict[str, str] | None:
    result = run([cc, "-dM", "-E", "-x", "c", *flags, str(scratch / "empty.c")])
    macros = {m.group(1): m.group(2).strip() for line in result.stdout.splitlines() if (m := DEFINE.match(line))}
    if result.returncode or "__STDC__" not in macros and not any(f in ("-undef",) for f in flags):
        return None
    return macros


def macro_delta(before: dict[str, str], after: dict[str, str]) -> list[str]:
    changed = [f"-{name}" for name in before if name not in after]
    changed += [name for name in after if before.get(name) != after[name]]
    return sorted(changed)


def gcc_options() -> dict[str, dict]:
    classes = {}
    for help_class in ("warnings", "optimizers", "target", "c", "common"):
        result = run(["gcc", "-Q", f"--help={help_class}"])
        for line in result.stdout.splitlines():
            match = re.match(r"\s{2}(-[\w+#.=<>|\[\]:,/-]+)", line)
            if not match:
                continue
            spelling = match.group(1)
            classes.setdefault(spelling, help_class)
    options = {}
    for spelling, help_class in classes.items():
        base = re.split(r"[<\[]", spelling)[0]
        if base.startswith("--") or base in ("-fhelp", "-fversion", "-fhelp=", "-ftarget-help"):
            continue
        values = re.findall(r"\[([^\]]+)\]", spelling)
        if base.endswith("=") or "<" in spelling:
            guesses = (values[0].split("|")[:2] if values else []) + list(VALUE_GUESSES)
        else:
            guesses = [""]
        options[base] = {"class": help_class, "guesses": guesses}
    return options


def probe_gcc(spelling: str, info: dict, scratch: Path, macros0: dict[str, str]) -> dict:
    empty = scratch / "empty.c"
    for value in info["guesses"]:
        flag = spelling + value
        result = run(["gcc", "-fsyntax-only", "-x", "c", flag, str(empty)])
        if result.returncode or "valid for" in result.stderr or "not supported" in result.stderr:
            continue
        macros = oracle_macros("gcc", [flag], scratch)
        if macros is None:
            continue
        return {"flag": flag, "accepted": True, "class": info["class"], "macros": macro_delta(macros0, macros)}
    return {"flag": spelling, "accepted": False, "class": info["class"]}


def slate_accepts(flavor: str, flag: str, scratch: Path) -> bool:
    if not flag:
        return False
    result = run([str(SLATE), "ir", str(scratch / "empty.c"), f"--flavor={flavor}", flag])
    return result.returncode == 0


def codegen_only_rows() -> set[str]:
    text = (ROOT / "src/compiler_args.rs").read_text()
    return set(re.findall(r'CodegenOnlyFlag::(?:switch|value)\("([^"]+)"', text))


def positive(spelling: str) -> str:
    return re.sub(r"^-(f|m)no-", r"-\1", spelling)


def corpus_counts(corpus: Path) -> collections.Counter:
    counts = collections.Counter()
    for database in corpus.glob("*/build-*/compile_commands.json"):
        try:
            entries = json.loads(database.read_text())
        except (OSError, json.JSONDecodeError):
            continue
        seen = set()
        for entry in entries:
            arguments = entry.get("arguments") or shlex.split(entry.get("command", ""))
            for argument in arguments:
                if argument.startswith("-") and not argument.startswith(("-D", "-I", "-W", "-o")):
                    key = argument.split("=", 1)[0] + ("=" if "=" in argument else "")
                    seen.add((database.parent.parent.name, key))
        for _, key in seen:
            counts[key] += 1
    return counts


def cl_options() -> list[str]:
    if not CL.exists():
        return []
    try:
        result = run([str(CL), "/?"], timeout=120)
    except subprocess.TimeoutExpired:
        return []
    text = result.stdout.replace("\r", "")
    return sorted(set(re.findall(r"(?:^|\s{2,})(/[A-Za-z][^\s]*)", text, re.M)))


def verdict(row: dict) -> str:
    clang, gcc = row.get("clang") or {}, row.get("gcc") or {}
    classes = clang.get("classes", [])
    if positive(row["key"]) in GCC_SEMANTIC_OPTIMIZERS and gcc.get("accepted"):
        return "semantic"
    if clang.get("ignored"):
        return "review" if gcc.get("class") in SEMANTIC_GCC or gcc.get("macros") else "clang-ignores"
    if "TargetOpts:feature" in classes and not any(c.startswith(("LangOpts", "CodeGenOpts")) for c in classes):
        return "isa"
    semantic = any(c.startswith(SEMANTIC_KEYS) and not c.endswith(":Benign") for c in classes)
    semantic |= any(c.startswith("cc1:") for c in classes)
    semantic |= bool(clang.get("macros") or gcc.get("macros"))
    if not clang.get("accepted") and gcc.get("class") in SEMANTIC_GCC:
        return "review"
    if semantic:
        return "semantic"
    if any(c.startswith("CodeGenOpts") for c in classes) or gcc.get("class") == "optimizers":
        return "codegen"
    if clang.get("unused"):
        return "unused-in-c"
    return "other"


def status(row: dict, flavor: str, codegen_only: set[str]) -> str:
    oracle = row.get(flavor) or {}
    if not oracle.get("accepted"):
        return ""
    if not row.get(f"slate_{flavor}"):
        return "missing"
    spelling = oracle["flag"].split("=", 1)[0] + ("=" if "=" in oracle["flag"] else "")
    if spelling in codegen_only or positive(spelling) in codegen_only:
        return "ignored"
    return "implemented"


def markdown(rows: list[dict], cl_rows: list[dict], counts: collections.Counter) -> str:
    lines = ["# Flag inventory", "", "Generated by `crates/slate-parser/tools/flag_inventory.py`. "
             "Oracles: installed clang and gcc on x86_64-linux, cl.exe under wine.", ""]
    tally = collections.Counter((row["verdict"], row["status_clang"] or "-", row["status_gcc"] or "-") for row in rows)
    lines += ["## Summary", "", "| verdict | clang status | gcc status | flags |", "| --- | --- | --- | --- |"]
    lines += [f"| {v} | {c} | {g} | {n} |" for (v, c, g), n in sorted(tally.items())]

    def table(title: str, selected: list[dict]) -> None:
        lines.extend(["", f"## {title} ({len(selected)})", "",
                      "| flag | projects | clang | gcc | clang class | gcc class | macros |",
                      "| --- | --- | --- | --- | --- | --- | --- |"])
        for row in selected:
            clang, gcc = row.get("clang") or {}, row.get("gcc") or {}
            macros = sorted(set(clang.get("macros", [])) | set(gcc.get("macros", [])))
            lines.append(
                f"| `{row['flag']}` | {counts.get(row['key'], 0)} | {row['status_clang'] or '-'} | "
                f"{row['status_gcc'] or '-'} | {', '.join(clang.get('classes', [])) or '-'} | "
                f"{gcc.get('class', '-')} | {' '.join(macros[:6]) or '-'} |"
            )

    by_usage = lambda row: (-counts.get(row["key"], 0), row["flag"])
    missing = lambda row: "missing" in (row["status_clang"], row["status_gcc"])
    table("Missing, semantic", sorted([r for r in rows if missing(r) and r["verdict"] == "semantic"], key=by_usage))
    table("Missing, gcc-only, needs review", sorted([r for r in rows if missing(r) and r["verdict"] == "review"], key=by_usage))
    table("Missing, codegen", sorted([r for r in rows if missing(r) and r["verdict"] == "codegen"], key=by_usage))
    table("Missing, ISA features", sorted([r for r in rows if missing(r) and r["verdict"] == "isa"], key=by_usage))
    table("Implemented or ignored, semantic", sorted(
        [r for r in rows if not missing(r) and r["verdict"] == "semantic"], key=by_usage))
    table("Missing, other", sorted([r for r in rows if missing(r) and r["verdict"] in ("other", "unused-in-c")], key=by_usage))
    if cl_rows:
        lines += ["", f"## cl.exe ({len(cl_rows)})", "", "slate models no `/` spelling. clang-cl's alias gives the meaning.", "",
                  "| cl flag | clang-cl alias | alias class | slate (clang flavor) |", "| --- | --- | --- | --- |"]
        for row in cl_rows:
            lines.append(f"| `{row['flag']}` | {row['alias'] or '-'} | {row['class'] or '-'} | {row['slate'] or '-'} |")
    return "\n".join(lines) + "\n"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--llvm", type=Path, default=Path.home() / "llvm-project")
    parser.add_argument("--corpus", type=Path, default=Path(os.environ.get("SLATE_CORPUS", Path.home() / "c-corpus")))
    parser.add_argument("--no-msvc", action="store_true")
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 8)
    parser.add_argument("--out", type=Path, default=WORKSPACE / "target/flag-inventory.md")
    parser.add_argument("--json", type=Path, default=WORKSPACE / "target/flag-inventory.json")
    args = parser.parse_args()
    if not SLATE.exists():
        sys.exit(f"{SLATE} missing: cargo build --release -p slate-parser")

    records = tblgen_options(args.llvm)
    compat = lang_compatibility(args.llvm)
    driver, cc1, cl = clang_records(records)
    candidates = [entry for entry in driver if is_c_candidate(entry)]
    gcc = gcc_options()
    codegen_only = codegen_only_rows()

    with tempfile.TemporaryDirectory() as directory:
        scratch = Path(directory)
        (scratch / "empty.c").write_text("int slate_probe_unit;\n")
        baseline = cc1_tokens(run(["clang", "-###", "-c", "-x", "c", str(scratch / "empty.c"), "-o", "/dev/null"]).stderr) or []
        clang0, gcc0 = oracle_macros("clang", [], scratch), oracle_macros("gcc", [], scratch)
        with concurrent.futures.ThreadPoolExecutor(args.jobs) as pool:
            clang_results = list(pool.map(lambda e: probe_clang(e, cc1, compat, scratch, baseline, clang0), candidates))
            gcc_results = list(pool.map(lambda item: probe_gcc(item[0], item[1], scratch, gcc0), gcc.items()))
            rows: dict[str, dict] = {}
            for result in clang_results:
                if result["accepted"]:
                    key = result["flag"].split("=", 1)[0] + ("=" if "=" in result["flag"] else "")
                    rows.setdefault(key, {"key": key, "flag": result["flag"]})["clang"] = result
            for result in gcc_results:
                if result["accepted"] and result["class"] != "warnings":
                    key = result["flag"].split("=", 1)[0] + ("=" if "=" in result["flag"] else "")
                    rows.setdefault(key, {"key": key, "flag": result["flag"]})["gcc"] = result
            jobs = [(row, flavor) for row in rows.values() for flavor in ("clang", "gcc") if (row.get(flavor) or {}).get("accepted")]
            accepted = pool.map(lambda job: slate_accepts(job[1], job[0][job[1]]["flag"], scratch), jobs)
            for (row, flavor), ok in zip(jobs, accepted):
                row[f"slate_{flavor}"] = ok
            cl_rows = []
            if not args.no_msvc:
                aliases = {entry["name"].lower(): entry for entry in cl}
                for flag in cl_options():
                    name = flag[1:].split(":", 1)[0]
                    entry = aliases.get(flag[1:].lower()) or aliases.get(name.lower()) or aliases.get((name + ":").lower())
                    alias = cc1_alias = None
                    if entry and entry["alias"]:
                        target = records.get(entry["alias"], {})
                        alias = "-" + target.get("Name", entry["alias"]) + "".join(entry["alias_args"])
                        cc1_alias = cc1.get("-" + target.get("Name", ""))
                    cl_rows.append({
                        "flag": flag,
                        "alias": alias,
                        "class": keypath_class(cc1_alias["keypath"], compat) if cc1_alias else "",
                        "slate": ("accepts" if slate_accepts("clang", alias, scratch) else "rejects") if alias else "",
                    })

    counts = corpus_counts(args.corpus)
    final = sorted(rows.values(), key=lambda row: row["flag"])
    for row in final:
        row["verdict"] = verdict(row)
        row["status_clang"] = status(row, "clang", codegen_only)
        row["status_gcc"] = status(row, "gcc", codegen_only)
        row["corpus"] = counts.get(row["key"], 0)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(markdown(final, cl_rows, counts))
    args.json.write_text(json.dumps({"flags": final, "cl": cl_rows}, indent=1))
    missing = sum("missing" in (row["status_clang"], row["status_gcc"]) and row["verdict"] == "semantic" for row in final)
    print(f"{len(final)} flags, {missing} missing semantic; wrote {args.out} and {args.json}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
