use clang_ast::Node;
use serde::Deserialize;
use slate_parser::ast::*;
use slate_parser::eval::Env;
use slate_parser::files::SearchPaths;
use slate_parser::parser::Parser;
use std::path::{Path, PathBuf};
use std::process::Command;

type ClangNode = Node<ClangKind>;

#[derive(Debug, Deserialize)]
enum ClangKind {
    FunctionDecl(ClangFunctionDecl),
    TypedefDecl(ClangTypedefDecl),
    VarDecl(ClangTypedDecl),
    RecordDecl(ClangRecordDecl),
    EnumDecl(ClangEnumDecl),
    ReturnStmt,
    IntegerLiteral(ClangIntegerLiteral),
    Other,
}

#[derive(Debug, Deserialize)]
struct ClangFunctionDecl {
    name: Option<String>,
    #[serde(rename = "isImplicit", default)]
    is_implicit: bool,
    r#type: ClangQualType,
}

#[derive(Debug, Deserialize)]
struct ClangTypedefDecl {
    name: String,
    #[serde(rename = "isImplicit", default)]
    is_implicit: bool,
    r#type: ClangQualType,
}

#[derive(Debug, Deserialize)]
struct ClangTypedDecl {
    name: String,
    r#type: ClangQualType,
}

#[derive(Debug, Deserialize)]
struct ClangRecordDecl {
    name: Option<String>,
    #[serde(rename = "tagUsed", default)]
    tag_used: Option<String>,
}

#[derive(Debug, Deserialize)]
struct ClangEnumDecl {
    name: Option<String>,
}

#[derive(Debug, Deserialize)]
struct ClangQualType {
    #[serde(rename = "qualType")]
    qual_type: String,
}

#[derive(Debug, Deserialize)]
struct ClangIntegerLiteral {
    value: String,
}

#[derive(Debug, PartialEq, Eq)]
enum DeclSummary {
    Function {
        name: String,
        returns: Vec<i64>,
        signature: Option<String>,
    },
    Typedef {
        name: String,
        type_name: String,
    },
    Object {
        name: String,
        type_facts: String,
    },
    Record {
        kind: String,
        name: String,
    },
    Enum {
        name: String,
    },
}

fn fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures")
}

fn filecheck() -> String {
    std::env::var("SLATE_FILECHECK").unwrap_or_else(|_| "FileCheck".into())
}

fn configurations(source: &str) -> Vec<(String, Vec<String>)> {
    source
        .lines()
        .filter_map(|line| {
            let rest = line.trim().strip_prefix("// SLATE-FILECHECK-DEFINES ")?;
            let mut fields = rest.split_whitespace();
            let prefix = fields.next()?;
            Some((prefix.to_string(), fields.map(str::to_string).collect()))
        })
        .collect()
}

fn isystem_paths(source: &str) -> Vec<String> {
    source
        .lines()
        .filter_map(|line| line.trim().strip_prefix("// SLATE-FILECHECK-ISYSTEM "))
        .flat_map(str::split_whitespace)
        .map(expand_home)
        .collect()
}

fn expand_home(path: &str) -> String {
    path.strip_prefix("~/").map_or_else(
        || path.to_string(),
        |rest| {
            let home = std::env::var("HOME").expect("HOME must be set to expand ~ in path");
            format!("{home}/{rest}")
        },
    )
}

fn error_configurations(source: &str) -> Vec<String> {
    source
        .lines()
        .filter_map(|line| {
            line.trim()
                .strip_prefix("// SLATE-FILECHECK-ERROR ")
                .map(str::to_string)
        })
        .collect()
}

