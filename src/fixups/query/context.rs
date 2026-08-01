use std::cell::RefCell;
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::marker::PhantomData;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingId, BindingKind, ConstValue, CountedLoopFact, EffectKind, EffectSubject,
    FixupFacts, FunctionId, HeapExtent, HeapOwnershipFact, HeapOwnershipKind, HeapReadSafety,
    NulTermination, PathSegment, PtrLenSliceFact, Purity, StringBufferFact, StringBufferKind,
    StringRecoveryCandidate, ValueSubject,
};
use crate::fixups::idents::expr_ident_count;
use crate::fixups::support::walk as support_walk;
use crate::function_identity::{CallBinding, FunctionIdentity, Known};
use crate::rust_ast::{
    Attr, Expr, ExternDecl, ImplItem, IndentStmt, Item, Prim, Program, RustValue, Stmt, Type,
    Visibility,
};

use super::{
    AnonymousStructField, AnonymousStructPlan, AnonymousStructSet, ArrayElementPointerOrigin,
    ByteExtent, ByteRepresentation, ByteSource, ByteView, DefinitionGroup, DefinitionKind,
    DefinitionLocation, DefinitionSelector, DefinitionSite, Evidence, EvidenceDetail, ExprSite,
    ExternFn, HeapOwnershipPlan, HeapOwnershipPlanSet, HeapOwnershipReallocPlan, InlineTempPlan,
    LazySingletonPlan, LazySingletonSet, NulPosition, NullaryMethodCall, Phase, PointerMutability,
    Predicate, Proof, PtrLenPlan, PtrLenPlanSet, QueryResult, Rejection, RejectionReason,
    ResolvedValue, StableExpr, StmtWindowSite, Usage, ValueSite, ZeroGroupUsers, ZeroUsers,
};

macro_rules! query_cache {
    ($(
        fn $name:ident(& $slf:tt $(, $arg:ident : $arg_ty:ty)*) -> QueryResult<$ret:ty>;
        key: $key_ty:ty = $key:expr;
        $body:block
    )*) => {
        #[derive(Default)]
        struct QueryCache<'snapshot> {
            $($name: RefCell<HashMap<$key_ty, QueryResult<$ret>>>,)*
        }

        impl<'snapshot> QueryContext<'snapshot> {
            $(
                pub(in crate::fixups) fn $name(&$slf, $($arg: $arg_ty),*) -> QueryResult<$ret> {
                    cached(&$slf.cache.$name, $key, || $body)
                }
            )*
        }
    };
}

