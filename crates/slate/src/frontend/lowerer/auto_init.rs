use super::*;
use slate_parser::ir::{FloatType, NumericType, PatternFill};

const X87_VALUE_BYTES: u64 = 10;
const MAX_RUNS: usize = 512;

struct Painter<'t, 'm> {
    tables: &'t Tables<'m>,
    fill: PatternFill,
    root: &'t ir::Type,
    image: Vec<Option<u8>>,
}

fn float_len(format: FloatType, storage: u64) -> u64 {
    match format {
        FloatType::F80 => storage.min(X87_VALUE_BYTES),
        _ => storage,
    }
}

impl Painter<'_, '_> {
    fn set(&mut self, offset: u64, len: u64, byte: u8) -> Result<()> {
        let range = offset as usize..(offset + len) as usize;
        let slot = self
            .image
            .get_mut(range)
            .ok_or_else(|| unsupported_type(self.root))?;
        slot.fill(Some(byte));
        Ok(())
    }

    fn size_of(&self, ty: &ir::Type) -> Result<u64> {
        self.tables
            .storage_of(ty)
            .map(|(size, _)| size)
            .ok_or_else(|| unsupported_type(ty))
    }

    fn paint_bit_run(&mut self, run: Option<(u64, u64)>) -> Result<()> {
        let Some((start, len)) = run.filter(|_| self.fill.bit_runs) else {
            return Ok(());
        };
        for bit in start..start + len {
            let slot = self
                .image
                .get_mut((bit / 8) as usize)
                .ok_or_else(|| unsupported_type(self.root))?;
            let byte = slot.get_or_insert(0);
            if (self.fill.integer >> ((bit - start) % 8)) & 1 == 1 {
                *byte |= 1 << (bit % 8);
            }
        }
        Ok(())
    }

    fn paint_numeric(&mut self, numeric: NumericType, offset: u64, size: u64) -> Result<()> {
        match numeric {
            NumericType::Integer { .. } => self.set(offset, size, self.fill.integer),
            NumericType::Float(format) => {
                self.set(offset, float_len(format, size), self.fill.floating)
            }
        }
    }

    fn paint(&mut self, ty: &ir::Type, base: u64, lone: bool) -> Result<()> {
        let tables = self.tables;
        match tables.resolve_type(ty) {
            ir::Type::Bool => {
                let byte = if lone {
                    self.fill.lone_bool
                } else {
                    self.fill.integer
                };
                let size = self.size_of(ty)?;
                self.set(base, size, byte)
            }
            ir::Type::Numeric(numeric) => {
                let size = self.size_of(ty)?;
                self.paint_numeric(*numeric, base, size)
            }
            ir::Type::Complex(numeric) => {
                let half = self.size_of(ty)? / 2;
                self.paint_numeric(*numeric, base, half)?;
                self.paint_numeric(*numeric, base + half, half)
            }
            ir::Type::Imaginary(format) => {
                let size = self.size_of(ty)?;
                self.set(base, float_len(*format, size), self.fill.floating)
            }
            ir::Type::Vector { element, lanes } => {
                let lane = self.size_of(&ir::Type::Numeric(*element))?;
                for index in 0..u64::from(*lanes) {
                    self.paint_numeric(*element, base + index * lane, lane)?;
                }
                Ok(())
            }
            ir::Type::FixedPoint(_) | ir::Type::Pointer { .. } | ir::Type::VaList => {
                let size = self.size_of(ty)?;
                self.set(base, size, self.fill.integer)
            }
            ir::Type::Array { element, length } => {
                let stride = self.size_of(element)?;
                for index in 0..length.unwrap_or(0) {
                    self.paint(element, base + index * stride, false)?;
                }
                Ok(())
            }
            ir::Type::Defined(id) => {
                match tables.types.get(id).map(|definition| &definition.kind) {
                    Some(ir::TypeDefinitionKind::Record {
                        fields: Some(fields),
                        layout: Some(layout),
                        ..
                    }) => {
                        let mut run: Option<(u64, u64)> = None;
                        for (index, field) in fields.iter().enumerate() {
                            match (field.bit_width, layout.bit_offsets.get(index)) {
                                (Some(0), _) => self.paint_bit_run(run.take())?,
                                (Some(width), Some(Some(bit))) => {
                                    let start = base * 8 + bit;
                                    let width = u64::from(width);
                                    run = match run {
                                        Some((first, len)) if first + len == start => {
                                            Some((first, len + width))
                                        }
                                        previous => {
                                            self.paint_bit_run(previous)?;
                                            Some((start, width))
                                        }
                                    };
                                }
                                (Some(_), _) => return Err(unsupported_type(ty)),
                                (None, _) => {
                                    self.paint_bit_run(run.take())?;
                                    self.paint(&field.ty, base + layout.offsets[index], false)?;
                                }
                            }
                        }
                        self.paint_bit_run(run)
                    }
                    Some(ir::TypeDefinitionKind::Enum {
                        layout: Some(layout),
                        ..
                    }) => self.set(base, layout.size_bytes, self.fill.integer),
                    _ => Err(unsupported_type(ty)),
                }
            }
            _ => Err(unsupported_type(ty)),
        }
    }

