#!/usr/bin/env python3
import argparse
import collections
import json
import os
import re
import shlex
import subprocess
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path.home() / "llvm-project/clang/test"
REPO = Path(__file__).resolve().parent.parent
SLATE = REPO / "target/release/slate-parser"
CL = REPO / "tools/cl.exe"
SYSROOTS = Path(os.environ.get("SLATE_SYSROOTS", Path.home() / ".local/share/slate/sysroots"))

SKIP_DIRS = {"Driver", "ClangScanDeps", "OffloadTools", "utils", "PCH", "CIR"}
MS_TRIPLE = re.compile(r"^(i[3-6]86|x86_64|aarch64|arm64|thumbv7a?|armv7)-[\w.]*-?(windows-msvc[\d.]*|win32)$")
ARCHES = {"i386": "i686", "i486": "i686", "i586": "i686", "i686": "i686", "x86_64": "x86_64", "aarch64": "aarch64", "arm64": "aarch64"}
MSVC_ARCH = {"i686": "x86", "x86_64": "x64", "aarch64": "arm64"}
TOOLS = ("%clang_cc1", "%clang_cl", "%clang")
CLANG_FEATURES = ("-fblocks", "-fenable-matrix", "-fexperimental-max-bitint-width=", "-fdefer-ts", "-fcoroutines", "-ffreestanding", "-fno-builtin")


def run_lines(text):
    lines = []
    pending = ""
    for line in text.splitlines():
        match = re.search(r"\bRUN:(.*)$", line)
        if not match:
            continue
        body = match.group(1).rstrip()
        if body.endswith("\\"):
            pending += body[:-1] + " "
            continue
        lines.append(pending + body)
        pending = ""
    return lines


def normalize_triple(triple):
    if not MS_TRIPLE.match(triple):
        return None
    arch = ARCHES.get(triple.split("-")[0])
    return f"{arch}-pc-windows-msvc" if arch else None


def verify_prefixes(opt):
    return opt.split("=", 1)[1].split(",") if "=" in opt else ["expected"]


def parse_run(line, test_dir):
    command = line.split("|")[0].strip()
    try:
        words = shlex.split(command)
    except ValueError:
        return None
    if not words or words[0] not in TOOLS or "%s" not in words:
        return None
    tool = words[0]
    triple = "x86_64-pc-windows-msvc" if tool == "%clang_cl" else None
    config = {"std": None, "defines": [], "includes": [], "forced": [], "verify": None, "notes": [], "clang_flags": [], "warnings": []}
    it = iter(words[1:])
    for word in it:
        if word in ("-triple", "-target", "--target"):
            triple = normalize_triple(next(it, ""))
            if triple is None:
                return None
        elif word.startswith(("--target=", "-target=")):
            triple = normalize_triple(word.split("=", 1)[1])
            if triple is None:
                return None
        elif word == "-x":
            if next(it, "") not in ("c", "c-header"):
                return None
        elif word in ("/TP", "-TP") or word.startswith("-std=c++") or word.startswith(("/std:c++", "-std:c++")):
            return None
        elif word.startswith(("-std=", "/std:", "-std:")):
            config["std"] = re.split(r"[=:]", word, maxsplit=1)[1]
        elif word.startswith(("-D", "/D", "-U", "/U")) and len(word) > 2:
            config["defines"].append("-" + word[1:])
        elif word in ("-D", "-U", "/D", "/U"):
            config["defines"].append("-" + word[1] + next(it, ""))
        elif word.startswith(("-I", "/I")):
            path = word[2:] or next(it, "")
            config["includes"].append(path.replace("%S", str(test_dir)))
        elif word in ("-isystem", "-include"):
            path = next(it, "").replace("%S", str(test_dir))
            (config["includes"] if word == "-isystem" else config["forced"]).append(path)
        elif word in ("-E", "/E", "/P", "-P"):
            return None
        elif word.startswith(CLANG_FEATURES):
            config["clang_flags"] += ["-Xclang", word] if tool == "%clang_cc1" else [word]
        elif word.startswith("-W") and not word.startswith(("-Wl", "-Wa")):
            config["warnings"].append(word)
        elif word.startswith("-verify"):
            config["verify"] = verify_prefixes(word)
        elif word.startswith("-fms-compatibility-version") or word.startswith("-fmsc-version"):
            config["notes"].append(word)
    if triple is None:
        return None
    config["triple"] = triple
    return config


def config_key(config):
    return (config["triple"], config["std"], tuple(config["defines"]), tuple(config["includes"]), tuple(config["forced"]), tuple(config["clang_flags"]), tuple(config["warnings"]))


