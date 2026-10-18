#!/usr/bin/env python3

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import generate_clang_builtins as clang  # noqa: E402

clang.FLOATS.update(
    {
        "_Float32": "Float32",
        "_Float64": "Float64",
        "_Float32x": "Float32x",
        "_Float64x": "Float64x",
        "_Float128": "Float128",
        "_Decimal64x": "Decimal64x",
    }
)

PRIMITIVES = {
    "BT_VOID": "void",
    "BT_BOOL": "bool",
    "BT_INT": "int",
    "BT_UINT": "unsigned int",
    "BT_COMPLEX_INT": "_Complex int",
    "BT_LONG": "long int",
    "BT_ULONG": "unsigned long int",
    "BT_LONGLONG": "long long int",
    "BT_ULONGLONG": "unsigned long long int",
    "BT_INTMAX": "int64_t",
    "BT_UINTMAX": "uint64_t",
    "BT_INT8": "signed char",
    "BT_INT16": "short",
    "BT_UINT8": "unsigned char",
    "BT_UINT16": "uint16_t",
    "BT_UINT32": "uint32_t",
    "BT_UINT64": "uint64_t",
    "BT_UINT128": "__uint128_t",
    "BT_WORD": "size_t",
    "BT_UNWINDWORD": "size_t",
    "BT_FLOAT": "float",
    "BT_DOUBLE": "double",
    "BT_LONGDOUBLE": "long double",
    "BT_BFLOAT16": "__bf16",
    "BT_FLOAT16": "_Float16",
    "BT_FLOAT32": "_Float32",
    "BT_FLOAT64": "_Float64",
    "BT_FLOAT128": "__float128",
    "BT_FLOAT32X": "_Float32x",
    "BT_FLOAT64X": "_Float64x",
    "BT_COMPLEX_FLOAT": "_Complex float",
    "BT_COMPLEX_DOUBLE": "_Complex double",
    "BT_COMPLEX_LONGDOUBLE": "_Complex long double",
    "BT_COMPLEX_FLOAT16": "_Complex _Float16",
    "BT_COMPLEX_FLOAT32": "_Complex _Float32",
    "BT_COMPLEX_FLOAT64": "_Complex _Float64",
    "BT_COMPLEX_FLOAT128": "_Complex __float128",
    "BT_COMPLEX_FLOAT32X": "_Complex _Float32x",
    "BT_COMPLEX_FLOAT64X": "_Complex _Float64x",
    "BT_PTR": "void*",
    "BT_FILEPTR": "void*",
    "BT_CONST_TM_PTR": "void const*",
    "BT_FENV_T_PTR": "void*",
    "BT_CONST_FENV_T_PTR": "void const*",
    "BT_FEXCEPT_T_PTR": "void*",
    "BT_CONST_FEXCEPT_T_PTR": "void const*",
    "BT_CONST_PTR": "void const*",
    "BT_VOLATILE_PTR": "void volatile*",
    "BT_CONST_VOLATILE_PTR": "void const volatile*",
    "BT_PTRMODE": "ptrdiff_t",
    "BT_INT_PTR": "int*",
    "BT_FLOAT_PTR": "float*",
    "BT_DOUBLE_PTR": "double*",
    "BT_CONST_DOUBLE_PTR": "double const*",
    "BT_LONGDOUBLE_PTR": "long double*",
    "BT_FLOAT16_PTR": "_Float16*",
    "BT_FLOAT32_PTR": "_Float32*",
    "BT_FLOAT64_PTR": "_Float64*",
    "BT_FLOAT128_PTR": "__float128*",
    "BT_FLOAT32X_PTR": "_Float32x*",
    "BT_FLOAT64X_PTR": "_Float64x*",
    "BT_PID": "int",
    "BT_SIZE": "size_t",
    "BT_CONST_SIZE": "size_t const",
    "BT_SSIZE": "ptrdiff_t",
    "BT_WINT": "unsigned int",
    "BT_STRING": "char*",
    "BT_CONST_STRING": "char const*",
    "BT_DFLOAT32": "_Decimal32",
    "BT_DFLOAT64": "_Decimal64",
    "BT_DFLOAT128": "_Decimal128",
    "BT_DFLOAT64X": "_Decimal64x",
    "BT_VALIST_REF": "__builtin_va_list_ref",
    "BT_VALIST_ARG": "__builtin_va_list",
    "BT_I1": "unsigned char",
    "BT_I2": "unsigned short",
    "BT_I4": "unsigned int",
    "BT_I8": "uint64_t",
    "BT_I16": "__uint128_t",
    "BT_PTR_CONST_STRING": "char* const*",
}

