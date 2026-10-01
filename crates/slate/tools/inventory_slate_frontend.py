#!/usr/bin/env python3

import argparse
import collections
import csv
import pathlib
import re
import subprocess


ROOT = pathlib.Path(__file__).resolve().parents[1]
PHASE4 = re.compile(
    r"(?:^|_)(?:atomic|atomics|volatile|bitfield|bitint|vla|goto|asm|varargs|"
    r"va_list|complex|float16|float128|f128|long_double|builtin|builtins|"
    r"intrinsics|thread_local|weakref|alias|pragma|pragmas|attribute|attributes|"
    r"target|c23|c11|c99|c89|gnu|stdlib|libc|stdio|printf|fprintf|sprintf|"
    r"snprintf|fputs|perror|ctype|setjmp|nonnull|noreturn|constructor|"
    r"destructor|transparent_union|compound_literal|statement_expr|generic_type|"
    r"packed|aligned|anonymous|anon|flexible_array|process_msvc|secure_crt|"
    r"function_provenance|extern_char_ptr_ffi|libyaml|timespec_get|qsort|"
    r"mkstemp|memcpy|memmove|memset|memcmp|memchr|cstr|alloca)(?:_|$)"
)
PHASE5 = re.compile(
    r"(?:^|_)(?:fixup|lift|lattice|mem2reg|worklist|heap|interprocedural|"
    r"option_box|salsa|borrowck|ptr_len|slice|buffer)(?:_|$)"
)
PHASE4_EXACT = {
    "assert_compile_time_true",
    "assert_recovery_preserves_result",
    "assert_runtime_false",
    "assert_runtime_true",
    "atoi_atof_const_fold",
    "atoi_atol_prelude_dynamic",
    "constant_object_query_ops",
    "dynamic_object_size_ops",
    "elifdef",
    "enum_wrapper_unwind_abi_propagation",
    "function_typedef_pointer_return_struct",
    "indirect_call_enum_return_capture",
    "indirect_call_unwind_abi_propagation",
    "inline_variadic_const_args",
    "mem_bcopy_overlap",
    "mem_bitcast_copy",
    "mem_bzero",
    "mt-atomics",
    "nested_aggregate_const_record_field_alignment",
    "return_cleanup",
    "saturating_arith",
    "switch_case_u128_values",
    "switch_torture",
    "type_qualifiers",
    "u8_string_literal",
    "vector_extension_ops",
    "void_ptr_function_pointer_cast",
}
PHASE5_EXACT = {
    "dense_temp_chain_completes_in_bounded_time",
    "filecheck_annotation_regions",
    "peel_casts",
    "peel_casts_ptr_chain",
    "redundant_parens",
    "serial_temp_chain_completes_in_bounded_time",
    "support_module_cleanup",
    "unnecessary_casts",
}
PARSER_FAILURE_TICKETS = {
    "c11": "slate-p58o.1.7",
    "c23_language": "slate-parser-dyd.38",
    "c23_library": "slate-p58o.1.6",
    "c23_nullptr": "slate-parser-dyd.51",
    "c23_stdlib_memory_management": "slate-p58o.1.6",
    "gnu_auto_type": "slate-parser-dyd.38",
    "gnu_function_pointer_arithmetic": "slate-parser-dyd.52",
    "gnu_init_designator_survey": "slate-parser-dyd.53",
    "gnu_misc_extension_survey": "slate-parser-dyd.26",
    "long_double_complex": "slate-p58o.1.7",
    "misc_language_survey": "slate-parser-dyd.54",
    "statement_expr_auto_type": "slate-parser-dyd.38",
    "transparent_union_call": "slate-parser-dyd.34",
}


def scope_owner(stem):
    if stem in PHASE4_EXACT or PHASE4.search(stem) or any(
        word in stem
        for word in (
            "fallthrough",
            "case_range",
            "misc_language_survey",
            "union_after_include",
            "stat_struct",
            "stat_mtime",
        )
    ):
        return "slate-p58o.4"
    if stem in PHASE5_EXACT or PHASE5.search(stem):
        return "slate-p58o.5"
    return "slate-p58o.3.24"


def run(binary, command, fixture):
    try:
        result = subprocess.run(
            [str(binary), command, str(fixture)],
            text=True,
            capture_output=True,
            timeout=15,
            check=False,
        )
        return result.returncode, result.stdout, result.stderr
    except subprocess.TimeoutExpired:
        return 124, "", f"{command} timed out after 15 seconds"


def module_barrier(binary, fixture):
    code, output, error = run(binary, "emit-slate-ir", fixture)
    if code:
        return "module.types or module.asm (IR emission failed)", "slate-p58o.4", error
    first = next(
        (line.strip() for line in output.splitlines() if line.lstrip().startswith("type @type")),
        None,
    )
    if first is None:
        return "module.asm", "slate-p58o.4", ""
    match = re.search(r"type @type\d+(?:\s+([^=]+?))?\s*=\s*(.*)", first)
    if match is None:
        return "module.types", "slate-p58o.3.35", ""
    raw_name, raw_body = match.groups()
    name, body = (raw_name or "<anonymous>").strip(), raw_body.strip()
    if body.startswith("struct"):
        kind, ticket = "record", "slate-p58o.3.15"
    elif body.startswith("union"):
        kind, ticket = "union", "slate-p58o.3.17"
    elif body.startswith("enum"):
        kind, ticket = "enum", "slate-p58o.3.20"
    else:
        kind, ticket = "alias", "slate-p58o.3.35"
    return f"module.types {kind} {name}", ticket, ""


