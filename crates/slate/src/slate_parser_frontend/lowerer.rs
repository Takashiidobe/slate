use crate::backend::rust_ast::{self as rust, BinOp, Expr, FnDef, FnParam, Item, Prim, Stmt};
use crate::function_identity::CallBinding;
use crate::function_identity::FunctionIdentity;
use slate_parser::ir::{self, BindingId, Number, PlaceKind, ValueKind};
use std::collections::HashMap;

type Result<T> = std::result::Result<T, super::Error>;

struct FunctionName {
    rust: String,
    is_extern: bool,
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
    let mut items = Vec::new();
    let mut externs = Vec::new();
    for function in &module.functions {
        let Some(body) = &function.body else {
            let decl = lower_extern(function, &bindings);
            if let Some(decl) = function_barrier(&mut report, &function.name, false, decl)? {
                externs.push(decl);
            }
            continue;
        };
        let item = lower_function(function, body, &names, &bindings, &strings);
        if let Some(item) = function_barrier(&mut report, &function.name, true, item)? {
            items.push(item);
        }
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
    if names.values().any(|name| name.rust == "__slate_main") {
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

fn lower_extern(
    function: &ir::Function,
    bindings: &HashMap<BindingId, String>,
) -> Result<rust::ExternDecl> {
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
                    name: binding_name(parameter.value.id, bindings),
                    mutable: false,
                    ty: lower_type(&parameter.ty)?,
                })
            })
            .collect::<Result<Vec<_>>>()?,
        variadic: *variadic,
        ret: function.return_type.as_ref().map(lower_type).transpose()?,
        safe: false,
    }))
}

fn lower_function(
    function: &ir::Function,
    body: &[slate_parser::ast::Span<ir::Statement>],
    names: &HashMap<BindingId, FunctionName>,
    bindings: &HashMap<BindingId, String>,
    strings: &HashMap<BindingId, Vec<u8>>,
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
                name: binding_name(param.value.id, bindings),
                mutable: true,
                ty: lower_type(&param.ty)?,
            })
        })
        .collect::<Result<Vec<_>>>()?;
    let mut continue_labels = Vec::new();
    let mut statements = body
        .iter()
        .map(|statement| lower_statement(statement, names, bindings, strings, &mut continue_labels))
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
        name: names[&function.id].rust.clone(),
        params,
        ret: function.return_type.as_ref().map(lower_type).transpose()?,
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

fn lower_type(ty: &ir::Type) -> Result<rust::Type> {
    let primitive = match ty {
        ir::Type::Void => return Ok(rust::Type::Unit),
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
                params: parameters.iter().map(lower_type).collect::<Result<_>>()?,
                ret: Box::new(match return_type {
                    Some(ret) => lower_type(ret)?,
                    None => rust::Type::Unit,
                }),
            });
        }
        ir::Type::Pointer {
            pointee, is_const, ..
        } => {
            return Ok(rust::Type::Ptr {
                mutable: !is_const,
                inner: Box::new(lower_type(pointee)?),
            });
        }
        ir::Type::Array {
            element,
            length: Some(length),
        } => {
            return Ok(rust::Type::Array {
                elem: Box::new(lower_type(element)?),
                len: *length,
            });
        }
        _ => return Err(super::Error::Unsupported(format!("type {ty}"))),
    };
    Ok(rust::Type::Prim(primitive))
}

