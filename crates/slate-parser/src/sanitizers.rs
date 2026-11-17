use crate::compiler_args::{CompilerArgError, CompilerFlavor};
use crate::compiler_options::Optimization;
use crate::target_info::TargetFamily;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Sanitizer {
    Address,
    KernelAddress,
    HwAddress,
    KernelHwAddress,
    Thread,
    Memory,
    KernelMemory,
    Leak,
    Dataflow,
    SafeStack,
    ShadowCallStack,
    Kcfi,
    Realtime,
    Type,
    Numerical,
    Scudo,
    Fuzzer,
    FuzzerNoLink,
    MemtagStack,
    PointerCompare,
    PointerSubtract,
    Alignment,
    Bool,
    Builtin,
    Bounds,
    BoundsStrict,
    ArrayBounds,
    LocalBounds,
    Enum,
    FloatCastOverflow,
    FloatDivideByZero,
    Function,
    IntegerDivideByZero,
    NonnullAttribute,
    Null,
    ObjectSize,
    PointerOverflow,
    Return,
    ReturnsNonnullAttribute,
    ShiftBase,
    ShiftExponent,
    SignedIntegerOverflow,
    UnsignedIntegerOverflow,
    UnsignedShiftBase,
    Unreachable,
    VlaBound,
    Vptr,
    ImplicitUnsignedIntegerTruncation,
    ImplicitSignedIntegerTruncation,
    ImplicitIntegerSignChange,
    ImplicitBitfieldConversion,
    NullabilityArg,
    NullabilityAssign,
    NullabilityReturn,
}

use Sanitizer::*;

const fn mask(sanitizers: &[Sanitizer]) -> u64 {
    let mut bits = 0;
    let mut index = 0;
    while index < sanitizers.len() {
        bits |= 1 << sanitizers[index] as u64;
        index += 1;
    }
    bits
}

const BOTH: &[CompilerFlavor] = &[CompilerFlavor::Gcc, CompilerFlavor::Clang];
const GCC: &[CompilerFlavor] = &[CompilerFlavor::Gcc];
const CLANG: &[CompilerFlavor] = &[CompilerFlavor::Clang];

const ATOMS: &[(Sanitizer, &str, &[CompilerFlavor])] = &[
    (Address, "address", BOTH),
    (KernelAddress, "kernel-address", BOTH),
    (HwAddress, "hwaddress", BOTH),
    (KernelHwAddress, "kernel-hwaddress", BOTH),
    (Thread, "thread", BOTH),
    (Memory, "memory", CLANG),
    (KernelMemory, "kernel-memory", CLANG),
    (Leak, "leak", BOTH),
    (Dataflow, "dataflow", CLANG),
    (SafeStack, "safe-stack", CLANG),
    (ShadowCallStack, "shadow-call-stack", BOTH),
    (Kcfi, "kcfi", CLANG),
    (Realtime, "realtime", CLANG),
    (Type, "type", CLANG),
    (Numerical, "numerical", CLANG),
    (Scudo, "scudo", CLANG),
    (Fuzzer, "fuzzer", CLANG),
    (FuzzerNoLink, "fuzzer-no-link", CLANG),
    (MemtagStack, "memtag-stack", GCC),
    (PointerCompare, "pointer-compare", BOTH),
    (PointerSubtract, "pointer-subtract", BOTH),
    (Alignment, "alignment", BOTH),
    (Bool, "bool", BOTH),
    (Builtin, "builtin", BOTH),
    (Bounds, "bounds", GCC),
    (BoundsStrict, "bounds-strict", GCC),
    (ArrayBounds, "array-bounds", CLANG),
    (LocalBounds, "local-bounds", CLANG),
    (Enum, "enum", BOTH),
    (FloatCastOverflow, "float-cast-overflow", BOTH),
    (FloatDivideByZero, "float-divide-by-zero", BOTH),
    (Function, "function", CLANG),
    (IntegerDivideByZero, "integer-divide-by-zero", BOTH),
    (NonnullAttribute, "nonnull-attribute", BOTH),
    (Null, "null", BOTH),
    (ObjectSize, "object-size", BOTH),
    (PointerOverflow, "pointer-overflow", BOTH),
    (Return, "return", BOTH),
    (ReturnsNonnullAttribute, "returns-nonnull-attribute", BOTH),
    (ShiftBase, "shift-base", BOTH),
    (ShiftExponent, "shift-exponent", BOTH),
    (SignedIntegerOverflow, "signed-integer-overflow", BOTH),
    (UnsignedIntegerOverflow, "unsigned-integer-overflow", CLANG),
    (UnsignedShiftBase, "unsigned-shift-base", CLANG),
    (Unreachable, "unreachable", BOTH),
    (VlaBound, "vla-bound", BOTH),
    (Vptr, "vptr", BOTH),
    (
        ImplicitUnsignedIntegerTruncation,
        "implicit-unsigned-integer-truncation",
        CLANG,
    ),
    (
        ImplicitSignedIntegerTruncation,
        "implicit-signed-integer-truncation",
        CLANG,
    ),
    (
        ImplicitIntegerSignChange,
        "implicit-integer-sign-change",
        CLANG,
    ),
    (
        ImplicitBitfieldConversion,
        "implicit-bitfield-conversion",
        CLANG,
    ),
    (NullabilityArg, "nullability-arg", CLANG),
    (NullabilityAssign, "nullability-assign", CLANG),
    (NullabilityReturn, "nullability-return", CLANG),
];

