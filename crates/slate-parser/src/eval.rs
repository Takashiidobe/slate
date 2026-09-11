use crate::ast::*;
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

impl Decl {
    fn eval(&self, env: &Env) -> Vec<ConcreteDecl> {
        match self {
            Self::Function(f) => vec![ConcreteDecl::Function(ConcreteFunctionDecl {
                ret_type: f.ret_type.clone(),
                name: f.name.clone(),
                body: Stmt::eval_all(&f.body, env),
                provenance: f.provenance,
                qualifiers: f.qualifiers,
                storage: f.storage,
                is_inline: f.is_inline,
                attributes: f.attributes.clone(),
            })],
            Self::Declaration {
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
            Self::Typedef {
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
            Self::Record(record) => vec![ConcreteDecl::Record(record.clone())],
            Self::Enum(enumeration) => vec![ConcreteDecl::Enum(enumeration.clone())],
            Self::Conditional(cond) => match cond.select_branch(env) {
                Some(body) => body.iter().flat_map(|decl| decl.eval(env)).collect(),
                None => vec![],
            },
        }
    }
}

impl Stmt {
    fn eval_all(stmts: &[Self], env: &Env) -> Vec<ConcreteStmt> {
        stmts.iter().flat_map(|stmt| stmt.eval(env)).collect()
    }

    fn eval(&self, env: &Env) -> Vec<ConcreteStmt> {
        match self {
            Self::Return(e) => vec![ConcreteStmt::Return(e.clone())],
            Self::Expr(e) => vec![ConcreteStmt::Expr(e.clone())],
            Self::Conditional(cond) => match cond.select_branch(env) {
                Some(body) => body.iter().flat_map(|stmt| stmt.eval(env)).collect(),
                None => vec![],
            },
        }
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
