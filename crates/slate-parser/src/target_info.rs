use crate::ir::{FloatType, NumericType, ShiftFill, Type};
use crate::target::isa::TargetIsa;
use crate::target::x86_isa::X86Feature;
use crate::target_registry::TargetProfile;
use std::collections::BTreeMap;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct TargetInfo {
    pub triple: String,
    pub endian: Endian,
    pub long_double: LongDoubleFormat,
    pub char_signed: bool,
    pub signed_right_shift: ShiftFill,
    pub short_width: u32,
    pub int_width: u32,
    pub long_width: u32,
    pub long_long_width: u32,
    pub pointer_width: u32,
    pub wchar_signed: bool,
    pub wchar_width: u32,
    pub scalars: ScalarLayouts,
    pub pointer: StorageLayout,
    pub abi: TargetAbi,
    pub family: TargetFamily,
    pub os: TargetOs,
    pub environment: TargetEnvironment,
    pub isa: TargetIsa,
    pub profile: TargetProfile,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TargetOs {
    Linux,
    Windows,
    Darwin,
    Android,
    FreeBsd,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TargetEnvironment {
    Gnu,
    GnuEabi,
    GnuEabiHf,
    Msvc,
    Darwin,
    Android,
    FreeBsd,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum VaListKind {
    CharPointer,
    X86_64Sysv,
    AArch64Aapcs,
    ArmAapcs,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TargetFamily {
    X86_64,
    X86,
    AArch64,
    Arm32,
}

#[derive(Debug, thiserror::Error)]
pub enum TargetError {
    #[error("unsupported target triple: {0}")]
    UnsupportedTriple(String),
    #[error("the {flavor:?} flavor is not supported on {triple}")]
    UnsupportedFlavor {
        triple: String,
        flavor: crate::compiler_args::CompilerFlavor,
    },
}

impl miette::Diagnostic for TargetError {}

impl Default for TargetInfo {
    fn default() -> Self {
        Self {
            triple: "x86_64-unknown-linux-gnu".into(),
            endian: Endian::Little,
            long_double: LongDoubleFormat::X87,
            char_signed: true,
            signed_right_shift: ShiftFill::SignExtend,
            short_width: 16,
            int_width: 32,
            long_width: 64,
            long_long_width: 64,
            pointer_width: 64,
            wchar_signed: true,
            wchar_width: 32,
            scalars: ScalarLayouts::x86_64_linux(),
            pointer: StorageLayout {
                size_bytes: 8,
                alignment_bytes: 8,
            },
            abi: TargetAbi::default(),
            family: TargetFamily::X86_64,
            os: TargetOs::Linux,
            environment: TargetEnvironment::Gnu,
            isa: TargetIsa::baseline(TargetFamily::X86_64, TargetEnvironment::Gnu),
            profile: crate::target_registry::X86_64_LINUX_GNU_PROFILE,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Endian {
    Little,
    Big,
}

impl Endian {
    pub fn as_str(self) -> &'static str {
        match self {
            Self::Little => "little",
            Self::Big => "big",
        }
    }
}

const MSVC_ATOMIC_LOCK_BYTES: u64 = 4;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct StorageLayout {
    pub size_bytes: u64,
    pub alignment_bytes: u32,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct TargetAbi {
    pub preferred_stack_alignment: u32,
    pub zero_width_bitfield_aligns_record: bool,
}

impl Default for TargetAbi {
    fn default() -> Self {
        Self {
            preferred_stack_alignment: 16,
            zero_width_bitfield_aligns_record: false,
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ScalarLayouts {
    entries: BTreeMap<ScalarKey, StorageLayout>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
enum ScalarKey {
    Bool,
    Integer { width: u32, signed: bool },
    Float(FloatType),
}

impl ScalarLayouts {
    fn x86_64_linux() -> Self {
        let mut entries = BTreeMap::new();
        entries.insert(
            ScalarKey::Bool,
            StorageLayout {
                size_bytes: 1,
                alignment_bytes: 1,
            },
        );
        for (width, size, alignment) in
            [(8, 1, 1), (16, 2, 2), (32, 4, 4), (64, 8, 8), (128, 16, 16)]
        {
            for signed in [false, true] {
                entries.insert(
                    ScalarKey::Integer { width, signed },
                    StorageLayout {
                        size_bytes: size,
                        alignment_bytes: alignment,
                    },
                );
            }
        }
        for (format, size, alignment) in [
            (FloatType::BF16, 2, 2),
            (FloatType::F16, 2, 2),
            (FloatType::F32, 4, 4),
            (FloatType::F64, 8, 8),
            (FloatType::F80, 16, 16),
            (FloatType::F128, 16, 16),
            (FloatType::D32, 4, 4),
            (FloatType::D64, 8, 8),
            (FloatType::D128, 16, 16),
        ] {
            entries.insert(
                ScalarKey::Float(format),
                StorageLayout {
                    size_bytes: size,
                    alignment_bytes: alignment,
                },
            );
        }
        Self { entries }
    }

    fn for_family(family: TargetFamily) -> Self {
        let mut layouts = Self::x86_64_linux();
        match family {
            TargetFamily::X86_64 => {}
            TargetFamily::X86 => {
                for signed in [false, true] {
                    layouts.set(ScalarKey::Integer { width: 64, signed }, 8, 4);
                }
                layouts.set(ScalarKey::Float(FloatType::F64), 8, 4);
                layouts.set(ScalarKey::Float(FloatType::F80), 12, 4);
            }
            TargetFamily::AArch64 => {
                layouts.entries.remove(&ScalarKey::Float(FloatType::F80));
            }
            TargetFamily::Arm32 => {
                layouts.entries.remove(&ScalarKey::Float(FloatType::F80));
                for signed in [false, true] {
                    layouts
                        .entries
                        .remove(&ScalarKey::Integer { width: 128, signed });
                }
                layouts.set(ScalarKey::Float(FloatType::F128), 16, 16);
            }
        }
        layouts
    }

    fn set(&mut self, key: ScalarKey, size_bytes: u64, alignment_bytes: u32) {
        self.entries.insert(
            key,
            StorageLayout {
                size_bytes,
                alignment_bytes,
            },
        );
    }

    fn get(&self, key: ScalarKey) -> Option<StorageLayout> {
        self.entries.get(&key).copied()
    }

    // bit-precise integers take the smallest standard layout that holds them
    fn bit_precise(&self, width: u32) -> Option<StorageLayout> {
        let integer = |width| {
            self.get(ScalarKey::Integer {
                width,
                signed: false,
            })
        };
        if let Some(layout) = [8u32, 16, 32, 64]
            .into_iter()
            .filter(|candidate| width <= *candidate)
            .find_map(integer)
        {
            return Some(layout);
        }
        let widest = integer(64)?;
        let alignment = u64::from(widest.alignment_bytes);
        Some(StorageLayout {
            size_bytes: u64::from(width).div_ceil(8).div_ceil(alignment) * alignment,
            alignment_bytes: widest.alignment_bytes,
        })
    }
}

#[derive(Debug, thiserror::Error)]
pub enum LayoutError {
    #[error("unsupported scalar storage layout for {0}")]
    UnsupportedScalar(Type),
}

impl TargetInfo {
    // msvc has no __float128
    pub fn has_float128(&self) -> bool {
        self.long_double == LongDoubleFormat::Binary128
            || (matches!(self.family, TargetFamily::X86_64 | TargetFamily::X86)
                && self.environment != TargetEnvironment::Msvc)
    }

    // gcc's _Float64x: the narrowest format wider than double
    pub fn float64x_format(&self) -> Option<FloatType> {
        match self.long_double {
            LongDoubleFormat::X87 => Some(FloatType::F80),
            LongDoubleFormat::Binary128 => Some(FloatType::F128),
            LongDoubleFormat::Binary64 => self.has_float128().then_some(FloatType::F128),
        }
    }

    pub fn for_triple(triple: &str) -> Result<Self, TargetError> {
        let spec = crate::target_registry::lookup(triple)?;
        Ok(Self {
            triple: spec.triple.into(),
            profile: spec.profile,
            ..(spec.layout)()
        })
    }

    pub fn for_triple_and_flavor(
        triple: &str,
        flavor: crate::compiler_args::CompilerFlavor,
    ) -> Result<Self, TargetError> {
        let target = Self::for_triple(triple)?;
        match target.profile.predefines(flavor) {
            Some(_) => Ok(target),
            None => Err(TargetError::UnsupportedFlavor {
                triple: target.triple,
                flavor,
            }),
        }
    }

    pub(crate) fn x86_64_windows_msvc() -> Self {
        Self::windows_msvc(TargetFamily::X86_64)
    }

    pub(crate) fn aarch64_windows_msvc() -> Self {
        Self::windows_msvc(TargetFamily::AArch64)
    }

    pub(crate) fn i686_windows_msvc() -> Self {
        let mut target = Self::windows_msvc(TargetFamily::X86);
        for signed in [false, true] {
            target
                .scalars
                .set(ScalarKey::Integer { width: 64, signed }, 8, 8);
        }
        target.scalars.set(ScalarKey::Float(FloatType::F64), 8, 8);
        Self {
            pointer_width: 32,
            pointer: StorageLayout {
                size_bytes: 4,
                alignment_bytes: 4,
            },
            abi: TargetAbi {
                preferred_stack_alignment: 4,
                ..TargetAbi::default()
            },
            ..target
        }
    }

    pub(crate) fn thumbv7a_windows_msvc() -> Self {
        Self {
            pointer_width: 32,
            pointer: StorageLayout {
                size_bytes: 4,
                alignment_bytes: 4,
            },
            abi: TargetAbi {
                preferred_stack_alignment: 8,
                ..TargetAbi::default()
            },
            ..Self::windows_msvc(TargetFamily::Arm32)
        }
    }

    fn windows_msvc(family: TargetFamily) -> Self {
        let mut scalars = ScalarLayouts::for_family(family);
        scalars.entries.remove(&ScalarKey::Float(FloatType::F80));
        Self {
            os: TargetOs::Windows,
            environment: TargetEnvironment::Msvc,
            family,
            isa: TargetIsa::baseline(family, TargetEnvironment::Msvc),
            long_width: 32,
            long_double: LongDoubleFormat::Binary64,
            wchar_signed: false,
            wchar_width: 16,
            scalars,
            ..Self::default()
        }
    }

    pub(crate) fn x86_linux() -> Self {
        Self {
            endian: Endian::Little,
            long_double: LongDoubleFormat::X87,
            char_signed: true,
            signed_right_shift: ShiftFill::SignExtend,
            short_width: 16,
            int_width: 32,
            long_width: 32,
            long_long_width: 64,
            pointer_width: 32,
            wchar_signed: true,
            wchar_width: 32,
            scalars: ScalarLayouts::for_family(TargetFamily::X86),
            pointer: StorageLayout {
                size_bytes: 4,
                alignment_bytes: 4,
            },
            abi: TargetAbi {
                preferred_stack_alignment: 16,
                zero_width_bitfield_aligns_record: false,
            },
            family: TargetFamily::X86,
            os: TargetOs::Linux,
            environment: TargetEnvironment::Gnu,
            isa: TargetIsa::baseline(TargetFamily::X86, TargetEnvironment::Gnu),
            ..Self::default()
        }
    }

    pub(crate) fn aarch64_linux() -> Self {
        Self {
            endian: Endian::Little,
            long_double: LongDoubleFormat::Binary128,
            char_signed: false,
            signed_right_shift: ShiftFill::SignExtend,
            short_width: 16,
            int_width: 32,
            long_width: 64,
            long_long_width: 64,
            pointer_width: 64,
            wchar_signed: false,
            wchar_width: 32,
            scalars: ScalarLayouts::for_family(TargetFamily::AArch64),
            pointer: StorageLayout {
                size_bytes: 8,
                alignment_bytes: 8,
            },
            abi: TargetAbi {
                preferred_stack_alignment: 16,
                zero_width_bitfield_aligns_record: true,
            },
            family: TargetFamily::AArch64,
            os: TargetOs::Linux,
            environment: TargetEnvironment::Gnu,
            isa: TargetIsa::baseline(TargetFamily::AArch64, TargetEnvironment::Gnu),
            ..Self::default()
        }
    }

    pub(crate) fn aarch64_apple_darwin() -> Self {
        Self {
            endian: Endian::Little,
            long_double: LongDoubleFormat::Binary64,
            char_signed: true,
            signed_right_shift: ShiftFill::SignExtend,
            short_width: 16,
            int_width: 32,
            long_width: 64,
            long_long_width: 64,
            pointer_width: 64,
            wchar_signed: true,
            wchar_width: 32,
            scalars: ScalarLayouts::for_family(TargetFamily::AArch64),
            pointer: StorageLayout {
                size_bytes: 8,
                alignment_bytes: 8,
            },
            abi: TargetAbi {
                preferred_stack_alignment: 16,
                zero_width_bitfield_aligns_record: true,
            },
            family: TargetFamily::AArch64,
            os: TargetOs::Darwin,
            environment: TargetEnvironment::Darwin,
            isa: TargetIsa::baseline(TargetFamily::AArch64, TargetEnvironment::Darwin),
            ..Self::default()
        }
    }

    pub(crate) fn x86_64_apple_darwin() -> Self {
        Self {
            os: TargetOs::Darwin,
            environment: TargetEnvironment::Darwin,
            long_double: LongDoubleFormat::X87,
            isa: TargetIsa::baseline(TargetFamily::X86_64, TargetEnvironment::Darwin),
            ..Self::default()
        }
    }

    pub(crate) fn aarch64_android() -> Self {
        let mut target = Self::aarch64_linux();
        target.os = TargetOs::Android;
        target.environment = TargetEnvironment::Android;
        target.isa = TargetIsa::baseline(TargetFamily::AArch64, TargetEnvironment::Android);
        target
    }

    pub(crate) fn x86_64_android() -> Self {
        Self {
            long_double: LongDoubleFormat::Binary128,
            os: TargetOs::Android,
            environment: TargetEnvironment::Android,
            isa: TargetIsa::baseline(TargetFamily::X86_64, TargetEnvironment::Android),
            ..Self::default()
        }
    }

    pub(crate) fn aarch64_freebsd() -> Self {
        let mut target = Self::aarch64_linux();
        target.os = TargetOs::FreeBsd;
        target.environment = TargetEnvironment::FreeBsd;
        target.isa = TargetIsa::baseline(TargetFamily::AArch64, TargetEnvironment::FreeBsd);
        target
    }

    pub(crate) fn x86_64_freebsd() -> Self {
        Self {
            long_double: LongDoubleFormat::X87,
            os: TargetOs::FreeBsd,
            environment: TargetEnvironment::FreeBsd,
            isa: TargetIsa::baseline(TargetFamily::X86_64, TargetEnvironment::FreeBsd),
            ..Self::default()
        }
    }

    pub(crate) fn arm32_linux_gnueabi() -> Self {
        Self::arm32_linux(TargetEnvironment::GnuEabi)
    }

    pub(crate) fn arm32_linux_gnueabihf() -> Self {
        Self::arm32_linux(TargetEnvironment::GnuEabiHf)
    }

    fn arm32_linux(environment: TargetEnvironment) -> Self {
        Self {
            endian: Endian::Little,
            long_double: LongDoubleFormat::Binary64,
            char_signed: false,
            signed_right_shift: ShiftFill::SignExtend,
            short_width: 16,
            int_width: 32,
            long_width: 32,
            long_long_width: 64,
            pointer_width: 32,
            wchar_signed: false,
            wchar_width: 32,
            scalars: ScalarLayouts::for_family(TargetFamily::Arm32),
            pointer: StorageLayout {
                size_bytes: 4,
                alignment_bytes: 4,
            },
            abi: TargetAbi {
                preferred_stack_alignment: 8,
                zero_width_bitfield_aligns_record: true,
            },
            family: TargetFamily::Arm32,
            os: TargetOs::Linux,
            environment,
            isa: TargetIsa::baseline(TargetFamily::Arm32, environment),
            ..Self::default()
        }
    }

    // clang's getMaxAtomicPromoteWidth: the widest object it will pad and
    // over-align so an atomic access can be lock-free
    pub fn max_atomic_promote_bytes(&self) -> u64 {
        match self.family {
            TargetFamily::X86_64 | TargetFamily::AArch64 => 16,
            TargetFamily::X86 | TargetFamily::Arm32 => 8,
        }
    }

    // clang's getMaxAtomicInlineWidth: the widest access done without libatomic
    pub fn max_atomic_inline_bytes(&self) -> u64 {
        match (self.family, self.isa) {
            (TargetFamily::X86_64, TargetIsa::X86(isa))
                if !isa.features.contains(X86Feature::Cx16) =>
            {
                8
            }
            (TargetFamily::X86_64 | TargetFamily::AArch64, _) => 16,
            (TargetFamily::X86 | TargetFamily::Arm32, _) => 8,
        }
    }

    pub fn atomic_storage(&self, layout: StorageLayout) -> StorageLayout {
        let size = layout.size_bytes.max(1);
        if size > self.max_atomic_promote_bytes() {
            return StorageLayout {
                size_bytes: size,
                ..layout
            };
        }
        let promoted = size.next_power_of_two();
        StorageLayout {
            size_bytes: promoted,
            alignment_bytes: u32::try_from(promoted)
                .unwrap_or(layout.alignment_bytes)
                .max(layout.alignment_bytes),
        }
    }

    pub fn msvc_atomic_storage(&self, layout: StorageLayout) -> StorageLayout {
        let size = layout.size_bytes.max(1);
        if matches!(size, 1 | 2 | 4 | 8) {
            return StorageLayout {
                size_bytes: size,
                alignment_bytes: u32::try_from(size)
                    .unwrap_or(layout.alignment_bytes)
                    .max(layout.alignment_bytes),
            };
        }
        let alignment = u64::from(layout.alignment_bytes).max(MSVC_ATOMIC_LOCK_BYTES);
        StorageLayout {
            size_bytes: alignment
                .checked_add(size)
                .and_then(|bytes| bytes.checked_next_multiple_of(alignment))
                .unwrap_or(size),
            alignment_bytes: u32::try_from(alignment).unwrap_or(layout.alignment_bytes),
        }
    }

    pub fn va_list_kind(&self) -> VaListKind {
        self.profile.va_list
    }

    fn va_list_storage(&self) -> StorageLayout {
        match self.va_list_kind() {
            VaListKind::X86_64Sysv => StorageLayout {
                size_bytes: 24,
                alignment_bytes: 8,
            },
            VaListKind::AArch64Aapcs => StorageLayout {
                size_bytes: 32,
                alignment_bytes: 8,
            },
            VaListKind::CharPointer | VaListKind::ArmAapcs => self.pointer,
        }
    }

    pub fn pointer_storage(&self, space: crate::ir::PointerSpace) -> StorageLayout {
        let width = space.width(self.pointer_width);
        if width == self.pointer_width {
            return self.pointer;
        }
        StorageLayout {
            size_bytes: (width / 8).into(),
            alignment_bytes: width / 8,
        }
    }

    pub fn storage_of(&self, ty: Type) -> Result<StorageLayout, LayoutError> {
        if let Type::Complex(component) = &ty {
            let component = self.storage_of(Type::Numeric(*component))?;
            return Ok(StorageLayout {
                size_bytes: component.size_bytes * 2,
                alignment_bytes: component.alignment_bytes,
            });
        }
        if let Type::Vector { element, lanes } = &ty {
            let element = self.storage_of(Type::Numeric(*element))?;
            let bytes = element
                .size_bytes
                .checked_mul(u64::from(*lanes))
                .ok_or(LayoutError::UnsupportedScalar(ty.clone()))?;
            let rounded = bytes.next_power_of_two();
            let alignment =
                u32::try_from(rounded).map_err(|_| LayoutError::UnsupportedScalar(ty.clone()))?;
            return Ok(StorageLayout {
                size_bytes: rounded,
                alignment_bytes: alignment,
            });
        }
        if let Type::FixedPoint(fixed) = ty {
            return self.storage_of(Type::Numeric(fixed.storage()));
        }
        let key = match ty {
            Type::Bool => ScalarKey::Bool,
            Type::Numeric(NumericType::Integer {
                width,
                signed,
                bit_precise,
            }) => {
                if bit_precise {
                    return self
                        .scalars
                        .bit_precise(width)
                        .ok_or(LayoutError::UnsupportedScalar(ty));
                }
                ScalarKey::Integer { width, signed }
            }
            Type::Numeric(NumericType::Float(format)) | Type::Imaginary(format) => {
                ScalarKey::Float(format)
            }
            Type::Pointer { space, .. } => return Ok(self.pointer_storage(space)),
            Type::VaList => return Ok(self.va_list_storage()),
            Type::Complex(_)
            | Type::Vector { .. }
            | Type::FixedPoint(_)
            | Type::Defined(_)
            | Type::Array { .. }
            | Type::VariableArray { .. }
            | Type::Function { .. }
            | Type::Void => {
                return Err(LayoutError::UnsupportedScalar(ty));
            }
        };
        self.scalars
            .get(key)
            .ok_or(LayoutError::UnsupportedScalar(ty))
    }

    pub fn with_long_double(mut self, format: LongDoubleFormat) -> Self {
        self.long_double = format;
        self
    }

    pub fn with_preferred_stack_alignment(mut self, alignment: u32) -> Self {
        self.abi.preferred_stack_alignment = alignment;
        self
    }

    // x86-64 psABI: an array object of at least this many bytes gets this alignment;
    // clang and gcc apply it on every x86-64 OS unless the declaration says `aligned`
    pub fn large_array_alignment(&self) -> Option<u64> {
        (self.family == TargetFamily::X86_64).then_some(16)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum LongDoubleFormat {
    Binary64,
    X87,
    Binary128,
}

impl std::str::FromStr for LongDoubleFormat {
    type Err = String;

    fn from_str(value: &str) -> Result<Self, Self::Err> {
        match value {
            "binary64" => Ok(Self::Binary64),
            "x87" => Ok(Self::X87),
            "binary128" => Ok(Self::Binary128),
            _ => Err(format!("unknown long double format: {value}")),
        }
    }
}

impl LongDoubleFormat {
    pub fn float_type(self) -> FloatType {
        match self {
            Self::Binary64 => FloatType::F64,
            Self::X87 => FloatType::F80,
            Self::Binary128 => FloatType::F128,
        }
    }

    pub fn predefines(self) -> Vec<String> {
        let (
            size,
            mantissa,
            decimal,
            digits,
            max_exp,
            max_10_exp,
            min_exp,
            min_10_exp,
            epsilon,
            min,
            max,
            denorm,
        ) = match self {
            Self::X87 => return Vec::new(),
            Self::Binary64 => (
                8,
                53,
                17,
                15,
                1024,
                308,
                "(-1021)",
                "(-307)",
                "2.2204460492503131e-16L",
                "2.2250738585072014e-308L",
                "1.7976931348623157e+308L",
                "4.9406564584124654e-324L",
            ),
            Self::Binary128 => (
                16,
                113,
                36,
                33,
                16384,
                4932,
                "(-16381)",
                "(-4931)",
                "1.92592994438723585305597794258492732e-34L",
                "3.36210314311209350626267781732175260e-4932L",
                "1.18973149535723176508575932662800702e+4932L",
                "6.47517511943802511092443895822764655e-4966L",
            ),
        };
        vec![
            format!("__SIZEOF_LONG_DOUBLE__={size}"),
            format!("__LDBL_MANT_DIG__={mantissa}"),
            format!("__LDBL_DECIMAL_DIG__={decimal}"),
            format!("__LDBL_DIG__={digits}"),
            format!("__LDBL_MAX_EXP__={max_exp}"),
            format!("__LDBL_MAX_10_EXP__={max_10_exp}"),
            format!("__LDBL_MIN_EXP__={min_exp}"),
            format!("__LDBL_MIN_10_EXP__={min_10_exp}"),
            format!("__LDBL_EPSILON__={epsilon}"),
            format!("__LDBL_MIN__={min}"),
            format!("__LDBL_MAX__={max}"),
            format!("__LDBL_NORM_MAX__={max}"),
            format!("__LDBL_DENORM_MIN__={denorm}"),
        ]
    }
}
