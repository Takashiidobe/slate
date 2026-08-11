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
            rust_bits: 1,
        },
    },
];

impl MacroDefinition {
    pub fn source_value_matches(&self, value: &str) -> bool {
        match self.value {
            MacroValue::Integer { source, .. } => value.parse::<i128>() == Ok(source),
            MacroValue::LongDouble { source, .. } => value == source,
        }
    }
}

pub fn lookup(name: &str) -> Option<&'static MacroDefinition> {
    MACROS.iter().find(|m| m.name == name)
}
