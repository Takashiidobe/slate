use super::*;
use crate::cir::ir::Attr;
use crate::cir::ir::Block;
use std::collections::BTreeMap;

fn op(name: &str, results: &[&str], operands: &[&str], ty: &str) -> Op {
    Op {
        results: results.iter().map(|s| s.to_string()).collect(),
        name: name.to_string(),
        operands: operands.iter().map(|s| s.to_string()).collect(),
        successors: Vec::new(),
        attrs: BTreeMap::new(),
        regions: Vec::new(),
        ty: Some(ty.to_string()),
        loc: None,
    }
}

fn const_op(result: &str, value: i64, ty: &str) -> Op {
    let mut o = op("cir.const", &[result], &[], &format!("() -> {ty}"));
    o.attrs.insert(
        "value".to_string(),
        Attr::Raw(format!("#cir.int<{value}> : {ty}")),
    );
    o
}

fn call_malloc(result: &str, size_operand: &str) -> Op {
    let mut o = op(
        "cir.call",
        &[result],
        &[size_operand],
        "(!u64i) -> !cir.ptr<!void>",
    );
    o.attrs
        .insert("callee".to_string(), Attr::Raw("@malloc".to_string()));
    o
}

fn call_free(ptr_operand: &str) -> Op {
    let mut o = op("cir.call", &[], &[ptr_operand], "(!cir.ptr<!s32i>) -> ()");
    o.attrs
        .insert("callee".to_string(), Attr::Raw("@free".to_string()));
    o
}

fn call_printf(result: &str, fmt_operand: &str, arg_operands: &[&str]) -> Op {
    let mut operands = vec![fmt_operand];
    operands.extend_from_slice(arg_operands);
    let mut o = op(
        "cir.call",
        &[result],
        &operands,
        "(!cir.ptr<!s8i>, ...) -> !s32i",
    );
    o.attrs
        .insert("callee".to_string(), Attr::Raw("@printf".to_string()));
    o
}

#[test]
fn printf_call_pushes_a_call_effect_with_only_the_substituted_args() {
    let ops = vec![
        op("cir.get_global", &["fmt"], &[], "() -> !cir.ptr<!s8i>"),
        const_op("v", 5, "!s32i"),
        call_printf("r", "fmt", &["v"]),
        op("cir.return", &[], &[], "() -> ()"),
    ];
    let trace = interpret(&ops);
    assert_eq!(
        trace.effects,
        vec![
            Effect::Call {
                name: "printf".to_string(),
                args: vec![int32(5)],
            },
            Effect::Exit(0),
        ]
    );
}

