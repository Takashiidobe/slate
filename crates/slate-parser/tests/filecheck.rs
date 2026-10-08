#[path = "filecheck/matcher.rs"]
mod matcher;

use clang_ast::Node;
use serde::Deserialize;
use slate_parser::ast::*;
use slate_parser::compiler_args::CompilerFlavor;
use slate_parser::compiler_headers;
use slate_parser::const_expr::Parser as ConstExprParser;
use slate_parser::dialect::Dialect;
use slate_parser::files::{SearchPaths, decode_source_bytes};
use slate_parser::parser::Parser;
use slate_parser::sysroot;
use slate_parser::target_info::{TargetInfo, TargetOs};
use std::ffi::OsString;
use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::Command;
use tempfile::{NamedTempFile, TempDir};

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

fn prefix_args(source: &str, prefix: &str) -> Vec<String> {
    source
        .lines()
        .filter_map(|line| line.trim().strip_prefix("// SLATE-FILECHECK-PREFIX-ARGS "))
        .filter_map(|rest| rest.split_once(char::is_whitespace))
        .filter(|(name, _)| *name == prefix)
        .flat_map(|(_, args)| args.split_whitespace())
        .map(str::to_string)
        .collect()
}

const OS_DIRECTORIES: [(&str, TargetOs); 5] = [
    ("linux", TargetOs::Linux),
    ("windows", TargetOs::Windows),
    ("darwin", TargetOs::Darwin),
    ("android", TargetOs::Android),
    ("freebsd", TargetOs::FreeBsd),
];

const CANONICAL_TRIPLES: [(&str, &str, &str); 12] = [
    ("linux", "x86_64", "x86_64-unknown-linux-gnu"),
    ("linux", "i686", "i686-unknown-linux-gnu"),
    ("linux", "aarch64", "aarch64-unknown-linux-gnu"),
    ("windows", "x86_64", "x86_64-pc-windows-msvc"),
    ("windows", "i686", "i686-pc-windows-msvc"),
    ("windows", "aarch64", "aarch64-pc-windows-msvc"),
    ("darwin", "x86_64", "x86_64-apple-darwin"),
    ("darwin", "aarch64", "aarch64-apple-darwin"),
    ("android", "x86_64", "x86_64-linux-android"),
    ("android", "aarch64", "aarch64-linux-android"),
    ("freebsd", "x86_64", "x86_64-unknown-freebsd"),
    ("freebsd", "aarch64", "aarch64-unknown-freebsd"),
];

struct Placement {
    flavor: String,
    triple: String,
}

fn fixture_placement(fixture: &Path) -> Placement {
    let relative = fixture
        .strip_prefix(fixtures_dir())
        .unwrap_or_else(|_| panic!("{} is outside tests/fixtures", fixture.display()));
    let mut dirs: Vec<&str> = relative
        .parent()
        .into_iter()
        .flat_map(Path::iter)
        .map(|part| part.to_str().expect("fixture path is UTF-8"))
        .collect();
    match dirs.as_slice() {
        ["error", ..] => {
            dirs.remove(0);
        }
        ["suites", _, ..] => {
            dirs.drain(..2);
        }
        _ => {}
    }
    let placement_error = |reason: &str| -> ! {
        panic!(
            "{}: {reason}; expected [error/ | suites/<name>/]<gcc|clang|msvc>/[<os>/[<arch> | <triple>]]/",
            fixture.display()
        )
    };
    let (flavor, rest) = dirs
        .split_first()
        .unwrap_or_else(|| placement_error("no compiler directory"));
    let compiler: CompilerFlavor = flavor
        .parse()
        .unwrap_or_else(|_| placement_error(&format!("`{flavor}` is not a compiler")));
    let os = rest.first().copied().unwrap_or(match compiler {
        CompilerFlavor::Msvc => "windows",
        CompilerFlavor::Gcc | CompilerFlavor::Clang => "linux",
    });
    let target_os = OS_DIRECTORIES
        .iter()
        .find(|(name, _)| *name == os)
        .map(|(_, target_os)| *target_os)
        .unwrap_or_else(|| placement_error(&format!("`{os}` is not an OS directory")));
    let canonical = |arch: &str| {
        CANONICAL_TRIPLES
            .iter()
            .find(|(name, candidate, _)| *name == os && *candidate == arch)
            .map(|(_, _, triple)| triple.to_string())
    };
    let triple = match rest {
        [] | [_] => canonical("x86_64")
            .unwrap_or_else(|| placement_error(&format!("`{os}` has no default arch"))),
        [_, leaf] => canonical(leaf)
            .or_else(|| {
                TargetInfo::for_triple(leaf)
                    .is_ok_and(|target| target.os == target_os)
                    .then(|| leaf.to_string())
            })
            .unwrap_or_else(|| placement_error(&format!("`{leaf}` is not an arch or {os} triple"))),
        _ => placement_error("too many directories"),
    };
    Placement {
        flavor: flavor.to_string(),
        triple,
    }
}

