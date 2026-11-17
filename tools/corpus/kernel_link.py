import argparse
import collections
import concurrent.futures
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

CRATE_MEMBER = "slate_crate-"
CRATE_CGU = re.compile(r"slate_crate-[0-9a-f]+\.slate_crate\.[0-9a-f]+(?:-(.+?))?\.[a-z0-9]+\.rcgu\.o")
UNIT_ITEM = re.compile(r"<?slate_crate(?:\[[0-9a-f]+\])?::(__slate_unit_[A-Za-z0-9_]+)::")
ORDERED_SECTIONS = re.compile(r"\.initcall|\.con_initcall|\.init\.setup$|__param$|__modver$|\.apicdrivers$"
                              r"|\.x86_cpu_dev\.init$|\.(early_)?lsm_info\.init$|__earlycon_table$"
                              r"|\.init_array|\.fini_array|\.ctors|\.dtors|\.kunit_(init_)?test_suites$")
ROOT_FORBIDDEN = re.compile(ORDERED_SECTIONS.pattern + r"|\.export_symbol|__tracepoints|\.altinstr|\.init\.|\.exit\."
                            r"|__jump_table$|\.discard\.annotate")
TARGET = "x86_64-unknown-none"


def output(command, cwd=None):
    return subprocess.run(command, cwd=cwd, check=True, capture_output=True, text=True).stdout


def defined_globals(paths, cwd=None):
    names = set()
    for line in output(["nm", "-g", "--defined-only", *map(str, paths)], cwd).splitlines():
        fields = line.split()
        if len(fields) == 3:
            names.add(fields[2])
    return names


def symbol_table(paths, *flags, cwd=None, skip_types=""):
    table = collections.defaultdict(set)
    paths = [str(path) for path in paths]
    for start in range(0, len(paths), 400):
        for line in output(["nm", "-A", *flags, *paths[start:start + 400]], cwd).splitlines():
            path, rest = line.split(":", 1)
            if (fields := rest.split(maxsplit=2)) and not (len(fields) == 3 and fields[1] in skip_types):
                table[path].add(fields[-1])
    return table


def section_table(paths):
    table = collections.defaultdict(set)
    paths = [str(path) for path in paths]
    for start in range(0, len(paths), 400):
        current = paths[start] if start + 1 == len(paths) else None
        for line in output(["readelf", "-SW", *paths[start:start + 400]]).splitlines():
            if line.startswith("File: "):
                current = line[len("File: "):].strip()
            elif current is not None and (match := re.match(r"\s*\[\s*\d+\]\s+(\S+)", line)):
                table[current].add(match[1])
    return table


def link_units(crate_dir, project_dir, native_dir):
    path = crate_dir / "link-order.json"
    if not path.exists():
        raise SystemExit(f"{path} is missing: the shard was not translated as a link-order crate")
    units = []
    for entry in json.loads(path.read_text())["units"]:
        source = Path(entry["file"])
        base = native_dir if source.is_relative_to(native_dir) else project_dir
        units.append((os.path.relpath(source.with_suffix(".o"), base), entry["module"]))
    return units


def unit_groups(name, units, objects):
    modules = {module for _, module in units if module}
    groups = collections.defaultdict(list)
    for member, path in objects.items():
        if not member.startswith(CRATE_MEMBER):
            continue
        cgu = match[1] if (match := CRATE_CGU.fullmatch(member)) else None
        if cgu == "cgu":
            raise SystemExit(f"{name}: {member} is a numbered codegen unit; build with CARGO_INCREMENTAL=1")
        top = cgu.split("-")[0] if cgu else None
        if top and top.startswith("__slate_unit_"):
            if top not in modules:
                raise SystemExit(f"{name}: {member} belongs to {top}, which link-order.json does not list")
            groups[top].append(path)
        else:
            groups[None].append(path)
    return groups


