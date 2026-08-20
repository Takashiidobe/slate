use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn lower_asm(&mut self, op: &Op) {
        if op.results.is_empty()
            && op.operands.is_empty()
            && let Some(raw) = attr_str(op, "asm_string")
            && !asm_template_has_placeholders(raw)
        {
            let Ok(template) = String::from_utf8(decode_cir_string(raw)) else {
                self.parent
                    .ctx
                    .diagnostics
                    .error("lower: inline assembly template is not valid UTF-8");
                return;
            };
            self.push_stmt(Self::unsafe_stmt(Stmt::Expr(asm_macro_expr(
                template.replace("$$", "$"),
                cir_asm_dialect(op),
            ))));
            return;
        }
        if self.lower_extended_asm(op) {
            return;
        }
        let Some((result, _)) = op.results.first() else {
            return;
        };
        let expr = op
            .operands
            .first()
            .map(|operand| self.operand_expr(operand))
            .unwrap_or_else(|| {
                op_result_type(op)
                    .map(|ty| self.parent.rust_type(ty))
                    .map(|ty| self.parent.default_value_expr(&ty))
                    .unwrap_or(Expr::Value(RustValue::I64(0)))
            });
        self.materialize_expr(result, expr, op_result_type(op));
    }

    pub(super) fn lower_extended_asm(&mut self, op: &Op) -> bool {
        let Some(raw_template) = attr_str(op, "asm_string") else {
            return false;
        };
        let Ok(template) = String::from_utf8(decode_cir_string(raw_template)) else {
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
        let Some(raw_constraints) = attr_str(op, "constraints") else {
            return false;
        };
        let constraints = raw_constraints
            .split(',')
            .map(str::trim)
            .take_while(|constraint| !constraint.starts_with("~{"))
            .collect::<Vec<_>>();
        let Some(output_count) = constraints.len().checked_sub(op.operands.len()) else {
            return false;
        };
        if output_count != op.results.len() && !(op.results.len() == 1 && output_count > 1) {
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
            let Some(result_types) = asm_output_types(op, &self.parent.aliases, output_count)
            else {
                return false;
            };
            result_types.into_iter().cloned().collect()
        };
        let Some(operand_types) = self.operand_types(op) else {
            return false;
        };
        if operand_types.len() != op.operands.len() {
            return false;
        }
        if constraints[output_count..]
            .iter()
            .zip(&op.operands)
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
            && (op.results.len() != output_count
                || op
                    .results
                    .iter()
                    .any(|(result, _)| !self.asm_output_places.contains_key(result)))
        {
            self.parent
                .ctx
                .diagnostics
                .error("lower: asm goto output does not have a direct CIR destination");
            return true;
        }
        for (output_index, constraint) in constraints[..output_count].iter().enumerate() {
            let direct_output = op
                .results
                .get(output_index)
                .and_then(|(result, _)| self.asm_output_places.get(result))
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
                    input: self.operand_expr(&op.operands[operand_index]),
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
                let value = self.known_arith_value(&op.operands[operand_index]).unwrap();
                AsmOperand::Const(int_value_expr(value))
            } else {
                AsmOperand::In {
                    reg: AsmReg::Class("reg".into()),
                    value: self.operand_expr(&op.operands[operand_index]),
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
            dialect: cir_asm_dialect(op),
            operands,
            raw: false,
        })));
        if output_count == op.results.len() {
            for (((result, _), output), name) in
                op.results.iter().zip(output_exprs).zip(output_names)
            {
                self.values.insert(result.clone(), Val::Expr(output));
                if let Some(name) = name {
                    self.immutable_temps.insert(name);
                }
            }
        } else {
            self.asm_outputs
                .insert(op.results[0].0.clone(), output_exprs);
            self.immutable_temps
                .extend(output_names.into_iter().flatten());
        }
        true
    }

    pub(super) fn lower_eh_setjmp(&mut self, op: &TypedEhSetjmp) {
        self.materialize_expr(&op.res, Expr::Value(RustValue::I64(0)), Some(&op.res_ty));
    }

    pub(super) fn lower_unsupported_value(&mut self, op: &Op, note: &str) {
        let Some((result, _)) = op.results.first() else {
            self.push_stmt(Stmt::Expr(Expr::Macro {
                name: "panic".into(),
                args: vec![Expr::Str(format!("unsupported CIR op: {note}"))],
            }));
            return;
        };
        self.materialize_expr(
            result,
            Expr::Macro {
                name: "panic".into(),
                args: vec![Expr::Str(format!("unsupported CIR op: {note}"))],
            },
            op_result_type(op),
        );
    }

    pub(super) fn lower_unsupported_value_typed(
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

    pub(super) fn lower_llvm_intrinsic(&mut self, op: &Op) {
        let Some(name) = attr_str(op, "intrinsic_name") else {
            self.lower_unsupported_value(op, "cir.call_llvm_intrinsic");
            return;
        };
        if name == "debugtrap" {
            self.parent.uses_breakpoint.set(true);
            self.push_stmt(Stmt::Expr(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["core", "arch", "breakpoint"].map(Ident::from),
                ))),
                args: Vec::new(),
            }));
        } else if !self.lower_x86_intrinsic(op, name) {
            self.lower_unsupported_value(op, "cir.call_llvm_intrinsic");
        }
    }

    pub(super) fn lower_x86_intrinsic(&mut self, op: &Op, name: &str) -> bool {
        let function = match name {
            "x86.sse2.pause" => "_mm_pause",
            "x86.sse2.lfence" => "_mm_lfence",
            "x86.sse2.mfence" => "_mm_mfence",
            "x86.sse.sfence" => "_mm_sfence",
            "x86.rdtsc" => "_rdtsc",
            "x86.sse42.crc32.32.8" => "_mm_crc32_u8",
            "x86.sse42.crc32.32.16" => "_mm_crc32_u16",
            "x86.sse42.crc32.32.32" => "_mm_crc32_u32",
            "x86.sse42.crc32.64.64" => "_mm_crc32_u64",
            "x86.rdtscp" => return self.lower_x86_rdtscp(op),
            _ => return false,
        };
        let Some(call) = self.x86_intrinsic_op_call(function, op) else {
            return false;
        };
        let call = Self::unsafe_expr(call);
        if matches!(
            name,
            "x86.sse2.pause" | "x86.sse2.lfence" | "x86.sse2.mfence" | "x86.sse.sfence"
        ) {
            self.push_stmt(Stmt::Expr(call));
        } else if let Some((result, _)) = op.results.first() {
            self.materialize_expr(result, call, op_result_type(op));
        }
        true
    }

    pub(super) fn lower_x86_rdtscp(&mut self, op: &Op) -> bool {
        let Some((result, _)) = op.results.first() else {
            return false;
        };
        let Some(result_ty) = op_result_type(op) else {
            return false;
        };
        let Type::Custom(record_name) = self.parent.rust_type(result_ty) else {
            return false;
        };
        let Some(record) = self.parent.records.get(&record_name) else {
            return false;
        };
        let [counter_field, auxiliary_field] = record.fields.as_slice() else {
            return false;
        };
        let counter_field = sanitize_ident(&counter_field.name).into_string();
        let auxiliary_field = sanitize_ident(&auxiliary_field.name).into_string();
        let auxiliary = self.unique_local_name("__slate_rdtscp_aux".into());
        let counter = self.unique_local_name("__slate_rdtscp_counter".into());
        let Some(call) = self.x86_intrinsic_call(
            "__rdtscp",
            &[Expr::Ref {
                mutable: true,
                expr: Box::new(Expr::Var(auxiliary.clone().into())),
            }],
        ) else {
            return false;
        };
        let expr = Expr::Block(Box::new(crate::backend::rust_ast::Block {
            stmts: vec![
                IndentStmt {
                    depth: 1,
                    stmt: Stmt::Let {
                        name: auxiliary.clone(),
                        mutable: true,
                        ty: Some(Type::Prim(Prim::U32)),
                        init: Some(Expr::Value(RustValue::I64(0))),
                    },
                },
                IndentStmt {
                    depth: 1,
                    stmt: Stmt::Let {
                        name: counter.clone(),
                        mutable: false,
                        ty: Some(Type::Prim(Prim::U64)),
                        init: Some(Self::unsafe_expr(call)),
                    },
                },
            ],
            tail: Some(Box::new(Expr::StructLit {
                name: record_name,
                fields: vec![
                    (counter_field, Expr::Var(counter.into())),
                    (auxiliary_field, Expr::Var(auxiliary.into())),
                ],
            })),
        }));
        self.materialize_expr(result, expr, Some(result_ty));
        true
    }

    pub(super) fn x86_intrinsic_op_call(&self, function: &str, op: &Op) -> Option<Expr> {
        let args = op
            .operands
            .iter()
            .map(|operand| self.operand_expr(operand))
            .collect::<Vec<_>>();
        self.x86_intrinsic_call(function, &args)
    }

    pub(super) fn x86_intrinsic_call(&self, function: &str, args: &[Expr]) -> Option<Expr> {
        let arch = match self.parent.target_arch.as_deref()? {
            "x86" => "x86",
            "x86_64" => "x86_64",
            _ => return None,
        };
        Some(Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(
                ["core", "arch", arch, function].map(Ident::from),
            ))),
            args: args.to_vec(),
        })
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

    pub(super) fn lower_vec_extract(&mut self, op: &TypedVecExtract) {
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

    pub(super) fn lower_vec_insert(&mut self, op: &TypedVecInsert) {
        let Some((_, len)) = parse_cir_vector_type(&op.result_ty) else {
            self.lower_unsupported_value_typed(&op.result, &op.result_ty, "cir.vec.insert");
            return;
        };
        let Some(index) = self
            .const_int_values
            .get(&op.index)
            .and_then(|i| u64::try_from(*i).ok())
            .filter(|i| *i < len)
        else {
            self.lower_unsupported_value_typed(
                &op.result,
                &op.result_ty,
                "cir.vec.insert dynamic index",
            );
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

    pub(super) fn lower_vec_shuffle(&mut self, op: &TypedVecShuffle) {
        let Some((_, len)) = parse_cir_vector_type(&op.result_ty) else {
            self.lower_unsupported_value_typed(&op.result, &op.result_ty, "cir.vec.shuffle");
            return;
        };
        let indices = int_array_attr(&op.indices).unwrap_or_default();
        if indices.len() != len as usize {
            self.lower_unsupported_value_typed(
                &op.result,
                &op.result_ty,
                "cir.vec.shuffle indices",
            );
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

    pub(super) fn lower_vec_shuffle_dynamic(&mut self, op: &TypedVecShuffleDynamic) {
        self.lower_unsupported_value_typed(&op.result, &op.result_ty, "cir.vec.shuffle.dynamic");
    }

    pub(super) fn lower_vec_create(&mut self, op: &TypedVecCreate) {
        let elems = op.elements.iter().map(|v| self.operand_expr(v)).collect();
        self.materialize_expr(&op.result, Expr::ArrayLit(elems), Some(&op.result_ty));
    }

    pub(super) fn lower_vec_splat(&mut self, op: &TypedVecSplat) {
        let Some((_, len)) = parse_cir_vector_type(&op.result_ty) else {
            self.lower_unsupported_value_typed(&op.result, &op.result_ty, "cir.vec.splat");
            return;
        };
        let value = self.operand_expr(&op.value);
        self.materialize_expr(
            &op.result,
            Expr::ArrayLit((0..len).map(|_| value.clone()).collect()),
            Some(&op.result_ty),
        );
    }

    pub(super) fn lower_vec_cmp(&mut self, op: &TypedVecCmp) {
        let Some((elem_ty, len)) = parse_cir_vector_type(&op.result_ty) else {
            self.lower_unsupported_value_typed(&op.result, &op.result_ty, "cir.vec.cmp");
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
                self.lower_unsupported_value_typed(&op.result, &op.result_ty, "cir.vec.cmp kind");
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

    pub(super) fn lower_opaque_pointer_typed(
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

    pub(super) fn lower_is_constant(&mut self, op: &TypedIsConstant) {
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

    pub(super) fn lower_objsize(&mut self, op: &TypedObjsize) {
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
}