// invalid values stay allowed so option-parsing errors remain testable
fn check_directive_placement(fixture: &Path, source: &str, placement: &Placement) {
    let args: Vec<&str> = source
        .lines()
        .filter_map(|line| {
            let line = line.trim();
            line.strip_prefix("// SLATE-FILECHECK-ARGS ").or_else(|| {
                line.strip_prefix("// SLATE-FILECHECK-PREFIX-ARGS ")
                    .and_then(|rest| rest.split_once(char::is_whitespace))
                    .map(|(_, args)| args)
            })
        })
        .flat_map(str::split_whitespace)
        .collect();
    for (index, arg) in args.iter().enumerate() {
        let option = arg.trim_start_matches('-');
        let (name, value) = match option.split_once('=') {
            Some((name, value)) => (name, Some(value)),
            None => (option, args.get(index + 1).copied()),
        };
        let Some(value) = value else { continue };
        let conflicts = match name {
            "target" => {
                TargetInfo::for_triple(value).is_ok_and(|target| target.triple != placement.triple)
            }
            "flavor" => value.parse::<CompilerFlavor>().is_ok() && value != placement.flavor,
            _ => false,
        };
        assert!(
            !conflicts,
            "{}: directive `{arg}` disagrees with the directory placement ({} {})",
            fixture.display(),
            placement.flavor,
            placement.triple
        );
    }
}

fn fixture_args(fixture: &Path) -> Vec<String> {
    let placement = fixture_placement(fixture);
    let mut args = vec![
        format!("--flavor={}", placement.flavor),
        format!("--target={}", placement.triple),
    ];
    args.extend(
        decode_source_bytes(&std::fs::read(fixture).expect("read fixture arguments"))
            .lines()
            .filter_map(|line| line.trim().strip_prefix("// SLATE-FILECHECK-ARGS "))
            .flat_map(str::split_whitespace)
            .map(str::to_string),
    );
    args
}