const SHIFT: u64 = mask(&[ShiftBase, ShiftExponent]);
const CLANG_UNDEFINED: u64 = SHIFT
    | mask(&[
        Alignment,
        Bool,
        Builtin,
        ArrayBounds,
        Enum,
        FloatCastOverflow,
        Function,
        IntegerDivideByZero,
        NonnullAttribute,
        Null,
        ObjectSize,
        PointerOverflow,
        Return,
        ReturnsNonnullAttribute,
        SignedIntegerOverflow,
        Unreachable,
        VlaBound,
        Vptr,
    ]);
const GCC_UNDEFINED: u64 = SHIFT
    | mask(&[
        Alignment,
        Bool,
        Builtin,
        Bounds,
        Enum,
        IntegerDivideByZero,
        NonnullAttribute,
        Null,
        ObjectSize,
        PointerOverflow,
        Return,
        ReturnsNonnullAttribute,
        SignedIntegerOverflow,
        Unreachable,
        VlaBound,
        Vptr,
    ]);
const IMPLICIT_INTEGER_TRUNCATION: u64 = mask(&[
    ImplicitUnsignedIntegerTruncation,
    ImplicitSignedIntegerTruncation,
]);
const IMPLICIT_INTEGER: u64 = IMPLICIT_INTEGER_TRUNCATION | mask(&[ImplicitIntegerSignChange]);

const GROUPS: &[(&str, u64, &[CompilerFlavor])] = &[
    ("shift", SHIFT, BOTH),
    ("undefined", CLANG_UNDEFINED, CLANG),
    ("undefined", GCC_UNDEFINED, GCC),
    ("undefined-trap", CLANG_UNDEFINED, CLANG),
    (
        "integer",
        SHIFT
            | IMPLICIT_INTEGER
            | mask(&[
                IntegerDivideByZero,
                SignedIntegerOverflow,
                UnsignedIntegerOverflow,
                UnsignedShiftBase,
            ]),
        CLANG,
    ),
    (
        "implicit-conversion",
        IMPLICIT_INTEGER | mask(&[ImplicitBitfieldConversion]),
        CLANG,
    ),
    (
        "implicit-integer-truncation",
        IMPLICIT_INTEGER_TRUNCATION,
        CLANG,
    ),
    (
        "implicit-integer-arithmetic-value-change",
        mask(&[ImplicitSignedIntegerTruncation, ImplicitIntegerSignChange]),
        CLANG,
    ),
    (
        "nullability",
        mask(&[NullabilityArg, NullabilityAssign, NullabilityReturn]),
        CLANG,
    ),
    ("bounds", mask(&[ArrayBounds, LocalBounds]), CLANG),
];