/// Mirrors (minus printf) the CIR clang actually emits for:
/// `int *p = malloc(2 * sizeof(int)); p[0] = 1; p[1] = 2;
///  free(p); return p[0] + p[1];`
fn malloc_array_fixture() -> Vec<Op> {
    vec![
        op("cir.alloca", &["p"], &[], "() -> !cir.ptr<!cir.ptr<!s32i>>"),
        const_op("c4", 4, "!u64i"),
        const_op("c2", 2, "!u64i"),
        op(
            "cir.mul",
            &["size"],
            &["c4", "c2"],
            "(!u64i, !u64i) -> !u64i",
        ),
        call_malloc("raw", "size"),
        op(
            "cir.cast",
            &["buf"],
            &["raw"],
            "(!cir.ptr<!void>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.store",
            &[],
            &["buf", "p"],
            "(!cir.ptr<!s32i>, !cir.ptr<!cir.ptr<!s32i>>) -> ()",
        ),
        const_op("v0", 1, "!s32i"),
        const_op("i0", 0, "!s64i"),
        op(
            "cir.load",
            &["p0"],
            &["p"],
            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.ptr_stride",
            &["loc0"],
            &["p0", "i0"],
            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.store",
            &[],
            &["v0", "loc0"],
            "(!s32i, !cir.ptr<!s32i>) -> ()",
        ),
        const_op("v1", 2, "!s32i"),
        const_op("i1", 1, "!s64i"),
        op(
            "cir.load",
            &["p1"],
            &["p"],
            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.ptr_stride",
            &["loc1"],
            &["p1", "i1"],
            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.store",
            &[],
            &["v1", "loc1"],
            "(!s32i, !cir.ptr<!s32i>) -> ()",
        ),
        const_op("i0b", 0, "!s64i"),
        op(
            "cir.load",
            &["p0b"],
            &["p"],
            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.ptr_stride",
            &["loc0b"],
            &["p0b", "i0b"],
            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.load",
            &["r0"],
            &["loc0b"],
            "(!cir.ptr<!s32i>) -> !s32i",
        ),
        const_op("i1b", 1, "!s64i"),
        op(
            "cir.load",
            &["p1b"],
            &["p"],
            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.ptr_stride",
            &["loc1b"],
            &["p1b", "i1b"],
            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.load",
            &["r1"],
            &["loc1b"],
            "(!cir.ptr<!s32i>) -> !s32i",
        ),
        op(
            "cir.add",
            &["sum"],
            &["r0", "r1"],
            "(!s32i, !s32i) -> !s32i",
        ),
        call_free("buf"),
        op("cir.return", &[], &["sum"], "(!s32i) -> ()"),
    ]
}

fn read_after_free_fixture() -> Vec<Op> {
    vec![
        op("cir.alloca", &["p"], &[], "() -> !cir.ptr<!cir.ptr<!s32i>>"),
        const_op("c4", 4, "!u64i"),
        call_malloc("raw", "c4"),
        op(
            "cir.cast",
            &["buf"],
            &["raw"],
            "(!cir.ptr<!void>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.store",
            &[],
            &["buf", "p"],
            "(!cir.ptr<!s32i>, !cir.ptr<!cir.ptr<!s32i>>) -> ()",
        ),
        call_free("buf"),
        const_op("i0", 0, "!s64i"),
        op(
            "cir.load",
            &["p0"],
            &["p"],
            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.ptr_stride",
            &["loc0"],
            &["p0", "i0"],
            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
        ),
        op("cir.load", &["r0"], &["loc0"], "(!cir.ptr<!s32i>) -> !s32i"),
        op("cir.return", &[], &["r0"], "(!s32i) -> ()"),
    ]
}

#[test]
#[should_panic(expected = "read from")]
fn reading_after_free_panics_instead_of_silently_succeeding() {
    interpret(&read_after_free_fixture());
}

fn double_free_fixture() -> Vec<Op> {
    vec![
        op("cir.alloca", &["p"], &[], "() -> !cir.ptr<!cir.ptr<!s32i>>"),
        const_op("c4", 4, "!u64i"),
        call_malloc("raw", "c4"),
        op(
            "cir.cast",
            &["buf"],
            &["raw"],
            "(!cir.ptr<!void>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.store",
            &[],
            &["buf", "p"],
            "(!cir.ptr<!s32i>, !cir.ptr<!cir.ptr<!s32i>>) -> ()",
        ),
        call_free("buf"),
        call_free("buf"),
        const_op("z", 0, "!s32i"),
        op("cir.return", &[], &["z"], "(!s32i) -> ()"),
    ]
}

#[test]
#[should_panic(expected = "double free")]
fn freeing_twice_panics() {
    interpret(&double_free_fixture());
}

fn int32(value: i128) -> Value {
    Value::Int {
        width: IntWidth::W32,
        signed: true,
        value,
    }
}