UNAVAILABLE_TYPES = {"BT_FLOAT128X", "BT_COMPLEX_FLOAT128X", "BT_FLOAT128X_PTR"}

UNSPELLABLE_TYPES = {"BT_PTR_FN_VOID_PTR", "BT_PTR_FN_VOID_PTR_PTR", "BT_PTR_FN_VOID_VAR"}

STUB_PROTOTYPES = {
    "BUILT_IN_STACK_SAVE": "void*()",
    "BUILT_IN_STACK_RESTORE": "void(void*)",
    "BUILT_IN_ALLOCA_WITH_ALIGN": "void*(size_t, size_t)",
    "BUILT_IN_ALLOCA_WITH_ALIGN_AND_MAX": "void*(size_t, size_t, size_t)",
}

FLAGS = {
    "flag_errno_math": 1,
    "flag_rounding_math": 0,
    "flag_non_call_exceptions": 0,
    "flag_openacc": 0,
    "flag_openmp": 0,
    "flag_tree_parallelize_loops": 1,
    "flag_tm": 0,
    "flag_sanitize": 0,
    "flag_sanitize_coverage": 0,
    "flag_coroutines": 0,
    "targetm.have_tls": 1,
    "SANITIZE_ADDRESS": 1,
    "SANITIZE_THREAD": 2,
    "SANITIZE_HWADDRESS": 4,
    "SANITIZE_MEMTAG_STACK": 8,
    "SANITIZE_UNDEFINED": 16,
    "SANITIZE_UNDEFINED_NONDEFAULT": 32,
    "true": 1,
    "false": 0,
}

STANDARD_GATES = {
    "flag_isoc94": "C94",
    "flag_isoc99": "C99",
    "flag_isoc11": "C11",
    "flag_isoc23": "C23",
    "flag_isoc2y": "C2y",
}

ATTRIBUTES = {
    "ATTR_CONST": "Const",
    "ATTR_PURE": "Pure",
    "ATTR_NORETURN": "NoReturn",
    "ATTR_NOTHROW": "NoThrow",
    "ATTR_NONNULL": "NonNull",
    "ATTR_TYPEGENERIC": "CustomTypeChecking",
    "ATTR_RETURNS_TWICE": "ReturnsTwice",
    "ATTR_PRINTF": "PrintfFormat",
    "ATTR_SCANF": "ScanfFormat",
}

EXPRESSION_TOKEN = re.compile(r"\|\||&&|[A-Za-z_][A-Za-z_0-9.]*|\d+|[?:()!|&>]")


class Expression:
    def __init__(self, text):
        self.tokens = EXPRESSION_TOKEN.findall(re.sub(r"\(\s*int\s*\)", "", text))
        self.index = 0

    def peek(self):
        return self.tokens[self.index] if self.index < len(self.tokens) else None

    def take(self, expected=None):
        token = self.peek()
        if token is None or (expected is not None and token != expected):
            raise ValueError(f"expected {expected!r} in {' '.join(self.tokens)!r}")
        self.index += 1
        return token

    def evaluate(self):
        value = self.conditional()
        if self.peek() is not None:
            raise ValueError(f"trailing {self.peek()!r} in {' '.join(self.tokens)!r}")
        return value

    def conditional(self):
        condition = self.binary()
        if self.peek() != "?":
            return condition
        self.take("?")
        yes = self.conditional()
        self.take(":")
        no = self.conditional()
        return yes if condition else no

    def binary(self):
        value = self.unary()
        while self.peek() in ("||", "&&", "|", "&", ">"):
            operator = self.take()
            right = self.unary()
            value = {
                "||": lambda: int(bool(value) or bool(right)),
                "&&": lambda: int(bool(value) and bool(right)),
                "|": lambda: value | right,
                "&": lambda: value & right,
                ">": lambda: int(value > right),
            }[operator]()
        return value

    def unary(self):
        token = self.take()
        if token == "!":
            return int(not self.unary())
        if token == "(":
            value = self.conditional()
            self.take(")")
            return value
        if token.isdigit():
            return int(token)
        if token.startswith("ATTR_"):
            return token
        if token in ("targetm.floatn_builtin_p", "targetm.libc_has_function"):
            self.take("(")
            depth = 1
            while depth:
                depth += {"(": 1, ")": -1}.get(self.take(), 0)
            return 1
        if token in FLAGS:
            return FLAGS[token]
        raise ValueError(f"unknown symbol {token!r}")