def expects_errors(text, prefixes):
    return any(re.search(rf"\b{re.escape(prefix)}-error\b", text) for prefix in prefixes)


def run(cmd, env=None):
    try:
        proc = subprocess.run(cmd, capture_output=True, timeout=60, env={**os.environ, "NO_COLOR": "1", **(env or {})})
        return proc.returncode, (proc.stdout + proc.stderr).decode(errors="replace")
    except subprocess.TimeoutExpired:
        return "timeout", ""


def sysroot_includes(triple):
    root = SYSROOTS / triple
    return [str(root / sub) for sub in ("crt/include", "sdk/include/ucrt", "sdk/include/shared", "sdk/include/um")]


def msvc_std(std):
    std = (std or "gnu17").removeprefix("gnu").removeprefix("iso9899:")
    if std in ("c11", "c1x", "2011"):
        return "c11"
    if std in ("c23", "c2x", "c2y", "2023"):
        return "clatest"
    return "c17"


def clang_std(std):
    return [f"-std={std}"] if std else []


# slate defaults every flavor to gnu23; pin the oracle's effective standard so the default does not mask other gaps
def slate_std(flavor, std):
    if flavor == "msvc":
        return {"clatest": "c23"}.get(msvc_std(std), msvc_std(std))
    return std or "gnu17"


def one_config(path, config):
    rel = path.relative_to(ROOT)
    triple = config["triple"]
    common = [*config["defines"], *[f"-I{p}" for p in config["includes"]], *config["warnings"]]
    forced = [arg for p in config["forced"] for arg in ("-include", p)]
    clang_cl = run(["clang", f"--target={triple}", "-fsyntax-only", "-x", "c", *clang_std(config["std"]), *config["clang_flags"], *common, *forced, *[f"-isystem{p}" for p in sysroot_includes(triple)], str(path)])
    result = {"file": str(rel), "triple": triple, "std": config["std"], "defines": config["defines"], "includes": config["includes"], "forced": config["forced"], "clang_flags": config["clang_flags"], "warnings": config["warnings"], "notes": config["notes"], "verify": config["verify"] is not None}
    slate = [str(SLATE), "parse", str(path), f"-target={triple}", *common, *forced, "--dump-ir"]
    for flavor in ("clang", "msvc"):
        rc, err = run([*slate, f"--flavor={flavor}", f"-std={slate_std(flavor, config['std'])}"])
        result[f"slate_{flavor}"] = rc
        result[f"slate_{flavor}_err"] = err.replace(str(path), rel.name)[-3000:] if rc != 0 else ""
    if config["std"] is None and result["slate_clang"] == 0:
        result["slate_clang_default_std"] = run([*slate, "--flavor=clang"])[0]
    arch = MSVC_ARCH.get(triple.split("-")[0])
    if arch:
        cl_args = [str(CL), "/nologo", "/Zs", "/TC", f"/std:{msvc_std(config['std'])}", *[f"/{d[1:]}" for d in config["defines"]], *[f"/I{p}" for p in config["includes"]], *[f"/FI{p}" for p in config["forced"]], str(path)]
        ms_rc, ms_err = run(cl_args, {"MSVC_ARCH": arch})
    else:
        ms_rc, ms_err = None, ""
    result["clang_cl"] = clang_cl[0]
    result["clang_cl_err"] = clang_cl[1][-1500:] if clang_cl[0] != 0 else ""
    result["msvc"] = ms_rc
    result["msvc_err"] = ms_err[-1500:] if ms_rc not in (0, None) else ""
    result["status"] = status(result)
    return result


def verdict(oracle, slate):
    if slate == "timeout" or oracle == "timeout":
        return "timeout"
    if oracle is None:
        return "no-oracle"
    if oracle == 0:
        return "accept" if slate == 0 else "gap"
    return "slate-only-accepts" if slate == 0 else "reject"


def status(result):
    clang = verdict(result["clang_cl"], result["slate_clang"])
    msvc = verdict(result["msvc"], result["slate_msvc"])
    if "timeout" in (clang, msvc):
        return "timeout"
    if clang == "gap" and msvc == "gap":
        return "shared-gap"
    if clang == "gap":
        return "clangcl-only-gap"
    if msvc == "gap":
        return "msvc-only-gap"
    if clang == "accept" and msvc in ("accept", "no-oracle"):
        return "all-accept"
    if clang == "accept":
        return "clang-only-accepts"
    if "slate-only-accepts" in (clang, msvc):
        return "slate-only-accepts"
    if msvc == "accept":
        return "msvc-only-accepts"
    return "all-reject"