const ADDRESS: u64 = mask(&[Address, KernelAddress]);
const HWADDRESS: u64 = mask(&[HwAddress, KernelHwAddress]);

const CLANG_CONFLICTS: &[(Sanitizer, u64)] = &[
    (
        Address,
        mask(&[
            KernelAddress,
            HwAddress,
            KernelHwAddress,
            Thread,
            Memory,
            KernelMemory,
            SafeStack,
            Realtime,
            Type,
            Scudo,
        ]),
    ),
    (
        KernelAddress,
        mask(&[
            HwAddress,
            KernelHwAddress,
            Thread,
            Memory,
            KernelMemory,
            Leak,
            SafeStack,
            Type,
            Scudo,
        ]),
    ),
    (
        HwAddress,
        mask(&[
            KernelHwAddress,
            Thread,
            Memory,
            KernelMemory,
            SafeStack,
            Scudo,
        ]),
    ),
    (KernelHwAddress, mask(&[Thread, Memory, Leak, SafeStack])),
    (
        Thread,
        mask(&[Memory, KernelMemory, Leak, SafeStack, Realtime, Type, Scudo]),
    ),
    (
        Memory,
        mask(&[KernelMemory, Leak, SafeStack, Realtime, Type, Scudo]),
    ),
    (KernelMemory, mask(&[Leak, SafeStack, Scudo])),
    (Leak, mask(&[SafeStack, Type, Scudo])),
    (Kcfi, mask(&[Function])),
    (Realtime, CLANG_UNDEFINED | mask(&[Type])),
];

const GCC_CONFLICTS: &[(Sanitizer, u64)] = &[
    (
        Address,
        mask(&[
            KernelAddress,
            HwAddress,
            KernelHwAddress,
            Thread,
            MemtagStack,
        ]),
    ),
    (
        KernelAddress,
        mask(&[HwAddress, KernelHwAddress, Thread, MemtagStack]),
    ),
    (HwAddress, mask(&[KernelHwAddress, Thread, MemtagStack])),
    (KernelHwAddress, mask(&[Thread, MemtagStack])),
    (Thread, mask(&[Leak])),
];

const CLANG_FEATURES: &[(&str, u64)] = &[
    ("address_sanitizer", ADDRESS),
    ("hwaddress_sanitizer", HWADDRESS),
    ("memory_sanitizer", mask(&[Memory, KernelMemory])),
    ("thread_sanitizer", mask(&[Thread])),
    ("leak_sanitizer", mask(&[Leak])),
    ("dataflow_sanitizer", mask(&[Dataflow])),
    ("safe_stack", mask(&[SafeStack])),
    ("shadow_call_stack", mask(&[ShadowCallStack])),
    ("kcfi", mask(&[Kcfi])),
    ("realtime_sanitizer", mask(&[Realtime])),
    ("type_sanitizer", mask(&[Type])),
    ("numerical_stability_sanitizer", mask(&[Numerical])),
    ("scudo", mask(&[Scudo])),
    ("coverage_sanitizer", mask(&[Fuzzer, FuzzerNoLink])),
    ("undefined_behavior_sanitizer", CLANG_UNDEFINED),
];

const GCC_FEATURES: &[(&str, u64)] = &[
    ("address_sanitizer", ADDRESS),
    ("hwaddress_sanitizer", HWADDRESS),
    ("thread_sanitizer", mask(&[Thread])),
    ("leak_sanitizer", mask(&[Leak])),
    (
        "undefined_behavior_sanitizer",
        GCC_UNDEFINED | mask(&[BoundsStrict]),
    ),
];

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq, Hash)]
pub struct Sanitizers(u64);

