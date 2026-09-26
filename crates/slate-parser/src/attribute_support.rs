use crate::compiler_args::CompilerFlavor;
use crate::target_info::{TargetFamily, TargetInfo, TargetOs};

#[derive(Clone, Copy)]
enum Gate {
    Never,
    Always,
    Windows,
    NotWindows,
    X86,
    X86OrArm32,
    NotAArch64,
    Arm32,
}

impl Gate {
    fn admits(self, target: &TargetInfo) -> bool {
        let x86 = matches!(target.family, TargetFamily::X86 | TargetFamily::X86_64);
        match self {
            Self::Never => false,
            Self::Always => true,
            Self::Windows => target.os == TargetOs::Windows,
            Self::NotWindows => target.os != TargetOs::Windows,
            Self::X86 => x86,
            Self::X86OrArm32 => x86 || target.family == TargetFamily::Arm32,
            Self::NotAArch64 => target.family != TargetFamily::AArch64,
            Self::Arm32 => target.family == TargetFamily::Arm32,
        }
    }
}

struct Support {
    name: &'static str,
    clang: Gate,
    gcc: Gate,
    msvc: Gate,
}

const fn support(name: &'static str, clang: Gate, gcc: Gate) -> Support {
    Support {
        name,
        clang,
        gcc,
        msvc: Gate::Never,
    }
}

const fn both(name: &'static str) -> Support {
    support(name, Gate::Always, Gate::Always)
}

const fn clang_only(name: &'static str) -> Support {
    support(name, Gate::Always, Gate::Never)
}

const fn gcc_only(name: &'static str) -> Support {
    support(name, Gate::Never, Gate::Always)
}

#[rustfmt::skip]
const GNU_ATTRIBUTES: &[Support] = &[
    both("packed"),
    clang_only("address_space"),
    clang_only("pass_object_size"),
    clang_only("pass_dynamic_object_size"),
    clang_only("lifetimebound"),
    clang_only("overloadable"),
    both("gnu_inline"),
    both("nothrow"),
    support("selectany", Gate::Always, Gate::Windows),
    support("thread", Gate::Never, Gate::Never),
    support("noalias", Gate::Never, Gate::Never),
    support("restrict", Gate::Never, Gate::Never),
    support("code_seg", Gate::Never, Gate::Never),
    clang_only("optnone"),
    both("unused"),
    clang_only("preserve_most"),
    clang_only("preserve_all"),
    both("preserve_none"),
    both("aligned"),
    both("vector_size"),
    both("mode"),
    both("visibility"),
    both("section"),
    clang_only("annotate"),
    both("target"),
    both("alias"),
    both("weakref"),
    both("nonnull"),
    both("weak"),
    both("used"),
    both("retain"),
    both("noinline"),
    both("always_inline"),
    both("noreturn"),
    both("constructor"),
    both("destructor"),
    both("malloc"),
    both("assume_aligned"),
    both("alloc_size"),
    both("alloc_align"),
    both("cleanup"),
    both("returns_nonnull"),
    both("warn_unused_result"),
    both("sentinel"),
    both("cold"),
    both("flatten"),
    both("hot"),
    both("leaf"),
    gcc_only("noipa"),
    gcc_only("noclone"),
    gcc_only("optimize"),
    support("naked", Gate::Always, Gate::X86OrArm32),
    support("interrupt", Gate::NotAArch64, Gate::X86OrArm32),
    both("no_split_stack"),
    both("returns_twice"),
    clang_only("cpu_dispatch"),
    clang_only("cpu_specific"),
    both("target_clones"),
    support("ifunc", Gate::NotWindows, Gate::Always),
    support("dllimport", Gate::Windows, Gate::Windows),
    support("dllexport", Gate::Windows, Gate::Windows),
    clang_only("weak_import"),
    both("tls_model"),
    support("ms_struct", Gate::Always, Gate::X86),
    support("gcc_struct", Gate::Always, Gate::X86),
    support("stdcall", Gate::Always, Gate::X86),
    support("cdecl", Gate::Always, Gate::X86),
    support("fastcall", Gate::Always, Gate::X86),
    clang_only("vectorcall"),
    support("thiscall", Gate::Always, Gate::X86),
    support("ms_abi", Gate::Always, Gate::X86),
    support("sysv_abi", Gate::Always, Gate::X86),
    support("regparm", Gate::Always, Gate::X86),
    support("pcs", Gate::Always, Gate::Arm32),
    support("nomips16", Gate::Never, Gate::Never),
    clang_only("availability"),
    clang_only("ext_vector_type"),
    gcc_only("scalar_storage_order"),
    both("transparent_union"),
    both("format"),
    both("format_arg"),
    both("common"),
    both("nocommon"),
    both("pure"),
    both("const"),
    both("may_alias"),
    both("deprecated"),
    support("nodiscard", Gate::Never, Gate::Never),
    support("maybe_unused", Gate::Never, Gate::Never),
    both("fallthrough"),
];

pub fn is_modeled(name: &str) -> bool {
    lookup(name).is_some()
}

pub fn gnu_registered(name: &str, flavor: CompilerFlavor, target: &TargetInfo) -> bool {
    lookup(name).is_some_and(|support| {
        match flavor {
            CompilerFlavor::Clang => support.clang,
            CompilerFlavor::Gcc => support.gcc,
            CompilerFlavor::Msvc => support.msvc,
        }
        .admits(target)
    })
}

pub fn scope_registered(scope: &str, flavor: CompilerFlavor) -> bool {
    match flavor {
        CompilerFlavor::Clang => matches!(scope, "gnu" | "clang" | "msvc"),
        CompilerFlavor::Gcc => scope == "gnu",
        CompilerFlavor::Msvc => scope == "msvc",
    }
}

fn lookup(name: &str) -> Option<&'static Support> {
    GNU_ATTRIBUTES.iter().find(|support| support.name == name)
}
