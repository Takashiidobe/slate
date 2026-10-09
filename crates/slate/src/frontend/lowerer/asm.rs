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

fn memory_reference(
    asm: &ir::InlineAsm,
    operand: &ir::AsmOperand,
    index: usize,
    displacement: Option<u32>,
) -> Result<String> {
    let base = match operand.kind {
        ir::AsmOperandKind::In(_)
        | ir::AsmOperandKind::InPlace(_)
        | ir::AsmOperandKind::Out { .. }
        | ir::AsmOperandKind::InOut { .. } => format!("{{{index}:r}}"),
        _ => return Err(unsupported_asm("address of a non-value operand")),
    };
    Ok(match (asm.dialect, displacement) {
        (Some(ir::AsmDialect::Intel), None) => format!("[{base}]"),
        (Some(ir::AsmDialect::Intel), Some(offset)) => format!("[{base}+{offset}]"),
        (_, None) => format!("({base})"),
        (_, Some(offset)) => format!("{offset}({base})"),
    })
}

fn intel_size(operand: &ir::AsmOperand) -> Result<&'static str> {
    match operand.width.ok_or(Invariant::AsmOperandWidth)? {
        8 => Ok("byte"),
        16 => Ok("word"),
        32 => Ok("dword"),
        64 => Ok("qword"),
        80 => Ok("tbyte"),
        128 => Ok("xmmword"),
        256 => Ok("ymmword"),
        512 => Ok("zmmword"),
        width => Err(unsupported_asm(format!("memory operand width {width}"))),
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
                match &operand.selected {
                    Some(ir::AsmOperandClass::Register(_)) if *modifier == Some('a') => {
                        used[*index] = true;
                        template.push_str(&memory_reference(asm, operand, *index, None)?);
                    }
                    Some(ir::AsmOperandClass::Memory) => {
                        let displacement = match modifier {
                            None => None,
                            Some('H') => Some(8),
                            _ => {
                                return Err(unsupported_asm(format!(
                                    "memory modifier {modifier:?}"
                                )));
                            }
                        };
                        used[*index] = true;
                        if asm.dialect == Some(ir::AsmDialect::Intel) {
                            template.push_str(intel_size(operand)?);
                            template.push_str(" ptr ");
                        }
                        template.push_str(&memory_reference(asm, operand, *index, displacement)?);
                    }
                    Some(ir::AsmOperandClass::Register(_)) => {
                        if modifier.is_some() && view.is_none() {
                            return Err(unsupported_asm(format!("operand modifier {modifier:?}")));
                        }
                        used[*index] = true;
                        let modifier = register_modifier(operand, *view)?;
                        template.push_str(&format!("{{{index}:{modifier}}}"));
                    }
                    Some(ir::AsmOperandClass::Immediate) => match modifier {
                        None | Some('c' | 'P' | 'p') => {
                            used[*index] = true;
                            if modifier.is_none() && asm.dialect == Some(ir::AsmDialect::Att) {
                                template.push('$');
                            }
                            template.push_str(&format!("{{{index}}}"));
                        }
                        Some('n') => {
                            let ir::AsmOperandKind::In(value) = &operand.kind else {
                                return Err(unsupported_asm("negated non-value operand"));
                            };
                            let number = match &value.node.value {
                                ValueKind::Constant(Number::Integer(number)) => number.to_string(),
                                ValueKind::Constant(Number::SignedInteger(number)) => {
                                    number.to_string()
                                }
                                _ => return Err(unsupported_asm("negated non-integer constant")),
                            };
                            if let Some(positive) = number.strip_prefix('-') {
                                template.push_str(positive);
                            } else {
                                template.push('-');
                                template.push_str(&number);
                            }
                        }
                        _ => {
                            return Err(unsupported_asm(format!(
                                "immediate modifier {modifier:?}"
                            )));
                        }
                    },
                    Some(ir::AsmOperandClass::Symbol) => {
                        if !matches!(modifier, None | Some('c' | 'P' | 'p')) {
                            return Err(unsupported_asm(format!("symbol modifier {modifier:?}")));
                        }
                        let ir::AsmOperandKind::Symbol(symbol) = &operand.kind else {
                            return Err(unsupported_asm("non-symbol operand"));
                        };
                        used[*index] = true;
                        if modifier.is_none() && asm.dialect == Some(ir::AsmDialect::Att) {
                            template.push('$');
                        }
                        template.push_str(&format!("{{{index}}}"));
                        if symbol.offset > 0 {
                            template.push_str(&format!("+{}", symbol.offset));
                        } else if symbol.offset < 0 {
                            template.push_str(&symbol.offset.to_string());
                        }
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
                Some(
                    ir::AsmOperandClass::Immediate
                    | ir::AsmOperandClass::Symbol
                    | ir::AsmOperandClass::Memory,
                ) => {
                    format!("{{{index}}}")
                }
                _ => return Err(unsupported_asm(format!("unused operand {index}"))),
            };
            template.push_str(&format!("\n# {reference}"));
        }
    }
    Ok(template)
}

