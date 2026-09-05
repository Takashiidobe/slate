use super::memory::{bitint_vector_lane_bits, pack_bitint_vector_expr, packed_mask_int_type};
use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn lower_clear_cache(&mut self, op: &inst::ClearCache) {
        self.lower_call_llvm_intrinsic(&inst::CallLlvmIntrinsic {
            result: None,
            result_ty: None,
            intrinsic_name: "clear_cache".into(),
            arg_ops: vec![op.begin.clone(), op.end.clone()],
            loc: op.loc.clone(),
        });
    }

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
        self.lower_extended_asm(op, &operands);
    }

    fn lower_extended_asm(&mut self, op: &inst::Asm, input_operands: &[&str]) {
        macro_rules! unsupported {
            ($($arg:tt)*) => {{
                self.parent.ctx.diagnostics.error(format!($($arg)*));
                return;
            }};
        }
        let Ok(template) = String::from_utf8(decode_cir_string(&op.asm_string)) else {
            unsupported!("lower: inline assembly template is not valid UTF-8");
        };
        let label_count = asm_template_label_count(&template);
        let asm_goto = if label_count == 0 {
            None
        } else {
            let Some(asm_goto) = self.asm_gotos.pop_front() else {
                unsupported!("lower: asm goto labels are missing from the Clang AST");
            };
            Some(asm_goto)
        };
        if asm_goto
            .as_ref()
            .is_some_and(|asm_goto| asm_goto.labels.len() != label_count)
        {
            unsupported!("lower: asm goto label count differs between CIR and the Clang AST");
        }
        let constraints = op
            .constraints
            .split(',')
            .map(str::trim)
            .take_while(|constraint| !constraint.starts_with("~{"))
            .collect::<Vec<_>>();
        let Some(output_count) = constraints.len().checked_sub(input_operands.len()) else {
            unsupported!(
                "lower: inline asm has more operands than constraints in `{}`",
                op.constraints
            );
        };
        let result_count = usize::from(op.res.is_some());
        if output_count != result_count && !(result_count == 1 && output_count > 1) {
            unsupported!(
                "lower: inline asm output count {output_count} does not match result count {result_count} in `{}`",
                op.constraints
            );
        }
        let flag_outputs: Vec<Option<&str>> = constraints[..output_count]
            .iter()
            .map(|constraint| parse_x86_flag_output_constraint(constraint))
            .collect();
        let Some(output_specs): Option<Vec<(AsmRegConstraint, bool)>> = constraints[..output_count]
            .iter()
            .zip(&flag_outputs)
            .map(|(constraint, flag)| {
                flag.map(|_| (AsmRegConstraint::Generic, false))
                    .or_else(|| parse_output_reg_constraint(constraint))
            })
            .collect()
        else {
            unsupported!(
                "lower: unsupported inline asm output constraint in `{}`",
                op.constraints
            );
        };
        let mut tied_outputs = vec![None; output_count];
        let mut input_specs: Vec<Option<AsmRegConstraint>> =
            Vec::with_capacity(input_operands.len());
        for (operand_index, constraint) in constraints[output_count..].iter().enumerate() {
            if let Ok(output_index) = constraint.parse::<usize>() {
                if output_index >= output_count || tied_outputs[output_index].is_some() {
                    unsupported!(
                        "lower: inline asm tied constraint `{constraint}` is out of range or duplicated in `{}`",
                        op.constraints
                    );
                }
                tied_outputs[output_index] = Some(operand_index);
                input_specs.push(None);
            } else if *constraint == "i" {
                input_specs.push(None);
            } else if let Some(spec) = parse_input_reg_constraint(constraint) {
                input_specs.push(Some(spec));
            } else {
                unsupported!("lower: unsupported inline asm input constraint `{constraint}`");
            }
        }
        let result_types: Vec<CirType> = if output_count == 0 {
            Vec::new()
        } else {
            let Some(result_types) =
                asm_output_types(op.res_ty.as_ref(), &self.parent.aliases, output_count)
            else {
                unsupported!("lower: could not determine inline asm output types");
            };
            result_types.into_iter().cloned().collect()
        };
        let Some(operand_types): Option<Vec<_>> = input_operands
            .iter()
            .map(|operand| self.value_type(operand).cloned())
            .collect()
        else {
            unsupported!("lower: could not determine inline asm input operand types");
        };
        if constraints[output_count..]
            .iter()
            .zip(input_operands)
            .any(|(constraint, operand)| {
                *constraint == "i" && self.known_arith_value(operand).is_none()
            })
        {
            unsupported!("lower: inline asm immediate operand is not a known constant");
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
        let dialect = cir_asm_dialect(op.asm_flavor);
        let (template, dialect) = normalize_asm_dialect_wrapper(template, dialect);
        let Some(mut template) = translate_asm_template(
            &template,
            &slot_to_rust,
            &template_constraints,
            &template_types,
            dialect,
        ) else {
            unsupported!(
                "lower: could not translate inline asm template `{}`",
                op.constraints
            );
        };
        for (output_index, condition) in flag_outputs.iter().enumerate() {
            let Some(condition) = condition else {
                continue;
            };
            let Some(suffix) = x86_flag_output_suffix(
                slot_to_rust[output_index],
                condition,
                &template_types[output_index],
                dialect,
            ) else {
                unsupported!(
                    "lower: unsupported inline asm flag-output constraint `{}`",
                    constraints[output_index]
                );
            };
            template.push_str("\n\t");
            template.push_str(&suffix);
        }
        let mut operands = Vec::new();
        let mut output_exprs = Vec::new();
        let mut output_names = Vec::new();
        struct EbxFixup {
            ebx_literal: String,
            scratch_literal: String,
            read: bool,
            write: bool,
        }
        let mut ebx_fixups: Vec<EbxFixup> = Vec::new();
        let mut used_reg_letters: BTreeSet<char> = output_specs
            .iter()
            .filter_map(|(spec, _)| reg_constraint_letter(spec))
            .chain(
                input_specs
                    .iter()
                    .flatten()
                    .filter_map(reg_constraint_letter),
            )
            .collect();
        if asm_goto.is_some()
            && output_count > 0
            && (result_count != output_count
                || op
                    .res
                    .as_ref()
                    .is_some_and(|result| !self.asm_output_places.contains_key(result)))
        {
            unsupported!("lower: asm goto output does not have a direct CIR destination");
        }
        for (output_index, (spec, early_clobber)) in output_specs.into_iter().enumerate() {
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
            let Some(mut reg) = asm_reg_for_constraint(spec, &template_types[output_index]) else {
                unsupported!(
                    "lower: unsupported inline asm output register in `{}`",
                    op.constraints
                );
            };
            if let AsmReg::Explicit(name) = &reg
                && is_ebx_family_reg(name)
            {
                let Some(scratch_letter) = pick_ebx_scratch_letter(&used_reg_letters) else {
                    unsupported!(
                        "lower: inline asm needs a spare register to save/restore ebx around `{}`",
                        op.constraints
                    );
                };
                used_reg_letters.insert(scratch_letter);
                let bits = asm_operand_bits(&template_types[output_index]);
                let Some(scratch_literal) = x86_fixed_register_name(scratch_letter, bits) else {
                    unsupported!(
                        "lower: unsupported inline asm output register in `{}`",
                        op.constraints
                    );
                };
                ebx_fixups.push(EbxFixup {
                    ebx_literal: name.clone(),
                    scratch_literal: scratch_literal.into(),
                    read: tied_outputs[output_index].is_some(),
                    write: true,
                });
                reg = AsmReg::Explicit(scratch_literal.into());
            }
            let late = !early_clobber;
            if let Some(operand_index) = tied_outputs[output_index] {
                operands.push(AsmOperand::InOut {
                    reg,
                    late,
                    input: self.operand_expr(input_operands[operand_index]),
                    output,
                });
            } else {
                operands.push(AsmOperand::Out {
                    reg,
                    late,
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
                let spec = input_specs[operand_index].clone().unwrap();
                let Some(mut reg) =
                    asm_reg_for_constraint(spec, &template_types[output_count + operand_index])
                else {
                    unsupported!(
                        "lower: unsupported inline asm input register in `{}`",
                        op.constraints
                    );
                };
                if let AsmReg::Explicit(name) = &reg
                    && is_ebx_family_reg(name)
                {
                    let Some(scratch_letter) = pick_ebx_scratch_letter(&used_reg_letters) else {
                        unsupported!(
                            "lower: inline asm needs a spare register to save/restore ebx around `{}`",
                            op.constraints
                        );
                    };
                    used_reg_letters.insert(scratch_letter);
                    let bits = asm_operand_bits(&template_types[output_count + operand_index]);
                    let Some(scratch_literal) = x86_fixed_register_name(scratch_letter, bits)
                    else {
                        unsupported!(
                            "lower: unsupported inline asm input register in `{}`",
                            op.constraints
                        );
                    };
                    ebx_fixups.push(EbxFixup {
                        ebx_literal: name.clone(),
                        scratch_literal: scratch_literal.into(),
                        read: true,
                        write: false,
                    });
                    reg = AsmReg::Explicit(scratch_literal.into());
                }
                AsmOperand::In {
                    reg,
                    value: self.operand_expr(input_operands[operand_index]),
                }
            };
            operands.push(value);
        }
        if let Some(asm_goto) = asm_goto {
            let Some(dispatch) = self.dispatch.as_ref() else {
                unsupported!("lower: asm goto requires dispatch control flow");
            };
            for label in asm_goto.labels {
                let states = dispatch.label_states.get(&label.name);
                let state = states.and_then(|states| {
                    if let Some(target) = label.target.as_ref() {
                        states
                            .iter()
                            .find(|(point, _)| point.as_ref() == Some(target))
                            .map(|(_, state)| *state)
                    } else {
                        (states.len() == 1).then_some(states[0].1)
                    }
                });
                let Some(state) = state else {
                    self.parent.ctx.diagnostics.error(format!(
                        "lower: asm goto target `{}` is missing or ambiguous in CIR",
                        label.name
                    ));
                    return;
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
        let template = if ebx_fixups.is_empty() {
            template
        } else {
            let att = !matches!(dialect, Some(AsmDialect::Intel));
            let reg_op = |name: &str| {
                if att {
                    format!("%{name}")
                } else {
                    name.to_string()
                }
            };
            let mov = |dst: &str, src: &str| {
                if att {
                    format!("mov {}, {}", reg_op(src), reg_op(dst))
                } else {
                    format!("mov {}, {}", reg_op(dst), reg_op(src))
                }
            };
            let mut prefix = format!("push {}\n\t", reg_op("rbx"));
            for fixup in ebx_fixups.iter().filter(|fixup| fixup.read) {
                prefix.push_str(&mov(&fixup.ebx_literal, &fixup.scratch_literal));
                prefix.push_str("\n\t");
            }
            let mut suffix = String::new();
            for fixup in ebx_fixups.iter().filter(|fixup| fixup.write) {
                suffix.push_str("\n\t");
                suffix.push_str(&mov(&fixup.scratch_literal, &fixup.ebx_literal));
            }
            suffix.push_str(&format!("\n\tpop {}", reg_op("rbx")));
            format!("{prefix}{template}{suffix}")
        };
        self.push_stmt(Self::unsafe_stmt(Stmt::InlineAsm(InlineAsm {
            template,
            dialect,
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
    }

    pub(super) fn lower_eh_setjmp(&mut self, op: &inst::EhSetjmp) {
        self.parent
            .synthetic_externs
            .entry("setjmp".into())
            .or_insert_with(builtin_setjmp_extern_decl);
        let env = self.operand_expr(&op.env);
        let call = Self::unsafe_expr(Expr::Call {
            func: Box::new(Expr::Var("setjmp".into())),
            args: vec![env],
            binding: CallBinding::Generated,
        });
        self.materialize_expr(&op.res, call, Some(&op.res_ty));
    }

    pub(super) fn lower_eh_longjmp(&mut self, op: &inst::EhLongjmp) {
        self.parent
            .synthetic_externs
            .entry("longjmp".into())
            .or_insert_with(builtin_longjmp_extern_decl);
        let env = self.operand_expr(&op.env);
        let call = Self::unsafe_expr(Expr::Call {
            func: Box::new(Expr::Var("longjmp".into())),
            args: vec![env, Expr::Value(RustValue::I64(1))],
            binding: CallBinding::Generated,
        });
        self.push_stmt(Stmt::Expr(call));
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
        let Some((_, len, _)) = op.result_ty.as_vector() else {
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
        let Some((_, len, _)) = op.result_ty.as_vector() else {
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

    pub(super) fn lower_vec_masked_load(&mut self, op: &inst::VecMaskedLoad) {
        let Some((elem_ty, len, _)) = op.result_ty.as_vector() else {
            self.lower_unsupported_value(&op.result, &op.result_ty, "cir.vec.masked_load");
            return;
        };
        let Some(mask_ty) = self.value_type(&op.mask).cloned() else {
            self.lower_unsupported_value(&op.result, &op.result_ty, "cir.vec.masked_load mask");
            return;
        };
        let Some((mask_elem_ty, _, _)) = mask_ty.as_vector() else {
            self.lower_unsupported_value(&op.result, &op.result_ty, "cir.vec.masked_load mask");
            return;
        };
        let mask_elem_rust_ty = self.parent.rust_type(mask_elem_ty);
        let elem_rust_ty = self.parent.rust_type(elem_ty);
        let addr = self.operand_expr(&op.addr);
        let mask = self.operand_expr(&op.mask);
        let pass_thru = self.operand_expr(&op.pass_thru);
        let elems = (0..len)
            .map(|i| {
                let mask_lane = vector_index_expr(mask.clone(), i);
                let Some((as_int, signed)) = bitint_to_int_expr(&mask_elem_rust_ty, mask_lane)
                else {
                    return Expr::Value(RustValue::Bool(false));
                };
                let zero = if signed {
                    RustValue::I128(0)
                } else {
                    RustValue::U128(0)
                };
                let active = Expr::Binary {
                    op: BinOp::Ne,
                    lhs: Box::new(as_int),
                    rhs: Box::new(Expr::Value(zero)),
                };
                let loaded = Self::unsafe_expr(Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::Cast {
                            expr: Box::new(addr.clone()),
                            ty: Type::Ptr {
                                mutable: false,
                                inner: Box::new(elem_rust_ty.clone()),
                            },
                        }),
                        method: "add".into(),
                        args: vec![Expr::Value(RustValue::Usize(i as usize))],
                    }),
                });
                Expr::If {
                    cond: Box::new(active),
                    then_expr: Box::new(loaded),
                    else_expr: Box::new(vector_index_expr(pass_thru.clone(), i)),
                }
            })
            .collect();
        self.materialize_expr(&op.result, Expr::ArrayLit(elems), Some(&op.result_ty));
    }

    pub(super) fn lower_vec_shuffle_dynamic(&mut self, op: &inst::VecShuffleDynamic) {
        self.lower_unsupported_value(&op.result, &op.result_ty, "cir.vec.shuffle.dynamic");
    }

    pub(super) fn lower_vec_create(&mut self, op: &inst::VecCreate) {
        let elems = op.elements.iter().map(|v| self.operand_expr(v)).collect();
        self.materialize_expr(&op.result, Expr::ArrayLit(elems), Some(&op.result_ty));
    }

    pub(super) fn lower_vec_splat(&mut self, op: &inst::VecSplat) {
        let Some((_, len, _)) = op.result_ty.as_vector() else {
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
        let Some((elem_ty, len, _)) = op.result_ty.as_vector() else {
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

    pub(super) fn lower_frame_address(&mut self, op: &inst::FrameAddress) {
        let probe = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: probe.clone(),
            mutable: true,
            ty: Some(Type::Prim(Prim::U8)),
            init: Some(Expr::Value(RustValue::TypedUInt(0, Prim::U8))),
        });
        let addr = Expr::Cast {
            expr: Box::new(Expr::AddrOf {
                mutable: true,
                expr: Box::new(Expr::Var(probe.into())),
            }),
            ty: self.parent.rust_type(&op.result_ty),
        };
        self.materialize_expr(&op.result, addr, Some(&op.result_ty));
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

        let raw_ret_type = op.result_ty.as_ref().map(|ty| self.parent.rust_type(ty));
        let stdarch_override =
            find_stdarch_override(&op.intrinsic_name, &raw_ret_type, &param_types);

        let mask_packed_param_types: Vec<Type> = match stdarch_override {
            Some(entry) => param_types
                .iter()
                .zip(entry.params)
                .map(|(ty, mined)| {
                    if let Type::Array { elem, len } = ty
                        && mined.starts_with('u')
                        && bitint_vector_lane_bits(elem) == Some(1)
                        && let Some(packed) = packed_mask_int_type(*len as u32)
                    {
                        return packed;
                    }
                    ty.clone()
                })
                .collect(),
            None => param_types.clone(),
        };
        let shim_param_types: Vec<Type> = mask_packed_param_types
            .iter()
            .map(|ty| simd_type(ty).unwrap_or_else(|| ty.clone()))
            .collect();
        let shim_ret_type = ret_type.as_ref().and_then(simd_type);
        let long_double_shim =
            long_double_intrinsic_shim(&op.intrinsic_name, &param_types, ret_type.as_ref());

        let shim_name = if let Some(shim) = long_double_shim {
            shim.to_string()
        } else {
            let sanitized = sanitize_ident(&op.intrinsic_name);
            let sig_key = format!("{sanitized}__{shim_ret_type:?}__{shim_param_types:?}");
            let sig_hash = {
                use std::hash::{Hash, Hasher};
                let mut hasher = std::collections::hash_map::DefaultHasher::new();
                sig_key.hash(&mut hasher);
                hasher.finish()
            };
            format!("__slate_intrinsic_{sanitized}_{sig_hash:x}")
        };

        let link_name = stdarch_override
            .map(|entry| entry.link_name.to_string())
            .unwrap_or_else(|| mangled_link_name(&op.intrinsic_name, &raw_ret_type, &param_types));

        if long_double_shim.is_none() {
            self.parent
                .llvm_intrinsic_shims
                .entry(shim_name.clone())
                .or_insert_with(|| ExternFnDecl {
                    attrs: vec![RustAttr::LinkName(link_name)],
                    identity: FunctionIdentity::Unknown,
                    name: shim_name.clone(),
                    declared_type: None,
                    trusted_headers: std::collections::BTreeSet::new(),
                    params: shim_param_types
                        .iter()
                        .enumerate()
                        .map(|(i, ty)| FnParam {
                            name: format!("_{i}"),
                            mutable: false,
                            ty: ty.clone(),
                        })
                        .collect(),
                    variadic: false,
                    ret: shim_ret_type.clone().or_else(|| ret_type.clone()),
                    safe: false,
                });
        }

        let args: Vec<Expr> = op
            .arg_ops
            .iter()
            .zip(param_types.iter())
            .zip(mask_packed_param_types.iter())
            .zip(shim_param_types.iter())
            .map(|(((id, orig_ty), packed_ty), shim_ty)| {
                let expr = self.operand_expr(id);
                match (orig_ty, packed_ty) {
                    (Type::Array { len, .. }, Type::Prim(prim)) if orig_ty != packed_ty => {
                        pack_bitint_vector_expr(expr, *len as usize, 1, *prim)
                    }
                    _ if matches!(shim_ty, Type::Generic { .. }) => simd_from_array_expr(expr),
                    _ => expr,
                }
            })
            .collect();
        let call_expr = Self::unsafe_expr(Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Var(shim_name.into())),
            args,
        });
        let call_expr = if shim_ret_type.is_some() {
            simd_to_array_expr(call_expr)
        } else {
            call_expr
        };
        match &op.result {
            Some(result) if ret_type.is_some() => {
                self.materialize_expr(result, call_expr, op.result_ty.as_ref())
            }
            _ => self.push_stmt(Self::unsafe_stmt(Stmt::Expr(call_expr))),
        }
    }
}

fn simd_element_prim(elem: &Type) -> Option<Prim> {
    match elem {
        Type::Prim(
            prim @ (Prim::I8
            | Prim::U8
            | Prim::I16
            | Prim::U16
            | Prim::I32
            | Prim::U32
            | Prim::I64
            | Prim::U64
            | Prim::F32
            | Prim::F64),
        ) => Some(*prim),
        _ => None,
    }
}

pub(super) fn extern_fn_decl_mentions_simd(decl: &ExternFnDecl) -> bool {
    decl.params
        .iter()
        .any(|param| type_mentions_simd(&param.ty))
        || decl.ret.as_ref().is_some_and(type_mentions_simd)
}

fn type_mentions_simd(ty: &Type) -> bool {
    matches!(ty, Type::Generic { name, .. } if name == "std::simd::Simd")
}

fn simd_type(ty: &Type) -> Option<Type> {
    let Type::Array { elem, len } = ty else {
        return None;
    };
    let prim = simd_element_prim(elem)?;
    Some(Type::Generic {
        name: "std::simd::Simd".into(),
        args: vec![Type::Prim(prim), Type::Custom(len.to_string())],
    })
}

fn simd_from_array_expr(expr: Expr) -> Expr {
    Expr::Call {
        binding: CallBinding::Generated,
        func: Box::new(Expr::Path(Path::new(
            ["std", "simd", "Simd", "from_array"].map(Ident::from),
        ))),
        args: vec![expr],
    }
}

fn simd_to_array_expr(expr: Expr) -> Expr {
    Expr::MethodCall {
        recv: Box::new(expr),
        method: "to_array".into(),
        args: vec![],
    }
}

fn long_double_intrinsic_shim(
    intrinsic_name: &str,
    param_types: &[Type],
    ret_type: Option<&Type>,
) -> Option<&'static str> {
    if !matches!(ret_type, Some(Type::LongDouble)) {
        return None;
    }
    let all_long_double = |count: usize| {
        param_types.len() == count && param_types.iter().all(|ty| matches!(ty, Type::LongDouble))
    };
    match intrinsic_name {
        "powi" if param_types == [Type::LongDouble, Type::Prim(Prim::I32)] => {
            Some("__slate_f80_powi")
        }
        "copysign" if all_long_double(2) => Some("__slate_f80_copysign"),
        "fmax" if all_long_double(2) => Some("__slate_f80_fmax"),
        "fmin" if all_long_double(2) => Some("__slate_f80_fmin"),
        "fma" if all_long_double(3) => Some("__slate_f80_fma"),
        "fmod" if all_long_double(2) => Some("__slate_f80_fmod"),
        "remainder" if all_long_double(2) => Some("__slate_f80_remainder"),
        "pow" if all_long_double(2) => Some("__slate_f80_pow"),
        "fdim" if all_long_double(2) => Some("__slate_f80_fdim"),
        "hypot" if all_long_double(2) => Some("__slate_f80_hypot"),
        "abs" | "fabs" if all_long_double(1) => Some("__slate_f80_abs"),
        "ceil" if all_long_double(1) => Some("__slate_f80_ceil"),
        "floor" if all_long_double(1) => Some("__slate_f80_floor"),
        "sqrt" if all_long_double(1) => Some("__slate_f80_sqrt"),
        "cbrt" if all_long_double(1) => Some("__slate_f80_cbrt"),
        "exp" if all_long_double(1) => Some("__slate_f80_exp"),
        "exp2" if all_long_double(1) => Some("__slate_f80_exp2"),
        "expm1" if all_long_double(1) => Some("__slate_f80_expm1"),
        "log" if all_long_double(1) => Some("__slate_f80_log"),
        "log2" if all_long_double(1) => Some("__slate_f80_log2"),
        "log10" if all_long_double(1) => Some("__slate_f80_log10"),
        "log1p" if all_long_double(1) => Some("__slate_f80_log1p"),
        "sin" if all_long_double(1) => Some("__slate_f80_sin"),
        "cos" if all_long_double(1) => Some("__slate_f80_cos"),
        "tan" if all_long_double(1) => Some("__slate_f80_tan"),
        "asin" if all_long_double(1) => Some("__slate_f80_asin"),
        "acos" if all_long_double(1) => Some("__slate_f80_acos"),
        "atan" if all_long_double(1) => Some("__slate_f80_atan"),
        "sinh" if all_long_double(1) => Some("__slate_f80_sinh"),
        "cosh" if all_long_double(1) => Some("__slate_f80_cosh"),
        "tanh" if all_long_double(1) => Some("__slate_f80_tanh"),
        "asinh" if all_long_double(1) => Some("__slate_f80_asinh"),
        "acosh" if all_long_double(1) => Some("__slate_f80_acosh"),
        "atanh" if all_long_double(1) => Some("__slate_f80_atanh"),
        "nearbyint" if all_long_double(1) => Some("__slate_f80_nearbyint"),
        "round" if all_long_double(1) => Some("__slate_f80_round"),
        "trunc" if all_long_double(1) => Some("__slate_f80_trunc"),
        "rint" if all_long_double(1) => Some("__slate_f80_rint"),
        _ => None,
    }
}

fn find_intrinsic_signature(
    llvm_name: &str,
) -> Option<&'static intrinsics_table::IntrinsicSignature> {
    [
        intrinsics_table::GENERAL_INTRINSICS,
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
        Type::LongDouble => Some("f80".into()),
        Type::Array { elem, len } => Some(format!("v{len}{}", mangle_llvm_type(elem)?)),
        Type::Ptr { .. } => Some("p0".into()),
        _ => bitint_generic_parts(ty).map(|(_, bits, _, _)| format!("i{bits}")),
    }
}

fn stdarch_shape(ty: &Type) -> Option<String> {
    match ty {
        Type::Prim(Prim::Bool) => Some("bool".into()),
        Type::Prim(Prim::I8) => Some("i8".into()),
        Type::Prim(Prim::U8) => Some("u8".into()),
        Type::Prim(Prim::I16) => Some("i16".into()),
        Type::Prim(Prim::U16) => Some("u16".into()),
        Type::Prim(Prim::I32) => Some("i32".into()),
        Type::Prim(Prim::U32) => Some("u32".into()),
        Type::Prim(Prim::I64) => Some("i64".into()),
        Type::Prim(Prim::U64) => Some("u64".into()),
        Type::Prim(Prim::F32) => Some("f32".into()),
        Type::Prim(Prim::F64) => Some("f64".into()),
        Type::Array { elem, len }
            if bitint_generic_parts(elem).is_some_and(|(_, bits, ..)| bits == "1") =>
        {
            matches!(len, 8 | 16 | 32 | 64).then(|| format!("u{len}"))
        }
        Type::Array { elem, len } => {
            let lane = match elem.as_ref() {
                Type::Prim(Prim::I8 | Prim::U8) => "i8",
                Type::Prim(Prim::I16 | Prim::U16) => "i16",
                Type::Prim(Prim::I32 | Prim::U32) => "i32",
                Type::Prim(Prim::I64 | Prim::U64) => "i64",
                Type::Prim(Prim::F32) => "f32",
                Type::Prim(Prim::F64) => "f64",
                _ => return None,
            };
            Some(format!("{lane}x{len}"))
        }
        _ => None,
    }
}

fn stdarch_param_matches(mined: &str, resolved: &Type) -> bool {
    let mined = mined.trim();
    if let Type::Ptr { .. } = resolved {
        return mined.starts_with("*mut") || mined.starts_with("*const");
    }
    stdarch_shape(resolved).is_some_and(|shape| shape == mined)
}

fn stdarch_ret_matches(mined: Option<&str>, resolved: &Option<Type>) -> bool {
    match (mined, resolved) {
        (None, None) => true,
        (Some(mined), Some(ty)) => stdarch_param_matches(mined, ty),
        _ => false,
    }
}

fn find_stdarch_override(
    intrinsic_name: &str,
    ret_type: &Option<Type>,
    param_types: &[Type],
) -> Option<&'static intrinsics_table::StdarchOverride> {
    let family_prefix = format!("llvm.{intrinsic_name}.");
    let exact_name = format!("llvm.{intrinsic_name}");
    let matches: Vec<&intrinsics_table::StdarchOverride> = intrinsics_table::X86_STDARCH_OVERRIDES
        .iter()
        .filter(|entry| {
            entry.link_name == exact_name || entry.link_name.starts_with(&family_prefix)
        })
        .filter(|entry| {
            entry.params.len() == param_types.len()
                && entry
                    .params
                    .iter()
                    .zip(param_types)
                    .all(|(mined, resolved)| stdarch_param_matches(mined, resolved))
                && stdarch_ret_matches(entry.ret, ret_type)
        })
        .collect();
    match matches.as_slice() {
        [only] => Some(only),
        _ => None,
    }
}

fn mangled_link_name(
    intrinsic_name: &str,
    ret_type: &Option<Type>,
    param_types: &[Type],
) -> String {
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
