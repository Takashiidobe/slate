#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct TargetInfo {
    pub long_double: LongDoubleFormat,
    pub char_signed: bool,
    pub short_width: u32,
    pub int_width: u32,
    pub long_width: u32,
    pub long_long_width: u32,
    pub pointer_width: u32,
    pub wchar_signed: bool,
    pub wchar_width: u32,
}

impl Default for TargetInfo {
    fn default() -> Self {
        Self {
            long_double: LongDoubleFormat::X87,
            char_signed: true,
            short_width: 16,
            int_width: 32,
            long_width: 64,
            long_long_width: 64,
            pointer_width: 64,
            wchar_signed: true,
            wchar_width: 32,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum LongDoubleFormat {
    Binary64,
    X87,
    Binary128,
}

impl LongDoubleFormat {
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
