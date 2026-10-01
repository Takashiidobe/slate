use super::long_double;
use crate::backend::rust_ast::{self as rust, Attr, BinOp, Expr, FnDef, FnParam, Item, Prim, Stmt};
use crate::function_identity::CallBinding;
use crate::function_identity::FunctionIdentity;
use slate_parser::ir::{self, BindingId, Number, PlaceKind, TypeId, ValueKind};
use slate_parser::target_info::TargetInfo;
use std::cell::{Cell, RefCell};
use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};

type Result<T> = std::result::Result<T, super::Error>;

struct FunctionName {
    rust: String,
    is_extern: bool,
    is_unsafe: bool,
    is_variadic: bool,
}

struct Context<'a> {
    names: HashMap<BindingId, FunctionName>,
    bindings: HashMap<BindingId, String>,
    strings: HashMap<BindingId, Vec<u8>>,
    statics: HashSet<BindingId>,
    over_aligned: HashMap<BindingId, u64>,
    target: &'a TargetInfo,
    types: HashMap<TypeId, &'a ir::TypeDefinition>,
    record_names: HashMap<TypeId, String>,
    records: RefCell<BTreeMap<u32, Option<std::result::Result<rust::RecordDef, String>>>>,
    temps: Cell<u32>,
    long_double: Cell<bool>,
    bridges: RefCell<BTreeMap<String, rust::ExternFnDecl>>,
}

impl Context<'_> {
    fn next_temp(&self) -> String {
        let index = self.temps.get();
        self.temps.set(index + 1);
        format!("__t{index}")
    }
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
    let mut statics = Vec::new();
    for global in &module.globals {
        match lower_string_global(global) {
            Some(Ok(bytes)) => {
                strings.insert(global.variable.id, bytes);
            }
            Some(Err(error)) => module_barrier(&mut report, error)?,
            None => statics.push(&global.value),
        }
    }
    let static_names: HashSet<_> = statics
        .iter()
        .map(|global| rust_binding_name(&global.variable.name))
        .collect();
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
                    is_unsafe: function.body.is_none()
                        || matches!(
                            function.parameters,
                            ir::Parameters::Prototype { variadic: true, .. }
                        ),
                    is_variadic: !matches!(
                        function.parameters,
                        ir::Parameters::Prototype {
                            variadic: false,
                            ..
                        }
                    ),
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
                let id = parameter.value.id;
                bindings.insert(
                    id,
                    parameter
                        .value
                        .name
                        .as_deref()
                        .map(|name| local_binding_name(name, id, &static_names))
                        .unwrap_or_else(|| format!("__v{}", id.0)),
                );
            }
        }
        if let Some(body) = &function.value.body {
            collect_statement_names(body, &mut bindings, &static_names);
        }
    }
    let mut cx = Context {
        names,
        bindings,
        strings,
        statics: statics.iter().map(|global| global.variable.id).collect(),
        over_aligned: HashMap::new(),
        target: &module.target,
        types: module
            .types
            .iter()
            .map(|definition| (definition.value.id, &definition.value))
            .collect(),
        record_names: record_names(module),
        records: RefCell::default(),
        temps: Cell::new(0),
        long_double: Cell::new(false),
        bridges: RefCell::default(),
    };
    cx.over_aligned = statics
        .iter()
        .filter(|global| global.definition)
        .filter_map(|global| Some((global.variable.id, over_alignment(&cx, &global.variable)?)))
        .collect();
    let mut items = Vec::new();
    let mut externs = Vec::new();
    for global in &statics {
        let name = binding_name(global.variable.id, &cx.bindings);
        match lower_static(global, &cx) {
            Ok((ty, Some(init))) => items.push(Item::Static {
                attrs: Vec::new(),
                vis: rust::Visibility::Private,
                mutable: true,
                name,
                ty,
                init,
            }),
            Ok((ty, None)) => externs.push(rust::ExternDecl::Static {
                attrs: Vec::new(),
                mutable: true,
                name,
                ty,
            }),
            Err(error) => module_barrier(&mut report, error)?,
        }
    }
    for function in &module.functions {
        let Some(body) = &function.body else {
            if passes_long_double(function, &cx) {
                continue;
            }
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
    let wrappers = cx
        .over_aligned
        .values()
        .copied()
        .collect::<BTreeSet<_>>()
        .into_iter()
        .map(|alignment| {
            Ok(Item::Struct(rust::StructDef {
                attrs: vec![Attr::Repr(vec![
                    rust::Repr::C,
                    rust::Repr::Align(u32::try_from(alignment).map_err(|_| {
                        super::Error::Unsupported(format!("alignment {alignment}"))
                    })?),
                ])],
                vis: rust::Visibility::Private,
                field_vis: rust::Visibility::Private,
                generics: vec![rust::GenericParam {
                    name: "T".into(),
                    bounds: Vec::new(),
                }],
                name: align_wrapper(alignment),
                fields: rust::StructFields::Tuple(vec![rust::Type::Custom("T".into())]),
            }))
        })
        .collect::<Result<Vec<_>>>()?;
    let records = cx
        .records
        .into_inner()
        .into_values()
        .filter_map(|record| record.and_then(|record| record.ok()))
        .map(Item::Record);
    items.splice(0..0, records.chain(wrappers));
    if cx.long_double.get() {
        items.splice(
            0..0,
            long_double::long_double_prelude(rust::Visibility::Private),
        );
        externs.extend(
            long_double::f80_shim_decls()
                .into_iter()
                .filter(|decl| decl.name.starts_with("__slate_f80_"))
                .chain(cx.bridges.take().into_values())
                .map(rust::ExternDecl::Fn),
        );
    }
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

fn lower_string_global(global: &ir::Global) -> Option<Result<Vec<u8>>> {
    if !global.variable.name.starts_with('.') {
        return None;
    }
    let ir::ValueKind::CodeUnits(units) = &global.variable.initializer.as_ref()?.node.value else {
        return None;
    };
    Some(
        units
            .iter()
            .map(|unit| {
                u8::try_from(*unit).map_err(|_| super::Error::Unsupported("wide string".into()))
            })
            .collect(),
    )
}

fn over_alignment(cx: &Context, variable: &ir::Variable) -> Option<u64> {
    let alignment = variable.alignment?;
    let abi_alignment = matches!(variable.ty, ir::Type::Array { .. })
        && variable.alignment == cx.target.large_array_alignment();
    let (_, natural) = storage_of(cx, &variable.ty)?;
    (!abi_alignment && alignment > natural).then_some(alignment)
}

fn align_wrapper(alignment: u64) -> String {
    format!("__SlateAlign{alignment}")
}

fn lower_static(global: &ir::Global, cx: &Context) -> Result<(rust::Type, Option<Expr>)> {
    let variable = &global.variable;
    let abi_alignment = matches!(variable.ty, ir::Type::Array { .. })
        && variable.alignment == cx.target.large_array_alignment();
    if !matches!(variable.storage, ir::StorageDuration::Static)
        || (variable.alignment.is_some()
            && !abi_alignment
            && storage_of(cx, &variable.ty).is_none())
        || !variable.access.is_plain()
        || global.symbol != ir::SymbolAttributes::default()
    {
        return Err(super::Error::Unsupported(format!(
            "global {} attributes",
            variable.name
        )));
    }
    let ty = lower_type(cx, &variable.ty)?;
    if !global.definition {
        return Ok((ty, None));
    }
    let init = match &variable.initializer {
        None => zeroed(),
        Some(value) if is_constant_initializer(value, cx) => lower_value(value, cx)?,
        Some(_) => {
            return Err(super::Error::Unsupported(format!(
                "initialized global {}",
                variable.name
            )));
        }
    };
    Ok(match cx.over_aligned.get(&variable.id) {
        Some(&alignment) => (
            rust::Type::Generic {
                name: align_wrapper(alignment),
                args: vec![ty],
            },
            Some(Expr::Call {
                func: Box::new(Expr::Var(align_wrapper(alignment).into())),
                args: vec![init],
                binding: CallBinding::Generated,
            }),
        ),
        None => (ty, Some(init)),
    })
}

fn is_constant_initializer(value: &ir::Value, cx: &Context) -> bool {
    match &value.node.value {
        ValueKind::Constant(_) | ValueKind::CodeUnits(_) | ValueKind::Null => true,
        ValueKind::ArrayDecay { place, .. } | ValueKind::AddressOf(place) => {
            is_constant_address(place, cx)
        }
        ValueKind::PointerOffset {
            pointer, amount, ..
        } => {
            is_constant_initializer(pointer, cx)
                && matches!(amount.node.value, ValueKind::Constant(_))
        }
        ValueKind::Convert { operand, .. } => {
            !is_long_double(cx, &operand.ty)
                && !is_long_double(cx, &value.ty)
                && is_constant_initializer(operand, cx)
        }
        ValueKind::Aggregate { members, .. } => members
            .iter()
            .all(|member| is_constant_initializer(&member.value, cx)),
        _ => false,
    }
}

fn is_constant_address(place: &ir::Place, cx: &Context) -> bool {
    match &place.kind {
        PlaceKind::Binding(id) => cx.statics.contains(id) || cx.strings.contains_key(id),
        PlaceKind::Field { base, .. } => is_constant_address(base, cx),
        PlaceKind::Deref(pointer) => is_constant_initializer(pointer, cx),
        PlaceKind::Index { base, index } => {
            is_constant_initializer(base, cx) && matches!(index.node.value, ValueKind::Constant(_))
        }
        _ => false,
    }
}

fn is_long_double(cx: &Context, ty: &ir::Type) -> bool {
    matches!(
        resolve_type(cx, ty),
        ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F80))
    )
}

