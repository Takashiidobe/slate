use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::types::TypeResolver;
use crate::ast::Span;
use crate::compiler_args::CompilerFlavor;
use crate::ir::{
    AbiChunk, AbiConvention, AbiPass, AbiSignature, CallConv, Field, FloatType, NumericType,
    RecordKind, Type, TypeDefinitionKind, Value,
};
use crate::target_info::{TargetInfo, TargetOs};

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
            convention,
            ..
        } = signature
        else {
            return Err(ResolveError::Internal("ABI of non-function type"));
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
            *convention,
        )
    }

    #[expect(clippy::wrong_self_convention, reason = "ok for now")]
    pub(super) fn from_parts(
        &self,
        return_type: Option<&AbiOperand>,
        parameters: &[AbiOperand],
        variadic: bool,
        fixed_count: usize,
        calling: CallConv,
    ) -> Result<AbiSignature, ResolveError> {
        let convention = self.abi_convention(variadic);
        let mut free_vector_registers = WIN32_VECTOR_REGISTERS;
        let arguments = parameters
            .iter()
            .enumerate()
            .map(|(index, operand)| -> Result<AbiPass, ResolveError> {
                let pass = self.abi_pass(operand, false, convention)?;
                if convention == AbiConvention::X86Win32 {
                    self.win32_argument(
                        operand,
                        pass,
                        index < fixed_count,
                        &mut free_vector_registers,
                    )
                } else if convention == AbiConvention::WinArm64
                    && variadic
                    && index >= fixed_count
                    && (operand.atomic && matches!(pass, AbiPass::Coerce(_))
                        || self.rust_passes_in_float_registers(&operand.ty))
                {
                    let layout = self.layout(operand)?;
                    Ok(if layout.size_bytes <= 16 {
                        integer_chunks(layout.size_bytes, 64)
                    } else {
                        AbiPass::ByReference {
                            align: layout.alignment_bytes,
                        }
                    })
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
            calling,
            arguments,
            result,
        })
    }

    fn abi_convention(&self, variadic: bool) -> AbiConvention {
        match self.target.profile.convention {
            AbiConvention::Aapcs32 if !variadic && self.target.isa.arm_hard_float() => {
                AbiConvention::Aapcs32HardFloat
            }
            convention => convention,
        }
    }

    fn win32_argument(
        &self,
        operand: &AbiOperand,
        pass: AbiPass,
        fixed: bool,
        free_vector_registers: &mut usize,
    ) -> Result<AbiPass, ResolveError> {
        let by_reference = || -> Result<AbiPass, ResolveError> {
            Ok(AbiPass::ByReference {
                align: self.layout(operand)?.alignment_bytes,
            })
        };
        if pass == AbiPass::Direct && self.is_vector(&operand.ty) {
            if *free_vector_registers == 0 {
                return by_reference();
            }
            *free_vector_registers -= 1;
            return Ok(pass);
        }
        if fixed
            && pass != AbiPass::NativeC
            && !self.atomic_is_memory(operand, AbiConvention::X86Win32)
            && self
                .types
                .required_alignment(&operand.ty)
                .is_some_and(|align| align > 4)
        {
            return by_reference();
        }
        Ok(pass)
    }

    fn win32_returns_in_register(&self, ty: &Type) -> Result<bool, ResolveError> {
        let size = self.types.qualified_storage(ty.clone(), false)?.size_bytes;
        if !matches!(size, 1 | 2 | 4 | 8) {
            return Ok(false);
        }
        Ok(match ty {
            Type::Numeric(NumericType::Integer {
                bit_precise: true, ..
            }) => false,
            Type::Bool
            | Type::Numeric(_)
            | Type::Imaginary(_)
            | Type::FixedPoint(_)
            | Type::Pointer { .. }
            | Type::Complex(_) => true,
            Type::Vector { .. } => size != 8,
            Type::Array {
                element,
                length: Some(_),
            } => self.win32_returns_in_register(element)?,
            Type::Defined(id) => match &self.types.definitions[id.0 as usize].kind {
                TypeDefinitionKind::Alias(inner) => self.win32_returns_in_register(inner)?,
                TypeDefinitionKind::Enum { .. } => true,
                TypeDefinitionKind::Record {
                    fields: Some(fields),
                    ..
                } => {
                    for field in fields {
                        if matches!(field.ty, Type::Array { length: None, .. }) {
                            return Ok(false);
                        }
                        if self.is_empty_field(field) {
                            continue;
                        }
                        if !self.win32_returns_in_register(&field.ty)? {
                            return Ok(false);
                        }
                    }
                    true
                }
                TypeDefinitionKind::Record { .. } => false,
            },
            _ => false,
        })
    }

    fn is_vector(&self, ty: &Type) -> bool {
        match ty {
            Type::Vector { .. } => true,
            Type::Defined(id) => matches!(
                &self.types.definitions[id.0 as usize].kind,
                TypeDefinitionKind::Alias(inner) if self.is_vector(inner)
            ),
            _ => false,
        }
    }

    fn is_empty_field(&self, field: &Field) -> bool {
        field.name.is_none() && field.bit_width.is_some() || self.is_empty_record(&field.ty)
    }

    fn is_empty_record(&self, ty: &Type) -> bool {
        match ty {
            Type::Array {
                length: Some(0), ..
            } => true,
            Type::Array { element, .. } => self.is_empty_record(element),
            Type::Defined(id) => match &self.types.definitions[id.0 as usize].kind {
                TypeDefinitionKind::Alias(inner) => self.is_empty_record(inner),
                TypeDefinitionKind::Record {
                    fields: Some(fields),
                    ..
                } => fields.iter().all(|field| self.is_empty_field(field)),
                _ => false,
            },
            _ => false,
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
        if !operand.atomic || !matches!(self.types.compiler_flavor(), CompilerFlavor::Clang) {
            return false;
        }
        if !matches!(
            convention,
            AbiConvention::SysV64 | AbiConvention::X86Cdecl | AbiConvention::X86Win32
        ) {
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
            } else if matches!(
                convention,
                AbiConvention::X86Cdecl | AbiConvention::X86Win32
            ) {
                AbiPass::ByValue {
                    align: align.min(4),
                }
            } else {
                AbiPass::ByValue { align }
            });
        }
        if operand.atomic
            && self.types.compiler_flavor() != CompilerFlavor::Gcc
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
            Type::Bool
            | Type::Numeric(_)
            | Type::Imaginary(_)
            | Type::FixedPoint(_)
            | Type::Pointer { .. }
            | Type::VaList => Ok(AbiPass::Scalar),
            Type::Complex(component) => self.complex_pass(*component, result, convention),
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
                    layout: Some(_),
                    ..
                } => {
                    let layout = self.layout(operand)?;
                    let record = AbiRecord {
                        ty,
                        kind: *kind,
                        fields,
                        size: layout.size_bytes,
                        align: layout.alignment_bytes,
                        atomic: operand.atomic,
                    };
                    self.record_abi(&record, result, convention)
                }
                _ => Err(ResolveError::Rejected("incomplete ABI type")),
            },
            Type::Array { .. } | Type::VariableArray { .. } | Type::Function { .. } => {
                Err(ResolveError::Internal("unadjusted ABI parameter type"))
            }
        }
    }

    fn record_abi(
        &self,
        record: &AbiRecord<'_>,
        result: bool,
        convention: AbiConvention,
    ) -> Result<AbiPass, ResolveError> {
        let AbiRecord { size, align, .. } = *record;
        let flexible_in_memory = self.types.compiler_flavor() != CompilerFlavor::Gcc
            && has_flexible_array(record.fields);
        Ok(match convention {
            AbiConvention::SysV64 => self.sysv_record(record, flexible_in_memory, result)?,
            AbiConvention::Win64 if flexible_in_memory && matches!(size, 1 | 2 | 4 | 8) => {
                if result {
                    AbiPass::SRet { align }
                } else {
                    AbiPass::ByReference { align }
                }
            }
            AbiConvention::X86Cdecl if result && size == 0 => AbiPass::SRet {
                align: align.min(4),
            },
            AbiConvention::X86Win32
                if result
                    && self.types.compiler_flavor() == CompilerFlavor::Clang
                    && self.is_empty_record(record.ty) =>
            {
                AbiPass::Void
            }
            AbiConvention::X86Win32
                if result
                    && matches!(size, 1 | 2 | 4 | 8)
                    && !self.win32_returns_in_register(record.ty)? =>
            {
                AbiPass::SRet { align }
            }
            AbiConvention::Aapcs64 | AbiConvention::WinArm64 | AbiConvention::Aapcs32HardFloat => {
                let c = if record.atomic && self.types.compiler_flavor() != CompilerFlavor::Gcc {
                    None
                } else {
                    self.homogeneous_record(
                        record.kind,
                        record.fields,
                        HfaView::c_compiler(convention),
                    )
                };
                if c == self.rust_homogeneous_record(record.kind, record.fields, convention) {
                    AbiPass::NativeC
                } else {
                    aapcs_record(size, align, c, result, convention)
                }
            }
            _ => AbiPass::NativeC,
        })
    }

    fn rust_homogeneous_record(
        &self,
        kind: RecordKind,
        fields: &[Span<Field>],
        convention: AbiConvention,
    ) -> Option<(FloatType, usize)> {
        if convention == AbiConvention::Aapcs32HardFloat && self.target.os == TargetOs::Windows {
            return None;
        }
        self.homogeneous_record(kind, fields, HfaView::RUST)
    }

    fn rust_passes_in_float_registers(&self, ty: &Type) -> bool {
        match ty {
            Type::Complex(component) => is_rust_float(*component),
            Type::Defined(id) => match &self.types.definitions[id.0 as usize].kind {
                TypeDefinitionKind::Alias(inner) => self.rust_passes_in_float_registers(inner),
                TypeDefinitionKind::Record {
                    kind,
                    fields: Some(fields),
                    ..
                } => self
                    .rust_homogeneous_record(*kind, fields, AbiConvention::WinArm64)
                    .is_some(),
                _ => false,
            },
            _ => false,
        }
    }

    fn homogeneous_type(&self, ty: &Type, view: HfaView) -> Option<HfaMembers> {
        match ty {
            Type::Numeric(NumericType::Float(format)) => {
                (view.base)(*format).then_some(HfaMembers {
                    format: Some(*format),
                    count: 1,
                })
            }
            Type::Array {
                length: Some(0) | None,
                ..
            } => view.skip_zero_arrays.then_some(HfaMembers::NONE),
            Type::Array {
                element,
                length: Some(length),
            } => {
                let members = self.homogeneous_type(element, view)?;
                let count = members.count.checked_mul(usize::try_from(*length).ok()?)?;
                (count <= 4).then_some(HfaMembers { count, ..members })
            }
            Type::Defined(id) => match &self.types.definitions[id.0 as usize].kind {
                TypeDefinitionKind::Alias(inner) => self.homogeneous_type(inner, view),
                TypeDefinitionKind::Record {
                    kind,
                    fields: Some(fields),
                    ..
                } => {
                    let members = self.homogeneous_members(*kind, fields, view)?;
                    let empty = members.format.is_none()
                        && self
                            .types
                            .qualified_storage(ty.clone(), false)
                            .is_ok_and(|layout| layout.size_bytes == 0);
                    (members.format.is_some() || empty).then_some(members)
                }
                _ => None,
            },
            _ => None,
        }
    }

    fn homogeneous_members(
        &self,
        kind: RecordKind,
        fields: &[Span<Field>],
        view: HfaView,
    ) -> Option<HfaMembers> {
        let mut members = HfaMembers::NONE;
        for field in fields {
            if field.bit_width.is_some() {
                return None;
            }
            let member = self.homogeneous_type(&field.ty, view)?;
            let Some(format) = member.format else {
                continue;
            };
            if members.format.is_some_and(|existing| existing != format) {
                return None;
            }
            members.format = Some(format);
            members.count = match kind {
                RecordKind::Struct => members.count.checked_add(member.count)?,
                RecordKind::Union => members.count.max(member.count),
            };
            if members.count > 4 {
                return None;
            }
        }
        Some(members)
    }

    fn homogeneous_record(
        &self,
        kind: RecordKind,
        fields: &[Span<Field>],
        view: HfaView,
    ) -> Option<(FloatType, usize)> {
        let members = self.homogeneous_members(kind, fields, view)?;
        Some((members.format?, members.count))
    }

    fn sysv_record(
        &self,
        record: &AbiRecord<'_>,
        flexible_in_memory: bool,
        result: bool,
    ) -> Result<AbiPass, ResolveError> {
        if record.size > 16 {
            return Ok(AbiPass::NativeC);
        }
        let mut c_leaves = Vec::new();
        let mut rust_leaves = Vec::new();
        if !self.sysv_leaves(record.ty, 0, false, &mut c_leaves)?
            || !self.sysv_leaves(record.ty, 0, true, &mut rust_leaves)?
        {
            return Ok(AbiPass::NativeC);
        }
        let c = if flexible_in_memory {
            Some(SysvShape::Memory)
        } else {
            sysv_classify(&c_leaves, record.size, result)
        };
        let rust = sysv_classify(&rust_leaves, record.size, result);
        Ok(match c {
            Some(shape) if Some(&shape) != rust.as_ref() => match shape {
                SysvShape::Memory if result => AbiPass::SRet {
                    align: record.align,
                },
                SysvShape::Memory => AbiPass::ByValue {
                    align: record.align,
                },
                SysvShape::Chunks(chunks) => AbiPass::Coerce(chunks),
            },
            _ => AbiPass::NativeC,
        })
    }

    fn sysv_leaves(
        &self,
        ty: &Type,
        offset: u64,
        rust_view: bool,
        leaves: &mut Vec<SysvLeaf>,
    ) -> Result<bool, ResolveError> {
        let size = || -> Result<u64, ResolveError> {
            Ok(self.types.qualified_storage(ty.clone(), false)?.size_bytes)
        };
        match ty {
            Type::Numeric(NumericType::Float(format)) | Type::Imaginary(format) => {
                let kind = if rust_view && !is_rust_float(NumericType::Float(*format)) {
                    SysvLeafKind::Integer
                } else {
                    SysvLeafKind::Float(*format)
                };
                leaves.push(SysvLeaf {
                    offset,
                    size: size()?,
                    kind,
                });
            }
            Type::Bool | Type::Numeric(_) | Type::FixedPoint(_) | Type::Pointer { .. } => {
                leaves.push(SysvLeaf {
                    offset,
                    size: size()?,
                    kind: SysvLeafKind::Integer,
                });
            }
            Type::Complex(component) => {
                let part = Type::Numeric(*component);
                let part_size = self
                    .types
                    .qualified_storage(part.clone(), false)?
                    .size_bytes;
                for index in 0..2 {
                    if !self.sysv_leaves(&part, offset + index * part_size, rust_view, leaves)? {
                        return Ok(false);
                    }
                }
            }
            Type::Array {
                element,
                length: Some(length),
            } => {
                let element_size = self
                    .types
                    .qualified_storage((**element).clone(), false)?
                    .size_bytes;
                for index in 0..*length {
                    if !self.sysv_leaves(
                        element,
                        offset + index * element_size,
                        rust_view,
                        leaves,
                    )? {
                        return Ok(false);
                    }
                }
            }
            Type::Array { length: None, .. } => {}
            Type::Defined(id) => match &self.types.definitions[id.0 as usize].kind {
                TypeDefinitionKind::Alias(inner) => {
                    return self.sysv_leaves(inner, offset, rust_view, leaves);
                }
                TypeDefinitionKind::Enum { .. } => leaves.push(SysvLeaf {
                    offset,
                    size: size()?,
                    kind: SysvLeafKind::Integer,
                }),
                TypeDefinitionKind::Record {
                    fields: Some(fields),
                    layout: Some(layout),
                    ..
                } => {
                    for (index, field) in fields.iter().enumerate() {
                        match (
                            field.bit_width,
                            layout.bit_offsets.get(index).copied().flatten(),
                        ) {
                            (Some(0), _) => {}
                            (Some(width), Some(bit_offset)) => leaves.push(SysvLeaf {
                                offset: offset + bit_offset / 8,
                                size: (bit_offset % 8 + u64::from(width)).div_ceil(8),
                                kind: SysvLeafKind::BitField,
                            }),
                            _ => {
                                let Some(field_offset) = layout.offsets.get(index) else {
                                    return Ok(false);
                                };
                                if !self.sysv_leaves(
                                    &field.ty,
                                    offset + field_offset,
                                    rust_view,
                                    leaves,
                                )? {
                                    return Ok(false);
                                }
                            }
                        }
                    }
                }
                TypeDefinitionKind::Record { .. } => return Ok(false),
            },
            _ => return Ok(false),
        }
        Ok(true)
    }

    fn vector_abi(
        &self,
        operand: &AbiOperand,
        result: bool,
        convention: AbiConvention,
    ) -> Result<AbiPass, ResolveError> {
        let Type::Vector { element, .. } = operand.ty else {
            return Err(ResolveError::Internal("vector ABI of non-vector type"));
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
            // i686 passes as an integer to keep MMX registers out of the ABI
            AbiConvention::X86Cdecl
                if size == 8
                    && !result
                    && matches!(element, NumericType::Integer { width, .. } if width < 64) =>
            {
                AbiPass::Coerce(vec![AbiChunk::Integer(64)])
            }
            AbiConvention::X86Cdecl => AbiPass::Direct,
            AbiConvention::X86Win32 if size > 64 && !result => AbiPass::ByReference { align },
            AbiConvention::X86Win32 => AbiPass::Direct,
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

    fn complex_pass(
        &self,
        component: NumericType,
        result: bool,
        convention: AbiConvention,
    ) -> Result<AbiPass, ResolveError> {
        let layout = self.target.storage_of(Type::Complex(component))?;
        let align = layout.alignment_bytes;
        let size = layout.size_bytes;
        let pass = match (convention, component) {
            (AbiConvention::SysV64, NumericType::Float(FloatType::F80)) => {
                if result {
                    AbiPass::Coerce(vec![AbiChunk::Float(FloatType::F80); 2])
                } else {
                    AbiPass::ByValue { align }
                }
            }
            (
                AbiConvention::SysV64,
                NumericType::Float(format @ (FloatType::F16 | FloatType::BF16)),
            ) => AbiPass::Coerce(vec![AbiChunk::FloatPair(format)]),
            (AbiConvention::SysV64, NumericType::Float(_)) if !is_rust_float(component) => {
                if result {
                    AbiPass::SRet { align }
                } else {
                    AbiPass::ByValue { align }
                }
            }
            (AbiConvention::Aapcs64 | AbiConvention::WinArm64, NumericType::Float(format))
                if !is_rust_float(component) =>
            {
                AbiPass::Coerce(vec![AbiChunk::Float(format); 2])
            }
            (AbiConvention::X86Cdecl, _) if result && size <= 8 => {
                AbiPass::Coerce(vec![AbiChunk::Integer((size * 8) as u32)])
            }
            (AbiConvention::Aapcs32HardFloat, NumericType::Float(format))
                if is_rust_float(component) && self.target.os == TargetOs::Windows =>
            {
                AbiPass::Coerce(vec![AbiChunk::Float(format); 2])
            }
            _ => AbiPass::NativeC,
        };
        Ok(pass)
    }
}

struct AbiRecord<'a> {
    ty: &'a Type,
    kind: RecordKind,
    fields: &'a [Span<Field>],
    size: u64,
    align: u32,
    atomic: bool,
}

