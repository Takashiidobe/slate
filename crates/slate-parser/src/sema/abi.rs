use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::types::TypeResolver;
use crate::ast::Span;
use crate::compiler_args::CompilerFlavor;
use crate::ir::{
    AbiChunk, AbiConvention, AbiPass, AbiSignature, Field, FloatType, NumericType, RecordKind,
    Type, TypeDefinitionKind, Value,
};
use crate::target_info::{TargetEnvironment, TargetFamily, TargetInfo};

impl Lowerer {
    pub(super) fn abi_signature(
        &self,
        signature: &Type,
        actual_arguments: Option<&[Value]>,
    ) -> Result<AbiSignature, ResolveError> {
        AbiClassifier::new(&self.types, &self.context.target).signature(
            signature,
            None,
            actual_arguments,
        )
    }

    pub(super) fn c_abi_signature(
        &self,
        signature: crate::sema::ctype::QualType,
        ir: &Type,
        actual_arguments: Option<&[Value]>,
    ) -> Result<AbiSignature, ResolveError> {
        AbiClassifier::new(&self.types, &self.context.target).signature(
            ir,
            Some(signature),
            actual_arguments,
        )
    }
}

#[derive(Debug, Clone)]
pub(super) struct AbiOperand {
    pub ty: Type,
    pub atomic: bool,
}

pub(super) struct AbiClassifier<'a> {
    types: &'a TypeResolver,
    target: &'a TargetInfo,
}

impl<'a> AbiClassifier<'a> {
    pub(super) fn new(types: &'a TypeResolver, target: &'a TargetInfo) -> Self {
        Self { types, target }
    }

    fn signature(
        &self,
        signature: &Type,
        c_signature: Option<crate::sema::ctype::QualType>,
        actual_arguments: Option<&[Value]>,
    ) -> Result<AbiSignature, ResolveError> {
        let Type::Function {
            return_type,
            parameters,
            variadic,
            ..
        } = signature
        else {
            return Err(ResolveError::Unsupported("ABI of non-function type"));
        };
        let c_parts = c_signature.and_then(|q| self.types.ctypes.function_parts(q));
        let atomic_result = c_parts.is_some_and(|(ret, ..)| self.types.ctypes.quals(ret).is_atomic);
        let atomic_arguments = |index: usize| {
            c_parts.is_some_and(|(_, params, ..)| {
                params
                    .get(index)
                    .is_some_and(|param| self.types.ctypes.quals(*param).is_atomic)
            })
        };
        let argument_types: Vec<_> = actual_arguments
            .map_or_else(
                || parameters.clone(),
                |values| values.iter().map(|value| value.ty.clone()).collect(),
            )
            .into_iter()
            .enumerate()
            .map(|(index, ty)| AbiOperand {
                ty,
                atomic: atomic_arguments(index),
            })
            .collect();
        let result = return_type.as_deref().map(|ty| AbiOperand {
            ty: ty.clone(),
            atomic: atomic_result,
        });
        self.from_parts(
            result.as_ref(),
            &argument_types,
            *variadic,
            parameters.len(),
        )
    }

    #[expect(clippy::wrong_self_convention, reason = "ok for now")]
    pub(super) fn from_parts(
        &self,
        return_type: Option<&AbiOperand>,
        parameters: &[AbiOperand],
        variadic: bool,
        fixed_count: usize,
    ) -> Result<AbiSignature, ResolveError> {
        let convention = self.abi_convention(variadic);
        let arguments = parameters
            .iter()
            .enumerate()
            .map(|(index, operand)| -> Result<AbiPass, ResolveError> {
                let pass = self.abi_pass(operand, false, convention)?;
                if convention == AbiConvention::WinArm64
                    && variadic
                    && index >= fixed_count
                    && matches!(pass, AbiPass::Coerce(_))
                    && matches!(operand.ty, Type::Complex(_) | Type::Defined(_))
                {
                    let layout = self.layout(operand)?;
                    Ok(integer_chunks(layout.size_bytes, 64))
                } else {
                    Ok(pass)
                }
            })
            .collect::<Result<Vec<_>, _>>()?;
        let result = match return_type {
            Some(operand) => self.abi_pass(operand, true, convention)?,
            None => AbiPass::Void,
        };
        Ok(AbiSignature {
            convention,
            arguments,
            result,
        })
    }

