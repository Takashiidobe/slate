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

import kernel_boot

TARGETS = ("breakpoints capabilities clone3 core exec filesystems futex ipc kcmp membarrier memfd mincore mm mount "
           "mount_setattr move_mount_set_group mqueue pidfd pipe prctl proc ptrace rlimits rseq seccomp signal "
           "splice static_keys sync syscall_user_dispatch timens timers tmpfs uevent x86").split()
HOST_TOOLS = {"bin/bash": "/usr/bin/bash", "usr/bin/timeout": "/usr/bin/timeout"}
INIT = """#!/bin/sh
mount -t proc proc /proc
mount -t sysfs sysfs /sys
mount -t devtmpfs devtmpfs /dev
mkdir -p /dev/pts /dev/shm /dev/mqueue
mount -t devpts devpts /dev/pts
mount -t tmpfs tmpfs /dev/shm
mount -t mqueue mqueue /dev/mqueue
mount -t tmpfs tmpfs /tmp
mount -t tmpfs tmpfs /run
mount -t debugfs debugfs /sys/kernel/debug
mount -t tracefs tracefs /sys/kernel/tracing
ip link set lo up
cd /kselftest
only=$(sed -n 's/.*kselftest=\\([^ ]*\\).*/\\1/p' /proc/cmdline)
if [ -n "$only" ]; then
  echo "$only" | tr , '\\n' | while read pattern; do grep -E "^$pattern(:|$)" kselftest-list.txt; done > /tmp/list
else
  cp kselftest-list.txt /tmp/list
fi
number=0
while IFS=: read collection test; do
  number=$((number + 1))
  seconds=$(sed -n 's/^timeout=//p' "$collection/settings" 2>/dev/null)
  seconds=${{seconds:-45}}
  [ "$seconds" = 0 ] && seconds={cap}
  echo "# selftests: $collection: $test"
  (cd "$collection" && exec /usr/bin/timeout -k 10 "$seconds" "./$test") > /tmp/output 2>&1 < /dev/null
  code=$?
  kill -KILL -1 2>/dev/null
  sed 's/^/# /' /tmp/output
  case $code in
    0) echo "ok $number selftests: $collection: $test" ;;
    4) echo "ok $number selftests: $collection: $test # SKIP" ;;
    124|137) echo "not ok $number selftests: $collection: $test # TIMEOUT $seconds seconds" ;;
    *) echo "not ok $number selftests: $collection: $test # exit=$code" ;;
  esac
done < /tmp/list
echo "{marker}"
sync
poweroff -f
"""
RESULT = re.compile(r"^(not ok|ok) \d+ selftests: (\S+): (\S+)(.*)$")
HEADER = re.compile(r"^# selftests: (\S+): (\S+)$")
SUBTEST = re.compile(r"^# (not ok|ok) \d+ (.*?)\s*$")


def tap_status(verdict, rest):
    if "# SKIP" in rest:
        return "skip"
    if "# TIMEOUT" in rest:
        return "timeout"
    return "ok" if verdict == "ok" else "fail"


def generic_glibc(cache):
    root = cache / "glibc-root"
    if not root.is_dir():
        staging = cache / ".glibc-root"
        shutil.rmtree(staging, ignore_errors=True)
        for name, directory in (("glibc", "usr/lib"), ("lib32-glibc", "usr/lib32")):
            shutil.copytree(generic_package(name, cache) / directory, staging / directory, symlinks=True)
        (staging / "usr/lib/32").symlink_to("../lib32")
        staging.rename(root)
    return root / "usr/lib"