    fn runs(&self, initial: u8) -> Vec<(u64, u64, u8)> {
        let mut runs: Vec<(u64, u64, u8)> = Vec::new();
        for (offset, byte) in self.image.iter().enumerate() {
            let Some(byte) = byte.filter(|byte| *byte != initial) else {
                continue;
            };
            match runs.last_mut() {
                Some((start, len, last)) if *last == byte && *start + *len == offset as u64 => {
                    *len += 1;
                }
                _ => runs.push((offset as u64, 1, byte)),
            }
        }
        runs
    }
}

fn byte_value(byte: u8) -> Expr {
    Expr::Value(rust::RustValue::TypedUInt(u128::from(byte), Prim::U8))
}

fn usize_value(value: u64) -> Expr {
    Expr::Value(rust::RustValue::Usize(value as usize))
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn auto_init_writes(
        &self,
        ty: &ir::Type,
        auto: ir::AutoInit,
        target: Expr,
        zeroed: bool,
    ) -> Result<Vec<Stmt>> {
        let tables = self.tables;
        let size = tables
            .storage_of(ty)
            .map(|(size, _)| size)
            .ok_or_else(|| unsupported_type(ty))?;
        let fill = match auto {
            ir::AutoInit::Zero => PatternFill {
                integer: 0,
                floating: 0,
                lone_bool: 0,
                padding: Some(0),
                bit_runs: false,
            },
            ir::AutoInit::Pattern(fill) => fill,
        };
        let mut painter = Painter {
            tables,
            fill,
            root: ty,
            image: vec![None; size as usize],
        };
        painter.paint(ty, 0, true)?;
        let initial = fill.padding.unwrap_or(0);
        let runs = painter.runs(initial);
        if runs.len() > MAX_RUNS {
            return Err(unsupported_type(ty));
        }
        let bytes = "__slate_auto_bytes";
        let write = |offset: u64, len: u64, byte: u8| {
            Stmt::Expr(Expr::WriteBytes {
                dst: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::Var(bytes.into())),
                    method: "add".into(),
                    args: vec![usize_value(offset)],
                }),
                val: Box::new(byte_value(byte)),
                count: Box::new(usize_value(len)),
            })
        };
        let mut writes = Vec::new();
        if initial != 0 || !zeroed {
            writes.push(write(0, size, initial));
        }
        writes.extend(
            runs.into_iter()
                .map(|(offset, len, byte)| write(offset, len, byte)),
        );
        if writes.is_empty() {
            return Ok(writes);
        }
        writes.insert(
            0,
            Stmt::Let {
                name: bytes.into(),
                mutable: false,
                ty: None,
                init: Some(target),
            },
        );
        Ok(writes)
    }

    pub(super) fn local_bytes(&self, id: BindingId) -> Expr {
        Expr::Cast {
            expr: Box::new(Expr::Cast {
                expr: Box::new(Expr::AddrOf {
                    mutable: true,
                    expr: Box::new(self.lower_binding(id)),
                }),
                ty: rust::Type::Custom("*mut _".into()),
            }),
            ty: rust::Type::Ptr {
                mutable: true,
                inner: Box::new(rust::Type::Prim(Prim::U8)),
            },
        }
    }
}
