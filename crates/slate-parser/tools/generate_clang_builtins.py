#!/usr/bin/env python3

import argparse
import json
import re
import subprocess
from pathlib import Path

TOKEN = re.compile(r"\.\.\.|[A-Za-z_][A-Za-z_0-9]*|\d+|[*&(),<>]|\S")

QUALIFIERS = {"const", "volatile", "restrict"}

INTEGERS = {
    "short": ("Short", True),
    "unsigned short": ("Short", False),
    "int": ("Int", True),
    "unsigned int": ("Int", False),
    "long int": ("Long", True),
    "unsigned long int": ("Long", False),
    "long long int": ("LongLong", True),
    "unsigned long long int": ("LongLong", False),
    "__int128_t": ("Int128", True),
    "__uint128_t": ("Int128", False),
}

FLOATS = {
    "__bf16": "BFloat16",
    "_Float16": "Float16",
    "__fp16": "Fp16",
    "float": "Float",
    "double": "Double",
    "long double": "LongDouble",
    "__float128": "Float128",
    "_Decimal32": "Decimal32",
    "_Decimal64": "Decimal64",
    "_Decimal128": "Decimal128",
}

FIXED_INTEGERS = {
    "int8_t": (8, True),
    "int16_t": (16, True),
    "int32_t": (32, True),
    "int64_t": (64, True),
    "uint8_t": (8, False),
    "uint16_t": (16, False),
    "uint32_t": (32, False),
    "uint64_t": (64, False),
}

SIMPLE = {
    "void": "Void",
    "bool": "Bool",
    "char": "Char",
    "signed char": "SChar",
    "unsigned char": "UChar",
    "size_t": "SizeT",
    "ptrdiff_t": "PtrdiffT",
    "wchar_t": "WcharT",
    "__builtin_va_list": "VaList",
    "__builtin_va_list_ref": "VaListRef",
}


VECTORS = {"_ExtVector": "ExtVector", "_Vector": "Vector"}


class PrototypeError(ValueError):
    pass


def rust_string(value):
    if value is None:
        return "None"
    return f"Some({json.dumps(value)})"


def quals_expr(quals):
    names = [f"Qualifiers::{name.upper()}" for name in sorted(quals)]
    if not names:
        return "Qualifiers::NONE"
    expression = names[0]
    for name in names[1:]:
        expression = f"{expression}.union({name})"
    return expression


def param_expr(ty, quals, constant):
    return (
        f"BuiltinParam {{ ty: {ty}, quals: {quals_expr(quals)}, "
        f"constant: {str(constant).lower()} }}"
    )


def base_expr(spelling):
    if spelling in SIMPLE:
        return f"&BuiltinType::{SIMPLE[spelling]}"
    if spelling in INTEGERS:
        rank, signed = INTEGERS[spelling]
        return (
            f"&BuiltinType::Int {{ rank: IntRank::{rank}, signed: {str(signed).lower()} }}"
        )
    if spelling in FLOATS:
        return f"&BuiltinType::Float(FloatKind::{FLOATS[spelling]})"
    if spelling in FIXED_INTEGERS:
        bits, signed = FIXED_INTEGERS[spelling]
        return f"&BuiltinType::FixedInt {{ bits: {bits}, signed: {str(signed).lower()} }}"
    if spelling.startswith("_Complex "):
        return f"&BuiltinType::Complex({base_expr(spelling[len('_Complex '):])})"
    return f"&BuiltinType::Opaque({json.dumps(spelling)})"


