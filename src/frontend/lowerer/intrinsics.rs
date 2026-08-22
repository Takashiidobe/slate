use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn lower_asm(&mut self, op: &inst::Asm) {
        let operands: Vec<&str> = op
            .asm_operands
            .iter()
            .flatten()
            .map(String::as_str)
            .collect();
        if op.res.is_none() && operands.is_empty() && !asm_template_has_placeholders(&op.asm_string)
        {
            let Ok(template) = String::from_utf8(decode_cir_string(&op.asm_string)) else {
                self.parent
                    .ctx
                    .diagnostics
                    .error("lower: inline assembly template is not valid UTF-8");
                return;
            };
            self.push_stmt(Self::unsafe_stmt(Stmt::Expr(asm_macro_expr(
                template.replace("$$", "$"),
                cir_asm_dialect(op.asm_flavor),
            ))));
            return;
        }
        if self.lower_extended_asm(op, &operands) {
            return;
        }
        let Some(result) = &op.res else {
            return;
        };
        let expr = operands
            .first()
            .map(|operand| self.operand_expr(operand))
            .unwrap_or_else(|| {
                op.res_ty
                    .as_ref()
                    .map(|ty| self.parent.rust_type(ty))
                    .map(|ty| self.parent.default_value_expr(&ty))
                    .unwrap_or(Expr::Value(RustValue::I64(0)))
            });
        self.materialize_expr(result, expr, op.res_ty.as_ref());
    }

    fn lower_extended_asm(&mut self, op: &inst::Asm, input_operands: &[&str]) -> bool {
        let Ok(template) = String::from_utf8(decode_cir_string(&op.asm_string)) else {
            self.parent
                .ctx
                .diagnostics
                .error("lower: inline assembly template is not valid UTF-8");
            return true;
        };
        let label_count = asm_template_label_count(&template);
        let asm_goto = if label_count == 0 {
            None
        } else {
            let Some(asm_goto) = self.asm_gotos.pop_front() else {
                self.parent
                    .ctx
                    .diagnostics
                    .error("lower: asm goto labels are missing from the Clang AST");
                return true;
            };
            Some(asm_goto)
        };
        if asm_goto
            .as_ref()
            .is_some_and(|asm_goto| asm_goto.labels.len() != label_count)
        {
            self.parent
                .ctx
                .diagnostics
                .error("lower: asm goto label count differs between CIR and the Clang AST");
            return true;
        }
        let constraints = op
            .constraints
            .split(',')
            .map(str::trim)
            .take_while(|constraint| !constraint.starts_with("~{"))
            .collect::<Vec<_>>();
        let Some(output_count) = constraints.len().checked_sub(input_operands.len()) else {
            return false;
        };
        let result_count = usize::from(op.res.is_some());
        if output_count != result_count && !(result_count == 1 && output_count > 1) {
            return false;
        }
        if constraints[..output_count]
            .iter()
            .any(|constraint| !matches!(*constraint, "=r" | "=&r"))
        {
            return false;
        }
        let mut tied_outputs = vec![None; output_count];
        for (operand_index, constraint) in constraints[output_count..].iter().enumerate() {
            if let Ok(output_index) = constraint.parse::<usize>() {
                if output_index >= output_count || tied_outputs[output_index].is_some() {
                    return false;
                }
                tied_outputs[output_index] = Some(operand_index);
            } else if !matches!(*constraint, "r" | "i") {
                return false;
            }
        }
        let result_types: Vec<CirType> = if output_count == 0 {
            Vec::new()
        } else {
            let Some(result_types) =
                asm_output_types(op.res_ty.as_ref(), &self.parent.aliases, output_count)
            else {
                return false;
            };
            result_types.into_iter().cloned().collect()
        };
        let Some(operand_types): Option<Vec<_>> = input_operands
            .iter()
            .map(|operand| self.value_type(operand).cloned())
            .collect()
        else {
            return false;
        };
        if constraints[output_count..]
            .iter()
            .zip(input_operands)
            .any(|(constraint, operand)| {
                *constraint == "i" && self.known_arith_value(operand).is_none()
            })
        {
            return false;
        }
        let mut slot_to_rust = vec![0; constraints.len() + label_count];
        for (output_index, slot) in slot_to_rust.iter_mut().take(output_count).enumerate() {
            *slot = output_index;
        }
        let mut next_rust_operand = output_count;
        for (operand_index, constraint) in constraints[output_count..].iter().enumerate() {
            let slot = output_count + operand_index;
            if let Ok(output_index) = constraint.parse::<usize>() {
                slot_to_rust[slot] = output_index;
            } else {
                slot_to_rust[slot] = next_rust_operand;
                next_rust_operand += 1;
            }
        }
        for slot in &mut slot_to_rust[constraints.len()..] {
            *slot = next_rust_operand;
            next_rust_operand += 1;
        }
        let mut template_constraints = constraints.clone();
        template_constraints.extend(std::iter::repeat_n("X", label_count));
        let mut template_types: Vec<Type> = result_types
            .iter()
            .chain(operand_types.iter())
            .map(|ty| self.parent.rust_type(ty))
            .collect();
        template_types.extend(std::iter::repeat_n(Type::Unit, label_count));
        let Some(template) = translate_asm_template(
            &template,
            &slot_to_rust,
            &template_constraints,
            &template_types,
        ) else {
            return false;
        };
        let mut operands = Vec::new();
        let mut output_exprs = Vec::new();
        let mut output_names = Vec::new();
        if asm_goto.is_some()
            && output_count > 0
            && (result_count != output_count
                || op
                    .res
                    .as_ref()
                    .is_some_and(|result| !self.asm_output_places.contains_key(result)))
        {
            self.parent
                .ctx
                .diagnostics
                .error("lower: asm goto output does not have a direct CIR destination");
            return true;
        }
        for (output_index, constraint) in constraints[..output_count].iter().enumerate() {
            let direct_output = (output_index == 0)
                .then_some(op.res.as_ref())
                .flatten()
                .and_then(|result| self.asm_output_places.get(result))
                .cloned();
            let (output, output_name) = if let Some(output) = direct_output {
                (output, None)
            } else {
                let name = self.next_temp();
                self.push_stmt(Stmt::Let {
                    name: name.clone(),
                    mutable: false,
                    ty: Some(self.parent.rust_type(&result_types[output_index])),
                    init: None,
                });
                (Expr::Var(name.clone().into()), Some(name))
            };
            if let Some(operand_index) = tied_outputs[output_index] {
                operands.push(AsmOperand::InOut {
                    reg: AsmReg::Class("reg".into()),
                    late: *constraint == "=r",
                    input: self.operand_expr(input_operands[operand_index]),
                    output,
                });
            } else {
                operands.push(AsmOperand::Out {
                    reg: AsmReg::Class("reg".into()),
                    late: *constraint == "=r",
                    value: output,
                });
            }
            output_exprs.push(
                operands
                    .last()
                    .and_then(|operand| match operand {
                        AsmOperand::Out { value, .. } => Some(value.clone()),
                        AsmOperand::InOut { output, .. } => Some(output.clone()),
                        _ => None,
                    })
                    .unwrap(),
            );
            output_names.push(output_name);
        }
        for (operand_index, constraint) in constraints[output_count..].iter().enumerate() {
            if constraint.parse::<usize>().is_ok() {
                continue;
            }
            let value = if *constraint == "i" {
                let value = self
                    .known_arith_value(input_operands[operand_index])
                    .unwrap();
                AsmOperand::Const(int_value_expr(value))
            } else {
                AsmOperand::In {
                    reg: AsmReg::Class("reg".into()),
                    value: self.operand_expr(input_operands[operand_index]),
                }
            };
            operands.push(value);
        }
        if let Some(asm_goto) = asm_goto {
            let Some(dispatch) = self.dispatch.as_ref() else {
                self.parent
                    .ctx
                    .diagnostics
                    .error("lower: asm goto requires dispatch control flow");
                return true;
            };
            for label in asm_goto.labels {
                let Some(state) = dispatch.label_to_state.get(&label).copied() else {
                    self.parent.ctx.diagnostics.error(format!(
                        "lower: asm goto target `{label}` is missing from CIR"
                    ));
                    return true;
                };
                operands.push(AsmOperand::Label {
                    state: Expr::Var(dispatch.state_var.clone().into()),
                    value: Expr::Value(RustValue::I64(state as i64)),
                    destination: dispatch.loop_label.clone(),
                });
            }
            if output_count > 0 {
                self.parent.uses_asm_goto_outputs.set(true);
            }
        }
        self.push_stmt(Self::unsafe_stmt(Stmt::InlineAsm(InlineAsm {
            template,
            dialect: cir_asm_dialect(op.asm_flavor),
            operands,
            raw: false,
        })));
        if output_count == result_count {
            for ((result, output), name) in op.res.iter().zip(output_exprs).zip(output_names) {
                self.values.insert(result.clone(), Val::Expr(output));
                if let Some(name) = name {
                    self.immutable_temps.insert(name);
                }
            }
        } else {
            self.asm_outputs
                .insert(op.res.clone().unwrap(), output_exprs);
            self.immutable_temps
                .extend(output_names.into_iter().flatten());
        }
        true
    }

    pub(super) fn lower_eh_setjmp(&mut self, op: &inst::EhSetjmp) {
        self.materialize_expr(&op.res, Expr::Value(RustValue::I64(0)), Some(&op.res_ty));
    }

    pub(super) fn lower_unsupported_value(
        &mut self,
        result: &str,
        result_ty: &CirType,
        note: &str,
    ) {
        self.materialize_expr(
            result,
            Expr::Macro {
                name: "panic".into(),
                args: vec![Expr::Str(format!("unsupported CIR op: {note}"))],
            },
            Some(result_ty),
        );
    }

    pub(super) fn vector_binary_expr(&self, lhs: &str, rhs: &str, len: u64, op: BinOp) -> Expr {
        let lhs = self.operand_expr(lhs);
        let rhs = self.operand_expr(rhs);
        Expr::ArrayLit(
            (0..len)
                .map(|i| Expr::Binary {
                    op,
                    lhs: Box::new(vector_index_expr(lhs.clone(), i)),
                    rhs: Box::new(vector_index_expr(rhs.clone(), i)),
                })
                .collect(),
        )
    }

    pub(super) fn lower_vec_extract(&mut self, op: &inst::VecExtract) {
        self.materialize_expr(
            &op.result,
            Expr::Index {
                base: Box::new(self.operand_expr(&op.vec)),
                index: Box::new(Expr::Cast {
                    expr: Box::new(self.operand_expr(&op.index)),
                    ty: Type::Prim(Prim::Usize),
                }),
            },
            Some(&op.result_ty),
        );
    }

    pub(super) fn lower_vec_insert(&mut self, op: &inst::VecInsert) {
        let Some((_, len)) = parse_cir_vector_type(&op.result_ty) else {
            self.lower_unsupported_value(&op.result, &op.result_ty, "cir.vec.insert");
            return;
        };
        let Some(index) = self
            .const_int_values
            .get(&op.index)
            .and_then(|i| u64::try_from(*i).ok())
            .filter(|i| *i < len)
        else {
            self.lower_unsupported_value(&op.result, &op.result_ty, "cir.vec.insert dynamic index");
            return;
        };
        let base = self.operand_expr(&op.vec);
        let value = self.operand_expr(&op.value);
        self.materialize_expr(
            &op.result,
            Expr::ArrayLit(
                (0..len)
                    .map(|i| {
                        if i == index {
                            value.clone()
                        } else {
                            vector_index_expr(base.clone(), i)
                        }
                    })
                    .collect(),
            ),
            Some(&op.result_ty),
        );
    }

    pub(super) fn lower_vec_shuffle(&mut self, op: &inst::VecShuffle) {
        let Some((_, len)) = parse_cir_vector_type(&op.result_ty) else {
            self.lower_unsupported_value(&op.result, &op.result_ty, "cir.vec.shuffle");
            return;
        };
        let indices = int_array_attr(&op.indices).unwrap_or_default();
        if indices.len() != len as usize {
            self.lower_unsupported_value(&op.result, &op.result_ty, "cir.vec.shuffle indices");
            return;
        }
        let lhs = self.operand_expr(&op.vec1);
        let rhs = self.operand_expr(&op.vec2);
        self.materialize_expr(
            &op.result,
            Expr::ArrayLit(
                indices
                    .into_iter()
                    .map(|index| {
                        if index < len {
                            vector_index_expr(lhs.clone(), index)
                        } else {
                            vector_index_expr(rhs.clone(), index - len)
                        }
                    })
                    .collect(),
            ),
            Some(&op.result_ty),
        );
    }

    pub(super) fn lower_vec_shuffle_dynamic(&mut self, op: &inst::VecShuffleDynamic) {
        self.lower_unsupported_value(&op.result, &op.result_ty, "cir.vec.shuffle.dynamic");
    }

    pub(super) fn lower_vec_create(&mut self, op: &inst::VecCreate) {
        let elems = op.elements.iter().map(|v| self.operand_expr(v)).collect();
        self.materialize_expr(&op.result, Expr::ArrayLit(elems), Some(&op.result_ty));
    }

    pub(super) fn lower_vec_splat(&mut self, op: &inst::VecSplat) {
        let Some((_, len)) = parse_cir_vector_type(&op.result_ty) else {
            self.lower_unsupported_value(&op.result, &op.result_ty, "cir.vec.splat");
            return;
        };
        let value = self.operand_expr(&op.value);
        self.materialize_expr(
            &op.result,
            Expr::ArrayLit((0..len).map(|_| value.clone()).collect()),
            Some(&op.result_ty),
        );
    }

    pub(super) fn lower_vec_cmp(&mut self, op: &inst::VecCmp) {
        let Some((elem_ty, len)) = parse_cir_vector_type(&op.result_ty) else {
            self.lower_unsupported_value(&op.result, &op.result_ty, "cir.vec.cmp");
            return;
        };
        let cmp = match op.kind {
            CmpOpKind::Lt => BinOp::Lt,
            CmpOpKind::Le => BinOp::Le,
            CmpOpKind::Gt => BinOp::Gt,
            CmpOpKind::Ge => BinOp::Ge,
            CmpOpKind::Eq => BinOp::Eq,
            CmpOpKind::Ne => BinOp::Ne,
            CmpOpKind::One | CmpOpKind::Uno => {
                self.lower_unsupported_value(&op.result, &op.result_ty, "cir.vec.cmp kind");
                return;
            }
        };
        let lhs = self.operand_expr(&op.lhs);
        let rhs = self.operand_expr(&op.rhs);
        let elem_rust_ty = self.parent.rust_type(elem_ty);
        self.materialize_expr(
            &op.result,
            Expr::ArrayLit(
                (0..len)
                    .map(|i| {
                        let l = vector_index_expr(lhs.clone(), i);
                        let r = vector_index_expr(rhs.clone(), i);
                        Expr::MethodCall {
                            recv: Box::new(Expr::Cast {
                                expr: Box::new(Expr::Binary {
                                    op: cmp,
                                    lhs: Box::new(l),
                                    rhs: Box::new(r),
                                }),
                                ty: elem_rust_ty.clone(),
                            }),
                            method: "wrapping_neg".into(),
                            args: vec![],
                        }
                    })
                    .collect(),
            ),
            Some(&op.result_ty),
        );
    }

    pub(super) fn lower_trap(&mut self) {
        self.push_stmt(Stmt::Expr(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(
                ["std", "process", "abort"].map(Ident::from),
            ))),
            args: Vec::new(),
        }));
    }

    pub(super) fn lower_unreachable(&mut self) {
        self.push_stmt(Stmt::Expr(Expr::Macro {
            name: "unreachable".into(),
            args: Vec::new(),
        }));
    }

    pub(super) fn lower_opaque_pointer(
        &mut self,
        result: &str,
        result_ty: &CirType,
        non_null: bool,
    ) {
        let ty = self.parent.rust_type(result_ty);
        let addr = if non_null { 1 } else { 0 };
        self.materialize_expr(
            result,
            Expr::Cast {
                expr: Box::new(Expr::Value(RustValue::Usize(addr))),
                ty,
            },
            Some(result_ty),
        );
    }

    pub(super) fn lower_is_constant(&mut self, op: &inst::IsConstant) {
        let is_constant = self.const_int_values.contains_key(&op.val)
            || self.values.get(&op.val).is_some_and(|value| match value {
                Val::Expr(expr) => matches!(
                    expr,
                    Expr::Value(_)
                        | Expr::Str(_)
                        | Expr::HexFloat(_)
                        | Expr::ByteStr(_)
                        | Expr::CStr(_)
                        | Expr::Path(_)
                ),
                Val::Global(_) => true,
            });
        let expr = self.bool_or_int_literal(is_constant, Some(&op.result_ty));
        self.materialize_expr(&op.result, expr, Some(&op.result_ty));
    }

    pub(super) fn lower_objsize(&mut self, op: &inst::Objsize) {
        let expr = if op.min {
            self.zero_literal(Some(&op.result_ty))
        } else {
            self.max_literal(Some(&op.result_ty))
        };
        self.materialize_expr(&op.result, expr, Some(&op.result_ty));
    }

    pub(super) fn bool_or_int_literal(&self, value: bool, cir_ty: Option<&CirType>) -> Expr {
        match cir_ty.map(|ty| self.parent.rust_type(ty)) {
            Some(Type::Prim(Prim::Bool)) => Expr::Value(RustValue::Bool(value)),
            _ => Expr::Value(RustValue::I64(if value { 1 } else { 0 })),
        }
    }

    pub(super) fn zero_literal(&self, cir_ty: Option<&CirType>) -> Expr {
        match cir_ty.map(|ty| self.parent.rust_type(ty)) {
            Some(Type::Prim(Prim::Bool)) => Expr::Value(RustValue::Bool(false)),
            _ => Expr::Value(RustValue::I64(0)),
        }
    }

    pub(super) fn max_literal(&self, cir_ty: Option<&CirType>) -> Expr {
        match cir_ty.map(|ty| self.parent.rust_type(ty)) {
            Some(Type::Prim(Prim::Bool)) => Expr::Value(RustValue::Bool(true)),
            Some(Type::Prim(prim)) => Expr::Path(Path::new([
                Ident::from(prim.spelling()),
                Ident::from("MAX"),
            ])),
            _ => Expr::Value(RustValue::I64(-1)),
        }
    }

    // Prototype: declare a per-signature `extern "unadjusted"` shim keyed by
    // `#[link_name = "llvm.<intrinsic_name>"]` and call it directly, using CIR's
    // own resolved operand/result types. No LLVM-side type table needed since
    // CIR already carries the concrete signature at the call site.
    pub(super) fn lower_call_llvm_intrinsic(&mut self, op: &inst::CallLlvmIntrinsic) {
        let param_types: Vec<Type> = op
            .arg_ops
            .iter()
            .map(|id| {
                self.value_type(id)
                    .map(|ty| self.parent.rust_type(ty))
                    .unwrap_or(Type::Prim(Prim::I64))
            })
            .collect();
        let ret_type = op
            .result_ty
            .as_ref()
            .map(|ty| self.parent.rust_type(ty))
            .filter(|ty| !matches!(ty, Type::CLib(c) if *c == CLibType::VOID));

        let sanitized = sanitize_ident(&op.intrinsic_name);
        let sig_key = format!("{sanitized}__{ret_type:?}__{param_types:?}");
        let sig_hash = {
            use std::hash::{Hash, Hasher};
            let mut hasher = std::collections::hash_map::DefaultHasher::new();
            sig_key.hash(&mut hasher);
            hasher.finish()
        };
        let shim_name = format!("__slate_intrinsic_{sanitized}_{sig_hash:x}");

        let raw_ret_type = op.result_ty.as_ref().map(|ty| self.parent.rust_type(ty));
        let link_name = mangled_link_name(&op.intrinsic_name, &raw_ret_type, &param_types);

        self.parent
            .llvm_intrinsic_shims
            .entry(shim_name.clone())
            .or_insert_with(|| ExternFnDecl {
                attrs: vec![RustAttr::LinkName(link_name)],
                identity: FunctionIdentity::Unknown,
                name: shim_name.clone(),
                declared_type: None,
                params: param_types
                    .iter()
                    .enumerate()
                    .map(|(i, ty)| FnParam {
                        name: format!("_{i}"),
                        mutable: false,
                        ty: ty.clone(),
                    })
                    .collect(),
                variadic: false,
                ret: ret_type.clone(),
                safe: false,
            });

        let args: Vec<Expr> = op.arg_ops.iter().map(|id| self.operand_expr(id)).collect();
        let call_expr = Self::unsafe_expr(Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Var(shim_name.into())),
            args,
        });
        match &op.result {
            Some(result) if ret_type.is_some() => {
                self.materialize_expr(result, call_expr, op.result_ty.as_ref())
            }
            _ => self.push_stmt(Self::unsafe_stmt(Stmt::Expr(call_expr))),
        }
    }
}

