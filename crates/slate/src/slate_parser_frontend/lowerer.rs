use crate::backend::rust_ast::{self as rust, BinOp, Expr, FnDef, FnParam, Item, Prim, Stmt};
use crate::function_identity::CallBinding;
use crate::function_identity::FunctionIdentity;
use slate_parser::ir::{self, BindingId, Number, PlaceKind, TypeId, ValueKind};
use slate_parser::target_info::TargetInfo;
use std::cell::RefCell;
use std::collections::{BTreeMap, HashMap};

type Result<T> = std::result::Result<T, super::Error>;

struct FunctionName {
    rust: String,
    is_extern: bool,
}

struct Context<'a> {
    names: HashMap<BindingId, FunctionName>,
    bindings: HashMap<BindingId, String>,
    strings: HashMap<BindingId, Vec<u8>>,
    target: &'a TargetInfo,
    types: HashMap<TypeId, &'a ir::TypeDefinition>,
    record_names: HashMap<TypeId, String>,
    records: RefCell<BTreeMap<u32, Option<std::result::Result<rust::RecordDef, String>>>>,
}

#[derive(Default)]
pub struct Report {
    pub module: Vec<String>,
    pub declarations: Vec<(String, String)>,
    pub functions: Vec<(String, Option<String>)>,
}

impl Report {
    pub fn is_clean(&self) -> bool {
        self.module.is_empty()
            && self.declarations.is_empty()
            && self.functions.iter().all(|(_, barrier)| barrier.is_none())
    }
}

pub fn lower(module: &ir::Module) -> Result<rust::Program> {
    lower_module(module, None)
}

pub fn report(module: &ir::Module) -> Report {
    let mut report = Report::default();
    lower_module(module, Some(&mut report)).expect("report mode records barriers");
    let describe = |error: &mut String| *error = describe_types(error, module);
    report.module.iter_mut().for_each(describe);
    report
        .declarations
        .iter_mut()
        .for_each(|(_, error)| describe(error));
    report
        .functions
        .iter_mut()
        .filter_map(|(_, error)| error.as_mut())
        .for_each(describe);
    report
}

fn describe_types(message: &str, module: &ir::Module) -> String {
    let mut out = String::with_capacity(message.len());
    let mut rest = message;
    while let Some(start) = rest.find("@type") {
        let digits = rest[start + 5..]
            .find(|c: char| !c.is_ascii_digit())
            .unwrap_or(rest.len() - start - 5);
        let end = start + 5 + digits;
        out.push_str(&rest[..end]);
        let definition = rest[start + 5..end].parse::<u32>().ok().and_then(|id| {
            module
                .types
                .iter()
                .find(|definition| definition.value.id.0 == id)
        });
        if let Some(definition) = definition {
            let kind = match &definition.kind {
                ir::TypeDefinitionKind::Alias(_) => "typedef",
                ir::TypeDefinitionKind::Record {
                    kind: ir::RecordKind::Struct,
                    ..
                } => "struct",
                ir::TypeDefinitionKind::Record {
                    kind: ir::RecordKind::Union,
                    ..
                } => "union",
                ir::TypeDefinitionKind::Enum { .. } => "enum",
            };
            out.push_str(&format!(
                "({kind} {})",
                definition.name.as_deref().unwrap_or("<anonymous>")
            ));
        }
        rest = &rest[end..];
    }
    out.push_str(rest);
    out
}

fn module_barrier(report: &mut Option<&mut Report>, error: super::Error) -> Result<()> {
    match report {
        Some(report) => {
            report.module.push(error.to_string());
            Ok(())
        }
        None => Err(error),
    }
}

fn function_barrier<T>(
    report: &mut Option<&mut Report>,
    name: &str,
    defined: bool,
    result: Result<T>,
) -> Result<Option<T>> {
    match (result, report) {
        (Ok(value), Some(report)) => {
            if defined {
                report.functions.push((name.to_owned(), None));
            }
            Ok(Some(value))
        }
        (Ok(value), None) => Ok(Some(value)),
        (Err(error), Some(report)) if defined => {
            report
                .functions
                .push((name.to_owned(), Some(error.to_string())));
            Ok(None)
        }
        (Err(error), Some(report)) => {
            report
                .declarations
                .push((name.to_owned(), error.to_string()));
            Ok(None)
        }
        (Err(error), None) => Err(error),
    }
}

