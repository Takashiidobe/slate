use crate::fixups::facts::Purity;
use crate::rust_ast::{Expr, IndentStmt, Label, Stmt, Type};

use super::field::Field;
use super::item::{Matcher, QueryItem, StatementMatch};
use super::{CallTarget, DefinitionGroup, DefinitionKind, ResolvedValue, StatementRange, Usage};

#[derive(Default)]
pub(in crate::fixups) struct FnCall<Cx = ()> {
    pub(in crate::fixups) target: Field<CallTarget, Cx>,
    pub(in crate::fixups) arity: Field<usize, Cx>,
    pub(in crate::fixups) arg_types: Field<Vec<Option<Type>>, Cx>,
}

#[derive(Default)]
pub(in crate::fixups) struct Definition<Cx = ()> {
    pub(in crate::fixups) kind: Field<DefinitionKind, Cx>,
    pub(in crate::fixups) name: Field<String, Cx>,
    pub(in crate::fixups) group: Field<Option<DefinitionGroup>, Cx>,
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

pub(in crate::fixups) struct StatementSequence {
    width: usize,
    first: Option<Local>,
}

impl StatementSequence {
    pub(in crate::fixups) fn new(width: usize) -> Self {
        assert!(width > 0);
        Self { width, first: None }
    }

    pub(in crate::fixups) fn starting_with(mut self, local: Local) -> Self {
        self.first = Some(local);
        self
    }
}

impl Matcher for StatementSequence {
    type Capture = StatementMatch;

    fn matches(
        &self,
        query: &super::QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture> {
        let QueryItem::Statement { site, tail } = item;
        let statements = tail.get(..self.width)?;
        let target = StatementRange {
            item_index: site.item_index,
            path: site.range().path,
            start: site.range().start,
            end: site.range().start + self.width,
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
        Some(StatementMatch::new(target, statements.to_vec()))
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