#[derive(Clone, Copy)]
struct HfaView {
    base: fn(FloatType) -> bool,
    skip_zero_arrays: bool,
}

impl HfaView {
    const RUST: Self = Self {
        base: |format| matches!(format, FloatType::F32 | FloatType::F64),
        skip_zero_arrays: true,
    };

    fn c_compiler(convention: AbiConvention) -> Self {
        Self {
            base: if convention == AbiConvention::Aapcs32HardFloat {
                |format| matches!(format, FloatType::F32 | FloatType::F64)
            } else {
                |format| !format.is_decimal()
            },
            skip_zero_arrays: false,
        }
    }
}

#[derive(Clone, Copy)]
struct HfaMembers {
    format: Option<FloatType>,
    count: usize,
}

impl HfaMembers {
    const NONE: Self = Self {
        format: None,
        count: 0,
    };
}

#[derive(Debug, Clone, Copy)]
struct SysvLeaf {
    offset: u64,
    size: u64,
    kind: SysvLeafKind,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum SysvLeafKind {
    Integer,
    BitField,
    Float(FloatType),
}

#[derive(Debug, PartialEq, Eq)]
enum SysvShape {
    Memory,
    Chunks(Vec<AbiChunk>),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum SysvClass {
    NoClass,
    Integer,
    Sse,
    SseUp,
    X87,
    X87Up,
    Memory,
}

impl SysvClass {
    fn merge(self, other: Self) -> Self {
        match (self, other) {
            (a, b) if a == b => a,
            (Self::NoClass, class) | (class, Self::NoClass) => class,
            (Self::Memory, _) | (_, Self::Memory) => Self::Memory,
            (Self::Integer, _) | (_, Self::Integer) => Self::Integer,
            (Self::X87 | Self::X87Up, _) | (_, Self::X87 | Self::X87Up) => Self::Memory,
            _ => Self::Sse,
        }
    }
}

fn is_rust_float(component: NumericType) -> bool {
    matches!(
        component,
        NumericType::Float(FloatType::F32 | FloatType::F64)
    )
}

fn has_flexible_array(fields: &[Span<Field>]) -> bool {
    fields
        .last()
        .is_some_and(|field| matches!(field.ty, Type::Array { length: None, .. }))
}

fn aapcs_record(
    size: u64,
    align: u32,
    homogeneous: Option<(FloatType, usize)>,
    result: bool,
    convention: AbiConvention,
) -> AbiPass {
    if let Some((format, count)) = homogeneous {
        return AbiPass::Coerce(vec![AbiChunk::Float(format); count]);
    }
    match convention {
        AbiConvention::Aapcs64 | AbiConvention::WinArm64 => {
            if size <= 16 {
                integer_chunks(size, 64)
            } else if result {
                AbiPass::SRet { align }
            } else {
                AbiPass::ByReference { align }
            }
        }
        _ if result && size <= 4 => AbiPass::Coerce(vec![AbiChunk::Integer(32)]),
        _ if result => AbiPass::SRet { align },
        _ => integer_chunks(size, if align >= 8 { 64 } else { 32 }),
    }
}

fn sysv_classify(leaves: &[SysvLeaf], size: u64, result: bool) -> Option<SysvShape> {
    let words = size.div_ceil(8) as usize;
    let mut classes = vec![SysvClass::NoClass; words];
    for leaf in leaves {
        let natural = match leaf.kind {
            SysvLeafKind::BitField => 1,
            SysvLeafKind::Float(FloatType::F80) => 16,
            _ => leaf.size.max(1),
        };
        if leaf.offset % natural != 0 {
            return Some(SysvShape::Memory);
        }
        let first = (leaf.offset / 8) as usize;
        let last = ((leaf.offset + leaf.size).div_ceil(8) as usize).max(first + 1);
        let pieces: Vec<SysvClass> = match leaf.kind {
            SysvLeafKind::Integer | SysvLeafKind::BitField => {
                vec![SysvClass::Integer; last - first]
            }
            SysvLeafKind::Float(FloatType::F80) => vec![SysvClass::X87, SysvClass::X87Up],
            SysvLeafKind::Float(_) if leaf.size == 16 => vec![SysvClass::Sse, SysvClass::SseUp],
            SysvLeafKind::Float(_) => vec![SysvClass::Sse],
        };
        for (index, class) in pieces.into_iter().enumerate() {
            let slot = classes.get_mut(first + index)?;
            *slot = slot.merge(class);
        }
    }
    for index in 0..words {
        let previous = index.checked_sub(1).map(|previous| classes[previous]);
        match classes[index] {
            SysvClass::Memory => return Some(SysvShape::Memory),
            SysvClass::X87 if !result => return Some(SysvShape::Memory),
            SysvClass::X87Up if previous != Some(SysvClass::X87) => {
                return Some(SysvShape::Memory);
            }
            SysvClass::SseUp if previous != Some(SysvClass::Sse) => {
                classes[index] = SysvClass::Sse;
            }
            _ => {}
        }
    }
    let mut chunks = Vec::with_capacity(words);
    for (index, class) in classes.iter().enumerate() {
        let start = index as u64 * 8;
        let in_word: Vec<&SysvLeaf> = leaves
            .iter()
            .filter(|leaf| leaf.offset < start + 8 && leaf.offset + leaf.size > start)
            .collect();
        match class {
            SysvClass::Integer => {
                chunks.push(AbiChunk::Integer(sysv_integer_bits(&in_word, start, size)))
            }
            SysvClass::Sse => chunks.push(sysv_sse_chunk(&in_word)?),
            SysvClass::X87 => chunks.push(AbiChunk::Float(FloatType::F80)),
            SysvClass::NoClass | SysvClass::SseUp | SysvClass::X87Up | SysvClass::Memory => {}
        }
    }
    Some(SysvShape::Chunks(chunks))
}

// clang narrows an eightbyte to a lone leading integer when nothing follows it
fn sysv_integer_bits(in_word: &[&SysvLeaf], start: u64, size: u64) -> u32 {
    let end = in_word
        .iter()
        .map(|leaf| leaf.offset + leaf.size)
        .max()
        .unwrap_or(start);
    let leading = in_word.iter().find(|leaf| {
        leaf.offset == start && leaf.kind == SysvLeafKind::Integer && leaf.offset + leaf.size == end
    });
    match leading {
        Some(leaf) if matches!(leaf.size, 1 | 2 | 4) => (leaf.size * 8) as u32,
        _ => ((size - start).min(8) * 8) as u32,
    }
}

fn sysv_sse_chunk(in_word: &[&SysvLeaf]) -> Option<AbiChunk> {
    let mut floats: Vec<(u64, u64, FloatType)> = Vec::new();
    for leaf in in_word {
        let SysvLeafKind::Float(format) = leaf.kind else {
            return None;
        };
        match floats
            .iter_mut()
            .find(|(offset, ..)| *offset == leaf.offset)
        {
            Some(existing) if existing.1 < leaf.size => {
                *existing = (leaf.offset, leaf.size, format)
            }
            Some(_) => {}
            None => floats.push((leaf.offset, leaf.size, format)),
        }
    }
    let half = floats
        .iter()
        .map(|(.., format)| *format)
        .find(|format| matches!(format, FloatType::F16 | FloatType::BF16));
    match (floats.as_slice(), half) {
        ([(.., format)], _) => Some(AbiChunk::Float(*format)),
        ([(.., FloatType::F32), (.., FloatType::F32)], None) => {
            Some(AbiChunk::FloatPair(FloatType::F32))
        }
        ([(_, 2, first), (_, 2, second)], Some(format)) if first == second => {
            Some(AbiChunk::FloatPair(format))
        }
        (_, Some(format)) => Some(AbiChunk::FloatQuad(format)),
        _ => None,
    }
}

const WIN32_VECTOR_REGISTERS: usize = 3;

fn integer_chunks(size: u64, width: u32) -> AbiPass {
    let bytes = u64::from(width / 8);
    let count = size.div_ceil(bytes);
    AbiPass::Coerce(vec![AbiChunk::Integer(width); count as usize])
}
