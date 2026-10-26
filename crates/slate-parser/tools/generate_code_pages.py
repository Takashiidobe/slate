#!/usr/bin/env python3
import argparse
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CL = ROOT / "tools" / "cl.exe"

CODE_PAGES = {
    437: ["ibm437"],
    737: ["ibm737"],
    775: ["ibm775"],
    850: ["ibm850"],
    852: ["ibm852"],
    855: ["ibm855"],
    857: ["ibm857"],
    860: ["ibm860"],
    861: ["ibm861"],
    862: ["dos-862"],
    863: ["ibm863"],
    864: ["ibm864"],
    865: ["ibm865"],
    866: ["cp866"],
    869: ["ibm869"],
    874: [],
    1250: ["windows-1250"],
    1251: ["windows-1251"],
    1252: ["windows-1252"],
    1253: ["windows-1253"],
    1254: ["windows-1254"],
    1255: ["windows-1255"],
    1256: ["windows-1256"],
    1257: ["windows-1257"],
    1258: ["windows-1258"],
    10000: ["macintosh"],
    20127: ["us-ascii", "ascii"],
    20866: ["koi8-r"],
    21866: ["koi8-u"],
    28591: ["iso-8859-1"],
    28592: ["iso-8859-2"],
    28593: ["iso-8859-3"],
    28594: ["iso-8859-4"],
    28595: ["iso-8859-5"],
    28596: ["iso-8859-6"],
    28597: ["iso-8859-7"],
    28598: ["iso-8859-8"],
    28599: ["iso-8859-9"],
    28603: ["iso-8859-13"],
    28605: ["iso-8859-15"],
}

MASKED = {20127}

SKIPPED_BYTES = {0x00, 0x0A, 0x0D, 0x22, 0x27, 0x5C}


def run_cl(work: Path, source: bytes, *flags: str) -> str:
    path = work / "probe.c"
    path.write_bytes(source)
    listing = work / "probe.asm"
    listing.unlink(missing_ok=True)
    result = subprocess.run(
        [str(CL), "/nologo", "/c", "/FAs", f"/Fa{listing}", *flags, str(path)],
        capture_output=True,
        cwd=work,
    )
    if result.returncode:
        sys.exit(f"cl.exe failed for {flags}:\n{result.stdout.decode('latin-1')}")
    return listing.read_text(encoding="latin-1")


def listing_strings(listing: str) -> dict[int, bytes]:
    strings = {}
    for name, body in re.findall(r"^s(\d+)\s+DB\s+(.*(?:\n\s+DB\s+.*)*)", listing, re.M):
        units = []
        for part in re.findall(r"'[^']*'|[0-9a-fA-F]+H", body.replace("DB", "")):
            if part.startswith("'"):
                units.extend(ord(c) for c in part[1:-1])
            else:
                units.append(int(part[:-1], 16))
        if units[-1] != 0:
            sys.exit(f"unterminated string s{name}")
        strings[int(name)] = bytes(units[:-1])
    return strings


def predefined_code_page(work: Path, name: str) -> int:
    path = work / "name.c"
    path.write_text("int v = _MSVC_EXECUTION_CHARACTER_SET;\n")
    result = subprocess.run(
        [str(CL), "/nologo", "/EP", f"/execution-charset:{name}", str(path)],
        capture_output=True,
        cwd=work,
    )
    if result.returncode:
        sys.exit(f"cl.exe rejects /execution-charset:{name}")
    return int(re.search(rb"int v = (\d+);", result.stdout).group(1))


def decode_table(work: Path, number: int) -> list[str]:
    if number in MASKED:
        return [chr(byte & 0x7F) for byte in range(256)]
    probed = [byte for byte in range(256) if byte not in SKIPPED_BYTES]
    source = b"".join(
        b'const char s%d[] = "' % byte + bytes([byte]) + b'";\n' for byte in probed
    )
    strings = listing_strings(
        run_cl(work, source, f"/source-charset:.{number}", "/execution-charset:utf-8")
    )
    table = [chr(byte) for byte in range(256)]
    for byte in probed:
        decoded = strings[byte].decode("utf-8")
        if len(decoded) != 1:
            sys.exit(f"{number}: byte {byte:02X} decodes to {decoded!r}")
        if byte < 0x80 and decoded != chr(byte):
            sys.exit(f"{number}: ASCII byte {byte:02X} decodes to {ord(decoded):04X}")
        table[byte] = decoded
    return table


def encode_map(work: Path, number: int, candidates: list[str]) -> dict[str, int]:
    source = "".join(
        f'const char s{index}[] = "{c}";\n' for index, c in enumerate(candidates)
    ).encode("utf-8")
    strings = listing_strings(
        run_cl(work, source, "/source-charset:utf-8", f"/execution-charset:.{number}")
    )
    encoded = {}
    for index, c in enumerate(candidates):
        units = strings[index]
        if len(units) != 1:
            sys.exit(f"{number}: {ord(c):04X} encodes to {units!r}")
        if units != b"?" or c == "?":
            encoded[c] = units[0]
    return encoded


def rust_char(c: str) -> str:
    return f"'\\u{{{ord(c):04X}}}'"


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, default=ROOT / "src" / "code_pages.rs")
    args = parser.parse_args()
    with tempfile.TemporaryDirectory() as directory:
        work = Path(directory)
        for number, names in CODE_PAGES.items():
            for name in [*names, f".{number}"]:
                if predefined_code_page(work, name) != number:
                    sys.exit(f"/execution-charset:{name} is not code page {number}")
        decodes = {number: decode_table(work, number) for number in CODE_PAGES}
        candidates = sorted(
            {c for table in decodes.values() for c in table if ord(c) >= 0x80}
            | {chr(value) for value in range(0x80, 0x250)}
            | {chr(value) for value in range(0x2000, 0x2200)}
        )
        candidates = [c for c in candidates if c not in "\n\r\"\\" and ord(c) not in range(0xD800, 0xE000)]
        lines = [
            "use crate::charset::CodePage;",
            "",
            f"pub(crate) static CODE_PAGES: [&CodePage; {len(CODE_PAGES)}] = [",
            *(f"    &CP{number}," for number in CODE_PAGES),
            "];",
        ]
        for number, names in CODE_PAGES.items():
            decode = decodes[number]
            encoded = encode_map(work, number, candidates)
            inverse = {}
            for byte in range(256):
                inverse.setdefault(decode[byte], byte)
            for c, byte in encoded.items():
                if ord(c) >= 0x80 and inverse.get(c) != byte:
                    sys.exit(f"{number}: {ord(c):04X} encodes to {byte:02X}, not its decoding")
            for c, byte in inverse.items():
                if ord(c) >= 0x80 and c in candidates and encoded.get(c) != byte:
                    sys.exit(f"{number}: {ord(c):04X} decodes from {byte:02X} but does not encode")
            lines.append("")
            lines.append(f"pub(crate) static CP{number}: CodePage = CodePage {{")
            lines.append(f"    number: {number},")
            lines.append(f"    names: &[{', '.join(f'{chr(34)}{name}{chr(34)}' for name in names)}],")
            lines.append("    high: [")
            for row in range(0x80, 0x100, 8):
                lines.append("        " + ", ".join(rust_char(decode[byte]) for byte in range(row, row + 8)) + ",")
            lines.append("    ],")
            lines.append("};")
        args.output.write_text("\n".join(lines) + "\n")
        subprocess.run(["rustfmt", "--edition", "2024", str(args.output)], check=True)


if __name__ == "__main__":
    main()
