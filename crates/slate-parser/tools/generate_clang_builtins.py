#!/usr/bin/env python3

import argparse
import json
import subprocess
from pathlib import Path


def rust_string(value):
    if value is None:
        return "None"
    return f"Some({json.dumps(value)})"


def expand(record):
    substitutions = record.get("Substitutions", [None])
    affixes = record.get("Affixes", [""])
    if len(substitutions) != len(affixes):
        raise ValueError(f"mismatched template fields for {record['!name']}")
    for substitution, affix in zip(substitutions, affixes):
        prototype = record.get("Prototype", "")
        if substitution is not None:
            prototype = prototype.replace("T", substitution)
        for spelling in record.get("Spellings", []):
            name = affix + spelling if record.get("AsPrefix", 0) else spelling + affix
            yield name, prototype


def kind(record):
    classes = set(record["!superclasses"])
    if "AtomicBuiltin" in classes:
        return "Atomic"
    if "TargetBuiltin" in classes:
        raise ValueError(f"target builtin {record['!name']} requires a target registry")
    if "LibBuiltin" in classes:
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


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--clang-tblgen", type=Path, required=True)
    parser.add_argument("--llvm-project", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    clang = args.llvm_project / "clang"
    command = [
        str(args.clang_tblgen),
        "--dump-json",
        "-I",
        str(clang / "include"),
        str(clang / "include/clang/Basic/Builtins.td"),
    ]
    data = json.loads(subprocess.run(command, check=True, capture_output=True, text=True).stdout)
    records = []
    for record_name in data["!instanceof"]["Builtin"]:
        record = data[record_name]
        attributes = [attribute_name(data, item) for item in record.get("Attributes", [])]
        for name, prototype in expand(record):
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
                        record.get("Features"),
                    )
                )
    records.sort(key=lambda item: item[0])
    lines = ["pub(super) static CLANG_BUILTINS: &[ClangBuiltin] = &["]
    for name, record, prototype, builtin_kind, attributes, languages, header, features in records:
        attrs = ", ".join(json.dumps(attribute) for attribute in attributes)
        lines.extend(
            [
                "    ClangBuiltin {",
                f"        name: {json.dumps(name)},",
                f"        record: {json.dumps(record)},",
                f"        prototype: {json.dumps(prototype)},",
                f"        kind: ClangBuiltinKind::{builtin_kind},",
                f"        attributes: &[{attrs}],",
                f"        languages: {rust_string(languages)},",
                f"        header: {rust_string(header)},",
                f"        features: {rust_string(features)},",
                "    },",
            ]
        )
    lines.append("];")
    args.output.write_text("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