def check_groups(name, groups, has_root_unit):
    paths = [path for group in groups.values() for path in group]
    items = symbol_table(paths, "--defined-only", "--demangle=rust")
    for module, group in groups.items():
        for path in group:
            found = {match[1] for symbol in items.get(str(path), ())
                     if (match := UNIT_ITEM.match(symbol))} - {module}
            if found:
                raise SystemExit(f"{name}: {path.name} ({module or 'crate root'}) defines items of {sorted(found)}")
    if not has_root_unit and groups.get(None):
        for path, sections in section_table(groups[None]).items():
            if ordered := sorted(section for section in sections if ROOT_FORBIDDEN.match(section)):
                raise SystemExit(f"{name}: crate-root object {Path(path).name} holds ordered sections {ordered}")


def unreferenced_probestack(path):
    sections = re.findall(r"\] (\.text\.\S*__rust_probestack)\s", output(["readelf", "-SW", str(path)]))
    if len(sections) != 1:
        return None
    references = [line for line in output(["readelf", "-rW", str(path)]).splitlines()
                  if "__rust_probestack" in line and not line.startswith("Relocation section")]
    return None if references else sections[0]


def make(build_dir, compiler, jobs, log, target="vmlinux"):
    command = ["make", f"O={build_dir.name}", f"CC={compiler}", f"HOSTCC={compiler}", f"-j{jobs}", target]
    with open(log, "w") as stream:
        return subprocess.run(command, cwd=build_dir.parent, stdout=stream, stderr=subprocess.STDOUT).returncode


def prepare(project_dir, mode, compiler, jobs, log):
    native = project_dir / f"build-{mode}"
    build_dir = project_dir / "build-slate"
    if not build_dir.exists():
        shutil.copytree(native, build_dir, symlinks=True)
        shutil.copy2(native / "vmlinux.a", build_dir / "vmlinux.a.native")
        if make(build_dir, compiler, jobs, log):
            raise SystemExit(f"native relink of {build_dir} failed; see {log}")
    return build_dir


def relink(destination, inputs):
    subprocess.run(["ld", "-m", "elf_x86_64", "-r", "-o", str(destination), *map(str, inputs)], check=True)
    return destination, defined_globals([destination])


def runtime_object(copies, work, kernel_symbols, wanted):
    shared = {}
    for crate, by_shard in sorted(copies.items()):
        exports = {shard: frozenset().union(*symbol_table(paths, "-g", "--defined-only").values())
                   for shard, paths in by_shard.items()}
        if len(set(exports.values())) != 1:
            raise SystemExit(f"{crate} differs between shards {sorted(by_shard)}; build every shard with the same "
                             "toolchain and flags")
        shared[crate] = next(iter(by_shard.values()))
    archive = work / "runtime.a"
    subprocess.run(["ar", "rcs", str(archive), *(str(path) for paths in shared.values() for path in paths)],
                   check=True)
    subprocess.run(["objcopy", f"--localize-symbols={kernel_symbols}", str(archive)], check=True)
    pulled = sorted(wanted & defined_globals([archive]))
    if not pulled:
        return None, set()
    destination = work / "runtime.o"
    subprocess.run(["ld", "-m", "elf_x86_64", "-r", "-o", str(destination), *(f"-u{symbol}" for symbol in pulled),
                    "--start-group", str(archive), "--end-group"], check=True)
    subprocess.run(["objcopy", "--remove-section=.eh_frame", "--remove-section=.rela.eh_frame", str(destination)],
                   check=True)
    if probestack := unreferenced_probestack(destination):
        subprocess.run(["objcopy", f"--remove-section={probestack}", str(destination)], check=True)
    return destination, set().union(*symbol_table([destination], "-u").values())


