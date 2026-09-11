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

pub fn eval_translation_unit(tu: &TranslationUnit, env: &Env) -> ConcreteTranslationUnit {
    ConcreteTranslationUnit {
        decls: eval_decls(&tu.decls, env),
    }
}

fn eval_decls(decls: &[Decl], env: &Env) -> Vec<ConcreteDecl> {
    decls.iter().flat_map(|d| eval_decl(d, env)).collect()
}

fn eval_decl(decl: &Decl, env: &Env) -> Vec<ConcreteDecl> {
    match decl {
        Decl::Function(f) => vec![ConcreteDecl::Function(ConcreteFunctionDecl {
            ret_type: f.ret_type.clone(),
            name: f.name.clone(),
            body: eval_stmts(&f.body, env),
            provenance: f.provenance,
        })],
        Decl::Declaration {
            declaration,
            provenance,
        } => vec![ConcreteDecl::Declaration {
            declaration: declaration.clone(),
            provenance: *provenance,
        }],
        Decl::Typedef { name, ty, provenance } => vec![ConcreteDecl::Typedef {
            name: name.clone(),
            ty: ty.clone(),
            provenance: *provenance,
        }],
        Decl::Conditional(cond) => match select_branch(cond, env) {
            Some(body) => eval_decls(body, env),
            None => vec![],
        },
    }
}

fn eval_stmts(stmts: &[Stmt], env: &Env) -> Vec<ConcreteStmt> {
    stmts.iter().flat_map(|s| eval_stmt(s, env)).collect()
}

fn eval_stmt(stmt: &Stmt, env: &Env) -> Vec<ConcreteStmt> {
    match stmt {
        Stmt::Return(e) => vec![ConcreteStmt::Return(e.clone())],
        Stmt::Conditional(cond) => match select_branch(cond, env) {
            Some(body) => eval_stmts(body, env),
            None => vec![],
        },
    }
}

fn select_branch<'a, T>(cond: &'a Conditional<T>, env: &Env) -> Option<&'a T> {
    let mut selected: Option<&T> = None;
    for (c, value) in &cond.branches {
        if eval_condition(c, env) {
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

fn eval_condition(cond: &Condition, env: &Env) -> bool {
    match cond {
        Condition::Defined(name) => env.is_defined(name),
        Condition::Not(inner) => !eval_condition(inner, env),
    }
}
