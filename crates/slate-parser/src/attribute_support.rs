mod registered;

use crate::compiler_args::{CompilerFlavor, LanguageStandard};
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
    NotArm32,
    Arm32,
}

impl Gate {
    fn admits(self, target: &TargetInfo) -> bool {
        let x86 = target.family.is_x86();
        match self {
            Self::Never => false,
            Self::Always => true,
            Self::Windows => target.os == TargetOs::Windows,
            Self::NotWindows => target.os != TargetOs::Windows,
            Self::X86 => x86,
            Self::X86OrArm32 => x86 || target.family == TargetFamily::Arm32,
            Self::NotAArch64 => target.family != TargetFamily::AArch64,
            Self::NotArm32 => target.family != TargetFamily::Arm32,
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

impl Support {
    fn gate(&self, flavor: CompilerFlavor) -> Gate {
        match flavor {
            CompilerFlavor::Clang => self.clang,
            CompilerFlavor::Gcc => self.gcc,
            CompilerFlavor::Msvc => self.msvc,
        }
    }
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
    support("preserve_none", Gate::NotArm32, Gate::Always),
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
    both("counted_by"),
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

// clang never unwraps `__name__` in a declspec and ignores every name not listed
#[rustfmt::skip]
const CLANG_DECLSPECS: &[(&str, Gate)] = &[
    ("align", Gate::Always),
    ("allocate", Gate::Always),
    ("allocator", Gate::Always),
    ("code_seg", Gate::Always),
    ("cpu_dispatch", Gate::Always),
    ("cpu_specific", Gate::Always),
    ("deprecated", Gate::Always),
    ("dllexport", Gate::Windows),
    ("dllimport", Gate::Windows),
    ("empty_bases", Gate::Windows),
    ("guard", Gate::Windows),
    ("layout_version", Gate::Windows),
    ("naked", Gate::Always),
    ("no_init_all", Gate::Always),
    ("noalias", Gate::Always),
    ("noinline", Gate::Always),
    ("noreturn", Gate::Always),
    ("nothrow", Gate::Always),
    ("novtable", Gate::Windows),
    ("property", Gate::Always),
    ("restrict", Gate::Always),
    ("safebuffers", Gate::Always),
    ("selectany", Gate::Always),
    ("thread", Gate::Always),
    ("uuid", Gate::Always),
];

// `__has_c_attribute` values of the standard attributes; cl answers only in C23 mode
#[rustfmt::skip]
const STANDARD_ATTRIBUTES: &[(&str, i64, i64, i64)] = &[
    // name            clang   gcc     msvc
    ("deprecated",     201904, 202311, 0),
    ("fallthrough",    201910, 202311, 202311),
    ("maybe_unused",   202106, 202311, 202311),
    ("nodiscard",      202003, 202311, 202311),
    ("noreturn",       202202, 202311, 0),
    ("_Noreturn",      202202, 202311, 0),
    ("unsequenced",    0,      202311, 0),
    ("reproducible",   0,      202311, 0),
];

pub fn is_modeled(name: &str) -> bool {
    lookup(name).is_some()
}

// unmodeled names fall back to the generated lists, which ignore the target
pub fn gnu_registered(name: &str, flavor: CompilerFlavor, target: &TargetInfo) -> bool {
    let name = unwrapped(name);
    match lookup(name) {
        Some(support) => support.gate(flavor).admits(target),
        None => match flavor {
            CompilerFlavor::Clang => listed(registered::CLANG, name),
            CompilerFlavor::Gcc => listed(registered::GCC, name),
            CompilerFlavor::Msvc => false,
        },
    }
}

// mingw gcc defines `__declspec(x)` as `__attribute__((x))`; cl rejects names it does not know
pub fn declspec_registered(name: &str, flavor: CompilerFlavor, target: &TargetInfo) -> bool {
    match flavor {
        CompilerFlavor::Clang => CLANG_DECLSPECS
            .iter()
            .any(|(declspec, gate)| *declspec == name && gate.admits(target)),
        CompilerFlavor::Gcc => gnu_registered(name, flavor, target),
        CompilerFlavor::Msvc => true,
    }
}

// gcc's `__has_attribute` also answers for standard attributes with no GNU spelling
pub fn has_attribute(name: &str, flavor: CompilerFlavor, target: &TargetInfo) -> bool {
    gnu_registered(name, flavor, target)
        || (flavor.is_gcc()
            && standard_attribute_value(unwrapped(name), flavor, LanguageStandard::C23) != 0)
}

pub fn has_c_attribute(
    spelling: &str,
    flavor: CompilerFlavor,
    standard: LanguageStandard,
    target: &TargetInfo,
) -> i64 {
    match spelling.split_once("::") {
        Some(_) => (!flavor.is_msvc() && spelling_registered(spelling, flavor, target)) as i64,
        None => standard_attribute_value(unwrapped(spelling), flavor, standard),
    }
}

pub fn spelling_registered(spelling: &str, flavor: CompilerFlavor, target: &TargetInfo) -> bool {
    let Some((scope, name)) = spelling.split_once("::") else {
        return gnu_registered(spelling, flavor, target);
    };
    let name = unwrapped(name);
    let scoped = match (flavor, unwrapped(scope)) {
        (CompilerFlavor::Clang, "gnu") => registered::CLANG_GNU_SCOPE,
        (CompilerFlavor::Clang, "clang") => registered::CLANG_CLANG_SCOPE,
        (CompilerFlavor::Clang, "msvc") => registered::CLANG_MSVC_SCOPE,
        (CompilerFlavor::Gcc, "gnu") => registered::GCC_GNU_SCOPE,
        (CompilerFlavor::Msvc, "msvc") => return true,
        _ => return false,
    };
    listed(scoped, name) && lookup(name).is_none_or(|support| support.gate(flavor).admits(target))
}

fn standard_attribute_value(name: &str, flavor: CompilerFlavor, standard: LanguageStandard) -> i64 {
    let Some(&(_, clang, gcc, msvc)) = STANDARD_ATTRIBUTES
        .iter()
        .find(|(standard_name, ..)| *standard_name == name)
    else {
        return 0;
    };
    match flavor {
        CompilerFlavor::Clang => clang,
        CompilerFlavor::Gcc => gcc,
        CompilerFlavor::Msvc if standard.at_least_c23() => msvc,
        CompilerFlavor::Msvc => 0,
    }
}

fn listed(names: &[&str], name: &str) -> bool {
    names.binary_search(&name).is_ok()
}

fn unwrapped(name: &str) -> &str {
    name.strip_prefix("__")
        .and_then(|name| name.strip_suffix("__"))
        .unwrap_or(name)
}

fn lookup(name: &str) -> Option<&'static Support> {
    GNU_ATTRIBUTES.iter().find(|support| support.name == name)
}
