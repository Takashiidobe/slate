import json
import os
import re
import subprocess
import sys
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor

B = "target/test-cache/release/slate-parser"
Z = os.path.expanduser("~/c-corpus/zlib")
M = json.load(open("tools/corpus/matrices/zlib.json"))
TARGET = sys.argv[1] if len(sys.argv) > 1 else "linux-x86_64"
T = next(t for t in M["targets"] if t["name"] == TARGET)
ARGS = [
    f"--target={T['triple']}",
    f"--flavor={T['flavor']}",
    *[a.replace("-I", f"-I{Z}/", 1) if a.startswith("-I") else a for a in T["args"]],
    *[a.replace("-I.", f"-I{Z}") if a == "-I." else a for a in M["args"]],
]
LINE = re.compile(r"^tokens (\S+) `(.*)` context=\[.*?\] macros=\[.*?\] roots=\[(.*?)\]$")
EXP = re.compile(r"^expansion (\S+) (\S+)( empty)? context=\[.*?\] macros=\[.*?\] roots=\[(.*?)\]$")


def trace(src, extra):
    r = subprocess.run([B, "pp", f"{Z}/{src}", "--trace", *ARGS, *extra], capture_output=True, text=True)
    if r.returncode:
        return None
    sites = defaultdict(list)
    roots = defaultdict(set)
    for line in r.stdout.splitlines():
        m = LINE.match(line)
        if m:
            sites[m[1]].append(m[2])
            roots[m[1]].update(x for x in m[3].split(", ") if x)
            continue
        m = EXP.match(line)
        if m:
            roots[m[2]].update(x for x in m[4].split(", ") if x)
    return {k: "".join("".join(v).split()) for k, v in sites.items()}, roots


def check(job):
    src, feature, others = job
    macro = feature["defines"][0].split("=")[0]
    context = [f"-D{d}" for other in others for d in other["defines"]]
    base = trace(src, context)
    on = trace(src, context + [f"-D{d}" for d in feature["defines"]])
    if base is None or on is None:
        return src, feature["name"], None
    missed, changed, flagged_same, flagged_total = [], 0, 0, 0
    for site in sorted(set(base[0]) | set(on[0])):
        differs = base[0].get(site) != on[0].get(site)
        flag = macro in base[1].get(site, ()) or macro in on[1].get(site, ())
        changed += differs
        flagged_total += flag
        flagged_same += flag and not differs
        if differs and not flag:
            missed.append((site, base[0].get(site), on[0].get(site)))
    return src, feature["name"], (changed, missed, flagged_same, flagged_total)


F = M["features"]
MODE = sys.argv[2] if len(sys.argv) > 2 else "single"
if MODE == "single":
    jobs = [(s, f, []) for s in M["sources"] for f in F]
elif MODE == "all":
    jobs = [(s, f, [o for o in F if o is not f]) for s in M["sources"] for f in F]
else:
    jobs = [(s, f, [o]) for s in M["sources"] for f in F for o in F if o is not f]
print("mode", MODE)
with ThreadPoolExecutor(8) as pool:
    results = list(pool.map(check, jobs))
tot = defaultdict(lambda: [0, 0, 0, 0])
missed = []
failed = []
for src, name, r in results:
    if r is None:
        failed.append((src, name))
        continue
    t = tot[name]
    t[0] += r[0]
    t[1] += len(r[1])
    t[2] += r[2]
    t[3] += r[3]
    missed.extend((src, name, *m) for m in r[1])
print(TARGET, "pairs", len(results), "failed", failed)
print(f"{'feature':24}{'changed':>9}{'missed':>8}{'flagged':>9}{'flag-same':>11}")
for name, t in tot.items():
    print(f"{name:24}{t[0]:>9}{t[1]:>8}{t[3]:>9}{t[2]:>11}")
print("total changed", sum(t[0] for t in tot.values()), "missed", len(missed))
for m in missed[:25]:
    print(" MISS", m[0], m[1], m[2], "|", (m[3] or "<none>")[:90], "|", (m[4] or "<none>")[:90])