#[test]
fn malloc_write_read_produces_expected_effects() {
    let trace = interpret(&malloc_array_fixture());
    let alloc = AllocId(0);
    assert_eq!(
        trace.effects,
        vec![
            Effect::Alloc { alloc, size: 8 },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 0
                },
                value: int32(1),
            },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 4
                },
                value: int32(2),
            },
            Effect::Read {
                loc: Location {
                    alloc,
                    byte_offset: 0
                },
                value: int32(1),
            },
            Effect::Read {
                loc: Location {
                    alloc,
                    byte_offset: 4
                },
                value: int32(2),
            },
            Effect::Dealloc { alloc },
            Effect::Exit(3),
        ]
    );
}

#[test]
fn local_pointer_variable_never_appears_as_an_effect() {
    let trace = interpret(&malloc_array_fixture());
    for effect in &trace.effects {
        if let Effect::Write { value, .. } | Effect::Read { value, .. } = effect {
            assert!(!matches!(value, Value::Ref(_)));
        }
    }
}

fn region(ops: Vec<Op>) -> Region {
    Region {
        blocks: vec![Block {
            label: None,
            args: Vec::new(),
            ops,
        }],
    }
}

/// Mirrors the CIR shape for:
/// `int *p = malloc(sizeof(int));
///  if (5 > 3) { p[0] = 1; } else { p[0] = 2; }
///  return p[0];`
fn if_else_fixture() -> Vec<Op> {
    vec![
        op("cir.alloca", &["p"], &[], "() -> !cir.ptr<!cir.ptr<!s32i>>"),
        const_op("c4", 4, "!u64i"),
        call_malloc("raw", "c4"),
        op(
            "cir.cast",
            &["buf"],
            &["raw"],
            "(!cir.ptr<!void>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.store",
            &[],
            &["buf", "p"],
            "(!cir.ptr<!s32i>, !cir.ptr<!cir.ptr<!s32i>>) -> ()",
        ),
        const_op("c5", 5, "!s32i"),
        const_op("c3", 3, "!s32i"),
        {
            let mut cmp = op(
                "cir.cmp",
                &["cond"],
                &["c5", "c3"],
                "(!s32i, !s32i) -> !cir.bool",
            );
            cmp.attrs.insert("kind".to_string(), Attr::Int(2));
            cmp
        },
        {
            let mut if_op = op("cir.if", &[], &["cond"], "(!cir.bool) -> ()");
            if_op.regions = vec![
                region(vec![
                    const_op("v1", 1, "!s32i"),
                    const_op("i0", 0, "!s64i"),
                    op(
                        "cir.load",
                        &["pld"],
                        &["p"],
                        "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
                    ),
                    op(
                        "cir.ptr_stride",
                        &["loc0"],
                        &["pld", "i0"],
                        "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
                    ),
                    op(
                        "cir.store",
                        &[],
                        &["v1", "loc0"],
                        "(!s32i, !cir.ptr<!s32i>) -> ()",
                    ),
                ]),
                region(vec![
                    const_op("v2", 2, "!s32i"),
                    const_op("i0b", 0, "!s64i"),
                    op(
                        "cir.load",
                        &["pldb"],
                        &["p"],
                        "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
                    ),
                    op(
                        "cir.ptr_stride",
                        &["loc0b"],
                        &["pldb", "i0b"],
                        "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
                    ),
                    op(
                        "cir.store",
                        &[],
                        &["v2", "loc0b"],
                        "(!s32i, !cir.ptr<!s32i>) -> ()",
                    ),
                ]),
            ];
            if_op
        },
        const_op("i0c", 0, "!s64i"),
        op(
            "cir.load",
            &["pldc"],
            &["p"],
            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.ptr_stride",
            &["loc0c"],
            &["pldc", "i0c"],
            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
        ),
        op("cir.load", &["r"], &["loc0c"], "(!cir.ptr<!s32i>) -> !s32i"),
        op("cir.return", &[], &["r"], "(!s32i) -> ()"),
    ]
}

