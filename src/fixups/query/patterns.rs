use std::collections::BTreeSet;

use crate::fixups::facts::{PlaceAccess, Purity};
use crate::rust_ast::{Expr, IndentStmt, Label, Stmt, Type};

use super::field::Field;
use super::item::{Matcher, QueryDomain, QueryItem, StatementMatch};
use super::{
    BindingCategory, CallTarget, DefinitionGroup, DefinitionKind, EnumVariantRef, ExpressionKind,
    ExpressionRef, ExpressionRole, FieldRef, MatchArmRef, ParameterRef, ProgramRef, ResolvedValue,
    StatementContainerRef, StatementRange, TypeUseKind, TypeUseRef, Usage,
};

#[derive(Default)]
pub(in crate::fixups) struct FnCall<Cx = ()> {
    pub(in crate::fixups) target: Field<CallTarget, Cx>,
    pub(in crate::fixups) arity: Field<usize, Cx>,
    pub(in crate::fixups) arg_types: Field<Vec<Option<Type>>, Cx>,
}

impl Matcher for FnCall {
    type Capture = super::CallRecord;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Expression
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
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
pub(in crate::fixups) struct AssignmentValue;

impl Matcher for AssignmentValue {
    type Capture = ExpressionRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Expression
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
        let QueryItem::Expression(expression) = item else {
            return None;
        };
        query
            .assign_value_sites()
            .contains(&expression.site)
            .then(|| expression.clone())
    }
}

pub(in crate::fixups) struct WholeProgram {
    candidate: for<'snapshot> fn(&super::QueryContext<'snapshot>) -> bool,
}

impl WholeProgram {
    pub(in crate::fixups) fn when(
        candidate: for<'snapshot> fn(&super::QueryContext<'snapshot>) -> bool,
    ) -> Self {
        Self { candidate }
    }
}

impl Matcher for WholeProgram {
    type Capture = ProgramRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Program
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
        let QueryItem::Program(program) = item else {
            return None;
        };
        (self.candidate)(query).then(|| program.clone())
    }
}

#[derive(Default)]
pub(in crate::fixups) struct ExprPattern {
    pub(in crate::fixups) kind: Field<ExpressionKind>,
    pub(in crate::fixups) roles: Field<BTreeSet<ExpressionRole>>,
    pub(in crate::fixups) parent: Field<Option<ExpressionKind>>,
    pub(in crate::fixups) ancestors: Field<Vec<ExpressionKind>>,
    pub(in crate::fixups) access: Field<Option<PlaceAccess>>,
}

impl Matcher for ExprPattern {
    type Capture = ExpressionRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Expression
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
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
pub(in crate::fixups) struct Definition<Cx = ()> {
    pub(in crate::fixups) kind: Field<DefinitionKind, Cx>,
    pub(in crate::fixups) name: Field<String, Cx>,
    pub(in crate::fixups) group: Field<Option<DefinitionGroup>, Cx>,
}

impl Matcher for Definition {
    type Capture = super::DefinitionSite;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Definition
    }