fn holds_long_double(cx: &Context, ty: &ir::Type) -> bool {
    is_long_double(cx, ty)
        || match resolve_type(cx, ty) {
            ir::Type::Array { element, .. } => holds_long_double(cx, element),
            ty => record_fields(cx, ty)
                .is_some_and(|fields| fields.iter().any(|field| holds_long_double(cx, &field.ty))),
        }
}

fn passes_long_double(function: &ir::Function, cx: &Context) -> bool {
    let ir::Parameters::Prototype { fixed, .. } = &function.parameters else {
        return false;
    };
    fixed
        .iter()
        .map(|parameter| &parameter.ty)
        .chain(&function.return_type)
        .any(|ty| holds_long_double(cx, ty))
}

fn long_double_literal(bits: u128) -> Expr {
    Expr::TupleStructLit {
        name: long_double::LONG_DOUBLE_TY.into(),
        fields: vec![Expr::ArrayLit(
            bits.to_le_bytes()[..10]
                .iter()
                .map(|byte| Expr::Value(rust::RustValue::I64(i64::from(*byte))))
                .collect(),
        )],
    }
}

fn long_double_shim(name: &str, operand: Expr) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(name.into())),
        args: vec![operand],
        binding: CallBinding::Generated,
    }
}

fn lower_long_double_conversion(cx: &Context, operand: &ir::Value, ty: &ir::Type) -> Result<Expr> {
    let lowered = lower_value(operand, cx)?;
    let (from, to) = (is_long_double(cx, &operand.ty), is_long_double(cx, ty));
    let shim = match (from, to) {
        (true, true) => return Ok(lowered),
        (false, true) => long_double::f80_cast_from_name(&lower_type(cx, &operand.ty)?),
        _ => long_double::f80_cast_to_name(&lower_type(cx, ty)?),
    };
    let shim = shim.ok_or_else(|| {
        super::Error::Unsupported(format!("long double conversion {} -> {ty}", operand.ty))
    })?;
    Ok(long_double_shim(shim, lowered))
}

