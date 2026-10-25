use super::*;
use slate_parser::ir::{FloatType, NumericType, PatternFill};

const X87_VALUE_BYTES: u64 = 10;
const MAX_WRITES: usize = 512;
const UNROLL_WRITES: u64 = 8;
const BYTES: &str = "__slate_auto_bytes";

#[derive(Clone)]
enum Write {
    Bytes {
        offset: u64,
        len: u64,
        byte: u8,
    },
    Repeat {
        offset: u64,
        stride: u64,
        count: u64,
        body: Vec<Write>,
    },
}

impl Write {
    fn shifted(&self, by: u64) -> Self {
        match self {
            Self::Bytes { offset, len, byte } => Self::Bytes {
                offset: offset + by,
                len: *len,
                byte: *byte,
            },
            Self::Repeat {
                offset,
                stride,
                count,
                body,
            } => Self::Repeat {
                offset: offset + by,
                stride: *stride,
                count: *count,
                body: body.clone(),
            },
        }
    }

    fn weight(&self) -> usize {
        match self {
            Self::Bytes { .. } => 1,
            Self::Repeat { body, .. } => 1 + weight(body),
        }
    }

    fn render(&self, image: &mut [u8]) -> Option<()> {
        match self {
            Self::Bytes { offset, len, byte } => image
                .get_mut(*offset as usize..(offset + len) as usize)?
                .iter_mut()
                .for_each(|slot| *slot |= byte),
            Self::Repeat {
                offset,
                stride,
                count,
                body,
            } => {
                for index in 0..*count {
                    for write in body {
                        write.shifted(offset + index * stride).render(image)?;
                    }
                }
            }
        }
        Some(())
    }
}

fn weight(writes: &[Write]) -> usize {
    writes.iter().map(Write::weight).sum()
}

fn push(writes: &mut Vec<Write>, write: Write) {
    if let (
        Some(Write::Bytes {
            offset,
            len,
            byte: last,
        }),
        Write::Bytes {
            offset: next,
            len: more,
            byte,
        },
    ) = (writes.last_mut(), &write)
        && *last == *byte
        && *offset + *len == *next
    {
        *len += more;
        return;
    }
    writes.push(write);
}

fn from_image(image: &[u8]) -> Vec<Write> {
    let mut writes = Vec::new();
    for (offset, byte) in image.iter().enumerate() {
        if *byte != 0 {
            push(
                &mut writes,
                Write::Bytes {
                    offset: offset as u64,
                    len: 1,
                    byte: *byte,
                },
            );
        }
    }
    writes
}

#[derive(Clone, Copy)]
enum Fill {
    Typed { integer: u8, floating: u8 },
    ValueBits(u8),
}

impl Fill {
    fn background(self) -> u8 {
        match self {
            Self::Typed { integer, .. } => integer,
            Self::ValueBits(_) => 0,
        }
    }
}

struct Painter<'t, 'm> {
    tables: &'t Tables<'m>,
    fill: Fill,
    root: &'t ir::Type,
}

impl Painter<'_, '_> {
    fn size_of(&self, ty: &ir::Type) -> Result<u64> {
        self.storage(ty).map(|(size, _)| size)
    }

    fn storage(&self, ty: &ir::Type) -> Result<(u64, u64)> {
        self.tables
            .storage_of(ty)
            .ok_or_else(|| unsupported_type(ty))
    }

    fn bytes(&self, writes: &mut Vec<Write>, offset: u64, len: u64, byte: u8) {
        if len > 0 && byte != self.fill.background() {
            push(writes, Write::Bytes { offset, len, byte });
        }
    }

    fn integer(&self, writes: &mut Vec<Write>, offset: u64, len: u64) {
        let byte = match self.fill {
            Fill::Typed { integer, .. } => integer,
            Fill::ValueBits(byte) => byte,
        };
        self.bytes(writes, offset, len, byte);
    }

    fn value_bits(&self, writes: &mut Vec<Write>, offset: u64, bits: u64) {
        let Fill::ValueBits(byte) = self.fill else {
            return;
        };
        self.bytes(writes, offset, bits / 8, byte);
        if !bits.is_multiple_of(8) {
            self.bytes(writes, offset + bits / 8, 1, byte & ((1 << (bits % 8)) - 1));
        }
    }