def lowerer_barrier(binary, fixture, diagnostic, scope):
    detail = diagnostic.removeprefix("error: unsupported slate-parser IR: ")
    if detail == "types or module assembly":
        return module_barrier(binary, fixture)
    if detail.startswith("global "):
        if fixture.stem in {"global_string_pointer", "global_array_pointer_decay"}:
            ticket = "slate-p58o.3.34"
        elif fixture.stem == "global_string_pointer_array":
            ticket = "slate-p58o.3.19"
        else:
            ticket = "slate-p58o.3.18"
        return "Global " + detail[7:], ticket, ""
    if detail.startswith("integer type "):
        return "Type " + detail[13:], "slate-p58o.4", ""
    if detail.startswith("type "):
        ty = detail[5:]
        if ty.startswith("fn("):
            ticket = "slate-p58o.3.11"
        elif ty.startswith("f32") or ty.startswith("f64"):
            ticket = "slate-p58o.3.6"
        else:
            ticket = "slate-p58o.4"
        return "Type " + ty, ticket, ""
    if detail.startswith("constant "):
        ticket = "slate-p58o.3.6" if scope != "slate-p58o.4" else scope
        return "Number::FloatBits", ticket, ""
    if detail.startswith("statement "):
        op = detail[10:].split("(")[0].split(" {")[0]
        ticket = (
            "slate-p58o.3.22" if op == "Switch" and scope != "slate-p58o.4" else "slate-p58o.4"
        )
        return "Statement::" + op, ticket, ""
    if detail.startswith("value "):
        value = detail[6:]
        op = re.match(r"[a-z_]+", value)
        name = op.group(0) if op else "unknown"
        tickets = {
            "neg": "slate-p58o.3.2",
            "not": "slate-p58o.3.4",
            "conditional": "slate-p58o.3.29",
            "update": "slate-p58o.3.30",
            "null": "slate-p58o.3.36",
            "code_units": "slate-p58o.3.37",
            "function_decay": "slate-p58o.3.11",
            "ptr_diff": "slate-p58o.3.12",
        }
        ticket = tickets.get(name, "slate-p58o.4")
        if name == "aggregate":
            if "<array<" in value:
                ticket = "slate-p58o.3.32"
            elif "<complex<" in value:
                ticket = "slate-p58o.4"
            else:
                ticket = "slate-p58o.3.16"
        if name == "read" and "atomic=" in value:
            ticket = "slate-p58o.4"
        if name == "update" and "atomic=" in value:
            ticket = "slate-p58o.4"
        if name == "call" and "__builtin_" in value:
            ticket = "slate-p58o.4"
        suffix = "[array]" if name == "aggregate" and "<array<" in value else ""
        return "ValueKind::" + name + suffix, ticket, ""
    return detail[:120], scope, ""


def inventory(binary):
    for fixture in sorted((ROOT / "tests/fixtures").glob("*.c")):
        source = fixture.read_text(errors="replace")
        scope = scope_owner(fixture.stem)
        code, _, error = run(binary, "translate-lowered", fixture)
        diagnostic = error.strip().splitlines()[0] if error.strip() else ""
        if code == 0:
            stage, barrier, ticket = "lowered-unverified", "-", scope
        elif "unsupported slate-parser IR:" in error:
            stage = "unsupported-lowering"
            barrier, ticket, extra = lowerer_barrier(binary, fixture, diagnostic, scope)
            diagnostic = extra.strip().splitlines()[0] if extra.strip() else diagnostic
            if ticket == "slate-p58o.4" and (
                barrier.startswith("ValueKind::overflow_")
                or "atomic=" in diagnostic
            ):
                scope = "slate-p58o.4"
        else:
            stage = "parser-sema"
            barrier = diagnostic.removeprefix("error: ")
            ticket = PARSER_FAILURE_TICKETS.get(fixture.stem, scope)
        diagnostic = diagnostic.replace(str(ROOT) + "/", "")[:200] or "-"
        yield (
            fixture.name,
            stage,
            barrier[:120],
            ticket,
            scope,
            "yes" if "SLATE-LOWERER" in source and "SLATE-REWRITES" in source else "no",
            diagnostic,
        )


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--binary", type=pathlib.Path, default=ROOT.parents[1] / "target/test-cache/release/slate")
    parser.add_argument("--output", type=pathlib.Path, default=ROOT / "wiki/concepts/slate-parser-fixture-inventory.tsv")
    args = parser.parse_args()
    rows = list(inventory(args.binary.resolve()))
    with args.output.open("w", newline="") as output:
        writer = csv.writer(output, delimiter="\t", lineterminator="\n")
        writer.writerow(
            ("fixture", "stage", "first_barrier", "first_ticket", "scope_owner", "slate_checks", "diagnostic")
        )
        writer.writerows(rows)
    print(f"wrote {len(rows)} fixtures to {args.output}")
    for stage, count in sorted(collections.Counter(row[1] for row in rows).items()):
        print(f"{stage}: {count}")


if __name__ == "__main__":
    main()
