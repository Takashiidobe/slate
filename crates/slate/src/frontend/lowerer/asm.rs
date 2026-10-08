use super::*;
use slate_parser::target_info::TargetFamily;

fn unsupported_asm(detail: impl Into<String>) -> Failure {
    Construct::Statement {
        kind: "Asm".into(),
        ir: format!("asm: {}", detail.into()),
    }
    .into()
}

fn asm_options(asm: &ir::InlineAsm) -> Result<Vec<rust::AsmOption>> {
    let options = asm
        .options
        .ok_or_else(|| unsupported_asm("naked assembly"))?;
    let mut lowered = Vec::new();
    if options.pure {
        lowered.push(rust::AsmOption::Pure);
    }
    match options.memory {
        ir::AsmMemory::None => lowered.push(rust::AsmOption::NoMem),
        ir::AsmMemory::ReadOnly => lowered.push(rust::AsmOption::ReadOnly),
        ir::AsmMemory::Any => {}
    }
    if options.nostack {
        lowered.push(rust::AsmOption::NoStack);
    }
    if options.preserves_flags {
        lowered.push(rust::AsmOption::PreservesFlags);
    }
    if options.may_unwind {
        lowered.push(rust::AsmOption::MayUnwind);
    }
    if asm.dialect == Some(ir::AsmDialect::Att) {
        lowered.push(rust::AsmOption::AttSyntax);
    }
    if !asm.has_sections() {
        lowered.push(rust::AsmOption::Raw);
    }
    Ok(lowered)
}

fn register_modifier(operand: &ir::AsmOperand, view: Option<ir::AsmRegisterView>) -> Result<char> {
    let width = match view {
        Some(ir::AsmRegisterView::HighByte) => {
            return if matches!(
                operand.selected,
                Some(ir::AsmOperandClass::Register(ir::AsmRegisterClass::RegAbcd))
            ) {
                Ok('h')
            } else {
                Err(unsupported_asm("high-byte view needs an abcd register"))
            };
        }
        Some(ir::AsmRegisterView::Bits(width)) => width,
        None => operand.width.ok_or(Invariant::AsmOperandWidth)?,
    };
    match width {
        8 => Ok('l'),
        16 => Ok('x'),
        32 => Ok('e'),
        64 => Ok('r'),
        _ => Err(unsupported_asm(format!("register view width {width}"))),
    }
}

fn asm_template(asm: &ir::InlineAsm) -> Result<String> {
    if !asm.has_sections() {
        return Ok(asm.template.clone());
    }
    let mut template = String::new();
    let mut used = vec![false; asm.operands.len()];
    for piece in &asm.pieces {
        match piece {
            ir::AsmPiece::Text(text) => {
                template.push_str(&text.replace('{', "{{").replace('}', "}}"));
            }
            ir::AsmPiece::Percent => template.push('%'),
            ir::AsmPiece::Operand {
                index,
                modifier,
                view,
            } => {
                let operand = asm
                    .operands
                    .get(*index)
                    .ok_or(Invariant::AsmOperandIndex(*index))?;
                used[*index] = true;
                match &operand.selected {
                    Some(ir::AsmOperandClass::Register(_)) => {
                        if modifier.is_some() && view.is_none() {
                            return Err(unsupported_asm(format!("operand modifier {modifier:?}")));
                        }
                        let modifier = register_modifier(operand, *view)?;
                        template.push_str(&format!("{{{index}:{modifier}}}"));
                    }
                    Some(ir::AsmOperandClass::Immediate) if modifier.is_none() => {
                        if asm.dialect == Some(ir::AsmDialect::Att) {
                            template.push('$');
                        }
                        template.push_str(&format!("{{{index}}}"));
                    }
                    _ => {
                        return Err(unsupported_asm(format!(
                            "template operand {index}, modifier {modifier:?}: {:?}",
                            operand.selected
                        )));
                    }
                }
            }
            piece => return Err(unsupported_asm(format!("template piece {piece}"))),
        }
    }
    for (index, used) in used.into_iter().enumerate() {
        if !used {
            let operand = &asm.operands[index];
            let reference = match operand.selected {
                Some(ir::AsmOperandClass::Register(_)) => {
                    let modifier = register_modifier(operand, None)?;
                    format!("{{{index}:{modifier}}}")
                }
                Some(ir::AsmOperandClass::Immediate) => format!("{{{index}}}"),
                _ => return Err(unsupported_asm(format!("unused operand {index}"))),
            };
            template.push_str(&format!("\n# {reference}"));
        }
    }
    Ok(template)
}

impl FunctionLowerer<'_, '_> {
    fn asm_fixed_place(&self, place: &ir::Place) -> bool {
        match place.kind {
            PlaceKind::Binding(id) => {
                self.register_locals.contains(&id) || self.tables.register_globals.contains_key(&id)
            }
            _ => false,
        }
    }

    fn asm_fixed_input(&self, value: &ir::Value) -> bool {
        match &value.node.value {
            ValueKind::Read { place, .. } => self.asm_fixed_place(place),
            ValueKind::Convert { operand, .. } | ValueKind::Copy { operand, .. } => {
                self.asm_fixed_input(operand)
            }
            _ => false,
        }
    }

    fn asm_scalar_type(&mut self, ty: &ir::Type) -> Result<rust::Type> {
        match self.tables.resolve_type(ty) {
            ir::Type::Numeric(ir::NumericType::Integer {
                width: 16 | 32 | 64,
                ..
            })
            | ir::Type::Pointer { .. } => self.lower_type(ty),
            _ => Err(unsupported_asm(format!(
                "operand type {ty} needs a register bridge"
            ))),
        }
    }

