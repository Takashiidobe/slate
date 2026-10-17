use super::*;
use slate_parser::target_info::Endian;

impl Tables<'_> {
    pub(super) fn padding_keep(&self, ty: &ir::Type) -> Option<Vec<u8>> {
        if !matches!(self.target.endian, Endian::Little) {
            return None;
        }
        let (size, _) = self.storage_of(ty)?;
        let mut keep = vec![0u8; usize::try_from(size).ok()?];
        self.mark_data(ty, 0, &mut keep)?;
        Some(keep)
    }

    fn mark_data(&self, ty: &ir::Type, base: u64, keep: &mut [u8]) -> Option<()> {
        match self.resolve_type(ty) {
            ir::Type::Defined(id) => {
                let ir::TypeDefinitionKind::Record {
                    fields: Some(fields),
                    layout: Some(layout),
                    ..
                } = &self.types.get(id)?.kind
                else {
                    return None;
                };
                for (index, field) in fields.iter().enumerate() {
                    match (field.bit_width, layout.bit_offsets[index]) {
                        (Some(width), Some(bit_offset)) if field.name.is_some() => {
                            for bit in bit_offset..bit_offset + u64::from(width) {
                                let byte = usize::try_from(base + bit / 8).ok()?;
                                *keep.get_mut(byte)? |= 1 << (bit % 8);
                            }
                        }
                        (Some(_), _) => {}
                        (None, _) => {
                            self.mark_data(&field.ty, base + layout.offsets[index], keep)?;
                        }
                    }
                }
                Some(())
            }
            ir::Type::Array { element, length } => {
                let (stride, _) = self.storage_of(element)?;
                (0..length.unwrap_or(0))
                    .try_for_each(|index| self.mark_data(element, base + index * stride, keep))
            }
            ty => {
                let (size, _) = self.storage_of(ty)?;
                let start = usize::try_from(base).ok()?;
                keep.get_mut(start..start + usize::try_from(size).ok()?)?
                    .fill(0xFF);
                Some(())
            }
        }
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_clear_padding(
        &mut self,
        value: &ir::Value,
        pointer: &ir::Value,
    ) -> Result<Expr> {
        let ir::Type::Pointer { pointee, .. } = self.tables.resolve_type(&pointer.ty) else {
            return Err(unsupported_value(value));
        };
        let keep = self
            .tables
            .padding_keep(pointee)
            .ok_or_else(|| unsupported_value(value))?;
        let base = self.lower_value(pointer)?;
        let bytes = self.next_temp();
        let byte_pointer = rust::Type::Ptr {
            mutable: true,
            inner: Box::new(rust::Type::Prim(Prim::U8)),
        };
        let mut stmts = vec![Stmt::Let {
            name: bytes.clone(),
            mutable: false,
            ty: Some(byte_pointer.clone()),
            init: Some(Expr::Cast {
                expr: Box::new(base),
                ty: byte_pointer,
            }),
        }];
        let at = |offset: usize| Expr::MethodCall {
            recv: Box::new(Expr::Var(bytes.as_str().into())),
            method: "add".into(),
            args: vec![Expr::Value(rust::RustValue::TypedUInt(
                offset as u128,
                Prim::Usize,
            ))],
        };
        let mut start = 0;
        while start < keep.len() {
            let mask = keep[start];
            let length = keep[start..]
                .iter()
                .take_while(|&&byte| byte == mask)
                .count();
            match mask {
                0xFF => {}
                0 => stmts.push(Stmt::Expr(Expr::MethodCall {
                    recv: Box::new(at(start)),
                    method: "write_bytes".into(),
                    args: vec![
                        Expr::Value(rust::RustValue::TypedUInt(0, Prim::U8)),
                        Expr::Value(rust::RustValue::TypedUInt(length as u128, Prim::Usize)),
                    ],
                })),
                mask => stmts.extend((start..start + length).map(|offset| Stmt::CompoundAssign {
                    target: Expr::Unary {
                        op: rust::UnaryOp::Deref,
                        expr: Box::new(at(offset)),
                    },
                    op: rust::BinOp::BitAnd,
                    value: Expr::Value(rust::RustValue::TypedUInt(mask.into(), Prim::U8)),
                })),
            }
            start += length;
        }
        Ok(Expr::Unsafe(Box::new(rust::Block { stmts, tail: None })))
    }
}
