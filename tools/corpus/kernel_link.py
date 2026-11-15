import argparse
import collections
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

RUNTIME_CRATES = ("core-", "compiler_builtins-")
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


def shard_object(staticlib, native_objects, destination, build_dir, kernel_symbols):
    with tempfile.TemporaryDirectory() as scratch:
        scratch = Path(scratch)
        members = output(["ar", "t", str(staticlib)]).split()
        subprocess.run(["ar", "x", str(staticlib)], cwd=scratch, check=True)
        crate = [scratch / name for name in members if not name.startswith(RUNTIME_CRATES)]
        runtime = [str(scratch / name) for name in members if name.startswith(RUNTIME_CRATES)]
        subprocess.run(["ar", "rcs", str(scratch / "runtime.a"), *runtime], check=True)
        subprocess.run(["objcopy", f"--localize-symbols={kernel_symbols}", str(scratch / "runtime.a")], check=True)
        subprocess.run(["ld", "-m", "elf_x86_64", "-r", "-o", str(destination), *map(str, crate),
                        "--start-group", str(scratch / "runtime.a"), "--end-group"], check=True)
    exported = defined_globals(native_objects, build_dir)
    keep = destination.with_suffix(".keep")
    keep.write_text("".join(f"{name}\n" for name in sorted(exported)))
    subprocess.run(["objcopy", f"--keep-global-symbols={keep}", "--remove-section=.eh_frame",
                    "--remove-section=.rela.eh_frame", str(destination)], check=True)
    if probestack := unreferenced_probestack(destination):
        subprocess.run(["objcopy", f"--remove-section={probestack}", str(destination)], check=True)
    return sorted(exported - defined_globals([destination]))


def unreferenced_probestack(path):
    sections = re.findall(r"\] (\.text\.\S*__rust_probestack)\s", output(["readelf", "-SW", str(path)]))
    if len(sections) != 1:
        return None
    references = [line for line in output(["readelf", "-rW", str(path)]).splitlines()
                  if "__rust_probestack" in line and not line.startswith("Relocation section")]
    return None if references else sections[0]


def make(build_dir, compiler, jobs, log):
    command = ["make", f"O={build_dir.name}", f"CC={compiler}", f"HOSTCC={compiler}", f"-j{jobs}", "vmlinux"]
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


def swap(shards, out, build_dir, selected, profile):
    members = output(["ar", "t", "vmlinux.a.native"], build_dir).split()
    index = {name: position for position, name in enumerate(members)}
    objects = build_dir / "slate"
    if objects.exists():
        shutil.rmtree(objects)
    objects.mkdir()
    kernel_symbols = objects / "kernel.symbols"
    kernel_symbols.write_text("".join(f"{name}\n" for name in sorted(defined_globals(members, build_dir))))
    report = {}
    replaced = {}
    for name in selected:
        units = shards[name]
        linked = [obj for obj in (str(Path(unit).with_suffix(".o")) for unit in units) if obj in index]
        staticlib = out / "shards" / name / "cargo-target" / TARGET / profile / "libslate_crate.a"
        if not linked or not staticlib.exists():
            continue
        destination = objects / f"{name}.o"
        missing = shard_object(staticlib, linked, destination, build_dir, kernel_symbols)
        for obj in linked:
            replaced[index[obj]] = None
        replaced[min(index[obj] for obj in linked)] = os.path.relpath(destination, build_dir)
        report[name] = {"units": len(units), "linked_units": len(linked), "missing": missing}
    archive = [kept for position, member in enumerate(members) if (kept := replaced.get(position, member))]
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
    swapped = swap(shards, out, build_dir, selected, profile)
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