def swap(shards, out, build_dir, native_dir, selected, profile, jobs=os.cpu_count() or 8):
    project_dir = build_dir.parent
    members = output(["ar", "t", "vmlinux.a.native"], build_dir).split()
    index = {name: position for position, name in enumerate(members)}
    work = build_dir / "slate"
    if work.exists():
        shutil.rmtree(work)
    work.mkdir()
    kernel_symbols = work / "kernel.symbols"
    kernel_symbols.write_text("".join(f"{name}\n" for name in sorted(defined_globals(members, build_dir))))

    plans = {}
    runtime_copies = collections.defaultdict(dict)
    for name in selected:
        staticlib = out / "shards" / name / "cargo-target" / TARGET / profile / "libslate_crate.a"
        if not staticlib.exists():
            continue
        units = link_units(out / "shards" / name / "crate", project_dir, native_dir)
        if not any(member in index for member, _ in units):
            continue
        extracted = work / name / "archive"
        extracted.mkdir(parents=True)
        subprocess.run(["ar", "x", str(staticlib)], cwd=extracted, check=True)
        objects = {path.name: path for path in sorted(extracted.iterdir())}
        for member, path in objects.items():
            if not member.startswith(CRATE_MEMBER):
                runtime_copies[member.split(".", 1)[0]].setdefault(name, []).append(path)
        groups = unit_groups(name, units, objects)
        check_groups(name, groups, any(module is None for _, module in units))
        plans[name] = (units, groups)

    crate_objects = [path for _, groups in plans.values() for group in groups.values() for path in group]
    undefined_by_object = symbol_table(crate_objects, "-u")
    undefined = {name: set().union(*(undefined_by_object.get(str(path), set())
                                     for group in groups.values() for path in group))
                 for name, (_, groups) in plans.items()}
    runtime, runtime_needs = runtime_object(runtime_copies, work, kernel_symbols, set().union(*undefined.values()))

    jobs_list = []
    placements = collections.defaultdict(list)
    removed = set()
    report = {}
    for name, (units, groups) in plans.items():
        root_member = next((member for member, module in units if module is None), None)
        linked = [(member, module) for member, module in units if member in index and module is not None]
        folded = [module for member, module in units if member not in index and module in groups]
        if built := [member for member, module in units if member not in index and (build_dir / member).exists()]:
            raise SystemExit(f"{name}: {built} are built but not in vmlinux.a under that name; their native "
                             "objects would stay linked next to the translated ones")
        root_inputs = [*groups.get(None, []), *(path for module in folded for path in groups[module])]
        anchor = root_member if root_member in index else min((member for member, _ in linked), key=index.get)
        if root_inputs:
            jobs_list.append((name, work / name / "__root.o", root_inputs, root_member if root_member in index
                              else None, anchor, True))
        for member, module in linked:
            if module in groups:
                jobs_list.append((name, work / name / f"{module}.o", groups[module], member, member, False))
        removed |= {index[member] for member, _ in units if member in index}
        report[name] = {"units": len(units), "linked_units": len(linked) + (root_member in index),
                        "folded_units": len(folded), "missing": []}

    with concurrent.futures.ThreadPoolExecutor(jobs) as pool:
        linked_objects = list(pool.map(lambda job: relink(job[1], job[2]), jobs_list))
    native_exports = symbol_table(sorted({job[3] for job in jobs_list if job[3]}), "-g", "--defined-only",
                                  cwd=build_dir)
    claimed = set()
    trims = []
    for (name, destination, _, member, anchor, root), (_, defined) in zip(jobs_list, linked_objects):
        exported = native_exports.get(member, set()) if member else set()
        keep = exported | (defined & undefined[name]) | ((defined & runtime_needs) - claimed)
        claimed |= defined & runtime_needs
        keep_file = destination.with_suffix(".keep")
        keep_file.write_text("".join(f"{symbol}\n" for symbol in sorted(keep or {"__slate_keep_none"})))
        trims.append(destination)
        slot = placements[index[anchor]]
        slot.insert(0 if root else len(slot), destination)
        report[name]["missing"] += sorted(exported - defined)

    with concurrent.futures.ThreadPoolExecutor(jobs) as pool:
        list(pool.map(lambda path: subprocess.run(
            ["objcopy", f"--keep-global-symbols={path.with_suffix('.keep')}", "--remove-section=.eh_frame",
             "--remove-section=.rela.eh_frame", str(path)], check=True), trims))
    owners = collections.defaultdict(list)
    for path, names in symbol_table([*trims, *([runtime] if runtime else [])], "-g", "--defined-only",
                                    skip_types="vVwW").items():
        for symbol in names:
            owners[symbol].append(path)
    if duplicates := {symbol: paths for symbol, paths in owners.items() if len(paths) > 1}:
        raise SystemExit(f"{len(duplicates)} symbols defined by several slate objects, e.g. "
                         f"{dict(list(duplicates.items())[:5])}")

    archive = []
    for position, member in enumerate(members):
        if position in placements:
            if runtime is not None:
                archive.append(os.path.relpath(runtime, build_dir))
                runtime = None
            archive += [os.path.relpath(path, build_dir) for path in placements[position]]
        elif position not in removed:
            archive.append(member)
    (build_dir / "vmlinux.a").unlink()
    subprocess.run(["ar", "cDPrST", "vmlinux.a", *archive], cwd=build_dir, check=True)
    return report