fn lower_module(module: &ir::Module, mut report: Option<&mut Report>) -> Result<rust::Program> {
    if !module.asm.is_empty() {
        module_barrier(
            &mut report,
            super::Error::Unsupported("module assembly".into()),
        )?;
    }
    let mut strings = HashMap::new();
    for global in &module.globals {
        match lower_string_global(global) {
            Ok(bytes) => {
                strings.insert(global.variable.id, bytes);
            }
            Err(error) => module_barrier(&mut report, error)?,
        }
    }
    let names: HashMap<_, _> = module
        .functions
        .iter()
        .map(|function| {
            (
                function.value.id,
                FunctionName {
                    rust: if function.name == "main" {
                        "__slate_main".into()
                    } else {
                        function.name.clone()
                    },
                    is_extern: function.body.is_none(),
                },
            )
        })
        .collect();
    let mut bindings = HashMap::new();
    for global in &module.globals {
        bindings.insert(
            global.value.variable.id,
            rust_binding_name(&global.value.variable.name),
        );
    }
    for function in &module.functions {
        if let ir::Parameters::Prototype { fixed, .. } = &function.value.parameters {
            for parameter in fixed {
                bindings.insert(
                    parameter.value.id,
                    parameter
                        .value
                        .name
                        .as_deref()
                        .map(rust_binding_name)
                        .unwrap_or_else(|| format!("__v{}", parameter.value.id.0)),
                );
            }
        }
        if let Some(body) = &function.value.body {
            collect_statement_names(body, &mut bindings);
        }
    }
    let cx = Context {
        names,
        bindings,
        strings,
        target: &module.target,
        types: module
            .types
            .iter()
            .map(|definition| (definition.value.id, &definition.value))
            .collect(),
        record_names: record_names(module),
        records: RefCell::default(),
    };
    let mut items = Vec::new();
    let mut externs = Vec::new();
    for function in &module.functions {
        let Some(body) = &function.body else {
            let decl = lower_extern(function, &cx);
            if let Some(decl) = function_barrier(&mut report, &function.name, false, decl)? {
                externs.push(decl);
            }
            continue;
        };
        let item = lower_function(function, body, &cx);
        if let Some(item) = function_barrier(&mut report, &function.name, true, item)? {
            items.push(item);
        }
    }
    let records = cx
        .records
        .into_inner()
        .into_values()
        .filter_map(|record| record.and_then(|record| record.ok()))
        .map(Item::Record);
    items.splice(0..0, records);
    if !externs.is_empty() {
        items.insert(
            0,
            Item::ExternBlock {
                abi: "C".into(),
                decls: externs,
            },
        );
    }
    if cx.names.values().any(|name| name.rust == "__slate_main") {
        items.push(Item::Fn(FnDef {
            attrs: Vec::new(),
            vis: rust::Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".into(),
            params: Vec::new(),
            ret: None,
            body: vec![Stmt::Expr(Expr::Call {
                func: Box::new(Expr::Var("std::process::exit".into())),
                args: vec![Expr::Call {
                    func: Box::new(Expr::Var("__slate_main".into())),
                    args: Vec::new(),
                    binding: CallBinding::Generated,
                }],
                binding: CallBinding::Generated,
            })],
        }));
    }
    Ok(rust::Program { items })
}

fn lower_string_global(global: &ir::Global) -> Result<Vec<u8>> {
    let Some(ir::ValueKind::CodeUnits(units)) = global
        .variable
        .initializer
        .as_ref()
        .map(|value| &value.node.value)
    else {
        let kind = match (global.definition, &global.variable.initializer) {
            (false, _) => "extern",
            (true, None) => "zero-initialized",
            (true, Some(_)) => "initialized",
        };
        return Err(super::Error::Unsupported(format!(
            "{kind} global {}",
            global.variable.name
        )));
    };
    units
        .iter()
        .map(|unit| {
            u8::try_from(*unit).map_err(|_| super::Error::Unsupported("wide string".into()))
        })
        .collect()
}

fn lower_extern(function: &ir::Function, cx: &Context) -> Result<rust::ExternDecl> {
    let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
        return Err(super::Error::Unsupported(format!(
            "unprototyped declaration {}",
            function.name
        )));
    };
    Ok(rust::ExternDecl::Fn(rust::ExternFnDecl {
        attrs: Vec::new(),
        name: function.name.clone(),
        identity: FunctionIdentity::Unknown,
        declared_type: None,
        trusted_headers: Default::default(),
        params: fixed
            .iter()
            .map(|parameter| {
                Ok(FnParam {
                    name: binding_name(parameter.value.id, &cx.bindings),
                    mutable: false,
                    ty: lower_type(cx, &parameter.ty)?,
                })
            })
            .collect::<Result<Vec<_>>>()?,
        variadic: *variadic,
        ret: function
            .return_type
            .as_ref()
            .map(|ty| lower_type(cx, ty))
            .transpose()?,
        safe: false,
    }))
}

fn lower_function(
    function: &ir::Function,
    body: &[slate_parser::ast::Span<ir::Statement>],
    cx: &Context,
) -> Result<Item> {
    let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
        return Err(super::Error::Unsupported(format!(
            "unprototyped function {}",
            function.name
        )));
    };
    if *variadic {
        return Err(super::Error::Unsupported(format!(
            "variadic definition {}",
            function.name
        )));
    }
    let params = fixed
        .iter()
        .map(|param| {
            Ok(FnParam {
                name: binding_name(param.value.id, &cx.bindings),
                mutable: true,
                ty: lower_type(cx, &param.ty)?,
            })
        })
        .collect::<Result<Vec<_>>>()?;
    let mut continue_labels = Vec::new();
    let mut statements = body
        .iter()
        .map(|statement| lower_statement(statement, cx, &mut continue_labels))
        .collect::<Result<Vec<_>>>()?;
    if matches!(function.fallthrough, Some(ir::Fallthrough::ReturnZero))
        && !matches!(statements.last(), Some(Stmt::Return(_)))
    {
        statements.push(Stmt::Return(Some(Expr::Value(rust::RustValue::I64(0)))));
    }
    Ok(Item::Fn(FnDef {
        attrs: Vec::new(),
        vis: rust::Visibility::Private,
        unsafe_: false,
        abi: None,
        name: cx.names[&function.id].rust.clone(),
        params,
        ret: function
            .return_type
            .as_ref()
            .map(|ty| lower_type(cx, ty))
            .transpose()?,
        body: statements,
    }))
}