#[test]
fn if_takes_true_branch_and_skips_false_branch_effects() {
    let trace = interpret(&if_else_fixture());
    let alloc = AllocId(0);
    assert_eq!(
        trace.effects,
        vec![
            Effect::Alloc { alloc, size: 4 },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 0
                },
                value: int32(1),
            },
            Effect::Read {
                loc: Location {
                    alloc,
                    byte_offset: 0
                },
                value: int32(1),
            },
            Effect::Exit(1),
        ]
    );
}

/// Mirrors the CIR shape for:
/// `int *p = malloc(3 * sizeof(int));
///  int i;
///  for (i = 0; i < 3; i = i + 1) { p[i] = i + 1; }
///  int sum = 0;
///  for (i = 0; i < 3; i = i + 1) { sum = sum + p[i]; }
///  return sum;`
fn for_loop_fill_and_sum_fixture() -> Vec<Op> {
    let cond_region = |limit: i64| {
        region(vec![
            op("cir.load", &["iv"], &["i"], "(!cir.ptr<!s32i>) -> !s32i"),
            const_op("limit", limit, "!s32i"),
            {
                let mut cmp = op(
                    "cir.cmp",
                    &["cond"],
                    &["iv", "limit"],
                    "(!s32i, !s32i) -> !cir.bool",
                );
                cmp.attrs.insert("kind".to_string(), Attr::Int(0));
                cmp
            },
            op("cir.condition", &[], &["cond"], "(!cir.bool) -> ()"),
        ])
    };
    let step_region = || {
        region(vec![
            op("cir.load", &["ivs"], &["i"], "(!cir.ptr<!s32i>) -> !s32i"),
            const_op("one_s", 1, "!s32i"),
            op(
                "cir.add",
                &["inc"],
                &["ivs", "one_s"],
                "(!s32i, !s32i) -> !s32i",
            ),
            op(
                "cir.store",
                &[],
                &["inc", "i"],
                "(!s32i, !cir.ptr<!s32i>) -> ()",
            ),
        ])
    };

    vec![
        op("cir.alloca", &["p"], &[], "() -> !cir.ptr<!cir.ptr<!s32i>>"),
        op("cir.alloca", &["i"], &[], "() -> !cir.ptr<!s32i>"),
        op("cir.alloca", &["sum"], &[], "() -> !cir.ptr<!s32i>"),
        const_op("c4", 4, "!u64i"),
        const_op("c3", 3, "!u64i"),
        op(
            "cir.mul",
            &["size"],
            &["c4", "c3"],
            "(!u64i, !u64i) -> !u64i",
        ),
        call_malloc("raw", "size"),
        op(
            "cir.cast",
            &["buf"],
            &["raw"],
            "(!cir.ptr<!void>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.store",
            &[],
            &["buf", "p"],
            "(!cir.ptr<!s32i>, !cir.ptr<!cir.ptr<!s32i>>) -> ()",
        ),
        const_op("zero0", 0, "!s32i"),
        op(
            "cir.store",
            &[],
            &["zero0", "i"],
            "(!s32i, !cir.ptr<!s32i>) -> ()",
        ),
        {
            let mut for_op = op("cir.for", &[], &[], "() -> ()");
            for_op.regions = vec![
                cond_region(3),
                region(vec![
                    op("cir.load", &["ivb"], &["i"], "(!cir.ptr<!s32i>) -> !s32i"),
                    const_op("one_b", 1, "!s32i"),
                    op(
                        "cir.add",
                        &["val"],
                        &["ivb", "one_b"],
                        "(!s32i, !s32i) -> !s32i",
                    ),
                    op(
                        "cir.load",
                        &["idx_s32"],
                        &["i"],
                        "(!cir.ptr<!s32i>) -> !s32i",
                    ),
                    op("cir.cast", &["idx"], &["idx_s32"], "(!s32i) -> !s64i"),
                    op(
                        "cir.load",
                        &["pld"],
                        &["p"],
                        "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
                    ),
                    op(
                        "cir.ptr_stride",
                        &["loc"],
                        &["pld", "idx"],
                        "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
                    ),
                    op(
                        "cir.store",
                        &[],
                        &["val", "loc"],
                        "(!s32i, !cir.ptr<!s32i>) -> ()",
                    ),
                ]),
                step_region(),
            ];
            for_op
        },
        const_op("zero1", 0, "!s32i"),
        op(
            "cir.store",
            &[],
            &["zero1", "i"],
            "(!s32i, !cir.ptr<!s32i>) -> ()",
        ),
        const_op("zero_sum", 0, "!s32i"),
        op(
            "cir.store",
            &[],
            &["zero_sum", "sum"],
            "(!s32i, !cir.ptr<!s32i>) -> ()",
        ),
        {
            let mut for_op = op("cir.for", &[], &[], "() -> ()");
            for_op.regions = vec![
                cond_region(3),
                region(vec![
                    op(
                        "cir.load",
                        &["idxr_s32"],
                        &["i"],
                        "(!cir.ptr<!s32i>) -> !s32i",
                    ),
                    op("cir.cast", &["idxr"], &["idxr_s32"], "(!s32i) -> !s64i"),
                    op(
                        "cir.load",
                        &["pldr"],
                        &["p"],
                        "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
                    ),
                    op(
                        "cir.ptr_stride",
                        &["locr"],
                        &["pldr", "idxr"],
                        "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
                    ),
                    op(
                        "cir.load",
                        &["elem"],
                        &["locr"],
                        "(!cir.ptr<!s32i>) -> !s32i",
                    ),
                    op(
                        "cir.load",
                        &["sum_old"],
                        &["sum"],
                        "(!cir.ptr<!s32i>) -> !s32i",
                    ),
                    op(
                        "cir.add",
                        &["sum_new"],
                        &["sum_old", "elem"],
                        "(!s32i, !s32i) -> !s32i",
                    ),
                    op(
                        "cir.store",
                        &[],
                        &["sum_new", "sum"],
                        "(!s32i, !cir.ptr<!s32i>) -> ()",
                    ),
                ]),
                step_region(),
            ];
            for_op
        },
        op(
            "cir.load",
            &["result"],
            &["sum"],
            "(!cir.ptr<!s32i>) -> !s32i",
        ),
        op("cir.return", &[], &["result"], "(!s32i) -> ()"),
    ]
}