fn lower_statement(
    statement: &ir::Statement,
    names: &HashMap<BindingId, FunctionName>,
    bindings: &HashMap<BindingId, String>,
    strings: &HashMap<BindingId, Vec<u8>>,
    continue_labels: &mut Vec<Option<rust::Label>>,
) -> Result<Stmt> {
    Ok(match statement {
        ir::Statement::Temporary {
            id,
            ty,
            initializer,
            ..
        } => Stmt::Let {
            name: binding_name(*id, bindings),
            mutable: false,
            ty: Some(lower_type(ty)?),
            init: initializer
                .as_ref()
                .map(|value| lower_value(value, names, bindings, strings))
                .transpose()?,
        },
        ir::Statement::Let(variable) => {
            let init = variable
                .initializer
                .as_ref()
                .map(|value| lower_value(value, names, bindings, strings))
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
                        ty: lower_type(element)?,
                    }),
                    len: *length as usize,
                }),
                (None, _) => None,
            };
            Stmt::Let {
                name: binding_name(variable.id, bindings),
                mutable: true,
                ty: Some(lower_type(&variable.ty)?),
                init,
            }
        }
        ir::Statement::Expression(value) => {
            if matches!(value.node.value, ValueKind::Void) {
                Stmt::Block(rust::Block::default())
            } else {
                Stmt::Expr(lower_value(value, names, bindings, strings)?)
            }
        }
        ir::Statement::Write { place, value, .. } => {
            let target = lower_place(place, names, bindings, strings)?;
            let value = lower_value(value, names, bindings, strings)?;
            let assignment = Stmt::Assign { target, value };
            if matches!(place.kind, PlaceKind::Deref(_)) {
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
                .map(|value| lower_value(value, names, bindings, strings))
                .transpose()?,
        ),
        ir::Statement::Block(body) => Stmt::Scope {
            body: body
                .iter()
                .map(|statement| {
                    lower_statement(statement, names, bindings, strings, continue_labels)
                })
                .collect::<Result<Vec<_>>>()?,
        },
        ir::Statement::If {
            condition,
            then_body,
            else_body,
        } => Stmt::If {
            cond: lower_condition(condition, names, bindings, strings)?,
            then_body: lower_statement_list(then_body, names, bindings, strings, continue_labels)?,
            else_body: else_body
                .as_ref()
                .map(|body| lower_statement_list(body, names, bindings, strings, continue_labels))
                .transpose()?
                .unwrap_or_default(),
        },
        ir::Statement::While {
            condition, body, ..
        } => {
            continue_labels.push(None);
            let body = lower_statement_list(body, names, bindings, strings, continue_labels)?;
            continue_labels.pop();
            let (mut prefix, condition) = lower_evaluation(condition, names, bindings, strings)?;
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
            let mut statements =
                lower_statement_list(init, names, bindings, strings, continue_labels)?;
            let label = rust::Label::new(format!("__slate_continue_{}", id.0));
            continue_labels.push(Some(label.clone()));
            let body = lower_statement_list(body, names, bindings, strings, continue_labels)?;
            continue_labels.pop();
            let mut loop_body = Vec::new();
            if let Some(condition) = condition {
                let (prefix, condition) = lower_evaluation(condition, names, bindings, strings)?;
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
                loop_body.extend(lower_evaluation_statements(
                    increment, names, bindings, strings,
                )?);
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
            let body = lower_statement_list(body, names, bindings, strings, continue_labels)?;
            continue_labels.pop();
            let mut loop_body = vec![Stmt::LabeledBlock { label, body }];
            let (prefix, condition) = lower_evaluation(condition, names, bindings, strings)?;
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
    names: &HashMap<BindingId, FunctionName>,
    bindings: &HashMap<BindingId, String>,
    strings: &HashMap<BindingId, Vec<u8>>,
    continue_labels: &mut Vec<Option<rust::Label>>,
) -> Result<Vec<Stmt>> {
    statements
        .iter()
        .map(|statement| lower_statement(statement, names, bindings, strings, continue_labels))
        .collect()
}

fn lower_evaluation_statements(
    evaluation: &ir::Evaluation,
    names: &HashMap<BindingId, FunctionName>,
    bindings: &HashMap<BindingId, String>,
    strings: &HashMap<BindingId, Vec<u8>>,
) -> Result<Vec<Stmt>> {
    let mut statements = lower_statement_list(
        &evaluation.statements,
        names,
        bindings,
        strings,
        &mut Vec::new(),
    )?;
    if !matches!(evaluation.value.node.value, ValueKind::Void) {
        statements.push(Stmt::Expr(lower_value(
            &evaluation.value,
            names,
            bindings,
            strings,
        )?));
    }
    Ok(statements)
}

fn lower_evaluation(
    evaluation: &ir::Evaluation,
    names: &HashMap<BindingId, FunctionName>,
    bindings: &HashMap<BindingId, String>,
    strings: &HashMap<BindingId, Vec<u8>>,
) -> Result<(Vec<Stmt>, Expr)> {
    let statements = lower_statement_list(
        &evaluation.statements,
        names,
        bindings,
        strings,
        &mut Vec::new(),
    )?;
    let condition = lower_condition(&evaluation.value, names, bindings, strings)?;
    Ok((statements, condition))
}

fn lower_condition(
    value: &ir::Value,
    names: &HashMap<BindingId, FunctionName>,
    bindings: &HashMap<BindingId, String>,
    strings: &HashMap<BindingId, Vec<u8>>,
) -> Result<Expr> {
    let condition = lower_value(value, names, bindings, strings)?;
    if matches!(value.ty, ir::Type::Bool) {
        Ok(condition)
    } else {
        Ok(Expr::Binary {
            op: BinOp::Ne,
            lhs: Box::new(condition),
            rhs: Box::new(lower_number(&Number::Integer(0u32.into()), &value.ty)?),
        })
    }
}

fn lower_value(
    value: &ir::Value,
    names: &HashMap<BindingId, FunctionName>,
    bindings: &HashMap<BindingId, String>,
    strings: &HashMap<BindingId, Vec<u8>>,
) -> Result<Expr> {
    Ok(match &value.node.value {
        ValueKind::Constant(number) => lower_number(number, &value.ty)?,
        ValueKind::Read {
            place,
            ordering: None,
        } => {
            let place = lower_place(place, names, bindings, strings)?;
            if matches!(
                value.node.value,
                ValueKind::Read {
                    place: ir::Place {
                        kind: PlaceKind::Deref(_),
                        ..
                    },
                    ..
                }
            ) {
                Expr::Unsafe(Box::new(rust::Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(place)),
                }))
            } else {
                place
            }
        }
        ValueKind::Copy { operand, .. } => lower_value(operand, names, bindings, strings)?,
        ValueKind::AddressOf(place) => match place.kind {
            PlaceKind::Deref(ref pointer) => lower_value(pointer, names, bindings, strings)?,
            _ => Expr::AddrOf {
                mutable: true,
                expr: Box::new(lower_place(place, names, bindings, strings)?),
            },
        },
        ValueKind::Convert { operand, .. } => Expr::Cast {
            expr: Box::new(lower_value(operand, names, bindings, strings)?),
            ty: lower_type(&value.ty)?,
        },
        ValueKind::ArrayDecay { place, .. } => {
            let PlaceKind::Binding(id) = place.kind else {
                return Err(super::Error::Unsupported(format!("array decay {place:?}")));
            };
            let Some(bytes) = strings.get(&id) else {
                return Ok(Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::Var(binding_name(id, bindings).as_str().into())),
                        method: if matches!(value.ty, ir::Type::Pointer { is_const: true, .. }) {
                            "as_ptr"
                        } else {
                            "as_mut_ptr"
                        }
                        .into(),
                        args: Vec::new(),
                    }),
                    ty: lower_type(&value.ty)?,
                });
            };
            Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::ByteStr(bytes.clone())),
                    method: "as_ptr".into(),
                    args: Vec::new(),
                }),
                ty: lower_type(&value.ty)?,
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
            lhs: Box::new(lower_value(left, names, bindings, strings)?),
            rhs: Box::new(lower_value(right, names, bindings, strings)?),
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
                expr: Box::new(lower_value(operand, names, bindings, strings)?),
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
            let operand = lower_value(operand, names, bindings, strings)?;
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
                lhs: Box::new(lower_condition(left, names, bindings, strings)?),
                rhs: Box::new(lower_condition(right, names, bindings, strings)?),
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
            lhs: Box::new(lower_value(left, names, bindings, strings)?),
            rhs: Box::new(lower_value(right, names, bindings, strings)?),
        },
        ValueKind::PointerOffset {
            pointer,
            amount,
            subtract,
            ..
        } => {
            let offset = Expr::Cast {
                expr: Box::new(lower_value(amount, names, bindings, strings)?),
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
                    recv: Box::new(lower_value(pointer, names, bindings, strings)?),
                    method: "offset".into(),
                    args: vec![offset],
                })),
            }))
        }
        ValueKind::FunctionDecay {
            place:
                ir::Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
        } => match names.get(id) {
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
            lower_type(&value.ty)?;
            Expr::Var("None".into())
        }
        ValueKind::Call {
            callee, arguments, ..
        } => {
            let func = match callee {
                ir::Callee::Direct(id) => Expr::Var(
                    names
                        .get(id)
                        .ok_or_else(|| {
                            super::Error::Unsupported(format!("unknown callee %{}", id.0))
                        })?
                        .rust
                        .as_str()
                        .into(),
                ),
                ir::Callee::Indirect(pointer) => Expr::MethodCall {
                    recv: Box::new(lower_value(pointer, names, bindings, strings)?),
                    method: "unwrap".into(),
                    args: Vec::new(),
                },
            };
            let call = Expr::Call {
                func: Box::new(func),
                args: arguments
                    .iter()
                    .map(|argument| lower_value(argument, names, bindings, strings))
                    .collect::<Result<Vec<_>>>()?,
                binding: CallBinding::unknown(),
            };
            let unsafe_call = match callee {
                ir::Callee::Direct(id) => names.get(id).is_some_and(|name| name.rust == "printf"),
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

fn lower_place(
    place: &ir::Place,
    names: &HashMap<BindingId, FunctionName>,
    bindings: &HashMap<BindingId, String>,
    strings: &HashMap<BindingId, Vec<u8>>,
) -> Result<Expr> {
    match place.kind {
        PlaceKind::Binding(id) => Ok(Expr::Var(binding_name(id, bindings).as_str().into())),
        PlaceKind::Deref(ref pointer) => Ok(Expr::Unary {
            op: rust::UnaryOp::Deref,
            expr: Box::new(lower_value(pointer, names, bindings, strings)?),
        }),
        PlaceKind::Index {
            ref base,
            ref index,
        } => Ok(Expr::Index {
            base: Box::new(lower_value(base, names, bindings, strings)?),
            index: Box::new(lower_value(index, names, bindings, strings)?),
        }),
        _ => Err(super::Error::Unsupported(format!("place {place:?}"))),
    }
}

fn lower_number(number: &Number, ty: &ir::Type) -> Result<Expr> {
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
        ty: lower_type(ty)?,
    })
}