def configs_for(path):
    text = path.read_text(errors="replace")
    configs = {}
    skipped = []
    for line in run_lines(text):
        config = parse_run(line, path.parent)
        if config is None:
            continue
        if config["verify"] is not None and expects_errors(text, config["verify"]):
            skipped.append(config["triple"])
            continue
        configs.setdefault(config_key(config), config)
    return list(configs.values()), skipped


def check_prefixes(text):
    prefixes = {"CHECK"}
    for line in run_lines(text):
        for match in re.finditer(r"-?-check-prefix(?:es)?[= ]\s*([\w,-]+)", line):
            prefixes.update(match.group(1).split(","))
    return prefixes


def stripped_source(text):
    prefixes = "|".join(re.escape(p) for p in sorted(check_prefixes(text), key=len, reverse=True))
    directive = re.compile(rf"^\s*//\s*(?:RUN|REQUIRES|UNSUPPORTED|XFAIL|END)\s*:|^\s*//\s*(?:{prefixes})(?:-[A-Z]+(?:-\d+)?)?\s*:")
    return "".join(line for line in text.splitlines(keepends=True) if not directive.match(line))


def fixture_directives(flavor, triple, configs):
    lines = [f"// SLATE-FILECHECK-FLAVOR {flavor}\n", f"// SLATE-FILECHECK-ARGS -target={triple}\n"]
    for index, config in enumerate(configs):
        prefix = "DEFAULT" if len(configs) == 1 else f"CFG{index}"
        defines = [d for d in config["defines"] if d.startswith("-D")]
        extra = [d for d in config["defines"] if d.startswith("-U")] + config["warnings"]
        lines.append(f"// SLATE-FILECHECK-DEFINES {prefix} {' '.join(defines)}".rstrip() + "\n")
        lines.append(f"// SLATE-FILECHECK-STD {prefix} {slate_std(flavor, config['std'])}\n")
        if extra:
            lines.append(f"// SLATE-FILECHECK-PREFIX-ARGS {prefix} {' '.join(extra)}\n")
    return "".join(lines)


def migrate(results, dest):
    groups = collections.defaultdict(list)
    for result in results:
        if result["clang_cl"] == 0 and result["slate_clang"] == 0 and not any(" " in d for d in result["defines"]):
            groups[(result["file"], result["triple"])].append(result)
    written = []
    for (file, triple), configs in sorted(groups.items()):
        source = stripped_source((ROOT / file).read_text(errors="replace"))
        stem = file.removesuffix(".c").replace("/", "__") + "-" + triple.split("-")[0]
        flavors = ["clang"]
        if all(c["msvc"] == 0 and c["slate_msvc"] == 0 for c in configs):
            flavors.append("msvc")
        for flavor in flavors:
            path = dest / (stem + ("-msvc" if flavor == "msvc" else "") + ".c")
            if path.exists():
                continue
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(source.rstrip("\n") + "\n\n" + fixture_directives(flavor, triple, configs))
            written.append(path)
    return written


def main():
    global ROOT
    parser = argparse.ArgumentParser(description="Compare slate-parser against clang-cl and cl.exe over the MS-mode C tests in clang/test")
    parser.add_argument("out", type=Path, help="JSON-lines result file")
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--jobs", type=int, default=os.cpu_count())
    parser.add_argument("--filter", help="only run files whose relative path contains this")
    parser.add_argument("--migrate", type=Path, metavar="DIR", help="write fixtures for accepted files that DIR does not have yet, e.g. tests/fixtures/msvc/clang-test")
    args = parser.parse_args()
    ROOT = args.root
    jobs = []
    verify_skipped = 0
    for path in sorted(ROOT.rglob("*.c")):
        rel = path.relative_to(ROOT)
        if rel.parts[0] in SKIP_DIRS or "Inputs" in rel.parts or (args.filter and args.filter not in str(rel)):
            continue
        configs, skipped = configs_for(path)
        verify_skipped += bool(skipped) and not configs
        jobs += [(path, config) for config in configs]
    counts = collections.Counter()
    results = []
    with ThreadPoolExecutor(max_workers=args.jobs) as pool, args.out.open("w") as out:
        for result in pool.map(lambda job: one_config(*job), jobs):
            counts[result["status"]] += 1
            results.append(result)
            out.write(json.dumps(result) + "\n")
    print(f"{len(jobs)} configurations, {verify_skipped} files skipped for expected-error -verify")
    print(dict(counts))
    if args.migrate:
        written = migrate(results, args.migrate)
        print(f"{len(written)} new fixtures; run tools/update_filecheck.py --in-place on them")
        for path in written:
            print(path)


main()