/// Mirrors the CIR clang emits for a pointer+length pair parameter:
/// `void bump(int *items, int len) {
///    items[0] += 1; items[1] += 1;
///    return items[0] + items[1] + len;
///  }`
fn bump_len_fixture() -> Vec<Op> {
    vec![
        op(
            "cir.alloca",
            &["items"],
            &[],
            "() -> !cir.ptr<!cir.ptr<!s32i>>",
        ),
        op("cir.alloca", &["len"], &[], "() -> !cir.ptr<!s32i>"),
        op(
            "cir.store",
            &[],
            &["arg0", "items"],
            "(!cir.ptr<!s32i>, !cir.ptr<!cir.ptr<!s32i>>) -> ()",
        ),
        op(
            "cir.store",
            &[],
            &["arg1", "len"],
            "(!s32i, !cir.ptr<!s32i>) -> ()",
        ),
        const_op("v0", 1, "!s32i"),
        const_op("i0", 0, "!s64i"),
        op(
            "cir.load",
            &["p0"],
            &["items"],
            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.ptr_stride",
            &["loc0"],
            &["p0", "i0"],
            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
        ),
        op("cir.load", &["r0"], &["loc0"], "(!cir.ptr<!s32i>) -> !s32i"),
        op(
            "cir.add",
            &["r0b"],
            &["r0", "v0"],
            "(!s32i, !s32i) -> !s32i",
        ),
        op(
            "cir.store",
            &[],
            &["r0b", "loc0"],
            "(!s32i, !cir.ptr<!s32i>) -> ()",
        ),
        const_op("v1", 1, "!s32i"),
        const_op("i1", 1, "!s64i"),
        op(
            "cir.load",
            &["p1"],
            &["items"],
            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.ptr_stride",
            &["loc1"],
            &["p1", "i1"],
            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
        ),
        op("cir.load", &["r1"], &["loc1"], "(!cir.ptr<!s32i>) -> !s32i"),
        op(
            "cir.add",
            &["r1b"],
            &["r1", "v1"],
            "(!s32i, !s32i) -> !s32i",
        ),
        op(
            "cir.store",
            &[],
            &["r1b", "loc1"],
            "(!s32i, !cir.ptr<!s32i>) -> ()",
        ),
        const_op("i0c", 0, "!s64i"),
        op(
            "cir.load",
            &["p0c"],
            &["items"],
            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.ptr_stride",
            &["loc0c"],
            &["p0c", "i0c"],
            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.load",
            &["r0c"],
            &["loc0c"],
            "(!cir.ptr<!s32i>) -> !s32i",
        ),
        const_op("i1c", 1, "!s64i"),
        op(
            "cir.load",
            &["p1c"],
            &["items"],
            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.ptr_stride",
            &["loc1c"],
            &["p1c", "i1c"],
            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
        ),
        op(
            "cir.load",
            &["r1c"],
            &["loc1c"],
            "(!cir.ptr<!s32i>) -> !s32i",
        ),
        op(
            "cir.add",
            &["sum1"],
            &["r0c", "r1c"],
            "(!s32i, !s32i) -> !s32i",
        ),
        op(
            "cir.load",
            &["lenv"],
            &["len"],
            "(!cir.ptr<!s32i>) -> !s32i",
        ),
        op(
            "cir.add",
            &["sum"],
            &["sum1", "lenv"],
            "(!s32i, !s32i) -> !s32i",
        ),
        op("cir.return", &[], &["sum"], "(!s32i) -> ()"),
    ]
}

