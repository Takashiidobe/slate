#!/usr/bin/env python3

import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import generate_clang_builtins as clang  # noqa: E402

DUMPS = Path(__file__).resolve().parent / "gcc_plugin" / "dumps"

FAMILIES = {
    "AARCH64": ["aarch64"],
    "ARM": ["arm", "arm-mve"],
}

ATTRIBUTES = {"const": ["Const"], "pure": ["Pure"], "": []}


def load(names):
    functions, types, tuples, enumerators = {}, {}, {}, {}
    for name in names:
        for line in (DUMPS / f"{name}.tsv").read_text().splitlines():
            kind, symbol, spelling, attributes, sizeless = line.split("\t")
            if sizeless == "1" and kind == "type":
                clang.SIZELESS[symbol] = symbol[2:].lower() if symbol.startswith("__SV") else symbol
                continue
            if kind == "enum":
                enumerators[symbol] = int(spelling)
                continue
            if kind == "fn" and attributes == "overload":
                continue
            if kind == "tuple":
                tuples[symbol] = (spelling, int(attributes))
                continue
            table = functions if kind == "fn" else types
            entry = (spelling, attributes)
            if table.setdefault(symbol, entry) != entry:
                raise ValueError(f"conflicting definitions of {symbol}")
    return functions, types, tuples, enumerators


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    families = {family: load(names) for family, names in FAMILIES.items()}
    prototypes = {}
    for functions, _, _, _ in families.values():
        for spelling, _ in functions.values():
            prototypes.setdefault(spelling, f"GCC_ARM_PROTOTYPE_{len(prototypes):04}")
    lines = []
    for spelling, name in sorted(prototypes.items(), key=lambda item: item[1]):
        try:
            lines.extend(clang.prototype_lines(name, spelling))
        except clang.PrototypeError as error:
            raise ValueError(f"prototype `{spelling}`: {error}") from error
        lines.append("")
    for family, (functions, types, tuples, enumerators) in families.items():
        lines.append(f"pub(super) static GCC_{family}_BUILTINS: &[ClangBuiltin] = &[")
        for name, (spelling, attributes) in sorted(functions.items()):
            attrs = ", ".join(f"BuiltinAttribute::{a}" for a in sorted({"NoThrow", *ATTRIBUTES[attributes]}))
            lines.extend(
                [
                    "    ClangBuiltin {",
                    f"        name: {json.dumps(name)},",
                    f"        record: {json.dumps(name)},",
                    f"        prototype: {clang.prototype_field(prototypes, spelling)},",
                    "        kind: ClangBuiltinKind::Builtin,",
                    f"        attributes: &[{attrs}],",
                    "        languages: None,",
                    "        header: None,",
                    "        features: None,",
                    "    },",
                ]
            )
        lines.extend(["];", ""])
        lines.append(f"pub(super) static GCC_{family}_TYPES: &[(&str, BuiltinParam)] = &[")
        entries = {name: spelling for name, (spelling, _) in types.items()}
        if family == "AARCH64":
            for name, canonical in clang.SIZELESS.items():
                entries[name] = name
                entries[canonical] = name
        for name, spelling in sorted(entries.items()):
            ret, _, _ = clang.parse_prototype(f"{spelling}()")
            lines.append(f"    ({json.dumps(name)}, {ret}),")
        lines.extend(["];", ""])
        if family == "AARCH64":
            lines.append("pub(super) static GCC_AARCH64_TUPLES: &[(&str, &str, u32)] = &[")
            for name, (element, count) in sorted(tuples.items()):
                lines.append(f"    ({json.dumps(name)}, {json.dumps(element)}, {count}),")
            lines.extend(["];", ""])
            lines.append("pub(super) static GCC_AARCH64_ENUMERATORS: &[(&str, &str, i64)] = &[")
            for name, value in sorted(enumerators.items(), key=lambda item: (item[0].startswith("SV_P") and not item[0].startswith("SV_POW"), item[1])):
                tag = "svprfop" if name.startswith(("SV_PLD", "SV_PST")) else "svpattern"
                lines.append(f"    ({json.dumps(name)}, {json.dumps(tag)}, {value}),")
            lines.extend(["];", ""])
    args.output.write_text("\n".join(lines))


if __name__ == "__main__":
    main()