fn rust_binding_name(name: &str) -> String {
    const KEYWORDS: &[&str] = &[
        "as", "break", "const", "continue", "crate", "else", "enum", "extern", "false", "fn",
        "for", "if", "impl", "in", "let", "loop", "match", "mod", "move", "mut", "pub", "ref",
        "return", "self", "Self", "static", "struct", "super", "trait", "true", "type", "unsafe",
        "use", "where", "while", "async", "await", "dyn",
    ];
    if KEYWORDS.contains(&name) {
        format!("r#{name}")
    } else {
        name.to_owned()
    }
}

fn binding_name(id: BindingId, bindings: &HashMap<BindingId, String>) -> String {
    bindings
        .get(&id)
        .cloned()
        .unwrap_or_else(|| format!("__v{}", id.0))
}

fn collect_statement_names(
    statements: &[slate_parser::ast::Span<ir::Statement>],
    bindings: &mut HashMap<BindingId, String>,
) {
    for statement in statements {
        match &statement.value {
            ir::Statement::Let(variable) => {
                bindings.insert(variable.id, rust_binding_name(&variable.name));
            }
            ir::Statement::Temporary { id, .. } => {
                bindings
                    .entry(*id)
                    .or_insert_with(|| format!("__v{}", id.0));
            }
            ir::Statement::Block(body)
            | ir::Statement::While { body, .. }
            | ir::Statement::DoWhile { body, .. }
            | ir::Statement::Switch { body, .. }
            | ir::Statement::Label { body, .. }
            | ir::Statement::Case { body, .. }
            | ir::Statement::Default { body, .. } => collect_statement_names(body, bindings),
            ir::Statement::For { init, body, .. } => {
                collect_statement_names(init, bindings);
                collect_statement_names(body, bindings);
            }
            ir::Statement::If {
                then_body,
                else_body,
                ..
            } => {
                collect_statement_names(then_body, bindings);
                if let Some(else_body) = else_body {
                    collect_statement_names(else_body, bindings);
                }
            }
            _ => {}
        }
    }
}

fn record_names(module: &ir::Module) -> HashMap<TypeId, String> {
    let mut counts = HashMap::<&str, usize>::new();
    for definition in &module.types {
        if let (Some(name), ir::TypeDefinitionKind::Record { .. }) =
            (&definition.name, &definition.kind)
        {
            *counts.entry(name).or_default() += 1;
        }
    }
    module
        .types
        .iter()
        .filter(|definition| matches!(definition.kind, ir::TypeDefinitionKind::Record { .. }))
        .map(|definition| {
            let id = definition.value.id;
            let name = match definition.name.as_deref() {
                Some(name) if counts[name] == 1 => name.to_owned(),
                Some(name) => format!("{name}_{}", id.0),
                None => format!("__SlateRecord{}", id.0),
            };
            (id, name)
        })
        .collect()
}

fn resolve_type<'a>(cx: &Context<'a>, mut ty: &'a ir::Type) -> &'a ir::Type {
    while let ir::Type::Defined(id) = ty
        && let Some(ir::TypeDefinitionKind::Alias(inner)) = cx.types.get(id).map(|d| &d.kind)
    {
        ty = inner;
    }
    ty
}

fn record_fields<'a>(
    cx: &Context<'a>,
    ty: &'a ir::Type,
) -> Option<&'a [slate_parser::ast::Span<ir::Field>]> {
    let ir::Type::Defined(id) = resolve_type(cx, ty) else {
        return None;
    };
    match &cx.types.get(id)?.kind {
        ir::TypeDefinitionKind::Record {
            kind: ir::RecordKind::Struct,
            fields: Some(fields),
            ..
        } => Some(fields),
        _ => None,
    }
}

fn storage_of(cx: &Context, ty: &ir::Type) -> Option<(u64, u64)> {
    match resolve_type(cx, ty) {
        ir::Type::Defined(id) => match &cx.types.get(id)?.kind {
            ir::TypeDefinitionKind::Record {
                layout: Some(layout),
                ..
            } => Some((layout.size, layout.align)),
            _ => None,
        },
        ir::Type::Array {
            element,
            length: Some(length),
        } => storage_of(cx, element).map(|(size, align)| (size * length, align)),
        ty => cx
            .target
            .storage_of(ty.clone())
            .ok()
            .map(|layout| (layout.size_bytes, layout.alignment_bytes.into())),
    }
}