fn lower_long_double_bridge(
    cx: &Context,
    function: &FunctionName,
    arguments: &[ir::Value],
    ret: &ir::Type,
) -> Result<Expr> {
    let callee = function.rust.as_str();
    if (function.is_variadic && crate::function_identity::Known::from_symbol(callee).is_none())
        || callee.contains("__")
    {
        return Err(super::Error::Unsupported(format!(
            "long double call to {callee}"
        )));
    }
    let params = arguments
        .iter()
        .map(|argument| lower_type(cx, &argument.ty))
        .collect::<Result<Vec<_>>>()?;
    let ret = lower_type(cx, ret)?;
    let tags = long_double_bridge_tags(callee, std::iter::once(&ret).chain(&params))?;
    let name = format!("__slate_{callee}__r{}", tags.join("_"));
    let args = arguments
        .iter()
        .map(|argument| lower_value(argument, cx))
        .collect::<Result<Vec<_>>>()?;
    Ok(call_long_double_bridge(cx, name, params, ret, args))
}

fn lower_long_double_variadic_trampoline(
    cx: &Context,
    function: &FunctionName,
    arguments: &[ir::Value],
    fixed: usize,
    ret: &ir::Type,
) -> Result<Expr> {
    let callee = function.rust.as_str();
    let params = arguments
        .iter()
        .map(|argument| lower_type(cx, &argument.ty))
        .collect::<Result<Vec<_>>>()?;
    let ret = lower_type(cx, ret)?;
    let fixed_tags =
        long_double_bridge_tags(callee, std::iter::once(&ret).chain(&params[..fixed]))?;
    let variadic_tags = long_double_bridge_tags(callee, &params[fixed..])?;
    let name = format!(
        "__slate_vcall__r{}__{}",
        fixed_tags.join("_"),
        variadic_tags.join("_")
    );
    let code_pointer = rust::Type::Ptr {
        mutable: false,
        inner: Box::new(rust::Type::Unit),
    };
    let args = std::iter::once(Ok(Expr::Cast {
        expr: Box::new(Expr::Var(callee.into())),
        ty: code_pointer.clone(),
    }))
    .chain(arguments.iter().map(|argument| lower_value(argument, cx)))
    .collect::<Result<Vec<_>>>()?;
    Ok(call_long_double_bridge(
        cx,
        name,
        std::iter::once(code_pointer).chain(params).collect(),
        ret,
        args,
    ))
}

fn long_double_bridge_tags<'a>(
    callee: &str,
    types: impl IntoIterator<Item = &'a rust::Type>,
) -> Result<Vec<String>> {
    types
        .into_iter()
        .map(|ty| match long_double::long_double_shim_type_tag(ty) {
            tag if tag == "x" => Err(super::Error::Unsupported(format!(
                "long double call to {callee} passing {}",
                crate::backend::codegen::type_to_string(ty)
            ))),
            tag => Ok(tag),
        })
        .collect()
}