    fn abi_convention(&self, variadic: bool) -> AbiConvention {
        let target = self.target;
        match (target.family, target.environment) {
            (TargetFamily::X86_64, TargetEnvironment::Msvc) => AbiConvention::Win64,
            (TargetFamily::X86_64, _) => AbiConvention::SysV64,
            (TargetFamily::X86, _) => AbiConvention::X86Cdecl,
            (TargetFamily::AArch64, TargetEnvironment::Msvc) => AbiConvention::WinArm64,
            (TargetFamily::AArch64, _) => AbiConvention::Aapcs64,
            (TargetFamily::Arm32, _) if !variadic && target.isa.arm_hard_float() => {
                AbiConvention::Aapcs32HardFloat
            }
            (TargetFamily::Arm32, _) => AbiConvention::Aapcs32,
        }
    }

    fn layout(
        &self,
        operand: &AbiOperand,
    ) -> Result<crate::target_info::StorageLayout, ResolveError> {
        self.types
            .qualified_storage(operand.ty.clone(), operand.atomic)
    }

    fn atomic_is_memory(&self, operand: &AbiOperand, convention: AbiConvention) -> bool {
        if !operand.atomic || !matches!(self.types.flavor, CompilerFlavor::Clang) {
            return false;
        }
        if !matches!(convention, AbiConvention::SysV64 | AbiConvention::X86Cdecl) {
            return false;
        }
        self.is_record_or_complex(&operand.ty)
    }

    fn is_record_or_complex(&self, ty: &Type) -> bool {
        match ty {
            Type::Complex(_) => true,
            Type::Defined(id) => match &self.types.definitions[id.0 as usize].kind {
                TypeDefinitionKind::Alias(inner) => self.is_record_or_complex(inner),
                TypeDefinitionKind::Record { .. } => true,
                TypeDefinitionKind::Enum { .. } => false,
            },
            _ => false,
        }
    }

    fn abi_pass(
        &self,
        operand: &AbiOperand,
        result: bool,
        convention: AbiConvention,
    ) -> Result<AbiPass, ResolveError> {
        if self.atomic_is_memory(operand, convention) {
            let align = self.layout(operand)?.alignment_bytes;
            return Ok(if result {
                AbiPass::SRet { align }
            } else if convention == AbiConvention::X86Cdecl {
                AbiPass::ByValue {
                    align: align.min(4),
                }
            } else {
                AbiPass::ByValue { align }
            });
        }
        if operand.atomic
            && self.types.flavor != CompilerFlavor::Gcc
            && matches!(convention, AbiConvention::Aapcs64 | AbiConvention::WinArm64)
            && self.is_record_or_complex(&operand.ty)
        {
            let layout = self.layout(operand)?;
            return Ok(if layout.size_bytes <= 16 {
                AbiPass::Coerce(vec![AbiChunk::Integer((layout.size_bytes * 8) as u32)])
            } else if result {
                AbiPass::SRet {
                    align: layout.alignment_bytes,
                }
            } else {
                AbiPass::ByReference {
                    align: layout.alignment_bytes,
                }
            });
        }
        let ty = &operand.ty;
        match ty {
            Type::Void => Ok(AbiPass::Void),
            Type::Numeric(NumericType::Integer { width: 128, .. })
                if convention == AbiConvention::Win64 =>
            {
                if result {
                    Ok(AbiPass::Coerce(vec![AbiChunk::Integer(64); 2]))
                } else {
                    Ok(AbiPass::ByReference { align: 16 })
                }
            }
            Type::Bool
            | Type::Numeric(_)
            | Type::Imaginary(_)
            | Type::FixedPoint(_)
            | Type::Pointer { .. }
            | Type::VaList => Ok(AbiPass::Scalar),
            Type::Complex(component) => self.complex_abi(*component, result, convention),
            Type::Vector { .. } => self.vector_abi(operand, result, convention),
            Type::Defined(id) => match &self.types.definitions[id.0 as usize].kind {
                TypeDefinitionKind::Alias(inner) => self.abi_pass(
                    &AbiOperand {
                        ty: inner.clone(),
                        atomic: operand.atomic,
                    },
                    result,
                    convention,
                ),
                TypeDefinitionKind::Enum { .. } => Ok(AbiPass::Scalar),
                TypeDefinitionKind::Record {
                    kind,
                    fields: Some(fields),
                    layout: Some(record_layout),
                    ..
                } => {
                    let layout = self.layout(operand)?;
                    let homogeneous = if operand.atomic && self.types.flavor != CompilerFlavor::Gcc
                    {
                        None
                    } else {
                        self.homogeneous_record(*kind, fields)
                    };
                    Ok(record_abi(
                        layout.size_bytes,
                        layout.alignment_bytes,
                        homogeneous,
                        sysv_record_chunks(fields, &record_layout.offsets, layout.size_bytes),
                        record_field_chunks(fields, self.target.pointer_width),
                        fields.iter().all(|field| {
                            matches!(
                                field.ty,
                                Type::Numeric(_) | Type::Bool | Type::Pointer { .. }
                            )
                        }),
                        result,
                        convention,
                    ))
                }
                _ => Err(ResolveError::Unsupported("incomplete ABI type")),
            },
            Type::Array { .. } | Type::VariableArray { .. } | Type::Function { .. } => {
                Err(ResolveError::Unsupported("unadjusted ABI parameter type"))
            }
        }
    }