def build(project_dir, directory, jobs):
    head = subprocess.run(["git", "rev-parse", "HEAD"], cwd=project_dir, capture_output=True, text=True).stdout
    compiler = f"gcc -B{generic_glibc(directory / 'packages')}/"
    stamp = json.dumps({"targets": TARGETS, "head": head.strip(), "cc": compiler})
    install = directory / "install"
    if (directory / "stamp.json").exists() and (directory / "stamp.json").read_text() == stamp:
        return install
    for stale in ("build", "install", "usr", "khdr"):
        shutil.rmtree(directory / stale, ignore_errors=True)
    log = directory / "build.log"
    with open(log, "w") as stream:
        steps = [["git", "clean", "-fdX", "--", *(f"tools/testing/selftests/{target}" for target in TARGETS)],
                 ["make", f"O={directory / 'khdr'}", "ARCH=x86_64", "headers_install",
                  f"INSTALL_HDR_PATH={directory / 'usr'}", f"-j{jobs}"],
                 ["make", "-C", "tools/testing/selftests", f"O={directory / 'build'}", f"TARGETS={' '.join(TARGETS)}",
                  f"KHDR_INCLUDES=-isystem {directory / 'usr/include'}", f"CC={compiler}", f"INSTALL_PATH={install}",
                  f"-j{jobs}", "install"]]
        for step in steps:
            if subprocess.run(step, cwd=project_dir, stdout=stream, stderr=subprocess.STDOUT).returncode:
                raise SystemExit(f"kselftest build failed; see {log}")
    (directory / "stamp.json").write_text(stamp)
    return install


def is_elf(path):
    with open(path, "rb") as stream:
        return stream.read(4) == b"\x7fELF"


def shared_libraries(paths):
    libraries = set()
    for path in paths:
        result = subprocess.run(["ldd", str(path)], capture_output=True, text=True)
        libraries.update(token for token in re.findall(r"/\S+", result.stdout) if Path(token).is_file())
    return sorted(libraries)


def arch_mirrors():
    servers = re.findall(r"^Server\s*=\s*(\S+)", Path("/etc/pacman.d/mirrorlist").read_text(), re.M)
    return servers or ["https://geo.mirror.pkgbuild.com/$repo/os/$arch"]


def generic_package(name, cache):
    root = cache / name
    if root.is_dir():
        return root
    for repo in ("core", "extra", "multilib"):
        info = subprocess.run(["pacman", "-Si", f"{repo}/{name}"], capture_output=True, text=True).stdout
        if fields := dict(re.findall(r"^(Version|Architecture)\s*:\s*(\S+)", info, re.M)):
            break
    else:
        raise SystemExit(f"no generic x86_64 package for {name} in core, extra or multilib")
    file = f"{name}-{fields['Version']}-{fields['Architecture']}.pkg.tar.zst"
    archive = cache / file
    for server in arch_mirrors():
        url = server.replace("$repo", repo).replace("$arch", "x86_64") + "/" + file
        if not subprocess.run(["curl", "-fsL", "-o", str(archive), url]).returncode:
            break
    else:
        raise SystemExit(f"could not download {file}")
    staging = cache / f".{name}"
    shutil.rmtree(staging, ignore_errors=True)
    staging.mkdir()
    subprocess.run(["bsdtar", "-xf", str(archive), "-C", str(staging)], check=True)
    staging.rename(root)
    return root


def generic_files(paths, cache):
    cache.mkdir(exist_ok=True)
    resolved = {path: str(Path(path).resolve()) for path in paths}
    owners = subprocess.run(["pacman", "-Qoq", *sorted(set(resolved.values()))], capture_output=True, text=True,
                            check=True).stdout.split()
    package = dict(zip(sorted(set(resolved.values())), owners))
    return {path: generic_package(package[real], cache) / real.lstrip("/") for path, real in resolved.items()}


