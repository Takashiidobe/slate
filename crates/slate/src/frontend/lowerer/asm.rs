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
    Spill(SpillRegister),
}

fn spill_reference(asm: &ir::InlineAsm, register: SpillRegister) -> String {
    let name = match register {
        SpillRegister::Stack(0) => "st".to_string(),
        SpillRegister::Stack(position) => format!("st({position})"),
        SpillRegister::Mmx(number) => format!("mm{number}"),
    };
    match asm.dialect {
        Some(ir::AsmDialect::Intel) => name,
        _ => format!("%{name}"),
    }
}

#[derive(Clone, Copy)]
enum SpillRegister {
    Stack(usize),
    Mmx(usize),
}

struct Spill {
    register: SpillRegister,
    bytes: u64,
    slot: usize,
}

const MMX_REGISTERS: [&str; 8] = ["mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7"];
const STACK_REGISTERS: [&str; 8] = [
    "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)",
];

fn stack_positions(asm: &ir::InlineAsm) -> Vec<Option<usize>> {
    let explicit = |operand: &ir::AsmOperand| match &operand.selected {
        Some(ir::AsmOperandClass::Explicit(register)) => STACK_REGISTERS[..2]
            .iter()
            .position(|name| register.canonical == Some(*name)),
        _ => None,
    };
    let mut next = asm
        .operands
        .iter()
        .filter(|operand| {
            matches!(
                operand.kind,
                ir::AsmOperandKind::In(_) | ir::AsmOperandKind::InOut { .. }
            )
        })
        .filter_map(explicit)
        .max()
        .map_or(0, |position| position + 1);
    asm.operands
        .iter()
        .map(|operand| match &operand.selected {
            Some(ir::AsmOperandClass::Register(ir::AsmRegisterClass::X87Reg)) => {
                next += 1;
                Some(next - 1)
            }
            _ => explicit(operand),
        })
        .collect()
}

