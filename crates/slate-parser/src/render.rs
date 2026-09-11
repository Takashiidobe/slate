use crate::ast::*;
use crate::eval::Env;
use crate::files::SearchPaths;
use std::path::Path;

pub fn render_file(src: &str, defines: &[String]) -> String {
    let ast = crate::parser::parse_translation_unit(src);
    render_ast(&ast, defines)
}

pub fn render_path(path: &Path, defines: &[String]) -> String {
    let (ast, _) = crate::parser::parse_translation_unit_from_file(path, &SearchPaths::default());
    render_ast(&ast, defines)
}

fn render_ast(ast: &TranslationUnit, defines: &[String]) -> String {
    let mut env = Env::new();
    for define in defines {
        env = env.define(define_name(define));
    }
    let concrete = crate::eval::eval_translation_unit(ast, &env);
    let mut out = String::new();
    out.push_str("polyvariant:\n");
    render_decls(&mut out, &ast.decls, "", "decl");
    out.push_str("concrete:\n");
    render_concrete_decls(&mut out, &concrete.decls, "", "decl");
    out
}

fn define_name(define: &str) -> String {
    define
        .strip_prefix("-D")
        .unwrap_or(define)
        .split_once('=')
        .map_or_else(
            || define.trim_start_matches("-D").to_string(),
            |(name, _)| name.to_string(),
        )
}

fn render_decls(out: &mut String, decls: &[Decl], indent: &str, label: &str) {
    for (index, decl) in decls.iter().enumerate() {
        match decl {
            Decl::Function(function) => {
                line(
                    out,
                    indent,
                    &format!(
                        "{label}[{index}]: function name={} return={}",
                        function.name,
                        type_name(&function.ret_type)
                    ),
                );
                render_stmts(out, &function.body, &format!("{indent}  "));
            }
            Decl::Declaration { declaration, .. } => line(
                out,
                indent,
                &format!(
                    "{label}[{index}]: declaration type={} declarator={}",
                    type_name(&declaration.specifiers.ty),
                    declarator_name(&declaration.declarator)
                ),
            ),
            Decl::Typedef { name, ty, .. } => line(
                out,
                indent,
                &format!(
                    "{label}[{index}]: typedef name={name} type={}",
                    type_name(ty)
                ),
            ),
            Decl::Conditional(conditional) => {
                line(out, indent, &format!("{label}[{index}]: conditional"));
                render_conditional_decls(out, conditional, &format!("{indent}  "));
            }
        }
    }
}

fn render_conditional_decls(out: &mut String, conditional: &Conditional<Vec<Decl>>, indent: &str) {
    for (index, (condition, decls)) in conditional.branches.iter().enumerate() {
        line(
            out,
            indent,
            &format!("branch[{index}]: when={}", condition_name(condition)),
        );
        render_decls(out, decls, &format!("{indent}  "), "decl");
    }
}

fn render_stmts(out: &mut String, stmts: &[Stmt], indent: &str) {
    for (index, stmt) in stmts.iter().enumerate() {
        match stmt {
            Stmt::Return(Expr::IntLit(value)) => {
                line(out, indent, &format!("stmt[{index}]: return {value}"));
            }
            Stmt::Conditional(conditional) => {
                line(out, indent, &format!("stmt[{index}]: conditional"));
                for (branch_index, (condition, branch)) in conditional.branches.iter().enumerate() {
                    line(
                        out,
                        &format!("{indent}  "),
                        &format!("branch[{branch_index}]: when={}", condition_name(condition)),
                    );
                    render_stmts(out, branch, &format!("{indent}    "));
                }
            }
        }
    }
}