fn warning_configurations(source: &str) -> Vec<String> {
    source
        .lines()
        .filter_map(|line| {
            line.trim()
                .strip_prefix("// SLATE-FILECHECK-WARNING ")
                .map(str::to_string)
        })
        .collect()
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

fn ir_error_configurations(source: &str) -> Vec<String> {
    source
        .lines()
        .filter_map(|line| {
            line.trim()
                .strip_prefix("// SLATE-FILECHECK-IR-ERROR ")
                .map(str::to_string)
        })
        .collect()
}

// the copy sits beside the original so relative includes still resolve
enum Scratch {
    Beside(NamedTempFile),
    Isolated(TempDir, OsString),
}

impl Scratch {
    fn new(fixture: &Path, source: &str) -> Self {
        let name = fixture.file_name().unwrap();
        if source.contains(&format!("#include \"{}\"", name.to_string_lossy())) {
            let directory =
                tempfile::tempdir().expect("create isolated self-include fixture directory");
            std::fs::write(directory.path().join(name), source)
                .expect("write fixture without FileCheck metadata");
            return Self::Isolated(directory, name.to_owned());
        }
        let mut file = tempfile::Builder::new()
            .prefix(&format!(
                ".{}.filecheck.",
                fixture.file_stem().unwrap().to_string_lossy()
            ))
            .suffix(".c")
            .tempfile_in(fixture.parent().unwrap())
            .expect("create fixture without FileCheck metadata");
        file.write_all(source.as_bytes())
            .expect("write fixture without FileCheck metadata");
        Self::Beside(file)
    }

    fn path(&self) -> PathBuf {
        match self {
            Self::Beside(file) => file.path().to_path_buf(),
            Self::Isolated(directory, name) => directory.path().join(name),
        }
    }

    fn name(&self) -> String {
        match self {
            Self::Beside(file) => file.path().file_name().unwrap().to_string_lossy().into(),
            Self::Isolated(_, name) => name.to_string_lossy().into(),
        }
    }
}

struct FixtureJob {
    fixture: PathBuf,
    prefix: String,
    defines: Vec<String>,
    isystem: Vec<String>,
    standard: Option<String>,
    show_ids: bool,
    error: bool,
    ir_error: bool,
    warnings: bool,
}

fn run_job(job: FixtureJob) {
    if job.error {
        run_error_fixture(
            &job.fixture,
            &job.prefix,
            &job.defines,
            &job.isystem,
            job.standard.as_deref(),
            job.show_ids,
        );
    } else if job.ir_error {
        run_ir_error_fixture(
            &job.fixture,
            &job.prefix,
            &job.defines,
            &job.isystem,
            job.standard.as_deref(),
            job.show_ids,
        );
    } else {
        run_fixture(
            &job.fixture,
            &job.prefix,
            &job.defines,
            &job.isystem,
            job.standard.as_deref(),
            job.show_ids,
            job.warnings,
        );
    }
}

fn run_fixture(
    fixture: &Path,
    prefix: &str,
    defines: &[String],
    isystem: &[String],
    standard: Option<&str>,
    show_ids: bool,
    warnings: bool,
) {
    let source = fixture_source(fixture);
    let parsed = Scratch::new(fixture, &source);
    let original = decode_source_bytes(&std::fs::read(fixture).expect("read fixture renderer"));
    let example = original
        .lines()
        .find_map(|line| line.trim().strip_prefix("// SLATE-FILECHECK-EXAMPLE "));
    let mut command = if let Some(example) = example {
        let mut command = Command::new("cargo");
        command.current_dir(env!("CARGO_MANIFEST_DIR")).args([
            "run",
            "--quiet",
            "--example",
            example.trim(),
            "--",
        ]);
        command
    } else {
        let mut command = Command::new(env!("CARGO_BIN_EXE_slate-parser"));
        command.arg("parse").arg(parsed.path());
        command
    };
    command
        .env_remove("FORCE_COLOR")
        .env_remove("CLICOLOR_FORCE")
        .env("NO_COLOR", "1");
    for define in defines {
        command.arg(format!("-D{}", define.trim_start_matches("-D")));
    }
    for path in isystem {
        command.arg(format!("-isystem{path}"));
    }
    if let Some(standard) = standard {
        command.arg(format!("-std={standard}"));
    }
    if show_ids {
        command.arg("--show-ids");
    }
    command.args(fixture_ir_args(fixture));
    command.args(prefix_args(&original, prefix));
    let rendered = command
        .output()
        .expect("run slate-parser filecheck renderer");
    assert!(
        rendered.status.success(),
        "renderer failed for {}:\n{}",
        fixture.display(),
        String::from_utf8_lossy(&rendered.stderr)
    );

    let checked = if warnings {
        String::from_utf8_lossy(&rendered.stderr)
            .replace(
                &parsed.name(),
                fixture.file_name().unwrap().to_str().unwrap(),
            )
            .into_bytes()
    } else {
        rendered.stdout.clone()
    };
    if checked.is_empty() {
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

    check_output(fixture, prefix, &checked);
    if warnings {
        check_output(fixture, &format!("IR-{prefix}"), &rendered.stdout);
    }

    if std::env::var_os("SLATE_CLANG_ORACLE").is_some() {
        assert_evaluated_matches_clang(fixture, defines, isystem);
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

fn fixture_ir_args(fixture: &Path) -> Vec<String> {
    let mut args = fixture_args(fixture);
    let source = decode_source_bytes(&std::fs::read(fixture).expect("read fixture"));
    let ast_only = source.lines().any(|line| line == "// SLATE-FILECHECK-AST");
    if !ast_only && !args.iter().any(|arg| arg.starts_with("--dump-ir")) {
        args.push("--dump-ir".into());
    }
    args
}

fn run_error_fixture(
    fixture: &Path,
    prefix: &str,
    defines: &[String],
    isystem: &[String],
    standard: Option<&str>,
    show_ids: bool,
) {
    run_expected_failure_fixture(fixture, prefix, defines, isystem, standard, show_ids, false);
}

fn run_ir_error_fixture(
    fixture: &Path,
    prefix: &str,
    defines: &[String],
    isystem: &[String],
    standard: Option<&str>,
    show_ids: bool,
) {
    run_expected_failure_fixture(fixture, prefix, defines, isystem, standard, show_ids, true);
}

fn run_expected_failure_fixture(
    fixture: &Path,
    prefix: &str,
    defines: &[String],
    isystem: &[String],
    standard: Option<&str>,
    show_ids: bool,
    ir: bool,
) {
    let file_name = fixture.file_name().unwrap().to_string_lossy();
    let parsed = Scratch::new(fixture, &fixture_source(fixture));
    let mut command = Command::new(env!("CARGO_BIN_EXE_slate-parser"));
    command.arg("parse").arg(parsed.path());
    for define in defines {
        command.arg(format!("-D{}", define.trim_start_matches("-D")));
    }
    for path in isystem {
        command.arg(format!("-isystem{path}"));
    }
    if let Some(standard) = standard {
        command.arg(format!("-std={standard}"));
    }
    if show_ids {
        command.arg("--show-ids");
    }
    let args = if ir {
        fixture_ir_args(fixture)
    } else {
        fixture_args(fixture)
    };
    let original = decode_source_bytes(&std::fs::read(fixture).expect("read failing fixture"));
    let output = command
        .args(args)
        .args(prefix_args(&original, prefix))
        .env_remove("FORCE_COLOR")
        .env_remove("CLICOLOR_FORCE")
        .env("NO_COLOR", "1")
        .output()
        .expect("run slate-parser failing fixture");
    assert!(
        !output.status.success(),
        "fixture unexpectedly succeeded: {}",
        fixture.display()
    );

    let diagnostic = String::from_utf8_lossy(&output.stderr).replace(&parsed.name(), &file_name);
    check_output(fixture, prefix, diagnostic.as_bytes());
}

/// Checks rendered output with the harness matcher, and runs FileCheck only
/// on a mismatch, for its annotated input dump.
fn check_output(fixture: &Path, prefix: &str, output: &[u8]) {
    let checks = decode_source_bytes(&std::fs::read(fixture).expect("read fixture checks"));
    let Err(error) = matcher::check(&checks, prefix, &String::from_utf8_lossy(output)) else {
        return;
    };
    let work = tempfile::tempdir().expect("create FileCheck work directory");
    let input = work.path().join("rendered.txt");
    std::fs::write(&input, output).expect("write rendered output");
    let filecheck_report = Command::new(filecheck())
        .arg(fixture)
        .arg(format!("--check-prefix={prefix}"))
        .arg("--input-file")
        .arg(&input)
        .arg("--dump-input=fail")
        .output()
        .map(|result| {
            format!(
                "{}{}",
                String::from_utf8_lossy(&result.stdout),
                String::from_utf8_lossy(&result.stderr)
            )
        })
        .unwrap_or_else(|error| format!("could not run FileCheck: {error}"));
    panic!(
        "check failed for {} ({prefix}), input kept at {}:\n{error}\n\nFileCheck:\n{filecheck_report}",
        fixture.display(),
        work.keep().join("rendered.txt").display(),
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
    let placement = fixture_placement(fixture);
    let flavor: CompilerFlavor = placement.flavor.parse().expect("valid fixture flavor");
    let target = TargetInfo::for_triple(&placement.triple).expect("valid fixture target");
    let mut system: Vec<PathBuf> = isystem.iter().map(PathBuf::from).collect();
    system.extend(compiler_headers::include_paths(&target, flavor));
    system.extend(sysroot::include_paths(&target, flavor));
    let search = SearchPaths {
        system,
        ..SearchPaths::default()
    };
    let mut parser = Parser::new(search, Dialect::for_flavor(flavor, target)).with_defines(
        defines
            .iter()
            .map(|define| define.trim_start_matches("-D").to_string()),
    );
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
    let target =
        TargetInfo::for_triple(&fixture_placement(fixture).triple).expect("valid fixture target");
    command.arg(format!("--target={}", target.triple));
    if let Some(resource_include) = compiler_headers::include_paths(&target, CompilerFlavor::Clang)
        .into_iter()
        .next()
        && let Some(resource_dir) = resource_include.parent()
    {
        command.arg("-resource-dir").arg(resource_dir);
    }
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
    for path in sysroot::include_paths(&target, CompilerFlavor::Clang) {
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

fn summarize_evaluated_decl(decl: &DeclKind) -> Vec<DeclSummary> {
    match decl {
        DeclKind::Comment(_)
        | DeclKind::StaticAssert { .. }
        | DeclKind::Asm { .. }
        | DeclKind::Pragma(_)
        | DeclKind::Attribute(_) => Vec::new(),
        DeclKind::Function(function) => vec![DeclSummary::Function {
            name: declarator_identifier(&function.declarator),
            returns: function
                .body
                .iter()
                .filter_map(|stmt| match &stmt.value {
                    StmtKind::Return(expression) => match &expression.value {
                        ExprKind::IntegerLiteral(_) => {
                            Some(ConstExprParser::evaluate_ast(expression).unwrap())
                        }
                        ExprKind::StatementExpression(_) => None,
                        _ => panic!("clang return was not an integer"),
                    },
                    StmtKind::ReturnVoid => None,
                    StmtKind::Comment(_)
                    | StmtKind::Expr(_)
                    | StmtKind::Decl(_)
                    | StmtKind::StaticAssert(_)
                    | StmtKind::Attribute(_)
                    | StmtKind::Attributed { .. }
                    | StmtKind::Block(_)
                    | StmtKind::Null
                    | StmtKind::If { .. }
                    | StmtKind::While { .. }
                    | StmtKind::DoWhile { .. }
                    | StmtKind::For { .. }
                    | StmtKind::Switch { .. }
                    | StmtKind::SwitchLabel { .. }
                    | StmtKind::Labeled { .. }
                    | StmtKind::LocalLabelDecl(_)
                    | StmtKind::Asm(_)
                    | StmtKind::MsAsm(_)
                    | StmtKind::Pragma(_)
                    | StmtKind::Goto(_)
                    | StmtKind::ComputedGoto(_)
                    | StmtKind::NestedFunction(_)
                    | StmtKind::NamedBreak(_)
                    | StmtKind::NamedContinue(_)
                    | StmtKind::Break
                    | StmtKind::Continue => None,
                })
                .collect(),
            signature: None,
        }],
        DeclKind::Declaration(declaration) => {
            let mut summaries = Vec::new();
            if let TypeSpecifier::Tag(TagSpecifier::Definition(id)) = &declaration.specifiers.ty {
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
        DeclSummary::Typedef {
            name,
            type_name: normalize_type(&declarator_spelling(
                specifiers_spelling(specifiers),
                declarator,
            )),
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

fn type_spelling(ty: &TypeSpecifier) -> String {
    match ty {
        TypeSpecifier::Bool => "_Bool".into(),
        TypeSpecifier::Integer(IntegerType::Char { signed }) => match signed {
            None => "char".into(),
            Some(true) => "signed char".into(),
            Some(false) => "unsigned char".into(),
        },
        TypeSpecifier::Integer(IntegerType::Ranked { rank, signed }) => {
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
        TypeSpecifier::Integer(IntegerType::BitInt { width, signed }) => {
            format!("{} _BitInt({width})", if *signed { "" } else { "unsigned" })
        }
        TypeSpecifier::Void => "void".into(),
        TypeSpecifier::Floating(kind) => match kind {
            FloatingType::BFloat16 => "__bf16".into(),
            FloatingType::Float => "float".into(),
            FloatingType::Float16 => "_Float16".into(),
            FloatingType::Fp16 => "__fp16".into(),
            FloatingType::Float32 => "_Float32".into(),
            FloatingType::Float64 => "_Float64".into(),
            FloatingType::Float32x => "_Float32x".into(),
            FloatingType::Float64x => "_Float64x".into(),
            FloatingType::Double => "double".into(),
            FloatingType::LongDouble => "long double".into(),
            FloatingType::Float128 => "_Float128".into(),
            FloatingType::Float128Ext => "__float128".into(),
            FloatingType::Float80 => "__float80".into(),
            FloatingType::Decimal32 => "_Decimal32".into(),
            FloatingType::Decimal64 => "_Decimal64".into(),
            FloatingType::Decimal128 => "_Decimal128".into(),
        },
        TypeSpecifier::Complex(element) => format!("_Complex {}", type_spelling(element)),
        TypeSpecifier::Atomic(element) => format!("_Atomic({})", type_name_spelling(element)),
        TypeSpecifier::Vector(vector) => match &vector.size {
            VectorSize::Bytes(size) => {
                format!("{} vector_size({size})", type_spelling(&vector.element))
            }
            VectorSize::Lanes(size) => {
                format!("{} ext_vector_type({size})", type_spelling(&vector.element))
            }
        },
        TypeSpecifier::Mode(mode) => format!("{} mode({})", type_spelling(&mode.base), mode.mode),
        TypeSpecifier::FixedPoint(fixed) => format!(
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
        TypeSpecifier::TypeOf(TypeOfOperand::Expression(expression)) => {
            format!("typeof({expression})")
        }
        TypeSpecifier::TypeOf(TypeOfOperand::Type(ty)) => {
            format!("typeof({})", type_name_spelling(ty))
        }
        TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(expression)) => {
            format!("typeof_unqual({expression})")
        }
        TypeSpecifier::TypeOfUnqual(TypeOfOperand::Type(ty)) => {
            format!("typeof_unqual({})", type_name_spelling(ty))
        }
        TypeSpecifier::Imaginary(element) => format!("_Imaginary {}", type_spelling(element)),
        TypeSpecifier::TargetBuiltin(name) => name.clone(),
        TypeSpecifier::Inferred => "auto".to_owned(),
        TypeSpecifier::Named(name) => name.value.clone(),
        TypeSpecifier::Tag(TagSpecifier::Reference { kind, name, .. }) => {
            format!("{} {}", tag_name(*kind), name.value)
        }
        TypeSpecifier::Tag(TagSpecifier::Definition(id)) => {
            let (kind, name) = defined_tag(*id);
            format!(
                "{} {}",
                tag_name(kind),
                name.as_deref().unwrap_or("<anonymous>")
            )
        }
    }
}

fn function_facts(base: &TypeSpecifier, declarator: &Declarator) -> String {
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

fn parameter_fact(parameter: &ParameterDeclaration) -> String {
    let pointer = matches!(parameter.declarator, Declarator::Pointer { .. });
    format!(
        "{}{}",
        normalize_type(&specifiers_spelling(&parameter.specifiers)),
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
    let mut base = specifiers_spelling(specifiers);
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

fn specifiers_spelling(specifiers: &DeclarationSpecifiers) -> String {
    let base = type_spelling(&specifiers.ty);
    if specifiers.qualifiers.is_atomic {
        format!("_Atomic({base})")
    } else {
        format!("{}{base}", qualifier_spelling(specifiers.qualifiers))
    }
}

fn type_name_spelling(type_name: &TypeName) -> String {
    declarator_spelling(
        specifiers_spelling(&type_name.specifiers),
        &type_name.declarator,
    )
}

fn declarator_spelling(base: String, declarator: &Declarator) -> String {
    match declarator {
        Declarator::Name(_) | Declarator::Abstract => base,
        Declarator::Grouped(inner) | Declarator::Attributed { inner, .. } => {
            declarator_spelling(base, inner)
        }
        Declarator::Pointer { inner, .. } => declarator_spelling(format!("{base} *"), inner),
        Declarator::Array { inner, size, .. } => {
            let (base, core) = pointer_prefix_spelling(base, inner);
            declarator_spelling(format!("{base}[{}]", array_size(size)), core)
        }
        Declarator::Function { inner, .. } => {
            let (base, core) = pointer_prefix_spelling(base, inner);
            declarator_spelling(format!("{base} ()"), core)
        }
    }
}

fn pointer_prefix_spelling(mut base: String, mut declarator: &Declarator) -> (String, &Declarator) {
    while let Declarator::Pointer { inner, .. } = declarator {
        base.push_str(" *");
        declarator = inner;
    }
    (base, declarator)
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
        } else if path.extension().and_then(|extension| extension.to_str()) == Some("c")
            && !path.file_name().unwrap().to_string_lossy().starts_with('.')
        {
            out.push(path);
        }
    }
}

fn fixture_jobs(fixture: &Path, jobs: &mut Vec<FixtureJob>) {
    let source = decode_source_bytes(&std::fs::read(fixture).expect("read fixture"));
    let configs = configurations(&source);
    let errors = error_configurations(&source);
    let ir_errors = ir_error_configurations(&source);
    let warnings = warning_configurations(&source);
    check_directive_placement(fixture, &source, &fixture_placement(fixture));
    let isystem = isystem_paths(&source);
    let config_names = configs
        .iter()
        .map(|(prefix, _)| prefix.as_str())
        .collect::<std::collections::HashSet<_>>();
    if !errors.is_empty()
        && errors
            .iter()
            .any(|prefix| !config_names.contains(prefix.as_str()))
    {
        for prefix in &errors {
            let defines = configs
                .iter()
                .find(|(name, _)| name == prefix)
                .map_or(&[][..], |(_, defines)| defines.as_slice());
            jobs.push(FixtureJob {
                fixture: fixture.to_path_buf(),
                prefix: prefix.clone(),
                defines: defines.to_vec(),
                isystem: isystem.clone(),
                standard: std_for_prefix(&source, prefix),
                show_ids: show_ids_for_prefix(&source, prefix),
                error: true,
                ir_error: false,
                warnings: false,
            });
        }
        return;
    }
    assert!(
        !configs.is_empty(),
        "fixture has no FileCheck configurations: {}",
        fixture.display()
    );
    for (prefix, defines) in &configs {
        jobs.push(FixtureJob {
            fixture: fixture.to_path_buf(),
            prefix: prefix.clone(),
            defines: defines.clone(),
            isystem: isystem.clone(),
            standard: std_for_prefix(&source, prefix),
            show_ids: show_ids_for_prefix(&source, prefix),
            error: errors.contains(prefix),
            ir_error: ir_errors.contains(prefix),
            warnings: warnings.contains(prefix),
        });
    }
}

fn main() {
    let arguments = libtest_mimic::Arguments::from_args();
    let fixtures_root = fixtures_dir();
    let fixtures = match arguments.filter.as_deref() {
        Some(name) if arguments.exact => name
            .rsplit_once("::")
            .map(|(fixture, _)| vec![fixtures_root.join(fixture)])
            .unwrap_or_default(),
        _ => {
            let mut fixtures = Vec::new();
            collect_c_fixtures(&fixtures_root, &mut fixtures);
            fixtures.sort();
            assert!(!fixtures.is_empty(), "no C fixtures found");
            fixtures
        }
    };

    let mut jobs = Vec::new();
    for fixture in &fixtures {
        fixture_jobs(fixture, &mut jobs);
    }
    assert!(!jobs.is_empty(), "no FileCheck configurations found");

    let trials = jobs
        .into_iter()
        .map(|job| {
            let name = format!(
                "{}::{}",
                job.fixture.strip_prefix(&fixtures_root).unwrap().display(),
                job.prefix
            );
            libtest_mimic::Trial::test(name, move || {
                run_job(job);
                Ok(())
            })
        })
        .collect();
    libtest_mimic::run(&arguments, trials).exit();
}