    fn homogeneous_type(&self, ty: &Type) -> Option<(FloatType, usize)> {
        match ty {
            Type::Numeric(NumericType::Float(format)) => Some((*format, 1)),
            Type::Array {
                element,
                length: Some(length @ 1..=4),
            } => {
                let (format, count) = self.homogeneous_type(element)?;
                let count = count.checked_mul(usize::try_from(*length).ok()?)?;
                (count <= 4).then_some((format, count))
            }
            Type::Defined(id) => match &self.types.definitions[id.0 as usize].kind {
                TypeDefinitionKind::Alias(inner) => self.homogeneous_type(inner),
                TypeDefinitionKind::Record {
                    kind,
                    fields: Some(fields),
                    ..
                } => self.homogeneous_record(*kind, fields),
                _ => None,
            },
            _ => None,
        }
    }

    fn homogeneous_record(
        &self,
        kind: RecordKind,
        fields: &[Span<Field>],
    ) -> Option<(FloatType, usize)> {
        let mut members = fields.iter().map(|field| {
            field
                .bit_width
                .is_none()
                .then(|| self.homogeneous_type(&field.ty))
                .flatten()
        });
        let (format, mut count) = members.next().flatten()?;
        for member in members {
            let (next_format, next_count) = member?;
            if next_format != format {
                return None;
            }
            count = match kind {
                RecordKind::Struct => count.checked_add(next_count)?,
                RecordKind::Union => count.max(next_count),
            };
            if count > 4 {
                return None;
            }
        }
        Some((format, count))
    }

    fn vector_abi(
        &self,
        operand: &AbiOperand,
        result: bool,
        convention: AbiConvention,
    ) -> Result<AbiPass, ResolveError> {
        let Type::Vector { element, .. } = operand.ty else {
            return Err(ResolveError::Unsupported("vector ABI of non-vector type"));
        };
        let layout = self.layout(operand)?;
        let size = layout.size_bytes;
        let align = layout.alignment_bytes;
        let register_bytes = self.target.isa.vector_register_bytes();
        let pass = match convention {
            AbiConvention::Win64 => AbiPass::Direct,
            AbiConvention::SysV64 if size < 8 => {
                AbiPass::Coerce(vec![AbiChunk::Integer((size * 8) as u32)])
            }
            // gcc passes a one-lane double vector in memory, and clang follows it
            AbiConvention::SysV64 if size == 8 && element == NumericType::Float(FloatType::F64) => {
                if result {
                    AbiPass::Direct
                } else {
                    AbiPass::ByValue { align }
                }
            }
            AbiConvention::SysV64 if size == 8 => {
                AbiPass::Coerce(vec![AbiChunk::Float(FloatType::F64)])
            }
            AbiConvention::SysV64 if size > register_bytes && !result => AbiPass::ByValue { align },
            AbiConvention::SysV64 => AbiPass::Direct,
            // an eight-byte vector of narrow integer lanes is an MMX type, which
            // i386 passes as an integer to keep MMX registers out of the ABI
            AbiConvention::X86Cdecl
                if size == 8
                    && !result
                    && matches!(element, NumericType::Integer { width, .. } if width < 64) =>
            {
                AbiPass::Coerce(vec![AbiChunk::Integer(64)])
            }
            AbiConvention::X86Cdecl => AbiPass::Direct,
            AbiConvention::Aapcs64 | AbiConvention::WinArm64 => match size {
                _ if size < 8 && !result => AbiPass::Coerce(vec![AbiChunk::Integer(32)]),
                _ if size <= 16 => AbiPass::Direct,
                _ if result => AbiPass::SRet {
                    align: align.min(16),
                },
                _ => AbiPass::ByReference {
                    align: align.min(16),
                },
            },
            AbiConvention::Aapcs32 | AbiConvention::Aapcs32HardFloat => match size {
                _ if size < 8 && !result => AbiPass::Coerce(vec![AbiChunk::Integer(32)]),
                _ if size > 16 && result => AbiPass::SRet {
                    align: align.min(8),
                },
                _ => AbiPass::Direct,
            },
        };
        Ok(pass)
    }