    fn numeric(&self, writes: &mut Vec<Write>, numeric: NumericType, offset: u64, size: u64) {
        match (numeric, self.fill) {
            (
                NumericType::Integer {
                    width,
                    bit_precise: true,
                    ..
                },
                Fill::ValueBits(_),
            ) => self.value_bits(writes, offset, u64::from(width).min(size * 8)),
            (NumericType::Integer { .. }, _) => self.integer(writes, offset, size),
            (NumericType::Float(format), _) => {
                let value = match format {
                    FloatType::F80 => size.min(X87_VALUE_BYTES),
                    _ => size,
                };
                let byte = match self.fill {
                    Fill::Typed { floating, .. } => floating,
                    Fill::ValueBits(byte) => byte,
                };
                self.bytes(writes, offset, value, byte);
                self.bytes(writes, offset + value, size - value, 0);
            }
        }
    }

    fn writes(&self, ty: &ir::Type) -> Result<Vec<Write>> {
        let tables = self.tables;
        let mut writes = Vec::new();
        match tables.resolve_type(ty) {
            ir::Type::Bool
            | ir::Type::FixedPoint(_)
            | ir::Type::Pointer { .. }
            | ir::Type::VaList => self.integer(&mut writes, 0, self.size_of(ty)?),
            ir::Type::Numeric(numeric) => {
                self.numeric(&mut writes, *numeric, 0, self.size_of(ty)?);
            }
            ir::Type::Complex(numeric) => {
                let half = self.size_of(ty)? / 2;
                self.numeric(&mut writes, *numeric, 0, half);
                self.numeric(&mut writes, *numeric, half, half);
            }
            ir::Type::Imaginary(format) => {
                let size = self.size_of(ty)?;
                self.numeric(&mut writes, NumericType::Float(*format), 0, size);
            }
            ir::Type::Vector { element, lanes } => {
                let lane = self.size_of(&ir::Type::Numeric(*element))?;
                for index in 0..u64::from(*lanes) {
                    self.numeric(&mut writes, *element, index * lane, lane);
                }
            }
            ir::Type::Array { element, length } => {
                let stride = self.size_of(element)?;
                return Ok(repeat(self.writes(element)?, stride, length.unwrap_or(0)));
            }
            ir::Type::Defined(id) => {
                match tables.types.get(id).map(|definition| &definition.kind) {
                    Some(ir::TypeDefinitionKind::Record {
                        kind,
                        fields: Some(fields),
                        layout: Some(layout),
                    }) => {
                        return match kind {
                            ir::RecordKind::Struct => self.structure(ty, fields, layout),
                            ir::RecordKind::Union => self.union(ty, fields, layout),
                        };
                    }
                    Some(ir::TypeDefinitionKind::Enum {
                        layout: Some(layout),
                        ..
                    }) => self.integer(&mut writes, 0, layout.size_bytes),
                    _ => return Err(unsupported_type(ty)),
                }
            }
            _ => return Err(unsupported_type(ty)),
        }
        Ok(writes)
    }

    fn structure(
        &self,
        ty: &ir::Type,
        fields: &[slate_parser::ast::Span<ir::Field>],
        layout: &ir::RecordLayout,
    ) -> Result<Vec<Write>> {
        let mut image = vec![0u8; layout.size as usize];
        let mut writes = Vec::new();
        let mut bit_fields = false;
        for (index, field) in fields.iter().enumerate() {
            match (field.bit_width, layout.bit_offsets.get(index)) {
                (Some(0), _) => {}
                (Some(width), Some(Some(bit))) => {
                    let Fill::ValueBits(byte) = self.fill else {
                        continue;
                    };
                    bit_fields = true;
                    for bit in *bit..bit + u64::from(width) {
                        let slot = image
                            .get_mut((bit / 8) as usize)
                            .ok_or_else(|| unsupported_type(self.root))?;
                        *slot |= byte & (1 << (bit % 8));
                    }
                }
                (Some(_), _) => return Err(unsupported_type(ty)),
                (None, _) => {
                    let offset = layout.offsets[index];
                    for write in self.writes(&field.ty)? {
                        push(&mut writes, write.shifted(offset));
                    }
                }
            }
        }
        if !bit_fields {
            return Ok(writes);
        }
        for write in &writes {
            write
                .render(&mut image)
                .ok_or_else(|| unsupported_type(self.root))?;
        }
        Ok(from_image(&image))
    }

