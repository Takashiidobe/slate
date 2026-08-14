pub struct MacroDefinition {
    pub name: &'static str,
    pub header: &'static str,
    pub value: MacroValue,
}

pub enum MacroValue {
    Integer {
        source: i128,
        rust_path: &'static str,
    },
    LongDouble {
        source: &'static str,
        f64_source: &'static str,
        f128_source: &'static str,
        rust_bits: u64,
        f80_bytes: [u8; 10],
    },
    Float {
        source: &'static str,
        rust_bits: u32,
    },
    Double {
        source: &'static str,
        rust_bits: u64,
    },
}

pub const MACROS: &[MacroDefinition] = &[
    MacroDefinition {
        name: "SCHAR_MIN",
        header: "limits.h",
        value: MacroValue::Integer {
            source: i8::MIN as i128,
            rust_path: "i8::MIN",
        },
    },
    MacroDefinition {
        name: "SCHAR_MAX",
        header: "limits.h",
        value: MacroValue::Integer {
            source: i8::MAX as i128,
            rust_path: "i8::MAX",
        },
    },
    MacroDefinition {
        name: "UCHAR_MAX",
        header: "limits.h",
        value: MacroValue::Integer {
            source: u8::MAX as i128,
            rust_path: "u8::MAX",
        },
    },
    MacroDefinition {
        name: "CHAR_MIN",
        header: "limits.h",
        value: MacroValue::Integer {
            source: i8::MIN as i128,
            rust_path: "i8::MIN",
        },
    },
    MacroDefinition {
        name: "CHAR_MAX",
        header: "limits.h",
        value: MacroValue::Integer {
            source: i8::MAX as i128,
            rust_path: "i8::MAX",
        },
    },
    MacroDefinition {
        name: "SHRT_MIN",
        header: "limits.h",
        value: MacroValue::Integer {
            source: i16::MIN as i128,
            rust_path: "i16::MIN",
        },
    },
    MacroDefinition {
        name: "SHRT_MAX",
        header: "limits.h",
        value: MacroValue::Integer {
            source: i16::MAX as i128,
            rust_path: "i16::MAX",
        },
    },
    MacroDefinition {
        name: "USHRT_MAX",
        header: "limits.h",
        value: MacroValue::Integer {
            source: u16::MAX as i128,
            rust_path: "u16::MAX",
        },
    },
    MacroDefinition {
        name: "INT_MIN",
        header: "limits.h",
        value: MacroValue::Integer {
            source: i32::MIN as i128,
            rust_path: "i32::MIN",
        },
    },
    MacroDefinition {
        name: "INT_MAX",
        header: "limits.h",
        value: MacroValue::Integer {
            source: i32::MAX as i128,
            rust_path: "i32::MAX",
        },
    },
    MacroDefinition {
        name: "UINT_MAX",
        header: "limits.h",
        value: MacroValue::Integer {
            source: u32::MAX as i128,
            rust_path: "u32::MAX",
        },
    },
    MacroDefinition {
        name: "LONG_MIN",
        header: "limits.h",
        value: MacroValue::Integer {
            source: i64::MIN as i128,
            rust_path: "i64::MIN",
        },
    },
    MacroDefinition {
        name: "LONG_MAX",
        header: "limits.h",
        value: MacroValue::Integer {
            source: i64::MAX as i128,
            rust_path: "i64::MAX",
        },
    },
    MacroDefinition {
        name: "ULONG_MAX",
        header: "limits.h",
        value: MacroValue::Integer {
            source: u64::MAX as i128,
            rust_path: "u64::MAX",
        },
    },
    MacroDefinition {
        name: "LLONG_MIN",
        header: "limits.h",
        value: MacroValue::Integer {
            source: i64::MIN as i128,
            rust_path: "i64::MIN",
        },
    },
    MacroDefinition {
        name: "LLONG_MAX",
        header: "limits.h",
        value: MacroValue::Integer {
            source: i64::MAX as i128,
            rust_path: "i64::MAX",
        },
    },
    MacroDefinition {
        name: "ULLONG_MAX",
        header: "limits.h",
        value: MacroValue::Integer {
            source: u64::MAX as i128,
            rust_path: "u64::MAX",
        },
    },
    MacroDefinition {
        name: "SIZE_MAX",
        header: "limits.h",
        value: MacroValue::Integer {
            source: u64::MAX as i128,
            rust_path: "usize::MAX",
        },
    },
    MacroDefinition {
        name: "CHAR_BIT",
        header: "limits.h",
        value: MacroValue::Integer {
            source: u8::BITS as i128,
            rust_path: "u8::BITS",
        },
    },
    MacroDefinition {
        name: "LDBL_TRUE_MIN",
        header: "float.h",
        value: MacroValue::LongDouble {
            source: "3.64519953188247460253E-4951",
            f64_source: "4.9406564584124654E-324",
            f128_source: "6.47517511943802511092443895822764655E-4966",
            rust_bits: 1,
            f80_bytes: [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        },
    },
    MacroDefinition {
        name: "LDBL_MIN",
        header: "float.h",
        value: MacroValue::LongDouble {
            source: "3.3621031431120935063E-4932",
            f64_source: "2.2250738585072014E-308",
            f128_source: "3.36210314311209350626267781732175260E-4932",
            rust_bits: 0x0010000000000000,
            f80_bytes: [0, 0, 0, 0, 0, 0, 0, 0x80, 0x01, 0x00],
        },
    },
    MacroDefinition {
        name: "LDBL_MAX",
        header: "float.h",
        value: MacroValue::LongDouble {
            source: "1.1897314953572317650E+4932",
            f64_source: "1.7976931348623157E+308",
            f128_source: "1.18973149535723176508575932662800702E+4932",
            rust_bits: 0x7fefffffffffffff,
            f80_bytes: [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f],
        },
    },
    MacroDefinition {
        name: "LDBL_EPSILON",
        header: "float.h",
        value: MacroValue::LongDouble {
            source: "1.0842021724855044340E-19",
            f64_source: "2.2204460492503131E-16",
            f128_source: "1.92592994438723585305597794258492732E-34",
            rust_bits: 0x3cb0000000000000,
            f80_bytes: [0, 0, 0, 0, 0, 0, 0, 0x80, 0xc0, 0x3f],
        },
    },
    MacroDefinition {
        name: "LDBL_MANT_DIG",
        header: "float.h",
        value: MacroValue::Integer {
            source: 64,
            rust_path: "64",
        },
    },
    MacroDefinition {
        name: "LDBL_MIN_EXP",
        header: "float.h",
        value: MacroValue::Integer {
            source: -16381,
            rust_path: "-16381",
        },
    },
    MacroDefinition {
        name: "LDBL_MAX_EXP",
        header: "float.h",
        value: MacroValue::Integer {
            source: 16384,
            rust_path: "16384",
        },
    },
    MacroDefinition {
        name: "LDBL_DIG",
        header: "float.h",
        value: MacroValue::Integer {
            source: 18,
            rust_path: "18",
        },
    },
    MacroDefinition {
        name: "LDBL_MIN_10_EXP",
        header: "float.h",
        value: MacroValue::Integer {
            source: -4931,
            rust_path: "-4931",
        },
    },
    MacroDefinition {
        name: "LDBL_MAX_10_EXP",
        header: "float.h",
        value: MacroValue::Integer {
            source: 4932,
            rust_path: "4932",
        },
    },
    MacroDefinition {
        name: "LDBL_DECIMAL_DIG",
        header: "float.h",
        value: MacroValue::Integer {
            source: 21,
            rust_path: "21",
        },
    },
    MacroDefinition {
        name: "DECIMAL_DIG",
        header: "float.h",
        value: MacroValue::Integer {
            source: 21,
            rust_path: "21",
        },
    },
    MacroDefinition {
        name: "FLT_RADIX",
        header: "float.h",
        value: MacroValue::Integer {
            source: 2,
            rust_path: "2",
        },
    },
    MacroDefinition {
        name: "FLT_ROUNDS",
        header: "float.h",
        value: MacroValue::Integer {
            source: 1,
            rust_path: "1",
        },
    },
    MacroDefinition {
        name: "FLT_EVAL_METHOD",
        header: "float.h",
        value: MacroValue::Integer {
            source: 0,
            rust_path: "0",
        },
    },
    MacroDefinition {
        name: "FLT_HAS_SUBNORM",
        header: "float.h",
        value: MacroValue::Integer {
            source: 1,
            rust_path: "1",
        },
    },
    MacroDefinition {
        name: "DBL_HAS_SUBNORM",
        header: "float.h",
        value: MacroValue::Integer {
            source: 1,
            rust_path: "1",
        },
    },
    MacroDefinition {
        name: "LDBL_HAS_SUBNORM",
        header: "float.h",
        value: MacroValue::Integer {
            source: 1,
            rust_path: "1",
        },
    },
    MacroDefinition {
        name: "FLT_MANT_DIG",
        header: "float.h",
        value: MacroValue::Integer {
            source: 24,
            rust_path: "24",
        },
    },
    MacroDefinition {
        name: "DBL_MANT_DIG",
        header: "float.h",
        value: MacroValue::Integer {
            source: 53,
            rust_path: "53",
        },
    },
    MacroDefinition {
        name: "FLT_DECIMAL_DIG",
        header: "float.h",
        value: MacroValue::Integer {
            source: 9,
            rust_path: "9",
        },
    },
    MacroDefinition {
        name: "DBL_DECIMAL_DIG",
        header: "float.h",
        value: MacroValue::Integer {
            source: 17,
            rust_path: "17",
        },
    },
    MacroDefinition {
        name: "FLT_DIG",
        header: "float.h",
        value: MacroValue::Integer {
            source: 6,
            rust_path: "6",
        },
    },
    MacroDefinition {
        name: "DBL_DIG",
        header: "float.h",
        value: MacroValue::Integer {
            source: 15,
            rust_path: "15",
        },
    },
    MacroDefinition {
        name: "FLT_MIN_EXP",
        header: "float.h",
        value: MacroValue::Integer {
            source: -125,
            rust_path: "-125",
        },
    },
    MacroDefinition {
        name: "DBL_MIN_EXP",
        header: "float.h",
        value: MacroValue::Integer {
            source: -1021,
            rust_path: "-1021",
        },
    },
    MacroDefinition {
        name: "FLT_MIN_10_EXP",
        header: "float.h",
        value: MacroValue::Integer {
            source: -37,
            rust_path: "-37",
        },
    },
    MacroDefinition {
        name: "DBL_MIN_10_EXP",
        header: "float.h",
        value: MacroValue::Integer {
            source: -307,
            rust_path: "-307",
        },
    },
    MacroDefinition {
        name: "FLT_MAX_EXP",
        header: "float.h",
        value: MacroValue::Integer {
            source: 128,
            rust_path: "128",
        },
    },
    MacroDefinition {
        name: "DBL_MAX_EXP",
        header: "float.h",
        value: MacroValue::Integer {
            source: 1024,
            rust_path: "1024",
        },
    },
    MacroDefinition {
        name: "FLT_MAX_10_EXP",
        header: "float.h",
        value: MacroValue::Integer {
            source: 38,
            rust_path: "38",
        },
    },
    MacroDefinition {
        name: "DBL_MAX_10_EXP",
        header: "float.h",
        value: MacroValue::Integer {
            source: 308,
            rust_path: "308",
        },
    },
    MacroDefinition {
        name: "FLT_MAX",
        header: "float.h",
        value: MacroValue::Float {
            source: "3.4028234663852886E+38",
            rust_bits: 0x7f7fffff,
        },
    },
    MacroDefinition {
        name: "FLT_EPSILON",
        header: "float.h",
        value: MacroValue::Float {
            source: "1.1920928955078125E-7",
            rust_bits: 0x34000000,
        },
    },
    MacroDefinition {
        name: "FLT_MIN",
        header: "float.h",
        value: MacroValue::Float {
            source: "1.1754943508222875E-38",
            rust_bits: 0x00800000,
        },
    },
    MacroDefinition {
        name: "FLT_TRUE_MIN",
        header: "float.h",
        value: MacroValue::Float {
            source: "1.4012984643248171E-45",
            rust_bits: 1,
        },
    },
    MacroDefinition {
        name: "DBL_MAX",
        header: "float.h",
        value: MacroValue::Double {
            source: "1.7976931348623157E+308",
            rust_bits: 0x7fefffffffffffff,
        },
    },
    MacroDefinition {
        name: "DBL_EPSILON",
        header: "float.h",
        value: MacroValue::Double {
            source: "2.2204460492503131E-16",
            rust_bits: 0x3cb0000000000000,
        },
    },
    MacroDefinition {
        name: "DBL_MIN",
        header: "float.h",
        value: MacroValue::Double {
            source: "2.2250738585072014E-308",
            rust_bits: 0x0010000000000000,
        },
    },
    MacroDefinition {
        name: "DBL_TRUE_MIN",
        header: "float.h",
        value: MacroValue::Double {
            source: "4.9406564584124654E-324",
            rust_bits: 1,
        },
    },
];

impl MacroDefinition {
    pub fn source_value_matches(&self, value: &str) -> bool {
        match self.value {
            MacroValue::Integer { source, .. } => value.parse::<i128>() == Ok(source),
            MacroValue::LongDouble {
                source,
                f64_source,
                f128_source,
                ..
            } => value == source || value == f64_source || value == f128_source,
            MacroValue::Float { source, .. } | MacroValue::Double { source, .. } => value == source,
        }
    }
}

pub fn lookup(name: &str) -> Option<&'static MacroDefinition> {
    MACROS.iter().find(|m| m.name == name)
}