fn lower_record(cx: &Context, id: TypeId) -> Result<String> {
    let name = cx.record_names[&id].clone();
    match cx.records.borrow().get(&id.0) {
        Some(Some(Err(error))) => return Err(super::Error::Unsupported(error.clone())),
        Some(_) => return Ok(name),
        None => {}
    }
    cx.records.borrow_mut().insert(id.0, None);
    let record = build_record(cx, &cx.types[&id].kind, &name).map_err(|error| match error {
        super::Error::Unsupported(message) => message,
        error => error.to_string(),
    });
    cx.records.borrow_mut().insert(id.0, Some(record.clone()));
    record.map(|_| name).map_err(super::Error::Unsupported)
}

fn build_record(
    cx: &Context,
    kind: &ir::TypeDefinitionKind,
    name: &str,
) -> Result<rust::RecordDef> {
    let ir::TypeDefinitionKind::Record {
        kind: ir::RecordKind::Struct,
        fields,
        layout,
    } = kind
    else {
        return Err(super::Error::Unsupported(format!("record {name}")));
    };
    let mut lowered = Vec::new();
    let mut end = 0u64;
    let mut align = 1u64;
    for (index, field) in fields.iter().flatten().enumerate() {
        let (Some(field_name), None, true) =
            (&field.name, field.bit_width, field.access.is_plain())
        else {
            return Err(super::Error::Unsupported(format!(
                "field {index} of record {name}"
            )));
        };
        let (field_size, field_align) = storage_of(cx, &field.ty)
            .ok_or_else(|| super::Error::Unsupported(format!("layout of {}", field.ty)))?;
        let offset = end.next_multiple_of(field_align);
        if layout.as_ref().map(|layout| layout.offsets[index]) != Some(offset) {
            return Err(super::Error::Unsupported(format!(
                "layout of record {name}"
            )));
        }
        end = offset + field_size;
        align = align.max(field_align);
        lowered.push(rust::RecordField {
            comments: Vec::new(),
            name: field_name.as_str().into(),
            ty: lower_type(cx, &field.ty)?,
        });
    }
    if let Some(layout) = layout
        && (layout.align != align || layout.size != end.next_multiple_of(align))
    {
        return Err(super::Error::Unsupported(format!(
            "layout of record {name}"
        )));
    }
    Ok(rust::RecordDef {
        comments: Vec::new(),
        vis: rust::Visibility::Private,
        field_vis: rust::Visibility::Private,
        is_union: false,
        allow_non_camel_case: !is_camel_case(name),
        name: name.to_owned(),
        fields: lowered,
        packed: None,
        align: None,
    })
}

fn is_camel_case(name: &str) -> bool {
    let name = name.trim_matches('_');
    let chars: Vec<char> = name.chars().collect();
    !chars.first().is_some_and(|first| first.is_lowercase())
        && !name.contains("__")
        && !chars.windows(2).any(|pair| {
            let has_case = |c: char| c.is_lowercase() || c.is_uppercase();
            (has_case(pair[0]) && pair[1] == '_') || (has_case(pair[1]) && pair[0] == '_')
        })
}

fn lower_type(cx: &Context, ty: &ir::Type) -> Result<rust::Type> {
    let primitive = match ty {
        ir::Type::Void => return Ok(rust::Type::Unit),
        ir::Type::Defined(id) => {
            return match cx.types.get(id).map(|definition| &definition.kind) {
                Some(ir::TypeDefinitionKind::Alias(inner)) => lower_type(cx, inner),
                Some(ir::TypeDefinitionKind::Record { .. }) => {
                    Ok(rust::Type::Custom(lower_record(cx, *id)?))
                }
                _ => Err(super::Error::Unsupported(format!("type {ty}"))),
            };
        }
        ir::Type::Bool => Prim::Bool,
        ir::Type::Numeric(ir::NumericType::Integer { width, signed, .. }) => {
            match (*width, *signed) {
                (8, true) => Prim::I8,
                (16, true) => Prim::I16,
                (32, true) => Prim::I32,
                (64, true) => Prim::I64,
                (128, true) => Prim::I128,
                (8, false) => Prim::U8,
                (16, false) => Prim::U16,
                (32, false) => Prim::U32,
                (64, false) => Prim::U64,
                (128, false) => Prim::U128,
                _ => return Err(super::Error::Unsupported(format!("integer type {ty}"))),
            }
        }
        ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F32)) => Prim::F32,
        ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F64)) => Prim::F64,
        ir::Type::Pointer { pointee, .. } if matches!(**pointee, ir::Type::Function { .. }) => {
            let ir::Type::Function {
                return_type,
                parameters,
                variadic: false,
                prototyped: true,
                convention: ir::CallConv::C,
            } = &**pointee
            else {
                return Err(super::Error::Unsupported(format!("type {ty}")));
            };
            return Ok(rust::Type::FnPtr {
                abi: rust::Abi::Rust,
                params: parameters
                    .iter()
                    .map(|ty| lower_type(cx, ty))
                    .collect::<Result<_>>()?,
                ret: Box::new(match return_type {
                    Some(ret) => lower_type(cx, ret)?,
                    None => rust::Type::Unit,
                }),
            });
        }
        ir::Type::Pointer {
            pointee, is_const, ..
        } => {
            return Ok(rust::Type::Ptr {
                mutable: !is_const,
                inner: Box::new(lower_type(cx, pointee)?),
            });
        }
        ir::Type::Array {
            element,
            length: Some(length),
        } => {
            return Ok(rust::Type::Array {
                elem: Box::new(lower_type(cx, element)?),
                len: *length,
            });
        }
        _ => return Err(super::Error::Unsupported(format!("type {ty}"))),
    };
    Ok(rust::Type::Prim(primitive))
}