    fn union(
        &self,
        ty: &ir::Type,
        fields: &[slate_parser::ast::Span<ir::Field>],
        layout: &ir::RecordLayout,
    ) -> Result<Vec<Write>> {
        match self.fill {
            Fill::Typed { .. } => {
                let mut storage: Option<(&ir::Field, u64, u64)> = None;
                for field in fields {
                    let (size, align) = match field.bit_width {
                        Some(0) => continue,
                        Some(width) => {
                            let bytes = u64::from(width).div_ceil(8).next_power_of_two();
                            (bytes, bytes)
                        }
                        None => self.storage(&field.ty)?,
                    };
                    if storage.is_none_or(|(_, best_size, best_align)| {
                        align > best_align || (align == best_align && size > best_size)
                    }) {
                        storage = Some((field, size, align));
                    }
                }
                match storage {
                    Some((field, ..)) if field.bit_width.is_none() => self.writes(&field.ty),
                    _ => Ok(Vec::new()),
                }
            }
            Fill::ValueBits(byte) => {
                let mut image = vec![0u8; layout.size as usize];
                for field in fields {
                    let writes = match field.bit_width {
                        Some(width) => vec![Write::Bytes {
                            offset: 0,
                            len: u64::from(width).div_ceil(8),
                            byte,
                        }],
                        None => self.writes(&field.ty)?,
                    };
                    for write in writes {
                        write
                            .render(&mut image)
                            .ok_or_else(|| unsupported_type(ty))?;
                    }
                }
                Ok(from_image(&image))
            }
        }
    }
}

fn repeat(element: Vec<Write>, stride: u64, count: u64) -> Vec<Write> {
    match element.as_slice() {
        [] => Vec::new(),
        [
            Write::Bytes {
                offset: 0,
                len,
                byte,
            },
        ] if *len == stride => vec![Write::Bytes {
            offset: 0,
            len: stride * count,
            byte: *byte,
        }],
        _ if count * weight(&element) as u64 <= UNROLL_WRITES => {
            let mut writes = Vec::new();
            for index in 0..count {
                for write in &element {
                    push(&mut writes, write.shifted(index * stride));
                }
            }
            writes
        }
        _ => vec![Write::Repeat {
            offset: 0,
            stride,
            count,
            body: element,
        }],
    }
}

fn byte_value(byte: u8) -> Expr {
    Expr::Value(rust::RustValue::TypedUInt(u128::from(byte), Prim::U8))
}

fn usize_value(value: u64) -> Expr {
    Expr::Value(rust::RustValue::Usize(value as usize))
}

fn emit(writes: &[Write], base: &str, depth: usize) -> Vec<Stmt> {
    writes
        .iter()
        .map(|write| match write {
            Write::Bytes { offset, len, byte } => Stmt::Expr(Expr::WriteBytes {
                dst: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::Var(base.into())),
                    method: "add".into(),
                    args: vec![usize_value(*offset)],
                }),
                val: Box::new(byte_value(*byte)),
                count: Box::new(usize_value(*len)),
            }),
            Write::Repeat {
                offset,
                stride,
                count,
                body,
            } => {
                let index = format!("__slate_auto_index{depth}");
                let element = format!("{BYTES}{depth}");
                let mut stmts = vec![Stmt::Let {
                    name: element.clone(),
                    mutable: false,
                    ty: None,
                    init: Some(Expr::MethodCall {
                        recv: Box::new(Expr::Var(base.into())),
                        method: "add".into(),
                        args: vec![Expr::Binary {
                            op: rust::BinOp::Add,
                            lhs: Box::new(usize_value(*offset)),
                            rhs: Box::new(Expr::Binary {
                                op: rust::BinOp::Mul,
                                lhs: Box::new(Expr::Var(index.as_str().into())),
                                rhs: Box::new(usize_value(*stride)),
                            }),
                        }],
                    }),
                }];
                stmts.extend(emit(body, &element, depth + 1));
                Stmt::For {
                    pat: index,
                    iter: Expr::Range {
                        start: Box::new(usize_value(0)),
                        end: Box::new(usize_value(*count)),
                    },
                    body: stmts,
                }
            }
        })
        .collect()
}

fn prim_bytes(prim: Prim, pointer_bytes: u32) -> Option<u32> {
    Some(match prim {
        Prim::I8 | Prim::U8 => 1,
        Prim::I16 | Prim::U16 => 2,
        Prim::I32 | Prim::U32 => 4,
        Prim::I64 | Prim::U64 => 8,
        Prim::I128 | Prim::U128 => 16,
        Prim::Isize | Prim::Usize => pointer_bytes,
        _ => return None,
    })
}

fn prim_signed(prim: Prim) -> bool {
    matches!(
        prim,
        Prim::I8 | Prim::I16 | Prim::I32 | Prim::I64 | Prim::I128 | Prim::Isize
    )
}

