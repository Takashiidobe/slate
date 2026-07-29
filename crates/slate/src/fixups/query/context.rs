use std::collections::{BTreeMap, BTreeSet};
use std::marker::PhantomData;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, ConstValue, EffectSubject, FixupFacts, FunctionId, NulTermination, PathSegment,
    Purity, StringBufferKind, ValueSubject,
};
use crate::function_identity::{CallBinding, FunctionIdentity, Known};
use crate::rust_ast::{Expr, Item, Prim, Program, Type};

use super::{
    ByteExtent, ByteRepresentation, ByteSource, ByteView, Evidence, EvidenceDetail, ExprSite,
    NulPosition, PointerMutability, Predicate, Proof, QueryResult, Rejection, RejectionReason,
    StableExpr,
};

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub(in crate::fixups) enum CallTarget {
    Known(Known),
    Generated(String),
    Direct(String),
    Indirect,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct CallRecord {
    pub(in crate::fixups) site: ExprSite,
    pub(in crate::fixups) target: CallTarget,
    pub(in crate::fixups) args: Vec<ExprSite>,
    pub(in crate::fixups) evidence: Vec<Evidence>,
}

pub(in crate::fixups) struct QueryContext<'snapshot> {
    program: &'snapshot Program,
    facts: &'snapshot FixupFacts,
    calls: BTreeMap<(CallTarget, usize), Vec<CallRecord>>,
}

impl<'snapshot> QueryContext<'snapshot> {
    pub(in crate::fixups) fn new(
        program: &'snapshot Program,
        facts: &'snapshot FixupFacts,
    ) -> Self {
        let mut calls: BTreeMap<(CallTarget, usize), Vec<CallRecord>> = BTreeMap::new();
        for (item_index, item) in program.items.iter().enumerate() {
            let Item::Fn(function) = item else {
                continue;
            };
            walk::body_exprs_with_path(&function.body, &mut Vec::new(), &mut |expr, path| {
                let site = expression_site(item_index, path);
                let Expr::Call {
                    func,
                    args,
                    binding,
                } = expr
                else {
                    return;
                };
                let target = call_target(func, binding);
                let arg_sites = (0..args.len())
                    .map(|index| child_site(&site, index + 1))
                    .collect::<Vec<_>>();
                let evidence = vec![Evidence {
                    predicate: Predicate::Call,
                    site: site.clone(),
                    detail: EvidenceDetail::IndexedCall {
                        target: target.clone(),
                        arity: args.len(),
                    },
                }];
                calls
                    .entry((target.clone(), args.len()))
                    .or_default()
                    .push(CallRecord {
                        site,
                        target,
                        args: arg_sites,
                        evidence,
                    });
            });
        }
        Self {
            program,
            facts,
            calls,
        }
    }

    pub(in crate::fixups) fn calls(&self, target: &CallTarget, arity: usize) -> &[CallRecord] {
        self.calls
            .get(&(target.clone(), arity))
            .map(Vec::as_slice)
            .unwrap_or_default()
    }