class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.index = 0

    def peek(self):
        return self.tokens[self.index] if self.index < len(self.tokens) else None

    def consume(self):
        token = self.peek()
        if token is None:
            raise PrototypeError("unexpected end of prototype")
        self.index += 1
        return token

    def expect(self, token):
        found = self.consume()
        if found != token:
            raise PrototypeError(f"expected `{token}`, found `{found}`")

    def at_end(self):
        return self.peek() is None

    def vector(self, variant):
        self.expect("<")
        lanes = self.consume()
        if not lanes.isdigit():
            raise PrototypeError(f"vector lane count `{lanes}`")
        self.expect(",")
        inner = self.take_until(">")
        split = next((i for i, token in enumerate(inner) if token in ("*", "&")), len(inner))
        self.tokens[self.index : self.index] = inner[split:]
        element = Parser(inner[:split])
        ty, quals, constant = element.base()
        if constant or not element.at_end():
            raise PrototypeError(f"vector element `{' '.join(inner)}`")
        return f"&BuiltinType::{variant} {{ lanes: {lanes}, element: &{param_expr(ty, set(), False)} }}", quals

    def take_until(self, closing):
        taken = []
        while self.peek() != closing:
            taken.append(self.consume())
        self.consume()
        return taken

    def base(self):
        if self.peek() in VECTORS:
            ty, quals = self.vector(VECTORS[self.consume()])
            return ty, quals, False
        words = []
        quals = set()
        constant = False
        while True:
            token = self.peek()
            if token in QUALIFIERS:
                quals.add(self.consume())
                continue
            if token == "_Constant":
                self.consume()
                constant = True
                continue
            if token is None or not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", token):
                break
            words.append(self.consume())
        if words[:1] == ["signed"] and words[1:] != ["char"]:
            words = words[1:]
        if not words:
            raise PrototypeError("missing type specifier")
        return base_expr(" ".join(words)), quals, constant

    def parameter(self):
        constant = False
        if self.peek() == "_Constant":
            self.consume()
            constant = True
        ty, quals, inner_constant = self.base()
        constant = constant or inner_constant
        while self.peek() in ("*", "&"):
            wrapper = "Pointer" if self.consume() == "*" else "Reference"
            ty = f"&BuiltinType::{wrapper}(&{param_expr(ty, quals, False)})"
            quals = set()
            while self.peek() in QUALIFIERS:
                quals.add(self.consume())
        if not self.at_end():
            raise PrototypeError(f"trailing `{self.peek()}` in parameter")
        return param_expr(ty, quals, constant)


def split_parameters(tokens):
    parameters = []
    current = []
    depth = 0
    for token in tokens:
        if token == "<":
            depth += 1
        elif token == ">":
            depth -= 1
        if token == "," and depth == 0:
            parameters.append(current)
            current = []
            continue
        current.append(token)
    if current:
        parameters.append(current)
    return parameters


def parse_prototype(spelling):
    tokens = TOKEN.findall(spelling)
    depth = 0
    for position, token in enumerate(tokens):
        if token == "<":
            depth += 1
        elif token == ">":
            depth -= 1
        elif token == "(" and depth == 0:
            break
    else:
        raise PrototypeError(f"no parameter list in `{spelling}`")
    if tokens[-1] != ")":
        raise PrototypeError(f"unterminated parameter list in `{spelling}`")
    ret = Parser(tokens[:position]).parameter()
    groups = split_parameters(tokens[position + 1 : -1])
    variadic = bool(groups) and groups[-1] == ["..."]
    if variadic:
        groups.pop()
    if any("..." in group for group in groups):
        raise PrototypeError(f"misplaced ellipsis in `{spelling}`")
    parameters = [Parser(group).parameter() for group in groups]
    return ret, parameters, variadic


def prototype_lines(name, spelling):
    ret, parameters, variadic = parse_prototype(spelling)
    lines = [
        f"static {name}: BuiltinPrototype = BuiltinPrototype {{",
        f"    spelling: {json.dumps(spelling)},",
        f"    ret: {ret},",
    ]
    if parameters:
        lines.append("    params: &[")
        lines.extend(f"        {parameter}," for parameter in parameters)
        lines.append("    ],")
    else:
        lines.append("    params: &[],")
    lines.extend([f"    variadic: {str(variadic).lower()},", "};"])
    return lines


def prototype_field(prototypes, spelling):
    if not spelling:
        return "None"
    return f"Some(&{prototypes[spelling]})"


def language_variant(spelling):
    return "".join(word.capitalize() for word in spelling.split("_"))


def expand(data, record):
    substitutions = record.get("Substitutions", [None])
    affixes = record.get("Affixes", [""])
    if len(substitutions) != len(affixes):
        raise ValueError(f"mismatched template fields for {record['!name']}")
    prefix = record.get("RequiredNamePrefix")
    prefix = "" if prefix is None else data[prefix["def"]]["Spelling"]
    for substitution, affix in zip(substitutions, affixes):
        prototype = record.get("Prototype", "")
        if substitution is not None:
            prototype = prototype.replace("T", substitution)
        for spelling in record.get("Spellings", []):
            name = affix + spelling if record.get("AsPrefix", 0) else spelling + affix
            yield prefix + name, prototype