fn run_fixture(fixture: &Path, prefix: &str, defines: &[String], isystem: &[String], slot: usize) {
    let mut command = Command::new(env!("CARGO_BIN_EXE_slate-parser"));
    command
        .arg("parse")
        .arg(fixture)
        .env_remove("FORCE_COLOR")
        .env_remove("CLICOLOR_FORCE")
        .env("NO_COLOR", "1");
    for define in defines {
        command.arg(format!("-D{}", define.trim_start_matches("-D")));
    }
    for path in isystem {
        command.arg(format!("-isystem{path}"));
    }
    let rendered = command
        .output()
        .expect("run slate-parser filecheck renderer");
    assert!(
        rendered.status.success(),
        "renderer failed for {}:\n{}",
        fixture.display(),
        String::from_utf8_lossy(&rendered.stderr)
    );

    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/filecheck")
        .join(format!(
            "{}.{}.{}",
            fixture.file_stem().unwrap().to_string_lossy(),
            std::process::id(),
            slot
        ));
    std::fs::create_dir_all(&work).expect("create FileCheck work directory");
    let input = work.join("rendered.txt");
    std::fs::write(&input, rendered.stdout).expect("write rendered AST");

    let result = Command::new(filecheck())
        .arg(fixture)
        .arg(format!("--check-prefix={prefix}"))
        .arg("--input-file")
        .arg(&input)
        .arg("--dump-input=fail")
        .output()
        .expect("run FileCheck");
    assert!(
        result.status.success(),
        "FileCheck failed for {} ({prefix}):\n{}{}",
        fixture.display(),
        String::from_utf8_lossy(&result.stdout),
        String::from_utf8_lossy(&result.stderr)
    );

    if std::env::var_os("SLATE_CLANG_ORACLE").is_some() {
        assert_evaluated_matches_clang(fixture, defines, isystem);
    }
}

fn run_error_fixture(fixture: &Path, prefix: &str, slot: usize) {
    let output = Command::new(env!("CARGO_BIN_EXE_slate-parser"))
        .arg("parse")
        .arg(fixture)
        .env_remove("FORCE_COLOR")
        .env_remove("CLICOLOR_FORCE")
        .env("NO_COLOR", "1")
        .output()
        .expect("run slate-parser failing fixture");
    assert!(
        !output.status.success(),
        "fixture unexpectedly parsed: {}",
        fixture.display()
    );

    let work = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/filecheck")
        .join(format!(
            "{}.{}.{}",
            fixture.file_stem().unwrap().to_string_lossy(),
            std::process::id(),
            slot
        ));
    std::fs::create_dir_all(&work).expect("create FileCheck work directory");
    let input = work.join("diagnostic.txt");
    std::fs::write(&input, output.stderr).expect("write diagnostic");
    let result = Command::new(filecheck())
        .arg(fixture)
        .arg(format!("--check-prefix={prefix}"))
        .arg("--input-file")
        .arg(&input)
        .arg("--dump-input=fail")
        .output()
        .expect("run FileCheck");
    assert!(
        result.status.success(),
        "diagnostic FileCheck failed for {} ({prefix}):\n{}{}",
        fixture.display(),
        String::from_utf8_lossy(&result.stdout),
        String::from_utf8_lossy(&result.stderr)
    );
}

fn assert_evaluated_matches_clang(fixture: &Path, defines: &[String], isystem: &[String]) {
    if matches!(
        fixture.file_stem().and_then(|name| name.to_str()),
        Some(
            "generic-statement-expressions"
                | "fixed-point-types"
                | "typeof-types"
                | "target-builtin-types",
        )
    ) {
        return;
    }
    let search = SearchPaths {
        system: isystem.iter().map(std::path::PathBuf::from).collect(),
        ..SearchPaths::default()
    };
    let mut parser = Parser::new(search);
    let (ast, _) = parser.parse_file(fixture).expect("parse fixture");
    let mut env = Env::new();
    for define in defines {
        env = env.define(macro_name(define));
    }
    let evaluated = ast.eval(&env);
    let ours = summarize_evaluated(&evaluated);
    let theirs = summarize_clang(&run_clang_ast(fixture, defines, isystem));
    for summary in &ours {
        if matches!(summary, DeclSummary::Object { name, .. } if name == "<abstract>") {
            continue;
        }
        if matches!(summary, DeclSummary::Record { name, .. } if name == "<anonymous>") {
            continue;
        }
        assert!(
            theirs.contains(summary),
            "our evaluated reachable summary is absent from clang for {}:\nours: {summary:?}\nclang: {theirs:?}",
            fixture.display()
        );
    }
}

fn macro_name(define: &str) -> String {
    define.trim_start_matches("-D").split_once('=').map_or_else(
        || define.trim_start_matches("-D").to_string(),
        |(name, _)| name.to_string(),
    )
}

