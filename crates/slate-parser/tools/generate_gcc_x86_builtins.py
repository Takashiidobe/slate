#!/usr/bin/env python3

import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import generate_clang_builtins as clang  # noqa: E402

PRIMITIVES = {
    "VOID": "void",
    "CHAR": "char",
    "UCHAR": "unsigned char",
    "QI": "char",
    "HI": "short",
    "SI": "int",
    "DI": "long long int",
    "UQI": "unsigned char",
    "UHI": "unsigned short",
    "USI": "unsigned int",
    "UDI": "unsigned long long int",
    "SHORT": "short",
    "USHORT": "unsigned short",
    "INT": "int",
    "UINT": "unsigned int",
    "UNSIGNED": "unsigned int",
    "ULONG": "unsigned long int",
    "LONGLONG": "long long int",
    "ULONGLONG": "unsigned long long int",
    "UINT8": "unsigned char",
    "UINT16": "unsigned short",
    "INT64": "long long int",
    "UINT64": "unsigned long long int",
    "FLOAT16": "_Float16",
    "BFLOAT16": "__bf16",
    "FLOAT": "float",
    "DOUBLE": "double",
    "FLOAT80": "long double",
    "FLOAT128": "__float128",
    "CONST_STRING": "char const*",
}

COMI_TYPES = {True: "INT_FTYPE_V2DF_V2DF", False: "INT_FTYPE_V4SF_V4SF"}

CATEGORY_ATTRIBUTES = {
    "special_args": [],
    "pure_args": ["Pure"],
    "cet": [],
    "ace": [],
}

REGISTRATION_ATTRIBUTES = {
    "def_builtin": [],
    "def_builtin_const": ["Const"],
    "def_builtin_pure": ["Pure"],
}


def split_arguments(text):
    arguments = []
    depth = 0
    current = []
    in_string = False
    for char in text:
        if char == '"':
            in_string = not in_string
        if not in_string:
            if char in "([":
                depth += 1
            elif char in ")]":
                depth -= 1
            elif char == "," and depth == 0:
                arguments.append("".join(current).strip())
                current = []
                continue
        current.append(char)
    arguments.append("".join(current).strip())
    return arguments


def calls(text, name_pattern):
    for match in re.finditer(rf"(?<![\w.>])({name_pattern})\s*\(", text):
        depth = 1
        index = match.end()
        in_string = False
        while depth:
            char = text[index]
            if char == '"':
                in_string = not in_string
            elif not in_string:
                depth += {"(": 1, ")": -1}.get(char, 0)
            index += 1
        yield match.group(1), split_arguments(text[match.end() : index - 1])


def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    return re.sub(r"//[^\n]*", "", text)


def type_tables(path):
    primitives, vectors, pointers, functions, aliases = {}, {}, {}, {}, []
    for line in path.read_text().splitlines():
        match = re.match(r"\s*(DEF_\w+)\s*\(([^)]*)\)", line)
        if not match:
            continue
        directive = match.group(1)
        args = [arg.strip() for arg in match.group(2).split(",")]
        if directive == "DEF_PRIMITIVE_TYPE":
            primitives[args[0]] = PRIMITIVES[args[0]]
        elif directive == "DEF_VECTOR_TYPE":
            vectors[args[0]] = args[1]
        elif directive == "DEF_POINTER_TYPE":
            pointers[args[0]] = (args[1], len(args) == 3 and args[2] == "CONST")
        elif directive == "DEF_FUNCTION_TYPE":
            name = args[0] + "_FTYPE_" + ("VOID" if len(args) == 1 else "_".join(args[1:]))
            functions[name] = args
        elif directive == "DEF_FUNCTION_TYPE_ALIAS":
            aliases.append((args[0], args[1]))
        else:
            raise ValueError(f"unknown directive {directive}")
    for base, suffix in aliases:
        functions[f"{base}_{suffix}"] = functions[base]
    spellings = dict(primitives)
    for name, base in vectors.items():
        lanes = int(re.match(r"V(\d+)", name).group(1))
        spellings[name] = f"_Vector<{lanes}, {spellings[base]}>"
    for name, (base, constant) in pointers.items():
        spellings[name] = spellings[base] + (" const*" if constant else "*")
    return spellings, functions


def function_spelling(name, spellings, functions):
    if name not in functions:
        raise ValueError(f"unknown function type {name}")
    ret, *params = functions[name]
    if name.endswith("_FTYPE_VOID"):
        params = []
    return f"{spellings[ret]}({', '.join(spellings[ty] for ty in params)})"


def feature_names(*masks):
    names = []
    for mask in masks:
        for part in re.findall(r"OPTION_MASK_ISA2?_(\w+)", mask):
            names.append(part.lower())
    return ",".join(sorted(set(names)))