fn find_intrinsic_signature(llvm_name: &str) -> Option<&'static intrinsics_table::IntrinsicSignature> {
    [
        intrinsics_table::X86_INTRINSICS,
        intrinsics_table::AARCH64_INTRINSICS,
        intrinsics_table::ARM_INTRINSICS,
        intrinsics_table::RISCV_INTRINSICS,
    ]
    .into_iter()
    .find_map(|table| table.iter().find(|entry| entry.name == llvm_name))
}

fn mangle_llvm_type(ty: &Type) -> Option<String> {
    match ty {
        Type::Prim(Prim::Bool) => Some("i1".into()),
        Type::Prim(Prim::I8 | Prim::U8) => Some("i8".into()),
        Type::Prim(Prim::I16 | Prim::U16) => Some("i16".into()),
        Type::Prim(Prim::I32 | Prim::U32) => Some("i32".into()),
        Type::Prim(Prim::I64 | Prim::U64) => Some("i64".into()),
        Type::Prim(Prim::I128 | Prim::U128) => Some("i128".into()),
        Type::Prim(Prim::F32) => Some("f32".into()),
        Type::Prim(Prim::F64) => Some("f64".into()),
        Type::Array { elem, len } => Some(format!("v{len}{}", mangle_llvm_type(elem)?)),
        Type::Ptr { .. } => Some("p0".into()),
        _ => bitint_generic_parts(ty).map(|(_, bits, _, _)| format!("i{bits}")),
    }
}

fn mangled_link_name(intrinsic_name: &str, ret_type: &Option<Type>, param_types: &[Type]) -> String {
    let llvm_name = format!("llvm.{intrinsic_name}");
    let Some(signature) = find_intrinsic_signature(&llvm_name) else {
        return llvm_name;
    };
    let Some(positions) = signature.overloaded_positions else {
        return llvm_name;
    };
    if positions.is_empty() {
        return llvm_name;
    }
    let mangled: Option<Vec<String>> = positions
        .iter()
        .map(|&pos| {
            let ty = if pos == 0 {
                ret_type.as_ref()?
            } else {
                param_types.get(pos as usize - 1)?
            };
            mangle_llvm_type(ty)
        })
        .collect();
    match mangled {
        Some(suffixes) => format!("{llvm_name}.{}", suffixes.join(".")),
        None => llvm_name,
    }
}
