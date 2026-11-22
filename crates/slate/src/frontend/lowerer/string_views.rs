use super::libc_calls::{
    c_content, cast, const_len, duplicable, method_call, support_call, usize_lit,
};
use super::*;
use crate::function_identity::Known;

#[derive(Clone)]
enum View {
    Str(Expr),
    CStr(Expr),
    Literal(Vec<u8>),
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum Mode {
    Str,
    CStr,
    Bytes,
}

pub(super) enum LocalString {
    Str(String),
    CStr(Vec<u8>),
}

fn utf8(bytes: &[u8]) -> bool {
    std::str::from_utf8(bytes).is_ok()
}

fn mode(views: &[&View]) -> Mode {
    if views.iter().all(|view| match view {
        View::Str(_) => true,
        View::Literal(bytes) => utf8(bytes),
        View::CStr(_) => false,
    }) {
        Mode::Str
    } else if views.iter().all(|view| !matches!(view, View::Str(_))) {
        Mode::CStr
    } else {
        Mode::Bytes
    }
}

fn render(view: &View, mode: Mode) -> Expr {
    match (view, mode) {
        (View::Literal(bytes), Mode::Str) => Expr::Str(String::from_utf8_lossy(bytes).into_owned()),
        (View::Literal(bytes), Mode::CStr) => Expr::CStr(bytes.clone()),
        (View::Literal(bytes), Mode::Bytes) => Expr::ByteStr(bytes.clone()),
        (View::Str(expr), Mode::Bytes) => method_call(expr.clone(), "as_bytes", Vec::new()),
        (View::CStr(expr), Mode::Bytes) => method_call(expr.clone(), "to_bytes", Vec::new()),
        (View::Str(expr) | View::CStr(expr), Mode::Str | Mode::CStr) => expr.clone(),
    }
}

fn length(view: &View) -> Expr {
    match view {
        View::CStr(expr) => method_call(expr.clone(), "count_bytes", Vec::new()),
        View::Str(_) | View::Literal(_) => {
            method_call(render(view, mode(&[view])), "len", Vec::new())
        }
    }
}

fn bytes_iter(view: &View, mode: Mode) -> Expr {
    if mode == Mode::Str {
        method_call(render(view, Mode::Str), "bytes", Vec::new())
    } else {
        method_call(render(view, Mode::Bytes), "iter", Vec::new())
    }
}

fn prefix(view: &View, mode: Mode, bound: Expr) -> Expr {
    method_call(bytes_iter(view, mode), "take", vec![bound])
}

fn span(subject: &View, set: &View, accepted: bool) -> Expr {
    let argument = |view: &View| match mode(&[view]) {
        Mode::Str => render(view, Mode::Str),
        Mode::CStr | Mode::Bytes => render(view, Mode::Bytes),
    };
    support_call(
        if accepted {
            "__slate_strspn"
        } else {
            "__slate_strcspn"
        },
        vec![argument(subject), argument(set)],
        false,
    )
}

fn parsed_integer(text: &str, prim: Prim) -> bool {
    let digits = text.strip_prefix(['+', '-']).unwrap_or(text);
    !digits.is_empty()
        && digits.bytes().all(|byte| byte.is_ascii_digit())
        && match prim {
            Prim::I32 => text.parse::<i32>().is_ok(),
            Prim::I64 => text.parse::<i64>().is_ok(),
            #[expect(
                slate::sensitive_wildcard,
                reason = "atoi, atol and atoll return i32 or i64"
            )]
            _ => false,
        }
}

fn parsed_float(text: &str) -> bool {
    text.bytes()
        .all(|byte| byte.is_ascii_digit() || b"+-.eE".contains(&byte))
        && text.parse::<f64>().is_ok_and(f64::is_finite)
}

fn binary(op: ir::CompareOp, lhs: Expr, rhs: Expr) -> Expr {
    Expr::Binary {
        op: match op {
            ir::CompareOp::Eq => BinOp::Eq,
            ir::CompareOp::Ne => BinOp::Ne,
            ir::CompareOp::Lt => BinOp::Lt,
            ir::CompareOp::Le => BinOp::Le,
            ir::CompareOp::Gt => BinOp::Gt,
            ir::CompareOp::Ge => BinOp::Ge,
        },
        lhs: Box::new(lhs),
        rhs: Box::new(rhs),
    }
}

fn negated(expr: Expr, op: ir::CompareOp) -> Expr {
    match op {
        ir::CompareOp::Ne => Expr::Unary {
            op: rust::UnaryOp::Not,
            expr: Box::new(expr),
        },
        ir::CompareOp::Eq
        | ir::CompareOp::Lt
        | ir::CompareOp::Le
        | ir::CompareOp::Gt
        | ir::CompareOp::Ge => expr,
    }
}