def descriptor_entries(path):
    text = strip_comments(path.read_text())
    category = None
    for name, arguments in calls(text, r"BDESC_FIRST|BDESC\b"):
        if name == "BDESC_FIRST":
            category = arguments[0]
            arguments = arguments[2:]
        mask, mask2, _icode, builtin, code, _comparison, flag = arguments
        if not builtin.startswith('"'):
            continue
        yield category, mask, mask2, json.loads(builtin), code, flag.replace("(int)", "").strip()


def category_type(category, mask, code, flag):
    if category == "comi":
        return COMI_TYPES[mask == "OPTION_MASK_ISA_SSE2"]
    if category == "pcmpestr":
        return (
            "V16QI_FTYPE_V16QI_INT_V16QI_INT_INT"
            if code == "IX86_BUILTIN_PCMPESTRM128"
            else "INT_FTYPE_V16QI_INT_V16QI_INT_INT"
        )
    if category == "pcmpistr":
        return (
            "V16QI_FTYPE_V16QI_V16QI_INT"
            if code == "IX86_BUILTIN_PCMPISTRM128"
            else "INT_FTYPE_V16QI_V16QI_INT"
        )
    return flag


def type_aliases(path):
    return dict(re.findall(r"#define\s+(MULTI_ARG_\w+)\s+(\w+_FTYPE_\w+)", path.read_text()))


def collect(source):
    spellings, functions = type_tables(source / "i386-builtin-types.def")
    aliases = type_aliases(source / "i386-builtins.h")
    entries = {}

    def add(name, code, mask, mask2, type_name, attributes):
        spelling = function_spelling(aliases.get(type_name, type_name), spellings, functions)
        entry = (
            name,
            code,
            spelling,
            sorted({"NoThrow", *attributes}),
            feature_names(mask, mask2),
            "OPTION_MASK_ISA_64BIT" in mask,
        )
        if name in entries and entries[name] != entry:
            raise ValueError(f"conflicting definitions of {name}: {entries[name]} vs {entry}")
        entries[name] = entry

    for category, mask, mask2, name, code, flag in descriptor_entries(source / "i386-builtin.def"):
        if category in ("comi", "pcmpestr", "pcmpistr", "args", "round_args", "multi_arg"):
            attributes = ["Const"]
        else:
            attributes = CATEGORY_ATTRIBUTES[category]
        add(name, code, mask, mask2, category_type(category, mask, code, flag), attributes)

    text = strip_comments((source / "i386-builtins.cc").read_text())
    for function, arguments in calls(text, r"def_builtin(?:_const|_pure)?"):
        if len(arguments) != 5 or not arguments[2].startswith('"'):
            continue
        mask, mask2, name, type_name, code = arguments
        add(json.loads(name), code, mask, mask2, type_name, REGISTRATION_ATTRIBUTES[function])
    return sorted(entries.values())


def table_lines(table, entries, prototypes):
    lines = [f"pub(super) static {table}: &[ClangBuiltin] = &["]
    for name, code, spelling, attributes, features, _ in entries:
        attrs = ", ".join(f"BuiltinAttribute::{attribute}" for attribute in attributes)
        feature = "None" if not features else json.dumps(features)
        feature = feature if feature == "None" else f"Some({feature})"
        lines.extend(
            [
                "    ClangBuiltin {",
                f"        name: {json.dumps(name)},",
                f"        record: {json.dumps(code)},",
                f"        prototype: {clang.prototype_field(prototypes, spelling)},",
                "        kind: ClangBuiltinKind::Builtin,",
                f"        attributes: &[{attrs}],",
                "        languages: None,",
                "        header: None,",
                f"        features: {feature},",
                "    },",
            ]
        )
    lines.append("];")
    return lines


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--gcc-source", type=Path, required=True, help="the gcc/ directory of a GCC checkout")
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    entries = collect(args.gcc_source / "config" / "i386")
    prototypes = {}
    for spelling in sorted({entry[2] for entry in entries}):
        prototypes[spelling] = f"GCC_X86_PROTOTYPE_{len(prototypes):04}"
    lines = []
    for spelling, name in prototypes.items():
        try:
            lines.extend(clang.prototype_lines(name, spelling))
        except clang.PrototypeError as error:
            raise ValueError(f"prototype `{spelling}`: {error}") from error
        lines.append("")
    both = [entry for entry in entries if not entry[5]]
    only_64 = [entry for entry in entries if entry[5]]
    lines.extend(table_lines("GCC_X86_BUILTINS", both, prototypes))
    lines.append("")
    lines.extend(table_lines("GCC_X86_64_BUILTINS", only_64, prototypes))
    args.output.write_text("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