    fn matches(
        &self,
        _query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
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
pub(in crate::fixups) struct Function {
    pub(in crate::fixups) name: Field<String>,
    pub(in crate::fixups) arity: Field<usize>,
    pub(in crate::fixups) parameter_names: Field<Vec<String>>,
    pub(in crate::fixups) parameter_types: Field<Vec<Type>>,
    pub(in crate::fixups) returns: Field<Option<Type>>,
    pub(in crate::fixups) body_len: Field<usize>,
}

impl Matcher for Function {
    type Capture = super::FunctionRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Function
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
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
pub(in crate::fixups) struct Parameter {
    pub(in crate::fixups) index: Field<usize>,
    pub(in crate::fixups) name: Field<String>,
    pub(in crate::fixups) mutable: Field<bool>,
    pub(in crate::fixups) ty: Field<Type>,
    pub(in crate::fixups) value: Value,
}

impl Matcher for Parameter {
    type Capture = ParameterRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Parameter
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
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
#[allow(dead_code)]
pub(in crate::fixups) struct StatementContainer {
    pub(in crate::fixups) len: Field<usize>,
}

impl Matcher for StatementContainer {
    type Capture = StatementContainerRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::StatementContainer
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
        let QueryItem::StatementContainer(container) = item else {
            return None;
        };
        self.len
            .matches(&query.statement_container(container)?.len(), &())
            .then(|| container.clone())
    }
}

#[derive(Default)]
#[allow(dead_code)]
pub(in crate::fixups) struct MatchArm {
    pub(in crate::fixups) index: Field<usize>,
    pub(in crate::fixups) body_len: Field<usize>,
}

impl Matcher for MatchArm {
    type Capture = MatchArmRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::MatchArm
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
        let QueryItem::MatchArm(arm) = item else {
            return None;
        };
        let definition = query.match_arm(arm)?;
        (self.index.matches(&arm.index, &()) && self.body_len.matches(&definition.body.len(), &()))
            .then(|| arm.clone())
    }
}

#[derive(Default)]
#[allow(dead_code)]
pub(in crate::fixups) struct RecordField {
    pub(in crate::fixups) index: Field<usize>,
    pub(in crate::fixups) name: Field<Option<String>>,
    pub(in crate::fixups) ty: Field<Type>,
}

impl Matcher for RecordField {
    type Capture = FieldRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Field
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
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
#[allow(dead_code)]
pub(in crate::fixups) struct EnumVariant {
    pub(in crate::fixups) index: Field<usize>,
    pub(in crate::fixups) name: Field<String>,
    pub(in crate::fixups) value: Field<i64>,
}

impl Matcher for EnumVariant {
    type Capture = EnumVariantRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::EnumVariant
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
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
#[allow(dead_code)]
pub(in crate::fixups) struct TypeUse {
    pub(in crate::fixups) kind: Field<TypeUseKind>,
    pub(in crate::fixups) ty: Field<Type>,
}

impl Matcher for TypeUse {
    type Capture = TypeUseRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::TypeUse
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
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
pub(in crate::fixups) struct Binding {
    pub(in crate::fixups) kind: Field<BindingCategory>,
    pub(in crate::fixups) name: Field<String>,
    pub(in crate::fixups) ty: Field<Option<Type>>,
    pub(in crate::fixups) value: Value,
}

impl Matcher for Binding {
    type Capture = super::BindingRef;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Binding
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
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
pub(in crate::fixups) struct Value<Cx = ()> {
    pub(in crate::fixups) ty: Field<Option<Type>, Cx>,
    pub(in crate::fixups) usage: Field<Option<Usage>, Cx>,
    pub(in crate::fixups) purity: Field<Option<Purity>, Cx>,
}

impl<Cx> Value<Cx> {
    pub(in crate::fixups) fn matches(&self, resolved: &ResolvedValue, cx: &Cx) -> bool {
        self.ty.matches(&resolved.ty, cx)
            && self.usage.matches(&resolved.usage, cx)
            && self.purity.matches(&resolved.purity, cx)
    }
}

#[derive(Default)]
pub(in crate::fixups) struct Local<Cx = ()> {
    pub(in crate::fixups) name: Field<String, Cx>,
    pub(in crate::fixups) mutable: Field<bool, Cx>,
    pub(in crate::fixups) value: Value<Cx>,
}

pub(in crate::fixups) struct StatementSequence<const N: usize> {
    first: Option<Local>,
}

impl<const N: usize> StatementSequence<N> {
    pub(in crate::fixups) fn new() -> Self {
        assert!(N > 0);
        Self { first: None }
    }

    pub(in crate::fixups) fn starting_with(mut self, local: Local) -> Self {
        self.first = Some(local);
        self
    }
}

impl<const N: usize> Matcher for StatementSequence<N> {
    type Capture = StatementMatch<N>;

    fn domain(&self) -> QueryDomain {
        QueryDomain::Statement
    }

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
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
pub(in crate::fixups) struct ExternFn<Cx = ()> {
    pub(in crate::fixups) name: Field<String, Cx>,
    pub(in crate::fixups) arity: Field<usize, Cx>,
    pub(in crate::fixups) returns: Field<Option<Type>, Cx>,
}

pub(in crate::fixups) struct NullaryMethodCall<Cx = ()> {
    method: Field<String, Cx>,
}

impl<Cx> NullaryMethodCall<Cx> {
    pub(in crate::fixups) fn named(name: impl Into<String>) -> Self {
        Self {
            method: Field::eq(name.into()),
        }
    }

    pub(in crate::fixups) fn one_of(names: &'static [&'static str]) -> Self {
        Self {
            method: Field::predicate(move |method: &String, _cx: &Cx| {
                names.contains(&method.as_str())
            }),
        }
    }

    pub(in crate::fixups) fn matches<'e>(
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

pub(in crate::fixups) struct LetStmtPattern<Cx = ()> {
    name: Field<String, Cx>,
}

impl<Cx> LetStmtPattern<Cx> {
    pub(in crate::fixups) fn any() -> Self {
        Self { name: Field::any() }
    }

    pub(in crate::fixups) fn matches<'s>(&self, stmt: &'s Stmt, cx: &Cx) -> Option<&'s str> {
        let Stmt::Let { name, .. } = stmt else {
            return None;
        };
        self.name.matches(name, cx).then_some(name.as_str())
    }
}

pub(in crate::fixups) struct LoopStmtPattern<Cx = ()> {
    label: Field<Option<Label>, Cx>,
}

impl<Cx> LoopStmtPattern<Cx> {
    pub(in crate::fixups) fn unlabeled() -> Self {
        Self {
            label: Field::eq(None),
        }
    }

    pub(in crate::fixups) fn matches<'s>(
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
