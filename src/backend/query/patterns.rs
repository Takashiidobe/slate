use std::collections::BTreeSet;

use crate::backend::facts::{PlaceAccess, Purity};
use crate::backend::rust_ast::{Expr, IndentStmt, Label, Stmt, Type};

use super::field::Field;
use super::item::{Matcher, QueryDomain, QueryItem, StatementMatch};
use super::{
    BindingCategory, CallTarget, DefinitionGroup, DefinitionKind, EnumVariantRef, ExpressionKind,
    ExpressionRef, ExpressionRole, FieldRef, MatchArmRef, ParameterRef, ProgramRef, ResolvedValue,
    StatementContainerRef, StatementRange, TypeUseKind, TypeUseRef, Usage,
};

#[derive(Default)]
pub(in crate::backend) struct FnCall<Cx = ()> {
    pub(in crate::backend) target: Field<CallTarget, Cx>,
    pub(in crate::backend) arity: Field<usize, Cx>,
    pub(in crate::backend) arg_types: Field<Vec<Option<Type>>, Cx>,
}

impl Matcher for FnCall {
    type Capture<'db> = super::CallRecord;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Expression
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::Expression(expression) = item else {
            return None;
        };
        query
            .expression_call(expression)
            .ok()
            .map(|proof| proof.value)
            .filter(|call| call.site == expression.site)
            .filter(|call| {
                self.target.matches(&call.target, &())
                    && self.arity.matches(&call.args.len(), &())
                    && self.arg_types.matches(&query.call_arg_types(call), &())
            })
    }
}

#[derive(Default)]
pub(in crate::backend) struct AssignmentValue;

impl Matcher for AssignmentValue {
    type Capture<'db> = ExpressionRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Expression
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::Expression(expression) = item else {
            return None;
        };
        query
            .assign_value_sites()
            .contains(&expression.site)
            .then(|| expression.clone())
    }
}

pub(in crate::backend) struct WholeProgram {
    candidate: for<'snapshot> fn(&super::QueryContext<'snapshot>) -> bool,
}

impl WholeProgram {
    pub(in crate::backend) fn when(
        candidate: for<'snapshot> fn(&super::QueryContext<'snapshot>) -> bool,
    ) -> Self {
        Self { candidate }
    }
}

impl Matcher for WholeProgram {
    type Capture<'db> = ProgramRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Program
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::Program(program) = item else {
            return None;
        };
        (self.candidate)(query).then(|| program.clone())
    }
}

#[derive(Default)]
pub(in crate::backend) struct ExprPattern {
    pub(in crate::backend) kind: Field<ExpressionKind>,
    pub(in crate::backend) roles: Field<BTreeSet<ExpressionRole>>,
    pub(in crate::backend) parent: Field<Option<ExpressionKind>>,
    pub(in crate::backend) ancestors: Field<Vec<ExpressionKind>>,
    pub(in crate::backend) access: Field<Option<PlaceAccess>>,
}

impl Matcher for ExprPattern {
    type Capture<'db> = ExpressionRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Expression
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::Expression(expression) = item else {
            return None;
        };
        let access = query
            .expression_place(expression)
            .ok()
            .map(|proof| proof.value.access);
        (self.kind.matches(&query.expression_kind(expression)?, &())
            && self.roles.matches(&query.expression_roles(expression), &())
            && self
                .parent
                .matches(&query.parent_expression_kind(expression), &())
            && self
                .ancestors
                .matches(&query.ancestor_expression_kinds(expression), &())
            && self.access.matches(&access, &()))
        .then(|| expression.clone())
    }
}

#[derive(Default)]
pub(in crate::backend) struct Definition<Cx = ()> {
    pub(in crate::backend) kind: Field<DefinitionKind, Cx>,
    pub(in crate::backend) name: Field<String, Cx>,
    pub(in crate::backend) group: Field<Option<DefinitionGroup>, Cx>,
}

impl Matcher for Definition {
    type Capture<'db> = super::DefinitionSite;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Definition
    }

    fn matches<'db>(
        &self,
        _query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::Definition(definition) = item else {
            return None;
        };
        (self.kind.matches(&definition.kind, &())
            && self.name.matches(&definition.name, &())
            && self.group.matches(&definition.group, &()))
        .then(|| (*definition).clone())
    }
}

#[derive(Default)]
pub(in crate::backend) struct Function {
    pub(in crate::backend) name: Field<String>,
    pub(in crate::backend) arity: Field<usize>,
    pub(in crate::backend) parameter_names: Field<Vec<String>>,
    pub(in crate::backend) parameter_types: Field<Vec<Type>>,
    pub(in crate::backend) returns: Field<Option<Type>>,
    pub(in crate::backend) body_len: Field<usize>,
}

