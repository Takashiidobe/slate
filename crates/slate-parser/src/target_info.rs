use crate::ir::{FloatType, NumericType, Type};
use std::collections::BTreeMap;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct TargetInfo {
    pub triple: String,
    pub endian: Endian,
    pub long_double: LongDoubleFormat,
    pub char_signed: bool,
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
}

#[derive(Debug, thiserror::Error)]
pub enum TargetError {
    #[error("unsupported target triple: {0}")]
    UnsupportedTriple(String),
}

impl miette::Diagnostic for TargetError {}

impl Default for TargetInfo {
    fn default() -> Self {
        Self {
            triple: "x86_64-unknown-linux-gnu".into(),
            endian: Endian::Little,
            long_double: LongDoubleFormat::X87,
            char_signed: true,
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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct StorageLayout {
    pub size_bytes: u64,
    pub alignment_bytes: u32,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct TargetAbi {
    pub preferred_stack_alignment: u32,
}

impl Default for TargetAbi {
    fn default() -> Self {
        Self {
            preferred_stack_alignment: 16,
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
            (FloatType::F16, 2, 2),
            (FloatType::F32, 4, 4),
            (FloatType::F64, 8, 8),
            (FloatType::F80, 16, 16),
            (FloatType::F128, 16, 16),
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

    fn get(&self, key: ScalarKey) -> Option<StorageLayout> {
        self.entries.get(&key).copied()
    }
}

#[derive(Debug, thiserror::Error)]
pub enum LayoutError {
    #[error("unsupported scalar storage layout for {0}")]
    UnsupportedScalar(Type),
}

impl TargetInfo {
    pub fn for_triple(triple: &str) -> Result<Self, TargetError> {
        if triple == "x86_64-unknown-linux-gnu" {
            Ok(Self::default())
        } else {
            Err(TargetError::UnsupportedTriple(triple.into()))
        }
    }

    pub fn storage_of(&self, ty: Type) -> Result<StorageLayout, LayoutError> {
        let key = match ty {
            Type::Bool => ScalarKey::Bool,
            Type::Numeric(NumericType::Integer { width, signed }) => {
                ScalarKey::Integer { width, signed }
            }
            Type::Numeric(NumericType::Float(format)) => ScalarKey::Float(format),
            Type::Defined(_) | Type::Void => return Err(LayoutError::UnsupportedScalar(ty)),
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