fn call_long_double_bridge(
    cx: &Context,
    name: String,
    params: Vec<rust::Type>,
    ret: rust::Type,
    args: Vec<Expr>,
) -> Expr {
    cx.bridges
        .borrow_mut()
        .entry(name.clone())
        .or_insert_with(|| rust::ExternFnDecl {
            attrs: Vec::new(),
            name: name.clone(),
            identity: FunctionIdentity::Unknown,
            declared_type: None,
            trusted_headers: Default::default(),
            params: params
                .into_iter()
                .enumerate()
                .map(|(index, ty)| FnParam {
                    name: format!("_{index}"),
                    mutable: false,
                    ty,
                })
                .collect(),
            variadic: false,
            ret: (!matches!(ret, rust::Type::Unit)).then_some(ret),
            safe: false,
        });
    Expr::Unsafe(Box::new(rust::Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            func: Box::new(Expr::Var(name.into())),
            args,
            binding: CallBinding::Generated,
        })),
    }))
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
    cx.temps.set(0);
    let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
        return Err(super::Error::Unsupported(format!(
            "unprototyped function {}",
            function.name
        )));
    };
    let mut params = fixed
        .iter()
        .map(|param| {
            Ok(FnParam {
                name: binding_name(param.value.id, &cx.bindings),
                mutable: true,
                ty: lower_type(cx, &param.ty)?,
            })
        })
        .collect::<Result<Vec<_>>>()?;
    if *variadic {
        params.push(FnParam {
            name: VA_ARGS.into(),
            mutable: true,
            ty: rust::Type::Variadic,
        });
    }
    let mut statements = body
        .iter()
        .map(|statement| lower_statement(statement, cx))
        .collect::<Result<Vec<_>>>()?;
    if matches!(function.fallthrough, Some(ir::Fallthrough::ReturnZero))
        && !matches!(statements.last(), Some(Stmt::Return(_)))
    {
        statements.push(Stmt::Return(Some(Expr::Value(rust::RustValue::I64(0)))));
    }
    if matches!(function.fallthrough, Some(ir::Fallthrough::UndefinedIfUsed))
        && !matches!(statements.last(), Some(Stmt::Return(_)))
    {
        statements.push(Stmt::Return(Some(zeroed())));
    }
    Ok(Item::Fn(FnDef {
        attrs: Vec::new(),
        vis: rust::Visibility::Private,
        unsafe_: *variadic,
        abi: variadic.then_some(rust::Abi::CUnwind),
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

fn local_binding_name(name: &str, id: BindingId, statics: &HashSet<String>) -> String {
    let name = rust_binding_name(name);
    if statics.contains(&name) {
        format!("{}_{}", name.trim_start_matches("r#"), id.0)
    } else {
        name
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
    reserved: &HashSet<String>,
) {
    for statement in statements {
        match &statement.value {
            ir::Statement::Let(variable) => {
                bindings.insert(
                    variable.id,
                    local_binding_name(&variable.name, variable.id, reserved),
                );
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
            | ir::Statement::Default { body, .. } => {
                collect_statement_names(body, bindings, reserved)
            }
            ir::Statement::For { init, body, .. } => {
                collect_statement_names(init, bindings, reserved);
                collect_statement_names(body, bindings, reserved);
            }
            ir::Statement::If {
                then_body,
                else_body,
                ..
            } => {
                collect_statement_names(then_body, bindings, reserved);
                if let Some(else_body) = else_body {
                    collect_statement_names(else_body, bindings, reserved);
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
        && let Some(
            ir::TypeDefinitionKind::Alias(inner)
            | ir::TypeDefinitionKind::Enum {
                underlying: Some(inner),
                ..
            },
        ) = cx.types.get(id).map(|d| &d.kind)
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
                Some(
                    ir::TypeDefinitionKind::Alias(inner)
                    | ir::TypeDefinitionKind::Enum {
                        underlying: Some(inner),
                        ..
                    },
                ) => lower_type(cx, inner),
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
        ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F80)) => {
            cx.long_double.set(true);
            return Ok(rust::Type::LongDouble);
        }
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
        ir::Type::VaList => return Ok(rust::Type::VaList),
        _ => return Err(super::Error::Unsupported(format!("type {ty}"))),
    };
    Ok(rust::Type::Prim(primitive))
}

const VA_ARGS: &str = "__va_args";

fn overflow_method(op: ir::ArithOp) -> Result<&'static str> {
    Ok(match op {
        ir::ArithOp::Add => "add",
        ir::ArithOp::Sub => "sub",
        ir::ArithOp::Mul => "mul",
        _ => return Err(super::Error::Unsupported(format!("overflow {op}"))),
    })
}

fn lower_overflow(
    cx: &Context,
    op: ir::ArithOp,
    left: &ir::Value,
    right: &ir::Value,
    result: &ir::Place,
) -> Result<Expr> {
    let method = overflow_method(op)?;
    for ty in [&left.ty, &right.ty, &result.ty] {
        if !matches!(
            resolve_type(cx, ty),
            ir::Type::Numeric(ir::NumericType::Integer { width, .. }) if *width <= 64
        ) {
            return Err(super::Error::Unsupported(format!("overflow operand {ty}")));
        }
    }
    let wide = rust::Type::Prim(Prim::I128);
    let target = lower_type(cx, &result.ty)?;
    let [lhs, rhs, exact] = [cx.next_temp(), cx.next_temp(), cx.next_temp()];
    let var = |name: &String| Expr::Var(name.as_str().into());
    let bind = |name: &String, init: Expr| Stmt::Let {
        name: name.clone(),
        mutable: false,
        ty: Some(wide.clone()),
        init: Some(init),
    };
    let call = |method: String| Expr::MethodCall {
        recv: Box::new(var(&lhs)),
        method,
        args: vec![var(&rhs)],
    };
    let truncated = Expr::Cast {
        expr: Box::new(var(&exact)),
        ty: target.clone(),
    };
    Ok(Expr::Block(Box::new(rust::Block {
        stmts: vec![
            bind(
                &lhs,
                Expr::Cast {
                    expr: Box::new(lower_value(left, cx)?),
                    ty: wide.clone(),
                },
            ),
            bind(
                &rhs,
                Expr::Cast {
                    expr: Box::new(lower_value(right, cx)?),
                    ty: wide.clone(),
                },
            ),
            bind(&exact, call(format!("wrapping_{method}"))),
            lower_assignment(cx, result, truncated.clone())?,
        ],
        tail: Some(Box::new(Expr::Binary {
            op: BinOp::Or,
            lhs: Box::new(Expr::MethodCall {
                recv: Box::new(call(format!("checked_{method}"))),
                method: "is_none".into(),
                args: Vec::new(),
            }),
            rhs: Box::new(Expr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(Expr::Cast {
                    expr: Box::new(truncated),
                    ty: wide.clone(),
                }),
                rhs: Box::new(var(&exact)),
            }),
        })),
    })))
}

fn clone_va_list(list: Expr) -> Expr {
    Expr::MethodCall {
        recv: Box::new(list),
        method: "clone".into(),
        args: Vec::new(),
    }
}

fn lower_assignment(cx: &Context, place: &ir::Place, value: Expr) -> Result<Stmt> {
    let assignment = Stmt::Assign {
        target: lower_place(place, cx)?,
        value,
    };
    Ok(if place_is_unsafe(cx, place) {
        Stmt::Unsafe {
            body: rust::Block {
                stmts: vec![assignment],
                tail: None,
            },
        }
    } else {
        assignment
    })
}

fn lower_statement(statement: &ir::Statement, cx: &Context) -> Result<Stmt> {
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
                ) => match lower_type(cx, element)? {
                    ty @ (rust::Type::Prim(_) | rust::Type::Ptr { .. }) => {
                        Some(Expr::ArrayRepeat {
                            elem: Box::new(Expr::Cast {
                                expr: Box::new(Expr::Value(rust::RustValue::I64(0))),
                                ty,
                            }),
                            len: *length as usize,
                        })
                    }
                    _ => Some(zeroed()),
                },
                (None, ty) => Some(match lower_type(cx, ty)? {
                    rust::Type::Prim(Prim::Bool) => Expr::Value(rust::RustValue::Bool(false)),
                    ty @ rust::Type::Prim(_) => Expr::Cast {
                        expr: Box::new(Expr::Value(rust::RustValue::I64(0))),
                        ty,
                    },
                    _ => zeroed(),
                }),
            };
            Stmt::Let {
                name: binding_name(variable.id, &cx.bindings),
                mutable: true,
                ty: Some(lower_type(cx, &variable.ty)?),
                init,
            }
        }
        ir::Statement::Expression(value) => match &value.node.value {
            ValueKind::Void | ValueKind::VaEnd { .. } => Stmt::Block(rust::Block::default()),
            ValueKind::VaStart { list } => {
                lower_assignment(cx, list, clone_va_list(Expr::Var(VA_ARGS.into())))?
            }
            ValueKind::VaCopy {
                destination,
                source,
            } => lower_assignment(cx, destination, clone_va_list(lower_place(source, cx)?))?,
            _ => Stmt::Expr(lower_value(value, cx)?),
        },
        ir::Statement::Write { place, value, .. } => {
            lower_assignment(cx, place, lower_value(value, cx)?)?
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
                .map(|statement| lower_statement(statement, cx))
                .collect::<Result<Vec<_>>>()?,
        },
        ir::Statement::If {
            condition,
            then_body,
            else_body,
        } => Stmt::If {
            cond: lower_condition(condition, cx)?,
            then_body: lower_statement_list(then_body, cx)?,
            else_body: else_body
                .as_ref()
                .map(|body| lower_statement_list(body, cx))
                .transpose()?
                .unwrap_or_default(),
        },
        ir::Statement::While {
            id,
            condition,
            body,
        } => {
            let body = lower_statement_list(body, cx)?;
            let (mut prefix, condition) = lower_evaluation(condition, cx)?;
            prefix.push(Stmt::If {
                cond: Expr::Unary {
                    op: rust::UnaryOp::Not,
                    expr: Box::new(condition),
                },
                then_body: vec![Stmt::Break(None)],
                else_body: Vec::new(),
            });
            prefix.push(Stmt::LabeledBlock {
                label: continue_label(*id),
                body,
            });
            Stmt::Loop {
                label: Some(break_label(*id)),
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
            let mut statements = lower_statement_list(init, cx)?;
            let body = lower_statement_list(body, cx)?;
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
            loop_body.push(Stmt::LabeledBlock {
                label: continue_label(*id),
                body,
            });
            if let Some(increment) = increment {
                loop_body.extend(lower_evaluation_statements(increment, cx)?);
            }
            statements.push(Stmt::Loop {
                label: Some(break_label(*id)),
                body: loop_body,
            });
            Stmt::Scope { body: statements }
        }
        ir::Statement::DoWhile {
            id,
            body,
            condition,
        } => {
            let body = lower_statement_list(body, cx)?;
            let mut loop_body = vec![Stmt::LabeledBlock {
                label: continue_label(*id),
                body,
            }];
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
                label: Some(break_label(*id)),
                body: loop_body,
            }
        }
        ir::Statement::Switch {
            id,
            discriminant,
            body,
        } => lower_switch(*id, discriminant, body, cx)?,
        ir::Statement::Break(id) => Stmt::Break(Some(break_label(*id))),
        ir::Statement::Continue(id) => Stmt::Break(Some(continue_label(*id))),
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
) -> Result<Vec<Stmt>> {
    statements
        .iter()
        .map(|statement| lower_statement(statement, cx))
        .collect()
}

fn break_label(id: BindingId) -> rust::Label {
    rust::Label::new(format!("__slate_break_{}", id.0))
}

fn continue_label(id: BindingId) -> rust::Label {
    rust::Label::new(format!("__slate_continue_{}", id.0))
}

struct SwitchArm<'a> {
    values: Vec<&'a ir::Value>,
    default: bool,
    body: Vec<&'a ir::Statement>,
}