fn flipped(op: ir::CompareOp) -> ir::CompareOp {
    match op {
        ir::CompareOp::Lt => ir::CompareOp::Gt,
        ir::CompareOp::Le => ir::CompareOp::Ge,
        ir::CompareOp::Gt => ir::CompareOp::Lt,
        ir::CompareOp::Ge => ir::CompareOp::Le,
        ir::CompareOp::Eq | ir::CompareOp::Ne => op,
    }
}

fn is_zero(value: &ir::Value) -> bool {
    match &value.node.value {
        ValueKind::Constant(ir::Number::Integer(number)) => number.bits() == 0,
        ValueKind::Constant(ir::Number::SignedInteger(number)) => number.bits() == 0,
        #[expect(
            slate::sensitive_wildcard,
            reason = "only an integer zero compares a strcmp result by sign"
        )]
        _ => false,
    }
}

fn literal_prefix_equal(views: &[View], bound: u64, op: ir::CompareOp) -> Option<Expr> {
    let (other, literal) = match views {
        [other, View::Literal(literal)] | [View::Literal(literal), other] => (other, literal),
        _ => return None,
    };
    let bound = usize::try_from(bound).ok()?;
    if bound > literal.len() {
        let literal = View::Literal(literal.clone());
        let mode = mode(&[other, &literal]);
        return Some(binary(op, render(other, mode), render(&literal, mode)));
    }
    let start = View::Literal(literal[..bound].to_vec());
    let expr = if mode(&[other, &start]) == Mode::Str {
        method_call(
            render(other, Mode::Str),
            "starts_with",
            vec![render(&start, Mode::Str)],
        )
    } else {
        method_call(
            render(other, Mode::Bytes),
            "starts_with",
            vec![render(&start, Mode::Bytes)],
        )
    };
    Some(negated(expr, op))
}

