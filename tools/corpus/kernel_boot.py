import argparse
import collections
import gzip
import json
import os
import re
import subprocess
import sys
import tempfile
import time
from pathlib import Path

import kernel_link

MARKER = "SLATE-BOOT-MARKER: init reached"
INIT = r"""
static long sys(long n, long a, long b, long c, long d, long e) {
  long r;
  register long r10 __asm__("r10") = d;
  register long r8 __asm__("r8") = e;
  __asm__ volatile("syscall" : "=a"(r) : "a"(n), "D"(a), "S"(b), "d"(c), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
  return r;
}

void _start(void) {
  static const char marker[] = "\n%s\n";
  sys(1, 1, (long)marker, sizeof marker - 1, 0, 0);
  sys(162, 0, 0, 0, 0, 0);
  sys(169, 0xfee1dead, 672274793, 0x4321fedc, 0, 0);
  for (;;)
    sys(34, 0, 0, 0, 0, 0);
}
"""
FAILURE = re.compile(r"Kernel panic|\bBUG:|Oops|general protection fault|WARNING:|Call Trace:|RIP: |"
                     r"stack-protector|UBSAN|KASAN|refcount_t:|list_(add|del) corruption|soft lockup|hung_task|"
                     r"rcu_.*(stall|self-detected)|segfault")
VOLATILE = [
    (re.compile(r"\x1b\[[0-9;?]*[A-Za-z]|\x1bc"), ""),
    (re.compile(r"^.*?\[\s*\d+\.\d+\]\s?"), ""),
    (re.compile(r"^Linux version .*"), "Linux version _"),
    (re.compile(r"(Not tainted|Tainted: \S+) (\S+) #\d+"), r"\1 \2 #N"),
    (re.compile(r"^CPU: \d+ "), "CPU: N "),
    (re.compile(r"0x[0-9a-fA-F]+"), "0x_"),
    (re.compile(r"\b[0-9a-f]{8,16}\b"), "_"),
    (re.compile(r"\b\d+(\.\d+)?\s*(K|KB|kB|MB|MiB|ms|us|usecs|ns|MHz|BogoMIPS|jiffies|secs?)\b"), "N\\2"),
    (re.compile(r"(lpj|max_idle_ns|max_cycles|ts|took|in|after|delay|count|mult|shift)[=: ]+\d+"), "\\1=N"),
    (re.compile(r"\d+\.\d+"), "N.N"),
    (re.compile(r"audit\([^)]*\)"), "audit(_)"),
    (re.compile(r"\b\d+k\b"), "Nk"),
    (re.compile(r"Embedded \d+ pages/cpu s\d+ r\d+ d\d+ u\d+"), "Embedded N pages/cpu sN rN dN uN"),
]
MANGLED = re.compile(r"_RNvNtCs([0-9A-Za-z]+)_[0-9A-Za-z_]+")
NOISE = re.compile(r"^ \? |used greatest stack depth|PM: RTC time|sched_clock: Marking stable|"
                   r"Refined TSC clocksource|clocksource: tsc: mask|Switched to clocksource tsc")


def initramfs(path):
    if path.exists() and path.stat().st_mtime > Path(__file__).stat().st_mtime:
        return path
    with tempfile.TemporaryDirectory() as scratch:
        source = Path(scratch) / "init.c"
        binary = Path(scratch) / "init"
        source.write_text(INIT % MARKER)
        subprocess.run(["clang", "-static", "-nostdlib", "-ffreestanding", "-fno-stack-protector", "-O2",
                        "-o", binary, source], check=True)
        write_cpio(path, [("proc", 0o040755, b"", 0), ("init", 0o100755, binary.read_bytes(), 0)])
    return path


def write_cpio(path, entries):
    entries = [("dev", 0o040755, b"", 0), ("dev/console", 0o020600, b"", os.makedev(5, 1)), *entries]
    archive = bytearray()
    for inode, (name, mode, data, device) in enumerate(entries + [("TRAILER!!!", 0, b"", 0)], 1):
        encoded = name.encode() + b"\0"
        fields = [inode, mode, 0, 0, 1, 0, len(data), 0, 0, os.major(device), os.minor(device), len(encoded), 0]
        archive += b"070701" + b"".join(b"%08X" % field for field in fields) + encoded
        archive += b"\0" * (-len(archive) % 4) + data
        archive += b"\0" * (-len(archive) % 4)
    archive += b"\0" * (-len(archive) % 512)
    path.write_bytes(gzip.compress(bytes(archive), compresslevel=6))


def bzimage(build_dir, compiler, jobs, log):
    image = build_dir / "arch/x86/boot/bzImage"
    if not image.exists() or image.stat().st_mtime < (build_dir / "vmlinux").stat().st_mtime:
        if kernel_link.make(build_dir, compiler, jobs, log, "bzImage"):
            raise SystemExit(f"make bzImage in {build_dir} failed; see {log}")
    return image