impl Matcher for Function {
    type Capture<'db> = super::FunctionRef<'db>;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Function
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::Function(function) = item else {
            return None;
        };
        let definition = query.function_def(function)?;
        (self.name.matches(&definition.name, &())
            && self.arity.matches(&definition.params.len(), &())
            && self.parameter_names.matches(
                &definition
                    .params
                    .iter()
                    .map(|parameter| parameter.name.clone())
                    .collect(),
                &(),
            )
            && self.parameter_types.matches(
                &definition
                    .params
                    .iter()
                    .map(|parameter| parameter.ty.clone())
                    .collect(),
                &(),
            )
            && self.returns.matches(&definition.ret, &())
            && self.body_len.matches(&definition.body.len(), &()))
        .then(|| function.clone())
    }
}

#[derive(Default)]
pub(in crate::backend) struct Parameter {
    pub(in crate::backend) index: Field<usize>,
    pub(in crate::backend) name: Field<String>,
    pub(in crate::backend) mutable: Field<bool>,
    pub(in crate::backend) ty: Field<Type>,
    pub(in crate::backend) value: Value,
}

impl Matcher for Parameter {
    type Capture<'db> = ParameterRef<'db>;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Parameter
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::Parameter(parameter) = item else {
            return None;
        };
        let definition = query.parameter_def(parameter)?;
        (self.index.matches(&parameter.index, &())
            && self.name.matches(&definition.name, &())
            && self.mutable.matches(&definition.mutable, &())
            && self.ty.matches(&definition.ty, &())
            && self
                .value
                .matches(&query.binding_value(&parameter.binding), &()))
        .then(|| parameter.clone())
    }
}

#[derive(Default)]
#[expect(
    dead_code,
    reason = "query API surface not yet wired into a fixup rule"
)]
pub(in crate::backend) struct StatementContainer {
    pub(in crate::backend) len: Field<usize>,
}

impl Matcher for StatementContainer {
    type Capture<'db> = StatementContainerRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::StatementContainer
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::StatementContainer(container) = item else {
            return None;
        };
        self.len
            .matches(&query.statement_container(container)?.len(), &())
            .then(|| container.clone())
    }
}

#[derive(Default)]
#[expect(
    dead_code,
    reason = "query API surface not yet wired into a fixup rule"
)]
pub(in crate::backend) struct MatchArm {
    pub(in crate::backend) index: Field<usize>,
    pub(in crate::backend) body_len: Field<usize>,
}

impl Matcher for MatchArm {
    type Capture<'db> = MatchArmRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::MatchArm
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::MatchArm(arm) = item else {
            return None;
        };
        let definition = query.match_arm(arm)?;
        (self.index.matches(&arm.index, &()) && self.body_len.matches(&definition.body.len(), &()))
            .then(|| arm.clone())
    }
}

#[derive(Default)]
#[expect(
    dead_code,
    reason = "query API surface not yet wired into a fixup rule"
)]
pub(in crate::backend) struct RecordField {
    pub(in crate::backend) index: Field<usize>,
    pub(in crate::backend) name: Field<Option<String>>,
    pub(in crate::backend) ty: Field<Type>,
}

impl Matcher for RecordField {
    type Capture<'db> = FieldRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Field
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::Field(field) = item else {
            return None;
        };
        let (name, ty) = query.field(field)?;
        (self.index.matches(&field.index, &())
            && self.name.matches(&name.map(str::to_string), &())
            && self.ty.matches(ty, &()))
        .then(|| field.clone())
    }
}

#[derive(Default)]
#[expect(
    dead_code,
    reason = "query API surface not yet wired into a fixup rule"
)]
pub(in crate::backend) struct EnumVariant {
    pub(in crate::backend) index: Field<usize>,
    pub(in crate::backend) name: Field<String>,
    pub(in crate::backend) value: Field<i64>,
}

impl Matcher for EnumVariant {
    type Capture<'db> = EnumVariantRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::EnumVariant
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::EnumVariant(variant) = item else {
            return None;
        };
        let definition = query.enum_variant(variant)?;
        (self.index.matches(&variant.index, &())
            && self.name.matches(&definition.name, &())
            && self.value.matches(&definition.value, &()))
        .then(|| variant.clone())
    }
}

#[derive(Default)]
#[expect(
    dead_code,
    reason = "query API surface not yet wired into a fixup rule"
)]
pub(in crate::backend) struct TypeUse {
    pub(in crate::backend) kind: Field<TypeUseKind>,
    pub(in crate::backend) ty: Field<Type>,
}

impl Matcher for TypeUse {
    type Capture<'db> = TypeUseRef<'db>;

    fn domain(&self) -> QueryDomain {
        QueryDomain::TypeUse
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::TypeUse(type_use) = item else {
            return None;
        };
        let kind = match type_use {
            TypeUseRef::FunctionReturn(_) => TypeUseKind::FunctionReturn,
            TypeUseRef::Parameter(_) => TypeUseKind::Parameter,
            TypeUseRef::Field(_) => TypeUseKind::Field,
        };
        (self.kind.matches(&kind, &()) && self.ty.matches(query.type_use(type_use)?, &()))
            .then(|| type_use.clone())
    }
}

#[derive(Default)]
pub(in crate::backend) struct Binding {
    pub(in crate::backend) kind: Field<BindingCategory>,
    pub(in crate::backend) name: Field<String>,
    pub(in crate::backend) ty: Field<Option<Type>>,
    pub(in crate::backend) value: Value,
}

