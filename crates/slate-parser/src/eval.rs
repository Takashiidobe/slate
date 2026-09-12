use crate::ast::*;
use crate::reachability::mark_unreachable;
use std::collections::HashSet;

#[derive(Debug, Clone, Default)]
pub struct Env {
    defined: HashSet<String>,
}

impl Env {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn define(mut self, name: impl Into<String>) -> Self {
        self.defined.insert(name.into());
        self
    }

    pub fn is_defined(&self, name: &str) -> bool {
        self.defined.contains(name)
    }
}

impl TranslationUnit {
    pub fn eval(&self, env: &Env) -> ConcreteTranslationUnit {
        ConcreteTranslationUnit {
            decls: self.decls.iter().flat_map(|decl| decl.eval(env)).collect(),
        }
    }
}

impl Span<Decl> {
    fn eval(&self, env: &Env) -> Vec<SpannedConcreteDecl> {
        let concrete = match &self.value {
            Decl::Comment {
                text,
                loc,
                provenance,
            } => vec![ConcreteDecl::Comment {
                text: text.clone(),
                loc: *loc,
                provenance: *provenance,
            }],
            Decl::Function(f) => vec![ConcreteDecl::Function(ConcreteFunctionDecl {
                ret_type: f.ret_type.clone(),
                name: f.name.clone(),
                parameters: f.parameters.clone(),
                variadic: f.variadic,
                body: mark_unreachable(Span::<Stmt>::eval_all(&f.body, env)),
                provenance: f.provenance,
                qualifiers: f.qualifiers,
                storage: f.storage,
                is_inline: f.is_inline,
                is_noreturn: f.is_noreturn,
                attributes: f.attributes.clone(),
            })],
            Decl::Declaration {
                declaration,
                provenance,
            } => vec![ConcreteDecl::Declaration {
                declaration: Declaration {
                    initializer: declaration
                        .initializer
                        .as_ref()
                        .and_then(|initializer| initializer.eval(env)),
                    ..declaration.clone()
                },
                provenance: *provenance,
            }],
            Decl::Typedef {
                name,
                ty,
                provenance,
                attributes,
            } => vec![ConcreteDecl::Typedef {
                name: name.clone(),
                ty: ty.clone(),
                provenance: *provenance,
                attributes: attributes.clone(),
            }],
            Decl::Record(record) => vec![ConcreteDecl::Record(record.eval(env))],
            Decl::Enum(enumeration) => vec![ConcreteDecl::Enum(enumeration.clone())],
            Decl::Conditional(cond) => {
                return match cond.select_branch(env) {
                    Some(body) => body.iter().flat_map(|decl| decl.eval(env)).collect(),
                    None => vec![],
                };
            }
        };
        concrete
            .into_iter()
            .map(|decl| self.clone().with_value(decl))
            .collect()
    }
}

impl Span<Stmt> {
    fn eval_all(stmts: &[SpannedStmt], env: &Env) -> Vec<SpannedConcreteStmt> {
        stmts.iter().flat_map(|stmt| stmt.eval(env)).collect()
    }