#[test]
fn function_parameters_seed_the_trace_as_a_buffer_and_a_scalar() {
    let params: Vec<(&str, ParamSeed)> = vec![
        ("arg0", ParamSeed::Buffer(vec![int32(1), int32(2)])),
        ("arg1", ParamSeed::Scalar(int32(2))),
    ];
    let trace = interpret_with_params(&bump_len_fixture(), &params);
    let alloc = AllocId(0);
    assert_eq!(
        trace.effects,
        vec![
            Effect::Read {
                loc: Location {
                    alloc,
                    byte_offset: 0
                },
                value: int32(1),
            },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 0
                },
                value: int32(2),
            },
            Effect::Read {
                loc: Location {
                    alloc,
                    byte_offset: 4
                },
                value: int32(2),
            },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 4
                },
                value: int32(3),
            },
            Effect::Read {
                loc: Location {
                    alloc,
                    byte_offset: 0
                },
                value: int32(2),
            },
            Effect::Read {
                loc: Location {
                    alloc,
                    byte_offset: 4
                },
                value: int32(3),
            },
            Effect::Exit(7),
        ]
    );
}

/// Mirrors the CIR clang emits for:
/// `struct point { int x; int y; };
///  struct point *p = malloc(sizeof(struct point));
///  p->x = 3; p->y = 4;
///  int sum = p->x + p->y;
///  free(p);
///  return sum;`
fn struct_field_fixture() -> Vec<Op> {
    let get_member = |result: &str, base: &str, index: i64, name: &str| {
        let mut o = op(
            "cir.get_member",
            &[result],
            &[base],
            "(!cir.ptr<!rec_point>) -> !cir.ptr<!s32i>",
        );
        o.attrs.insert("index_attr".to_string(), Attr::Int(index));
        o.attrs
            .insert("name".to_string(), Attr::Str(name.to_string()));
        o
    };
    vec![
        op(
            "cir.alloca",
            &["p"],
            &[],
            "() -> !cir.ptr<!cir.ptr<!rec_point>>",
        ),
        const_op("c8", 8, "!u64i"),
        call_malloc("raw", "c8"),
        op(
            "cir.cast",
            &["buf"],
            &["raw"],
            "(!cir.ptr<!void>) -> !cir.ptr<!rec_point>",
        ),
        op(
            "cir.store",
            &[],
            &["buf", "p"],
            "(!cir.ptr<!rec_point>, !cir.ptr<!cir.ptr<!rec_point>>) -> ()",
        ),
        const_op("v3", 3, "!s32i"),
        op(
            "cir.load",
            &["p0"],
            &["p"],
            "(!cir.ptr<!cir.ptr<!rec_point>>) -> !cir.ptr<!rec_point>",
        ),
        get_member("locx", "p0", 0, "x"),
        op(
            "cir.store",
            &[],
            &["v3", "locx"],
            "(!s32i, !cir.ptr<!s32i>) -> ()",
        ),
        const_op("v4", 4, "!s32i"),
        op(
            "cir.load",
            &["p1"],
            &["p"],
            "(!cir.ptr<!cir.ptr<!rec_point>>) -> !cir.ptr<!rec_point>",
        ),
        get_member("locy", "p1", 1, "y"),
        op(
            "cir.store",
            &[],
            &["v4", "locy"],
            "(!s32i, !cir.ptr<!s32i>) -> ()",
        ),
        op(
            "cir.load",
            &["p2"],
            &["p"],
            "(!cir.ptr<!cir.ptr<!rec_point>>) -> !cir.ptr<!rec_point>",
        ),
        get_member("locxr", "p2", 0, "x"),
        op(
            "cir.load",
            &["rx"],
            &["locxr"],
            "(!cir.ptr<!s32i>) -> !s32i",
        ),
        op(
            "cir.load",
            &["p3"],
            &["p"],
            "(!cir.ptr<!cir.ptr<!rec_point>>) -> !cir.ptr<!rec_point>",
        ),
        get_member("locyr", "p3", 1, "y"),
        op(
            "cir.load",
            &["ry"],
            &["locyr"],
            "(!cir.ptr<!s32i>) -> !s32i",
        ),
        op(
            "cir.add",
            &["sum"],
            &["rx", "ry"],
            "(!s32i, !s32i) -> !s32i",
        ),
        call_free("buf"),
        op("cir.return", &[], &["sum"], "(!s32i) -> ()"),
    ]
}