impl Matcher for Binding {
    type Capture<'db> = super::BindingRef<'db>;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Binding
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::Binding(binding) = item else {
            return None;
        };
        (self.kind.matches(&binding.kind, &())
            && self.name.matches(&binding.name, &())
            && self.ty.matches(&binding.ty, &())
            && self.value.matches(&query.binding_value(binding), &()))
        .then(|| binding.clone())
    }
}

#[derive(Default)]
pub(in crate::backend) struct Value<Cx = ()> {
    pub(in crate::backend) ty: Field<Option<Type>, Cx>,
    pub(in crate::backend) usage: Field<Option<Usage>, Cx>,
    pub(in crate::backend) purity: Field<Option<Purity>, Cx>,
}

impl<Cx> Value<Cx> {
    pub(in crate::backend) fn matches(&self, resolved: &ResolvedValue, cx: &Cx) -> bool {
        self.ty.matches(&resolved.ty, cx)
            && self.usage.matches(&resolved.usage, cx)
            && self.purity.matches(&resolved.purity, cx)
    }
}

#[derive(Default)]
pub(in crate::backend) struct Local<Cx = ()> {
    pub(in crate::backend) name: Field<String, Cx>,
    pub(in crate::backend) mutable: Field<bool, Cx>,
    pub(in crate::backend) value: Value<Cx>,
}

pub(in crate::backend) struct StatementSequence<const N: usize> {
    first: Option<Local>,
}

impl<const N: usize> StatementSequence<N> {
    pub(in crate::backend) fn new() -> Self {
        assert!(N > 0);
        Self { first: None }
    }

    pub(in crate::backend) fn starting_with(mut self, local: Local) -> Self {
        self.first = Some(local);
        self
    }
}

impl<const N: usize> Matcher for StatementSequence<N> {
    type Capture<'db> = StatementMatch<N>;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Statement
    }

    fn matches<'db>(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>> {
        let QueryItem::Statement(site) = item else {
            return None;
        };
        let tail = query.statement_tail(site)?;
        let statements = tail.get(..N)?;
        let target = StatementRange {
            item_index: site.item_index,
            path: site.range().path,
            start: site.range().start,
            end: site.range().start + N,
        };
        if let Some(local) = &self.first {
            let Stmt::Let { name, mutable, .. } = &statements[0].stmt else {
                return None;
            };
            if !local.name.matches(name, &())
                || !local.mutable.matches(mutable, &())
                || !local.value.matches(&query.local_value(&target, name), &())
            {
                return None;
            }
        }
        Some(StatementMatch::new(target))
    }
}

#[derive(Default)]
pub(in crate::backend) struct ExternFn<Cx = ()> {
    pub(in crate::backend) name: Field<String, Cx>,
    pub(in crate::backend) arity: Field<usize, Cx>,
    pub(in crate::backend) returns: Field<Option<Type>, Cx>,
}

pub(in crate::backend) struct NullaryMethodCall<Cx = ()> {
    method: Field<String, Cx>,
}

impl<Cx> NullaryMethodCall<Cx> {
    pub(in crate::backend) fn named(name: impl Into<String>) -> Self {
        Self {
            method: Field::eq(name.into()),
        }
    }

    pub(in crate::backend) fn one_of(names: &'static [&'static str]) -> Self {
        Self {
            method: Field::predicate(move |method: &String, _cx: &Cx| {
                names.contains(&method.as_str())
            }),
        }
    }

    pub(in crate::backend) fn matches<'e>(
        &self,
        expr: &'e Expr,
        cx: &Cx,
    ) -> Option<(&'e str, &'e Expr)> {
        let Expr::MethodCall { recv, method, args } = expr else {
            return None;
        };
        (args.is_empty() && self.method.matches(method, cx)).then_some((method.as_str(), &**recv))
    }
}

pub(in crate::backend) struct LetStmtPattern<Cx = ()> {
    name: Field<String, Cx>,
}

impl<Cx> LetStmtPattern<Cx> {
    pub(in crate::backend) fn any() -> Self {
        Self { name: Field::any() }
    }

    pub(in crate::backend) fn matches<'s>(&self, stmt: &'s Stmt, cx: &Cx) -> Option<&'s str> {
        let Stmt::Let { name, .. } = stmt else {
            return None;
        };
        self.name.matches(name, cx).then_some(name.as_str())
    }
}

pub(in crate::backend) struct LoopStmtPattern<Cx = ()> {
    label: Field<Option<Label>, Cx>,
}

impl<Cx> LoopStmtPattern<Cx> {
    pub(in crate::backend) fn unlabeled() -> Self {
        Self {
            label: Field::eq(None),
        }
    }

    pub(in crate::backend) fn matches<'s>(
        &self,
        stmt: &'s Stmt,
        cx: &Cx,
    ) -> Option<&'s [IndentStmt]> {
        let Stmt::Loop { label, body } = stmt else {
            return None;
        };
        self.label.matches(label, cx).then_some(body.as_slice())
    }
}