def initramfs(path, install, busybox, cap):
    newest = max(path_.stat().st_mtime for path_ in (install.parent / "stamp.json", busybox, Path(__file__)))
    if path.exists() and path.stat().st_mtime > newest:
        return path
    files = {}
    for source in sorted(install.rglob("*")):
        name = "kselftest/" + str(source.relative_to(install))
        if source.is_symlink():
            files[name] = (0o120777, os.readlink(source).encode())
        elif source.is_dir():
            files[name] = (0o040755, b"")
        else:
            files[name] = (0o100000 | (source.stat().st_mode & 0o7777), source.read_bytes())
    files["bin/busybox"] = (0o100755, busybox.read_bytes())
    applets = subprocess.run([str(busybox), "--list-full"], check=True, capture_output=True, text=True).stdout.split()
    for applet in applets:
        files.setdefault(applet, (0o120777, b"/bin/busybox"))
    elves = [source for source in install.rglob("*") if source.is_file() and not source.is_symlink() and is_elf(source)]
    libraries = shared_libraries(elves + [Path(source) for source in HOST_TOOLS.values()])
    generic = generic_files([*HOST_TOOLS.values(), *libraries], install.parent / "packages")
    for name, source in HOST_TOOLS.items():
        files[name] = (0o100755, generic[source].read_bytes())
    for library in libraries:
        files[library.lstrip("/")] = (0o100755, generic[library].read_bytes())
    files["etc/passwd"] = (0o100644, b"root:x:0:0:root:/root:/bin/sh\nnobody:x:65534:65534:nobody:/:/bin/false\n")
    files["etc/group"] = (0o100644, b"root:x:0:\nnobody:x:65534:\n")
    files["init"] = (0o100755, INIT.format(cap=cap, marker=kernel_boot.MARKER).encode())
    for directory in ("proc", "sys", "tmp", "run", "root"):
        files[directory] = (0o040755, b"")
    for name in list(files):
        parent = Path(name).parent
        while str(parent) != ".":
            files.setdefault(str(parent), (0o040755, b""))
            parent = parent.parent
    entries = [(name, mode, data, 0) for name, (mode, data) in sorted(files.items())]
    kernel_boot.write_cpio(path, entries)
    return path


def parse(lines):
    tests, subtests = {}, {}
    current = None
    for line in lines:
        line = line.replace("\r", "").rstrip()
        if match := HEADER.match(line):
            current = f"{match[1]}:{match[2]}"
        elif match := RESULT.match(line):
            tests[f"{match[2]}:{match[3]}"] = tap_status(match[1], match[4])
            current = None
        elif current and (match := SUBTEST.match(line)):
            name, _, directive = match[2].partition(" # ")
            key = f"{current}/{name}"
            status = tap_status(match[1], " # " + directive if directive else "")
            subtests[key] = status if key not in subtests or subtests[key] == status else "fail"
    return tests, subtests


def compare(native_runs, translated):
    stable, flaky = {}, set()
    for key in set().union(*native_runs):
        statuses = {run.get(key, "missing") for run in native_runs}
        if len(statuses) == 1:
            stable[key] = statuses.pop()
        else:
            flaky.add(key)
    regressions, fixed = {}, {}
    for key, expected in stable.items():
        actual = translated.get(key, "missing")
        if actual != expected:
            (fixed if expected in ("fail", "timeout") else regressions)[key] = (expected, actual)
    extra = sorted(set(translated) - set(stable) - flaky)
    return stable, flaky, regressions, fixed, extra