fn run_clang_ast(fixture: &Path, defines: &[String], isystem: &[String]) -> ClangNode {
    let mut command = Command::new("clang");
    if fixture.file_stem().and_then(|name| name.to_str()) == Some("c23-literals") {
        command.arg("-std=c2x");
    }
    command.args(["-Xclang", "-ast-dump=json", "-fsyntax-only"]);
    for define in defines {
        command.arg(format!("-D{}", define.trim_start_matches("-D")));
    }
    for path in isystem {
        command.arg("-isystem").arg(path);
    }
    let output = command
        .arg(fixture)
        .output()
        .expect("invoke clang AST oracle");
    assert!(
        output.status.success(),
        "clang rejected {}:\n{}",
        fixture.display(),
        String::from_utf8_lossy(&output.stderr)
    );
    serde_json::from_slice(&output.stdout).expect("clang emitted invalid AST JSON")
}

fn summarize_evaluated(tu: &ConcreteTranslationUnit) -> Vec<DeclSummary> {
    tu.decls
        .iter()
        .filter(|decl| match decl {
            ConcreteDecl::Declaration { declaration, .. } => declaration.attributes.is_empty(),
            ConcreteDecl::Typedef { attributes, .. } => attributes.is_empty(),
            _ => true,
        })
        .map(summarize_evaluated_decl)
        .collect()
}

fn summarize_evaluated_decl(decl: &ConcreteDecl) -> DeclSummary {
    match decl {
        ConcreteDecl::Function(function) => DeclSummary::Function {
            name: function.name.clone(),
            returns: function
                .body
                .iter()
                .filter_map(|stmt| match stmt {
                    ConcreteStmt::Return(Expr::IntLit(value)) => Some(*value),
                    ConcreteStmt::Return(Expr::StringLit(_)) => {
                        panic!("clang return was not an integer")
                    }
                    ConcreteStmt::Return(Expr::Generic { .. } | Expr::StatementExpression(_)) => {
                        None
                    }
                    ConcreteStmt::Return(
                        Expr::Identifier(_)
                        | Expr::Const(_)
                        | Expr::Unary { .. }
                        | Expr::Binary { .. }
                        | Expr::SizeOf(_),
                    ) => {
                        panic!("clang return was not an integer")
                    }
                    ConcreteStmt::Expr(_)
                    | ConcreteStmt::Decl(_)
                    | ConcreteStmt::Block(_)
                    | ConcreteStmt::If { .. }
                    | ConcreteStmt::While { .. }
                    | ConcreteStmt::DoWhile { .. }
                    | ConcreteStmt::For { .. }
                    | ConcreteStmt::Switch { .. }
                    | ConcreteStmt::Case(_)
                    | ConcreteStmt::Default
                    | ConcreteStmt::Labeled(_)
                    | ConcreteStmt::Goto(_)
                    | ConcreteStmt::ComputedGoto(_)
                    | ConcreteStmt::NestedFunction(_)
                    | ConcreteStmt::Break
                    | ConcreteStmt::Continue
                    | ConcreteStmt::Unreachable(_) => None,
                })
                .collect(),
            signature: None,
        },
        ConcreteDecl::Typedef { name, ty, .. } => DeclSummary::Typedef {
            name: name.clone(),
            type_name: normalize_type(&type_spelling(ty)),
        },
        ConcreteDecl::Declaration { declaration, .. } => {
            let name = declarator_identifier(&declaration.declarator);
            if matches!(declaration.declarator, Declarator::Function { .. }) {
                DeclSummary::Function {
                    name,
                    returns: vec![],
                    signature: Some(function_facts(
                        &declaration.specifiers.ty,
                        &declaration.declarator,
                    )),
                }
            } else {
                DeclSummary::Object {
                    name,
                    type_facts: object_facts(&declaration.specifiers, &declaration.declarator),
                }
            }
        }
        ConcreteDecl::Record(record) => DeclSummary::Record {
            kind: tag_name(record.kind).into(),
            name: record.name.clone().unwrap_or_else(|| "<anonymous>".into()),
        },
        ConcreteDecl::Enum(enumeration) => DeclSummary::Enum {
            name: enumeration
                .name
                .clone()
                .unwrap_or_else(|| "<anonymous>".into()),
        },
    }
}