fn switch_label<'a>(
    statement: &'a ir::Statement,
    switch: BindingId,
) -> Result<Option<SwitchArm<'a>>> {
    let (value, body) = match statement {
        ir::Statement::Case {
            switch: owner,
            start,
            end,
            body,
        } if *owner == switch => {
            if end.is_some() {
                return Err(super::Error::Unsupported("switch case range".into()));
            }
            (Some(start), body)
        }
        ir::Statement::Default {
            switch: owner,
            body,
        } if *owner == switch => (None, body),
        _ => return Ok(None),
    };
    let mut arm = match body.first() {
        Some(first) => match switch_label(first, switch)? {
            Some(mut nested) => {
                nested
                    .body
                    .extend(body[1..].iter().map(|statement| &**statement));
                nested
            }
            None => SwitchArm {
                values: Vec::new(),
                default: false,
                body: body.iter().map(|statement| &**statement).collect(),
            },
        },
        None => SwitchArm {
            values: Vec::new(),
            default: false,
            body: Vec::new(),
        },
    };
    match value {
        Some(value) => arm.values.insert(0, value),
        None => arm.default = true,
    }
    Ok(Some(arm))
}

fn contains_switch_label(statements: &[&ir::Statement], switch: BindingId) -> bool {
    statements.iter().any(|statement| match statement {
        ir::Statement::Case { switch: owner, .. }
        | ir::Statement::Default { switch: owner, .. }
            if *owner == switch =>
        {
            true
        }
        ir::Statement::Block(body)
        | ir::Statement::While { body, .. }
        | ir::Statement::DoWhile { body, .. }
        | ir::Statement::Switch { body, .. }
        | ir::Statement::Label { body, .. }
        | ir::Statement::Case { body, .. }
        | ir::Statement::Default { body, .. } => contains_switch_label(
            &body
                .iter()
                .map(|statement| &**statement)
                .collect::<Vec<_>>(),
            switch,
        ),
        ir::Statement::For { init, body, .. } => contains_switch_label(
            &init
                .iter()
                .chain(body)
                .map(|statement| &**statement)
                .collect::<Vec<_>>(),
            switch,
        ),
        ir::Statement::If {
            then_body,
            else_body,
            ..
        } => contains_switch_label(
            &then_body
                .iter()
                .chain(else_body.iter().flatten())
                .map(|statement| &**statement)
                .collect::<Vec<_>>(),
            switch,
        ),
        _ => false,
    })
}

fn ends_in_jump(statement: &ir::Statement) -> bool {
    match statement {
        ir::Statement::Break(_) | ir::Statement::Continue(_) | ir::Statement::Return(_) => true,
        ir::Statement::Block(body) => body.last().is_some_and(|last| ends_in_jump(last)),
        _ => false,
    }
}