fn cached<K, V>(cache: &RefCell<HashMap<K, V>>, key: K, compute: impl FnOnce() -> V) -> V
where
    K: Eq + std::hash::Hash + Clone,
    V: Clone,
{
    if let Some(hit) = cache.borrow().get(&key) {
        return hit.clone();
    }
    let value = compute();
    cache.borrow_mut().insert(key, value.clone());
    value
}

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
    cache: QueryCache<'snapshot>,
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
            cache: QueryCache::default(),
        }
    }

    pub(in crate::fixups) fn all_calls(&self) -> impl Iterator<Item = &CallRecord> {
        self.calls.values().flatten()
    }

    pub(in crate::fixups) fn call_arg_types(&self, call: &CallRecord) -> Vec<Option<Type>> {
        let Some(function) = self.facts.function_by_item_index(call.site.item_index) else {
            return vec![None; call.args.len()];
        };
        call.args
            .iter()
            .map(|arg| {
                self.facts
                    .call_arg_at(function, &arg.path)
                    .and_then(|(_, arg_fact)| arg_fact.declared_ty.clone())
            })
            .collect()
    }

    pub(in crate::fixups) fn local_value(
        &self,
        window: &StmtWindowSite,
        name: &str,
    ) -> ResolvedValue {
        let mut def_path = window.path.0.clone();
        def_path.push(PathSegment::Stmt(window.start));
        self.resolved_value_at(window.item_index, &AstPath(def_path), name)
    }

    pub(in crate::fixups) fn value_local(&self, site: &ValueSite, name: &str) -> ResolvedValue {
        self.resolved_value_at(site.item_index, &site.path, name)
    }

    fn resolved_value_at(
        &self,
        item_index: usize,
        def_path: &AstPath,
        name: &str,
    ) -> ResolvedValue {
        let Some(function) = self.facts.function_by_item_index(item_index) else {
            return ResolvedValue {
                ty: None,
                usage: None,
                purity: None,
            };
        };
        let binding = self.facts.binding_by_local_path(function, name, def_path);
        let ty = binding.and_then(|binding| self.facts.binding_type_ast(binding).cloned());
        let usage = binding
            .and_then(|binding| self.facts.def_use(binding))
            .map(|uses| Usage {
                reads: uses.reads.len(),
                writes: uses.writes.len(),
            });
        let purity = self
            .facts
            .effect(function, EffectSubject::Expr, def_path)
            .map(|effect| effect.purity);
        ResolvedValue { ty, usage, purity }
    }

    pub(in crate::fixups) fn extern_fn(&self, matcher: &ExternFn) -> QueryResult<()> {
        let predicate = Predicate::ExternFn;
        let mut saw_name = false;
        for (item_index, item) in self.program.items.iter().enumerate() {
            let Item::ExternBlock { decls, .. } = item else {
                continue;
            };
            for decl in decls {
                let ExternDecl::Fn(function) = decl else {
                    continue;
                };
                if !matcher.name.matches(&function.name, &()) {
                    continue;
                }
                saw_name = true;
                let arity = function.params.len();
                let ret = function.ret.clone();
                if !matcher.arity.matches(&arity, &()) || !matcher.returns.matches(&ret, &()) {
                    continue;
                }
                let site = expression_site(item_index, &[]);
                return Ok(Proof::new(
                    (),
                    vec![Evidence {
                        predicate,
                        site,
                        detail: EvidenceDetail::ExternFnDeclaration {
                            name: function.name.clone(),
                            arity,
                            returns_never: matches!(ret, Some(Type::Never)),
                        },
                    }],
                ));
            }
        }
        Err(Rejection::new(
            predicate,
            None,
            if saw_name {
                RejectionReason::Contradicted
            } else {
                RejectionReason::MissingEvidence
            },
            Vec::new(),
        ))
    }

    pub(in crate::fixups) fn all_exprs(&self, item_index: usize) -> QueryResult<Vec<ExprSite>> {
        let predicate = Predicate::AllExprs;
        let evidence_site = expression_site(item_index, &[]);
        let Some(Item::Fn(function)) = self.program.items.get(item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let mut sites = Vec::new();
        walk::body_exprs_with_path(&function.body, &mut Vec::new(), &mut |_, path| {
            sites.push(expression_site(item_index, path));
        });
        let evidence = vec![Evidence {
            predicate,
            site: evidence_site,
            detail: EvidenceDetail::AllExprs { count: sites.len() },
        }];
        Ok(Proof::new(sites, evidence))
    }

    pub(in crate::fixups) fn string_buffer(
        &self,
        site: &ValueSite,
    ) -> QueryResult<StringBufferFact> {
        let predicate = Predicate::StringBuffer;
        let evidence_site = expression_site(site.item_index, &site.path.0);
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(buffer) = self.facts.string_buffer_at(function, &site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site: evidence_site,
            detail: EvidenceDetail::StringBuffer {
                bytes: buffer.bytes.as_ref().map_or(0, Vec::len),
            },
        }];
        Ok(Proof::new(buffer.clone(), evidence))
    }

    pub(in crate::fixups) fn value_uses(
        &self,
        site: &ValueSite,
        name: &str,
    ) -> QueryResult<Vec<ExprSite>> {
        let predicate = Predicate::ReadPath;
        let evidence_site = expression_site(site.item_index, &site.path.0);
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(binding) = self.facts.binding_by_local_path(function, name, &site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let sites = self
            .facts
            .def_use(binding)
            .map(|uses| {
                uses.reads
                    .iter()
                    .chain(uses.writes.iter())
                    .map(|path| expression_site(site.item_index, &path.0))
                    .collect()
            })
            .unwrap_or_default();
        let evidence = vec![Evidence {
            predicate,
            site: evidence_site,
            detail: EvidenceDetail::Binding {
                name: name.to_string(),
            },
        }];
        Ok(Proof::new(sites, evidence))
    }

    pub(in crate::fixups) fn string_pointer_view_sites(
        &self,
        site: &ValueSite,
        name: &str,
    ) -> QueryResult<Vec<ExprSite>> {
        let predicate = Predicate::ReadPath;
        let evidence_site = expression_site(site.item_index, &site.path.0);
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(binding) = self.facts.binding_by_local_path(function, name, &site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let sites = self
            .facts
            .string_pointer_views
            .iter()
            .filter(|view| view.site.function == function && view.source == binding)
            .map(|view| expression_site(site.item_index, &view.site.path.0))
            .collect();
        let evidence = vec![Evidence {
            predicate,
            site: evidence_site,
            detail: EvidenceDetail::Binding {
                name: name.to_string(),
            },
        }];
        Ok(Proof::new(sites, evidence))
    }

    pub(in crate::fixups) fn string_use_allows_lift(
        &self,
        site: &ValueSite,
        name: &str,
        use_site: &ExprSite,
        recovery: StringRecoveryCandidate,
    ) -> QueryResult<bool> {
        let predicate = Predicate::StringUse;
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(use_site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(binding) = self.facts.binding_by_local_path(function, name, &site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(use_site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let liftable = self.facts.liftable_string_bindings(function, recovery);
        let allowed =
            self.facts
                .string_use_allowed(function, &use_site.path, binding, recovery, &liftable);
        let evidence = vec![Evidence {
            predicate,
            site: use_site.clone(),
            detail: EvidenceDetail::StringUse { allowed },
        }];
        Ok(Proof::new(allowed, evidence))
    }

    pub(in crate::fixups) fn pointer_origin(
        &self,
        site: &ValueSite,
        name: &str,
    ) -> QueryResult<ArrayElementPointerOrigin> {
        let predicate = Predicate::ArrayElementPointerOrigin;
        let evidence_site = expression_site(site.item_index, &site.path.0);
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let mut origins = self
            .facts
            .array_element_pointer_origins
            .iter()
            .filter(|fact| fact.site.function == function)
            .filter_map(|fact| {
                Some((
                    self.facts.binding_name(fact.pointer)?.to_string(),
                    ArrayElementPointerOrigin {
                        pointer_name: self.facts.binding_name(fact.pointer)?.to_string(),
                        base_name: self.facts.binding_name(fact.base)?.to_string(),
                        index: fact.index.clone(),
                    },
                ))
            })
            .collect::<BTreeMap<_, _>>();
        let Item::Fn(function_item) = &self.program.items[site.item_index] else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        collect_array_element_pointer_aliases(&function_item.body, &mut origins);
        let Some(origin) = origins.remove(name) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site: evidence_site,
            detail: EvidenceDetail::ArrayElementPointerOrigin { origins: 1 },
        }];
        Ok(Proof::new(origin, evidence))
    }

    pub(in crate::fixups) fn has_anonymous_structs(&self) -> bool {
        !self.facts.anonymous_structs.is_empty()
    }

    pub(in crate::fixups) fn has_lazy_singletons(&self) -> bool {
        !self.facts.lazy_init_singletons.is_empty()
    }

    pub(in crate::fixups) fn has_ptr_len_slices(&self) -> bool {
        !self.facts.ptr_len_slices.is_empty()
    }

    pub(super) fn snapshot_program(&self) -> &'snapshot Program {
        self.program
    }

    pub(super) fn snapshot_facts(&self) -> &'snapshot FixupFacts {
        self.facts
    }

    pub(in crate::fixups) fn expr(&self, site: &ExprSite) -> Option<&'snapshot Expr> {
        walk::target_expr_at_path(self.program, site.item_index, &site.path)
    }

    pub(in crate::fixups) fn all_definitions(&self) -> impl Iterator<Item = &DefinitionSite> {
        self.definitions.values().flatten()
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

query_cache! {
    fn anonymous_structs(&self) -> QueryResult<AnonymousStructSet>;
    key: () = ();
    {
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

    fn zero_users(&self, definition: &DefinitionSite) -> QueryResult<ZeroUsers>;
    key: DefinitionLocation = definition.location.clone();
    {
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

    fn zero_group_users(&self, group: &DefinitionGroup) -> QueryResult<ZeroGroupUsers>;
    key: DefinitionGroup = group.clone();
    {
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

    fn byte_source(&self, site: &ExprSite) -> QueryResult<ByteSource<'snapshot>>;
    key: ExprSite = site.clone();
    {
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

    fn const_u8(&self, site: &ExprSite) -> QueryResult<u8>;
    key: ExprSite = site.clone();
    {
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

    fn const_usize(&self, site: &ExprSite) -> QueryResult<usize>;
    key: ExprSite = site.clone();
    {
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

    fn full_byte_view(&self, source: &ByteSource<'snapshot>, count: &ExprSite) -> QueryResult<ByteView<'snapshot>>;
    key: (ExprSite, ExprSite) = (source.site.clone(), count.clone());
    {
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

    fn first_nul(&self, source: &ByteSource<'snapshot>) -> QueryResult<NulPosition>;
    key: BindingId = source.binding;
    {
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

    fn prefix_contains(&self, count: &ExprSite, nul: NulPosition) -> QueryResult<()>;
    key: (ExprSite, NulPosition) = (count.clone(), nul);
    {
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

    fn pure(&self, site: &ExprSite) -> QueryResult<StableExpr>;
    key: ExprSite = site.clone();
    {
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

    fn counted_loop(&self, window: &StmtWindowSite) -> QueryResult<CountedLoopFact>;
    key: StmtWindowSite = window.clone();
    {
        let predicate = Predicate::CountedLoop;
        let evidence_site = stmt_window_evidence_site(window, window.start);
        let function = self.facts.function_by_item_index(window.item_index).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(evidence_site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let mut loop_path = window.path.0.clone();
        loop_path.push(PathSegment::Stmt(window.start + 1));
        let loop_path = AstPath(loop_path);
        let Some(fact) = self
            .facts
            .counted_loops
            .iter()
            .find(|fact| fact.site.function == function && fact.site.loop_path == loop_path)
        else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        Ok(Proof::new(
            fact.clone(),
            vec![Evidence {
                predicate,
                site: evidence_site,
                detail: EvidenceDetail::CountedLoop {
                    start: fact.start,
                    step: fact.step,
                    index_use: fact.index_use,
                },
            }],
        ))
    }

    fn read_path(&self, window: &StmtWindowSite, name: &str) -> QueryResult<AstPath>;
    key: (StmtWindowSite, String) = (window.clone(), name.to_string());
    {
        let predicate = Predicate::ReadPath;
        let evidence_site = stmt_window_evidence_site(window, window.start);
        let function = self.facts.function_by_item_index(window.item_index).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(evidence_site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let mut def_path = window.path.0.clone();
        def_path.push(PathSegment::Stmt(window.start));
        let def_path = AstPath(def_path);
        let Some(binding) = self.facts.binding_by_local_path(function, name, &def_path) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(uses) = self.facts.def_use(binding) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let [read] = uses.reads.as_slice() else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::Ambiguous,
                Vec::new(),
            ));
        };
        Ok(Proof::new(
            read.clone(),
            vec![Evidence {
                predicate,
                site: evidence_site,
                detail: EvidenceDetail::Binding {
                    name: name.to_string(),
                },
            }],
        ))
    }

    fn no_effects(&self, window: &StmtWindowSite) -> QueryResult<()>;
    key: StmtWindowSite = window.clone();
    {
        let predicate = Predicate::NoEffects;
        let evidence_site = stmt_window_evidence_site(window, window.start);
        let function = self.facts.function_by_item_index(window.item_index).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(evidence_site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let mut def_path = window.path.0.clone();
        def_path.push(PathSegment::Stmt(window.start));
        let def_path = AstPath(def_path);
        let Some(effect) = self.facts.effect(function, EffectSubject::Expr, &def_path) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        if !effect.effects.is_empty() {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::Contradicted,
                Vec::new(),
            ));
        }
        Ok(Proof::new(
            (),
            vec![Evidence {
                predicate,
                site: evidence_site,
                detail: EvidenceDetail::NoEffects,
            }],
        ))
    }

    fn lazy_singletons(&self) -> QueryResult<LazySingletonSet>;
    key: () = ();
    {
        let predicate = Predicate::LazySingletonDomain;
        let mut singletons = Vec::new();
        for singleton in &self.facts.lazy_init_singletons {
            let Some(function_item_index) = self.facts.function_item_index(singleton.function)
            else {
                return Err(Rejection::new(
                    predicate,
                    None,
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            let Some(function_name) = self.facts.function_name(singleton.function) else {
                return Err(Rejection::new(
                    predicate,
                    None,
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            if !matches!(
                self.program.items.get(function_item_index),
                Some(Item::Fn(f)) if f.name == function_name
            ) {
                return Err(Rejection::new(
                    predicate,
                    None,
                    RejectionReason::Contradicted,
                    Vec::new(),
                ));
            }
            let Some(payload_item_index) = static_item_index(self.program, &singleton.payload_name)
            else {
                return Err(Rejection::new(
                    predicate,
                    None,
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            let Some(flag_item_index) = static_item_index(self.program, &singleton.flag_name)
            else {
                return Err(Rejection::new(
                    predicate,
                    None,
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            singletons.push(LazySingletonPlan {
                function_item_index,
                function_name: function_name.to_string(),
                payload_item_index,
                payload_name: singleton.payload_name.clone(),
                payload_ty: singleton.payload_ty.clone(),
                init_expr: singleton.init_expr.clone(),
                flag_item_index,
                flag_name: singleton.flag_name.clone(),
            });
        }
        let site = expression_site(
            singletons.first().map_or(0, |plan| plan.function_item_index),
            &[],
        );
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::LazySingletonDomain {
                singletons: singletons.len(),
            },
        }];
        Ok(Proof::new(LazySingletonSet { singletons }, evidence))
    }

    fn heap_ownership_plans(&self, definition: &DefinitionSite) -> QueryResult<HeapOwnershipPlanSet>;
    key: DefinitionLocation = definition.location.clone();
    {
        let predicate = Predicate::HeapOwnershipPlan;
        let site = definition_evidence_site(definition);
        let Some(function) = self
            .facts
            .function_by_item_index(definition.location.item_index())
        else {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let plans = heap_ownership_plans_for_function(self.facts, function);
        if plans.is_empty() {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::HeapOwnershipPlan { plans: plans.len() },
        }];
        Ok(Proof::new(HeapOwnershipPlanSet { plans }, evidence))
    }

    fn ptr_len_slices(&self) -> QueryResult<PtrLenPlanSet>;
    key: () = ();
    {
        let predicate = Predicate::PtrLenSlice;
        let plans = ptr_len_plans_from_facts(self.facts);
        let site = expression_site(plans.first().map_or(0, |plan| plan.item_index), &[]);
        if plans.is_empty() {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::PtrLenSlice { plans: plans.len() },
        }];
        Ok(Proof::new(PtrLenPlanSet { plans }, evidence))
    }

    fn inline_temp_candidate(
        &self,
        definition: &DefinitionSite,
        phase: Phase
    ) -> QueryResult<InlineTempPlan>;
    key: (DefinitionLocation, Phase) = (definition.location.clone(), phase);
    {
        let predicate = Predicate::InlineTemp;
        let site = definition_evidence_site(definition);
        let Some(function) = self
            .facts
            .function_by_item_index(definition.location.item_index())
        else {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Item::Fn(function_item) = &self.program.items[definition.location.item_index()]
        else {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(plan) = find_inline_temp_candidate(
            &function_item.body,
            &mut Vec::new(),
            function,
            self.facts,
            phase,
        ) else {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::InlineTemp {
                name: plan.name.clone(),
            },
        }];
        Ok(Proof::new(plan, evidence))
    }
}

fn collect_array_element_pointer_aliases(
    body: &[IndentStmt],
    origins: &mut BTreeMap<String, ArrayElementPointerOrigin>,
) {
    let mut changed = true;
    while changed {
        changed = false;
        collect_array_element_pointer_aliases_once(body, origins, &mut changed);
    }
}

fn collect_array_element_pointer_aliases_once(
    body: &[IndentStmt],
    origins: &mut BTreeMap<String, ArrayElementPointerOrigin>,
    changed: &mut bool,
) {
    for indent in body {
        let alias = match &indent.stmt {
            Stmt::Let {
                name,
                init: Some(Expr::Var(source)),
                ..
            } => Some((name.as_str(), source.as_str())),
            Stmt::Assign {
                target: Expr::Var(name),
                value: Expr::Var(source),
            } => Some((name.as_str(), source.as_str())),
            _ => None,
        };
        if let Some((name, source)) = alias
            && let Some(origin) = origins.get(source).cloned()
            && !origins.contains_key(name)
        {
            origins.insert(
                name.to_string(),
                ArrayElementPointerOrigin {
                    pointer_name: name.to_string(),
                    ..origin
                },
            );
            *changed = true;
        }
        walk::nested_body_vecs_with_path(&indent.stmt, &mut Vec::new(), &mut |nested, _| {
            collect_array_element_pointer_aliases_once(nested, origins, changed);
        });
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

fn ptr_len_plans_from_facts(facts: &FixupFacts) -> Vec<PtrLenPlan> {
    let mut grouped = BTreeMap::<(FunctionId, BindingId), Vec<&PtrLenSliceFact>>::new();
    for fact in &facts.ptr_len_slices {
        grouped
            .entry((fact.callee, fact.ptr_param))
            .or_default()
            .push(fact);
    }
    let mut plans = Vec::new();
    for ((function, ptr_param), calls) in grouped {
        let Some(function_fact) = facts.functions.iter().find(|fact| fact.id == function) else {
            continue;
        };
        let Some(ptr_binding) = facts
            .bindings
            .iter()
            .find(|binding| binding.id == ptr_param)
        else {
            continue;
        };
        let BindingKind::Param { index: ptr_index } = ptr_binding.kind else {
            continue;
        };
        let mutable = calls.iter().any(|call| call.mutable);
        plans.push(PtrLenPlan {
            item_index: function_fact.item_index,
            function_name: function_fact.name.clone(),
            ptr_index,
            ptr_name: ptr_binding.name.clone(),
            mutable,
            elem: calls[0].elem_ty.clone(),
        });
    }
    plans
}

fn heap_ownership_plans_for_function(
    facts: &FixupFacts,
    function: FunctionId,
) -> Vec<HeapOwnershipPlan> {
    let mut plans = Vec::new();
    for fact in &facts.heap_ownership {
        if fact.function != function {
            continue;
        }
        let kind = fact.kind;
        if kind == HeapOwnershipKind::VecBuffer
            && fact.read_safety == HeapReadSafety::MayReadUninitialized
        {
            continue;
        }
        let Some(pointer_name) = facts.binding_name(fact.pointer) else {
            continue;
        };
        let Some(init) = init_for_fact(fact) else {
            continue;
        };
        let count = count_for_extent(&fact.extent);
        if kind == HeapOwnershipKind::VecBuffer && count.is_none() {
            continue;
        }
        let reallocs = fact
            .reallocations
            .iter()
            .map(|realloc| {
                Some(HeapOwnershipReallocPlan {
                    source_temp_stmt: previous_stmt_index(&realloc.allocation_path)
                        .and_then(|index| index.checked_sub(1)),
                    size_stmt: previous_stmt_index(&realloc.allocation_path),
                    allocation_stmt: stmt_index(&realloc.allocation_path),
                    assign_stmt: stmt_index(&realloc.assign_path),
                    resize: realloc.resize,
                    count: count_for_extent(&realloc.new_extent)?,
                })
            })
            .collect::<Option<Vec<_>>>();
        let Some(reallocs) = reallocs else {
            continue;
        };
        plans.push(HeapOwnershipPlan {
            pointer_name: pointer_name.to_string(),
            kind,
            pointer_stmt: stmt_index(&fact.pointer_path),
            size_stmt: fact
                .size_temp
                .and_then(|_| previous_stmt_index(&fact.allocation_path)),
            allocation_stmt: stmt_index(&fact.allocation_path),
            assign_stmt: stmt_index(&fact.assign_path),
            free_temp_stmt: fact
                .free_temp
                .and_then(|_| previous_stmt_index(&fact.free_path)),
            free_stmt: stmt_index(&fact.free_path),
            reallocs,
            elem_ty: fact.elem_ty.clone(),
            init,
            count,
        });
    }
    plans
}

fn init_for_fact(fact: &HeapOwnershipFact) -> Option<Expr> {
    match fact.kind {
        HeapOwnershipKind::ScalarBox => Some(default_value(&fact.elem_ty)),
        HeapOwnershipKind::VecBuffer => match fact.read_safety {
            HeapReadSafety::ZeroInitialized | HeapReadSafety::ReadsAfterWrites => {
                Some(default_value(&fact.elem_ty))
            }
            HeapReadSafety::MayReadUninitialized => None,
        },
    }
}

fn count_for_extent(extent: &HeapExtent) -> Option<Expr> {
    match extent {
        HeapExtent::Scalar => Some(Expr::Value(RustValue::I64(1))),
        HeapExtent::Elements { count } => Some(count.clone()),
        HeapExtent::Unknown => None,
    }
}

pub(super) fn default_value(ty: &Type) -> Expr {
    match ty {
        Type::Prim(Prim::Bool) => Expr::Value(RustValue::Bool(false)),
        Type::Prim(Prim::F32 | Prim::F64) => Expr::Value(RustValue::Float(0.0)),
        Type::Prim(Prim::F128) => Expr::HexFloat("0.0f128".into()),
        Type::Ptr { .. } => Expr::Value(RustValue::NullPtr),
        _ => Expr::Value(RustValue::I64(0)),
    }
}

fn stmt_index(path: &AstPath) -> Option<usize> {
    match path.0.as_slice() {
        [PathSegment::Stmt(index)] => Some(*index),
        _ => None,
    }
}

fn previous_stmt_index(path: &AstPath) -> Option<usize> {
    stmt_index(path).and_then(|index| index.checked_sub(1))
}

#[derive(Clone, Copy)]
struct TempCandidate<'a> {
    binding: BindingId,
    init: &'a Expr,
    ty: Option<&'a Type>,
}

#[derive(Clone, Copy)]
struct InlineEnv<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    body_path: &'a [PathSegment],
    phase: Phase,
}

fn find_inline_temp_candidate(
    body: &[IndentStmt],
    path: &mut Vec<PathSegment>,
    function: FunctionId,
    facts: &FixupFacts,
    phase: Phase,
) -> Option<InlineTempPlan> {
    for (index, indent) in body.iter().enumerate() {
        let mut found = None;
        support_walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            support_walk::nested_body_vecs_with_path(&indent.stmt, path, &mut |nested, path| {
                if found.is_none() {
                    found = find_inline_temp_candidate(nested, path, function, facts, phase);
                }
            });
        });
        if found.is_some() {
            return found;
        }
    }
    for (index, indent) in body.iter().enumerate() {
        let mut def_path = path.clone();
        def_path.push(PathSegment::Stmt(index));
        let Stmt::Let {
            name,
            mutable: false,
            init: Some(init),
            ty,
            ..
        } = &indent.stmt
        else {
            continue;
        };
        if !is_temp_name(name) {
            continue;
        }
        let Some(binding) = facts.binding_by_local_path(function, name, &AstPath(def_path.clone()))
        else {
            continue;
        };
        if let Some(plan) = eligible_inline_temp(
            body,
            index,
            TempCandidate {
                binding,
                init,
                ty: ty.as_ref(),
            },
            InlineEnv {
                function,
                facts,
                body_path: path,
                phase,
            },
            name,
        ) {
            return Some(plan);
        }
    }
    None
}

fn eligible_inline_temp(
    body: &[IndentStmt],
    def_index: usize,
    temp: TempCandidate<'_>,
    env: InlineEnv<'_>,
    name: &str,
) -> Option<InlineTempPlan> {
    let fact = env.facts.temp_chains.iter().find(|fact| {
        fact.function == env.function
            && fact.binding == temp.binding
            && fact.producer_path == AstPath(inline_stmt_path(env.body_path, def_index))
    })?;
    let use_index = direct_stmt_index(env.body_path, &fact.consumer_path)?;
    if use_index <= def_index || use_index >= body.len() {
        return None;
    }
    let use_path = inline_stmt_path(env.body_path, use_index);
    let allowed_receiver = is_option_receiver_use(&body[use_index].stmt, name, temp.ty);
    let producer_path = inline_stmt_path(env.body_path, def_index);
    if env.phase == Phase::Early
        && is_effectful_expr(env.function, env.facts, &producer_path)
        && !early_effectful_consumer(&body[use_index].stmt, name)
    {
        return None;
    }
    let allowed_arg = is_allowed_argument_use(ArgumentUse {
        stmt: &body[use_index].stmt,
        name,
        init: temp.init,
        ty: temp.ty,
        producer_path: &producer_path,
        adjacent: use_index == def_index + 1,
        env,
    });
    if (stmt_contains_call(env.function, env.facts, &use_path) && !allowed_receiver && !allowed_arg)
        || (is_receiver_use(&body[use_index].stmt, name) && !allowed_receiver)
    {
        return None;
    }
    Some(InlineTempPlan {
        name: name.to_string(),
        init: temp.init.clone(),
        def_path: AstPath(producer_path),
        use_path: AstPath(use_path),
    })
}

fn stmt_contains_call(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Stmt, &AstPath(path.to_vec()))
        .is_some_and(|fact| {
            fact.effects.contains(&EffectKind::ReadOnlyCall)
                || fact.effects.contains(&EffectKind::UnknownCall)
                || fact.effects.contains(&EffectKind::MethodCall)
                || fact.effects.contains(&EffectKind::MacroExpansion)
        })
}

fn is_receiver_use(stmt: &Stmt, name: &str) -> bool {
    support_walk::stmt_expr_any(stmt, &mut |expr| {
        let receiver = match expr {
            Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => Some(&**recv),
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => Some(&**base),
            _ => None,
        };
        matches!(receiver, Some(Expr::Var(v)) if v.as_str() == name)
    })
}

fn is_option_receiver_use(stmt: &Stmt, name: &str, ty: Option<&Type>) -> bool {
    if !ty.is_some_and(is_option_like_type) {
        return false;
    }
    support_walk::stmt_expr_any(stmt, &mut |expr| {
        matches!(
            expr,
            Expr::MethodCall { recv, method, args }
                if matches!(method.as_str(), "is_some" | "is_none" | "unwrap")
                    && args.is_empty()
                    && matches!(&**recv, Expr::Var(v) if v.as_str() == name)
        )
    })
}

fn is_option_like_type(ty: &Type) -> bool {
    match ty {
        Type::FnPtr { .. } => true,
        Type::Generic { name, .. } => name == "Option",
        Type::Custom(name) => name.starts_with("Option<"),
        _ => false,
    }
}

fn early_effectful_consumer(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Assign { target, value } => {
            matches!(target, Expr::Var(target) if target.as_str() == "__retval")
                && matches!(value, Expr::Var(value) if value.as_str() == name)
        }
        Stmt::Return(Some(expr)) => matches!(expr, Expr::Var(value) if value.as_str() == name),
        _ => false,
    }
}

struct ArgumentUse<'a> {
    stmt: &'a Stmt,
    name: &'a str,
    init: &'a Expr,
    ty: Option<&'a Type>,
    producer_path: &'a [PathSegment],
    adjacent: bool,
    env: InlineEnv<'a>,
}

fn is_allowed_argument_use(arg: ArgumentUse<'_>) -> bool {
    if arg.env.phase == Phase::Early {
        return false;
    }
    if is_effectful_expr(arg.env.function, arg.env.facts, arg.producer_path) {
        return arg.adjacent && simple_macro_arg_use(arg.stmt, arg.name);
    }
    if method_arg_use(arg.stmt, arg.name) && contains_integer_literal(arg.init) {
        return false;
    }
    type_stable_arg_init(arg.init, arg.ty) && call_or_macro_arg_use(arg.stmt, arg.name)
}

fn is_effectful_expr(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
        .is_some_and(|fact| {
            fact.effects.iter().any(|effect| {
                matches!(
                    effect,
                    EffectKind::UnknownCall
                        | EffectKind::MethodCall
                        | EffectKind::MacroExpansion
                        | EffectKind::UnknownSideEffect
                        | EffectKind::VolatileRead
                        | EffectKind::VolatileWrite
                        | EffectKind::AtomicWrite
                        | EffectKind::MemoryWrite
                )
            })
        })
}

fn type_stable_arg_init(init: &Expr, ty: Option<&Type>) -> bool {
    match init {
        Expr::Var(_) | Expr::Cast { .. } => true,
        Expr::Unary { .. } => ty.is_some(),
        Expr::Binary { .. } => ty.is_some() && !contains_integer_literal(init),
        Expr::Index { .. } => ty.is_some(),
        Expr::Block(block) | Expr::Unsafe(block) if block.stmts.is_empty() => block
            .tail
            .as_deref()
            .is_some_and(|tail| type_stable_arg_init(tail, ty)),
        Expr::Value(RustValue::I64(_)) => matches!(ty, Some(Type::Prim(Prim::I32))),
        Expr::Value(RustValue::Bool(_)) => true,
        _ => false,
    }
}

fn contains_integer_literal(expr: &Expr) -> bool {
    match expr {
        Expr::Value(RustValue::I64(_) | RustValue::I128(_) | RustValue::Usize(_)) => true,
        Expr::Unary { expr, .. } | Expr::Cast { expr, .. } => contains_integer_literal(expr),
        Expr::Binary { lhs, rhs, .. } => {
            contains_integer_literal(lhs) || contains_integer_literal(rhs)
        }
        Expr::Index { base, .. } => contains_integer_literal(base),
        _ => false,
    }
}

fn call_or_macro_arg_use(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_some_and(|expr| call_or_macro_arg_use_expr(expr, name)),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            call_or_macro_arg_use_expr(target, name) || call_or_macro_arg_use_expr(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => call_or_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn method_arg_use(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_some_and(|expr| method_arg_use_expr(expr, name)),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            method_arg_use_expr(target, name) || method_arg_use_expr(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => method_arg_use_expr(expr, name),
        _ => false,
    }
}

fn method_arg_use_expr(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::MethodCall { args, .. } | Expr::MethodCallGeneric { args, .. } => {
            args.iter().any(|arg| expr_ident_count(arg, name) > 0)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| method_arg_use_expr(tail, name)),
        Expr::Cast { expr, .. } => method_arg_use_expr(expr, name),
        _ => false,
    }
}

fn call_or_macro_arg_use_expr(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Call { args, .. } | Expr::Macro { args, .. } => {
            args.iter().any(|arg| call_arg_uses_name(arg, name))
        }
        Expr::MethodCall { args, .. } | Expr::MethodCallGeneric { args, .. } => {
            args.iter().any(|arg| expr_ident_count(arg, name) > 0)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| call_or_macro_arg_use_expr(tail, name)),
        Expr::Cast { expr, .. } => call_or_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn call_arg_uses_name(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(var) => var.as_str() == name,
        Expr::Cast { expr, .. } => call_arg_uses_name(expr, name),
        Expr::Block(block) | Expr::Unsafe(block) if block.stmts.is_empty() => block
            .tail
            .as_deref()
            .is_some_and(|tail| call_arg_uses_name(tail, name)),
        _ => false,
    }
}

fn simple_macro_arg_use(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => simple_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn simple_macro_arg_use_expr(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Macro { args, .. } => {
            args.iter().any(|arg| simple_macro_arg_uses_name(arg, name))
                && args
                    .iter()
                    .all(|arg| simple_macro_arg_uses_name(arg, name) || is_obviously_pure_expr(arg))
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| simple_macro_arg_use_expr(tail, name)),
        _ => false,
    }
}

fn simple_macro_arg_uses_name(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(var) => var.as_str() == name,
        Expr::Cast { expr, .. } | Expr::Unary { expr, .. } => {
            simple_macro_arg_uses_name(expr, name)
        }
        _ => false,
    }
}

fn is_obviously_pure_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::HexFloat(_)
        | Expr::Var(_)
        | Expr::Path(_) => true,
        Expr::Cast { expr, .. } | Expr::Unary { expr, .. } => is_obviously_pure_expr(expr),
        _ => false,
    }
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

fn inline_stmt_path(body_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = body_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

fn direct_stmt_index(body_path: &[PathSegment], read: &AstPath) -> Option<usize> {
    let rest = read.0.strip_prefix(body_path)?;
    match rest {
        [PathSegment::Stmt(index), ..] => Some(*index),
        _ => None,
    }
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

/// An `ExprSite` pointing at one statement in a `StmtWindowSite`'s window,
/// for `Evidence`/`Rejection` locations - `Evidence`/`Rejection` are shared
/// by every rule kind and only know how to carry an `ExprSite`.
fn stmt_window_evidence_site(window: &StmtWindowSite, stmt_index: usize) -> ExprSite {
    let mut path = window.path.0.clone();
    path.push(PathSegment::Stmt(stmt_index));
    expression_site(window.item_index, &path)
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
        _ => {
            let (method, recv) = pointer_method().matches(expr, &())?;
            let Expr::Var(name) = recv else {
                return None;
            };
            Some((name.as_str(), method == "as_mut_ptr"))
        }
    }
}

fn pointer_method() -> NullaryMethodCall {
    NullaryMethodCall::one_of(&["as_ptr", "as_mut_ptr"])
}

fn count_matches_source_len(source: &ByteSource<'_>, count: &Expr) -> bool {
    match count {
        Expr::Cast { expr, .. } => count_matches_source_len(source, expr),
        _ => NullaryMethodCall::named("len")
            .matches(count, &())
            .is_some_and(|(_, recv)| matches_source_expr(source, recv)),
    }
}

fn matches_source_expr(source: &ByteSource<'_>, expr: &Expr) -> bool {
    match expr {
        Expr::Var(name) => name.as_str() == source.name,
        _ => {
            source.representation == ByteRepresentation::Str
                && NullaryMethodCall::named("as_bytes")
                    .matches(expr, &())
                    .is_some_and(|(_, recv)| matches_source_expr(source, recv))
        }
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

fn static_item_index(program: &Program, name: &str) -> Option<usize> {
    program.items.iter().position(
        |item| matches!(item, Item::Static { name: static_name, .. } if static_name == name),
    )
}
