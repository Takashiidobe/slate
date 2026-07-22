//! The `<limits.h>` constants Slate recognizes when a C source literally
//! invokes one of these macro names (confirmed via the macro-dump plugin,
//! not guessed from a coincidentally-matching literal value).

pub struct LimitsMacro {
    pub name: &'static str,
    pub value: i128,
    pub rust_path: &'static str,
}

pub const LIMITS_MACROS: &[LimitsMacro] = &[
    LimitsMacro {
        name: "SCHAR_MIN",
        value: i8::MIN as i128,
        rust_path: "i8::MIN",
    },
    LimitsMacro {
        name: "SCHAR_MAX",
        value: i8::MAX as i128,
        rust_path: "i8::MAX",
    },
    LimitsMacro {
        name: "UCHAR_MAX",
        value: u8::MAX as i128,
        rust_path: "u8::MAX",
    },
    LimitsMacro {
        name: "CHAR_MIN",
        value: i8::MIN as i128,
        rust_path: "i8::MIN",
    },
    LimitsMacro {
        name: "CHAR_MAX",
        value: i8::MAX as i128,
        rust_path: "i8::MAX",
    },
    LimitsMacro {
        name: "SHRT_MIN",
        value: i16::MIN as i128,
        rust_path: "i16::MIN",
    },
    LimitsMacro {
        name: "SHRT_MAX",
        value: i16::MAX as i128,
        rust_path: "i16::MAX",
    },
    LimitsMacro {
        name: "USHRT_MAX",
        value: u16::MAX as i128,
        rust_path: "u16::MAX",
    },
    LimitsMacro {
        name: "INT_MIN",
        value: i32::MIN as i128,
        rust_path: "i32::MIN",
    },
    LimitsMacro {
        name: "INT_MAX",
        value: i32::MAX as i128,
        rust_path: "i32::MAX",
    },
    LimitsMacro {
        name: "UINT_MAX",
        value: u32::MAX as i128,
        rust_path: "u32::MAX",
    },
    LimitsMacro {
        name: "LONG_MIN",
        value: i64::MIN as i128,
        rust_path: "i64::MIN",
    },
    LimitsMacro {
        name: "LONG_MAX",
        value: i64::MAX as i128,
        rust_path: "i64::MAX",
    },
    LimitsMacro {
        name: "ULONG_MAX",
        value: u64::MAX as i128,
        rust_path: "u64::MAX",
    },
    LimitsMacro {
        name: "LLONG_MIN",
        value: i64::MIN as i128,
        rust_path: "i64::MIN",
    },
    LimitsMacro {
        name: "LLONG_MAX",
        value: i64::MAX as i128,
        rust_path: "i64::MAX",
    },
    LimitsMacro {
        name: "ULLONG_MAX",
        value: u64::MAX as i128,
        rust_path: "u64::MAX",
    },
    LimitsMacro {
        name: "SIZE_MAX",
        value: u64::MAX as i128,
        rust_path: "usize::MAX",
    },
    LimitsMacro {
        name: "CHAR_BIT",
        value: u8::BITS as i128,
        rust_path: "u8::BITS",
    },
];

pub fn lookup(name: &str) -> Option<&'static LimitsMacro> {
    LIMITS_MACROS.iter().find(|m| m.name == name)
}