    fn eval(&self, env: &Env) -> Vec<SpannedConcreteStmt> {
        let concrete = match &self.value {
            Stmt::Comment {
                text,
                loc,
                provenance,
            } => vec![ConcreteStmt::Comment {
                text: text.clone(),
                loc: *loc,
                provenance: *provenance,
            }],
            Stmt::Return(e) => vec![ConcreteStmt::Return(e.clone())],
            Stmt::Expr(e) => vec![ConcreteStmt::Expr(e.clone())],
            Stmt::Decl(declaration) => vec![ConcreteStmt::Decl(Declaration {
                initializer: declaration
                    .initializer
                    .as_ref()
                    .and_then(|initializer| initializer.eval(env)),
                ..declaration.clone()
            })],
            Stmt::Block(body) => vec![ConcreteStmt::Block(Self::eval_all(body, env))],
            Stmt::Conditional(cond) => {
                return match cond.select_branch(env) {
                    Some(body) => body.iter().flat_map(|stmt| stmt.eval(env)).collect(),
                    None => vec![],
                };
            }
            Stmt::If {
                condition,
                then_branch,
                else_branch,
            } => vec![ConcreteStmt::If {
                condition: condition.clone(),
                then_branch: Self::eval_all(then_branch, env),
                else_branch: else_branch
                    .as_ref()
                    .map(|branch| Self::eval_all(branch, env)),
            }],
            Stmt::While { condition, body } => vec![ConcreteStmt::While {
                condition: condition.clone(),
                body: Self::eval_all(body, env),
            }],
            Stmt::DoWhile { body, condition } => vec![ConcreteStmt::DoWhile {
                body: Self::eval_all(body, env),
                condition: condition.clone(),
            }],
            Stmt::For {
                init,
                condition,
                increment,
                body,
            } => vec![ConcreteStmt::For {
                init: init
                    .as_ref()
                    .and_then(|init| init.eval(env).into_iter().next().map(Box::new)),
                condition: condition.clone(),
                increment: increment.clone(),
                body: Self::eval_all(body, env),
            }],
            Stmt::Switch { discriminant, body } => vec![ConcreteStmt::Switch {
                discriminant: discriminant.clone(),
                body: Self::eval_all(body, env),
            }],
            Stmt::Case(value) => vec![ConcreteStmt::Case(value.clone())],
            Stmt::Default => vec![ConcreteStmt::Default],
            Stmt::Labeled(name) => vec![ConcreteStmt::Labeled(name.clone())],
            Stmt::Goto(name) => vec![ConcreteStmt::Goto(name.clone())],
            Stmt::ComputedGoto(target) => vec![ConcreteStmt::ComputedGoto(target.clone())],
            Stmt::NestedFunction(function) => {
                vec![ConcreteStmt::NestedFunction(Box::new(
                    ConcreteFunctionDecl {
                        ret_type: function.ret_type.clone(),
                        name: function.name.clone(),
                        parameters: function.parameters.clone(),
                        variadic: function.variadic,
                        body: mark_unreachable(Self::eval_all(&function.body, env)),
                        provenance: function.provenance,
                        qualifiers: function.qualifiers,
                        storage: function.storage,
                        is_inline: function.is_inline,
                        is_noreturn: function.is_noreturn,
                        attributes: function.attributes.clone(),
                    },
                ))]
            }
            Stmt::Break => vec![ConcreteStmt::Break],
            Stmt::Continue => vec![ConcreteStmt::Continue],
        };
        concrete
            .into_iter()
            .map(|stmt| self.clone().with_value(stmt))
            .collect()
    }
}

impl<T> Conditional<T> {
    fn select_branch(&self, env: &Env) -> Option<&T> {
        let mut selected: Option<&T> = None;
        for (c, value) in &self.branches {
            if c.eval(env) {
                assert!(
                    selected.is_none(),
                    "overlapping conditions: more than one branch evaluated true for the same \
                 conditional region under this environment"
                );
                selected = Some(value);
            }
        }
        selected
    }
}

impl RecordDecl {
    fn eval(&self, env: &Env) -> Self {
        Self {
            fields: self.fields.iter().flat_map(|item| item.eval(env)).collect(),
            ..self.clone()
        }
    }
}

impl Span<FieldItem> {
    fn eval(&self, env: &Env) -> Vec<SpannedFieldItem> {
        match &self.value {
            FieldItem::Comment {
                text,
                loc,
                provenance,
            } => vec![self.clone().with_value(FieldItem::Comment {
                text: text.clone(),
                loc: *loc,
                provenance: *provenance,
            })],
            FieldItem::Field(field) => {
                vec![self.clone().with_value(FieldItem::Field(field.clone()))]
            }
            FieldItem::Conditional(cond) => match cond.select_branch(env) {
                Some(items) => items.iter().flat_map(|item| item.eval(env)).collect(),
                None => vec![],
            },
        }
    }
}

impl Initializer {
    pub fn eval(&self, env: &Env) -> Option<Self> {
        match self {
            Self::Expr(_) => Some(self.clone()),
            Self::List(items) => Some(Self::List(
                items
                    .iter()
                    .filter_map(|item| {
                        Some(InitializerItem {
                            designators: item.designators.clone(),
                            value: item.value.eval(env)?,
                        })
                    })
                    .collect(),
            )),
            Self::Conditional(conditional) => conditional
                .select_branch(env)
                .and_then(|initializer| initializer.eval(env)),
        }
    }
}

impl Condition {
    fn eval(&self, env: &Env) -> bool {
        match self {
            Self::Defined(name) => env.is_defined(name),
            Self::Constant(value) => *value != 0,
            Self::Not(inner) => !inner.eval(env),
            Self::And(left, right) => left.eval(env) && right.eval(env),
            Self::Or(left, right) => left.eval(env) || right.eval(env),
        }
    }
}
