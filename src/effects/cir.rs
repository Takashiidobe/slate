//! CIR op-tree -> [`super::EffectTrace`].
//!
//! Scope is deliberately narrow (slate-0hf.2): a flat, single-block op
//! sequence covering exactly what a `malloc` + indexed-store + indexed-load
//! fixture needs — `cir.alloca`, `cir.const`, `cir.mul`/`cir.add`, `cir.call`
//! (only the `malloc` callee), `cir.cast` (pointer bitcast only), `cir.store`,
//! `cir.load`, `cir.ptr_stride`, and `cir.return`. No control flow, no general
//! C stdlib coverage — an unrecognized op or callee panics rather than
//! silently dropping an effect, since a dropped effect would be a false
//! "equivalent" verdict later, which is worse than a loud failure now.

use std::collections::{HashMap, HashSet};

use crate::cir::ir::{Attr, Op};

use super::{AllocId, Effect, EffectTrace, IntWidth, Location, Value};

/// Walks a flat function-body op sequence and returns the effects it produced.
pub fn interpret(ops: &[Op]) -> EffectTrace {
    let mut interp = Interp::default();
    interp.run(ops);
    interp.trace
}

#[derive(Default)]
struct Interp {
    /// Every SSA name's current value, including the pointer/int value
    /// currently held by an alloca'd local (see `locals`).
    env: HashMap<String, Value>,
    /// SSA names that denote a stack slot (an alloca result) rather than an
    /// ordinary value binding — stores/loads through these are local-variable
    /// bookkeeping, not observable effects.
    locals: HashSet<String>,
    heap: HashMap<Location, Value>,
    next_alloc: u32,
    trace: EffectTrace,
}

impl Interp {
    fn run(&mut self, ops: &[Op]) {
        for op in ops {
            self.step(op);
        }
    }

    fn step(&mut self, op: &Op) {
        match op.name.as_str() {
            "cir.alloca" => {
                let result = first_result(op);
                self.locals.insert(result.to_string());
            }
            // The get_global preceding a direct `callee = @sym` call is never
            // consumed by that call (the callee comes from the attribute, not
            // an operand) — nothing to model.
            "cir.get_global" => {}
            "cir.const" => {
                let result = first_result(op);
                let raw = attr_str(op, "value").unwrap_or_default();
                let value = int_const_value(raw, result_type(op));
                self.env.insert(result.to_string(), value);
            }
            "cir.mul" => self.binop(op, i128::wrapping_mul),
            "cir.add" => self.binop(op, i128::wrapping_add),
            "cir.cast" => {
                // Only pointer bitcasts appear in this slice (void* <-> T*);
                // the abstract Location a Ref carries doesn't change shape.
                let result = first_result(op);
                let value = self.resolve(&op.operands[0]);
                self.env.insert(result.to_string(), value);
            }
            "cir.call" => self.call(op),
            "cir.ptr_stride" => self.ptr_stride(op),
            "cir.store" => self.store(op),
            "cir.load" => self.load(op),
            "cir.return" => {
                let value = op
                    .operands
                    .first()
                    .map(|name| self.resolve(name))
                    .unwrap_or(Value::Null);
                self.trace.push(Effect::Return(value));
            }
            other => panic!("effects::cir: unsupported op `{other}`"),
        }
    }

    fn binop(&mut self, op: &Op, f: impl FnOnce(i128, i128) -> i128) {
        let result = first_result(op);
        let (a, width, signed) = self.resolve_int(&op.operands[0]);
        let (b, _, _) = self.resolve_int(&op.operands[1]);
        let value = Value::Int {
            width,
            signed,
            value: f(a, b),
        };
        self.env.insert(result.to_string(), value);
    }

    fn call(&mut self, op: &Op) {
        let callee = attr_str(op, "callee")
            .map(|s| s.trim_start_matches('@'))
            .unwrap_or_default();
        match callee {
            "malloc" => {
                let (size, ..) = self.resolve_int(&op.operands[0]);
                let alloc = AllocId(self.next_alloc);
                self.next_alloc += 1;
                self.trace.push(Effect::Alloc {
                    alloc,
                    size: size as u64,
                });
                let result = first_result(op);
                self.env.insert(
                    result.to_string(),
                    Value::Ref(Location {
                        alloc,
                        byte_offset: 0,
                    }),
                );
            }
            other => panic!("effects::cir: unsupported call target `{other}`"),
        }
    }