impl FunctionLowerer<'_, '_> {
    fn asm_symbol(&mut self, symbol: &ir::AsmSymbol) -> Result<Expr> {
        let id = symbol.binding;
        if let Some(function) = self.tables.names.get(&id) {
            if self.tables.intrinsics.contains_key(&id) || function.builtin.is_some() {
                return Err(unsupported_asm("builtin symbol operand"));
            }
            self.dependencies.taken_functions.insert(id);
            self.dependencies
                .address_taken
                .insert(function.rust.clone());
            return Ok(Expr::Var(function.rust.as_str().into()));
        }
        if let Some(bytes) = self.tables.strings.get(&id) {
            let name = format!("__slate_asm_string_{}", id.0);
            self.dependencies
                .asm_strings
                .entry(id.0)
                .or_insert_with(|| Item::Static {
                    comments: Vec::new(),
                    attrs: Vec::new(),
                    vis: rust::Visibility::Private,
                    mutable: false,
                    name: name.clone(),
                    ty: rust::Type::Array {
                        elem: Box::new(rust::Type::Prim(Prim::U8)),
                        len: bytes.len() as u64,
                    },
                    init: Expr::Unary {
                        op: rust::UnaryOp::Deref,
                        expr: Box::new(Expr::ByteStr(bytes.clone())),
                    },
                });
            return Ok(Expr::Var(name.as_str().into()));
        }
        if self.tables.statics.contains(&id) {
            return Ok(Expr::Var(
                binding_name(id, &self.tables.bindings).as_str().into(),
            ));
        }
        Err(unsupported_asm(format!("symbol binding {id:?}")))
    }

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

    fn asm_full_storage(&self, ty: &ir::Type) -> bool {
        match self.tables.resolve_type(ty) {
            ir::Type::Defined(id) => {
                let Some(ir::TypeDefinitionKind::Record {
                    kind,
                    fields: Some(fields),
                    layout: Some(layout),
                }) = self.tables.types.get(id).map(|definition| &definition.kind)
                else {
                    return false;
                };
                if fields
                    .iter()
                    .any(|field| field.bit_width.is_some() || !self.asm_full_storage(&field.ty))
                {
                    return false;
                }
                let sizes = fields
                    .iter()
                    .map(|field| self.tables.storage_of(&field.ty).map(|(size, _)| size));
                if matches!(kind, ir::RecordKind::Union) {
                    sizes.into_iter().all(|size| size == Some(layout.size))
                } else {
                    sizes
                        .collect::<Option<Vec<_>>>()
                        .is_some_and(|sizes| sizes.iter().sum::<u64>() == layout.size)
                }
            }
            ir::Type::Bool => false,
            ir::Type::Array { element, .. } => self.asm_full_storage(element),
            ir::Type::Numeric(_) | ir::Type::Pointer { .. } => true,
            _ => false,
        }
    }

    fn asm_bits_type(&self, ty: &ir::Type) -> Result<rust::Type> {
        if !matches!(self.tables.resolve_type(ty), ir::Type::Bool) && !self.asm_full_storage(ty) {
            return Err(unsupported_asm(
                "aggregate operand needs a field-wise bridge",
            ));
        }
        let size = self.tables.storage_of(ty).map(|(size, _)| size);
        let prim = match size {
            Some(1) => Prim::U8,
            Some(2) => Prim::U16,
            Some(4) => Prim::U32,
            Some(8) => Prim::U64,
            _ => {
                return Err(unsupported_asm(format!(
                    "operand type {ty} needs a register bridge"
                )));
            }
        };
        Ok(rust::Type::Prim(prim))
    }

    fn asm_scratch_type(&self, operand: &ir::AsmOperand) -> Result<rust::Type> {
        let (output, input) = match &operand.kind {
            ir::AsmOperandKind::In(value) => (&value.ty, None),
            ir::AsmOperandKind::Out { place, .. } => (&place.ty, None),
            ir::AsmOperandKind::InOut { place, input, .. } => {
                (&place.ty, input.as_ref().map(|input| &input.value.ty))
            }
            _ => return Err(unsupported_asm("non-value register operand")),
        };
        let width = |ty| -> Result<u32> {
            match self.asm_bits_type(ty)? {
                rust::Type::Prim(Prim::U8) => Ok(8),
                rust::Type::Prim(Prim::U16) => Ok(16),
                rust::Type::Prim(Prim::U32) => Ok(32),
                rust::Type::Prim(Prim::U64) => Ok(64),
                _ => unreachable!(),
            }
        };
        let bits = width(output)?.max(input.map(width).transpose()?.unwrap_or(0));
        Ok(rust::Type::Prim(match bits {
            8 => Prim::U32,
            16 => Prim::U16,
            32 => Prim::U32,
            _ => Prim::U64,
        }))
    }