#[test]
fn struct_field_read_write_produces_expected_effects() {
    let trace = interpret(&struct_field_fixture());
    let alloc = AllocId(0);
    assert_eq!(
        trace.effects,
        vec![
            Effect::Alloc { alloc, size: 8 },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 0
                },
                value: int32(3),
            },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 4
                },
                value: int32(4),
            },
            Effect::Read {
                loc: Location {
                    alloc,
                    byte_offset: 0
                },
                value: int32(3),
            },
            Effect::Read {
                loc: Location {
                    alloc,
                    byte_offset: 4
                },
                value: int32(4),
            },
            Effect::Dealloc { alloc },
            Effect::Exit(7),
        ]
    );
}

#[test]
fn for_loop_fills_array_then_sums_it() {
    let trace = interpret(&for_loop_fill_and_sum_fixture());
    let alloc = AllocId(0);
    assert_eq!(
        trace.effects,
        vec![
            Effect::Alloc { alloc, size: 12 },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 0
                },
                value: int32(1),
            },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 4
                },
                value: int32(2),
            },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 8
                },
                value: int32(3),
            },
            Effect::Read {
                loc: Location {
                    alloc,
                    byte_offset: 0
                },
                value: int32(1),
            },
            Effect::Read {
                loc: Location {
                    alloc,
                    byte_offset: 4
                },
                value: int32(2),
            },
            Effect::Read {
                loc: Location {
                    alloc,
                    byte_offset: 8
                },
                value: int32(3),
            },
            Effect::Exit(6),
        ]
    );
}

