use std::collections::{BTreeMap, BTreeSet};
use std::marker::PhantomData;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, ConstValue, EffectSubject, FixupFacts, FunctionId, NulTermination, PathSegment,
    Purity, StringBufferKind, ValueSubject,
};
use crate::function_identity::{CallBinding, FunctionIdentity, Known, known_declaration};
use crate::rust_ast::{Attr, Expr, ExternDecl, ImplItem, Item, Prim, Program, Type, Visibility};

use super::{
    AnonymousStructField, AnonymousStructPlan, AnonymousStructSet, ByteExtent, ByteRepresentation,
    ByteSource, ByteView, DefinitionGroup, DefinitionKind, DefinitionLocation, DefinitionSelector,
    DefinitionSite, Evidence, EvidenceDetail, ExprSite, NulPosition, PointerMutability, Predicate,
    Proof, QueryResult, Rejection, RejectionReason, StableExpr, ZeroGroupUsers, ZeroUsers,
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
    pub(in crate::fixups) trivial_unsafe_site: Option<ExprSite>,
    pub(in crate::fixups) target: CallTarget,
    pub(in crate::fixups) args: Vec<ExprSite>,
    pub(in crate::fixups) evidence: Vec<Evidence>,
}

pub(in crate::fixups) struct QueryContext<'snapshot> {
    program: &'snapshot Program,
    facts: &'snapshot FixupFacts,
    calls: BTreeMap<(CallTarget, usize), Vec<CallRecord>>,
    definitions: BTreeMap<DefinitionSelector, Vec<DefinitionSite>>,
    symbol_uses: BTreeMap<String, Vec<usize>>,
    use_domain_complete: bool,
}