fn lower_switch(
    id: BindingId,
    discriminant: &ir::Value,
    body: &[slate_parser::ast::Span<ir::Statement>],
    cx: &Context,
) -> Result<Stmt> {
    let statements = match body {
        [single] => match &**single {
            ir::Statement::Block(inner) => inner.as_slice(),
            _ => body,
        },
        _ => body,
    };
    let mut arms: Vec<SwitchArm> = Vec::new();
    for statement in statements {
        match switch_label(statement, id)? {
            Some(arm) => arms.push(arm),
            None => match arms.last_mut() {
                Some(arm) => arm.body.push(statement),
                None => {
                    return Err(super::Error::Unsupported(
                        "statement before first switch case".into(),
                    ));
                }
            },
        }
    }
    for (index, arm) in arms.iter().enumerate() {
        if contains_switch_label(&arm.body, id) {
            return Err(super::Error::Unsupported("nested switch case label".into()));
        }
        if !arm.body.last().is_some_and(|last| ends_in_jump(last)) && index + 1 < arms.len() {
            return Err(super::Error::Unsupported("switch fallthrough".into()));
        }
    }
    let selector = cx.next_temp();
    let lower_arm = |arm: &SwitchArm| -> Result<Vec<Stmt>> {
        arm.body
            .iter()
            .map(|statement| lower_statement(statement, cx))
            .collect()
    };
    let mut chain = match arms.iter().find(|arm| arm.default) {
        Some(arm) => lower_arm(arm)?,
        None => Vec::new(),
    };
    for arm in arms.iter().rev().filter(|arm| !arm.default) {
        let mut cond: Option<Expr> = None;
        for value in &arm.values {
            let test = Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(Expr::Var(selector.clone().into())),
                rhs: Box::new(lower_value(value, cx)?),
            };
            cond = Some(match cond {
                Some(previous) => Expr::Binary {
                    op: BinOp::Or,
                    lhs: Box::new(previous),
                    rhs: Box::new(test),
                },
                None => test,
            });
        }
        let Some(cond) = cond else {
            continue;
        };
        chain = vec![Stmt::If {
            cond,
            then_body: lower_arm(arm)?,
            else_body: chain,
        }];
    }
    let mut block = vec![Stmt::Let {
        name: selector,
        mutable: false,
        ty: Some(lower_type(cx, &discriminant.ty)?),
        init: Some(lower_value(discriminant, cx)?),
    }];
    block.extend(chain);
    Ok(Stmt::LabeledBlock {
        label: break_label(id),
        body: block,
    })
}

fn lower_evaluation_statements(evaluation: &ir::Evaluation, cx: &Context) -> Result<Vec<Stmt>> {
    let mut statements = lower_statement_list(&evaluation.statements, cx)?;
    if !matches!(evaluation.value.node.value, ValueKind::Void) {
        statements.push(Stmt::Expr(lower_value(&evaluation.value, cx)?));
    }
    Ok(statements)
}

fn lower_evaluation(evaluation: &ir::Evaluation, cx: &Context) -> Result<(Vec<Stmt>, Expr)> {
    let statements = lower_statement_list(&evaluation.statements, cx)?;
    let condition = lower_condition(&evaluation.value, cx)?;
    Ok((statements, condition))
}

fn lower_condition(value: &ir::Value, cx: &Context) -> Result<Expr> {
    let condition = lower_value(value, cx)?;
    if matches!(value.ty, ir::Type::Bool) {
        Ok(condition)
    } else if is_long_double(cx, &value.ty) {
        Ok(Expr::Binary {
            op: BinOp::Ne,
            lhs: Box::new(condition),
            rhs: Box::new(long_double_literal(0)),
        })
    } else {
        Ok(Expr::Binary {
            op: BinOp::Ne,
            lhs: Box::new(condition),
            rhs: Box::new(lower_number(cx, &Number::Integer(0u32.into()), &value.ty)?),
        })
    }
}

fn byte_pointer_type() -> rust::Type {
    rust::Type::Ptr {
        mutable: false,
        inner: Box::new(rust::Type::Prim(Prim::U8)),
    }
}

fn byte_pointer(pointer: &ir::Value, cx: &Context) -> Result<Expr> {
    let expr = Box::new(lower_value(pointer, cx)?);
    Ok(match lower_type(cx, &pointer.ty)? {
        from @ rust::Type::FnPtr { .. } => Expr::Transmute {
            from,
            to: byte_pointer_type(),
            expr,
        },
        _ => Expr::Cast {
            expr,
            ty: byte_pointer_type(),
        },
    })
}

