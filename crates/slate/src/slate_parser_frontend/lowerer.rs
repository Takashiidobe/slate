use crate::backend::rust_ast::{self as rust, BinOp, Expr, FnDef, FnParam, Item, Prim, Stmt};
use crate::function_identity::CallBinding;
use crate::function_identity::FunctionIdentity;
use slate_parser::ir::{self, BindingId, Number, PlaceKind, ValueKind};
use std::collections::HashMap;

type Result<T> = std::result::Result<T, super::Error>;

pub fn lower(module: &ir::Module) -> Result<rust::Program> {
    if !module.types.is_empty() || !module.asm.is_empty() {
        return Err(super::Error::Unsupported("types or module assembly".into()));
    }
    let strings = module
        .globals
        .iter()
        .map(|global| {
            let Some(ir::ValueKind::CodeUnits(units)) = global
                .variable
                .initializer
                .as_ref()
                .map(|value| &value.node.value)
            else {
                return Err(super::Error::Unsupported(format!(
                    "global {}",
                    global.variable.name
                )));
            };
            let bytes = units
                .iter()
                .map(|unit| {
                    u8::try_from(*unit).map_err(|_| super::Error::Unsupported("wide string".into()))
                })
                .collect::<Result<Vec<_>>>()?;
            Ok((global.variable.id, bytes))
        })
        .collect::<Result<HashMap<_, _>>>()?;
    let names: HashMap<_, _> = module
        .functions
        .iter()
        .map(|function| {
            (
                function.value.id,
                if function.name == "main" {
                    "__slate_main".into()
                } else {
                    function.name.clone()
                },
            )
        })
        .collect();
    let mut items = Vec::new();
    let mut externs = Vec::new();
    for function in &module.functions {
        let Some(body) = &function.body else {
            let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
                return Err(super::Error::Unsupported(format!(
                    "unprototyped declaration {}",
                    function.name
                )));
            };
            externs.push(rust::ExternDecl::Fn(rust::ExternFnDecl {
                attrs: Vec::new(),
                name: function.name.clone(),
                identity: FunctionIdentity::Unknown,
                declared_type: None,
                trusted_headers: Default::default(),
                params: fixed
                    .iter()
                    .map(|parameter| {
                        Ok(FnParam {
                            name: binding_name(parameter.value.id),
                            mutable: false,
                            ty: lower_type(&parameter.ty)?,
                        })
                    })
                    .collect::<Result<Vec<_>>>()?,
                variadic: *variadic,
                ret: function.return_type.as_ref().map(lower_type).transpose()?,
                safe: false,
            }));
            continue;
        };
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
                    name: binding_name(param.value.id),
                    mutable: true,
                    ty: lower_type(&param.ty)?,
                })
            })
            .collect::<Result<Vec<_>>>()?;
        let mut statements = body
            .iter()
            .map(|statement| lower_statement(statement, &names, &strings))
            .collect::<Result<Vec<_>>>()?;
        if matches!(function.fallthrough, Some(ir::Fallthrough::ReturnZero))
            && !matches!(statements.last(), Some(Stmt::Return(_)))
        {
            statements.push(Stmt::Return(Some(Expr::Value(rust::RustValue::I64(0)))));
        }
        items.push(Item::Fn(FnDef {
            attrs: Vec::new(),
            vis: rust::Visibility::Private,
            unsafe_: false,
            abi: None,
            name: names[&function.value.id].clone(),
            params,
            ret: function.return_type.as_ref().map(lower_type).transpose()?,
            body: statements,
        }));
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
    if names.values().any(|name| name == "__slate_main") {
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

fn binding_name(id: BindingId) -> String {
    format!("__s{}", id.0)
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
    names: &HashMap<BindingId, String>,
    strings: &HashMap<BindingId, Vec<u8>>,
) -> Result<Stmt> {
    Ok(match statement {
        ir::Statement::Let(variable) => Stmt::Let {
            name: binding_name(variable.id),
            mutable: true,
            ty: Some(lower_type(&variable.ty)?),
            init: variable
                .initializer
                .as_ref()
                .map(|value| lower_value(value, names, strings))
                .transpose()?,
        },
        ir::Statement::Expression(value) => Stmt::Expr(lower_value(value, names, strings)?),
        ir::Statement::Return(value) => Stmt::Return(
            value
                .as_ref()
                .map(|value| lower_value(value, names, strings))
                .transpose()?,
        ),
        ir::Statement::Block(body) => Stmt::Scope {
            body: body
                .iter()
                .map(|statement| lower_statement(statement, names, strings))
                .collect::<Result<Vec<_>>>()?,
        },
        ir::Statement::Null => Stmt::Block(rust::Block::default()),
        _ => {
            return Err(super::Error::Unsupported(format!(
                "statement {statement:?}"
            )));
        }
    })
}

fn lower_value(
    value: &ir::Value,
    names: &HashMap<BindingId, String>,
    strings: &HashMap<BindingId, Vec<u8>>,
) -> Result<Expr> {
    Ok(match &value.node.value {
        ValueKind::Constant(number) => lower_number(number, &value.ty)?,
        ValueKind::Read {
            place,
            ordering: None,
        } => lower_place(place)?,
        ValueKind::Copy { operand, .. } => lower_value(operand, names, strings)?,
        ValueKind::Convert { operand, .. } => Expr::Cast {
            expr: Box::new(lower_value(operand, names, strings)?),
            ty: lower_type(&value.ty)?,
        },
        ValueKind::ArrayDecay { place, .. } => {
            let PlaceKind::Binding(id) = place.kind else {
                return Err(super::Error::Unsupported(format!("array decay {place:?}")));
            };
            let bytes = strings
                .get(&id)
                .ok_or_else(|| super::Error::Unsupported(format!("array binding %{}", id.0)))?;
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
            lhs: Box::new(lower_value(left, names, strings)?),
            rhs: Box::new(lower_value(right, names, strings)?),
        },
        ValueKind::Call {
            callee: ir::Callee::Direct(id),
            arguments,
            ..
        } => {
            let call = Expr::Call {
                func: Box::new(Expr::Var(
                    names
                        .get(id)
                        .ok_or_else(|| {
                            super::Error::Unsupported(format!("unknown callee %{}", id.0))
                        })?
                        .as_str()
                        .into(),
                )),
                args: arguments
                    .iter()
                    .map(|argument| lower_value(argument, names, strings))
                    .collect::<Result<Vec<_>>>()?,
                binding: CallBinding::unknown(),
            };
            if names.get(id).is_some_and(|name| name == "printf") {
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

fn lower_place(place: &ir::Place) -> Result<Expr> {
    match place.kind {
        PlaceKind::Binding(id) => Ok(Expr::Var(binding_name(id).as_str().into())),
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
        _ => return Err(super::Error::Unsupported(format!("constant {number:?}"))),
    };
    Ok(Expr::Cast {
        expr: Box::new(Expr::Value(value)),
        ty: lower_type(ty)?,
    })
}