    fn complex_abi(
        &self,
        component: NumericType,
        result: bool,
        convention: AbiConvention,
    ) -> Result<AbiPass, ResolveError> {
        let layout = self.target.storage_of(Type::Complex(component))?;
        let align = layout.alignment_bytes;
        let size = layout.size_bytes;
        let pass = match (convention, component) {
            (AbiConvention::SysV64, NumericType::Float(FloatType::F32)) => {
                AbiPass::Coerce(vec![AbiChunk::FloatPair(FloatType::F32)])
            }
            (AbiConvention::SysV64, NumericType::Float(FloatType::F80)) => {
                if result {
                    AbiPass::Coerce(vec![AbiChunk::Float(FloatType::F80); 2])
                } else {
                    AbiPass::ByValue { align }
                }
            }
            (AbiConvention::SysV64, NumericType::Float(format)) => {
                AbiPass::Coerce(vec![AbiChunk::Float(format); 2])
            }
            (AbiConvention::Win64, _) if size <= 8 => {
                AbiPass::Coerce(vec![AbiChunk::Integer((size * 8) as u32)])
            }
            (AbiConvention::Win64, _) => {
                if result {
                    AbiPass::SRet { align }
                } else {
                    AbiPass::ByReference { align }
                }
            }
            (AbiConvention::X86Cdecl, NumericType::Float(FloatType::F32)) if result => {
                AbiPass::Coerce(vec![AbiChunk::Integer(64)])
            }
            (AbiConvention::X86Cdecl, _) if result => AbiPass::SRet {
                align: align.min(4),
            },
            (AbiConvention::X86Cdecl, _) => AbiPass::ByValue {
                align: align.min(4),
            },
            (AbiConvention::Aapcs64 | AbiConvention::WinArm64, NumericType::Float(format)) => {
                AbiPass::Coerce(vec![AbiChunk::Float(format); 2])
            }
            (AbiConvention::Aapcs32HardFloat, NumericType::Float(format)) => {
                AbiPass::Coerce(vec![AbiChunk::Float(format); 2])
            }
            (AbiConvention::Aapcs32 | AbiConvention::Aapcs32HardFloat, _) if result => {
                AbiPass::SRet { align }
            }
            (AbiConvention::Aapcs32 | AbiConvention::Aapcs32HardFloat, _) => {
                integer_chunks(size, if align >= 8 { 64 } else { 32 })
            }
            _ if result && size > 16 => AbiPass::SRet { align },
            _ if size > 16 => AbiPass::ByReference { align },
            _ => integer_chunks(size, 64),
        };
        Ok(pass)
    }
}

#[expect(
    clippy::too_many_arguments,
    reason = "Abi needs to take lots of params"
)]
fn record_abi(
    size: u64,
    align: u32,
    homogeneous: Option<(FloatType, usize)>,
    sysv_chunks: Option<Vec<AbiChunk>>,
    field_chunks: Option<Vec<AbiChunk>>,
    flat: bool,
    result: bool,
    convention: AbiConvention,
) -> AbiPass {
    match convention {
        // win64 classifies on size alone, so it needs no field walk to be sure
        AbiConvention::Win64 => {
            if matches!(size, 1 | 2 | 4 | 8) {
                AbiPass::Coerce(vec![AbiChunk::Integer((size * 8) as u32)])
            } else if result {
                AbiPass::SRet { align }
            } else {
                AbiPass::ByReference { align }
            }
        }
        _ if !flat && !matches!(convention, AbiConvention::Aapcs64 | AbiConvention::WinArm64) => {
            AbiPass::NativeC
        }
        AbiConvention::SysV64 => {
            if size > 16 {
                if result {
                    AbiPass::SRet { align }
                } else {
                    AbiPass::ByValue { align }
                }
            } else if align > 8 {
                AbiPass::NativeC
            } else if let Some(chunks) = sysv_chunks {
                AbiPass::Coerce(chunks)
            } else {
                AbiPass::NativeC
            }
        }
        AbiConvention::X86Cdecl => {
            if result {
                AbiPass::SRet {
                    align: align.min(4),
                }
            } else if let Some(chunks) = field_chunks {
                AbiPass::Coerce(chunks)
            } else {
                AbiPass::NativeC
            }
        }
        AbiConvention::Aapcs64 | AbiConvention::WinArm64 => {
            if let Some((format, count)) = homogeneous {
                AbiPass::Coerce(vec![AbiChunk::Float(format); count])
            } else if size <= 16 {
                integer_chunks(size, 64)
            } else if result {
                AbiPass::SRet { align }
            } else {
                AbiPass::ByReference { align }
            }
        }
        AbiConvention::Aapcs32HardFloat if homogeneous.is_some() => {
            let (format, count) = homogeneous.unwrap();
            AbiPass::Coerce(vec![AbiChunk::Float(format); count])
        }
        AbiConvention::Aapcs32 | AbiConvention::Aapcs32HardFloat => {
            if result {
                AbiPass::SRet { align }
            } else {
                integer_chunks(size, if align >= 8 { 64 } else { 32 })
            }
        }
    }
}

