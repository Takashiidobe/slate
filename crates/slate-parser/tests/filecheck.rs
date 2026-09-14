use clang_ast::Node;
use serde::Deserialize;
use slate_parser::ast::*;
use slate_parser::compiler_args::CompilerFlavor;
use slate_parser::const_expr::Parser as ConstExprParser;
use slate_parser::files::{SearchPaths, decode_source_bytes};
use slate_parser::parser::{Parser, apply_abstract_declarator};
use std::path::{Path, PathBuf};
use std::process::Command;

type ClangNode = Node<ClangKind>;

thread_local! {
    static TAGS: std::cell::RefCell<Vec<(TagId, TagKind, Option<String>)>> =
        const { std::cell::RefCell::new(Vec::new()) };
}

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

fn flavor(source: &str) -> Option<String> {
    source
        .lines()
        .find_map(|line| line.trim().strip_prefix("// SLATE-FILECHECK-FLAVOR "))
        .map(|name| name.trim().to_string())
}

fn std_for_prefix(source: &str, prefix: &str) -> Option<String> {
    source.lines().find_map(|line| {
        let rest = line.trim().strip_prefix("// SLATE-FILECHECK-STD ")?;
        let mut fields = rest.split_whitespace();
        if fields.next()? != prefix {
            return None;
        }
        fields.next().map(str::to_string)
    })
}

fn show_ids_for_prefix(source: &str, prefix: &str) -> bool {
    source.lines().any(|line| {
        line.trim()
            .strip_prefix("// SLATE-FILECHECK-SHOW-IDS ")
            .is_some_and(|rest| rest.trim() == prefix)
    })
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

struct FixtureJob {
    fixture: PathBuf,
    prefix: String,
    defines: Vec<String>,
    isystem: Vec<String>,
    flavor: Option<String>,
    standard: Option<String>,
    show_ids: bool,
    error: bool,
    slot: usize,
}

fn run_job(job: FixtureJob) {
    if job.error {
        run_error_fixture(
            &job.fixture,
            &job.prefix,
            &job.defines,
            job.flavor.as_deref(),
            job.standard.as_deref(),
            job.slot,
        );
    } else {
        run_fixture(
            &job.fixture,
            &job.prefix,
            &job.defines,
            &job.isystem,
            job.flavor.as_deref(),
            job.standard.as_deref(),
            job.show_ids,
            job.slot,
        );
    }
}

#[expect(
    clippy::too_many_arguments,
    reason = "each param is an independent fixture config knob"
)]
fn run_fixture(
    fixture: &Path,
    prefix: &str,
    defines: &[String],
    isystem: &[String],
    flavor: Option<&str>,
    standard: Option<&str>,
    show_ids: bool,
    slot: usize,
) {
    let source = fixture_source(fixture);
    let self_include = format!(
        "#include \"{}\"",
        fixture.file_name().unwrap().to_string_lossy()
    );
    let temp_dir = std::env::temp_dir().join(format!(
        "slate-parser-filecheck-{}.{}.{}",
        fixture.file_stem().unwrap().to_string_lossy(),
        std::process::id(),
        slot
    ));
    let parsed_fixture = if source.contains(&self_include) {
        std::fs::create_dir_all(&temp_dir).expect("create isolated self-include fixture directory");
        temp_dir.join(fixture.file_name().unwrap())
    } else {
        fixture.with_file_name(format!(
            ".{}.filecheck.{}.{}.c",
            fixture.file_stem().unwrap().to_string_lossy(),
            std::process::id(),
            slot
        ))
    };
    std::fs::write(&parsed_fixture, &source).expect("write fixture without FileCheck metadata");
    let mut command = Command::new(env!("CARGO_BIN_EXE_slate-parser"));
    command
        .arg("parse")
        .arg(&parsed_fixture)
        .env_remove("FORCE_COLOR")
        .env_remove("CLICOLOR_FORCE")
        .env("NO_COLOR", "1");
    for define in defines {
        command.arg(format!("-D{}", define.trim_start_matches("-D")));
    }
    for path in isystem {
        command.arg(format!("-isystem{path}"));
    }
    if let Some(flavor) = flavor {
        command.arg(format!("--flavor={flavor}"));
    }
    if let Some(standard) = standard {
        command.arg(format!("-std={standard}"));
    }
    if show_ids {
        command.arg("--show-ids");
    }
    let rendered = command
        .output()
        .expect("run slate-parser filecheck renderer");
    std::fs::remove_file(&parsed_fixture).expect("remove fixture without FileCheck metadata");
    if source.contains(&self_include) {
        std::fs::remove_dir(&temp_dir).expect("remove isolated self-include fixture directory");
    }
    assert!(
        rendered.status.success(),
        "renderer failed for {}:\n{}",
        fixture.display(),
        String::from_utf8_lossy(&rendered.stderr)
    );

    if rendered.stdout.is_empty() {
        let raw_source = decode_source_bytes(&std::fs::read(fixture).expect("read fixture"));
        let has_checks = raw_source.lines().any(|line| {
            line.starts_with(&format!("// {prefix}:"))
                || line.starts_with(&format!("// {prefix}-NEXT:"))
        });
        assert!(
            !has_checks,
            "renderer produced no output for {} ({prefix}) but fixture still has CHECK lines",
            fixture.display()
        );
        return;
    }

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
        assert_evaluated_matches_clang(fixture, defines, isystem, flavor);
    }
}