fn lower_statement(
    statement: &ir::Statement,
    cx: &Context,
    continue_labels: &mut Vec<Option<rust::Label>>,
) -> Result<Stmt> {
    Ok(match statement {
        ir::Statement::Temporary {
            id,
            ty,
            initializer,
            ..
        } => Stmt::Let {
            name: binding_name(*id, &cx.bindings),
            mutable: false,
            ty: Some(lower_type(cx, ty)?),
            init: initializer
                .as_ref()
                .map(|value| lower_value(value, cx))
                .transpose()?,
        },
        ir::Statement::Let(variable) => {
            let init = variable
                .initializer
                .as_ref()
                .map(|value| lower_value(value, cx))
                .transpose()?;
            let init = match (init, &variable.ty) {
                (Some(init), _) => Some(init),
                (
                    None,
                    ir::Type::Array {
                        element,
                        length: Some(length),
                    },
                ) => Some(Expr::ArrayRepeat {
                    elem: Box::new(Expr::Cast {
                        expr: Box::new(Expr::Value(rust::RustValue::I64(0))),
                        ty: lower_type(cx, element)?,
                    }),
                    len: *length as usize,
                }),
                (None, ty) if record_fields(cx, ty).is_some() => {
                    Some(Expr::Unsafe(Box::new(rust::Block {
                        stmts: Vec::new(),
                        tail: Some(Box::new(Expr::Call {
                            func: Box::new(Expr::Var("std::mem::zeroed".into())),
                            args: Vec::new(),
                            binding: CallBinding::Generated,
                        })),
                    })))
                }
                (None, _) => None,
            };
            Stmt::Let {
                name: binding_name(variable.id, &cx.bindings),
                mutable: true,
                ty: Some(lower_type(cx, &variable.ty)?),
                init,
            }
        }
        ir::Statement::Expression(value) => {
            if matches!(value.node.value, ValueKind::Void) {
                Stmt::Block(rust::Block::default())
            } else {
                Stmt::Expr(lower_value(value, cx)?)
            }
        }
        ir::Statement::Write { place, value, .. } => {
            let target = lower_place(place, cx)?;
            let value = lower_value(value, cx)?;
            let assignment = Stmt::Assign { target, value };
            if place_dereferences(place) {
                Stmt::Unsafe {
                    body: rust::Block {
                        stmts: vec![assignment],
                        tail: None,
                    },
                }
            } else {
                assignment
            }
        }
        ir::Statement::Return(value) => Stmt::Return(
            value
                .as_ref()
                .map(|value| lower_value(value, cx))
                .transpose()?,
        ),
        ir::Statement::Block(body) => Stmt::Scope {
            body: body
                .iter()
                .map(|statement| lower_statement(statement, cx, continue_labels))
                .collect::<Result<Vec<_>>>()?,
        },
        ir::Statement::If {
            condition,
            then_body,
            else_body,
        } => Stmt::If {
            cond: lower_condition(condition, cx)?,
            then_body: lower_statement_list(then_body, cx, continue_labels)?,
            else_body: else_body
                .as_ref()
                .map(|body| lower_statement_list(body, cx, continue_labels))
                .transpose()?
                .unwrap_or_default(),
        },
        ir::Statement::While {
            condition, body, ..
        } => {
            continue_labels.push(None);
            let body = lower_statement_list(body, cx, continue_labels)?;
            continue_labels.pop();
            let (mut prefix, condition) = lower_evaluation(condition, cx)?;
            prefix.push(Stmt::If {
                cond: Expr::Unary {
                    op: rust::UnaryOp::Not,
                    expr: Box::new(condition),
                },
                then_body: vec![Stmt::Break(None)],
                else_body: Vec::new(),
            });
            prefix.extend(body);
            Stmt::Loop {
                label: None,
                body: prefix,
            }
        }
        ir::Statement::For {
            id,
            init,
            condition,
            increment,
            body,
        } => {
            let mut statements = lower_statement_list(init, cx, continue_labels)?;
            let label = rust::Label::new(format!("__slate_continue_{}", id.0));
            continue_labels.push(Some(label.clone()));
            let body = lower_statement_list(body, cx, continue_labels)?;
            continue_labels.pop();
            let mut loop_body = Vec::new();
            if let Some(condition) = condition {
                let (prefix, condition) = lower_evaluation(condition, cx)?;
                loop_body.extend(prefix);
                loop_body.push(Stmt::If {
                    cond: Expr::Unary {
                        op: rust::UnaryOp::Not,
                        expr: Box::new(condition),
                    },
                    then_body: vec![Stmt::Break(None)],
                    else_body: Vec::new(),
                });
            }
            loop_body.push(Stmt::LabeledBlock { label, body });
            if let Some(increment) = increment {
                loop_body.extend(lower_evaluation_statements(increment, cx)?);
            }
            statements.push(Stmt::Loop {
                label: None,
                body: loop_body,
            });
            Stmt::Scope { body: statements }
        }
        ir::Statement::DoWhile {
            id,
            body,
            condition,
        } => {
            let label = rust::Label::new(format!("__slate_continue_{}", id.0));
            continue_labels.push(Some(label.clone()));
            let body = lower_statement_list(body, cx, continue_labels)?;
            continue_labels.pop();
            let mut loop_body = vec![Stmt::LabeledBlock { label, body }];
            let (prefix, condition) = lower_evaluation(condition, cx)?;
            loop_body.extend(prefix);
            loop_body.push(Stmt::If {
                cond: Expr::Unary {
                    op: rust::UnaryOp::Not,
                    expr: Box::new(condition),
                },
                then_body: vec![Stmt::Break(None)],
                else_body: Vec::new(),
            });
            Stmt::Loop {
                label: None,
                body: loop_body,
            }
        }
        ir::Statement::Break(_) => Stmt::Break(None),
        ir::Statement::Continue(_) => match continue_labels.last().cloned().flatten() {
            Some(label) => Stmt::Break(Some(label)),
            None => Stmt::Continue(None),
        },
        ir::Statement::Null => Stmt::Block(rust::Block::default()),
        _ => {
            return Err(super::Error::Unsupported(format!(
                "statement {statement:?}"
            )));
        }
    })
}

