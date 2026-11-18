#!/usr/bin/env python3
import argparse
import ctypes
import ctypes.util
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

CHARSETS = {
    37: ["IBM037", "CP037", "EBCDIC-CP-US", "CSIBM037"],
    1047: ["IBM1047", "CP1047", "IBM-1047", "1047"],
    437: ["IBM437", "CP437", "437"],
    737: ["CP737"],
    775: ["IBM775", "CP775"],
    850: ["IBM850", "CP850"],
    852: ["IBM852", "CP852"],
    855: ["IBM855", "CP855"],
    857: ["IBM857", "CP857"],
    860: ["IBM860", "CP860"],
    861: ["IBM861", "CP861"],
    862: ["IBM862", "CP862"],
    863: ["IBM863", "CP863"],
    864: ["IBM864", "CP864"],
    865: ["IBM865", "CP865"],
    866: ["IBM866", "CP866"],
    869: ["IBM869", "CP869"],
    874: ["CP874", "WINDOWS-874"],
    1250: ["CP1250", "WINDOWS-1250"],
    1251: ["CP1251", "WINDOWS-1251"],
    1252: ["CP1252", "WINDOWS-1252"],
    1253: ["CP1253", "WINDOWS-1253"],
    1254: ["CP1254", "WINDOWS-1254"],
    1256: ["CP1256", "WINDOWS-1256"],
    1257: ["CP1257", "WINDOWS-1257"],
    10000: ["MACINTOSH", "MAC"],
    20127: ["ASCII", "US-ASCII", "ANSI_X3.4-1968"],
    20866: ["KOI8-R"],
    21866: ["KOI8-U"],
    28591: ["ISO-8859-1", "ISO8859-1", "LATIN1"],
    28592: ["ISO-8859-2", "ISO8859-2", "LATIN2"],
    28593: ["ISO-8859-3", "ISO8859-3", "LATIN3"],
    28594: ["ISO-8859-4", "ISO8859-4", "LATIN4"],
    28595: ["ISO-8859-5", "ISO8859-5", "CYRILLIC"],
    28596: ["ISO-8859-6", "ISO8859-6", "ARABIC"],
    28597: ["ISO-8859-7", "ISO8859-7", "GREEK"],
    28598: ["ISO-8859-8", "ISO8859-8", "HEBREW"],
    28599: ["ISO-8859-9", "ISO8859-9", "LATIN5"],
    28603: ["ISO-8859-13", "ISO8859-13", "LATIN7"],
    28605: ["ISO-8859-15", "ISO8859-15", "LATIN-9"],
}

ENCODE_CANDIDATES = [
    *range(0x80, 0x250),
    *range(0x370, 0x700),
    *range(0x2000, 0x2600),
    *range(0xE000, 0xE100),
    *range(0xF000, 0xF100),
    *range(0xFB00, 0xFF00),
]

libc = ctypes.CDLL(ctypes.util.find_library("c"), use_errno=True)
libc.iconv_open.restype = ctypes.c_void_p
libc.iconv_open.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
libc.iconv.restype = ctypes.c_size_t
libc.iconv.argtypes = [
    ctypes.c_void_p,
    ctypes.POINTER(ctypes.c_char_p),
    ctypes.POINTER(ctypes.c_size_t),
    ctypes.POINTER(ctypes.c_char_p),
    ctypes.POINTER(ctypes.c_size_t),
]
libc.iconv_close.argtypes = [ctypes.c_void_p]


def convert(to: str, source: str, data: bytes) -> bytes | None:
    handle = libc.iconv_open(to.encode(), source.encode())
    if handle in (None, ctypes.c_void_p(-1).value):
        sys.exit(f"iconv does not know {source} -> {to}")
    try:
        output = ctypes.create_string_buffer(64)
        in_ptr = ctypes.c_char_p(data)
        in_left = ctypes.c_size_t(len(data))
        out_ptr = ctypes.cast(output, ctypes.c_char_p)
        out_left = ctypes.c_size_t(64)
        if libc.iconv(handle, ctypes.byref(in_ptr), ctypes.byref(in_left),
                      ctypes.byref(out_ptr), ctypes.byref(out_left)) == ctypes.c_size_t(-1).value:
            return None
        if libc.iconv(handle, None, None, ctypes.byref(out_ptr), ctypes.byref(out_left)) == ctypes.c_size_t(-1).value:
            return None
        return output.raw[: 64 - out_left.value]
    finally:
        libc.iconv_close(handle)


