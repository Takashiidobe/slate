#!/usr/bin/env python3
import argparse
import re
import subprocess
import sys
from pathlib import Path

from update_filecheck import placement


def clang_layouts(source: Path, clang: str, target: str | None) -> dict[str, tuple[int, int, list[int], list[int | None]]]:
    command = [clang]
    if target is not None:
        command.extend(["-target", target])
    command.extend(["-std=c23", "-Xclang", "-fdump-record-layouts-complete", "-fsyntax-only", str(source)])
    result = subprocess.run(
        command,
        capture_output=True,
        text=True,
        check=True,
    )
    layouts = {}
    for block in result.stdout.split("*** Dumping AST Record Layout"):
        header = re.search(r"^\s*0 \| (struct|union) (\w+)\s*$", block, re.MULTILINE)
        size = re.search(r"\[sizeof=(\d+), align=(\d+)\]", block)
        if header is None or size is None:
            continue
        offsets = []
        bits = []
        for line in block.splitlines():
            if "|" not in line:
                continue
            position, field = line.split("|", 1)
            if len(field) - len(field.lstrip()) != 3:
                continue
            match = re.fullmatch(r"\s*(\d+)(?::(?:(\d+)(?:-\d+)?|-))?\s*", position)
            if match is None:
                continue
            byte = int(match.group(1))
            offsets.append(byte)
            bits.append(byte * 8 + int(match.group(2) or 0) if ":" in position else None)
        layouts[header.group(2)] = (int(size.group(1)), int(size.group(2)), offsets, bits)
    return layouts


def slate_layouts(source: Path, binary: Path, target: str | None) -> dict[str, tuple[int, int, list[int], list[int | None]]]:
    command = [str(binary), "parse", str(source), "-std=c23", "--dump-ir-types"]
    if target is not None:
        command.append(f"--target={target}")
    result = subprocess.run(
        command,
        capture_output=True,
        text=True,
        check=True,
    )
    layouts = {}
    pattern = re.compile(
        r"type @type\d+ (\w+) = (?:struct|union) \{.*?\} "
        r"\[size=(\d+), align=(\d+), offsets=\[([^]]*)\](?:, bit_offsets=\[([^]]*)\])?",
        re.DOTALL,
    )
    for match in pattern.finditer(result.stdout):
        offsets = [int(value) for value in match.group(4).split(", ") if value]
        bits = ([None if value == "None" else int(value.removeprefix("Some(").removesuffix(")"))
                for value in match.group(5).split(", ") if value]
                if match.group(5) is not None else [None] * len(offsets))
        layouts[match.group(1)] = (int(match.group(2)), int(match.group(3)), offsets, bits)
    return layouts


def fixture_target(source: Path) -> str | None:
    try:
        return placement(source)[1]
    except ValueError:
        return None


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("--clang", default="clang-22")
    parser.add_argument("--binary", type=Path, default=Path("target/debug/slate-parser"))
    parser.add_argument("--target")
    args = parser.parse_args()
    target = args.target or fixture_target(args.source)
    expected = clang_layouts(args.source, args.clang, target)
    actual = slate_layouts(args.source, args.binary, target)
    mismatches = []
    for name, layout in expected.items():
        if name.startswith("__"):
            continue
        if actual.get(name) != layout:
            mismatches.append(f"{name}: clang={layout}, slate={actual.get(name)}")
    if mismatches:
        print("\n".join(mismatches), file=sys.stderr)
        return 1
    print(f"matched {len(expected) - sum(name.startswith('__') for name in expected)} clang record layouts")
    return 0


if __name__ == "__main__":
    sys.exit(main())