fn lower_statement_list(
    statements: &[slate_parser::ast::Span<ir::Statement>],
    cx: &Context,
    continue_labels: &mut Vec<Option<rust::Label>>,
) -> Result<Vec<Stmt>> {
    statements
        .iter()
        .map(|statement| lower_statement(statement, cx, continue_labels))
        .collect()
}

fn lower_evaluation_statements(evaluation: &ir::Evaluation, cx: &Context) -> Result<Vec<Stmt>> {
    let mut statements = lower_statement_list(&evaluation.statements, cx, &mut Vec::new())?;
    if !matches!(evaluation.value.node.value, ValueKind::Void) {
        statements.push(Stmt::Expr(lower_value(&evaluation.value, cx)?));
    }
    Ok(statements)
}

fn lower_evaluation(evaluation: &ir::Evaluation, cx: &Context) -> Result<(Vec<Stmt>, Expr)> {
    let statements = lower_statement_list(&evaluation.statements, cx, &mut Vec::new())?;
    let condition = lower_condition(&evaluation.value, cx)?;
    Ok((statements, condition))
}

fn lower_condition(value: &ir::Value, cx: &Context) -> Result<Expr> {
    let condition = lower_value(value, cx)?;
    if matches!(value.ty, ir::Type::Bool) {
        Ok(condition)
    } else {
        Ok(Expr::Binary {
            op: BinOp::Ne,
            lhs: Box::new(condition),
            rhs: Box::new(lower_number(cx, &Number::Integer(0u32.into()), &value.ty)?),
        })
    }
}