def evaluate(text):
    return Expression(text).evaluate()


def preprocess(gcc, prelude, source):
    command = [*gcc, "-E", "-P", "-x", "c", "-I", str(source.parent), "-"]
    text = prelude + f'#include "{source.name}"\n'
    return subprocess.run(command, input=text, check=True, capture_output=True, text=True).stdout


def records(text, marker, fields):
    for body in re.findall(rf"@{marker}@(.*?)@END@", text, re.S):
        parts = [" ".join(part.split()) for part in body.split("@|")]
        if len(parts) != fields:
            raise ValueError(f"malformed {marker} record: {body!r}")
        yield parts


TYPE_PRELUDE = """
#define DEF_PRIMITIVE_TYPE(ENUM, VALUE) @P@ ENUM @END@
#define DEF_POINTER_TYPE(ENUM, TYPE) @R@ ENUM @| TYPE @END@
#define DEF_FUNCTION_TYPE_0(ENUM, RETURN) @F@ ENUM @| 0 @| RETURN @END@
#define DEF_FUNCTION_TYPE_VAR_0(ENUM, RETURN) @F@ ENUM @| 1 @| RETURN @END@
"""
for arity in range(1, 12):
    parameters = ", ".join(f"A{index}" for index in range(1, arity + 1))
    joined = " ".join(f"A{index}" for index in range(1, arity + 1))
    TYPE_PRELUDE += f"#define DEF_FUNCTION_TYPE_{arity}(ENUM, RETURN, {parameters}) @F@ ENUM @| 0 @| RETURN {joined} @END@\n"
    TYPE_PRELUDE += f"#define DEF_FUNCTION_TYPE_VAR_{arity}(ENUM, RETURN, {parameters}) @F@ ENUM @| 1 @| RETURN {joined} @END@\n"


def type_spellings(gcc, source):
    text = preprocess(gcc, TYPE_PRELUDE, source)
    spellings = {}
    for (name,) in records(text, "P", 1):
        if name in UNAVAILABLE_TYPES:
            continue
        if name not in PRIMITIVES:
            raise ValueError(f"unmapped primitive type {name}")
        spellings[name] = PRIMITIVES[name]
    for name, pointee in records(text, "R", 2):
        if name not in UNSPELLABLE_TYPES:
            spellings[name] = spellings[pointee] + "*"
    functions = {}
    for name, variadic, types in records(text, "F", 3):
        ret, *params = types.split()
        functions[name] = (ret, params, variadic == "1")
    return spellings, functions


def function_spelling(name, spellings, functions):
    ret, params, variadic = functions[name]
    if any(ty in UNAVAILABLE_TYPES for ty in [ret, *params]):
        return None
    for ty in [ret, *params]:
        if ty in UNSPELLABLE_TYPES:
            return "custom"
        if ty not in spellings:
            raise ValueError(f"function type {name} uses unmapped {ty}")
    arguments = [spellings[ty] for ty in params] + (["..."] if variadic else [])
    return f"{spellings[ret]}({', '.join(arguments)})"


ATTRIBUTE_PRELUDE = """
#define DEF_ATTR_NULL_TREE(ENUM) @A@ ENUM @| ATTR_NULL @| ATTR_NULL @| ATTR_NULL @END@
#define DEF_ATTR_INT(ENUM, VALUE) @A@ ENUM @| ATTR_NULL @| ATTR_NULL @| ATTR_NULL @END@
#define DEF_ATTR_STRING(ENUM, VALUE) @A@ ENUM @| ATTR_NULL @| ATTR_NULL @| ATTR_NULL @END@
#define DEF_ATTR_IDENT(ENUM, STRING) @A@ ENUM @| ATTR_NULL @| ATTR_NULL @| ATTR_NULL @END@
#define DEF_ATTR_TREE_LIST(ENUM, PURPOSE, VALUE, CHAIN) @A@ ENUM @| PURPOSE @| VALUE @| CHAIN @END@
"""


