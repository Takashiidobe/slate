#!/usr/bin/env python3
"""Extract #define blocks for missing macros from an oracle header, in oracle
source order, for splicing into libc-shim during header porting.

Usage:
    python3 tools/extract_missing_macros.py <missing-names-file> <oracle-header> [out-file]

<missing-names-file> is a newline-separated list of macro names, e.g. from:
    grep -oP 'elf\\.h:\\K[A-Za-z0-9_]+(?= macro is missing)' matrix-errors.log | sort -u

Output preserves the oracle's original textual order (including backslash
line continuations) so found blocks can be reviewed and placed relative to
their neighboring, already-reconciled declarations instead of alphabetically,
which is how the generated declaration-matrix fixture lists them.
"""

import re
import sys


def extract(missing_names, oracle_lines):
    missing = set(missing_names)
    found = {}
    i = 0
    n = len(oracle_lines)
    while i < n:
        line = oracle_lines[i]
        m = re.match(r"#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)", line)
        if m and m.group(1) in missing and m.group(1) not in found:
            block = [line]
            while block[-1].rstrip().endswith("\\"):
                i += 1
                block.append(oracle_lines[i])
            found[m.group(1)] = block
        i += 1
    return found, missing - found.keys()


def main(argv):
    if len(argv) not in (3, 4):
        print(__doc__, file=sys.stderr)
        return 2
    missing_path, oracle_path = argv[1], argv[2]
    out_path = argv[3] if len(argv) == 4 else None

    missing_names = [l.strip() for l in open(missing_path) if l.strip()]
    oracle_lines = open(oracle_path).read().split("\n")

    found, notfound = extract(missing_names, oracle_lines)

    print(f"found {len(found)} / {len(missing_names)} macros as #define lines", file=sys.stderr)
    if notfound:
        print("NOT FOUND (may be enum/typedef/other, not a plain #define):", file=sys.stderr)
        for name in sorted(notfound):
            print(" ", name, file=sys.stderr)

    out = sys.stdout if out_path is None else open(out_path, "w")
    for block in found.values():
        out.write("\n".join(block) + "\n")
    if out_path is not None:
        out.close()

    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