def link_errors(log):
    clusters = collections.defaultdict(lambda: [0, ""])
    objtool = collections.Counter()
    for line in log.read_text(errors="replace").splitlines():
        if match := re.search(r"(undefined reference to|multiple definition of) `([^']*)'", line):
            cluster = clusters[f"{match[1]} {re.sub(r'[0-9]+', 'N', match[2])}"]
        elif match := re.search(r"objtool: [^ ]*: (.*)", line):
            objtool[re.sub(r"_R[A-Za-z0-9_.]+|0x[0-9a-f]+", "_", match[1])] += 1
            continue
        elif re.search(r"\berror\b|Error \d|^ERROR: ", line) and "__sub-make" not in line and "Makefile" not in line:
            cluster = clusters[re.sub(r"[0-9]+", "N", line.strip())]
        else:
            continue
        cluster[0] += 1
        cluster[1] = cluster[1] or line.strip()
    return clusters, objtool


def link_stage(shards, out, project_dir, mode, compiler, selected, profile, jobs):
    build_dir = prepare(project_dir, mode, compiler, jobs, out / "link-native.log")
    swapped = swap(shards, out, build_dir, project_dir / f"build-{mode}", selected, profile)
    log = out / "link.log"
    code = make(build_dir, compiler, jobs, log)
    clusters, objtool = link_errors(log)
    report = [f"# link: vmlinux {'failed' if code else 'ok'}, {len(swapped)} shards swapped into "
              f"{build_dir}/vmlinux.a", "",
              "| count | error | example |", "| --- | --- | --- |"]
    report += [f"| {count} | {cluster} | `{example}` |"
               for cluster, (count, example) in sorted(clusters.items(), key=lambda item: -item[1][0])]
    report += ["", "| count | objtool warning |", "| --- | --- |"]
    report += [f"| {count} | {warning} |" for warning, count in objtool.most_common()]
    report += ["", "| shard | units in vmlinux.a | exported symbols missing |", "| --- | --- | --- |"]
    report += [f"| `{name}` | {entry['linked_units']}/{entry['units']} | {', '.join(entry['missing'][:5]) or '-'} |"
               for name, entry in swapped.items()]
    (out / "link.md").write_text("\n".join(report) + "\n")
    return code, {"swapped": len(swapped), "errors": sum(count for count, _ in clusters.values()),
                  "objtool_warnings": sum(objtool.values()), "report": "link.md"}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("out", type=Path, help="e2e.py output directory with shards.json and shards/")
    parser.add_argument("--project-dir", type=Path, default=Path.home() / "c-corpus/linux")
    parser.add_argument("--mode", default="clang")
    parser.add_argument("--profile", default="release")
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 8)
    parser.add_argument("shards", nargs="*", help="shards to swap in (default: all)")
    args = parser.parse_args()
    out = args.out.resolve()
    shards = json.loads((out / "shards.json").read_text())
    code, details = link_stage(shards, out, args.project_dir.resolve(), args.mode, "clang",
                               args.shards or list(shards), args.profile, args.jobs)
    json.dump(details, sys.stdout)
    print()
    return code


if __name__ == "__main__":
    sys.exit(main())