fn render_concrete_decls(out: &mut String, decls: &[ConcreteDecl], indent: &str, label: &str) {
    for (index, decl) in decls.iter().enumerate() {
        match decl {
            ConcreteDecl::Function(function) => {
                line(
                    out,
                    indent,
                    &format!(
                        "{label}[{index}]: function name={} return={}",
                        function.name,
                        type_name(&function.ret_type)
                    ),
                );
                for (stmt_index, stmt) in function.body.iter().enumerate() {
                    match stmt {
                        ConcreteStmt::Return(Expr::IntLit(value)) => line(
                            out,
                            &format!("{indent}  "),
                            &format!("stmt[{stmt_index}]: return {value}"),
                        ),
                    }
                }
            }
            ConcreteDecl::Declaration { declaration, .. } => line(
                out,
                indent,
                &format!(
                    "{label}[{index}]: declaration type={} declarator={}",
                    type_name(&declaration.specifiers.ty),
                    declarator_name(&declaration.declarator)
                ),
            ),
            ConcreteDecl::Typedef { name, ty, .. } => line(
                out,
                indent,
                &format!(
                    "{label}[{index}]: typedef name={name} type={}",
                    type_name(ty)
                ),
            ),
        }
    }
}

fn line(out: &mut String, indent: &str, text: &str) {
    out.push_str(indent);
    out.push_str(text);
    out.push('\n');
}

fn condition_name(condition: &Condition) -> String {
    match condition {
        Condition::Defined(name) => format!("defined({name})"),
        Condition::Not(inner) => format!("not({})", condition_name(inner)),
    }
}

fn type_name(ty: &CType) -> String {
    match ty {
        CType::Void => "void".into(),
        CType::Bool => "bool".into(),
        CType::Char => "char".into(),
        CType::Int => "int".into(),
        CType::Named(name) => name.clone(),
        CType::Tagged { kind, name } => format!(
            "{} {}",
            tag_name(*kind),
            name.as_deref().unwrap_or("<anonymous>")
        ),
        CType::Qualified { ty, .. } => format!("qualified({})", type_name(ty)),
        CType::Pointer { pointee, .. } => format!("ptr({})", type_name(pointee)),
        CType::Array { element, size } => format!(
            "array({},size={})",
            type_name(element),
            array_size_name(size)
        ),
        CType::Function {
            return_type,
            parameters,
            variadic,
        } => format!(
            "function({},params={},variadic={variadic})",
            type_name(return_type),
            parameters
                .iter()
                .map(parameter_name)
                .collect::<Vec<_>>()
                .join(",")
        ),
    }
}

fn tag_name(kind: TagKind) -> &'static str {
    match kind {
        TagKind::Struct => "struct",
        TagKind::Union => "union",
        TagKind::Enum => "enum",
    }
}

fn array_size_name(size: &ArraySize) -> String {
    match size {
        ArraySize::Unspecified => "unspecified".into(),
        ArraySize::Expression(value) => match value.as_ref() {
            Expr::IntLit(value) => value.to_string(),
        },
    }
}

fn declarator_name(declarator: &Declarator) -> String {
    match declarator {
        Declarator::Abstract => "_".into(),
        Declarator::Name(name) => format!("name={name}"),
        Declarator::Pointer { inner, .. } => format!("pointer({})", declarator_name(inner)),
        Declarator::Array { inner, size } => {
            format!(
                "array({},size={})",
                declarator_name(inner),
                array_size_name(size)
            )
        }
        Declarator::Function {
            inner,
            parameters,
            variadic,
        } => format!(
            "function({},params={},variadic={variadic})",
            declarator_name(inner),
            parameters
                .iter()
                .map(parameter_name)
                .collect::<Vec<_>>()
                .join(",")
        ),
    }
}

fn parameter_name(parameter: &Parameter) -> String {
    match &parameter.declarator {
        None => type_name(&parameter.ty),
        Some(Declarator::Name(name)) => format!("{} {name}", type_name(&parameter.ty)),
        Some(declarator) => parameter_declarator_name(&parameter.ty, declarator),
    }
}

fn parameter_declarator_name(ty: &CType, declarator: &Declarator) -> String {
    match declarator {
        Declarator::Abstract => type_name(ty),
        Declarator::Name(name) => format!("{} {name}", type_name(ty)),
        Declarator::Pointer { inner, .. } => {
            format!("ptr({})", parameter_declarator_name(ty, inner))
        }
        Declarator::Array { inner, size } => format!(
            "array({},size={})",
            parameter_declarator_name(ty, inner),
            array_size_name(size)
        ),
        Declarator::Function { inner, .. } => {
            format!("function({})", parameter_declarator_name(ty, inner))
        }
    }
}