fn lower_value(value: &ir::Value, cx: &Context) -> Result<Expr> {
    Ok(match &value.node.value {
        ValueKind::Constant(number) => lower_number(cx, number, &value.ty)?,
        ValueKind::CodeUnits(units) => {
            let ir::Type::Array { element, .. } = &value.ty else {
                return Err(super::Error::Unsupported(format!(
                    "code units of {}",
                    value.ty
                )));
            };
            let signed_width = match **element {
                ir::Type::Numeric(ir::NumericType::Integer {
                    width,
                    signed: true,
                    ..
                }) => Some(128 - width),
                _ => None,
            };
            let element = lower_type(cx, element)?;
            Expr::ArrayLit(
                units
                    .iter()
                    .map(|unit| Expr::Cast {
                        expr: Box::new(Expr::Value(match signed_width {
                            Some(shift) => {
                                rust::RustValue::I128((i128::from(*unit) << shift) >> shift)
                            }
                            None => rust::RustValue::U128(u128::from(*unit)),
                        })),
                        ty: element.clone(),
                    })
                    .collect(),
            )
        }
        ValueKind::VaArg { list } if is_long_double(cx, &value.ty) => {
            Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::Call {
                    func: Box::new(Expr::Var("__slate_f80_va_arg".into())),
                    args: vec![Expr::AddrOf {
                        mutable: true,
                        expr: Box::new(lower_place(list, cx)?),
                    }],
                    binding: CallBinding::Generated,
                })),
            }))
        }
        ValueKind::VaArg { list } => Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::MethodCallGeneric {
                recv: Box::new(lower_place(list, cx)?),
                method: "next_arg".into(),
                type_args: vec![lower_type(cx, &value.ty)?],
                args: Vec::new(),
            })),
        })),
        ValueKind::Read {
            place,
            ordering: None,
        } => {
            let lowered = lower_place(place, cx)?;
            let lowered = if matches!(value.ty, ir::Type::VaList) {
                clone_va_list(lowered)
            } else {
                lowered
            };
            if place_is_unsafe(cx, place) {
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
            _ => {
                let address = Expr::AddrOf {
                    mutable: true,
                    expr: Box::new(lower_place(place, cx)?),
                };
                if place_is_unsafe(cx, place) {
                    Expr::Unsafe(Box::new(rust::Block {
                        stmts: Vec::new(),
                        tail: Some(Box::new(address)),
                    }))
                } else {
                    address
                }
            }
        },
        ValueKind::Convert { operand, .. }
            if is_long_double(cx, &operand.ty) || is_long_double(cx, &value.ty) =>
        {
            lower_long_double_conversion(cx, operand, &value.ty)?
        }
        ValueKind::Convert { operand, .. } => Expr::Cast {
            expr: Box::new(lower_value(operand, cx)?),
            ty: lower_type(cx, &value.ty)?,
        },
        ValueKind::ArrayDecay { place, .. } => {
            let bytes = match place.kind {
                PlaceKind::Binding(id) => cx.strings.get(&id),
                _ => None,
            };
            let Some(bytes) = bytes else {
                let array = lower_place(place, cx)?;
                let decayed = Expr::Cast {
                    expr: Box::new(if place_is_static(cx, place) {
                        Expr::AddrOf {
                            mutable: true,
                            expr: Box::new(array),
                        }
                    } else {
                        Expr::MethodCall {
                            recv: Box::new(array),
                            method: if matches!(value.ty, ir::Type::Pointer { is_const: true, .. })
                            {
                                "as_ptr"
                            } else {
                                "as_mut_ptr"
                            }
                            .into(),
                            args: Vec::new(),
                        }
                    }),
                    ty: lower_type(cx, &value.ty)?,
                };
                return Ok(if place_is_unsafe(cx, place) {
                    Expr::Unsafe(Box::new(rust::Block {
                        stmts: Vec::new(),
                        tail: Some(Box::new(decayed)),
                    }))
                } else {
                    decayed
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
            op: op @ (ir::ArithOp::Add | ir::ArithOp::Sub | ir::ArithOp::Mul),
            left,
            right,
            semantics:
                ir::ArithSema::Integer {
                    overflow: ir::Overflow::Wrap,
                },
        } => Expr::MethodCall {
            recv: Box::new(lower_value(left, cx)?),
            method: format!("wrapping_{}", overflow_method(*op)?),
            args: vec![lower_value(right, cx)?],
        },
        ValueKind::Overflow {
            op,
            left,
            right,
            result,
        } => lower_overflow(cx, *op, left, right, result)?,
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
                ir::FloatType::F32 | ir::FloatType::F64 | ir::FloatType::F80
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
        ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            let select = Expr::If {
                cond: Box::new(lower_condition(condition, cx)?),
                then_expr: Box::new(lower_value(then_value, cx)?),
                else_expr: Box::new(lower_value(else_value, cx)?),
            };
            match lower_type(cx, &value.ty)? {
                ty @ rust::Type::FnPtr { .. } => {
                    let temp = cx.next_temp();
                    Expr::Block(Box::new(rust::Block {
                        stmts: vec![Stmt::Let {
                            name: temp.clone(),
                            mutable: false,
                            ty: Some(ty),
                            init: Some(select),
                        }],
                        tail: Some(Box::new(Expr::Var(temp.into()))),
                    }))
                }
                _ => select,
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
            element,
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
            match element {
                ir::Type::Void => Expr::Cast {
                    expr: Box::new(Expr::Unsafe(Box::new(rust::Block {
                        stmts: Vec::new(),
                        tail: Some(Box::new(Expr::MethodCall {
                            recv: Box::new(byte_pointer(pointer, cx)?),
                            method: "offset".into(),
                            args: vec![offset],
                        })),
                    }))),
                    ty: lower_type(cx, &value.ty)?,
                },
                ir::Type::Function { .. } => Expr::Transmute {
                    from: byte_pointer_type(),
                    to: lower_type(cx, &value.ty)?,
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(byte_pointer(pointer, cx)?),
                        method: "wrapping_offset".into(),
                        args: vec![offset],
                    }),
                },
                _ => Expr::Unsafe(Box::new(rust::Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(Expr::MethodCall {
                        recv: Box::new(lower_value(pointer, cx)?),
                        method: "offset".into(),
                        args: vec![offset],
                    })),
                })),
            }
        }
        ValueKind::PointerDifference {
            left,
            right,
            element: ir::Type::Void,
        } => Expr::Cast {
            expr: Box::new(Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::MethodCall {
                    recv: Box::new(byte_pointer(left, cx)?),
                    method: "offset_from".into(),
                    args: vec![byte_pointer(right, cx)?],
                })),
            }))),
            ty: lower_type(cx, &value.ty)?,
        },
        ValueKind::PointerDifference {
            left,
            right,
            element: ir::Type::Function { .. },
        } => Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(Expr::Cast {
                    expr: Box::new(byte_pointer(left, cx)?),
                    ty: rust::Type::Prim(Prim::Isize),
                }),
                method: "wrapping_sub".into(),
                args: vec![Expr::Cast {
                    expr: Box::new(byte_pointer(right, cx)?),
                    ty: rust::Type::Prim(Prim::Isize),
                }],
            }),
            ty: lower_type(cx, &value.ty)?,
        },
        ValueKind::PointerDifference { left, right, .. } => Expr::Cast {
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
        ValueKind::Null => {
            let rust::Type::Ptr { mutable, inner } = lower_type(cx, &value.ty)? else {
                return Err(super::Error::Unsupported(format!(
                    "value {}",
                    value.display(false)
                )));
            };
            Expr::Call {
                func: Box::new(Expr::Var(
                    format!(
                        "std::ptr::{}::<{}>",
                        if mutable { "null_mut" } else { "null" },
                        crate::backend::codegen::type_to_string(&inner)
                    )
                    .into(),
                )),
                args: Vec::new(),
                binding: CallBinding::Generated,
            }
        }
        ValueKind::Call {
            callee: ir::Callee::Direct(id),
            arguments,
            ..
        } if cx.names.get(id).is_some_and(|name| name.is_extern)
            && std::iter::once(&value.ty)
                .chain(arguments.iter().map(|argument| &argument.ty))
                .any(|ty| holds_long_double(cx, ty)) =>
        {
            lower_long_double_bridge(cx, &cx.names[id], arguments, &value.ty)?
        }
        ValueKind::Call {
            callee: ir::Callee::Direct(id),
            signature: ir::Type::Function { parameters, .. },
            arguments,
            ..
        } if cx.names.get(id).is_some_and(|name| name.is_variadic)
            && arguments
                .iter()
                .skip(parameters.len())
                .any(|argument| holds_long_double(cx, &argument.ty)) =>
        {
            lower_long_double_variadic_trampoline(
                cx,
                &cx.names[id],
                arguments,
                parameters.len(),
                &value.ty,
            )?
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
                ir::Callee::Direct(id) => cx.names.get(id).is_some_and(|name| name.is_unsafe),
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
        ValueKind::Aggregate { members, zero_fill } => {
            lower_aggregate(cx, &value.ty, members, *zero_fill)?
        }
        _ => {
            return Err(super::Error::Unsupported(format!(
                "value {}",
                value.display(false)
            )));
        }
    })
}