    pub(in crate::fixups) fn expr(&self, site: &ExprSite) -> Option<&'snapshot Expr> {
        walk::target_expr_at_path(self.program, site.item_index, &site.path)
    }

    pub(in crate::fixups) fn byte_source(
        &self,
        site: &ExprSite,
    ) -> QueryResult<ByteSource<'snapshot>> {
        let predicate = Predicate::ByteSource;
        let function = self.function(site).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let pointer = self
            .facts
            .string_pointer_view(function, &site.fact_path)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::UnsupportedShape,
                    Vec::new(),
                )
            })?;
        let name = self
            .facts
            .binding_name(pointer.source)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?
            .to_string();
        let ty = self.facts.binding_type_ast(pointer.source).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let buffer = self.facts.string_buffer(pointer.source);
        let representation = buffer
            .map(|buffer| representation_for_buffer(buffer.kind))
            .or_else(|| representation_for_type(ty))
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::UnsupportedShape,
                    Vec::new(),
                )
            })?;
        let mutability = if pointer.mutable {
            PointerMutability::Mut
        } else {
            PointerMutability::Const
        };
        let extent = byte_extent(ty);
        let evidence = vec![
            Evidence {
                predicate,
                site: site.clone(),
                detail: EvidenceDetail::Binding { name: name.clone() },
            },
            Evidence {
                predicate,
                site: site.clone(),
                detail: EvidenceDetail::PointerView {
                    representation,
                    mutability,
                },
            },
            Evidence {
                predicate,
                site: site.clone(),
                detail: EvidenceDetail::Extent(extent),
            },
        ];
        Ok(Proof::new(
            ByteSource {
                site: site.clone(),
                name,
                representation,
                mutability,
                extent,
                binding: pointer.source,
                snapshot: PhantomData,
            },
            evidence,
        ))
    }

    pub(in crate::fixups) fn const_u8(&self, site: &ExprSite) -> QueryResult<u8> {
        let values = self.constant_values(Predicate::ConstantU8, site, |value| match value {
            ConstValue::Integer(value) => u8::try_from(*value)
                .map(Some)
                .map_err(|_| RejectionReason::OutOfRange),
            ConstValue::Usize(value) => u8::try_from(*value)
                .map(Some)
                .map_err(|_| RejectionReason::OutOfRange),
            ConstValue::Zero => Ok(Some(0)),
            _ => Ok(None),
        })?;
        if values.len() != 1 {
            return Err(Rejection::new(
                Predicate::ConstantU8,
                Some(site.clone()),
                RejectionReason::Ambiguous,
                Vec::new(),
            ));
        }
        let value = *values.first().unwrap();
        Ok(Proof::new(
            value,
            vec![Evidence {
                predicate: Predicate::ConstantU8,
                site: site.clone(),
                detail: EvidenceDetail::ConstantU8(value),
            }],
        ))
    }

    pub(in crate::fixups) fn const_usize(&self, site: &ExprSite) -> QueryResult<usize> {
        let values = self.constant_values(Predicate::ConstantUsize, site, |value| match value {
            ConstValue::Integer(value) => usize::try_from(*value)
                .map(Some)
                .map_err(|_| RejectionReason::OutOfRange),
            ConstValue::Usize(value) => Ok(Some(*value)),
            ConstValue::Zero => Ok(Some(0)),
            ConstValue::ArrayLength(value) => Ok(Some(*value)),
            _ => Ok(None),
        })?;
        if values.len() != 1 {
            return Err(Rejection::new(
                Predicate::ConstantUsize,
                Some(site.clone()),
                RejectionReason::Ambiguous,
                Vec::new(),
            ));
        }
        let value = *values.first().unwrap();
        Ok(Proof::new(
            value,
            vec![Evidence {
                predicate: Predicate::ConstantUsize,
                site: site.clone(),
                detail: EvidenceDetail::ConstantUsize(value),
            }],
        ))
    }

    pub(in crate::fixups) fn full_byte_view(
        &self,
        source: &ByteSource<'snapshot>,
        count: &ExprSite,
    ) -> QueryResult<ByteView<'snapshot>> {
        let count_proof = self.const_usize(count)?;
        let count_value = count_proof.value;
        let ByteExtent::Constant(extent) = source.extent else {
            return Err(Rejection::new(
                Predicate::FullByteView,
                Some(count.clone()),
                RejectionReason::MissingEvidence,
                count_proof.evidence,
            ));
        };
        if extent != count_value {
            return Err(Rejection::new(
                Predicate::FullByteView,
                Some(count.clone()),
                RejectionReason::Contradicted,
                count_proof.evidence,
            ));
        }
        let mut evidence = count_proof.evidence;
        evidence.push(Evidence {
            predicate: Predicate::FullByteView,
            site: source.site.clone(),
            detail: EvidenceDetail::Extent(source.extent),
        });
        Ok(Proof::new(
            ByteView {
                source: source.clone(),
                extent,
            },
            evidence,
        ))
    }

    pub(in crate::fixups) fn first_nul(
        &self,
        source: &ByteSource<'snapshot>,
    ) -> QueryResult<NulPosition> {
        let predicate = Predicate::FirstNul;
        let Some(buffer) = self.facts.string_buffer(source.binding) else {
            return Err(Rejection::new(
                predicate,
                Some(source.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        if buffer.interior_nul {
            return Err(Rejection::new(
                predicate,
                Some(source.site.clone()),
                RejectionReason::Contradicted,
                Vec::new(),
            ));
        }
        if !buffer.ascii_only {
            let reason = if buffer.bytes.is_some() {
                RejectionReason::Contradicted
            } else {
                RejectionReason::MissingEvidence
            };
            return Err(Rejection::new(
                predicate,
                Some(source.site.clone()),
                reason,
                Vec::new(),
            ));
        }
        if !matches!(
            buffer.nul_termination,
            NulTermination::Terminated | NulTermination::AllZero
        ) {
            let reason = if buffer.nul_termination == NulTermination::Unterminated {
                RejectionReason::Contradicted
            } else {
                RejectionReason::MissingEvidence
            };
            return Err(Rejection::new(
                predicate,
                Some(source.site.clone()),
                reason,
                Vec::new(),
            ));
        }
        let position = buffer
            .bytes
            .as_ref()
            .map(|bytes| NulPosition::Constant(bytes.len()))
            .unwrap_or(NulPosition::ByteLength);
        Ok(Proof::new(
            position,
            vec![Evidence {
                predicate,
                site: source.site.clone(),
                detail: EvidenceDetail::NulPosition(position),
            }],
        ))
    }

    pub(in crate::fixups) fn prefix_contains(
        &self,
        count: &ExprSite,
        nul: NulPosition,
    ) -> QueryResult<()> {
        let count_proof = self.const_usize(count)?;
        let NulPosition::Constant(nul) = nul else {
            return Err(Rejection::new(
                Predicate::PrefixContains,
                Some(count.clone()),
                RejectionReason::MissingEvidence,
                count_proof.evidence,
            ));
        };
        if nul >= count_proof.value {
            return Err(Rejection::new(
                Predicate::PrefixContains,
                Some(count.clone()),
                RejectionReason::Contradicted,
                count_proof.evidence,
            ));
        }
        let count_value = count_proof.value;
        let mut evidence = count_proof.evidence;
        evidence.push(Evidence {
            predicate: Predicate::PrefixContains,
            site: count.clone(),
            detail: EvidenceDetail::PrefixContains {
                count: count_value,
                nul,
            },
        });
        Ok(Proof::new((), evidence))
    }

    pub(in crate::fixups) fn pure(&self, site: &ExprSite) -> QueryResult<StableExpr> {
        let predicate = Predicate::MovablePure;
        let function = self.function(site).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let effect = self
            .facts
            .effect(function, EffectSubject::Expr, &site.fact_path)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        if effect.purity != Purity::MovablePure {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::Contradicted,
                Vec::new(),
            ));
        }
        Ok(Proof::new(
            StableExpr { site: site.clone() },
            vec![Evidence {
                predicate,
                site: site.clone(),
                detail: EvidenceDetail::MovablePure,
            }],
        ))
    }

    fn function(&self, site: &ExprSite) -> Option<FunctionId> {
        self.program.items.get(site.item_index)?;
        self.facts.function_by_item_index(site.item_index)
    }

    fn constant_values<T: Ord>(
        &self,
        predicate: Predicate,
        site: &ExprSite,
        convert: impl Fn(&ConstValue) -> Result<Option<T>, RejectionReason>,
    ) -> Result<Vec<T>, Rejection> {
        let function = self.function(site).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let mut values = BTreeSet::new();
        for value in self
            .facts
            .values_at(function, ValueSubject::Expr, &site.fact_path)
        {
            match convert(value) {
                Ok(Some(value)) => {
                    values.insert(value);
                }
                Ok(None) => {}
                Err(reason) => {
                    return Err(Rejection::new(
                        predicate,
                        Some(site.clone()),
                        reason,
                        Vec::new(),
                    ));
                }
            }
        }
        if values.is_empty() {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        Ok(values.into_iter().collect())
    }
}

fn expression_site(item_index: usize, path: &[PathSegment]) -> ExprSite {
    let path = AstPath(path.to_vec());
    ExprSite {
        item_index,
        fact_path: fact_path(&path),
        path,
    }
}

fn child_site(parent: &ExprSite, index: usize) -> ExprSite {
    let mut path = parent.path.clone();
    path.0.push(PathSegment::Expr(index));
    ExprSite {
        item_index: parent.item_index,
        fact_path: fact_path(&path),
        path,
    }
}

fn fact_path(path: &AstPath) -> AstPath {
    let mut fact = path.0.clone();
    if let Some(stmt) = fact
        .iter()
        .rposition(|segment| matches!(segment, PathSegment::Stmt(_)))
        && matches!(fact.get(stmt + 1), Some(PathSegment::Expr(_)))
    {
        fact.remove(stmt + 1);
    } else if let Some(stmt) = fact
        .iter()
        .rposition(|segment| matches!(segment, PathSegment::Stmt(_)))
        && matches!(
            fact.get(stmt + 1..stmt + 3),
            Some([PathSegment::Then | PathSegment::Else, PathSegment::Expr(0)])
        )
    {
        fact.remove(stmt + 2);
    }
    AstPath(fact)
}

fn call_target(func: &Expr, binding: &CallBinding) -> CallTarget {
    let name = match func {
        Expr::Var(name) => Some(name.as_str()),
        _ => None,
    };
    match binding {
        CallBinding::Generated => name
            .map(|name| CallTarget::Generated(name.to_string()))
            .unwrap_or(CallTarget::Indirect),
        CallBinding::Direct {
            identity: FunctionIdentity::Known(known),
            ..
        } if name.is_some_and(|name| known.matches_symbol(name)) => CallTarget::Known(*known),
        CallBinding::Direct { .. } => name
            .map(|name| CallTarget::Direct(name.to_string()))
            .unwrap_or(CallTarget::Indirect),
        CallBinding::Indirect => CallTarget::Indirect,
    }
}

fn representation_for_buffer(kind: StringBufferKind) -> ByteRepresentation {
    match kind {
        StringBufferKind::CharArray => ByteRepresentation::Collection,
        StringBufferKind::BorrowedBytes => ByteRepresentation::Bytes,
        StringBufferKind::BorrowedCStr => ByteRepresentation::CStr,
        StringBufferKind::BorrowedStr | StringBufferKind::OwnedString => ByteRepresentation::Str,
    }
}

fn representation_for_type(ty: &Type) -> Option<ByteRepresentation> {
    match ty.peel_aligned() {
        Type::Array { elem, .. } | Type::Slice(elem)
            if matches!(&**elem, Type::Prim(Prim::I8 | Prim::U8)) =>
        {
            Some(ByteRepresentation::Collection)
        }
        Type::Ref { inner, .. } => match &**inner {
            Type::Slice(elem) if matches!(&**elem, Type::Prim(Prim::I8 | Prim::U8)) => {
                Some(ByteRepresentation::Bytes)
            }
            Type::Str => Some(ByteRepresentation::Str),
            Type::Custom(name) if name == "core::ffi::CStr" => Some(ByteRepresentation::CStr),
            _ => None,
        },
        Type::Custom(name) if name == "String" => Some(ByteRepresentation::Str),
        _ => None,
    }
}

fn byte_extent(ty: &Type) -> ByteExtent {
    match ty.peel_aligned() {
        Type::Array { len, elem } if matches!(&**elem, Type::Prim(Prim::I8 | Prim::U8)) => {
            usize::try_from(*len)
                .map(ByteExtent::Constant)
                .unwrap_or(ByteExtent::Dynamic)
        }
        _ => ByteExtent::Dynamic,
    }
}

#[cfg(test)]
mod tests {
    use crate::fixups::facts;
    use crate::fixups::query::{
        ByteExtent, ByteRepresentation, CallTarget, NulPosition, PointerMutability, QueryContext,
        RejectionReason,
    };
    use crate::fixups::test_support::{call, func, int, temp, var};
    use crate::function_identity::Known;
    use crate::rust_ast::{Expr, Item, Program, RustValue};

    fn array_ptr(name: &str, mutable: bool) -> Expr {
        Expr::ArrayPtr {
            array: Box::new(var(name)),
            mutable,
        }
    }

    fn program() -> Program {
        Program {
            items: vec![Item::Fn(func(
                Vec::new(),
                None,
                vec![
                    temp(
                        "bytes",
                        "[u8; 4]",
                        Expr::ArrayLit(vec![int(10), int(20), int(30), int(40)]),
                    ),
                    temp(
                        "text",
                        "[i8; 4]",
                        Expr::ArrayLit(vec![int(97), int(98), int(99), int(0)]),
                    ),
                    temp(
                        "needle",
                        "i32",
                        Expr::Index {
                            base: Box::new(var("bytes")),
                            index: Box::new(int(1)),
                        },
                    ),
                    temp(
                        "found_bytes",
                        "*mut core::ffi::c_void",
                        call(
                            "__slate_memchr",
                            vec![array_ptr("bytes", true), var("needle"), int(4)],
                        ),
                    ),
                    temp(
                        "found_nul",
                        "*mut core::ffi::c_void",
                        call(
                            "__slate_memchr",
                            vec![array_ptr("text", false), int(0), int(4)],
                        ),
                    ),
                    temp(
                        "short_nul",
                        "*mut core::ffi::c_void",
                        call(
                            "__slate_memchr",
                            vec![array_ptr("text", false), int(0), int(3)],
                        ),
                    ),
                    temp("raw", "*const u8", Expr::Value(RustValue::NullPtr)),
                    temp(
                        "raw_search",
                        "*mut core::ffi::c_void",
                        call("__slate_memchr", vec![var("raw"), int(0), int(4)]),
                    ),
                    temp(
                        "wide_needle",
                        "*mut core::ffi::c_void",
                        call(
                            "__slate_memchr",
                            vec![array_ptr("bytes", true), int(300), int(4)],
                        ),
                    ),
                    temp(
                        "text_len",
                        "usize",
                        call("strlen", vec![array_ptr("text", false)]),
                    ),
                ],
            ))],
        }
    }

    #[test]
    fn indexes_calls_and_proves_memchr_capabilities() {
        let program = program();
        let analyzed = facts::analyze(&program);
        let query = QueryContext::new(analyzed.program, &analyzed.facts);
        let memchr = query.calls(&CallTarget::Generated("__slate_memchr".into()), 3);

        assert_eq!(memchr.len(), 5);
        assert!(matches!(
            query.expr(&memchr[0].site),
            Some(Expr::Call { .. })
        ));
        assert!(matches!(
            query.expr(&memchr[0].args[1]),
            Some(Expr::Var(name)) if name.as_str() == "needle"
        ));

        let bytes = query.byte_source(&memchr[0].args[0]).unwrap().value;
        assert_eq!(bytes.name, "bytes");
        assert_eq!(bytes.representation, ByteRepresentation::Collection);
        assert_eq!(bytes.mutability, PointerMutability::Mut);
        assert_eq!(bytes.extent, ByteExtent::Constant(4));
        assert_eq!(
            query
                .full_byte_view(&bytes, &memchr[0].args[2])
                .unwrap()
                .value
                .extent,
            4
        );
        assert!(query.pure(&memchr[0].args[1]).is_ok());

        let text = query.byte_source(&memchr[1].args[0]).unwrap().value;
        assert_eq!(text.mutability, PointerMutability::Const);
        assert_eq!(query.const_u8(&memchr[1].args[1]).unwrap().value, 0);
        assert_eq!(query.const_usize(&memchr[1].args[2]).unwrap().value, 4);
        let nul = query.first_nul(&text).unwrap().value;
        assert_eq!(nul, NulPosition::Constant(3));
        assert!(query.prefix_contains(&memchr[1].args[2], nul).is_ok());
        assert_eq!(
            query
                .prefix_contains(&memchr[2].args[2], nul)
                .unwrap_err()
                .reason,
            RejectionReason::Contradicted
        );
        assert_eq!(
            query.byte_source(&memchr[3].args[0]).unwrap_err().reason,
            RejectionReason::UnsupportedShape
        );
        assert_eq!(
            query.const_u8(&memchr[4].args[1]).unwrap_err().reason,
            RejectionReason::OutOfRange
        );
        assert_eq!(query.calls(&CallTarget::Known(Known::StrLen), 1).len(), 1);
    }

    #[test]
    fn rejects_unproven_constants_and_partial_views() {
        let program = program();
        let analyzed = facts::analyze(&program);
        let query = QueryContext::new(analyzed.program, &analyzed.facts);
        let memchr = query.calls(&CallTarget::Generated("__slate_memchr".into()), 3);
        let bytes = query.byte_source(&memchr[0].args[0]).unwrap().value;

        assert_eq!(
            query.const_u8(&memchr[0].args[1]).unwrap_err().reason,
            RejectionReason::MissingEvidence
        );
        assert_eq!(
            query
                .full_byte_view(&bytes, &memchr[2].args[2])
                .unwrap_err()
                .reason,
            RejectionReason::Contradicted
        );
    }
}
