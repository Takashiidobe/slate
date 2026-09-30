#!/usr/bin/env python3
"""Compare slate's predefined macros with a real compiler's.

usage: gcc_macro_diff.py '<compiler + flags>' '<slate-parser args>'
e.g.   gcc_macro_diff.py 'gcc -m32 -march=x86-64-v3' '--flavor=gcc -target=i686-unknown-linux-gnu -march=x86-64-v3'

Checks every macro name the compiler defines, the checked-in predefine
snapshots mention, or the ISA generators emit. Needs target/test-cache/release/slate-parser.
"""
import glob
import pathlib
import re
import subprocess
import sys
import tempfile

compiler, slate_args = sys.argv[1].split(), sys.argv[2].split()
root = pathlib.Path(__file__).resolve().parents[1]

defined = subprocess.run(
    [*compiler, "-dM", "-E", "-x", "c", "/dev/null"], capture_output=True, text=True, check=True
).stdout
names = set(re.findall(r"^#define (\w+)[ \n]", defined, re.M))
for path in glob.glob(str(root / "src/predefines/*.h")):
    names |= set(re.findall(r"^#define (\w+)", open(path).read(), re.M))
for path in glob.glob(str(root / "src/target/*.rs")):
    names |= set(re.findall(r'"(__[A-Za-z0-9_]+?)(?:=|")', open(path).read()))
names = sorted(names - {"S", "S2"})

probe = ["#define S2(...) #__VA_ARGS__", "#define S(...) S2(__VA_ARGS__)"]
for name in names:
    probe += [f"#ifdef {name}", f"char m_{name}[] = S({name});", "#endif"]
with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as source:
    source.write("\n".join(probe) + "\n")

expanded = subprocess.run(
    [*compiler, "-E", "-P", source.name], capture_output=True, text=True, check=True
).stdout
theirs = {m[1]: eval(m[2]) for m in re.finditer(r'char m_(\w+)\[\] = (".*");', expanded)}

rendered = subprocess.run(
    [str(root.parents[1] / "target/test-cache/release/slate-parser"), "parse", source.name, "--dump-ir", "--compact-ir", *slate_args],
    capture_output=True,
    text=True,
)
pathlib.Path(source.name).unlink()
if rendered.returncode:
    sys.exit(rendered.stderr)
ours = {
    m[1]: bytes(int(b) for b in m[2].split(", ") if b).rstrip(b"\0").decode()
    for m in re.finditer(r"global %\d+ m_(\w+): .*?code_units<[^>]*>>\(\[([0-9, ]*)\]\)", rendered.stdout)
}

normalize = lambda value: None if value is None else re.sub(r"\s+", " ", value)
differences = 0
for name in names:
    if normalize(theirs.get(name)) != normalize(ours.get(name)):
        differences += 1
        print(f"{name}: compiler={theirs.get(name)!r} slate={ours.get(name)!r}")
print(f"-- {differences} differences over {len(names)} names")
