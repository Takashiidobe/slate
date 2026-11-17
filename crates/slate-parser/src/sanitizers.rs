use crate::compiler_args::{CompilerArgError, CompilerFlavor, parse_clang_integer};
use crate::compiler_options::Optimization;
use crate::target_info::{TargetFamily, TargetOs};

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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Coverage {
    Func,
    Bb,
    Edge,
    TraceBb,
    TraceCmp,
    TraceDiv,
    TraceGep,
    Inline8bitCounters,
    TracePc,
    TracePcGuard,
    NoPrune,
    StackDepth,
    InlineBoolFlag,
    PcTable,
    IndirectCalls,
    Legacy8bitCounters,
    TraceLoads,
    TraceStores,
    ControlFlow,
}

const fn coverage_mask(features: &[Coverage]) -> u32 {
    let mut bits = 0;
    let mut index = 0;
    while index < features.len() {
        bits |= 1 << features[index] as u32;
        index += 1;
    }
    bits
}

const COVERAGE_NAMES: &[(Coverage, &str, &[CompilerFlavor])] = &[
    (Coverage::Func, "func", CLANG),
    (Coverage::Bb, "bb", CLANG),
    (Coverage::Edge, "edge", CLANG),
    (Coverage::TraceBb, "trace-bb", CLANG),
    (Coverage::TraceCmp, "trace-cmp", BOTH),
    (Coverage::TraceDiv, "trace-div", CLANG),
    (Coverage::TraceGep, "trace-gep", CLANG),
    (Coverage::Inline8bitCounters, "inline-8bit-counters", CLANG),
    (Coverage::TracePc, "trace-pc", BOTH),
    (Coverage::TracePcGuard, "trace-pc-guard", CLANG),
    (Coverage::NoPrune, "no-prune", CLANG),
    (Coverage::StackDepth, "stack-depth", CLANG),
    (Coverage::InlineBoolFlag, "inline-bool-flag", CLANG),
    (Coverage::PcTable, "pc-table", CLANG),
    (Coverage::IndirectCalls, "indirect-calls", CLANG),
    (Coverage::Legacy8bitCounters, "8bit-counters", CLANG),
    (Coverage::TraceLoads, "trace-loads", CLANG),
    (Coverage::TraceStores, "trace-stores", CLANG),
    (Coverage::ControlFlow, "control-flow", CLANG),
];

const INSERTION_POINTS: u32 = coverage_mask(&[Coverage::Func, Coverage::Bb, Coverage::Edge]);
const IMPLIES_EDGE: u32 = coverage_mask(&[
    Coverage::TracePc,
    Coverage::TracePcGuard,
    Coverage::Inline8bitCounters,
    Coverage::InlineBoolFlag,
    Coverage::ControlFlow,
]);
const COVERAGE_FEATURE: u32 = INSERTION_POINTS
    | coverage_mask(&[
        Coverage::IndirectCalls,
        Coverage::TraceCmp,
        Coverage::TraceLoads,
        Coverage::TraceStores,
        Coverage::ControlFlow,
    ]);
const FUZZER_COVERAGE: u32 = coverage_mask(&[
    Coverage::Inline8bitCounters,
    Coverage::IndirectCalls,
    Coverage::TraceCmp,
    Coverage::PcTable,
]);
const COVERAGE_UNSUPPORTED: u64 = mask(&[Scudo, Realtime]);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Instrumentation {
    Address,
    KernelAddress,
    Thread,
}

impl Instrumentation {
    pub fn name(self) -> &'static str {
        match self {
            Self::Address => "address",
            Self::KernelAddress => "kernel-address",
            Self::Thread => "thread",
        }
    }
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq, Hash)]
pub struct Sanitizers {
    checks: u64,
    coverage: bool,
}