def attribute_lists(gcc, source):
    text = preprocess(gcc, ATTRIBUTE_PRELUDE, source)
    lists = {name: (purpose, value, chain) for name, purpose, value, chain in records(text, "A", 4)}
    resolved = {}

    def idents(name):
        if name not in resolved:
            if name not in lists:
                raise ValueError(f"unknown attribute list {name}")
            parts = lists[name]
            found = {name} if parts == ("ATTR_NULL", "ATTR_NULL", "ATTR_NULL") else set()
            for part in parts:
                if part != "ATTR_NULL":
                    found |= idents(part)
            resolved[name] = found
        return resolved[name]

    return idents


BUILTIN_PRELUDE = """
#define DEF_BUILTIN(ENUM, NAME, CLASS, TYPE, LIBTYPE, BOTH_P, FALLBACK_P, NONANSI_P, ATTRS, IMPLICIT, COND) \
  @B@ ENUM @| NAME @| TYPE @| BOTH_P @| NONANSI_P @| ATTRS @| COND @END@
"""


def standard_gate(text):
    match = re.fullmatch(r"!\s*(flag_isoc\w+)", text)
    if match:
        return STANDARD_GATES[match.group(1)]
    return "Gnu" if evaluate(text) else None


def builtin_records(gcc, source, spellings, functions, attribute_idents):
    text = preprocess(gcc, BUILTIN_PRELUDE, source)
    entries = []
    for enum, name, ty, both, nonansi, attrs, cond in records(text, "B", 7):
        if name.startswith("("):
            continue
        name = "".join(json.loads(part) for part in re.findall(r'"[^"]*"', name))
        if enum in STUB_PROTOTYPES:
            entries.append((name, enum, STUB_PROTOTYPES[enum], "Builtin", ["NoThrow"], None))
            continue
        if ty == "BT_LAST" or not evaluate(cond):
            continue
        prototype = function_spelling(ty, spellings, functions)
        if prototype is None:
            continue
        idents = attribute_idents(evaluate(attrs))
        names = sorted({ATTRIBUTES[ident] for ident in idents if ident in ATTRIBUTES})
        if prototype == "custom":
            prototype = None
            names = sorted({*names, "CustomTypeChecking"})
        if evaluate(both):
            entries.append((name.removeprefix("__builtin_"), enum, prototype, "Library", names, standard_gate(nonansi)))
        else:
            entries.append((name, enum, prototype, "Builtin", names, None))
    entries.sort(key=lambda entry: entry[0])
    for previous, current in zip(entries, entries[1:]):
        if previous[0] == current[0]:
            raise ValueError(f"duplicate builtin {current[0]}")
    return entries


def table_lines(entries, prototypes):
    lines = ["pub(super) static GCC_BUILTINS: &[ClangBuiltin] = &["]
    for name, enum, prototype, kind, attributes, gate in entries:
        attrs = ", ".join(f"BuiltinAttribute::{attribute}" for attribute in attributes)
        language = "None" if gate is None else f"Some(BuiltinLanguage::Gcc{gate})"
        lines.extend(
            [
                "    ClangBuiltin {",
                f"        name: {json.dumps(name)},",
                f"        record: {json.dumps(enum)},",
                f"        prototype: {clang.prototype_field(prototypes, prototype)},",
                f"        kind: ClangBuiltinKind::{kind},",
                f"        attributes: &[{attrs}],",
                f"        languages: {language},",
                "        header: None,",
                "        features: None,",
                "    },",
            ]
        )
    lines.append("];")
    return lines


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--gcc-source", type=Path, required=True, help="the gcc/ directory of a GCC checkout")
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--cc", default="gcc")
    args = parser.parse_args()
    gcc = [args.cc]
    spellings, functions = type_spellings(gcc, args.gcc_source / "builtin-types.def")
    attribute_idents = attribute_lists(gcc, args.gcc_source / "builtin-attrs.def")
    entries = builtin_records(gcc, args.gcc_source / "builtins.def", spellings, functions, attribute_idents)
    prototypes = {}
    for spelling in sorted({entry[2] for entry in entries if entry[2]}):
        prototypes[spelling] = f"GCC_PROTOTYPE_{len(prototypes):04}"
    lines = []
    for spelling, name in prototypes.items():
        try:
            lines.extend(clang.prototype_lines(name, spelling))
        except clang.PrototypeError as error:
            raise ValueError(f"prototype `{spelling}`: {error}") from error
        lines.append("")
    lines.extend(table_lines(entries, prototypes))
    args.output.write_text("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