    fn asm_read(&mut self, statement: &Span<ir::Statement>, place: &ir::Place) -> Result<Expr> {
        self.lower_value(&ir::Value {
            ty: place.ty.clone(),
            node: statement.derive(ValueKind::Read {
                place: place.clone(),
                ordering: None,
            }),
        })
    }

    pub(super) fn lower_asm(
        &mut self,
        statement: &Span<ir::Statement>,
        asm: &ir::InlineAsm,
    ) -> Result<Stmt> {
        if self.tables.target.family != TargetFamily::X86_64 {
            return Err(unsupported_asm("target needs assembly lowering"));
        }
        if asm.goto || !asm.labels.is_empty() {
            return Err(unsupported_asm("asm goto"));
        }
        if !asm.operands.is_empty() && asm.alternative.is_none() {
            return Err(unsupported_asm(format!(
                "no usable constraint alternative: {:?}",
                asm.rejected
            )));
        }
        let options = asm_options(asm)?;
        let mut operands = Vec::new();
        let mut prefix = Vec::new();
        let mut writebacks = Vec::new();
        for (index, operand) in asm.operands.iter().enumerate() {
            let fixed = match &operand.kind {
                ir::AsmOperandKind::In(value) => self.asm_fixed_input(value),
                ir::AsmOperandKind::Out { place, .. } => self.asm_fixed_place(place),
                ir::AsmOperandKind::InOut { place, input, .. } => {
                    self.asm_fixed_place(place)
                        || input
                            .as_ref()
                            .is_some_and(|input| self.asm_fixed_input(&input.value))
                }
                _ => false,
            };
            if fixed {
                return Err(unsupported_asm(
                    "register variable needs explicit register lowering",
                ));
            }
            let reg = match &operand.selected {
                Some(ir::AsmOperandClass::Immediate) => {
                    let ir::AsmOperandKind::In(value) = &operand.kind else {
                        return Err(unsupported_asm(format!("constant output {index}")));
                    };
                    if !matches!(
                        value.node.value,
                        ValueKind::Constant(Number::Integer(_) | Number::SignedInteger(_))
                    ) {
                        return Err(unsupported_asm(format!(
                            "non-integer constant operand {index}"
                        )));
                    }
                    operands.push(rust::AsmOperand::Const(self.lower_value(value)?));
                    continue;
                }
                Some(ir::AsmOperandClass::Register(
                    class @ (ir::AsmRegisterClass::Reg | ir::AsmRegisterClass::RegAbcd),
                )) => rust::AsmReg::Class(class.as_str().into()),
                selected => return Err(unsupported_asm(format!("operand {index}: {selected:?}"))),
            };
            let lowered = match &operand.kind {
                ir::AsmOperandKind::In(value) => {
                    self.asm_scalar_type(&value.ty)?;
                    rust::AsmOperand::In {
                        reg,
                        value: self.lower_value(value)?,
                    }
                }
                ir::AsmOperandKind::Out { place, .. } | ir::AsmOperandKind::InOut { place, .. } => {
                    let ty = self.asm_scalar_type(&place.ty)?;
                    let temp = self.next_temp();
                    prefix.push(Stmt::Let {
                        name: temp.clone(),
                        mutable: false,
                        ty: Some(ty.clone()),
                        init: None,
                    });
                    let output = Expr::Var(temp.as_str().into());
                    writebacks.push(self.lower_assignment(place, output.clone())?);
                    let late = matches!(
                        operand.direction(),
                        ir::AsmDirection::LateOut | ir::AsmDirection::InLateOut
                    );
                    match &operand.kind {
                        ir::AsmOperandKind::InOut { input, .. } => {
                            let input = match input {
                                Some(input) => {
                                    if self.asm_scalar_type(&input.value.ty)? != ty {
                                        return Err(unsupported_asm(
                                            "tied input needs a type bridge",
                                        ));
                                    }
                                    self.lower_value(&input.value)?
                                }
                                None => self.asm_read(statement, place)?,
                            };
                            rust::AsmOperand::InOut {
                                reg,
                                late,
                                input,
                                output,
                            }
                        }
                        _ => rust::AsmOperand::Out {
                            reg,
                            late,
                            value: output,
                        },
                    }
                }
                _ => return Err(unsupported_asm(format!("operand kind at {index}"))),
            };
            operands.push(lowered);
        }
        let mut clobbered = BTreeSet::new();
        for clobber in &asm.clobbers {
            if let ir::AsmClobber::Register(register) = clobber {
                let canonical = register.canonical.ok_or_else(|| {
                    unsupported_asm(format!("unknown clobber {}", register.spelling))
                })?;
                if matches!(canonical, "bx" | "bp" | "sp") {
                    return Err(unsupported_asm(format!(
                        "reserved register clobber {canonical}"
                    )));
                }
                if !clobbered.insert(canonical) {
                    continue;
                }
                operands.push(rust::AsmOperand::Out {
                    reg: rust::AsmReg::Explicit(canonical.into()),
                    late: true,
                    value: Expr::Var("_".into()),
                });
            }
        }
        let template = asm_template(asm)?;
        self.dependencies.asm_unwind |= options.contains(&rust::AsmOption::MayUnwind);
        prefix.push(Stmt::InlineAsm(rust::InlineAsm {
            template,
            operands,
            options,
        }));
        prefix.extend(writebacks);
        Ok(Stmt::Unsafe {
            body: rust::Block {
                stmts: prefix,
                tail: None,
            },
        })
    }
}