fn lower_value(value: &ir::Value, cx: &Context) -> Result<Expr> {
    Ok(match &value.node.value {
        ValueKind::Constant(number) => lower_number(cx, number, &value.ty)?,
        ValueKind::Read {
            place,
            ordering: None,
        } => {
            let lowered = lower_place(place, cx)?;
            if place_dereferences(place) {
                Expr::Unsafe(Box::new(rust::Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(lowered)),
                }))
            } else {
                lowered
            }
        }
        ValueKind::Copy { operand, .. } => lower_value(operand, cx)?,
        ValueKind::AddressOf(place) => match place.kind {
            PlaceKind::Deref(ref pointer) => lower_value(pointer, cx)?,
            _ => Expr::AddrOf {
                mutable: true,
                expr: Box::new(lower_place(place, cx)?),
            },
        },
        ValueKind::Convert { operand, .. } => Expr::Cast {
            expr: Box::new(lower_value(operand, cx)?),
            ty: lower_type(cx, &value.ty)?,
        },
        ValueKind::ArrayDecay { place, .. } => {
            let PlaceKind::Binding(id) = place.kind else {
                return Err(super::Error::Unsupported(format!("array decay {place:?}")));
            };
            let Some(bytes) = cx.strings.get(&id) else {
                return Ok(Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::Var(binding_name(id, &cx.bindings).as_str().into())),
                        method: if matches!(value.ty, ir::Type::Pointer { is_const: true, .. }) {
                            "as_ptr"
                        } else {
                            "as_mut_ptr"
                        }
                        .into(),
                        args: Vec::new(),
                    }),
                    ty: lower_type(cx, &value.ty)?,
                });
            };
            Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::ByteStr(bytes.clone())),
                    method: "as_ptr".into(),
                    args: Vec::new(),
                }),
                ty: lower_type(cx, &value.ty)?,
            }
        }
        ValueKind::Arith {
            op, left, right, ..
        } => Expr::Binary {
            op: match op {
                ir::ArithOp::Add => BinOp::Add,
                ir::ArithOp::Sub => BinOp::Sub,
                ir::ArithOp::Mul => BinOp::Mul,
                ir::ArithOp::Div => BinOp::Div,
                ir::ArithOp::Rem => BinOp::Rem,
                ir::ArithOp::And => BinOp::BitAnd,
                ir::ArithOp::Or => BinOp::BitOr,
                ir::ArithOp::Xor => BinOp::BitXor,
                ir::ArithOp::Shl => BinOp::Shl,
                ir::ArithOp::Shr => BinOp::Shr,
                _ => return Err(super::Error::Unsupported(format!("arithmetic {op}"))),
            },
            lhs: Box::new(lower_value(left, cx)?),
            rhs: Box::new(lower_value(right, cx)?),
        },
        ValueKind::Unary {
            op: ir::UnaryArithOp::Neg,
            operand,
            ..
        } if matches!(
            value.ty,
            ir::Type::Numeric(ir::NumericType::Float(
                ir::FloatType::F32 | ir::FloatType::F64
            ))
        ) =>
        {
            Expr::Unary {
                op: rust::UnaryOp::Neg,
                expr: Box::new(lower_value(operand, cx)?),
            }
        }
        ValueKind::Unary {
            op,
            operand,
            semantics,
        } if matches!(
            value.ty,
            ir::Type::Bool | ir::Type::Numeric(ir::NumericType::Integer { .. })
        ) =>
        {
            let operand = lower_value(operand, cx)?;
            match (op, semantics) {
                (ir::UnaryArithOp::Not, _) => Expr::Unary {
                    op: rust::UnaryOp::Not,
                    expr: Box::new(operand),
                },
                (
                    ir::UnaryArithOp::Neg,
                    ir::ArithSema::Integer {
                        overflow: ir::Overflow::Undefined,
                    },
                ) => Expr::Unary {
                    op: rust::UnaryOp::Neg,
                    expr: Box::new(operand),
                },
                (
                    ir::UnaryArithOp::Neg,
                    ir::ArithSema::Integer {
                        overflow: ir::Overflow::Wrap,
                    },
                ) => Expr::MethodCall {
                    recv: Box::new(operand),
                    method: "wrapping_neg".into(),
                    args: Vec::new(),
                },
                _ => {
                    return Err(super::Error::Unsupported(format!(
                        "value {}",
                        value.display(false)
                    )));
                }
            }
        }
        ValueKind::Logical { op, left, right } if matches!(value.ty, ir::Type::Bool) => {
            Expr::Binary {
                op: match op {
                    ir::LogicalOp::And => BinOp::And,
                    ir::LogicalOp::Or => BinOp::Or,
                },
                lhs: Box::new(lower_condition(left, cx)?),
                rhs: Box::new(lower_condition(right, cx)?),
            }
        }
        ValueKind::Compare {
            op, left, right, ..
        } => Expr::Binary {
            op: match op {
                ir::CompareOp::Eq => BinOp::Eq,
                ir::CompareOp::Ne => BinOp::Ne,
                ir::CompareOp::Lt => BinOp::Lt,
                ir::CompareOp::Le => BinOp::Le,
                ir::CompareOp::Gt => BinOp::Gt,
                ir::CompareOp::Ge => BinOp::Ge,
            },
            lhs: Box::new(lower_value(left, cx)?),
            rhs: Box::new(lower_value(right, cx)?),
        },
        ValueKind::PointerOffset {
            pointer,
            amount,
            subtract,
            ..
        } => {
            let offset = Expr::Cast {
                expr: Box::new(lower_value(amount, cx)?),
                ty: rust::Type::Prim(Prim::Isize),
            };
            let offset = if *subtract {
                Expr::Unary {
                    op: rust::UnaryOp::Neg,
                    expr: Box::new(offset),
                }
            } else {
                offset
            };
            Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::MethodCall {
                    recv: Box::new(lower_value(pointer, cx)?),
                    method: "offset".into(),
                    args: vec![offset],
                })),
            }))
        }
        ValueKind::PointerDifference {
            left,
            right,
            element,
        } if !matches!(element, ir::Type::Void | ir::Type::Function { .. }) => Expr::Cast {
            expr: Box::new(Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::MethodCall {
                    recv: Box::new(lower_value(left, cx)?),
                    method: "offset_from".into(),
                    args: vec![Expr::Cast {
                        expr: Box::new(lower_value(right, cx)?),
                        ty: lower_type(cx, &left.ty)?,
                    }],
                })),
            }))),
            ty: lower_type(cx, &value.ty)?,
        },
        ValueKind::FunctionDecay {
            place:
                ir::Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
        } => match cx.names.get(id) {
            Some(name) if !name.is_extern => Expr::Call {
                func: Box::new(Expr::Var("Some".into())),
                args: vec![Expr::Var(name.rust.as_str().into())],
                binding: CallBinding::Generated,
            },
            _ => {
                return Err(super::Error::Unsupported(format!(
                    "value {}",
                    value.display(false)
                )));
            }
        },
        ValueKind::Null if matches!(&value.ty, ir::Type::Pointer { pointee, .. } if matches!(**pointee, ir::Type::Function { .. })) =>
        {
            lower_type(cx, &value.ty)?;
            Expr::Var("None".into())
        }
        ValueKind::Call {
            callee, arguments, ..
        } => {
            let func = match callee {
                ir::Callee::Direct(id) => Expr::Var(
                    cx.names
                        .get(id)
                        .ok_or_else(|| {
                            super::Error::Unsupported(format!("unknown callee %{}", id.0))
                        })?
                        .rust
                        .as_str()
                        .into(),
                ),
                ir::Callee::Indirect(pointer) => Expr::MethodCall {
                    recv: Box::new(lower_value(pointer, cx)?),
                    method: "unwrap".into(),
                    args: Vec::new(),
                },
            };
            let call = Expr::Call {
                func: Box::new(func),
                args: arguments
                    .iter()
                    .map(|argument| lower_value(argument, cx))
                    .collect::<Result<Vec<_>>>()?,
                binding: CallBinding::unknown(),
            };
            let unsafe_call = match callee {
                ir::Callee::Direct(id) => {
                    cx.names.get(id).is_some_and(|name| name.rust == "printf")
                }
                ir::Callee::Indirect(_) => true,
            };
            if unsafe_call {
                Expr::Unsafe(Box::new(rust::Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(call)),
                }))
            } else {
                call
            }
        }
        _ => {
            return Err(super::Error::Unsupported(format!(
                "value {}",
                value.display(false)
            )));
        }
    })
}