fn call_strlen(result: &str, ptr_operand: &str) -> Op {
    let mut o = op(
        "cir.call",
        &[result],
        &[ptr_operand],
        "(!cir.ptr<!s8i>) -> !u64i",
    );
    o.attrs
        .insert("callee".to_string(), Attr::Raw("@strlen".to_string()));
    o
}

/// Mirrors: `char *s = malloc(4); s[0]='a'; s[1]='b'; s[2]='c'; s[3]='\0';
/// int len = strlen(s); free(s); return len;`
fn string_strlen_fixture() -> Vec<Op> {
    let store_byte = |value_name: &str, index_name: &str, loc_name: &str, load_name: &str| {
        vec![
            op(
                "cir.load",
                &[load_name],
                &["s"],
                "(!cir.ptr<!cir.ptr<!s8i>>) -> !cir.ptr<!s8i>",
            ),
            op(
                "cir.ptr_stride",
                &[loc_name],
                &[load_name, index_name],
                "(!cir.ptr<!s8i>, !s64i) -> !cir.ptr<!s8i>",
            ),
            op(
                "cir.store",
                &[],
                &[value_name, loc_name],
                "(!s8i, !cir.ptr<!s8i>) -> ()",
            ),
        ]
    };
    let mut ops = vec![
        op("cir.alloca", &["s"], &[], "() -> !cir.ptr<!cir.ptr<!s8i>>"),
        const_op("c4", 4, "!u64i"),
        call_malloc("raw", "c4"),
        op(
            "cir.cast",
            &["buf"],
            &["raw"],
            "(!cir.ptr<!void>) -> !cir.ptr<!s8i>",
        ),
        op(
            "cir.store",
            &[],
            &["buf", "s"],
            "(!cir.ptr<!s8i>, !cir.ptr<!cir.ptr<!s8i>>) -> ()",
        ),
        const_op("va", 97, "!s8i"),
        const_op("i0", 0, "!s64i"),
    ];
    ops.extend(store_byte("va", "i0", "loc0", "p0"));
    ops.push(const_op("vb", 98, "!s8i"));
    ops.push(const_op("i1", 1, "!s64i"));
    ops.extend(store_byte("vb", "i1", "loc1", "p1"));
    ops.push(const_op("vc", 99, "!s8i"));
    ops.push(const_op("i2", 2, "!s64i"));
    ops.extend(store_byte("vc", "i2", "loc2", "p2"));
    ops.push(const_op("vz", 0, "!s8i"));
    ops.push(const_op("i3", 3, "!s64i"));
    ops.extend(store_byte("vz", "i3", "loc3", "p3"));
    ops.push(op(
        "cir.load",
        &["p4"],
        &["s"],
        "(!cir.ptr<!cir.ptr<!s8i>>) -> !cir.ptr<!s8i>",
    ));
    ops.push(call_strlen("len", "p4"));
    ops.push(call_free("p4"));
    ops.push(op("cir.return", &[], &["len"], "(!u64i) -> ()"));
    ops
}

#[test]
fn strlen_scans_a_malloced_buffer_and_pushes_a_call_effect() {
    let trace = interpret(&string_strlen_fixture());
    let alloc = AllocId(0);
    let byte = |value: i128| Value::Int {
        width: IntWidth::W8,
        signed: true,
        value,
    };
    assert_eq!(
        trace.effects,
        vec![
            Effect::Alloc { alloc, size: 4 },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 0
                },
                value: byte(97),
            },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 1
                },
                value: byte(98),
            },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 2
                },
                value: byte(99),
            },
            Effect::Write {
                loc: Location {
                    alloc,
                    byte_offset: 3
                },
                value: byte(0),
            },
            Effect::Call {
                name: "strlen".to_string(),
                args: vec![],
            },
            Effect::Dealloc { alloc },
            Effect::Exit(3),
        ]
    );
}
