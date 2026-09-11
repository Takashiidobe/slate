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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::parser::parse_translation_unit;

    const MAIN: Provenance = Provenance {
        file: FileId(0),
        kind: HeaderKind::User,
    };

    #[test]
    fn selects_win32_branch_when_defined() {
        let tu = parse_translation_unit(
            "int main() {\n#ifdef _WIN32\nreturn 2;\n#else\nreturn 3;\n#endif\n}\n",
        );
        let concrete = eval_translation_unit(&tu, &Env::new().define("_WIN32"));
        assert_eq!(
            concrete,
            ConcreteTranslationUnit {
                decls: vec![ConcreteDecl::Function(ConcreteFunctionDecl {
                    ret_type: Type::Int,
                    name: "main".into(),
                    body: vec![ConcreteStmt::Return(Expr::IntLit(2))],
                    provenance: MAIN,
                })],
            }
        );
    }

    #[test]
    fn selects_else_branch_when_undefined() {
        let tu = parse_translation_unit(
            "int main() {\n#ifdef _WIN32\nreturn 2;\n#else\nreturn 3;\n#endif\n}\n",
        );
        let concrete = eval_translation_unit(&tu, &Env::new());
        assert_eq!(
            concrete,
            ConcreteTranslationUnit {
                decls: vec![ConcreteDecl::Function(ConcreteFunctionDecl {
                    ret_type: Type::Int,
                    name: "main".into(),
                    body: vec![ConcreteStmt::Return(Expr::IntLit(3))],
                    provenance: MAIN,
                })],
            }
        );
    }

    #[test]
    fn typedef_socket_resolves_per_flag() {
        let tu = parse_translation_unit(
            "#ifdef _WIN32\ntypedef HANDLE Socket;\n#else\ntypedef int Socket;\n#endif\n",
        );

        let win = eval_translation_unit(&tu, &Env::new().define("_WIN32"));
        assert_eq!(
            win.decls,
            vec![ConcreteDecl::Typedef {
                name: "Socket".into(),
                ty: CType::Named("HANDLE".into()),
                provenance: MAIN,
            }]
        );

        let other = eval_translation_unit(&tu, &Env::new());
        assert_eq!(
            other.decls,
            vec![ConcreteDecl::Typedef {
                name: "Socket".into(),
                ty: CType::Int,
                provenance: MAIN,
            }]
        );
    }

    #[test]
    fn ifdef_without_else_contributes_nothing_when_undefined() {
        let tu = parse_translation_unit("#ifdef _WIN32\ntypedef HANDLE Socket;\n#endif\n");
        let concrete = eval_translation_unit(&tu, &Env::new());
        assert_eq!(concrete.decls, vec![]);
    }
}
