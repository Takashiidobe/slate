#!/usr/bin/env python3

import argparse
import collections
import json
import pathlib
import re
import subprocess


ROOT = pathlib.Path(__file__).resolve().parents[1]
PREFIX = "unsupported slate-parser IR: "
DROP_WITH_VALUE = {"-o", "-MF", "-MT", "-MQ"}
DROP = re.compile(r"^-(c|g.*|O.*|M[DM]?|MMD|MP)$")
CATEGORIES = [
    (re.compile(r"^(extern|zero-initialized|initialized) global "), lambda m: f"{m[1]} global"),
    (re.compile(r"^type @type\d+\((\w+) "), lambda m: f"type {m[1]}"),
    (re.compile(r"^type fn\("), lambda m: "type fn"),
    (re.compile(r"^place Place \{.*?\bkind: (\w+)"), lambda m: f"place {m[1]}"),
    (re.compile(r"^(value|statement|place|constant|arithmetic) (\w+)"), lambda m: f"{m[1]} {m[2]}"),
    (
        re.compile(r"^(unprototyped function|unprototyped declaration|variadic definition|unknown callee) "),
        lambda m: m[1],
    ),
    (re.compile(r"^(type|integer type) (.+)$"), lambda m: f"{m[1]} {m[2]}"),
]


def category(message):
    message = message.removeprefix(PREFIX)
    for pattern, name in CATEGORIES:
        match = pattern.match(message)
        if match:
            return name(match)
    return message[:80]


def revision(repo):
    head = subprocess.run(
        ["git", "-C", str(repo), "rev-parse", "--short=12", "HEAD"],
        capture_output=True,
        text=True,
    ).stdout.strip()
    dirty = subprocess.run(
        ["git", "-C", str(repo), "status", "--porcelain", "--untracked-files=no", "--", ".", ":!.beads"],
        capture_output=True,
        text=True,
    ).stdout.strip()
    return f"{head}{'-dirty' if dirty else ''}"


def compiler_args(entry):
    arguments = entry.get("arguments") or entry["command"].split()
    source = entry["file"]
    kept = []
    skip = False
    for argument in arguments[1:]:
        if skip:
            skip = False
        elif argument in DROP_WITH_VALUE:
            skip = True
        elif argument == source or DROP.match(argument):
            continue
        else:
            kept.append(argument)
    return kept


def lower(binary, entry):
    directory = pathlib.Path(entry["directory"])
    command = [str(binary), "lowering-barriers", *compiler_args(entry), entry["file"]]
    result = subprocess.run(command, cwd=directory, capture_output=True, text=True)
    rows = [line.split("\t", 1) for line in result.stdout.splitlines() if "\t" in line]
    if result.returncode != 0 and not rows:
        error = result.stderr.strip().splitlines()
        return None, error[0].removeprefix("error: ") if error else f"exit {result.returncode}"
    return rows, None


def histogram_table(title, counter):
    lines = [f"### {title}", "", "| count | barrier |", "| ---: | --- |"]
    lines += [f"| {count} | `{name}` |" for name, count in counter.most_common()]
    return lines + [""]


def report(binary, compile_commands):
    entries = json.loads(compile_commands.read_text())
    functions = collections.Counter()
    module = collections.Counter()
    declarations = collections.Counter()
    tu_first = collections.Counter()
    per_tu = []
    for entry in sorted(entries, key=lambda entry: entry["file"]):
        rows, error = lower(binary, entry)
        if rows is None:
            tu_first[f"frontend: {category(error)}"] += 1
            per_tu.append((entry["file"], 0, 0, 0, 0, error))
            continue
        ok = total = module_count = declaration_count = 0
        first = None
        for scope, outcome in rows:
            if scope == "<module>":
                module_count += 1
                module[category(outcome)] += 1
            elif scope.startswith("<declaration "):
                declaration_count += 1
                declarations[category(outcome)] += 1
            else:
                total += 1
                if outcome == "ok":
                    ok += 1
                    continue
                functions[category(outcome)] += 1
            first = first or outcome
        tu_first[category(first) if first else "clean"] += 1
        per_tu.append((entry["file"], ok, total, module_count, declaration_count, first or "clean"))

    lines = [
        f"# Slate lowering barriers: {compile_commands}",
        "",
        "Regenerate with `python3 tools/corpus_barriers.py --output wiki/concepts/chibicc-lowering-barriers.md`.",
        "",
        f"- slate `{revision(ROOT)}`",
        f"- slate-parser `{revision(ROOT.parent / 'slate-parser')}`",
        f"- functions lowered: {sum(row[1] for row in per_tu)} / {sum(row[2] for row in per_tu)}",
        "",
        "| TU | functions ok | module barriers | declaration barriers | first barrier |",
        "| --- | ---: | ---: | ---: | --- |",
    ]
    lines += [
        f"| {tu} | {ok}/{total} | {module_count} | {declaration_count} | `{category(first)}` |"
        for tu, ok, total, module_count, declaration_count, first in per_tu
    ]
    lines.append("")
    lines += histogram_table("Defined functions by first barrier", functions)
    lines += histogram_table("Module-level barriers", module)
    lines += histogram_table("Declaration barriers", declarations)
    lines += histogram_table("TUs by first reported barrier", tu_first)
    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--compile-commands",
        type=pathlib.Path,
        default=pathlib.Path.home() / "c-corpus/chibicc/compile_commands.json",
    )
    parser.add_argument("--binary", type=pathlib.Path, default=ROOT.parents[1] / "target/test-cache/release/slate")
    parser.add_argument("--output", type=pathlib.Path)
    args = parser.parse_args()
    text = report(args.binary, args.compile_commands)
    if args.output:
        args.output.write_text(text + "\n")
    print(text)


if __name__ == "__main__":
    main()
