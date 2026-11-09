use super::*;
use slate_parser::target_info::{TargetFamily, TargetOs};
use std::fmt::Write as _;

#[derive(Default)]
struct ComdatData {
    directives: String,
    operands: Vec<rust::AsmOperand>,
    cursor: u64,
}

impl ComdatData {
    fn seek(&mut self, offset: u64) -> Option<()> {
        if offset < self.cursor {
            return None;
        }
        if offset > self.cursor {
            let _ = writeln!(self.directives, ".zero {}", offset - self.cursor);
            self.cursor = offset;
        }
        Some(())
    }

    fn scalar(&mut self, offset: u64, size: u64, value: ScalarData) -> Option<()> {
        let directive = match size {
            1 => ".byte",
            2 => ".short",
            4 => ".long",
            8 => ".quad",
            _ => return None,
        };
        self.seek(offset)?;
        match value {
            ScalarData::Literal(literal) => {
                let _ = writeln!(self.directives, "{directive} {literal}");
            }
            ScalarData::Const(expr) => {
                let _ = writeln!(self.directives, "{directive} {{}}");
                self.operands.push(rust::AsmOperand::Const(expr));
            }
        }
        self.cursor = offset + size;
        Some(())
    }
}

enum ScalarData {
    Literal(u64),
    Const(Expr),
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_selectany(
        &mut self,
        global: &ir::Global,
    ) -> Result<(rust::Type, rust::InlineAsm)> {
        let tables = self.tables;
        let variable = &global.variable;
        let unsupported = |detail: &str| -> Failure {
            Construct::Global {
                name: variable.name.clone(),
                detail: format!("selectany {detail}"),
            }
            .into()
        };
        let rest = ir::SymbolAttributes {
            asm_name: None,
            selectany: false,
            include: false,
            ..global.symbol.clone()
        };
        if tables.target.os != TargetOs::Windows {
            return Err(unsupported("outside COFF targets"));
        }
        if rest != ir::SymbolAttributes::default()
            || !matches!(variable.storage, ir::StorageDuration::Static)
            || !variable.access.is_plain()
            || tables.flexible_member(global).is_some()
        {
            return Err(unsupported("with other attributes"));
        }
        let ty = self.lower_binding_type(&variable.ty, variable.id)?;
        let (size, natural) = tables
            .storage_of(&variable.ty)
            .ok_or_else(|| unsupported_type(&variable.ty))?;
        let align = natural.max(variable.alignment.unwrap_or(1));
        let mut data = ComdatData::default();
        if let Some(value) = &variable.initializer {
            self.in_static_initializer = true;
            let lowered = self.comdat_value(value, 0, &mut data);
            self.in_static_initializer = false;
            lowered?;
        }
        data.seek(size)
            .ok_or_else(|| unsupported("initializer overruns its storage"))?;
        let symbol = match &global.symbol.asm_name {
            Some(name) => name.clone(),
            None if tables.target.family == TargetFamily::X86 => format!("_{}", variable.name),
            None => variable.name.clone(),
        };
        let (section, flags) = if variable.is_const {
            (".rdata", "dr")
        } else {
            (".data", "dw")
        };
        let include = if global.symbol.include {
            format!(".section .drectve,\"yni\"\n.ascii \" /include:{symbol}\"\n")
        } else {
            String::new()
        };
        let template = format!(
            ".section {section}${symbol},\"{flags}\",discard,{symbol}\n.globl {symbol}\n.p2align {}\n{symbol}:\n{}{include}.text",
            align.trailing_zeros(),
            data.directives,
        );
        Ok((
            ty,
            rust::InlineAsm {
                template,
                operands: data.operands,
                options: Vec::new(),
                kind: rust::AsmKind::Global,
            },
        ))
    }

    fn comdat_value(
        &mut self,
        value: &ir::Value,
        offset: u64,
        data: &mut ComdatData,
    ) -> Result<()> {
        let tables = self.tables;
        let overlap = || -> Failure {
            Construct::Value {
                kind: "selectany initializer".into(),
                ir: value.display(false).to_string(),
            }
            .into()
        };
        match &value.node.value {
            ValueKind::Aggregate { members, .. } => {
                for member in members {
                    match member.target {
                        ir::AggregateTarget::Field(index) => {
                            let ir::Type::Defined(id) = tables.resolve_type(&value.ty) else {
                                return Err(overlap());
                            };
                            let Some(ir::TypeDefinitionKind::Record {
                                fields: Some(fields),
                                layout: Some(layout),
                                ..
                            }) = tables.types.get(id).map(|definition| &definition.kind)
                            else {
                                return Err(overlap());
                            };
                            if fields
                                .get(index)
                                .is_none_or(|field| field.bit_width.is_some())
                            {
                                return Err(overlap());
                            }
                            self.comdat_value(&member.value, offset + layout.offsets[index], data)?;
                        }
                        ir::AggregateTarget::Index(index) => {
                            let stride = self.comdat_stride(&value.ty)?;
                            self.comdat_value(&member.value, offset + index * stride, data)?;
                        }
                        ir::AggregateTarget::Range { start, end } => {
                            let stride = self.comdat_stride(&value.ty)?;
                            for index in start..=end {
                                self.comdat_value(&member.value, offset + index * stride, data)?;
                            }
                        }
                    }
                }
                Ok(())
            }
            ValueKind::CodeUnits(units) => {
                let stride = self.comdat_stride(&value.ty)?;
                units.iter().enumerate().try_for_each(|(index, &unit)| {
                    data.scalar(
                        offset + index as u64 * stride,
                        stride,
                        ScalarData::Literal(u64::from(unit)),
                    )
                    .ok_or_else(overlap)
                })
            }
            _ => {
                let (size, _) = tables
                    .storage_of(&value.ty)
                    .ok_or_else(|| unsupported_type(&value.ty))?;
                let lowered = self.lower_value(value)?;
                let scalar = match tables.resolve_type(&value.ty) {
                    ir::Type::Bool => Expr::Cast {
                        expr: Box::new(lowered),
                        ty: rust::Type::Prim(Prim::U8),
                    },
                    ir::Type::Numeric(ir::NumericType::Integer {
                        bit_precise: false, ..
                    }) => lowered,
                    ir::Type::Numeric(ir::NumericType::Float(
                        ir::FloatType::F32 | ir::FloatType::F64,
                    )) => Expr::MethodCall {
                        recv: Box::new(lowered),
                        method: "to_bits".into(),
                        args: Vec::new(),
                    },
                    _ => return Err(overlap()),
                };
                data.scalar(offset, size, ScalarData::Const(scalar))
                    .ok_or_else(overlap)
            }
        }
    }

    fn comdat_stride(&self, ty: &ir::Type) -> Result<u64> {
        let ir::Type::Array { element, .. } = self.tables.resolve_type(ty) else {
            return Err(unsupported_type(ty));
        };
        self.tables
            .storage_of(element)
            .map(|(size, _)| size)
            .ok_or_else(|| unsupported_type(element))
    }
}