def selftest_stage(out, project_dir, mode, compiler, jobs, timeout=3600, memory="2G", cpus=2, native_runs=2,
                   append="", test_cap=1200, only=()):
    append = f"{append} kselftest={','.join(only)}".strip() if only else append
    directory = out / "kselftest"
    directory.mkdir(exist_ok=True)
    busybox = out / "boot/busybox"
    if not busybox.exists():
        raise SystemExit(f"{busybox} is missing: put a generic static x86_64 busybox there")
    install = build(project_dir, directory, jobs)
    initrd = initramfs(directory / "initramfs.cpio.gz", install, busybox, test_cap)
    native_image = kernel_boot.bzimage(project_dir / f"build-{mode}", compiler, jobs, directory / "bzimage-native.log")
    image = kernel_boot.bzimage(project_dir / "build-slate", compiler, jobs, directory / "bzimage.log")
    boots = [(native_image, f"native-{index}.log") for index in range(1, native_runs + 1)]
    with concurrent.futures.ThreadPoolExecutor(len(boots) + 1) as pool:
        runs = list(pool.map(lambda job: kernel_boot.boot(job[0], initrd, directory / job[1], timeout, memory, cpus,
                                                          append), boots + [(image, "translated.log")]))
    natives, translated = runs[:-1], runs[-1]
    native_parsed = [parse(run["lines"]) for run in natives]
    tests, subtests = parse(translated["lines"])
    test_cmp = compare([parsed[0] for parsed in native_parsed], tests)
    subtest_cmp = compare([parsed[1] for parsed in native_parsed], subtests)
    native_failures = collections.Counter(kernel_boot.normalize_line(line)
                                          for run in natives for line in run["failures"])
    new_failures = [line for line in translated["failures"] if not native_failures[kernel_boot.normalize_line(line)]]
    ok = translated["marker"] and not test_cmp[2] and not subtest_cmp[2]

    def counts(results):
        return dict(sorted(collections.Counter(results.values()).items()))

    report = [f"# kselftest: {'ok' if ok else 'failed'}", "",
              f"{len(TARGETS)} collections built from tools/testing/selftests"
              f"{'; selected: ' + ', '.join(only) if only else ''}; native booted {native_runs}x, "
              "results that differ between native runs are flaky and not compared.", "",
              "| kernel | marker | seconds | timed out | tests | subtests |", "| --- | --- | --- | --- | --- | --- |"]
    for name, run, (run_tests, run_subtests) in [*((f"native {index}", run, parsed) for index, (run, parsed)
                                                  in enumerate(zip(natives, native_parsed), 1)),
                                                 ("translated", translated, (tests, subtests))]:
        report.append(f"| {name} | {'yes' if run['marker'] else 'no'} | {run['seconds']} | "
                      f"{'yes' if run['timed_out'] else 'no'} | {counts(run_tests)} | {counts(run_subtests)} |")
    for title, (stable, flaky, regressions, fixed, extra) in (("Tests", test_cmp), ("Subtests", subtest_cmp)):
        report += ["", f"## {title}: {len(stable)} compared, {len(regressions)} regressions, {len(flaky)} flaky", ""]
        if regressions:
            report += ["| test | native | translated |", "| --- | --- | --- |",
                       *[f"| `{key}` | {expected} | {actual} |" for key, (expected, actual)
                         in sorted(regressions.items())]]
        if fixed:
            report += ["", "Fails natively, differs translated:", "",
                       *[f"- `{key}`: {expected} -> {actual}" for key, (expected, actual) in sorted(fixed.items())]]
        if flaky:
            report += ["", "Flaky natively: " + ", ".join(f"`{key}`" for key in sorted(flaky))]
        if extra:
            report += ["", "Only in translated: " + ", ".join(f"`{key}`" for key in extra)]
    if new_failures:
        report += ["", "## Kernel failure lines only in the translated run", "", "```", *new_failures[:60], "```"]
    (out / "kselftest.md").write_text("\n".join(report) + "\n")
    return (0 if ok else 1), {"marker": translated["marker"], "seconds": translated["seconds"],
                              "tests": len(test_cmp[0]), "regressions": len(test_cmp[2]),
                              "subtests": len(subtest_cmp[0]), "subtest_regressions": len(subtest_cmp[2]),
                              "flaky": len(test_cmp[1]) + len(subtest_cmp[1]), "kernel_failures": len(new_failures),
                              "report": "kselftest.md"}


def main():
    parser = argparse.ArgumentParser(description="run kernel selftests on native and translated bzImages under QEMU")
    parser.add_argument("out", type=Path, help="e2e.py output directory")
    parser.add_argument("--project-dir", type=Path, default=Path.home() / "c-corpus/linux")
    parser.add_argument("--mode", default="clang")
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 8)
    parser.add_argument("--timeout", type=int, default=3600)
    parser.add_argument("--memory", default="2G")
    parser.add_argument("--cpus", type=int, default=2)
    parser.add_argument("--native-runs", type=int, default=2)
    parser.add_argument("--append", default="", help="extra kernel command line")
    parser.add_argument("--only", type=lambda value: value.split(","), default=(),
                        help="comma-separated collections or collection:test entries to run")
    args = parser.parse_args()
    code, details = selftest_stage(args.out.resolve(), args.project_dir.resolve(), args.mode, "clang", args.jobs,
                                   args.timeout, args.memory, args.cpus, args.native_runs, args.append, only=args.only)
    json.dump(details, sys.stdout)
    print()
    return code


if __name__ == "__main__":
    sys.exit(main())
