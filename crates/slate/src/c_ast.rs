use serde_json::Value;
use std::cell::RefCell;
use std::collections::{BTreeSet, HashMap};
use std::path::{Path, PathBuf};
use std::process::Command;

use crate::function_identity::{CallBinding, FunctionIdentity, Provenance, classify_function};

thread_local! {
    static TYPEDEFS: RefCell<HashMap<String, String>> = RefCell::new(HashMap::new());
    static ENUM_SIGNED: RefCell<HashMap<String, bool>> = RefCell::new(HashMap::new());
    static ENUM_TYPEDEFS: RefCell<HashMap<String, String>> = RefCell::new(HashMap::new());
    static CALL_FACTS: RefCell<HashMap<usize, CallFact>> = RefCell::new(HashMap::new());
}

#[derive(Debug, Default, Clone)]
pub struct Unit {
    pub enums: Vec<Enum>,
    pub records: Vec<Record>,
    pub anonymous_header_records: Vec<Record>,
    pub functions: Vec<Function>,
    pub weak_refs: Vec<WeakRefAttribute>,
    pub function_allocation_attributes: HashMap<String, FunctionAllocationAttributes>,
    call_bindings: HashMap<Loc, CallBinding>,
}

#[derive(Debug, Clone)]
pub struct Record {
    pub name: String,
    pub comments: Vec<String>,
    pub kind: RecordKind,
    pub fields: Vec<Decl>,
    pub packed: Option<u32>,
    pub align: Option<u32>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum RecordKind {
    Struct,
    Union,
}

#[derive(Debug, Clone)]
pub struct Enum {
    pub name: String,
    pub tag: Option<String>,
    pub comments: Vec<String>,
    pub variants: Vec<EnumVariant>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct EnumVariant {
    pub name: String,
    pub comments: Vec<String>,
    pub value: i64,
}

#[derive(Debug, Clone)]
pub struct Function {
    pub name: String,
    pub params: Vec<Decl>,
    pub ret: CType,
    pub body: Option<Vec<Stmt>>,
    pub loc: Option<Loc>,
    pub raw: Option<Value>,
    pub layout_queries: Vec<LayoutQuery>,
    pub nonnull_static_params: BTreeSet<usize>,
    pub macro_consts: Vec<MacroConst>,
    pub asm_gotos: Vec<AsmGoto>,
}

#[derive(Debug, Clone)]
pub struct MacroConst {
    pub name: String,
    pub loc: Loc,
}

#[derive(Debug, Clone)]
pub struct AsmGoto {
    pub labels: Vec<String>,
}

#[derive(Debug, Clone)]
pub struct WeakRefAttribute {
    pub name: String,
    pub target: String,
}

#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub struct FunctionAllocationAttributes {
    pub fresh: bool,
    pub size_arguments: Vec<usize>,
    pub alignment_argument: Option<usize>,
    pub assumed_alignment: Option<AssumedAlignmentAttribute>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct AssumedAlignmentAttribute {
    pub bytes: u64,
    pub offset: u64,
}

#[derive(Debug, Clone)]
pub struct Decl {
    pub name: String,
    pub comments: Vec<String>,
    pub ty: CType,
    pub bit_width: Option<u32>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum CType {
    Void,
    Bool,
    Int { signed: bool, bits: u32 },
    Float { bits: u32 },
    Ptr(Box<CType>),
    FuncPtr { ret: Box<CType>, params: Vec<CType> },
    Array(Box<CType>, Option<u64>),
    Record(String),
    Enum(String),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum LayoutQuery {
    Size(CType),
    Align(CType),
    Offset { record: String, field: String },
}

#[derive(Debug, Clone)]
pub enum Stmt {
    Decl(Decl, Option<Expr>),
    Expr(Expr),
    Return(Option<Expr>),
    For {
        init: Box<Option<Stmt>>,
        cond: Option<Expr>,
        step: Option<Expr>,
        body: Vec<Stmt>,
    },
    While {
        cond: Expr,
        body: Vec<Stmt>,
    },
    If {
        cond: Expr,
        then: Vec<Stmt>,
        otherwise: Option<Vec<Stmt>>,
    },
}

#[derive(Debug, Clone)]
pub enum Expr {
    Int(i64),
    Str(String),
    Ident(String),
    Unary {
        op: String,
        expr: Box<Expr>,
    },
    Binary {
        op: String,
        lhs: Box<Expr>,
        rhs: Box<Expr>,
    },
    Call {
        name: String,
        args: Vec<Expr>,
        binding: CallBinding,
        loc: Option<Loc>,
    },
    Assign {
        target: Box<Expr>,
        value: Box<Expr>,
    },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct Loc {
    pub line: u32,
    pub col: u32,
}

impl Unit {
    pub fn call_bindings(&self) -> HashMap<Loc, CallBinding> {
        let mut bindings = self.call_bindings.clone();
        for body in self
            .functions
            .iter()
            .filter_map(|function| function.body.as_deref())
        {
            collect_call_bindings_in_stmts(body, &mut bindings);
        }
        bindings
    }
}

fn collect_call_bindings_in_stmts(stmts: &[Stmt], out: &mut HashMap<Loc, CallBinding>) {
    for stmt in stmts {
        match stmt {
            Stmt::Decl(_, init) => init
                .iter()
                .for_each(|expr| collect_call_bindings_in_expr(expr, out)),
            Stmt::Expr(expr) => collect_call_bindings_in_expr(expr, out),
            Stmt::Return(expr) => expr
                .iter()
                .for_each(|expr| collect_call_bindings_in_expr(expr, out)),
            Stmt::For {
                init,
                cond,
                step,
                body,
            } => {
                init.as_ref().as_ref().iter().for_each(|stmt| {
                    collect_call_bindings_in_stmts(std::slice::from_ref(stmt), out)
                });
                cond.iter()
                    .for_each(|expr| collect_call_bindings_in_expr(expr, out));
                step.iter()
                    .for_each(|expr| collect_call_bindings_in_expr(expr, out));
                collect_call_bindings_in_stmts(body, out);
            }
            Stmt::While { cond, body } => {
                collect_call_bindings_in_expr(cond, out);
                collect_call_bindings_in_stmts(body, out);
            }
            Stmt::If {
                cond,
                then,
                otherwise,
            } => {
                collect_call_bindings_in_expr(cond, out);
                collect_call_bindings_in_stmts(then, out);
                if let Some(body) = otherwise {
                    collect_call_bindings_in_stmts(body, out);
                }
            }
        }
    }
}

fn collect_call_bindings_in_expr(expr: &Expr, out: &mut HashMap<Loc, CallBinding>) {
    match expr {
        Expr::Call {
            args, binding, loc, ..
        } => {
            if let Some(loc) = loc {
                out.insert(*loc, binding.clone());
            }
            args.iter()
                .for_each(|arg| collect_call_bindings_in_expr(arg, out));
        }
        Expr::Unary { expr, .. } => collect_call_bindings_in_expr(expr, out),
        Expr::Binary { lhs, rhs, .. } => {
            collect_call_bindings_in_expr(lhs, out);
            collect_call_bindings_in_expr(rhs, out);
        }
        Expr::Assign { target, value } => {
            collect_call_bindings_in_expr(target, out);
            collect_call_bindings_in_expr(value, out);
        }
        Expr::Int(_) | Expr::Str(_) | Expr::Ident(_) => {}
    }
}

#[derive(Debug, Clone)]
struct CallFact {
    name: String,
    binding: CallBinding,
    loc: Option<Loc>,
}

#[derive(Debug, Default)]
struct PluginEvents {
    macros: HashMap<usize, MacroExpansion>,
    calls: HashMap<usize, CallFact>,
    pack_attributes: Vec<PackAttribute>,
}

#[derive(Debug)]
struct MacroExpansion {
    name: String,
    headers: BTreeSet<String>,
}

#[derive(Debug)]
struct PackAttribute {
    name: String,
    file: String,
    offset: usize,
    alignment: u32,
}

fn clang() -> String {
    std::env::var("SLATE_CLANG").unwrap_or_else(|_| {
        format!(
            "{}/llvm-project/build-cir/bin/clang",
            std::env::var("HOME").expect("HOME not set")
        )
    })
}

fn macro_dump_plugin() -> String {
    std::env::var("SLATE_MACRO_DUMP_PLUGIN").unwrap_or_else(|_| {
        let clang = PathBuf::from(clang());
        clang
            .parent()
            .and_then(Path::parent)
            .filter(|path| !path.as_os_str().is_empty())
            .map(|path| path.join("lib/SlateMacroDump.so"))
            .unwrap_or_else(|| {
                PathBuf::from(std::env::var("HOME").expect("HOME not set"))
                    .join("llvm-project/build-cir/lib/SlateMacroDump.so")
            })
            .to_string_lossy()
            .into_owned()
    })
}

fn parse_plugin_events(stderr: &str) -> PluginEvents {
    let mut out = PluginEvents::default();
    for line in stderr.lines() {
        let (kind, json) = if let Some(json) = line.strip_prefix("MACRO_EXPANSION ") {
            ("macro", json)
        } else if let Some(json) = line.strip_prefix("FUNCTION_PROVENANCE ") {
            ("function", json)
        } else if let Some(json) = line.strip_prefix("RECORD_PACKING ") {
            ("record", json)
        } else {
            continue;
        };
        let Ok(event) = serde_json::from_str::<Value>(json) else {
            continue;
        };
        let Some(offset) = event.get("offset").and_then(Value::as_u64) else {
            continue;
        };
        if kind == "record" {
            let (Some(name), Some(file), Some(alignment_bits)) = (
                event.get("name").and_then(Value::as_str),
                event.get("file").and_then(Value::as_str),
                event.get("alignment_bits").and_then(Value::as_u64),
            ) else {
                continue;
            };
            let Ok(alignment) = u32::try_from(alignment_bits / 8) else {
                continue;
            };
            if alignment == 0 || alignment_bits % 8 != 0 {
                continue;
            }
            out.pack_attributes.push(PackAttribute {
                name: name.to_string(),
                file: file.to_string(),
                offset: offset as usize,
                alignment,
            });
            continue;
        }
        let Some(name) = event.get("name").and_then(Value::as_str) else {
            continue;
        };
        if kind == "macro" {
            let headers = event
                .get("headers")
                .and_then(Value::as_array)
                .into_iter()
                .flatten()
                .filter_map(Value::as_str)
                .map(str::to_string)
                .collect();
            out.macros.insert(
                offset as usize,
                MacroExpansion {
                    name: name.to_string(),
                    headers,
                },
            );
            continue;
        }
        let canonical_type = event
            .get("canonical_type")
            .and_then(Value::as_str)
            .map(str::to_string);
        let binding = if event.get("direct").and_then(Value::as_bool) == Some(false) {
            CallBinding::Indirect
        } else {
            let provenance = match event.get("provenance").and_then(Value::as_str) {
                Some("trusted_header") => Provenance::TrustedHeader,
                _ => Provenance::Unknown,
            };
            let headers = event
                .get("headers")
                .and_then(Value::as_array)
                .into_iter()
                .flatten()
                .filter_map(Value::as_str);
            let identity = canonical_type
                .as_deref()
                .map_or(FunctionIdentity::Unknown, |ty| {
                    classify_function(name, headers, ty, provenance)
                });
            CallBinding::Direct {
                identity,
                canonical_type,
            }
        };
        out.calls.insert(
            offset as usize,
            CallFact {
                name: name.to_string(),
                binding,
                loc: None,
            },
        );
    }
    out
}

fn run_clang_ast_dump(src: &Path, extra_args: &[String]) -> Result<(String, PluginEvents), String> {
    let out = Command::new(clang())
        .args([
            "-Xclang",
            "-ast-dump=json",
            "-fsyntax-only",
            "-fparse-all-comments",
        ])
        .arg(format!("-fplugin={}", macro_dump_plugin()))
        .args(crate::cir::emit::target_args())
        .args(extra_args)
        .arg(src)
        .output()
        .map_err(|e| format!("spawn {}: {e}", clang()))?;
    if !out.status.success() {
        return Err(format!(
            "clang -ast-dump=json failed:\n{}",
            String::from_utf8_lossy(&out.stderr)
        ));
    }
    let plugin_events = parse_plugin_events(&String::from_utf8_lossy(&out.stderr));
    Ok((
        String::from_utf8_lossy(&out.stdout).into_owned(),
        plugin_events,
    ))
}

pub fn parse_file(src: &Path) -> Result<Unit, String> {
    parse_file_with_args(src, &[])
}

pub fn parse_file_with_args(src: &Path, extra_args: &[String]) -> Result<Unit, String> {
    let (json, plugin_events) = run_clang_ast_dump(src, extra_args)?;
    parse_json_with_record_roots(&json, &src.to_string_lossy(), &[], plugin_events)
}

pub fn parse_file_with_project_records(src: &Path, project_root: &Path) -> Result<Unit, String> {
    let project_root = project_root
        .canonicalize()
        .map_err(|e| format!("canonicalize {}: {e}", project_root.display()))?;
    let (json, plugin_events) = run_clang_ast_dump(src, &[])?;
    parse_json_with_record_roots(
        &json,
        &src.to_string_lossy(),
        &[project_root],
        plugin_events,
    )
}

pub fn parse(src: &str) -> Result<Unit, String> {
    parse_json(src, "")
}

pub fn parse_json(json: &str, source_file: &str) -> Result<Unit, String> {
    parse_json_with_record_roots(json, source_file, &[], PluginEvents::default())
}

fn parse_json_with_record_roots(
    json: &str,
    source_file: &str,
    record_roots: &[PathBuf],
    mut plugin_events: PluginEvents,
) -> Result<Unit, String> {
    let root: Value =
        serde_json::from_str(json).map_err(|e| format!("parse clang AST JSON: {e}"))?;
    let mut typedefs = HashMap::new();
    collect_typedefs(&root, &mut typedefs);
    TYPEDEFS.with(|table| *table.borrow_mut() = typedefs);
    let mut enum_signed = HashMap::new();
    collect_enum_signedness(&root, &mut enum_signed);
    ENUM_SIGNED.with(|table| *table.borrow_mut() = enum_signed);
    let mut enum_typedefs = HashMap::new();
    collect_enum_typedefs(&root, &mut enum_typedefs);
    ENUM_TYPEDEFS.with(|table| *table.borrow_mut() = enum_typedefs.clone());
    let mut enums = Vec::new();
    let mut records = Vec::new();
    let mut anonymous_header_records = Vec::new();
    let mut functions = Vec::new();
    let mut weak_refs = Vec::new();
    let mut function_allocation_attributes = HashMap::new();
    collect_enums(&root, source_file, record_roots, &enum_typedefs, &mut enums);
    collect_records(
        &root,
        source_file,
        record_roots,
        &plugin_events.pack_attributes,
        &mut records,
        &mut anonymous_header_records,
    );
    let source_text = (!source_file.is_empty())
        .then(|| std::fs::read_to_string(source_file).ok())
        .flatten();
    if let Some(source) = source_text.as_deref() {
        for (offset, fact) in &mut plugin_events.calls {
            fact.loc = loc_from_offset(source, *offset);
        }
    }
    let call_bindings = plugin_events
        .calls
        .values()
        .filter_map(|fact| Some((fact.loc?, fact.binding.clone())))
        .collect();
    CALL_FACTS.with(|facts| *facts.borrow_mut() = plugin_events.calls);
    collect_functions(
        &root,
        source_file,
        source_text.as_deref(),
        &plugin_events.macros,
        &mut functions,
    );
    collect_function_allocation_attributes(
        &root,
        source_file,
        source_text.as_deref(),
        &mut function_allocation_attributes,
    );
    collect_weak_ref_attributes(&root, source_file, &mut weak_refs);
    Ok(Unit {
        enums,
        records,
        anonymous_header_records,
        functions,
        weak_refs,
        function_allocation_attributes,
        call_bindings,
    })
}

fn collect_typedefs(node: &Value, out: &mut HashMap<String, String>) {
    if kind(node) == Some("TypedefDecl")
        && let (Some(name), Some(underlying)) =
            (node.get("name").and_then(Value::as_str), qual_type(node))
    {
        out.entry(name.to_string())
            .or_insert_with(|| underlying.to_string());
    }
    for child in children(node) {
        collect_typedefs(child, out);
    }
}

fn collect_enum_signedness(node: &Value, out: &mut HashMap<String, bool>) {
    if kind(node) == Some("EnumDecl")
        && let Some(name) = node.get("name").and_then(Value::as_str)
    {
        let signed = children(node).iter().any(|child| {
            kind(child) == Some("EnumConstantDecl")
                && enum_constant_value(child).is_some_and(|v| v < 0)
        });
        out.entry(name.to_string()).or_insert(signed);
    }
    for child in children(node) {
        collect_enum_signedness(child, out);
    }
}

fn collect_enum_typedefs(node: &Value, out: &mut HashMap<String, String>) {
    let kids = children(node);
    for (i, child) in kids.iter().enumerate() {
        if kind(child) == Some("EnumDecl")
            && let Some(tag) = child.get("name").and_then(Value::as_str)
            && !tag.is_empty()
            && let Some(alias) = next_enum_typedef_name(&kids, i + 1, tag)
        {
            out.entry(tag.to_string()).or_insert(alias);
        }
        collect_enum_typedefs(child, out);
    }
}

fn collect_enums(
    node: &Value,
    source_file: &str,
    record_roots: &[PathBuf],
    enum_typedefs: &HashMap<String, String>,
    out: &mut Vec<Enum>,
) {
    if kind(node) == Some("EnumDecl")
        && (is_source_node(node, source_file) || is_in_record_roots(node, record_roots))
    {
        if let Some(enm) = extract_enum(node, enum_typedefs) {
            out.push(enm);
        }
        return;
    }
    for child in children(node) {
        collect_enums(child, source_file, record_roots, enum_typedefs, out);
    }
}

fn collect_functions(
    node: &Value,
    source_file: &str,
    source_text: Option<&str>,
    macro_events: &HashMap<usize, MacroExpansion>,
    out: &mut Vec<Function>,
) {
    if kind(node) == Some("FunctionDecl") && is_source_node(node, source_file) && has_body(node) {
        if let Some(function) = extract_function(node, source_text, macro_events) {
            out.push(function);
        }
        return;
    }
    for child in children(node) {
        collect_functions(child, source_file, source_text, macro_events, out);
    }
}

fn collect_function_allocation_attributes(
    node: &Value,
    source_file: &str,
    source_text: Option<&str>,
    out: &mut HashMap<String, FunctionAllocationAttributes>,
) {
    if kind(node) == Some("FunctionDecl")
        && is_source_node(node, source_file)
        && let Some(name) = node.get("name").and_then(Value::as_str)
    {
        let mut attributes = FunctionAllocationAttributes::default();
        for child in children(node) {
            match kind(child) {
                Some("RestrictAttr")
                    if attribute_text(child, source_text).is_some_and(|text| {
                        text.split_once('(')
                            .map_or(text, |(name, _)| name)
                            .trim()
                            .trim_matches('_')
                            == "malloc"
                    }) =>
                {
                    attributes.fresh = true;
                }
                Some("AllocSizeAttr") => {
                    attributes.size_arguments = attribute_indices(child, source_text);
                }
                Some("AllocAlignAttr") => {
                    attributes.alignment_argument =
                        attribute_indices(child, source_text).into_iter().next();
                }
                Some("AssumeAlignedAttr") => {
                    let values = attribute_integer_values(child);
                    if let Some(bytes) = values.first().copied() {
                        attributes.assumed_alignment = Some(AssumedAlignmentAttribute {
                            bytes,
                            offset: values.get(1).copied().unwrap_or(0),
                        });
                    }
                }
                _ => {}
            }
        }
        if attributes != FunctionAllocationAttributes::default() {
            let existing = out.entry(name.to_string()).or_default();
            existing.fresh |= attributes.fresh;
            if !attributes.size_arguments.is_empty() {
                existing.size_arguments = attributes.size_arguments;
            }
            if attributes.alignment_argument.is_some() {
                existing.alignment_argument = attributes.alignment_argument;
            }
            if attributes.assumed_alignment.is_some() {
                existing.assumed_alignment = attributes.assumed_alignment;
            }
        }
    }
    for child in children(node) {
        collect_function_allocation_attributes(child, source_file, source_text, out);
    }
}

fn attribute_text<'a>(node: &Value, source_text: Option<&'a str>) -> Option<&'a str> {
    let source = source_text?;
    let begin = expansion_offset(node)?;
    let end = expansion_end_after_token(node)?;
    source.get(begin..end)
}

fn expansion_end_after_token(node: &Value) -> Option<usize> {
    let end = node.get("range")?.get("end")?;
    let location = end.get("expansionLoc").unwrap_or(end);
    let offset = location.get("offset")?.as_u64()? as usize;
    let token_len = location
        .get("tokLen")
        .or_else(|| end.get("tokLen"))
        .and_then(Value::as_u64)? as usize;
    offset.checked_add(token_len)
}

fn attribute_indices(node: &Value, source_text: Option<&str>) -> Vec<usize> {
    let Some(text) = attribute_text(node, source_text) else {
        return Vec::new();
    };
    let Some((_, arguments)) = text.split_once('(') else {
        return Vec::new();
    };
    let Some(arguments) = arguments.rsplit_once(')').map(|(arguments, _)| arguments) else {
        return Vec::new();
    };
    arguments
        .split(',')
        .filter_map(|argument| argument.trim().parse::<usize>().ok())
        .filter_map(|index| index.checked_sub(1))
        .collect()
}

fn attribute_integer_values(node: &Value) -> Vec<u64> {
    children(node)
        .iter()
        .filter_map(|child| child.get("value").and_then(Value::as_str))
        .filter_map(|value| value.parse().ok())
        .collect()
}

fn collect_weak_ref_attributes(node: &Value, source_file: &str, out: &mut Vec<WeakRefAttribute>) {
    if kind(node) == Some("FunctionDecl")
        && is_source_node(node, source_file)
        && children(node)
            .iter()
            .any(|child| kind(child) == Some("WeakRefAttr"))
        && let Some(name) = node.get("name").and_then(Value::as_str)
        && let Some(target) = children(node)
            .iter()
            .find(|child| kind(child) == Some("AliasAttr"))
            .and_then(|child| child.get("aliasee"))
            .and_then(Value::as_str)
    {
        out.push(WeakRefAttribute {
            name: name.to_string(),
            target: target.to_string(),
        });
    }
    for child in children(node) {
        collect_weak_ref_attributes(child, source_file, out);
    }
}

fn collect_records(
    node: &Value,
    source_file: &str,
    record_roots: &[PathBuf],
    pack_attributes: &[PackAttribute],
    out: &mut Vec<Record>,
    anonymous_header_out: &mut Vec<Record>,
) {
    if kind(node) == Some("RecordDecl")
        && (is_source_node(node, source_file) || is_in_record_roots(node, record_roots))
        && node
            .get("completeDefinition")
            .and_then(Value::as_bool)
            .unwrap_or(false)
        && let Some(record) = extract_record(node, None, source_file, pack_attributes)
    {
        out.push(record);
    }
    let kids = children(node);
    for (i, child) in kids.iter().enumerate() {
        if kind(child) == Some("RecordDecl")
            && child
                .get("completeDefinition")
                .and_then(Value::as_bool)
                .unwrap_or(false)
            && child
                .get("name")
                .and_then(Value::as_str)
                .unwrap_or("")
                .is_empty()
        {
            if is_included_record(child, source_file, record_roots)
                && let Some(record) = next_anonymous_field_name(&kids, i + 1)
                    .or_else(|| next_anonymous_typedef_name(&kids, i + 1))
                    .and_then(|name| {
                        extract_record(child, Some(name), source_file, pack_attributes)
                    })
            {
                out.push(record);
            } else if let Some(record) = next_referenced_anonymous_typedef_name(&kids, i + 1)
                .and_then(|name| extract_record(child, Some(name), source_file, pack_attributes))
            {
                anonymous_header_out.push(record);
            }
        }
        collect_records(
            child,
            source_file,
            record_roots,
            pack_attributes,
            out,
            anonymous_header_out,
        );
    }
}

fn extract_record(
    node: &Value,
    name_override: Option<String>,
    source_file: &str,
    pack_attributes: &[PackAttribute],
) -> Option<Record> {
    let name = name_override.or_else(|| node.get("name")?.as_str().map(str::to_string))?;
    if name.is_empty() {
        return None;
    }
    let record_kind = match node.get("tagUsed")?.as_str()? {
        "struct" => RecordKind::Struct,
        "union" => RecordKind::Union,
        _ => return None,
    };
    let fields = children(node)
        .iter()
        .filter(|child| kind(child) == Some("FieldDecl"))
        .enumerate()
        .filter_map(|(index, child)| {
            let name = child
                .get("name")
                .and_then(Value::as_str)
                .map(str::to_string)
                .or_else(|| {
                    let ty = qual_type(child)?;
                    (child.get("isImplicit").and_then(Value::as_bool) == Some(true)
                        && (ty.starts_with("struct ") || ty.starts_with("union "))
                        && (ty.contains("(anonymous at ") || ty.contains("(unnamed at ")))
                    .then(|| format!("__slate_anon_{index}"))
                })?;
            Some(Decl {
                name,
                comments: attached_comment(child),
                ty: parse_c_type(qual_type(child).unwrap_or("int")),
                bit_width: child
                    .get("isBitfield")
                    .and_then(Value::as_bool)
                    .unwrap_or(false)
                    .then(|| constant_expr_value(child))
                    .flatten(),
            })
        })
        .collect();
    let (packed, align) = record_layout_attrs(node, source_file, pack_attributes);
    Some(Record {
        name,
        comments: attached_comment(node),
        kind: record_kind,
        fields,
        packed,
        align,
    })
}

fn record_layout_attrs(
    node: &Value,
    source_file: &str,
    pack_attributes: &[PackAttribute],
) -> (Option<u32>, Option<u32>) {
    let mut packed = record_packing(node, source_file, pack_attributes);
    let mut align = None;
    for child in children(node) {
        match kind(child) {
            Some("PackedAttr") => packed = Some(1),
            Some("AlignedAttr") => {
                if let Some(n) = constant_expr_value(child) {
                    align = Some(align.unwrap_or(0).max(n));
                }
            }
            Some("FieldDecl") => {
                for attr in children(child) {
                    if kind(attr) == Some("AlignedAttr")
                        && let Some(n) = constant_expr_value(attr)
                    {
                        align = Some(align.unwrap_or(0).max(n));
                    }
                }
            }
            _ => {}
        }
    }
    (packed, align)
}

fn record_packing(
    node: &Value,
    source_file: &str,
    pack_attributes: &[PackAttribute],
) -> Option<u32> {
    let name = node.get("name").and_then(Value::as_str).unwrap_or("");
    let offset = source_offset(node.get("loc")?)?;
    let files = source_files(node);
    pack_attributes
        .iter()
        .find(|attribute| {
            attribute.name == name
                && attribute.offset == offset
                && if files.is_empty() {
                    same_source_file(&attribute.file, source_file)
                } else {
                    files
                        .iter()
                        .any(|file| same_source_file(&attribute.file, file))
                }
        })
        .map(|attribute| attribute.alignment)
}

fn source_offset(node: &Value) -> Option<usize> {
    node.get("expansionLoc")
        .and_then(source_offset)
        .or_else(|| {
            node.get("offset")
                .and_then(Value::as_u64)
                .map(|n| n as usize)
        })
        .or_else(|| node.get("spellingLoc").and_then(source_offset))
}

fn constant_expr_value(node: &Value) -> Option<u32> {
    if kind(node) == Some("ConstantExpr")
        && let Some(v) = node.get("value").and_then(Value::as_str)
    {
        return v.parse().ok();
    }
    children(node).iter().find_map(|c| constant_expr_value(c))
}

fn anonymous_record_name_from_field(node: &Value) -> Option<String> {
    if kind(node) != Some("FieldDecl") {
        return None;
    }
    let ty = qual_type(node)?;
    let name = ty
        .strip_prefix("struct (unnamed at ")
        .or_else(|| ty.strip_prefix("union (unnamed at "))?
        .strip_suffix(')')?;
    Some(format!("(unnamed at {name})"))
}

fn next_anonymous_field_name(kids: &[&Value], start: usize) -> Option<String> {
    kids.iter()
        .skip(start)
        .find_map(|sibling| anonymous_record_name_from_field(sibling))
}

fn next_anonymous_typedef_name(kids: &[&Value], start: usize) -> Option<String> {
    let sibling = kids.get(start)?;
    if kind(sibling) != Some("TypedefDecl") {
        return None;
    }
    let name = sibling.get("name")?.as_str()?;
    let ty = qual_type(sibling)?;
    (ty == format!("struct {name}") || ty == format!("union {name}")).then(|| name.to_string())
}

fn next_referenced_anonymous_typedef_name(kids: &[&Value], start: usize) -> Option<String> {
    let sibling = kids.get(start)?;
    (sibling.get("isReferenced").and_then(Value::as_bool) == Some(true))
        .then(|| next_anonymous_typedef_name(kids, start))
        .flatten()
}

fn next_enum_typedef_name(kids: &[&Value], start: usize, tag: &str) -> Option<String> {
    let sibling = kids.get(start)?;
    if kind(sibling) != Some("TypedefDecl") {
        return None;
    }
    let name = sibling.get("name")?.as_str()?;
    let ty = qual_type(sibling)?;
    (ty == format!("enum {tag}")).then(|| name.to_string())
}

fn extract_enum(node: &Value, enum_typedefs: &HashMap<String, String>) -> Option<Enum> {
    let tag = node.get("name").and_then(Value::as_str).map(str::to_string);
    let name = tag
        .as_deref()
        .and_then(|tag| enum_typedefs.get(tag))
        .cloned()
        .or_else(|| tag.clone())?;
    if name.is_empty() {
        return None;
    }
    let mut next_value = 0;
    let mut variants = Vec::new();
    for child in children(node) {
        if kind(child) != Some("EnumConstantDecl") {
            continue;
        }
        let name = child.get("name")?.as_str()?.to_string();
        let comments = attached_comment(child);
        let value = enum_constant_value(child).unwrap_or(next_value);
        next_value = value + 1;
        variants.push(EnumVariant {
            name,
            comments,
            value,
        });
    }
    Some(Enum {
        name,
        tag,
        comments: attached_comment(node),
        variants,
    })
}

fn enum_constant_value(node: &Value) -> Option<i64> {
    children(node)
        .iter()
        .find(|child| kind(child) == Some("ConstantExpr"))
        .and_then(|child| child.get("value"))
        .and_then(Value::as_str)
        .and_then(|value| value.parse().ok())
}

fn extract_function(
    node: &Value,
    source_text: Option<&str>,
    macro_events: &HashMap<usize, MacroExpansion>,
) -> Option<Function> {
    let name = node.get("name")?.as_str()?.to_string();
    let fn_qual_type = qual_type(node).unwrap_or("int ()");
    let (ret, _) = parse_function_decl_qual_type(fn_qual_type);
    let param_nodes: Vec<_> = children(node)
        .iter()
        .filter(|child| kind(child) == Some("ParmVarDecl"))
        .copied()
        .collect();
    let params = param_nodes
        .iter()
        .filter_map(|child| {
            Some(Decl {
                name: child.get("name")?.as_str()?.to_string(),
                comments: attached_comment(child),
                ty: parse_c_type(qual_type(child).unwrap_or("int")),
                bit_width: None,
            })
        })
        .collect();
    let nonnull_static_params = param_nodes
        .iter()
        .enumerate()
        .filter_map(|(i, child)| {
            let source = source_text?;
            let offset = expansion_offset(child)?;
            param_is_static_array(source, offset).then_some(i)
        })
        .collect();
    let body = children(node)
        .iter()
        .find(|child| kind(child) == Some("CompoundStmt"))
        .map(|child| parse_compound_stmt(child));

    Some(Function {
        name,
        params,
        ret,
        body,
        loc: loc(node),
        raw: Some(node.clone()),
        layout_queries: collect_layout_queries(node, source_text),
        nonnull_static_params,
        macro_consts: collect_macro_consts(node, source_text, macro_events),
        asm_gotos: collect_asm_gotos(node, source_text),
    })
}

fn param_is_static_array(source: &str, offset: usize) -> bool {
    let Some(rest) = source.get(offset..) else {
        return false;
    };
    let mut paren_depth = 0i32;
    for (i, ch) in rest.char_indices() {
        match ch {
            '(' => paren_depth += 1,
            ')' if paren_depth == 0 => return false,
            ')' => paren_depth -= 1,
            ',' if paren_depth == 0 => return false,
            '[' if paren_depth == 0 => {
                return rest[i + 1..].trim_start().starts_with("static");
            }
            _ => {}
        }
    }
    false
}

fn collect_layout_queries(node: &Value, source_text: Option<&str>) -> Vec<LayoutQuery> {
    let mut out = Vec::new();
    collect_layout_queries_at(node, source_text, &mut out);
    out
}

fn collect_layout_queries_at(node: &Value, source_text: Option<&str>, out: &mut Vec<LayoutQuery>) {
    match kind(node) {
        Some("UnaryExprOrTypeTraitExpr") => {
            let ty = node
                .get("argType")
                .and_then(|arg| arg.get("qualType"))
                .and_then(Value::as_str)
                .map(parse_c_type);
            match (node.get("name").and_then(Value::as_str), ty) {
                (Some("sizeof"), Some(ty)) => out.push(LayoutQuery::Size(ty)),
                (Some("alignof" | "_Alignof" | "__alignof"), Some(ty)) => {
                    out.push(LayoutQuery::Align(ty))
                }
                _ => {}
            }
        }
        Some("OffsetOfExpr") => {
            if let Some(query) = source_text.and_then(|source| {
                expansion_offset(node).and_then(|offset| parse_offsetof(source, offset))
            }) {
                out.push(query);
            }
        }
        _ => {}
    }
    for child in children(node) {
        collect_layout_queries_at(child, source_text, out);
    }
}

fn expansion_offset(node: &Value) -> Option<usize> {
    let begin = node.get("range")?.get("begin")?;
    begin
        .get("expansionLoc")
        .unwrap_or(begin)
        .get("offset")?
        .as_u64()
        .map(|offset| offset as usize)
}

fn loc_from_offset(source: &str, offset: usize) -> Option<Loc> {
    let prefix = source.get(..offset)?;
    Some(Loc {
        line: prefix.bytes().filter(|byte| *byte == b'\n').count() as u32 + 1,
        col: prefix
            .rsplit_once('\n')
            .map_or(prefix.len(), |(_, line)| line.len()) as u32
            + 1,
    })
}

fn expansion_end_offset(node: &Value) -> Option<usize> {
    let end = node.get("range")?.get("end")?;
    end.get("expansionLoc")
        .unwrap_or(end)
        .get("offset")?
        .as_u64()
        .map(|offset| offset as usize)
}

fn collect_asm_gotos(node: &Value, source_text: Option<&str>) -> Vec<AsmGoto> {
    let mut out = Vec::new();
    if let Some(source) = source_text {
        collect_asm_gotos_at(node, source, &mut out);
    }
    out
}

fn collect_asm_gotos_at(node: &Value, source: &str, out: &mut Vec<AsmGoto>) {
    if kind(node) == Some("GCCAsmStmt")
        && let (Some(begin), Some(end)) = (expansion_offset(node), expansion_end_offset(node))
        && let Some(text) = source.get(begin..=end)
        && let Some(labels) = parse_asm_goto_labels(text)
    {
        out.push(AsmGoto { labels });
        return;
    }
    for child in children(node) {
        collect_asm_gotos_at(child, source, out);
    }
}

fn parse_asm_goto_labels(text: &str) -> Option<Vec<String>> {
    let open = text.find('(')?;
    if !text[..open]
        .split(|ch: char| ch != '_' && !ch.is_ascii_alphanumeric())
        .any(|word| word == "goto")
    {
        return None;
    }
    let close = matching_asm_paren(&text[open + 1..])?;
    let sections = split_asm_sections(&text[open + 1..open + 1 + close], ':');
    let labels = sections.get(4)?;
    split_asm_sections(labels, ',')
        .into_iter()
        .map(str::trim)
        .map(|label| {
            (!label.is_empty()
                && label.chars().enumerate().all(|(i, ch)| {
                    ch == '_'
                        || if i == 0 {
                            ch.is_ascii_alphabetic()
                        } else {
                            ch.is_ascii_alphanumeric()
                        }
                }))
            .then(|| label.to_string())
        })
        .collect()
}

fn matching_asm_paren(text: &str) -> Option<usize> {
    let mut paren = 0usize;
    let mut quote = None;
    let mut escaped = false;
    for (i, ch) in text.char_indices() {
        if let Some(delimiter) = quote {
            if escaped {
                escaped = false;
            } else if ch == '\\' {
                escaped = true;
            } else if ch == delimiter {
                quote = None;
            }
            continue;
        }
        match ch {
            '"' | '\'' => quote = Some(ch),
            '(' => paren += 1,
            ')' if paren == 0 => return Some(i),
            ')' => paren -= 1,
            _ => {}
        }
    }
    None
}

fn split_asm_sections(text: &str, delimiter: char) -> Vec<&str> {
    let mut sections = Vec::new();
    let mut start = 0usize;
    let mut paren = 0usize;
    let mut bracket = 0usize;
    let mut brace = 0usize;
    let mut quote = None;
    let mut escaped = false;
    for (i, ch) in text.char_indices() {
        if let Some(quote_delimiter) = quote {
            if escaped {
                escaped = false;
            } else if ch == '\\' {
                escaped = true;
            } else if ch == quote_delimiter {
                quote = None;
            }
            continue;
        }
        match ch {
            '"' | '\'' => quote = Some(ch),
            '(' => paren += 1,
            ')' => paren = paren.saturating_sub(1),
            '[' => bracket += 1,
            ']' => bracket = bracket.saturating_sub(1),
            '{' => brace += 1,
            '}' => brace = brace.saturating_sub(1),
            ch if ch == delimiter && paren == 0 && bracket == 0 && brace == 0 => {
                sections.push(&text[start..i]);
                start = i + ch.len_utf8();
            }
            _ => {}
        }
    }
    sections.push(&text[start..]);
    sections
}

fn collect_macro_consts(
    node: &Value,
    source_text: Option<&str>,
    macro_events: &HashMap<usize, MacroExpansion>,
) -> Vec<MacroConst> {
    let mut out = Vec::new();
    if let Some(source) = source_text {
        collect_macro_consts_at(node, source, macro_events, &mut out);
    }
    out
}

fn collect_macro_consts_at(
    node: &Value,
    source: &str,
    macro_events: &HashMap<usize, MacroExpansion>,
    out: &mut Vec<MacroConst>,
) {
    if let (Some(begin), Some(end)) = (expansion_offset(node), expansion_end_offset(node))
        && begin == end
        && let Some(event) = macro_events.get(&begin)
        && let Some(known) = crate::macros::lookup(&event.name)
        && event.headers.contains(known.header)
        && node
            .get("value")
            .and_then(Value::as_str)
            .is_some_and(|value| known.source_value_matches(value))
        && let Some(loc) = loc_from_offset(source, begin)
    {
        out.push(MacroConst {
            name: event.name.clone(),
            loc,
        });
        return;
    }
    for child in children(node) {
        collect_macro_consts_at(child, source, macro_events, out);
    }
}

fn parse_offsetof(source: &str, offset: usize) -> Option<LayoutQuery> {
    let rest = source.get(offset..)?;
    let args = rest.strip_prefix("offsetof")?;
    let args = args.get(args.find('(')? + 1..)?;
    let close = matching_paren(args)?;
    let args: Vec<_> = split_c_type_list(&args[..close]).collect();
    let [ty, field] = args.as_slice() else {
        return None;
    };
    let CType::Record(record) = parse_c_type(ty) else {
        return None;
    };
    Some(LayoutQuery::Offset {
        record,
        field: field.trim().to_string(),
    })
}

fn matching_paren(s: &str) -> Option<usize> {
    let mut depth = 0usize;
    for (idx, ch) in s.char_indices() {
        match ch {
            '(' => depth += 1,
            ')' if depth == 0 => return Some(idx),
            ')' => depth -= 1,
            _ => {}
        }
    }
    None
}

fn parse_compound_stmt(node: &Value) -> Vec<Stmt> {
    children(node)
        .iter()
        .filter_map(|child| parse_stmt(child))
        .collect()
}

fn parse_stmt(node: &Value) -> Option<Stmt> {
    match kind(node)? {
        "CompoundStmt" => Some(Stmt::Expr(Expr::Ident("{...}".into()))),
        "DeclStmt" => parse_decl_stmt(node),
        "ReturnStmt" => Some(Stmt::Return(
            children(node).first().and_then(|child| parse_expr(child)),
        )),
        "ForStmt" => parse_for_stmt(node),
        "WhileStmt" => {
            let kids = children(node);
            Some(Stmt::While {
                cond: kids.first().and_then(|child| parse_expr(child))?,
                body: kids
                    .get(1)
                    .map_or_else(Vec::new, |child| parse_stmt_body(child)),
            })
        }
        "IfStmt" => parse_if_stmt(node),
        _ => parse_expr(node).map(Stmt::Expr),
    }
}

fn parse_decl_stmt(node: &Value) -> Option<Stmt> {
    let kids = children(node);
    let decl = kids
        .into_iter()
        .find(|child| kind(child) == Some("VarDecl"))?;
    let name = decl.get("name")?.as_str()?.to_string();
    let ty = parse_c_type(qual_type(decl).unwrap_or("int"));
    let init = children(decl).first().and_then(|child| parse_expr(child));
    Some(Stmt::Decl(
        Decl {
            name,
            comments: attached_comment(decl),
            ty,
            bit_width: None,
        },
        init,
    ))
}

fn parse_for_stmt(node: &Value) -> Option<Stmt> {
    let kids = children(node);
    let init = kids.first().and_then(|child| parse_stmt(child));
    let cond = kids.get(1).and_then(|child| parse_expr(child));
    let step = kids.get(2).and_then(|child| parse_expr(child));
    let body = kids
        .get(3)
        .map_or_else(Vec::new, |child| parse_stmt_body(child));
    Some(Stmt::For {
        init: Box::new(init),
        cond,
        step,
        body,
    })
}

fn parse_if_stmt(node: &Value) -> Option<Stmt> {
    let kids = children(node);
    let cond = kids.first().and_then(|child| parse_expr(child))?;
    let then = kids
        .get(1)
        .map_or_else(Vec::new, |child| parse_stmt_body(child));
    let otherwise = kids.get(2).map(|child| parse_stmt_body(child));
    Some(Stmt::If {
        cond,
        then,
        otherwise,
    })
}

fn parse_stmt_body(node: &Value) -> Vec<Stmt> {
    if kind(node) == Some("CompoundStmt") {
        parse_compound_stmt(node)
    } else {
        parse_stmt(node).into_iter().collect()
    }
}

fn parse_expr(node: &Value) -> Option<Expr> {
    match kind(node)? {
        "IntegerLiteral" => node
            .get("value")
            .and_then(Value::as_str)
            .and_then(|s| s.parse().ok())
            .map(Expr::Int),
        "StringLiteral" => node
            .get("value")
            .and_then(Value::as_str)
            .map(|s| Expr::Str(s.to_string())),
        "DeclRefExpr" => decl_ref_name(node).map(Expr::Ident),
        "ImplicitCastExpr" | "ParenExpr" | "ExprWithCleanups" => {
            children(node).first().and_then(|child| parse_expr(child))
        }
        "UnaryOperator" => {
            let op = node.get("opcode")?.as_str()?.to_string();
            let expr = children(node).first().and_then(|child| parse_expr(child))?;
            Some(Expr::Unary {
                op,
                expr: Box::new(expr),
            })
        }
        "BinaryOperator" | "CompoundAssignOperator" => {
            let kids = children(node);
            let op = node.get("opcode")?.as_str()?.to_string();
            let lhs = kids.first().and_then(|child| parse_expr(child))?;
            let rhs = kids.get(1).and_then(|child| parse_expr(child))?;
            if kind(node) == Some("CompoundAssignOperator") {
                Some(Expr::Assign {
                    target: Box::new(lhs.clone()),
                    value: Box::new(Expr::Binary {
                        op: op.trim_end_matches('=').to_string(),
                        lhs: Box::new(lhs),
                        rhs: Box::new(rhs),
                    }),
                })
            } else if op == "=" {
                Some(Expr::Assign {
                    target: Box::new(lhs),
                    value: Box::new(rhs),
                })
            } else {
                Some(Expr::Binary {
                    op,
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                })
            }
        }
        "CallExpr" => {
            let kids = children(node);
            let callee = kids.first().and_then(|child| parse_expr(child))?;
            let Expr::Ident(name) = callee else {
                return None;
            };
            let args = kids
                .iter()
                .skip(1)
                .filter_map(|child| parse_expr(child))
                .collect();
            let fact = expansion_offset(node)
                .and_then(|offset| CALL_FACTS.with(|facts| facts.borrow().get(&offset).cloned()))
                .filter(|fact| matches!(fact.binding, CallBinding::Indirect) || fact.name == name);
            let binding = fact
                .as_ref()
                .map(|fact| fact.binding.clone())
                .unwrap_or_else(|| CallBinding::direct_unknown(None));
            let loc = fact.and_then(|fact| fact.loc).or_else(|| loc(node));
            Some(Expr::Call {
                name,
                args,
                binding,
                loc,
            })
        }
        _ => children(node).first().and_then(|child| parse_expr(child)),
    }
}

fn parse_function_decl_qual_type(s: &str) -> (CType, Vec<CType>) {
    let Some((ret, params)) = s.split_once('(') else {
        return (parse_c_type(s), Vec::new());
    };
    let params = params.strip_suffix(')').unwrap_or(params).trim();
    let params = if params.is_empty() || params == "void" {
        Vec::new()
    } else {
        split_c_type_list(params)
            .map(|param| parse_c_type(param.trim()))
            .collect()
    };
    (parse_c_type(ret.trim()), params)
}

fn parse_c_type(s: &str) -> CType {
    let s = s.trim();
    let s = strip_type_qualifiers(s);
    if let Some(inner) = s.strip_prefix("_Atomic(").and_then(|s| s.strip_suffix(')')) {
        return parse_c_type(inner);
    }
    let s = strip_trailing_type_qualifiers(s);
    if let Some(ty) = parse_bitint_type(s) {
        return ty;
    }
    if let Some((ret, params)) = parse_function_pointer_qual_type(s) {
        return CType::FuncPtr {
            ret: Box::new(ret),
            params,
        };
    }
    if let Some((ret, params)) = parse_function_qual_type(s) {
        return CType::FuncPtr {
            ret: Box::new(ret),
            params,
        };
    }
    if s == "void" {
        CType::Void
    } else if let Some(inner) = s.strip_suffix('*') {
        let inner = parse_c_type(inner.trim());
        if matches!(inner, CType::FuncPtr { .. }) {
            inner
        } else {
            CType::Ptr(Box::new(inner))
        }
    } else if let Some((inner, size)) = s.split_once('[') {
        let size = size.trim_end_matches(']').parse().ok();
        CType::Array(Box::new(parse_c_type(inner.trim())), size)
    } else if let Some(name) = s.strip_prefix("union ") {
        CType::Record(name.trim().to_string())
    } else if let Some(name) = s.strip_prefix("struct ") {
        CType::Record(name.trim().to_string())
    } else if s == "_Bool" || s == "bool" {
        CType::Bool
    } else if s == "float" {
        CType::Float { bits: 32 }
    } else if s == "double" {
        CType::Float { bits: 64 }
    } else if s == "long double" {
        CType::Float { bits: 80 }
    } else if matches!(s, "_Float128" | "__float128") {
        CType::Float { bits: 128 }
    } else if let Some(underlying) = lookup_typedef(s) {
        parse_c_type(&underlying)
    } else if let Some(name) = s.strip_prefix("enum ") {
        CType::Enum(enum_rust_name(name.trim()))
    } else {
        let signed = !s.contains("unsigned");
        CType::Int {
            signed,
            bits: int_bits(s),
        }
    }
}

fn lookup_typedef(name: &str) -> Option<String> {
    TYPEDEFS.with(|table| {
        let underlying = table.borrow().get(name)?.clone();
        (underlying != name).then_some(underlying)
    })
}

fn enum_is_signed(name: &str) -> bool {
    ENUM_SIGNED.with(|table| table.borrow().get(name).copied().unwrap_or(false))
}

fn enum_rust_name(name: &str) -> String {
    ENUM_TYPEDEFS.with(|table| {
        table
            .borrow()
            .get(name)
            .cloned()
            .unwrap_or_else(|| name.to_string())
    })
}

fn parse_function_pointer_qual_type(s: &str) -> Option<(CType, Vec<CType>)> {
    let (ret, rest) = s.split_once("(*)")?;
    let params = rest.trim().strip_prefix('(')?.strip_suffix(')')?.trim();
    Some((parse_c_type(ret.trim()), parse_function_params(params)))
}

fn parse_function_qual_type(s: &str) -> Option<(CType, Vec<CType>)> {
    let open = s.find('(')?;
    let ret = s[..open].trim();
    if ret.is_empty() || ret.contains('*') || matches!(ret, "struct" | "union" | "enum") {
        return None;
    }
    let params = s[open + 1..].strip_suffix(')')?.trim();
    if params.contains("unnamed at") {
        return None;
    }
    Some((parse_c_type(ret), parse_function_params(params)))
}

fn parse_function_params(params: &str) -> Vec<CType> {
    if params.is_empty() || params == "void" {
        Vec::new()
    } else {
        split_c_type_list(params)
            .map(str::trim)
            .filter(|param| !param.is_empty() && *param != "...")
            .map(parse_c_type)
            .collect()
    }
}

fn split_c_type_list(s: &str) -> impl Iterator<Item = &str> {
    split_top_level(s, ',').into_iter()
}

fn strip_type_qualifiers(mut s: &str) -> &str {
    loop {
        let stripped = s
            .strip_prefix("volatile ")
            .or_else(|| s.strip_prefix("volatile\t"))
            .or_else(|| s.strip_prefix("const "))
            .or_else(|| s.strip_prefix("const\t"))
            .or_else(|| s.strip_prefix("restrict "))
            .or_else(|| s.strip_prefix("restrict\t"))
            .or_else(|| s.strip_prefix("_Atomic "))
            .or_else(|| s.strip_prefix("_Atomic\t"));
        let Some(next) = stripped else {
            return s.trim();
        };
        s = next.trim_start();
    }
}

fn strip_trailing_type_qualifiers(mut s: &str) -> &str {
    'outer: loop {
        let trimmed = s.trim_end();
        for qualifier in ["volatile", "const", "restrict", "_Atomic"] {
            if let Some(head) = trimmed.strip_suffix(qualifier) {
                let head = head.trim_end();
                if head.ends_with('*') || head.chars().last().is_some_and(char::is_whitespace) {
                    s = head;
                    continue 'outer;
                }
            }
        }
        return trimmed;
    }
}

fn split_top_level(s: &str, delimiter: char) -> Vec<&str> {
    let mut parts = Vec::new();
    let mut start = 0usize;
    let mut paren = 0usize;
    for (i, c) in s.char_indices() {
        match c {
            '(' => paren += 1,
            ')' => paren = paren.saturating_sub(1),
            c if c == delimiter && paren == 0 => {
                parts.push(&s[start..i]);
                start = i + c.len_utf8();
            }
            _ => {}
        }
    }
    parts.push(&s[start..]);
    parts
}

fn int_bits(s: &str) -> u32 {
    if let Some(bits) = bitint_width(s) {
        bits
    } else if s.contains("char") {
        8
    } else if s.contains("short") {
        16
    } else if s.contains("__int128") {
        128
    } else if s.contains("long") {
        64
    } else {
        32
    }
}

fn bitint_width(s: &str) -> Option<u32> {
    let rest = s.split_once("_BitInt(")?.1;
    rest.split_once(')')?.0.parse().ok()
}

fn parse_bitint_type(s: &str) -> Option<CType> {
    let (signed, rest) = match s.strip_prefix("unsigned ") {
        Some(rest) => (false, rest),
        None => (true, s),
    };
    let bits = rest
        .strip_prefix("_BitInt(")?
        .strip_suffix(')')?
        .parse()
        .ok()?;
    Some(CType::Int { signed, bits })
}

fn has_body(node: &Value) -> bool {
    children(node)
        .iter()
        .any(|child| kind(child) == Some("CompoundStmt"))
}

fn is_source_node(node: &Value, source_file: &str) -> bool {
    if source_file.is_empty() {
        return true;
    }
    let files = source_files(node);
    if files
        .into_iter()
        .any(|file| same_source_file(file, source_file))
    {
        return true;
    }
    has_source_loc_without_file(node) && !has_included_from(node)
}

fn is_included_record(node: &Value, source_file: &str, record_roots: &[PathBuf]) -> bool {
    is_source_node(node, source_file) || is_in_record_roots(node, record_roots)
}

fn same_source_file(file: &str, source_file: &str) -> bool {
    file == source_file
        || Path::new(file) == Path::new(source_file)
        || file.ends_with(source_file)
        || source_file.ends_with(file)
}

fn is_in_record_roots(node: &Value, record_roots: &[PathBuf]) -> bool {
    !record_roots.is_empty()
        && source_files(node).into_iter().any(|file| {
            record_roots
                .iter()
                .any(|root| Path::new(file).starts_with(root))
        })
}

fn source_files(node: &Value) -> Vec<&str> {
    let mut files = Vec::new();
    collect_source_files(node.get("loc"), &mut files);
    collect_source_files(
        node.get("range").and_then(|range| range.get("begin")),
        &mut files,
    );
    collect_source_files(
        node.get("range").and_then(|range| range.get("end")),
        &mut files,
    );
    files
}

fn collect_source_files<'a>(node: Option<&'a Value>, out: &mut Vec<&'a str>) {
    let Some(node) = node else {
        return;
    };
    if let Some(file) = node.get("file").and_then(Value::as_str) {
        out.push(file);
    }
    collect_source_files(node.get("includedFrom"), out);
    collect_source_files(node.get("spellingLoc"), out);
    collect_source_files(node.get("expansionLoc"), out);
}

fn has_source_loc_without_file(node: &Value) -> bool {
    let loc = node.get("loc");
    loc.and_then(|loc| loc.get("line")).is_some()
        && loc.and_then(|loc| loc.get("file")).is_none()
        && loc.and_then(|loc| loc.get("includedFrom")).is_none()
}

fn has_included_from(node: &Value) -> bool {
    location_has_included_from(node.get("loc"))
        || location_has_included_from(node.get("range").and_then(|range| range.get("begin")))
        || location_has_included_from(node.get("range").and_then(|range| range.get("end")))
}

fn location_has_included_from(node: Option<&Value>) -> bool {
    let Some(node) = node else {
        return false;
    };
    node.get("includedFrom").is_some()
        || location_has_included_from(node.get("spellingLoc"))
        || location_has_included_from(node.get("expansionLoc"))
}

fn attached_comment(node: &Value) -> Vec<String> {
    children(node)
        .iter()
        .find(|child| kind(child) == Some("FullComment"))
        .map_or_else(Vec::new, |child| full_comment_lines(child))
}

fn full_comment_lines(node: &Value) -> Vec<String> {
    let mut lines = Vec::new();
    collect_comment_text(node, &mut lines);
    lines
}

fn collect_comment_text(node: &Value, out: &mut Vec<String>) {
    if kind(node) == Some("TextComment")
        && let Some(text) = node.get("text").and_then(Value::as_str)
    {
        let line = normalize_comment_line(text);
        if !line.is_empty() {
            out.push(line);
        }
    }
    for child in children(node) {
        collect_comment_text(child, out);
    }
}

fn normalize_comment_line(line: &str) -> String {
    line.trim().trim_start_matches('*').trim_start().to_string()
}

fn loc(node: &Value) -> Option<Loc> {
    let loc = node.get("loc")?;
    Some(Loc {
        line: loc.get("line")?.as_u64()? as u32,
        col: loc.get("col")?.as_u64()? as u32,
    })
}

fn qual_type(node: &Value) -> Option<&str> {
    let ty = node.get("type")?;
    ty.get("desugaredQualType")
        .and_then(Value::as_str)
        .or_else(|| ty.get("qualType").and_then(Value::as_str))
}

fn kind(node: &Value) -> Option<&str> {
    node.get("kind")?.as_str()
}

fn children(node: &Value) -> Vec<&Value> {
    node.get("inner")
        .and_then(Value::as_array)
        .map(|items| items.iter().collect())
        .unwrap_or_default()
}

fn decl_ref_name(node: &Value) -> Option<String> {
    node.get("referencedDecl")
        .and_then(|decl| decl.get("name"))
        .and_then(Value::as_str)
        .or_else(|| node.get("name").and_then(Value::as_str))
        .map(str::to_string)
}