    fn ptr_stride(&mut self, op: &Op) {
        let result = first_result(op);
        let base = self.resolve_ref(&op.operands[0]);
        let (index, ..) = self.resolve_int(&op.operands[1]);
        let elem_size = pointee_byte_size(result_type(op)).expect("ptr_stride pointee size");
        let loc = Location {
            alloc: base.alloc,
            byte_offset: base
                .byte_offset
                .wrapping_add((index * elem_size as i128) as u64),
        };
        self.env.insert(result.to_string(), Value::Ref(loc));
    }

    fn store(&mut self, op: &Op) {
        let value = self.resolve(&op.operands[0]);
        let place = &op.operands[1];
        if self.locals.contains(place) {
            self.env.insert(place.clone(), value);
        } else {
            let loc = self.resolve_ref(place);
            self.heap.insert(loc, value);
            self.trace.push(Effect::Write { loc, value });
        }
    }

    fn load(&mut self, op: &Op) {
        let result = first_result(op);
        let place = &op.operands[0];
        if self.locals.contains(place) {
            let value = self.env[place];
            self.env.insert(result.to_string(), value);
        } else {
            let loc = self.resolve_ref(place);
            let value = *self
                .heap
                .get(&loc)
                .unwrap_or_else(|| panic!("effects::cir: read from never-written {loc:?}"));
            self.env.insert(result.to_string(), value);
            self.trace.push(Effect::Read { loc, value });
        }
    }

    fn resolve(&self, name: &str) -> Value {
        self.env[name]
    }

    fn resolve_int(&self, name: &str) -> (i128, IntWidth, bool) {
        match self.env[name] {
            Value::Int {
                width,
                signed,
                value,
            } => (value, width, signed),
            other => panic!("effects::cir: expected int value for {name}, found {other:?}"),
        }
    }

    fn resolve_ref(&self, name: &str) -> Location {
        match self.env[name] {
            Value::Ref(loc) => loc,
            other => panic!("effects::cir: expected pointer value for {name}, found {other:?}"),
        }
    }
}

fn first_result(op: &Op) -> &str {
    op.results.first().map(String::as_str).unwrap_or_default()
}

fn attr_str<'a>(op: &'a Op, key: &str) -> Option<&'a str> {
    op.attrs.get(key).and_then(Attr::as_str)
}

/// `op.ty` is `(params) -> ret`; this slice's types never nest a second
/// `->`, so a plain rightmost split is enough (unlike lower.rs's general
/// top-level-aware splitter, which this module intentionally does not
/// depend on — this interpreter stays self-contained from the real lowerer).
fn result_type(op: &Op) -> Option<&str> {
    let ty = op.ty.as_deref()?;
    let idx = ty.rfind("->")?;
    Some(ty[idx + 2..].trim())
}

/// `!s32i` / `!u64i` etc. — CIR's generic printer always spells fixed-width
/// integers this way, so no alias-table lookup is needed.
fn int_type_width_signed(ty: &str) -> Option<(bool, u32)> {
    let rest = ty.trim().strip_prefix('!')?;
    let signed = match rest.as_bytes().first()? {
        b's' => true,
        b'u' => false,
        _ => return None,
    };
    let bits: u32 = rest[1..].strip_suffix('i')?.parse().ok()?;
    Some((signed, bits))
}

fn int_width(bits: u32) -> IntWidth {
    match bits {
        8 => IntWidth::W8,
        16 => IntWidth::W16,
        32 => IntWidth::W32,
        64 => IntWidth::W64,
        128 => IntWidth::W128,
        _ => IntWidth::PointerSized,
    }
}

fn int_const_value(raw: &str, ty: Option<&str>) -> Value {
    let start = raw
        .find("#cir.int<")
        .expect("cir.const: expected #cir.int<>");
    let rest = &raw[start + "#cir.int<".len()..];
    let end = rest.find('>').expect("cir.const: unterminated #cir.int<>");
    let value: i128 = rest[..end].parse().expect("cir.const: non-integer literal");
    let (signed, bits) = ty.and_then(int_type_width_signed).unwrap_or((true, 32));
    Value::Int {
        width: int_width(bits),
        signed,
        value,
    }
}

fn pointee_byte_size(ptr_ty: Option<&str>) -> Option<u64> {
    let inner = ptr_ty?
        .trim()
        .strip_prefix("!cir.ptr<")?
        .strip_suffix('>')?;
    let (_, bits) = int_type_width_signed(inner)?;
    Some((bits / 8) as u64)
}

#[cfg(test)]
mod tests {
    use super::*;
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

    /// Mirrors (minus printf/free) the CIR clang actually emits for:
    /// `int *p = malloc(2 * sizeof(int)); p[0] = 1; p[1] = 2;
    ///  return p[0] + p[1];`
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
            op("cir.return", &[], &["sum"], "(!s32i) -> ()"),
        ]
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
                Effect::Return(int32(3)),
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
}