fn lower_place(place: &ir::Place, cx: &Context) -> Result<Expr> {
    match place.kind {
        PlaceKind::Binding(id) => Ok(Expr::Var(binding_name(id, &cx.bindings).as_str().into())),
        PlaceKind::Deref(ref pointer) => Ok(Expr::Unary {
            op: rust::UnaryOp::Deref,
            expr: Box::new(lower_value(pointer, cx)?),
        }),
        PlaceKind::Index {
            ref base,
            ref index,
        } => Ok(Expr::Index {
            base: Box::new(lower_value(base, cx)?),
            index: Box::new(lower_value(index, cx)?),
        }),
        PlaceKind::Field {
            ref base,
            index,
            bits: None,
        } if let Some(Some(name)) = record_fields(cx, &base.ty)
            .and_then(|fields| fields.get(index))
            .map(|field| &field.name) =>
        {
            lower_type(cx, &base.ty)?;
            Ok(Expr::Field {
                base: Box::new(lower_place(base, cx)?),
                field: name.clone(),
            })
        }
        _ => Err(super::Error::Unsupported(format!("place {place:?}"))),
    }
}

fn place_dereferences(place: &ir::Place) -> bool {
    match &place.kind {
        PlaceKind::Deref(_) => true,
        PlaceKind::Field { base, .. } => place_dereferences(base),
        _ => false,
    }
}

fn lower_number(cx: &Context, number: &Number, ty: &ir::Type) -> Result<Expr> {
    let value = match number {
        Number::Bool(value) => rust::RustValue::Bool(*value),
        Number::Integer(value) => rust::RustValue::U128(
            value
                .to_string()
                .parse()
                .map_err(|_| super::Error::Unsupported(format!("integer constant {value}")))?,
        ),
        Number::SignedInteger(value) => rust::RustValue::I128(
            value
                .to_string()
                .parse()
                .map_err(|_| super::Error::Unsupported(format!("integer constant {value}")))?,
        ),
        Number::FloatBits(bits) => {
            let (literal, finite, name) = match ty {
                ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F32)) => {
                    let value = f32::from_bits(*bits as u32);
                    (format!("{value:?}"), value.is_finite(), "f32")
                }
                ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F64)) => {
                    let value = f64::from_bits(*bits as u64);
                    (format!("{value:?}"), value.is_finite(), "f64")
                }
                _ => return Err(super::Error::Unsupported(format!("constant {number:?}"))),
            };
            if !finite {
                return Ok(Expr::HexFloat(format!("{name}::from_bits({bits:#x})")));
            }
            return Ok(match literal.strip_prefix('-') {
                Some(magnitude) => Expr::Unary {
                    op: rust::UnaryOp::Neg,
                    expr: Box::new(Expr::HexFloat(format!("{magnitude}{name}"))),
                },
                None => Expr::HexFloat(format!("{literal}{name}")),
            });
        }
        _ => return Err(super::Error::Unsupported(format!("constant {number:?}"))),
    };
    Ok(Expr::Cast {
        expr: Box::new(Expr::Value(value)),
        ty: lower_type(cx, ty)?,
    })
}