impl Sanitizers {
    pub fn resolve(
        arguments: &[String],
        flavor: CompilerFlavor,
        family: TargetFamily,
        optimization: Optimization,
    ) -> Result<Self, CompilerArgError> {
        let mut enabled = 0;
        let mut requested = 0;
        for argument in arguments {
            let (enable, list) = if let Some(list) = argument.strip_prefix("-fsanitize=") {
                (Some(true), list)
            } else if let Some(list) = argument.strip_prefix("-fno-sanitize=") {
                (Some(false), list)
            } else {
                (None, argument.split_once('=').map_or("", |(_, list)| list))
            };
            if enable == Some(true) && list.is_empty() && flavor.is_gcc() {
                return Err(invalid(argument, "missing argument"));
            }
            let mut bits = 0;
            for name in list.split(',').filter(|name| !name.is_empty()) {
                bits |= match name {
                    "all" if enable == Some(true) => None,
                    "all" => Some(u64::MAX),
                    _ => lookup(name, flavor),
                }
                .ok_or_else(|| invalid(argument, &format!("unsupported argument `{name}`")))?;
            }
            match enable {
                Some(true) => {
                    enabled |= bits;
                    requested |= bits;
                }
                Some(false) => enabled &= !bits,
                None => {}
            }
        }
        if flavor.is_clang() {
            enabled &= !mask(&[Vptr]);
            if optimization == Optimization::None {
                enabled &= !mask(&[ObjectSize]);
            }
        }
        if flavor.is_gcc()
            && enabled & mask(&[ShadowCallStack]) != 0
            && family != TargetFamily::AArch64
        {
            return Err(invalid(
                "-fsanitize=shadow-call-stack",
                "not supported on this target",
            ));
        }
        let conflicts = if flavor.is_gcc() {
            GCC_CONFLICTS
        } else {
            CLANG_CONFLICTS
        };
        for &(sanitizer, others) in conflicts {
            if enabled & mask(&[sanitizer]) != 0 && enabled & others != 0 {
                return Err(invalid(
                    &format!("-fsanitize={}", name(sanitizer as u32)),
                    &format!(
                        "not allowed with `-fsanitize={}`",
                        name((enabled & others).trailing_zeros())
                    ),
                ));
            }
        }
        let pointer_checks = enabled & mask(&[PointerCompare, PointerSubtract]);
        let address = if flavor.is_gcc() {
            enabled & ADDRESS
        } else {
            requested & mask(&[Address])
        };
        if pointer_checks != 0 && address == 0 {
            return Err(invalid(
                &format!("-fsanitize={}", name(pointer_checks.trailing_zeros())),
                "only allowed with `-fsanitize=address`",
            ));
        }
        Ok(Self(enabled))
    }

    pub fn has_feature(self, name: &str, flavor: CompilerFlavor) -> Option<bool> {
        let features = match flavor {
            CompilerFlavor::Gcc => GCC_FEATURES,
            CompilerFlavor::Clang => CLANG_FEATURES,
            _ => return None,
        };
        features
            .iter()
            .find(|(feature, _)| *feature == name)
            .map(|(_, bits)| self.0 & bits != 0)
    }

    pub fn predefines(self) -> impl Iterator<Item = &'static str> {
        [
            ("__SANITIZE_ADDRESS__", ADDRESS),
            ("__SANITIZE_HWADDRESS__", HWADDRESS),
            ("__SANITIZE_THREAD__", mask(&[Thread])),
        ]
        .into_iter()
        .filter(move |(_, bits)| self.0 & bits != 0)
        .map(|(name, _)| name)
    }
}

fn lookup(name: &str, flavor: CompilerFlavor) -> Option<u64> {
    GROUPS
        .iter()
        .find(|(group, _, flavors)| *group == name && flavors.contains(&flavor))
        .map(|(_, bits, _)| *bits)
        .or_else(|| {
            ATOMS
                .iter()
                .find(|(_, atom, flavors)| *atom == name && flavors.contains(&flavor))
                .map(|(sanitizer, _, _)| mask(&[*sanitizer]))
        })
}

fn name(bit: u32) -> &'static str {
    ATOMS
        .iter()
        .find(|(sanitizer, _, _)| *sanitizer as u32 == bit)
        .map_or("", |(_, name, _)| name)
}

fn invalid(argument: &str, reason: &str) -> CompilerArgError {
    CompilerArgError::Invalid {
        argument: argument.to_owned(),
        reason: reason.to_owned(),
    }
}