fn repeated(byte: u8, bytes: u32) -> u128 {
    (0..bytes).fold(0, |value, _| value << 8 | u128::from(byte))
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn auto_init_value(
        &mut self,
        ty: &ir::Type,
        auto: ir::AutoInit,
    ) -> Result<Option<Expr>> {
        let ir::AutoInit::Pattern(fill) = auto else {
            return Ok(None);
        };
        let (integer, floating) = match fill {
            PatternFill::Typed { integer, floating } => (integer, floating),
            PatternFill::ValueBits(byte) => (byte, byte),
        };
        let tables = self.tables;
        let leaf = match tables.resolve_type(ty) {
            ir::Type::Array {
                element,
                length: Some(length),
            } => {
                return Ok(self
                    .auto_init_value(element, auto)?
                    .map(|elem| Expr::ArrayRepeat {
                        elem: Box::new(elem),
                        len: *length as usize,
                    }));
            }
            ir::Type::Numeric(NumericType::Integer {
                bit_precise: false, ..
            })
            | ir::Type::Pointer { .. } => integer,
            ir::Type::Numeric(NumericType::Float(FloatType::F32 | FloatType::F64)) => floating,
            ir::Type::Bool if integer <= 1 => {
                return Ok(Some(Expr::Value(rust::RustValue::Bool(integer == 1))));
            }
            _ => return Ok(None),
        };
        Ok(match self.lower_type(ty)? {
            rust::Type::Prim(prim @ (Prim::F32 | Prim::F64)) => {
                let (bits, width, method) = match prim {
                    Prim::F32 => (Prim::U32, 4, "f32::from_bits"),
                    _ => (Prim::U64, 8, "f64::from_bits"),
                };
                Some(Expr::Call {
                    func: Box::new(Expr::Var(method.into())),
                    args: vec![Expr::Value(rust::RustValue::TypedUInt(
                        repeated(leaf, width),
                        bits,
                    ))],
                    binding: CallBinding::Generated,
                })
            }
            rust::Type::Prim(prim) => {
                let Some(bytes) = prim_bytes(prim, self.target_pointer_bytes()) else {
                    return Ok(None);
                };
                let value = repeated(leaf, bytes);
                Some(Expr::Value(if prim_signed(prim) {
                    let shift = 128 - bytes * 8;
                    rust::RustValue::TypedInt(((value << shift) as i128) >> shift, prim)
                } else {
                    rust::RustValue::TypedUInt(value, prim)
                }))
            }
            ty @ rust::Type::Ptr { .. } => {
                let bytes = self.target_pointer_bytes();
                Some(Expr::Cast {
                    expr: Box::new(Expr::Value(rust::RustValue::TypedUInt(
                        repeated(leaf, bytes),
                        Prim::Usize,
                    ))),
                    ty,
                })
            }
            _ => None,
        })
    }

    fn target_pointer_bytes(&self) -> u32 {
        self.tables.target.pointer_width / 8
    }

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
            ir::AutoInit::Zero => Fill::ValueBits(0),
            ir::AutoInit::Pattern(PatternFill::Typed { integer, floating }) => {
                Fill::Typed { integer, floating }
            }
            ir::AutoInit::Pattern(PatternFill::ValueBits(byte)) => Fill::ValueBits(byte),
        };
        let painter = Painter {
            tables,
            fill,
            root: ty,
        };
        let writes = painter.writes(ty)?;
        if weight(&writes) > MAX_WRITES {
            return Err(unsupported_type(ty));
        }
        let background = fill.background();
        let covered = matches!(
            writes.as_slice(),
            [Write::Bytes { offset: 0, len, .. }] if *len == size
        );
        let mut stmts = Vec::new();
        if !covered && (background != 0 || !zeroed || tables.has_padding_bytes(ty)) {
            stmts.extend(emit(
                &[Write::Bytes {
                    offset: 0,
                    len: size,
                    byte: background,
                }],
                BYTES,
                0,
            ));
        }
        stmts.extend(emit(&writes, BYTES, 0));
        if stmts.is_empty() {
            return Ok(stmts);
        }
        stmts.insert(
            0,
            Stmt::Let {
                name: BYTES.into(),
                mutable: false,
                ty: None,
                init: Some(target),
            },
        );
        Ok(stmts)
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

impl Tables<'_> {
    fn has_padding_bytes(&self, ty: &ir::Type) -> bool {
        match self.resolve_type(ty) {
            ir::Type::Array { element, .. } => self.has_padding_bytes(element),
            ir::Type::Defined(id) => matches!(
                self.types.get(id).map(|definition| &definition.kind),
                Some(ir::TypeDefinitionKind::Record { .. })
            ),
            ir::Type::Numeric(NumericType::Float(FloatType::F80))
            | ir::Type::Complex(NumericType::Float(FloatType::F80))
            | ir::Type::Imaginary(FloatType::F80) => true,
            ir::Type::Numeric(NumericType::Integer {
                bit_precise: true, ..
            }) => true,
            _ => false,
        }
    }
}