fn record_field_chunks(fields: &[Span<Field>], pointer_width: u32) -> Option<Vec<AbiChunk>> {
    fields
        .iter()
        .map(|field| {
            if field.bit_width.is_some() {
                return None;
            }
            match &field.ty {
                Type::Numeric(NumericType::Float(format)) => Some(AbiChunk::Float(*format)),
                Type::Numeric(NumericType::Integer { width, .. }) => {
                    Some(AbiChunk::Integer(*width))
                }
                Type::Bool => Some(AbiChunk::Integer(8)),
                Type::Pointer { .. } => Some(AbiChunk::Integer(pointer_width)),
                _ => None,
            }
        })
        .collect()
}

fn sysv_record_chunks(fields: &[Span<Field>], offsets: &[u64], size: u64) -> Option<Vec<AbiChunk>> {
    if size == 0 || size > 16 || fields.len() != offsets.len() {
        return None;
    }
    let mut slots = vec![Vec::new(); size.div_ceil(8) as usize];
    let mut integers = vec![0u32; slots.len()];
    for (field, offset) in fields.iter().zip(offsets) {
        let slot = usize::try_from(offset / 8).ok()?;
        if slot >= slots.len() {
            return None;
        }
        match &field.ty {
            Type::Numeric(NumericType::Float(format @ (FloatType::F32 | FloatType::F64)))
                if field.bit_width.is_none() =>
            {
                let bytes: u64 = if *format == FloatType::F32 { 4 } else { 8 };
                if offset % bytes != 0 || offset % 8 + bytes > 8 {
                    return None;
                }
                slots[slot].push(*format);
            }
            Type::Numeric(NumericType::Integer { width, .. }) => {
                let bytes = u64::from(width.div_ceil(8));
                if offset % bytes != 0 || offset % 8 + bytes > 8 {
                    return None;
                }
                integers[slot] = integers[slot].max((offset % 8 * 8) as u32 + *width);
            }
            Type::Bool => integers[slot] = integers[slot].max((offset % 8 * 8) as u32 + 8),
            Type::Pointer { .. } => {
                if offset % 8 != 0 {
                    return None;
                }
                integers[slot] = 64;
            }
            _ => return None,
        }
    }
    let mut chunks = Vec::with_capacity(slots.len());
    for (index, formats) in slots.into_iter().enumerate() {
        if integers[index] != 0 || formats.is_empty() {
            let remaining = size - index as u64 * 8;
            let float_bits = formats
                .iter()
                .map(|format| if *format == FloatType::F32 { 32 } else { 64 })
                .sum::<u32>();
            let bits = integers[index]
                .max(float_bits)
                .max(8)
                .min((remaining.min(8) * 8) as u32);
            chunks.push(AbiChunk::Integer(bits.next_multiple_of(8)));
        } else if formats.len() == 2 && formats.iter().all(|kind| *kind == FloatType::F32) {
            chunks.push(AbiChunk::FloatPair(FloatType::F32));
        } else if formats.len() == 1 {
            chunks.push(AbiChunk::Float(formats[0]));
        } else {
            return None;
        }
    }
    Some(chunks)
}

fn integer_chunks(size: u64, width: u32) -> AbiPass {
    let bytes = u64::from(width / 8);
    let count = size.div_ceil(bytes);
    AbiPass::Coerce(vec![AbiChunk::Integer(width); count as usize])
}