fn spill_instruction(asm: &ir::InlineAsm, spill: &Spill, load: bool) -> String {
    let intel = asm.dialect == Some(ir::AsmDialect::Intel);
    let memory = if intel {
        let size = match spill.bytes {
            4 => "dword",
            8 => "qword",
            _ => "tbyte",
        };
        format!("{size} ptr [{{{}:r}}]", spill.slot)
    } else {
        format!("({{{}:r}})", spill.slot)
    };
    match spill.register {
        SpillRegister::Stack(_) => {
            let base = if load { "fld" } else { "fstp" };
            let suffix = match (intel, spill.bytes) {
                (true, _) => "",
                (_, 4) => "s",
                (_, 8) => "l",
                _ => "t",
            };
            format!("{base}{suffix} {memory}")
        }
        SpillRegister::Mmx(number) => {
            let mnemonic = if spill.bytes == 4 { "movd" } else { "movq" };
            let register = spill_reference(asm, SpillRegister::Mmx(number));
            match (intel, load) {
                (true, true) | (false, false) => format!("{mnemonic} {register}, {memory}"),
                _ => format!("{mnemonic} {memory}, {register}"),
            }
        }
    }
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

fn asm_template(asm: &ir::InlineAsm, slots: &[Slot], labels: &[usize]) -> Result<String> {
    if !asm.has_sections() {
        return Ok(asm.template.clone());
    }
    let mut template = String::new();
    let mut used = vec![false; asm.operands.len()];
    let mut used_labels = vec![false; labels.len()];
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
                    Slot::Spill(register) => {
                        if modifier.is_some() || view.is_some() {
                            return Err(unsupported_asm(format!(
                                "spilled register modifier {modifier:?}"
                            )));
                        }
                        template.push_str(&spill_reference(asm, register));
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
            ir::AsmPiece::Label(index) => {
                let slot = labels
                    .get(*index)
                    .ok_or_else(|| unsupported_asm(format!("label operand {index}")))?;
                used_labels[*index] = true;
                template.push_str(&format!("{{{slot}}}"));
            }
            piece => return Err(unsupported_asm(format!("template piece {piece}"))),
        }
    }
    for (slot, _) in labels.iter().zip(used_labels).filter(|(_, used)| !used) {
        template.push_str(&format!("\n# {{{slot}}}"));
    }
    for (index, used) in used.into_iter().enumerate() {
        if let (false, Slot::Positional(slot)) = (used, slots[index]) {
            let operand = &asm.operands[index];
            if let Some(ir::AsmOperandClass::Flags(condition)) = operand.selected {
                template.push_str(&format!("\nset{condition} {{{slot}}}"));
                continue;
            }
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

    pub(super) fn lower_naked_body(&mut self, body: &[Span<ir::Statement>]) -> Result<Stmt> {
        if self.tables.target.family != TargetFamily::X86_64 {
            return Err(unsupported_asm("target needs assembly lowering"));
        }
        let mut templates = Vec::new();
        let mut operands = Vec::new();
        let mut dialects = Vec::new();
        for statement in body {
            let asm = match &statement.value {
                ir::Statement::Asm(asm) => asm,
                ir::Statement::Null | ir::Statement::Comment(_) => continue,
                _ => return Err(unsupported_asm("naked function with non-asm statements")),
            };
            if !asm.operands.is_empty() && asm.alternative.is_none() {
                return Err(unsupported_asm(format!(
                    "no usable constraint alternative: {:?}",
                    asm.rejected
                )));
            }
            let mut slots = Vec::new();
            for operand in &asm.operands {
                slots.push(Slot::Positional(operands.len()));
                operands.push(match (&operand.selected, &operand.kind) {
                    (Some(ir::AsmOperandClass::Immediate), ir::AsmOperandKind::In(value)) => {
                        rust::AsmOperand::Const(self.lower_value(value)?)
                    }
                    (Some(ir::AsmOperandClass::Symbol), ir::AsmOperandKind::Symbol(symbol)) => {
                        rust::AsmOperand::Sym(self.asm_symbol(symbol)?)
                    }
                    (selected, _) => {
                        return Err(unsupported_asm(format!("naked operand: {selected:?}")));
                    }
                });
            }
            templates.push(if asm.has_sections() {
                asm_template(asm, &slots, &[])?
            } else {
                asm.template.replace('{', "{{").replace('}', "}}")
            });
            if !dialects.contains(&asm.dialect) {
                dialects.push(asm.dialect);
            }
        }
        let options = match dialects.as_slice() {
            [] | [Some(ir::AsmDialect::Att)] => vec![rust::AsmOption::AttSyntax],
            [Some(ir::AsmDialect::Intel)] => Vec::new(),
            dialects => {
                return Err(unsupported_asm(format!("naked dialects {dialects:?}")));
            }
        };
        Ok(Stmt::InlineAsm(rust::InlineAsm {
            template: templates.join("\n"),
            operands,
            options,
            naked: true,
        }))
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

    fn asm_spill_bytes(&self, register: SpillRegister, ty: &ir::Type) -> Result<u64> {
        let bytes = match (register, self.tables.resolve_type(ty)) {
            (SpillRegister::Stack(_), ir::Type::Numeric(ir::NumericType::Float(float))) => {
                match float {
                    ir::FloatType::F32 => Some(4),
                    ir::FloatType::F64 => Some(8),
                    ir::FloatType::F80 => Some(16),
                    _ => None,
                }
            }
            (SpillRegister::Mmx(_), _) => self
                .tables
                .storage_of(ty)
                .map(|(size, _)| size)
                .filter(|size| matches!(size, 4 | 8)),
            _ => None,
        };
        bytes.ok_or_else(|| unsupported_asm(format!("spilled operand type {ty}")))
    }

    fn asm_spill(
        &mut self,
        statement: &Span<ir::Statement>,
        operand: &ir::AsmOperand,
        register: SpillRegister,
        operands: &mut Vec<rust::AsmOperand>,
        prefix: &mut Vec<Stmt>,
        writebacks: &mut Vec<Stmt>,
    ) -> Result<[Option<Spill>; 2]> {
        let input = match &operand.kind {
            ir::AsmOperandKind::In(value)
            | ir::AsmOperandKind::InOut {
                input: Some(ir::AsmTiedInput { value, .. }),
                ..
            } => Some((self.lower_value(value)?, &value.ty)),
            ir::AsmOperandKind::InOut {
                place, input: None, ..
            } => Some((self.asm_read(statement, place)?, &place.ty)),
            ir::AsmOperandKind::Out { .. } => None,
            _ => return Err(unsupported_asm("spilled operand kind")),
        };
        let output = match &operand.kind {
            ir::AsmOperandKind::Out { place, .. } | ir::AsmOperandKind::InOut { place, .. } => {
                Some(place)
            }
            _ => None,
        };
        let mut spill = |this: &mut Self, ty: &ir::Type, init: Expr, mutable: bool| {
            let bytes = this.asm_spill_bytes(register, ty)?;
            let temp = this.next_temp();
            prefix.push(Stmt::Let {
                name: temp.clone(),
                mutable,
                ty: Some(this.lower_type(ty)?),
                init: Some(init),
            });
            operands.push(rust::AsmOperand::In {
                reg: rust::AsmReg::Class("reg".into()),
                value: Expr::AddrOf {
                    mutable,
                    expr: Box::new(Expr::Var(temp.as_str().into())),
                },
            });
            Ok::<_, Failure>((
                Spill {
                    register,
                    bytes,
                    slot: operands.len() - 1,
                },
                temp,
            ))
        };
        let load = match input {
            Some((value, ty)) => Some(spill(self, ty, value, false)?.0),
            None => None,
        };
        let store = match output {
            Some(place) => {
                let (store, temp) = spill(self, &place.ty, zeroed(), true)?;
                writebacks.push(self.lower_assignment(place, Expr::Var(temp.as_str().into()))?);
                Some(store)
            }
            None => None,
        };
        Ok([load, store])
    }

    pub(super) fn lower_asm(
        &mut self,
        statement: &Span<ir::Statement>,
        asm: &ir::InlineAsm,
        labels: &[usize],
    ) -> Result<Stmt> {
        if self.tables.target.family != TargetFamily::X86_64 {
            return Err(unsupported_asm("target needs assembly lowering"));
        }
        if asm.labels.len() != labels.len() {
            return Err(unsupported_asm("asm goto outside dispatch"));
        }
        if !asm.operands.is_empty() && asm.alternative.is_none() {
            return Err(unsupported_asm(format!(
                "no usable constraint alternative: {:?}",
                asm.rejected
            )));
        }
        let mut options = asm_options(asm)?;
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
        let mut spills = Vec::new();
        for (operand, position) in asm.operands.iter().zip(stack_positions(asm)) {
            spills.push(match (position, &operand.selected) {
                (Some(position), _) => Some(SpillRegister::Stack(position)),
                (None, Some(ir::AsmOperandClass::Register(ir::AsmRegisterClass::MmxReg))) => {
                    let number = (0..MMX_REGISTERS.len())
                        .find(|number| taken.insert(MMX_REGISTERS[*number]))
                        .ok_or_else(|| unsupported_asm("no free mmx register"))?;
                    Some(SpillRegister::Mmx(number))
                }
                _ => None,
            });
        }
        let mut loads = Vec::new();
        let mut stores = Vec::new();
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
            if let Some(register) = spills[index] {
                let [load, store] = self.asm_spill(
                    statement,
                    operand,
                    register,
                    &mut operands,
                    &mut prefix,
                    &mut writebacks,
                )?;
                loads.extend(load);
                stores.extend(store);
                slots.push(Slot::Spill(register));
                continue;
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
                Some(ir::AsmOperandClass::Flags(_)) => {
                    let ir::AsmOperandKind::Out { place, .. } = &operand.kind else {
                        return Err(unsupported_asm("read-write flag output"));
                    };
                    let temp = self.next_temp();
                    prefix.push(Stmt::Let {
                        name: temp.clone(),
                        mutable: false,
                        ty: Some(rust::Type::Prim(Prim::U8)),
                        init: None,
                    });
                    let output = Expr::Var(temp.as_str().into());
                    let decoded = self.asm_decode(output.clone(), &place.ty)?;
                    writebacks.push(self.lower_assignment(place, decoded)?);
                    slots.push(Slot::Positional(operands.len()));
                    operands.push(rust::AsmOperand::Out {
                        reg: rust::AsmReg::Class("reg_byte".into()),
                        late: operand.direction() == ir::AsmDirection::LateOut,
                        value: output,
                    });
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
        let position = |spill: &Spill| match spill.register {
            SpillRegister::Stack(position) => Some(position),
            SpillRegister::Mmx(_) => None,
        };
        for spills in [&loads, &stores] {
            let mut positions = spills.iter().filter_map(position).collect::<Vec<_>>();
            positions.sort_unstable();
            if positions.len() > STACK_REGISTERS.len()
                || positions.iter().copied().ne(0..positions.len())
            {
                return Err(unsupported_asm(format!("x87 stack layout {positions:?}")));
            }
        }
        loads.sort_by_key(|spill| std::cmp::Reverse(position(spill)));
        stores.sort_by_key(position);
        let pops = asm
            .operands
            .iter()
            .zip(&spills)
            .filter(|(operand, register)| match (&operand.kind, register) {
                (ir::AsmOperandKind::In(_), Some(SpillRegister::Stack(position))) => {
                    !asm.clobbers.iter().any(|clobber| {
                        matches!(clobber, ir::AsmClobber::Register(register)
                            if register.canonical == Some(STACK_REGISTERS[*position]))
                    })
                }
                _ => false,
            })
            .count();
        let mut spilled = Vec::new();
        for register in spills.iter().flatten() {
            match register {
                SpillRegister::Stack(_) if !clobbered.contains("st") => {
                    clobbered.extend(STACK_REGISTERS);
                    spilled
                        .extend((0..STACK_REGISTERS.len()).map(|number| format!("st({number})")));
                }
                SpillRegister::Stack(_) => {}
                SpillRegister::Mmx(number) => spilled.push(MMX_REGISTERS[*number].to_string()),
            }
        }
        explicit.extend(spilled.into_iter().map(|name| rust::AsmOperand::Out {
            reg: rust::AsmReg::Explicit(name),
            late: true,
            value: Expr::Var("_".into()),
        }));
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
        let label_slots: Vec<_> = labels
            .iter()
            .map(|label| {
                operands.push(rust::AsmOperand::Label {
                    state: Expr::Var("__slate_state".into()),
                    value: control_flow::state(*label),
                });
                operands.len() - 1
            })
            .collect();
        let mut template = asm_template(asm, &slots, &label_slots)?;
        if !loads.is_empty() || !stores.is_empty() {
            let pop = match asm.dialect {
                Some(ir::AsmDialect::Intel) => "fstp st(0)",
                _ => "fstp %st(0)",
            };
            template = loads
                .iter()
                .map(|spill| spill_instruction(asm, spill, true))
                .chain(std::iter::once(template))
                .chain(
                    stores
                        .iter()
                        .map(|spill| spill_instruction(asm, spill, false)),
                )
                .chain(std::iter::repeat_n(pop.to_string(), pops))
                .collect::<Vec<_>>()
                .join("\n");
        }
        if !stores.is_empty() {
            options.retain(|option| {
                !matches!(
                    option,
                    rust::AsmOption::Pure | rust::AsmOption::NoMem | rust::AsmOption::ReadOnly
                )
            });
        } else if !loads.is_empty() {
            for option in &mut options {
                if matches!(option, rust::AsmOption::NoMem) {
                    *option = rust::AsmOption::ReadOnly;
                }
            }
        }
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
        self.dependencies.asm_goto_with_outputs |= !labels.is_empty()
            && operands.iter().any(|operand| {
                matches!(
                    operand,
                    rust::AsmOperand::Out { .. } | rust::AsmOperand::InOut { .. }
                )
            });
        prefix.push(Stmt::InlineAsm(rust::InlineAsm {
            template,
            operands,
            options,
            naked: false,
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
