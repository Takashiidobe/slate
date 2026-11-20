use super::codegen::item_to_string;
use super::rust_ast::{BinOp, Cfg, Expr, FnDef, Ident, Item, RustValue, Stmt, UnaryOp, Visibility};
use crate::function_identity::CallBinding;

#[derive(Debug, Clone, Default)]
pub struct BuildScript {
    body: Vec<Stmt>,
}

impl BuildScript {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn is_empty(&self) -> bool {
        self.body.is_empty()
    }

    pub fn push(&mut self, step: Step) {
        self.body.push(step.into_stmt());
    }

    pub fn extend(&mut self, steps: impl IntoIterator<Item = Step>) {
        self.body.extend(steps.into_iter().map(Step::into_stmt));
    }

    pub fn render(self) -> String {
        let main = Item::Fn(FnDef {
            comments: Vec::new(),
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".into(),
            params: Vec::new(),
            ret: None,
            noreturn: false,
            body: self.body,
        });
        item_to_string(&main)
    }
}

#[derive(Debug, Clone)]
pub enum Step {
    RerunIfChanged(String),
    LinkArg { target: String, arg: String },
    CompileC { file: String, library: String },
    When { cfg: Cfg, steps: Vec<Step> },
}

impl Step {
    fn into_stmt(self) -> Stmt {
        match self {
            Self::RerunIfChanged(path) => cargo_directive("rerun-if-changed", &path),
            Self::LinkArg { target, arg } => {
                cargo_directive(&format!("rustc-link-arg-{target}"), &arg)
            }
            Self::CompileC { file, library } => {
                let build = call("cc::Build::new", Vec::new());
                let build = method(build, "file", vec![Expr::Str(file)]);
                Stmt::Expr(method(build, "compile", vec![Expr::Str(library)]))
            }
            Self::When { cfg, steps } => Stmt::If {
                cond: cfg_condition(&cfg),
                then_body: steps.into_iter().map(Self::into_stmt).collect(),
                else_body: Vec::new(),
            },
        }
    }
}

fn cargo_directive(key: &str, value: &str) -> Stmt {
    let line = format!("cargo:{key}={value}")
        .replace('{', "{{")
        .replace('}', "}}");
    Stmt::Expr(Expr::Macro {
        name: "println".into(),
        args: vec![Expr::Str(line)],
    })
}

fn call(path: &str, args: Vec<Expr>) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(Ident::new(path))),
        args,
        binding: CallBinding::Generated,
    }
}

fn method(recv: Expr, name: &str, args: Vec<Expr>) -> Expr {
    Expr::MethodCall {
        recv: Box::new(recv),
        method: name.into(),
        args,
    }
}

fn var(name: &str) -> Expr {
    Expr::Var(Ident::new(name))
}

fn binary(op: BinOp, lhs: Expr, rhs: Expr) -> Expr {
    Expr::Binary {
        op,
        lhs: Box::new(lhs),
        rhs: Box::new(rhs),
    }
}

fn env_name(name: &str) -> String {
    name.to_uppercase().replace('-', "_")
}

fn env_is_set(variable: String) -> Expr {
    method(
        call("std::env::var_os", vec![Expr::Str(variable)]),
        "is_some",
        Vec::new(),
    )
}

fn env_list_contains(variable: String, value: &str) -> Expr {
    let entries = method(var("v"), "split", vec![Expr::Str(",".into())]);
    let matches = Expr::Closure {
        params: vec![Ident::new("v")],
        body: Box::new(binary(BinOp::Eq, var("v"), Expr::Str(value.into()))),
    };
    let any = Expr::Closure {
        params: vec![Ident::new("v")],
        body: Box::new(method(entries, "any", vec![matches])),
    };
    method(
        call("std::env::var", vec![Expr::Str(variable)]),
        "is_ok_and",
        vec![any],
    )
}

fn cfg_condition(cfg: &Cfg) -> Expr {
    let fold = |items: &[Cfg], op: BinOp, empty: bool| {
        items
            .iter()
            .map(cfg_condition)
            .reduce(|lhs, rhs| binary(op, lhs, rhs))
            .unwrap_or(Expr::Value(RustValue::Bool(empty)))
    };
    match cfg {
        Cfg::Flag(flag) => env_is_set(format!("CARGO_CFG_{}", env_name(flag))),
        Cfg::Opt { key, value } if key == "feature" => {
            env_is_set(format!("CARGO_FEATURE_{}", env_name(value)))
        }
        Cfg::Opt { key, value } => env_list_contains(format!("CARGO_CFG_{}", env_name(key)), value),
        Cfg::Not(inner) => Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(cfg_condition(inner)),
        },
        Cfg::Any(items) => fold(items, BinOp::Or, false),
        Cfg::All(items) => fold(items, BinOp::And, true),
    }
}