impl Sanitizers {
    pub fn resolve(
        arguments: &[String],
        flavor: CompilerFlavor,
        family: TargetFamily,
        os: TargetOs,
        optimization: Optimization,
    ) -> Result<Self, CompilerArgError> {
        let mut enabled = 0;
        let mut requested = 0;
        let mut removed = 0;
        for argument in arguments {
            if is_coverage_argument(argument) {
                continue;
            }
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
                Some(false) => {
                    enabled &= !bits;
                    removed |= bits;
                }
                None => {}
            }
        }
        let coverage = resolve_coverage(arguments, flavor, os, enabled, removed)?;
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
        Ok(Self {
            checks: enabled,
            coverage,
        })
    }

    pub fn has_feature(self, name: &str, flavor: CompilerFlavor) -> Option<bool> {
        let features = match flavor {
            CompilerFlavor::Gcc => GCC_FEATURES,
            CompilerFlavor::Clang => CLANG_FEATURES,
            _ => return None,
        };
        if flavor.is_clang() && name == "coverage_sanitizer" {
            return Some(self.coverage);
        }
        features
            .iter()
            .find(|(feature, _)| *feature == name)
            .map(|(_, bits)| self.checks & bits != 0)
    }

    pub fn instrumentation(self) -> Option<Instrumentation> {
        [
            (Instrumentation::Address, Address),
            (Instrumentation::KernelAddress, KernelAddress),
            (Instrumentation::Thread, Thread),
        ]
        .into_iter()
        .find(|(_, sanitizer)| self.checks & mask(&[*sanitizer]) != 0)
        .map(|(instrumentation, _)| instrumentation)
    }

    pub fn predefines(self) -> impl Iterator<Item = &'static str> {
        [
            ("__SANITIZE_ADDRESS__", ADDRESS),
            ("__SANITIZE_HWADDRESS__", HWADDRESS),
            ("__SANITIZE_THREAD__", mask(&[Thread])),
        ]
        .into_iter()
        .filter(move |(_, bits)| self.checks & bits != 0)
        .map(|(name, _)| name)
    }
}

fn is_coverage_argument(argument: &str) -> bool {
    argument.starts_with("-fsanitize-coverage=") || argument.starts_with("-fno-sanitize-coverage=")
}

fn resolve_coverage(
    arguments: &[String],
    flavor: CompilerFlavor,
    os: TargetOs,
    enabled: u64,
    removed: u64,
) -> Result<bool, CompilerArgError> {
    let mut features = 0;
    if enabled & mask(&[Fuzzer, FuzzerNoLink]) != 0 {
        features = FUZZER_COVERAGE;
        if matches!(os, TargetOs::Linux | TargetOs::Android) {
            features |= coverage_mask(&[Coverage::StackDepth]);
        }
    }
    let unsupported = enabled & !removed & COVERAGE_UNSUPPORTED != 0;
    for argument in arguments
        .iter()
        .filter(|argument| is_coverage_argument(argument))
    {
        let (enable, list) = match argument.strip_prefix("-fsanitize-coverage=") {
            Some(list) => (true, list),
            None => (false, argument.split_once('=').map_or("", |(_, list)| list)),
        };
        if flavor.is_gcc() && list.is_empty() {
            return Err(invalid(argument, "missing argument"));
        }
        if enable && flavor.is_clang() && parse_clang_integer(list).is_some() {
            features = 0;
            continue;
        }
        let mut bits = 0;
        for name in list
            .split(',')
            .filter(|name| flavor.is_gcc() || !name.is_empty())
        {
            bits |= COVERAGE_NAMES
                .iter()
                .find(|(_, feature, flavors)| *feature == name && flavors.contains(&flavor))
                .map(|(feature, _, _)| coverage_mask(&[*feature]))
                .ok_or_else(|| invalid(argument, &format!("unsupported argument `{name}`")))?;
        }
        if !enable {
            features &= !bits;
        } else if unsupported {
            features = 0;
        } else {
            features |= bits;
        }
    }
    for (first, second) in [
        (Coverage::Func, Coverage::Bb),
        (Coverage::Func, Coverage::Edge),
        (Coverage::Bb, Coverage::Edge),
    ] {
        if features & coverage_mask(&[first, second]) == coverage_mask(&[first, second]) {
            return Err(invalid(
                &format!("-fsanitize-coverage={}", coverage_name(first)),
                &format!(
                    "not allowed with `-fsanitize-coverage={}`",
                    coverage_name(second)
                ),
            ));
        }
    }
    if features & INSERTION_POINTS == 0 {
        if features & IMPLIES_EDGE != 0 {
            features |= coverage_mask(&[Coverage::Edge]);
        }
        if features & coverage_mask(&[Coverage::StackDepth]) != 0 {
            features |= coverage_mask(&[Coverage::Func]);
        }
    }
    Ok(flavor.is_clang() && features & COVERAGE_FEATURE != 0)
}

fn coverage_name(feature: Coverage) -> &'static str {
    COVERAGE_NAMES
        .iter()
        .find(|(candidate, _, _)| *candidate == feature)
        .map_or("", |(_, name, _)| name)
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