    fn asm_encode(&mut self, value: Expr, ty: &ir::Type, scratch: &rust::Type) -> Result<Expr> {
        let bits = self.asm_bits_type(ty)?;
        let source = self.lower_type(ty)?;
        let value = if matches!(source, rust::Type::Prim(_) | rust::Type::Ptr { .. })
            && !matches!(
                source,
                rust::Type::Prim(Prim::F16 | Prim::F32 | Prim::F64 | Prim::F128)
            ) {
            Expr::Cast {
                expr: Box::new(value),
                ty: bits,
            }
        } else {
            Expr::Transmute {
                from: source,
                to: bits,
                expr: Box::new(value),
            }
        };
        Ok(Expr::Cast {
            expr: Box::new(value),
            ty: scratch.clone(),
        })
    }

    fn asm_decode(&mut self, value: Expr, ty: &ir::Type) -> Result<Expr> {
        let bits = self.asm_bits_type(ty)?;
        let target = self.lower_type(ty)?;
        let value = Expr::Cast {
            expr: Box::new(value),
            ty: bits,
        };
        Ok(if matches!(target, rust::Type::Prim(Prim::Bool)) {
            Expr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(value),
                rhs: Box::new(Expr::Value(0i64.into())),
            }
        } else {
            Expr::Transmute {
                from: self.asm_bits_type(ty)?,
                to: target,
                expr: Box::new(value),
            }
        })
    }

    fn asm_address(&mut self, place: &ir::Place, mutable: bool) -> Result<Expr> {
        if let Some(address) = self.vla_address(place)? {
            return Ok(address);
        }
        Ok(match &place.kind {
            PlaceKind::Deref(pointer) => self.lower_value(pointer)?,
            _ => Expr::AddrOf {
                mutable,
                expr: Box::new(self.lower_place(place)?),
            },
        })
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
                Some(ir::AsmOperandClass::Symbol) => {
                    let ir::AsmOperandKind::Symbol(symbol) = &operand.kind else {
                        return Err(unsupported_asm(format!("non-symbol operand {index}")));
                    };
                    operands.push(rust::AsmOperand::Sym(self.asm_symbol(symbol)?));
                    continue;
                }
                Some(ir::AsmOperandClass::Memory) => {
                    let address = match &operand.kind {
                        ir::AsmOperandKind::InPlace(place) => self.asm_address(place, false)?,
                        ir::AsmOperandKind::Out { place, .. }
                        | ir::AsmOperandKind::InOut {
                            place, input: None, ..
                        } => self.asm_address(place, true)?,
                        ir::AsmOperandKind::In(value) => {
                            let temp = self.next_temp();
                            prefix.push(Stmt::Let {
                                name: temp.clone(),
                                mutable: false,
                                ty: Some(self.lower_type(&value.ty)?),
                                init: Some(self.lower_value(value)?),
                            });
                            Expr::AddrOf {
                                mutable: false,
                                expr: Box::new(Expr::Var(temp.as_str().into())),
                            }
                        }
                        _ => {
                            return Err(unsupported_asm(format!("memory operand kind at {index}")));
                        }
                    };
                    operands.push(rust::AsmOperand::In {
                        reg: rust::AsmReg::Class("reg".into()),
                        value: address,
                    });
                    continue;
                }
                Some(ir::AsmOperandClass::Register(
                    class @ (ir::AsmRegisterClass::Reg | ir::AsmRegisterClass::RegAbcd),
                )) => rust::AsmReg::Class(class.as_str().into()),
                selected => return Err(unsupported_asm(format!("operand {index}: {selected:?}"))),
            };
            let lowered = match &operand.kind {
                ir::AsmOperandKind::In(value) => {
                    let scratch = self.asm_scratch_type(operand)?;
                    let input = self.lower_value(value)?;
                    rust::AsmOperand::In {
                        reg,
                        value: self.asm_encode(input, &value.ty, &scratch)?,
                    }
                }
                ir::AsmOperandKind::Out { place, .. } | ir::AsmOperandKind::InOut { place, .. } => {
                    let ty = self.asm_scratch_type(operand)?;
                    let temp = self.next_temp();
                    prefix.push(Stmt::Let {
                        name: temp.clone(),
                        mutable: false,
                        ty: Some(ty.clone()),
                        init: None,
                    });
                    let output = Expr::Var(temp.as_str().into());
                    let decoded = self.asm_decode(output.clone(), &place.ty)?;
                    writebacks.push(self.lower_assignment(place, decoded)?);
                    let late = matches!(
                        operand.direction(),
                        ir::AsmDirection::LateOut | ir::AsmDirection::InLateOut
                    );
                    match &operand.kind {
                        ir::AsmOperandKind::InOut { input, .. } => {
                            let input = match input {
                                Some(input) => {
                                    let value = self.lower_value(&input.value)?;
                                    self.asm_encode(value, &input.value.ty, &ty)?
                                }
                                None => {
                                    let value = self.asm_read(statement, place)?;
                                    self.asm_encode(value, &place.ty, &ty)?
                                }
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