impl<'snapshot> QueryContext<'snapshot> {
    pub(in crate::fixups) fn new(
        program: &'snapshot Program,
        facts: &'snapshot FixupFacts,
    ) -> Self {
        let mut calls: BTreeMap<(CallTarget, usize), Vec<CallRecord>> = BTreeMap::new();
        let mut definitions = BTreeMap::<DefinitionSelector, Vec<DefinitionSite>>::new();
        let mut symbol_uses = BTreeMap::<String, Vec<usize>>::new();
        let mut use_domain_complete = true;
        for (item_index, item) in program.items.iter().enumerate() {
            index_definitions(item, item_index, &mut definitions);
            use_domain_complete &= item_use_domain_complete(item);
            index_item_uses(item, item_index, &mut symbol_uses);
            if let Item::Fn(function) = item {
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
                            trivial_unsafe_site: trivial_unsafe_site(program, &site),
                            site,
                            target,
                            args: arg_sites,
                            evidence,
                        });
                });
            }
        }
        Self {
            program,
            facts,
            calls,
            definitions,
            symbol_uses,
            use_domain_complete,
        }
    }

    pub(in crate::fixups) fn calls(&self, target: &CallTarget, arity: usize) -> &[CallRecord] {
        self.calls
            .get(&(target.clone(), arity))
            .map(Vec::as_slice)
            .unwrap_or_default()
    }

    pub(in crate::fixups) fn has_anonymous_structs(&self) -> bool {
        !self.facts.anonymous_structs.is_empty()
    }

    pub(in crate::fixups) fn anonymous_structs(&self) -> QueryResult<AnonymousStructSet> {
        let records = self
            .program
            .items
            .iter()
            .enumerate()
            .filter_map(|(item_index, item)| match item {
                Item::Record(record) if !record.is_union && record.name.starts_with("anon_") => {
                    Some((item_index, record))
                }
                _ => None,
            })
            .collect::<Vec<_>>();
        let generated = self
            .facts
            .anonymous_structs
            .iter()
            .map(|fact| fact.generated_name.as_str())
            .collect::<BTreeSet<_>>();
        let originals = self
            .facts
            .anonymous_structs
            .iter()
            .map(|fact| fact.original_name.as_str())
            .collect::<BTreeSet<_>>();
        let record_names = records
            .iter()
            .map(|(_, record)| record.name.as_str())
            .collect::<BTreeSet<_>>();
        let occupied = self
            .program
            .items
            .iter()
            .filter_map(item_type_name)
            .collect::<BTreeSet<_>>();
        let conflicts = generated.intersection(&occupied).count()
            + self
                .facts
                .anonymous_structs
                .len()
                .saturating_sub(generated.len())
            + self
                .facts
                .anonymous_structs
                .len()
                .saturating_sub(originals.len())
            + records.len().saturating_sub(record_names.len());
        let complete = !records.is_empty()
            && records.len() == self.facts.anonymous_structs.len()
            && conflicts == 0
            && self.facts.anonymous_structs.iter().all(|fact| {
                records.iter().any(|(_, record)| {
                    record.name == fact.original_name
                        && record.fields.len() == fact.fields.len()
                        && record
                            .fields
                            .iter()
                            .zip(&fact.fields)
                            .all(|(field, fact)| field.name.as_str() == fact.name)
                })
            });
        let site = ExprSite {
            item_index: records.first().map(|(index, _)| *index).unwrap_or(0),
            path: AstPath(Vec::new()),
            fact_path: AstPath(Vec::new()),
        };
        let evidence = vec![Evidence {
            predicate: Predicate::AnonymousStructDomain,
            site: site.clone(),
            detail: EvidenceDetail::AnonymousStructDomain {
                records: records.len(),
                facts: self.facts.anonymous_structs.len(),
                conflicts,
                complete,
            },
        }];
        if !complete {
            return Err(Rejection::new(
                Predicate::AnonymousStructDomain,
                Some(site),
                RejectionReason::IncompleteDomain,
                evidence,
            ));
        }
        let structs = self
            .facts
            .anonymous_structs
            .iter()
            .map(|fact| {
                let item_index = records
                    .iter()
                    .find_map(|(item_index, record)| {
                        (record.name == fact.original_name).then_some(*item_index)
                    })
                    .unwrap();
                AnonymousStructPlan {
                    item_index,
                    original_name: fact.original_name.clone(),
                    generated_name: fact.generated_name.clone(),
                    fields: fact
                        .fields
                        .iter()
                        .map(|field| AnonymousStructField {
                            name: field.name.clone(),
                            ty: field.ty.clone(),
                        })
                        .collect(),
                }
            })
            .collect();
        Ok(Proof::new(AnonymousStructSet { structs }, evidence))
    }

    pub(super) fn snapshot_program(&self) -> &'snapshot Program {
        self.program
    }

    pub(super) fn snapshot_facts(&self) -> &'snapshot FixupFacts {
        self.facts
    }

    pub(in crate::fixups) fn never_returning_extern(&self, call: &CallRecord) -> QueryResult<()> {
        let predicate = Predicate::NeverReturningExtern;
        let CallTarget::Known(target) = call.target else {
            return Err(Rejection::new(
                predicate,
                Some(call.site.clone()),
                RejectionReason::UnsupportedShape,
                Vec::new(),
            ));
        };
        let mut saw_declaration = false;
        let mut evidence = Vec::new();
        for item in &self.program.items {
            let Item::ExternBlock { decls, .. } = item else {
                continue;
            };
            for decl in decls {
                let ExternDecl::Fn(function) = decl else {
                    continue;
                };
                if known_declaration(function.identity, &function.name) != Some(target) {
                    continue;
                }
                saw_declaration = true;
                let returns_never = matches!(function.ret, Some(Type::Never));
                evidence.push(Evidence {
                    predicate,
                    site: call.site.clone(),
                    detail: EvidenceDetail::KnownExternDeclaration {
                        target,
                        arity: function.params.len(),
                        returns_never,
                    },
                });
                if function.params.len() == call.args.len() && returns_never {
                    return Ok(Proof::new((), evidence));
                }
            }
        }
        Err(Rejection::new(
            predicate,
            Some(call.site.clone()),
            if saw_declaration {
                RejectionReason::Contradicted
            } else {
                RejectionReason::MissingEvidence
            },
            evidence,
        ))
    }

    pub(in crate::fixups) fn expr(&self, site: &ExprSite) -> Option<&'snapshot Expr> {
        walk::target_expr_at_path(self.program, site.item_index, &site.path)
    }

    pub(in crate::fixups) fn definitions(
        &self,
        selector: &DefinitionSelector,
    ) -> &[DefinitionSite] {
        self.definitions
            .get(selector)
            .map(Vec::as_slice)
            .unwrap_or_default()
    }

    pub(in crate::fixups) fn definitions_in_group(
        &self,
        group: &DefinitionGroup,
    ) -> Vec<DefinitionSite> {
        self.definitions
            .values()
            .flatten()
            .filter(|definition| definition.group.as_ref() == Some(group))
            .cloned()
            .collect()
    }

    pub(in crate::fixups) fn definitions_of_kind(
        &self,
        kind: DefinitionKind,
    ) -> Vec<DefinitionSite> {
        self.definitions
            .values()
            .flatten()
            .filter(|definition| definition.kind == kind)
            .cloned()
            .collect()
    }

    pub(in crate::fixups) fn zero_users(
        &self,
        definition: &DefinitionSite,
    ) -> QueryResult<ZeroUsers> {
        let users = self.definition_users(
            definition,
            &BTreeSet::from([definition.location.item_index()]),
        );
        let complete = self.use_domain_complete && !definition.externally_reachable;
        let evidence = vec![Evidence {
            predicate: Predicate::ZeroUsers,
            site: definition_evidence_site(definition),
            detail: EvidenceDetail::UseDomain {
                name: definition.name.clone(),
                users,
                complete,
            },
        }];
        if !complete {
            return Err(Rejection::new(
                Predicate::ZeroUsers,
                Some(definition_evidence_site(definition)),
                RejectionReason::IncompleteDomain,
                evidence,
            ));
        }
        if users != 0 {
            return Err(Rejection::new(
                Predicate::ZeroUsers,
                Some(definition_evidence_site(definition)),
                RejectionReason::Contradicted,
                evidence,
            ));
        }
        Ok(Proof::new(
            ZeroUsers {
                definition: definition.clone(),
            },
            evidence,
        ))
    }

    pub(in crate::fixups) fn zero_group_users(
        &self,
        group: &DefinitionGroup,
    ) -> QueryResult<ZeroGroupUsers> {
        let definitions = self.definitions_in_group(group);
        let definition_items = definitions
            .iter()
            .map(|definition| definition.location.item_index())
            .collect::<BTreeSet<_>>();
        let users = definitions
            .iter()
            .map(|definition| self.definition_users(definition, &definition_items))
            .sum();
        let complete = !definitions.is_empty()
            && self.use_domain_complete
            && definitions
                .iter()
                .all(|definition| !definition.externally_reachable);
        let site = definitions
            .first()
            .map(definition_evidence_site)
            .unwrap_or_else(|| ExprSite {
                item_index: 0,
                path: AstPath(Vec::new()),
                fact_path: AstPath(Vec::new()),
            });
        let evidence = vec![Evidence {
            predicate: Predicate::ZeroGroupUsers,
            site: site.clone(),
            detail: EvidenceDetail::GroupUseDomain {
                group: group.clone(),
                definitions: definitions.len(),
                users,
                complete,
            },
        }];
        if !complete {
            return Err(Rejection::new(
                Predicate::ZeroGroupUsers,
                Some(site),
                RejectionReason::IncompleteDomain,
                evidence,
            ));
        }
        if users != 0 {
            return Err(Rejection::new(
                Predicate::ZeroGroupUsers,
                Some(site),
                RejectionReason::Contradicted,
                evidence,
            ));
        }
        Ok(Proof::new(
            ZeroGroupUsers {
                group: group.clone(),
            },
            evidence,
        ))
    }

    fn definition_users(
        &self,
        definition: &DefinitionSite,
        definition_items: &BTreeSet<usize>,
    ) -> usize {
        definition
            .symbols
            .iter()
            .map(|symbol| {
                self.symbol_uses
                    .get(symbol)
                    .map(|uses| {
                        uses.iter()
                            .filter(|item_index| !definition_items.contains(item_index))
                            .count()
                    })
                    .unwrap_or(0)
            })
            .sum()
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
        let (shape_name, shape_mutable) = self
            .expr(site)
            .and_then(stable_pointer_source)
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
        if shape_name != name || shape_mutable != pointer.mutable {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::Contradicted,
                Vec::new(),
            ));
        }
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
        let constant = self.const_usize(count);
        if let (ByteExtent::Constant(extent), Ok(count_proof)) = (source.extent, &constant) {
            if extent != count_proof.value {
                return Err(Rejection::new(
                    Predicate::FullByteView,
                    Some(count.clone()),
                    RejectionReason::Contradicted,
                    count_proof.evidence.clone(),
                ));
            }
            let mut evidence = count_proof.evidence.clone();
            evidence.extend(self.pure(count)?.evidence);
            evidence.push(Evidence {
                predicate: Predicate::FullByteView,
                site: source.site.clone(),
                detail: EvidenceDetail::Extent(source.extent),
            });
            return Ok(Proof::new(
                ByteView {
                    source: source.clone(),
                    extent: source.extent,
                },
                evidence,
            ));
        }
        if self.count_matches_source_len(source, count) {
            return Ok(Proof::new(
                ByteView {
                    source: source.clone(),
                    extent: source.extent,
                },
                vec![Evidence {
                    predicate: Predicate::FullByteView,
                    site: count.clone(),
                    detail: EvidenceDetail::SourceLength,
                }],
            ));
        }
        let evidence = constant
            .map(|proof| proof.evidence)
            .unwrap_or_else(|rejection| rejection.evidence);
        Err(Rejection::new(
            Predicate::FullByteView,
            Some(count.clone()),
            RejectionReason::MissingEvidence,
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
        let count_stable = self.pure(count)?;
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
        evidence.extend(count_stable.evidence);
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

    fn count_matches_source_len(&self, source: &ByteSource<'snapshot>, count: &ExprSite) -> bool {
        let Some(count) = self.expr(count) else {
            return false;
        };
        count_matches_source_len(source, count)
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

fn index_definitions(
    item: &Item,
    item_index: usize,
    definitions: &mut BTreeMap<DefinitionSelector, Vec<DefinitionSite>>,
) {
    match item {
        Item::Fn(function) => {
            let site = DefinitionSite {
                location: DefinitionLocation::Item(item_index),
                kind: DefinitionKind::Function,
                name: function.name.clone(),
                symbols: vec![function.name.clone()],
                group: None,
                externally_reachable: function.vis == Visibility::Pub
                    || function.abi.is_some()
                    || function.attrs.iter().any(exporting_attr),
            };
            definitions
                .entry(DefinitionSelector {
                    kind: site.kind,
                    name: site.name.clone(),
                })
                .or_default()
                .push(site);
        }
        Item::ExternBlock { decls, .. } => {
            for (decl_index, decl) in decls.iter().enumerate() {
                let (kind, name, group) = match decl {
                    ExternDecl::Fn(function) => (
                        DefinitionKind::ExternFunction,
                        function.name.clone(),
                        match function.identity {
                            FunctionIdentity::Known(known) => {
                                Some(DefinitionGroup::Header(known.header().into()))
                            }
                            FunctionIdentity::Unknown => None,
                        },
                    ),
                    ExternDecl::Static { name, .. } => {
                        (DefinitionKind::ExternStatic, name.clone(), None)
                    }
                };
                let site = DefinitionSite {
                    location: DefinitionLocation::ExternDecl {
                        item_index,
                        decl_index,
                    },
                    kind,
                    symbols: vec![name.clone()],
                    name,
                    group,
                    externally_reachable: false,
                };
                definitions
                    .entry(DefinitionSelector {
                        kind: site.kind,
                        name: site.name.clone(),
                    })
                    .or_default()
                    .push(site);
            }
        }
        Item::SupportModule(module) => {
            let name = module.name.as_str().to_owned();
            let site = DefinitionSite {
                location: DefinitionLocation::Item(item_index),
                kind: DefinitionKind::SupportModule,
                name: name.clone(),
                symbols: module.exports.iter().map(qualified_path).collect(),
                group: Some(DefinitionGroup::SupportModule(name)),
                externally_reachable: false,
            };
            definitions
                .entry(DefinitionSelector {
                    kind: site.kind,
                    name: site.name.clone(),
                })
                .or_default()
                .push(site);
        }
        _ => {}
    }
}

fn trivial_unsafe_site(program: &Program, call: &ExprSite) -> Option<ExprSite> {
    let path = &call.path.0;
    if !path.ends_with(&[PathSegment::BlockBody, PathSegment::BlockTail]) {
        return None;
    }
    let parent = expression_site(call.item_index, &path[..path.len() - 2]);
    let Expr::Unsafe(block) = walk::target_expr_at_path(program, parent.item_index, &parent.path)?
    else {
        return None;
    };
    (block.stmts.is_empty() && block.tail.is_some()).then_some(parent)
}

fn exporting_attr(attr: &Attr) -> bool {
    matches!(
        attr,
        Attr::NoMangle
            | Attr::WeakLinkage
            | Attr::ExternWeakLinkage
            | Attr::Used(_)
            | Attr::LinkSection(_)
    )
}

fn index_item_uses(item: &Item, item_index: usize, uses: &mut BTreeMap<String, Vec<usize>>) {
    let mut index_expr = |expr: &Expr| index_expr_use(expr, item_index, uses);
    match item {
        Item::Fn(function) => walk::body_exprs(&function.body, &mut index_expr),
        Item::Static { init, .. } | Item::Const { init, .. } => walk::exprs(init, &mut index_expr),
        Item::Impl(block) => {
            for item in &block.items {
                if let ImplItem::Method(method) = item {
                    walk::exprs(&method.body, &mut index_expr);
                }
            }
        }
        Item::Macro { args, .. } => {
            for arg in args {
                walk::exprs(arg, &mut index_expr);
            }
        }
        Item::Cfg { item, .. } => index_item_uses(item, item_index, uses),
        Item::Use { path } => {
            for segment in &path.segments {
                uses.entry(segment.as_str().to_owned())
                    .or_default()
                    .push(item_index);
            }
        }
        Item::Comment(_)
        | Item::CrateAttrs(_)
        | Item::Mod { .. }
        | Item::ExternBlock { .. }
        | Item::Enum(_)
        | Item::Record(_)
        | Item::Struct(_)
        | Item::SupportModule(_) => {}
    }
}

fn item_use_domain_complete(item: &Item) -> bool {
    match item {
        Item::Cfg { item, .. } => item_use_domain_complete(item),
        _ => true,
    }
}

fn index_expr_use(expr: &Expr, item_index: usize, uses: &mut BTreeMap<String, Vec<usize>>) {
    match expr {
        Expr::Var(name) => uses
            .entry(name.as_str().to_owned())
            .or_default()
            .push(item_index),
        Expr::Path(path) => uses
            .entry(qualified_path(path))
            .or_default()
            .push(item_index),
        _ => {}
    }
}

fn qualified_path(path: &crate::rust_ast::Path) -> String {
    path.segments
        .iter()
        .map(crate::rust_ast::Ident::as_str)
        .collect::<Vec<_>>()
        .join("::")
}

fn definition_evidence_site(definition: &DefinitionSite) -> ExprSite {
    ExprSite {
        item_index: definition.location.item_index(),
        path: AstPath(Vec::new()),
        fact_path: AstPath(Vec::new()),
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

fn stable_pointer_source(expr: &Expr) -> Option<(&str, bool)> {
    match expr {
        Expr::Cast { expr, .. } => stable_pointer_source(expr),
        Expr::ArrayPtr { array, mutable } => {
            let Expr::Var(name) = &**array else {
                return None;
            };
            Some((name.as_str(), *mutable))
        }
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            let Expr::Var(name) = &**recv else {
                return None;
            };
            Some((name.as_str(), method == "as_mut_ptr"))
        }
        _ => None,
    }
}

fn count_matches_source_len(source: &ByteSource<'_>, count: &Expr) -> bool {
    match count {
        Expr::Cast { expr, .. } => count_matches_source_len(source, expr),
        Expr::MethodCall { recv, method, args } if args.is_empty() && method == "len" => {
            matches_source_expr(source, recv)
        }
        _ => false,
    }
}

fn matches_source_expr(source: &ByteSource<'_>, expr: &Expr) -> bool {
    match expr {
        Expr::Var(name) => name.as_str() == source.name,
        Expr::MethodCall { recv, method, args }
            if args.is_empty()
                && method == "as_bytes"
                && source.representation == ByteRepresentation::Str =>
        {
            matches_source_expr(source, recv)
        }
        _ => false,
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

fn item_type_name(item: &Item) -> Option<&str> {
    match item {
        Item::Record(record) => Some(record.name.as_str()),
        Item::Struct(record) => Some(record.name.as_str()),
        Item::Enum(record) => Some(record.name.as_str()),
        Item::Cfg { item, .. } => item_type_name(item),
        _ => None,
    }
}
