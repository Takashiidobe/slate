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

#[derive(Clone, Copy)]
enum Slot {
    Positional(usize),
    Register(&'static str),
    PairLow(&'static str),
}

fn gpr_name(canonical: &str, view: Option<ir::AsmRegisterView>, width: u64) -> Option<String> {
    let bits = match view {
        Some(ir::AsmRegisterView::HighByte) => {
            return matches!(canonical, "ax" | "bx" | "cx" | "dx")
                .then(|| format!("{}h", &canonical[..1]));
        }
        Some(ir::AsmRegisterView::Bits(bits)) => bits,
        None => width,
    };
    if let Some(number) = canonical.strip_prefix('r') {
        return match bits {
            8 => Some(format!("r{number}b")),
            16 => Some(format!("r{number}w")),
            32 => Some(format!("r{number}d")),
            64 => Some(canonical.to_string()),
            _ => None,
        };
    }
    let abcd = matches!(canonical, "ax" | "bx" | "cx" | "dx");
    match bits {
        8 if abcd => Some(format!("{}l", &canonical[..1])),
        8 => Some(format!("{canonical}l")),
        16 => Some(canonical.to_string()),
        32 => Some(format!("e{canonical}")),
        64 => Some(format!("r{canonical}")),
        _ => None,
    }
}

fn explicit_reference(
    asm: &ir::InlineAsm,
    width: u64,
    canonical: &str,
    modifier: Option<char>,
    view: Option<ir::AsmRegisterView>,
) -> Result<String> {
    if modifier.is_some() && view.is_none() {
        return Err(unsupported_asm(format!(
            "explicit register modifier {modifier:?}"
        )));
    }
    let name = gpr_name(canonical, view, width)
        .ok_or_else(|| unsupported_asm(format!("register {canonical} at width {width}")))?;
    Ok(match asm.dialect {
        Some(ir::AsmDialect::Att) => format!("%{name}"),
        _ => name,
    })
}

fn asm_template(asm: &ir::InlineAsm, slots: &[Slot]) -> Result<String> {
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
                let slot = match slots[*index] {
                    Slot::Positional(slot) => slot,
                    Slot::Register(canonical) => {
                        let width = operand.width.ok_or(Invariant::AsmOperandWidth)?;
                        template.push_str(&explicit_reference(
                            asm, width, canonical, *modifier, *view,
                        )?);
                        continue;
                    }
                    Slot::PairLow(canonical) => {
                        template
                            .push_str(&explicit_reference(asm, 64, canonical, *modifier, *view)?);
                        continue;
                    }
                };
                match &operand.selected {
                    Some(ir::AsmOperandClass::Register(_)) if *modifier == Some('a') => {
                        used[*index] = true;
                        template.push_str(&memory_reference(asm, operand, slot, None)?);
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
                        template.push_str(&memory_reference(asm, operand, slot, displacement)?);
                    }
                    Some(ir::AsmOperandClass::Register(_)) => {
                        if modifier.is_some() && view.is_none() {
                            return Err(unsupported_asm(format!("operand modifier {modifier:?}")));
                        }
                        used[*index] = true;
                        let modifier = register_modifier(operand, *view)?;
                        template.push_str(&format!("{{{slot}:{modifier}}}"));
                    }
                    Some(ir::AsmOperandClass::Immediate) => match modifier {
                        None | Some('c' | 'P' | 'p') => {
                            used[*index] = true;
                            if modifier.is_none() && asm.dialect == Some(ir::AsmDialect::Att) {
                                template.push('$');
                            }
                            template.push_str(&format!("{{{slot}}}"));
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
                        template.push_str(&format!("{{{slot}}}"));
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
        if let (false, Slot::Positional(slot)) = (used, slots[index]) {
            let operand = &asm.operands[index];
            let reference = match operand.selected {
                Some(ir::AsmOperandClass::Register(_)) => {
                    let modifier = register_modifier(operand, None)?;
                    format!("{{{slot}:{modifier}}}")
                }
                Some(
                    ir::AsmOperandClass::Immediate
                    | ir::AsmOperandClass::Symbol
                    | ir::AsmOperandClass::Memory,
                ) => {
                    format!("{{{slot}}}")
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

    fn asm_fixed_place(&self, place: &ir::Place, pinned: bool) -> bool {
        match place.kind {
            PlaceKind::Binding(id) => {
                (!pinned && self.register_locals.contains(&id))
                    || self.tables.register_globals.contains_key(&id)
            }
            _ => false,
        }
    }

    fn asm_fixed_input(&self, value: &ir::Value, pinned: bool) -> bool {
        match &value.node.value {
            ValueKind::Read { place, .. } => self.asm_fixed_place(place, pinned),
            ValueKind::Convert { operand, .. } | ValueKind::Copy { operand, .. } => {
                self.asm_fixed_input(operand, pinned)
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

    fn asm_wide_integer(&mut self, ty: &ir::Type) -> Result<rust::Type> {
        let lowered = self.lower_type(ty)?;
        if !matches!(lowered, rust::Type::Prim(Prim::I128 | Prim::U128)) {
            return Err(unsupported_asm(format!("register pair operand type {ty}")));
        }
        Ok(lowered)
    }

    fn asm_pair(
        &mut self,
        statement: &Span<ir::Statement>,
        operand: &ir::AsmOperand,
        [low, high]: [&'static str; 2],
        prefix: &mut Vec<Stmt>,
        writebacks: &mut Vec<Stmt>,
    ) -> Result<Vec<rust::AsmOperand>> {
        let wide = rust::Type::Prim(Prim::U128);
        let half = rust::Type::Prim(Prim::U64);
        let cast = |expr: Expr, ty: &rust::Type| Expr::Cast {
            expr: Box::new(expr),
            ty: ty.clone(),
        };
        let shift = |op, expr: Expr| Expr::Binary {
            op,
            lhs: Box::new(expr),
            rhs: Box::new(Expr::Value(64i64.into())),
        };
        let input = match &operand.kind {
            ir::AsmOperandKind::In(value)
            | ir::AsmOperandKind::InOut {
                input: Some(ir::AsmTiedInput { value, .. }),
                ..
            } => {
                self.asm_wide_integer(&value.ty)?;
                Some(self.lower_value(value)?)
            }
            ir::AsmOperandKind::InOut {
                place, input: None, ..
            } => {
                self.asm_wide_integer(&place.ty)?;
                Some(self.asm_read(statement, place)?)
            }
            ir::AsmOperandKind::Out { .. } => None,
            _ => return Err(unsupported_asm("register pair operand kind")),
        };
        let halves = match input {
            Some(input) => {
                let temp = self.next_temp();
                prefix.push(Stmt::Let {
                    name: temp.clone(),
                    mutable: false,
                    ty: Some(wide.clone()),
                    init: Some(cast(input, &wide)),
                });
                let whole = Expr::Var(temp.as_str().into());
                Some([
                    cast(whole.clone(), &half),
                    cast(shift(BinOp::Shr, whole), &half),
                ])
            }
            None => None,
        };
        let outputs = match &operand.kind {
            ir::AsmOperandKind::Out { place, .. } | ir::AsmOperandKind::InOut { place, .. } => {
                let target = self.asm_wide_integer(&place.ty)?;
                let [low_temp, high_temp] = [self.next_temp(), self.next_temp()];
                for temp in [&low_temp, &high_temp] {
                    prefix.push(Stmt::Let {
                        name: temp.clone(),
                        mutable: false,
                        ty: Some(half.clone()),
                        init: None,
                    });
                }
                let [low_value, high_value] =
                    [&low_temp, &high_temp].map(|temp| Expr::Var(temp.as_str().into()));
                let combined = Expr::Binary {
                    op: BinOp::BitOr,
                    lhs: Box::new(shift(BinOp::Shl, cast(high_value.clone(), &wide))),
                    rhs: Box::new(cast(low_value.clone(), &wide)),
                };
                writebacks.push(self.lower_assignment(place, cast(combined, &target))?);
                Some([low_value, high_value])
            }
            _ => None,
        };
        let late = matches!(
            operand.direction(),
            ir::AsmDirection::LateOut | ir::AsmDirection::InLateOut
        );
        let registers = [low, high].map(|name| rust::AsmReg::Explicit(name.into()));
        Ok(match (halves, outputs) {
            (Some(inputs), Some(outputs)) => registers
                .into_iter()
                .zip(inputs.into_iter().zip(outputs))
                .map(|(reg, (input, output))| rust::AsmOperand::InOut {
                    reg,
                    late,
                    input,
                    output,
                })
                .collect(),
            (Some(inputs), None) => registers
                .into_iter()
                .zip(inputs)
                .map(|(reg, value)| rust::AsmOperand::In { reg, value })
                .collect(),
            (None, Some(outputs)) => registers
                .into_iter()
                .zip(outputs)
                .map(|(reg, value)| rust::AsmOperand::Out { reg, late, value })
                .collect(),
            (None, None) => return Err(unsupported_asm("register pair operand kind")),
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
        let mut explicit = Vec::new();
        let mut slots = Vec::new();
        let mut saved_rbx = None;
        let mut taken = asm
            .operands
            .iter()
            .flat_map(|operand| match &operand.selected {
                Some(ir::AsmOperandClass::Explicit(register)) => vec![register.canonical],
                Some(ir::AsmOperandClass::Pair { low, high }) => {
                    vec![low.canonical, high.canonical]
                }
                _ => Vec::new(),
            })
            .flatten()
            .chain(asm.clobbers.iter().filter_map(|clobber| match clobber {
                ir::AsmClobber::Register(register) => register.canonical,
                _ => None,
            }))
            .collect::<BTreeSet<_>>();
        let mut prefix = Vec::new();
        let mut writebacks = Vec::new();
        for (index, operand) in asm.operands.iter().enumerate() {
            let explicit_class = matches!(operand.selected, Some(ir::AsmOperandClass::Explicit(_)));
            let fixed = match &operand.kind {
                ir::AsmOperandKind::In(value) => self.asm_fixed_input(value, explicit_class),
                ir::AsmOperandKind::Out { place, .. } => {
                    self.asm_fixed_place(place, explicit_class)
                }
                ir::AsmOperandKind::InOut { place, input, .. } => {
                    self.asm_fixed_place(place, explicit_class)
                        || input
                            .as_ref()
                            .is_some_and(|input| self.asm_fixed_input(&input.value, explicit_class))
                }
                _ => false,
            };
            let mut pinned = None;
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
                    slots.push(Slot::Positional(operands.len()));
                    operands.push(rust::AsmOperand::Const(self.lower_value(value)?));
                    continue;
                }
                Some(ir::AsmOperandClass::Symbol) => {
                    let ir::AsmOperandKind::Symbol(symbol) = &operand.kind else {
                        return Err(unsupported_asm(format!("non-symbol operand {index}")));
                    };
                    slots.push(Slot::Positional(operands.len()));
                    operands.push(rust::AsmOperand::Sym(self.asm_symbol(symbol)?));
                    continue;
                }
                Some(ir::AsmOperandClass::Pair { low, high }) => {
                    let (Some(low), Some(high)) = (low.canonical, high.canonical) else {
                        return Err(unsupported_asm("unnamed register pair"));
                    };
                    slots.push(Slot::PairLow(low));
                    explicit.extend(self.asm_pair(
                        statement,
                        operand,
                        [low, high],
                        &mut prefix,
                        &mut writebacks,
                    )?);
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
                    slots.push(Slot::Positional(operands.len()));
                    operands.push(rust::AsmOperand::In {
                        reg: rust::AsmReg::Class("reg".into()),
                        value: address,
                    });
                    continue;
                }
                Some(ir::AsmOperandClass::Register(
                    class @ (ir::AsmRegisterClass::Reg | ir::AsmRegisterClass::RegAbcd),
                )) => rust::AsmReg::Class(class.as_str().into()),
                Some(ir::AsmOperandClass::Register(ir::AsmRegisterClass::RegLegacy)) => {
                    let canonical = ["ax", "cx", "dx", "si", "di"]
                        .into_iter()
                        .find(|candidate| taken.insert(candidate))
                        .ok_or_else(|| unsupported_asm("no free legacy register"))?;
                    pinned = Some(canonical);
                    rust::AsmReg::Explicit(canonical.into())
                }
                Some(ir::AsmOperandClass::Explicit(register)) => {
                    let canonical = register
                        .canonical
                        .filter(|canonical| gpr_name(canonical, None, 64).is_some())
                        .ok_or_else(|| {
                            unsupported_asm(format!("explicit register {}", register.spelling))
                        })?;
                    match canonical {
                        "bp" | "sp" => {
                            return Err(unsupported_asm(format!(
                                "reserved register operand {canonical}"
                            )));
                        }
                        "bx" if saved_rbx.is_some() => {
                            return Err(unsupported_asm("second rbx operand"));
                        }
                        "bx" => {
                            saved_rbx = Some(operands.len());
                            rust::AsmReg::Class("reg".into())
                        }
                        _ => {
                            pinned = Some(canonical);
                            rust::AsmReg::Explicit(canonical.into())
                        }
                    }
                }
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
            match (pinned, lowered) {
                (None, lowered) if explicit_class && saved_rbx == Some(operands.len()) => {
                    slots.push(Slot::Register("bx"));
                    operands.push(match lowered {
                        rust::AsmOperand::In { reg, value } => rust::AsmOperand::InOut {
                            reg,
                            late: false,
                            input: value,
                            output: Expr::Var("_".into()),
                        },
                        rust::AsmOperand::Out { reg, value, .. } => rust::AsmOperand::Out {
                            reg,
                            late: false,
                            value,
                        },
                        rust::AsmOperand::InOut {
                            reg, input, output, ..
                        } => rust::AsmOperand::InOut {
                            reg,
                            late: false,
                            input,
                            output,
                        },
                        lowered => lowered,
                    });
                }
                (Some(canonical), lowered) => {
                    slots.push(Slot::Register(canonical));
                    explicit.push(lowered);
                }
                (_, lowered) => {
                    slots.push(Slot::Positional(operands.len()));
                    operands.push(lowered);
                }
            }
        }
        let mut saved_rbx = saved_rbx.map(|slot| (slot, true));
        let mut clobbered = BTreeSet::new();
        for clobber in &asm.clobbers {
            if let ir::AsmClobber::Register(register) = clobber {
                let canonical = register.canonical.ok_or_else(|| {
                    unsupported_asm(format!("unknown clobber {}", register.spelling))
                })?;
                if matches!(canonical, "bp" | "sp") {
                    return Err(unsupported_asm(format!(
                        "reserved register clobber {canonical}"
                    )));
                }
                if !clobbered.insert(canonical) {
                    continue;
                }
                if canonical == "bx" {
                    if saved_rbx.is_none() {
                        saved_rbx = Some((operands.len(), false));
                        operands.push(rust::AsmOperand::Out {
                            reg: rust::AsmReg::Class("reg".into()),
                            late: false,
                            value: Expr::Var("_".into()),
                        });
                    }
                    continue;
                }
                explicit.push(rust::AsmOperand::Out {
                    reg: rust::AsmReg::Explicit(canonical.into()),
                    late: true,
                    value: Expr::Var("_".into()),
                });
            }
        }
        let mut template = asm_template(asm, &slots)?;
        if let Some((slot, swap)) = saved_rbx {
            let (save, restore) = match (asm.dialect, swap) {
                (Some(ir::AsmDialect::Intel), false) => (
                    format!("mov {{{slot}:r}}, rbx"),
                    format!("mov rbx, {{{slot}:r}}"),
                ),
                (Some(ir::AsmDialect::Intel), _) => {
                    let swap = format!("xchg {{{slot}:r}}, rbx");
                    (swap.clone(), swap)
                }
                (_, false) => (
                    format!("mov %rbx, {{{slot}:r}}"),
                    format!("mov {{{slot}:r}}, %rbx"),
                ),
                _ => {
                    let swap = format!("xchg {{{slot}:r}}, %rbx");
                    (swap.clone(), swap)
                }
            };
            template = format!("{save}\n{template}\n{restore}");
        }
        operands.extend(explicit);
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