def boot(image, initrd, log, timeout, memory, cpus, append):
    command = ["qemu-system-x86_64", "-kernel", image, "-initrd", initrd, "-m", memory, "-smp", str(cpus),
               "-nographic", "-no-reboot", "-append",
               f"console=ttyS0 rdinit=/init panic=-1 oops=panic {append}".strip()]
    command += ["-enable-kvm", "-cpu", "host"] if os.access("/dev/kvm", os.R_OK | os.W_OK) else ["-cpu", "max"]
    start = time.monotonic()
    with open(log, "wb") as stream:
        try:
            subprocess.run([str(part) for part in command], stdout=stream, stderr=subprocess.STDOUT,
                           stdin=subprocess.DEVNULL, timeout=timeout)
            timed_out = False
        except subprocess.TimeoutExpired:
            timed_out = True
    seconds = round(time.monotonic() - start, 1)
    lines = log.read_text(errors="replace").splitlines()
    return {"marker": any(MARKER in line for line in lines), "timed_out": timed_out, "seconds": seconds,
            "failures": [line.strip() for line in lines if FAILURE.search(line)], "lines": lines}


def demangle(match):
    text, index = match[0], len("_RNvNtCs") + len(match[1]) + 1
    names = []
    while index < len(text) and text[index].isdigit():
        digits = re.match(r"\d+", text[index:])[0]
        index += len(digits)
        if text[index:index + 1] == "_":
            index += 1
        names.append(text[index:index + int(digits)])
        index += int(digits)
    return names[-1] + text[index:] if len(names) == 3 else text


def normalize_line(line):
    line = MANGLED.sub(demangle, line.replace("\r", ""))
    for pattern, replacement in VOLATILE:
        line = pattern.sub(replacement, line)
    return line


def normalize(lines):
    kernel = []
    started = False
    for line in lines:
        line = normalize_line(line)
        started = started or line.startswith("Linux version")
        if started and line.strip() and MARKER not in line and not NOISE.search(line):
            kernel.append(line.rstrip())
    return kernel


def divergence(native, translated):
    only_native = collections.Counter(native) - collections.Counter(translated)
    only_translated = collections.Counter(translated) - collections.Counter(native)
    first = next((index for index, line in enumerate(translated) if line in only_translated), None)
    return first, only_native, only_translated


def boot_stage(out, project_dir, mode, compiler, jobs, timeout=60, memory="2G", cpus=2, append=""):
    boot_dir = out / "boot"
    boot_dir.mkdir(exist_ok=True)
    initrd = initramfs(boot_dir / "initramfs-marker.cpio.gz")
    native_image = bzimage(project_dir / f"build-{mode}", compiler, jobs, boot_dir / "bzimage-native.log")
    image = bzimage(project_dir / "build-slate", compiler, jobs, boot_dir / "bzimage.log")
    native = boot(native_image, initrd, boot_dir / "native.log", timeout, memory, cpus, append)
    translated = boot(image, initrd, boot_dir / "translated.log", timeout, memory, cpus, append)
    reference, candidate = normalize(native["lines"]), normalize(translated["lines"])
    first, only_native, only_translated = divergence(reference, candidate)
    native_failures = collections.Counter(map(normalize_line, native["failures"]))
    new_failures = [line for line in translated["failures"] if not native_failures[normalize_line(line)]]
    ok = translated["marker"] and not new_failures
    report = [f"# boot: {'ok' if ok else 'failed'}", "",
              "| kernel | marker | seconds | timed out | failure lines |", "| --- | --- | --- | --- | --- |"]
    for name, result in (("native", native), ("translated", translated)):
        report.append(f"| {name} | {'yes' if result['marker'] else 'no'} | {result['seconds']} | "
                      f"{'yes' if result['timed_out'] else 'no'} | {len(result['failures'])} |")
    if new_failures:
        report += ["", "## Failure lines only in the translated boot", "", "```", *new_failures[:60], "```"]
    report += ["", f"## First divergence ({sum(only_native.values())} native-only, "
               f"{sum(only_translated.values())} translated-only normalized lines; reordering ignored)", ""]
    if first is None:
        report.append("none")
    else:
        report += ["```diff", *[f"  {line}" for line in candidate[max(0, first - 3):first]],
                   f"+ {candidate[first]}", "```"]
    for title, lines in (("Only in translated", only_translated), ("Only in native", only_native)):
        if lines:
            report += ["", f"## {title}", "", "```",
                       *[f"{count}x {line}" if count > 1 else line for line, count in list(lines.items())[:80]],
                       "```"]
    (out / "boot.md").write_text("\n".join(report) + "\n")
    (boot_dir / "native.normalized").write_text("\n".join(reference) + "\n")
    (boot_dir / "translated.normalized").write_text("\n".join(candidate) + "\n")
    return (0 if ok else 1), {"marker": translated["marker"], "native_marker": native["marker"],
                              "seconds": translated["seconds"], "failures": len(new_failures),
                              "only_translated": sum(only_translated.values()),
                              "only_native": sum(only_native.values()), "report": "boot.md"}


def main():
    parser = argparse.ArgumentParser(description="boot native and translated bzImages under QEMU and diff the logs")
    parser.add_argument("out", type=Path, help="e2e.py output directory")
    parser.add_argument("--project-dir", type=Path, default=Path.home() / "c-corpus/linux")
    parser.add_argument("--mode", default="clang")
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 8)
    parser.add_argument("--timeout", type=int, default=60)
    parser.add_argument("--memory", default="2G")
    parser.add_argument("--cpus", type=int, default=2)
    parser.add_argument("--append", default="", help="extra kernel command line")
    args = parser.parse_args()
    code, details = boot_stage(args.out.resolve(), args.project_dir.resolve(), args.mode, "clang", args.jobs,
                               args.timeout, args.memory, args.cpus, args.append)
    json.dump(details, sys.stdout)
    print()
    return code


if __name__ == "__main__":
    sys.exit(main())