fn wrapped(stmts: Vec<Stmt>, expr: Expr) -> Expr {
    if stmts.is_empty() {
        expr
    } else {
        Expr::Block(Box::new(rust::Block {
            stmts,
            tail: Some(Box::new(expr)),
        }))
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn string_local(&self, binding: BindingId) -> Option<LocalString> {
        if self.unborrowed.contains(&binding) || self.switch_lets.contains(&binding) {
            return None;
        }
        let fact = self.tables.constant_string_of(binding)?;
        let content = c_content(&fact.bytes);
        if fact.only_read_as_string
            && let Ok(text) = std::str::from_utf8(content)
            && fact.readers.iter().all(|&reader| self.parses(reader, text))
        {
            return Some(LocalString::Str(text.to_owned()));
        }
        let lifts = fact.readers.iter().any(|reader| {
            !matches!(
                reader,
                Known::Atoi | Known::Atol | Known::Atoll | Known::Atof
            )
        });
        (lifts && (fact.only_read_as_string || !fact.bytes.contains(&0)))
            .then(|| LocalString::CStr(content.to_vec()))
    }

    fn parses(&self, reader: Known, text: &str) -> bool {
        match reader {
            Known::Atoi => parsed_integer(text, Prim::I32),
            Known::Atol => {
                parsed_integer(text, crate::backend::engine::long_prim(self.tables.target))
            }
            Known::Atoll => parsed_integer(text, Prim::I64),
            Known::Atof => parsed_float(text),
            #[expect(
                slate::sensitive_wildcard,
                reason = "only the ato* readers need their argument to parse"
            )]
            _ => true,
        }
    }

    fn string_view(&self, value: &ir::Value) -> Option<View> {
        if self.tables.macro_values.contains_key(&value.node.id) {
            return None;
        }
        match &value.node.value {
            ValueKind::Copy { operand, .. }
            | ValueKind::Convert {
                kind: ir::ConversionKind::PointerCast,
                operand,
                ..
            } => self.string_view(operand),
            ValueKind::ArrayDecay {
                place:
                    ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ..
            } => {
                let (0, content) = self.tables.strings.get(id)?.split_last()? else {
                    return None;
                };
                Some(View::Literal(c_content(content).to_vec()))
            }
            ValueKind::Read {
                place:
                    ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ordering: None,
            } => {
                let name = Expr::var(self.binding_name(*id).as_str().into());
                match self.string_local(*id) {
                    Some(LocalString::Str(_)) => Some(View::Str(name)),
                    Some(LocalString::CStr(_)) => Some(View::CStr(name)),
                    None => self.c_string_of(*id).then_some(View::CStr(name)),
                }
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only literals, constant-string locals and C-string parameters have a string view"
            )]
            _ => None,
        }
    }

    fn string_views(&self, known: Known, arguments: &[ir::Value]) -> Option<Vec<View>> {
        let (strings, arity) = known.string_arguments()?;
        if arguments.len() != arity {
            return None;
        }
        strings
            .iter()
            .map(|&index| self.string_view(&arguments[index]))
            .collect()
    }

    fn string_bound(&mut self, value: &ir::Value, stmts: &mut Vec<Stmt>) -> Result<Expr> {
        let lowered = self.lower_value(value)?;
        if let Some(n) = const_len(&lowered) {
            return Ok(usize_lit(n));
        }
        let bound = cast(lowered, rust::Type::Prim(Prim::Usize));
        if duplicable(&bound) {
            return Ok(bound);
        }
        let temp = self.next_temp();
        stmts.push(Stmt::Let {
            origin: rust::LocalOrigin::Synthetic,
            name: temp.clone(),
            mutable: false,
            ty: None,
            init: Some(bound),
        });
        Ok(Expr::var(temp.as_str().into()))
    }

    pub(super) fn lower_string_call(
        &mut self,
        known: Known,
        arguments: &[ir::Value],
        signature: &ir::Type,
    ) -> Result<Option<Expr>> {
        let Some(views) = self.string_views(known, arguments) else {
            return Ok(None);
        };
        let result = self.call_result(signature)?;
        let mut stmts = Vec::new();
        let expr = match known {
            Known::StrLen => cast(length(&views[0]), result),
            Known::StrNLen => {
                let bound = self.string_bound(&arguments[1], &mut stmts)?;
                cast(method_call(length(&views[0]), "min", vec![bound]), result)
            }
            Known::StrCmp => {
                let mode = mode(&[&views[0], &views[1]]);
                cast(
                    method_call(
                        render(&views[0], mode),
                        "cmp",
                        vec![render(&views[1], mode)],
                    ),
                    result,
                )
            }
            Known::StrNCmp => {
                let bound = self.string_bound(&arguments[2], &mut stmts)?;
                let mode = mode(&[&views[0], &views[1]]);
                cast(
                    method_call(
                        prefix(&views[0], mode, bound.clone()),
                        "cmp",
                        vec![prefix(&views[1], mode, bound)],
                    ),
                    result,
                )
            }
            Known::StrSpn => cast(span(&views[0], &views[1], true), result),
            Known::StrCSpn => cast(span(&views[0], &views[1], false), result),
            Known::Atoi | Known::Atol | Known::Atoll | Known::Atof => {
                let rust::Type::Prim(prim) = result else {
                    return Ok(None);
                };
                let parses = |text: &str| match known {
                    Known::Atof => parsed_float(text),
                    #[expect(
                        slate::sensitive_wildcard,
                        reason = "every other reader here parses an integer"
                    )]
                    _ => parsed_integer(text, prim),
                };
                let text = match &views[0] {
                    View::Literal(bytes) if std::str::from_utf8(bytes).is_ok_and(parses) => {
                        render(&views[0], Mode::Str)
                    }
                    View::Str(expr) => expr.clone(),
                    View::Literal(_) | View::CStr(_) => return Ok(None),
                };
                method_call(
                    Expr::MethodCallGeneric {
                        recv: Box::new(text),
                        method: "parse".into(),
                        type_args: vec![result],
                        args: Vec::new(),
                        unsafe_: rust::Unsafe(false),
                    },
                    "unwrap",
                    Vec::new(),
                )
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "string_views returned None for every other function"
            )]
            _ => return Ok(None),
        };
        Ok(Some(wrapped(stmts, expr)))
    }

    pub(super) fn string_comparison(
        &mut self,
        op: ir::CompareOp,
        left: &ir::Value,
        right: &ir::Value,
    ) -> Result<Option<Expr>> {
        let (call, op) = if is_zero(right) {
            (left, op)
        } else if is_zero(left) {
            (right, flipped(op))
        } else {
            return Ok(None);
        };
        let ValueKind::Call {
            callee, arguments, ..
        } = &call.node.value
        else {
            return Ok(None);
        };
        let Some(known @ (Known::StrCmp | Known::StrNCmp)) = self.known_callee(callee) else {
            return Ok(None);
        };
        let Some(views) = self.string_views(known, arguments) else {
            return Ok(None);
        };
        let ordered = !matches!(op, ir::CompareOp::Eq | ir::CompareOp::Ne);
        let mode = mode(&[&views[0], &views[1]]);
        if known == Known::StrCmp {
            if ordered && mode == Mode::Bytes {
                return Ok(None);
            }
            return Ok(Some(binary(
                op,
                render(&views[0], mode),
                render(&views[1], mode),
            )));
        }
        if ordered {
            return Ok(None);
        }
        let mut stmts = Vec::new();
        let bound = self.string_bound(&arguments[2], &mut stmts)?;
        if let Some(n) = const_len(&bound)
            && let Some(expr) = literal_prefix_equal(&views, n, op)
        {
            return Ok(Some(wrapped(stmts, expr)));
        }
        let equal = method_call(
            prefix(&views[0], mode, bound.clone()),
            "eq",
            vec![prefix(&views[1], mode, bound)],
        );
        Ok(Some(wrapped(stmts, negated(equal, op))))
    }
}
