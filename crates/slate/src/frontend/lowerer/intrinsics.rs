use super::*;
use intrinsics_table::IntrinsicSignature;
use slate_parser::target_info::TargetFamily;

pub(super) fn builtin_intrinsic(
    module: &ir::Module,
    function: &Span<ir::Function>,
) -> Option<&'static IntrinsicSignature> {
    if function.body.is_some()
        || !module
            .metadata
            .get(&function.id)
            .is_some_and(|entries| entries.iter().any(|(key, _)| key == "c_builtin"))
    {
        return None;
    }
    let target_intrinsics = match module.target.family {
        TargetFamily::X86 | TargetFamily::X86_64 => intrinsics_table::X86_INTRINSICS,
        TargetFamily::AArch64 => intrinsics_table::AARCH64_INTRINSICS,
        TargetFamily::Arm32 => intrinsics_table::ARM_INTRINSICS,
    };
    target_intrinsics
        .iter()
        .chain(intrinsics_table::GENERAL_INTRINSICS)
        .find(|intrinsic| intrinsic.builtins.contains(&function.name.as_str()))
}

fn scalar_type(llvm_type: &str) -> Option<rust::Type> {
    let primitive = match llvm_type {
        "void" => return Some(rust::Type::Unit),
        "i1" => Prim::Bool,
        "i8" => Prim::I8,
        "i16" => Prim::I16,
        "i32" => Prim::I32,
        "i64" => Prim::I64,
        "i128" => Prim::I128,
        "float" => Prim::F32,
        "double" => Prim::F64,
        _ => return None,
    };
    Some(rust::Type::Prim(primitive))
}

fn scalar_matches(tables: &Tables<'_>, ty: &ir::Type, llvm_type: &str) -> bool {
    match (tables.resolve_type(ty), llvm_type) {
        (ir::Type::Void, "void") | (ir::Type::Bool, "i1") => true,
        (ir::Type::Numeric(ir::NumericType::Integer { width, .. }), llvm_type) => {
            llvm_type
                .strip_prefix('i')
                .and_then(|width| width.parse::<u32>().ok())
                == Some(*width)
        }
        (ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F32)), "float")
        | (ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F64)), "double") => true,
        _ => false,
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_intrinsic(
        &mut self,
        id: BindingId,
        intrinsic: &IntrinsicSignature,
        arguments: &[ir::Value],
        return_type: &ir::Type,
    ) -> Result<Expr> {
        let unsupported = || Construct::Function {
            name: self.tables.names[&id].rust.clone(),
            detail: format!(
                "intrinsic {} requires unsupported signature adaptation",
                intrinsic.name
            ),
        };
        let params = intrinsic
            .params
            .filter(|_| !intrinsic.overloaded)
            .ok_or_else(unsupported)?;
        let ret = intrinsic.ret.ok_or_else(unsupported)?;
        if params.len() != arguments.len()
            || !scalar_matches(self.tables, return_type, ret)
            || params.iter().zip(arguments).any(|(param, argument)| {
                param.immarg || !scalar_matches(self.tables, &argument.ty, param.llvm_type)
            })
        {
            return Err(unsupported().into());
        }
        let ret = scalar_type(ret).ok_or_else(unsupported)?;
        let mut args = Vec::new();
        let mut fixed = Vec::new();
        for (index, (param, argument)) in params.iter().zip(arguments).enumerate() {
            let ty = scalar_type(param.llvm_type).ok_or_else(unsupported)?;
            let expr = self.lower_value(argument)?;
            args.push(if self.lower_type(&argument.ty)? == ty {
                expr
            } else {
                Expr::Cast {
                    expr: Box::new(expr),
                    ty: ty.clone(),
                }
            });
            fixed.push(FnParam {
                name: format!("arg{index}"),
                mutable: false,
                ty,
            });
        }
        let name = format!("__slate_intrinsic_{}", id.0);
        self.dependencies.intrinsics.insert(
            name.clone(),
            rust::ExternFnDecl {
                attrs: vec![Attr::LinkName(intrinsic.name.to_owned())],
                name: name.clone(),
                identity: FunctionIdentity::Unknown,
                declared_type: None,
                trusted_headers: Default::default(),
                params: fixed,
                variadic: false,
                ret: (ret != rust::Type::Unit).then_some(ret.clone()),
                safe: false,
            },
        );
        let call = Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::Call {
                func: Box::new(Expr::Var(name.into())),
                args,
                binding: CallBinding::Generated,
            })),
        }));
        let to = self.lower_type(return_type)?;
        Ok(if to == ret {
            call
        } else {
            Expr::Cast {
                expr: Box::new(call),
                ty: to,
            }
        })
    }
}