fn summarize_clang(root: &ClangNode) -> Vec<DeclSummary> {
    root.inner.iter().filter_map(summarize_clang_decl).collect()
}

fn summarize_clang_decl(node: &ClangNode) -> Option<DeclSummary> {
    match &node.kind {
        ClangKind::FunctionDecl(function) if !function.is_implicit => Some(DeclSummary::Function {
            name: function.name.clone().expect("unnamed clang function"),
            returns: collect_returns(node),
            signature: if collect_returns(node).is_empty() {
                Some(clang_function_facts(&function.r#type.qual_type))
            } else {
                None
            },
        }),
        ClangKind::TypedefDecl(typedef) if !typedef.is_implicit => Some(DeclSummary::Typedef {
            name: typedef.name.clone(),
            type_name: normalize_type(&typedef.r#type.qual_type),
        }),
        ClangKind::VarDecl(declaration) if declaration.r#type.qual_type.contains(")(") => {
            Some(DeclSummary::Function {
                name: declaration.name.clone(),
                returns: vec![],
                signature: Some(clang_function_facts(&declaration.r#type.qual_type)),
            })
        }
        ClangKind::VarDecl(declaration) => Some(DeclSummary::Object {
            name: declaration.name.clone(),
            type_facts: clang_object_facts(&declaration.r#type.qual_type),
        }),
        ClangKind::RecordDecl(record) => record.name.clone().map(|name| DeclSummary::Record {
            kind: record.tag_used.clone().unwrap_or_else(|| "struct".into()),
            name,
        }),
        ClangKind::EnumDecl(enumeration) => enumeration
            .name
            .clone()
            .map(|name| DeclSummary::Enum { name }),
        _ => None,
    }
}

fn collect_returns(node: &ClangNode) -> Vec<i64> {
    let mut returns = Vec::new();
    collect_returns_into(node, &mut returns);
    returns
}

fn collect_returns_into(node: &ClangNode, returns: &mut Vec<i64>) {
    if let ClangKind::ReturnStmt = &node.kind
        && let Some(child) = node.inner.first()
        && let ClangKind::IntegerLiteral(literal) = &child.kind
    {
        returns.push(
            literal
                .value
                .parse()
                .expect("clang return was not an integer"),
        );
    }
    for child in &node.inner {
        collect_returns_into(child, returns);
    }
}

fn type_spelling(ty: &CType) -> String {
    match ty {
        CType::Bool => "_Bool".into(),
        CType::Integer(IntegerType::Char { signed }) => match signed {
            None => "char".into(),
            Some(true) => "signed char".into(),
            Some(false) => "unsigned char".into(),
        },
        CType::Integer(IntegerType::Ranked { rank, signed }) => {
            let prefix = if *signed { "" } else { "unsigned " };
            let name = match rank {
                IntegerRank::Short => "short",
                IntegerRank::Int => "int",
                IntegerRank::Long => "long",
                IntegerRank::LongLong => "long long",
                IntegerRank::Int128 => "__int128",
            };
            format!("{prefix}{name}")
        }
        CType::Integer(IntegerType::BitInt { width, signed }) => {
            format!("{} _BitInt({width})", if *signed { "" } else { "unsigned" })
        }
        CType::Void => "void".into(),
        CType::Floating(kind) => match kind {
            FloatingType::BFloat16 => "__bf16".into(),
            FloatingType::Float => "float".into(),
            FloatingType::Float16 => "_Float16".into(),
            FloatingType::Fp16 => "__fp16".into(),
            FloatingType::Float64x => "_Float64x".into(),
            FloatingType::Double => "double".into(),
            FloatingType::LongDouble => "long double".into(),
            FloatingType::Float128 => "_Float128".into(),
            FloatingType::Float128Ext => "__float128".into(),
        },
        CType::Complex(element) => format!("_Complex {}", type_spelling(element)),
        CType::Atomic(element) => format!("_Atomic({})", type_spelling(element)),
        CType::Vector(vector) => match &vector.size {
            VectorSize::Bytes(size) => {
                format!("{} vector_size({size})", type_spelling(&vector.element))
            }
            VectorSize::Lanes(size) => {
                format!("{} ext_vector_type({size})", type_spelling(&vector.element))
            }
        },
        CType::FixedPoint(fixed) => format!(
            "{}{}_{}",
            if fixed.saturated { "_Sat " } else { "" },
            match fixed.rank {
                FixedPointRank::Default => "",
                FixedPointRank::Short => "short ",
                FixedPointRank::Long => "long ",
                FixedPointRank::LongLong => "long long ",
            },
            match fixed.kind {
                FixedPointKind::Fract => "Fract",
                FixedPointKind::Accum => "Accum",
            }
        ),
        CType::TypeOf(TypeOfOperand::Expression(expression)) => {
            format!("typeof({expression})")
        }
        CType::TypeOf(TypeOfOperand::Type(ty)) => format!("typeof({})", type_spelling(ty)),
        CType::Imaginary(element) => format!("_Imaginary {}", type_spelling(element)),
        CType::TargetBuiltin(name) => name.clone(),
        CType::Named(name) => name.clone(),
        CType::Tagged { kind, name, .. } => format!(
            "{} {}",
            tag_name(*kind),
            name.as_deref().unwrap_or("<anonymous>")
        ),
        CType::Qualified { qualifiers, ty } => {
            let name = type_spelling(ty);
            let prefix = qualifier_spelling(*qualifiers);
            if qualifiers.is_atomic {
                format!("_Atomic({name})")
            } else {
                format!("{prefix}{name}")
            }
        }
        CType::Pointer { pointee, .. } => format!("{} *", type_spelling(pointee)),
        CType::Array { element, size } => {
            format!("{}[{}]", type_spelling(element), array_size(size))
        }
        CType::Function { return_type, .. } => format!("{} ()", type_spelling(return_type)),
    }
}

fn function_facts(base: &CType, declarator: &Declarator) -> String {
    let Declarator::Function {
        inner,
        parameters,
        variadic,
    } = declarator
    else {
        panic!("expected function declarator")
    };
    let pointer_to_function = matches!(inner.as_ref(), Declarator::Grouped(_));
    let return_pointer =
        !pointer_to_function && matches!(inner.as_ref(), Declarator::Pointer { .. });
    let params = parameters
        .iter()
        .map(parameter_fact)
        .collect::<Vec<_>>()
        .join(",");
    format!(
        "return={};return_pointer={return_pointer};pointer_to_function={pointer_to_function};params={params};variadic={variadic}",
        normalize_type(&type_spelling(base))
    )
}

fn parameter_fact(parameter: &Parameter) -> String {
    let pointer = matches!(parameter.declarator, Some(Declarator::Pointer { .. }));
    format!(
        "{}{}",
        normalize_type(&type_spelling(&parameter.ty)),
        if pointer { "*" } else { "" }
    )
}

fn object_facts(specifiers: &DeclarationSpecifiers, declarator: &Declarator) -> String {
    let mut dimensions = Vec::new();
    let mut current = declarator;
    while let Declarator::Array { inner, size } = current {
        dimensions.push(array_size(size));
        current = inner;
    }
    dimensions.reverse();
    let mut base = type_spelling(&specifiers.ty);
    let qualifiers = qualifier_spelling(specifiers.qualifiers);
    if specifiers.qualifiers.is_atomic {
        base = format!("_Atomic({base})");
    } else if !qualifiers.is_empty() {
        base = format!("{qualifiers}{base}");
    }
    if let Declarator::Pointer { qualifiers, .. } = current {
        let pointer_qualifiers = qualifier_spelling(*qualifiers);
        if !pointer_qualifiers.is_empty() {
            base.push_str(&format!("*{pointer_qualifiers}"));
        } else {
            base.push('*');
        }
    }
    format!(
        "base={};arrays=[{}]",
        normalize_type(&base),
        dimensions.join(",")
    )
}

fn qualifier_spelling(qualifiers: Qualifiers) -> String {
    [
        (qualifiers.is_const, "const"),
        (qualifiers.is_volatile, "volatile"),
        (qualifiers.is_restrict, "restrict"),
    ]
    .into_iter()
    .filter_map(|(enabled, name)| enabled.then_some(name))
    .collect()
}

fn clang_function_facts(qual_type: &str) -> String {
    let pointer_to_function = qual_type.contains("(*)");
    let (prefix, args_start) = if pointer_to_function {
        let marker = qual_type
            .find("(*)")
            .expect("clang function pointer marker");
        let args = qual_type
            .find(")(")
            .expect("clang function pointer parameters");
        (&qual_type[..marker], args + 2)
    } else {
        let open = qual_type
            .find('(')
            .expect("clang function type missing `(`");
        (&qual_type[..open], open + 1)
    };
    let prefix = prefix.trim();
    let args = qual_type[args_start..qual_type.len() - 1].trim();
    let return_pointer = !pointer_to_function && prefix.ends_with('*');
    let return_type = prefix.trim_end_matches('*').trim();
    let params = if args.is_empty() || args == "void" {
        String::new()
    } else {
        args.split(',')
            .filter(|param| param.trim() != "...")
            .map(normalize_type)
            .collect::<Vec<_>>()
            .join(",")
    };
    let variadic = args.split(',').any(|param| param.trim() == "...");
    format!(
        "return={return_type};return_pointer={return_pointer};pointer_to_function={pointer_to_function};params={params};variadic={variadic}",
        return_type = normalize_type(return_type)
    )
}

fn clang_object_facts(qual_type: &str) -> String {
    let first_array = qual_type.find('[');
    let (base, suffix) = first_array.map_or((qual_type, ""), |index| {
        (&qual_type[..index], &qual_type[index..])
    });
    let dimensions = suffix
        .split('[')
        .skip(1)
        .map(|dimension| dimension.trim_end_matches(']').trim())
        .collect::<Vec<_>>();
    format!(
        "base={};arrays=[{}]",
        normalize_type(base),
        dimensions.join(",")
    )
}

fn normalize_type(ty: &str) -> String {
    ty.split_whitespace().collect::<String>()
}

fn declarator_identifier(declarator: &Declarator) -> String {
    match declarator {
        Declarator::Name(name) => name.clone(),
        Declarator::Abstract => "<abstract>".into(),
        Declarator::Grouped(inner)
        | Declarator::Pointer { inner, .. }
        | Declarator::Array { inner, .. }
        | Declarator::Function { inner, .. } => declarator_identifier(inner),
    }
}

fn array_size(size: &ArraySize) -> String {
    match size {
        ArraySize::Unspecified => "".into(),
        ArraySize::Star => "*".into(),
        ArraySize::Expression(expression) => match expression.as_ref() {
            Expr::IntLit(value) => value.to_string(),
            Expr::StringLit(_) => panic!("array bound was not an integer"),
            Expr::Identifier(_)
            | Expr::Const(_)
            | Expr::Unary { .. }
            | Expr::Binary { .. }
            | Expr::SizeOf(_) => panic!("array bound was not an integer"),
            Expr::Generic { .. } | Expr::StatementExpression(_) => {
                panic!("array bound was not an integer")
            }
        },
    }
}

fn tag_name(kind: TagKind) -> &'static str {
    match kind {
        TagKind::Struct => "struct",
        TagKind::Union => "union",
        TagKind::Enum => "enum",
    }
}

#[test]
fn fixtures_are_filechecked() {
    let mut fixtures = std::fs::read_dir(fixtures_dir())
        .expect("read fixture directory")
        .map(|entry| entry.expect("read fixture entry").path())
        .filter(|path| path.extension().and_then(|extension| extension.to_str()) == Some("c"))
        .collect::<Vec<_>>();
    fixtures.sort();
    assert!(!fixtures.is_empty(), "no C fixtures found");

    let mut checked = 0;
    for fixture in fixtures {
        let source = std::fs::read_to_string(&fixture).expect("read fixture");
        let errors = error_configurations(&source);
        if !errors.is_empty() {
            for (slot, prefix) in errors.iter().enumerate() {
                run_error_fixture(&fixture, prefix, slot);
                checked += 1;
            }
            continue;
        }
        let configs = configurations(&source);
        assert!(
            !configs.is_empty(),
            "fixture has no FileCheck configurations: {}",
            fixture.display()
        );
        let isystem = isystem_paths(&source);
        for (slot, (prefix, defines)) in configs.iter().enumerate() {
            run_fixture(&fixture, prefix, defines, &isystem, slot);
            checked += 1;
        }
    }
    assert!(checked > 0, "no FileCheck configurations found");
}