def decode_table(name: str) -> list[str | None]:
    table = []
    for byte in range(256):
        decoded = convert("UTF-32LE", name, bytes([byte]))
        if decoded is None:
            table.append(None)
        elif len(decoded) == 4:
            table.append(chr(int.from_bytes(decoded, "little")))
        else:
            sys.exit(f"{name}: byte {byte:02X} decodes to {decoded!r}")
    return table


def check_encoding(name: str, table: list[str | None]) -> None:
    inverse = {}
    for byte, c in enumerate(table):
        if c is not None:
            inverse.setdefault(c, byte)
    for value in sorted(set(ENCODE_CANDIDATES) | {ord(c) for c in inverse}):
        if 0xD800 <= value < 0xE000:
            continue
        encoded = convert(name, "UTF-32LE", value.to_bytes(4, "little"))
        expected = inverse.get(chr(value))
        if encoded is None and expected is None:
            continue
        if encoded is None or len(encoded) != 1 or encoded[0] != expected:
            sys.exit(f"{name}: U+{value:04X} encodes to {encoded!r}, decoding says {expected}")


def check_gcc(work: Path, name: str, table: list[str | None]) -> None:
    values = [ord(c) for c in table if c is not None and ord(c) not in (0,)]
    escapes = {"\\": "\\\\", '"': '\\"', "?": "\\?", "\n": "\\n", "\r": "\\r"}
    literal = "".join(escapes.get(chr(value), chr(value)) for value in values)
    source = work / "probe.c"
    source.write_text(
        "typedef __SIZE_TYPE__ size_t;\n"
        "size_t fwrite(const void *, size_t, size_t, void *);\n"
        "extern void *stdout;\n"
        f'static const char s[] = "{literal}";\n'
        "int main(void) { fwrite(s, 1, sizeof s - 1, stdout); return 0; }\n"
    )
    binary = work / "probe"
    result = subprocess.run(["gcc", f"-fexec-charset={name}", "-w", str(source), "-o", str(binary)],
                            capture_output=True, text=True)
    if result.returncode:
        sys.exit(f"gcc -fexec-charset={name} failed:\n{result.stderr}")
    output = subprocess.run([str(binary)], capture_output=True, check=True).stdout
    inverse = {}
    for byte, c in enumerate(table):
        if c is not None:
            inverse.setdefault(ord(c), byte)
    expected = bytes(inverse[value] for value in values)
    if output != expected:
        sys.exit(f"gcc -fexec-charset={name} disagrees with iconv")


def rust_entry(c: str | None) -> str:
    return "None" if c is None else f"Some('\\u{{{ord(c):04X}}}')"


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, default=ROOT / "src" / "gnu_charsets.rs")
    args = parser.parse_args()
    lines = [
        "use crate::charset::ByteTable;",
        "",
        f"pub(crate) static GNU_CHARSETS: [&ByteTable; {len(CHARSETS)}] = [",
        *(f"    &GNU{number}," for number in CHARSETS),
        "];",
    ]
    with tempfile.TemporaryDirectory() as directory:
        work = Path(directory)
        for number, names in CHARSETS.items():
            table = decode_table(names[0])
            for alias in names[1:]:
                if decode_table(alias) != table:
                    sys.exit(f"{alias} decodes differently from {names[0]}")
            check_encoding(names[0], table)
            for alias in names:
                check_gcc(work, alias, table)
            lines.append("")
            lines.append("#[rustfmt::skip]")
            lines.append(f"static GNU{number}: ByteTable = ByteTable {{")
            lines.append(f"    number: {number},")
            lines.append(f"    names: &[{', '.join(f'{chr(34)}{name.lower()}{chr(34)}' for name in names)}],")
            lines.append("    table: [")
            for row in range(0, 256, 8):
                lines.append("        " + ", ".join(rust_entry(table[byte]) for byte in range(row, row + 8)) + ",")
            lines.append("    ],")
            lines.append("};")
    args.output.write_text("\n".join(lines) + "\n")
    subprocess.run(["rustfmt", "--edition", "2024", str(args.output)], check=True)


if __name__ == "__main__":
    main()