fn fixture_source(fixture: &Path) -> String {
    let source = decode_source_bytes(&std::fs::read(fixture).expect("read fixture"));
    let mut result = String::new();
    let mut in_checks = false;
    for line in source.lines() {
        if line.starts_with("// SLATE-FILECHECK-BEGIN ") {
            in_checks = true;
        }
        if !in_checks && !line.starts_with("// SLATE-FILECHECK-") {
            result.push_str(line);
            result.push('\n');
        }
        if line.starts_with("// SLATE-FILECHECK-END ") {
            in_checks = false;
        }
    }
    result
}

fn run_error_fixture(
    fixture: &Path,
    prefix: &str,
    defines: &[String],
    flavor: Option<&str>,
    standard: Option<&str>,
    slot: usize,
) {
    let file_name = fixture.file_name().unwrap().to_string_lossy();
    let parsed_name = format!(
        ".{}.filecheck.{}.{}.c",
        fixture.file_stem().unwrap().to_string_lossy(),
        std::process::id(),
        slot
    );
    let parsed_fixture = fixture.with_file_name(&parsed_name);
    std::fs::write(&parsed_fixture, fixture_source(fixture))
        .expect("write fixture without FileCheck metadata");
    let output = Command::new(env!("CARGO_BIN_EXE_slate-parser"))
        .arg("parse")
        .arg(&parsed_fixture)
        .args(
            defines
                .iter()
                .map(|define| format!("-D{}", define.trim_start_matches("-D"))),
        )
        .args(flavor.map(|flavor| format!("--flavor={flavor}")))
        .args(standard.map(|standard| format!("-std={standard}")))
        .env_remove("FORCE_COLOR")
        .env_remove("CLICOLOR_FORCE")
        .env("NO_COLOR", "1")
        .output()
        .expect("run slate-parser failing fixture");
    std::fs::remove_file(&parsed_fixture).expect("remove fixture without FileCheck metadata");
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
    let diagnostic = String::from_utf8_lossy(&output.stderr).replace(&parsed_name, &file_name);
    std::fs::write(&input, diagnostic).expect("write diagnostic");
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

fn assert_evaluated_matches_clang(
    fixture: &Path,
    defines: &[String],
    isystem: &[String],
    flavor: Option<&str>,
) {
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
    let mut parser = Parser::new(search)
        .with_defines(
            defines
                .iter()
                .map(|define| define.trim_start_matches("-D").to_string()),
        )
        .with_flavor(flavor.map_or_else(CompilerFlavor::default, |name| {
            name.parse().expect("valid fixture flavor")
        }));
    let (ast, _) = parser.parse_file(fixture).expect("parse fixture");
    let ours = summarize_evaluated(&ast);
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

fn summarize_evaluated(tu: &TranslationUnit) -> Vec<DeclSummary> {
    TAGS.with_borrow_mut(|tags| {
        *tags = tu
            .tags
            .iter()
            .map(|tag| (tag.value.id, tag.value.kind, tag.value.name.clone()))
            .collect();
    });
    tu.decls
        .iter()
        .flat_map(|decl| summarize_evaluated_decl(&decl.value))
        .collect()
}

fn summarize_evaluated_decl(decl: &Decl) -> Vec<DeclSummary> {
    match decl {
        Decl::Comment(_) | Decl::StaticAssert { .. } | Decl::Asm { .. } => Vec::new(),
        Decl::Function(function) => vec![DeclSummary::Function {
            name: declarator_identifier(&function.declarator),
            returns: function
                .body
                .iter()
                .filter_map(|stmt| match &stmt.value {
                    Stmt::Return(expression) => match &expression.value {
                        ExprKind::IntegerLiteral(_) => {
                            Some(ConstExprParser::evaluate_ast(expression).unwrap())
                        }
                        ExprKind::StatementExpression(_) => None,
                        _ => panic!("clang return was not an integer"),
                    },
                    Stmt::ReturnVoid => None,
                    Stmt::Comment(_)
                    | Stmt::Expr(_)
                    | Stmt::Decl(_)
                    | Stmt::StaticAssert(_)
                    | Stmt::Attribute(_)
                    | Stmt::Block(_)
                    | Stmt::If { .. }
                    | Stmt::While { .. }
                    | Stmt::DoWhile { .. }
                    | Stmt::For { .. }
                    | Stmt::Switch { .. }
                    | Stmt::SwitchLabel { .. }
                    | Stmt::Labeled { .. }
                    | Stmt::LocalLabelDecl(_)
                    | Stmt::Asm(_)
                    | Stmt::Goto(_)
                    | Stmt::ComputedGoto(_)
                    | Stmt::NestedFunction(_)
                    | Stmt::Break
                    | Stmt::Continue => None,
                })
                .collect(),
            signature: None,
        }],
        Decl::Declaration { declaration, .. } => {
            let mut summaries = Vec::new();
            if let CType::Tag(TagSpecifier::Definition(id)) = &declaration.specifiers.ty {
                let (kind, name) = defined_tag(*id);
                let name = name.unwrap_or_else(|| "<anonymous>".into());
                summaries.push(if kind == TagKind::Enum {
                    DeclSummary::Enum { name }
                } else {
                    DeclSummary::Record {
                        kind: tag_name(kind).into(),
                        name,
                    }
                });
            }
            if declaration.specifiers.attributes.is_empty() {
                summaries.extend(
                    declaration
                        .declarators
                        .iter()
                        .filter(|declarator| declarator.attributes.is_empty())
                        .map(|declarator| {
                            summarize_declarator(&declaration.specifiers, &declarator.declarator)
                        }),
                );
            }
            summaries
        }
    }
}

fn summarize_declarator(
    specifiers: &DeclarationSpecifiers,
    declarator: &Declarator,
) -> DeclSummary {
    let name = declarator_identifier(declarator);
    if specifiers.storage == StorageClass::Typedef {
        let base = if specifiers.qualifiers == Qualifiers::default() {
            specifiers.ty.clone()
        } else {
            CType::Qualified {
                qualifiers: specifiers.qualifiers,
                ty: Box::new(specifiers.ty.clone()),
            }
        };
        let ty = apply_abstract_declarator(base, declarator.clone());
        DeclSummary::Typedef {
            name,
            type_name: normalize_type(&type_spelling(&ty)),
        }
    } else if matches!(declarator, Declarator::Function { .. }) {
        DeclSummary::Function {
            name,
            returns: vec![],
            signature: Some(function_facts(&specifiers.ty, declarator)),
        }
    } else {
        DeclSummary::Object {
            name,
            type_facts: object_facts(specifiers, declarator),
        }
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
        CType::TypeOfUnqual(TypeOfOperand::Expression(expression)) => {
            format!("typeof_unqual({expression})")
        }
        CType::TypeOfUnqual(TypeOfOperand::Type(ty)) => {
            format!("typeof_unqual({})", type_spelling(ty))
        }
        CType::Imaginary(element) => format!("_Imaginary {}", type_spelling(element)),
        CType::TargetBuiltin(name) => name.clone(),
        CType::Named(name) => name.clone(),
        CType::Tag(TagSpecifier::Reference { kind, name }) => {
            format!("{} {name}", tag_name(*kind))
        }
        CType::Tag(TagSpecifier::Definition(id)) => {
            let (kind, name) = defined_tag(*id);
            format!(
                "{} {}",
                tag_name(kind),
                name.as_deref().unwrap_or("<anonymous>")
            )
        }
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
    let Declarator::Function { inner, parameters } = declarator else {
        panic!("expected function declarator")
    };
    let variadic = parameters.is_variadic();
    let parameters = parameters.parameters();
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
    while let Declarator::Array { inner, size, .. } = current {
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
        | Declarator::Attributed { inner, .. }
        | Declarator::Pointer { inner, .. }
        | Declarator::Array { inner, .. }
        | Declarator::Function { inner, .. } => declarator_identifier(inner),
    }
}

fn array_size(size: &ArraySize) -> String {
    match size {
        ArraySize::Unspecified => "".into(),
        ArraySize::Star => "*".into(),
        ArraySize::Expression(expression) => ConstExprParser::evaluate_ast(expression)
            .unwrap_or_else(|error| panic!("array bound was not constant: {error}"))
            .to_string(),
    }
}

fn defined_tag(id: TagId) -> (TagKind, Option<String>) {
    TAGS.with_borrow(|tags| {
        tags.iter()
            .find(|(tag_id, _, _)| *tag_id == id)
            .map(|(_, kind, name)| (*kind, name.clone()))
            .expect("tag definition is in the translation unit")
    })
}

fn tag_name(kind: TagKind) -> &'static str {
    match kind {
        TagKind::Struct => "struct",
        TagKind::Union => "union",
        TagKind::Enum => "enum",
    }
}

fn collect_c_fixtures(dir: &Path, out: &mut Vec<PathBuf>) {
    for entry in std::fs::read_dir(dir).expect("read fixture directory") {
        let path = entry.expect("read fixture entry").path();
        if path.is_dir() {
            collect_c_fixtures(&path, out);
        } else if path.extension().and_then(|extension| extension.to_str()) == Some("c") {
            out.push(path);
        }
    }
}

#[test]
fn fixtures_are_filechecked() {
    let mut fixtures = Vec::new();
    collect_c_fixtures(&fixtures_dir(), &mut fixtures);
    fixtures.sort();
    assert!(!fixtures.is_empty(), "no C fixtures found");

    let mut jobs = Vec::new();
    for fixture in fixtures {
        let source = decode_source_bytes(&std::fs::read(&fixture).expect("read fixture"));
        let configs = configurations(&source);
        let errors = error_configurations(&source);
        let flavor = flavor(&source);
        if !errors.is_empty() {
            for (slot, prefix) in errors.iter().enumerate() {
                let defines = configs
                    .iter()
                    .find(|(name, _)| name == prefix)
                    .map_or(&[][..], |(_, defines)| defines.as_slice());
                jobs.push(FixtureJob {
                    fixture: fixture.clone(),
                    prefix: prefix.clone(),
                    defines: defines.to_vec(),
                    isystem: Vec::new(),
                    flavor: flavor.clone(),
                    standard: std_for_prefix(&source, prefix),
                    show_ids: show_ids_for_prefix(&source, prefix),
                    error: true,
                    slot,
                });
            }
            continue;
        }
        assert!(
            !configs.is_empty(),
            "fixture has no FileCheck configurations: {}",
            fixture.display()
        );
        let isystem = isystem_paths(&source);
        for (slot, (prefix, defines)) in configs.iter().enumerate() {
            jobs.push(FixtureJob {
                fixture: fixture.clone(),
                prefix: prefix.clone(),
                defines: defines.clone(),
                isystem: isystem.clone(),
                flavor: flavor.clone(),
                standard: std_for_prefix(&source, prefix),
                show_ids: show_ids_for_prefix(&source, prefix),
                error: false,
                slot,
            });
        }
    }
    assert!(!jobs.is_empty(), "no FileCheck configurations found");

    let workers = jobs.len().min(6);
    let chunk_size = jobs.len().div_ceil(workers);
    std::thread::scope(|scope| {
        let handles = jobs
            .chunks(chunk_size)
            .map(|jobs| {
                scope.spawn(move || {
                    jobs.iter().for_each(|job| {
                        run_job(FixtureJob {
                            fixture: job.fixture.clone(),
                            prefix: job.prefix.clone(),
                            defines: job.defines.clone(),
                            isystem: job.isystem.clone(),
                            flavor: job.flavor.clone(),
                            standard: job.standard.clone(),
                            show_ids: job.show_ids,
                            error: job.error,
                            slot: job.slot,
                        })
                    })
                })
            })
            .collect::<Vec<_>>();
        for handle in handles {
            if let Err(payload) = handle.join() {
                std::panic::resume_unwind(payload);
            }
        }
    });
}