fn zeroed() -> Expr {
    Expr::Unsafe(Box::new(rust::Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            func: Box::new(Expr::Var("std::mem::zeroed".into())),
            args: Vec::new(),
            binding: CallBinding::Generated,
        })),
    }))
}

fn lower_aggregate(
    cx: &Context,
    ty: &ir::Type,
    members: &[ir::AggregateMember],
    zero_fill: bool,
) -> Result<Expr> {
    let unsupported = || super::Error::Unsupported(format!("aggregate of {ty}"));
    let lowered_ty = lower_type(cx, ty)?;
    let fields = record_fields(cx, ty);
    let resolved = resolve_type(cx, ty);
    if fields.is_none() && !matches!(resolved, ir::Type::Array { .. }) {
        return Err(unsupported());
    }
    let field_name = |index: usize| {
        fields
            .and_then(|fields| fields.get(index))
            .and_then(|field| field.name.clone())
            .ok_or_else(unsupported)
    };
    let complete = !zero_fill && members.iter().enumerate().all(|(position, member)| {
        matches!(
            (&member.target, fields),
            (ir::AggregateTarget::Field(index), Some(_)) if *index == position
        ) || matches!(
            (&member.target, resolved),
            (ir::AggregateTarget::Index(index), ir::Type::Array { .. }) if *index == position as u64
        )
    });
    if complete {
        let values = members
            .iter()
            .map(|member| lower_value(&member.value, cx))
            .collect::<Result<Vec<_>>>()?;
        return Ok(match (&lowered_ty, fields) {
            (rust::Type::Custom(name), Some(_)) => Expr::StructLit {
                name: name.clone(),
                fields: (0..values.len())
                    .map(field_name)
                    .collect::<Result<Vec<_>>>()?
                    .into_iter()
                    .zip(values)
                    .collect(),
            },
            _ => Expr::ArrayLit(values),
        });
    }
    let target = cx.next_temp();
    let mut stmts = vec![Stmt::Let {
        name: target.clone(),
        mutable: true,
        ty: Some(lowered_ty),
        init: Some(zeroed()),
    }];
    for member in members {
        let place = match member.target {
            ir::AggregateTarget::Field(index) => Expr::Field {
                base: Box::new(Expr::Var(target.as_str().into())),
                field: field_name(index)?,
            },
            ir::AggregateTarget::Index(index) => Expr::Index {
                base: Box::new(Expr::Var(target.as_str().into())),
                index: Box::new(Expr::Value(rust::RustValue::U128(index.into()))),
            },
            ir::AggregateTarget::Range { .. } => return Err(unsupported()),
        };
        stmts.push(Stmt::Assign {
            target: place,
            value: lower_value(&member.value, cx)?,
        });
    }
    Ok(Expr::Block(Box::new(rust::Block {
        stmts,
        tail: Some(Box::new(Expr::Var(target.as_str().into()))),
    })))
}

fn lower_place(place: &ir::Place, cx: &Context) -> Result<Expr> {
    match place.kind {
        PlaceKind::Binding(id) => {
            let binding = Expr::Var(binding_name(id, &cx.bindings).as_str().into());
            Ok(if cx.over_aligned.contains_key(&id) {
                Expr::TupleField {
                    base: Box::new(binding),
                    index: 0,
                }
            } else {
                binding
            })
        }
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

fn place_is_unsafe(cx: &Context, place: &ir::Place) -> bool {
    match &place.kind {
        PlaceKind::Deref(_) => true,
        PlaceKind::Field { base, .. } => place_is_unsafe(cx, base),
        _ => place_is_static(cx, place),
    }
}

fn place_is_static(cx: &Context, place: &ir::Place) -> bool {
    match &place.kind {
        PlaceKind::Binding(id) => cx.statics.contains(id),
        PlaceKind::Field { base, .. } => place_is_static(cx, base),
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
                ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F80)) => {
                    lower_type(cx, ty)?;
                    return Ok(long_double_literal(*bits));
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