def kind(record):
    classes = set(record["!superclasses"])
    if "AtomicBuiltin" in classes:
        return "Atomic"
    if "LibBuiltin" in classes or "TargetLibBuiltin" in classes:
        return "Library"
    if "LangBuiltin" in classes:
        return "Language"
    return "Builtin"


def attribute_name(data, item):
    name = item["def"]
    if not name.startswith("anonymous_"):
        return name
    bases = {"Attribute", "IndexedAttribute", "MultiIndexAttribute"}
    classes = [
        superclass
        for superclass in data[name]["!superclasses"]
        if superclass not in bases
    ]
    if not classes:
        raise ValueError(f"attribute {name} has no semantic class")
    return classes[-1]


TABLES = [
    ("CLANG_BUILTINS", "Builtins.td"),
    ("CLANG_X86_BUILTINS", "BuiltinsX86.td"),
    ("CLANG_X86_64_BUILTINS", "BuiltinsX86_64.td"),
]


def table_records(clang_tblgen, clang, source):
    command = [
        str(clang_tblgen),
        "--dump-json",
        "-I",
        str(clang / "include"),
        str(clang / "include/clang/Basic" / source),
    ]
    data = json.loads(subprocess.run(command, check=True, capture_output=True, text=True).stdout)
    records = []
    for record_name in data["!instanceof"]["Builtin"]:
        record = data[record_name]
        attributes = [attribute_name(data, item) for item in record.get("Attributes", [])]
        for name, prototype in expand(data, record):
            entries = [(name, prototype, kind(record))]
            if record.get("AddBuiltinPrefixedAlias", 0):
                entries.append((f"__builtin_{name}", prototype, "Builtin"))
            for spelling, spelling_prototype, spelling_kind in entries:
                records.append(
                    (
                        spelling,
                        record_name,
                        spelling_prototype,
                        spelling_kind,
                        attributes,
                        record.get("Languages"),
                        record.get("Header"),
                        record.get("Features") or None,
                    )
                )
    records.sort(key=lambda item: item[0])
    return records


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--clang-tblgen", type=Path, required=True)
    parser.add_argument("--llvm-project", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    clang = args.llvm_project / "clang"
    tables = [
        (table, table_records(args.clang_tblgen, clang, source)) for table, source in TABLES
    ]
    prototypes = {}
    spellings = {item[2] for _, records in tables for item in records if item[2]}
    for spelling in sorted(spellings):
        prototypes[spelling] = f"PROTOTYPE_{len(prototypes):04}"
    lines = []
    for spelling, name in prototypes.items():
        try:
            lines.extend(prototype_lines(name, spelling))
        except PrototypeError as error:
            raise ValueError(f"prototype `{spelling}`: {error}") from error
        lines.append("")
    for table, records in tables:
        lines.extend(table_lines(table, records, prototypes))
    args.output.write_text("\n".join(lines) + "\n")


def table_lines(table, records, prototypes):
    lines = [f"pub(super) static {table}: &[ClangBuiltin] = &["]
    for name, record, prototype, builtin_kind, attributes, languages, header, features in records:
        attrs = ", ".join(f"BuiltinAttribute::{attribute}" for attribute in attributes)
        language = (
            "None" if languages is None else f"Some(BuiltinLanguage::{language_variant(languages)})"
        )
        lines.extend(
            [
                "    ClangBuiltin {",
                f"        name: {json.dumps(name)},",
                f"        record: {json.dumps(record)},",
                f"        prototype: {prototype_field(prototypes, prototype)},",
                f"        kind: ClangBuiltinKind::{builtin_kind},",
                f"        attributes: &[{attrs}],",
                f"        languages: {language},",
                f"        header: {rust_string(header)},",
                f"        features: {rust_string(features)},",
                "    },",
            ]
        )
    lines.append("];")
    return lines


if __name__ == "__main__":
    main()
