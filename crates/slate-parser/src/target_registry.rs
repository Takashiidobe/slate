use crate::compiler_args::CompilerFlavor;
use crate::ir::AbiConvention;
use crate::target_info::{TargetError, TargetInfo, VaListKind};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Predefines {
    pub flavors: &'static [CompilerFlavor],
    pub name: &'static str,
    pub source: &'static str,
    pub defaults: &'static str,
    pub gnu_namespace: &'static str,
}

impl Predefines {
    pub fn value(&self, name: &str) -> Option<&'static str> {
        self.source.lines().find_map(|line| {
            line.strip_prefix("#define ")?
                .strip_prefix(name)?
                .strip_prefix(' ')
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum SysrootLayout {
    WindowsKits,
    Unix { multiarch: Option<&'static str> },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ClangHeaders {
    Upstream,
    AppleFirst,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum GccHeaders {
    X86,
    Aarch64,
    Arm,
}

impl GccHeaders {
    pub fn family(self) -> &'static str {
        match self {
            Self::X86 => "x86",
            Self::Aarch64 => "aarch64",
            Self::Arm => "arm",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct TargetProfile {
    pub predefines: &'static [Predefines],
    pub sysroot: SysrootLayout,
    pub clang_headers: ClangHeaders,
    pub gcc_headers: Option<GccHeaders>,
    pub va_list: VaListKind,
    pub convention: AbiConvention,
}

impl TargetProfile {
    pub fn predefines(&self, flavor: CompilerFlavor) -> Option<&'static Predefines> {
        self.predefines
            .iter()
            .find(|predefines| predefines.flavors.contains(&flavor))
    }

    pub fn flavors(&self) -> impl Iterator<Item = CompilerFlavor> {
        self.predefines
            .iter()
            .flat_map(|predefines| predefines.flavors.iter().copied())
    }
}

pub struct TargetSpec {
    pub triple: &'static str,
    pub layout: fn() -> TargetInfo,
    pub profile: TargetProfile,
}

pub fn lookup(triple: &str) -> Result<&'static TargetSpec, TargetError> {
    let with_vendor = gnu_short_triple(triple);
    TARGETS
        .iter()
        .find(|spec| spec.triple == triple)
        .or_else(|| {
            TARGETS
                .iter()
                .find(|spec| with_vendor.as_deref() == Some(spec.triple))
        })
        .ok_or_else(|| TargetError::UnsupportedTriple(triple.into()))
}

fn gnu_short_triple(triple: &str) -> Option<String> {
    let (arch, rest) = triple.split_once('-')?;
    rest.starts_with("linux-")
        .then(|| format!("{arch}-unknown-{rest}"))
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ArchMode {
    Code16,
    Bits32,
    Bits64,
    X32,
}

impl ArchMode {
    pub fn spelling(self) -> &'static str {
        match self {
            Self::Code16 => "-m16",
            Self::Bits32 => "-m32",
            Self::Bits64 => "-m64",
            Self::X32 => "-mx32",
        }
    }
}

pub fn arch_variant(triple: &str, mode: ArchMode) -> Result<String, TargetError> {
    let unsupported = || TargetError::UnsupportedArchMode {
        triple: triple.into(),
        mode: mode.spelling(),
    };
    let (arch, rest) = triple.split_once('-').ok_or_else(unsupported)?;
    let x86_32 = matches!(arch, "i386" | "i486" | "i586" | "i686");
    let variant_arch = match (arch, mode) {
        ("x86_64", ArchMode::Code16 | ArchMode::Bits32) => "i686",
        (_, ArchMode::Code16 | ArchMode::Bits32) if x86_32 => arch,
        ("x86_64" | "aarch64", ArchMode::Bits64) => arch,
        (_, ArchMode::Bits64) if x86_32 => "x86_64",
        ("x86_64", ArchMode::X32) if rest.ends_with("-gnu") => {
            return Ok(format!("x86_64-{rest}x32"));
        }
        _ => return Err(unsupported()),
    };
    Ok(format!("{variant_arch}-{rest}"))
}

const CLANG_AND_MSVC: &[CompilerFlavor] = &[CompilerFlavor::Clang, CompilerFlavor::Msvc];
const GCC: &[CompilerFlavor] = &[CompilerFlavor::Gcc];
const CLANG: &[CompilerFlavor] = &[CompilerFlavor::Clang];
const MSVC: &[CompilerFlavor] = &[CompilerFlavor::Msvc];

const fn clang_only(name: &'static str, source: &'static str) -> Predefines {
    Predefines {
        flavors: CLANG,
        name,
        source,
        defaults: "",
        gnu_namespace: "",
    }
}

const LINUX_GNU_NAMESPACE: &str = include_str!("predefines/slate_gnu_namespace_linux.h");

const X86_64_LINUX_GNU: Predefines = Predefines {
    flavors: CLANG_AND_MSVC,
    name: "<clang-x86_64-linux-gnu-predefines>",
    source: include_str!("predefines/clang-22.1.8_x86_64_linux_gnu.h"),
    defaults: include_str!("predefines/slate_target_defaults.h"),
    gnu_namespace: LINUX_GNU_NAMESPACE,
};

const I686_LINUX_GNU: Predefines = Predefines {
    flavors: CLANG_AND_MSVC,
    name: "<clang-i686-linux-gnu-predefines>",
    source: include_str!("predefines/clang-22.1.8_i686_linux_gnu.h"),
    defaults: include_str!("predefines/slate_x86_linux_defaults.h"),
    gnu_namespace: include_str!("predefines/slate_gnu_namespace_i686_linux.h"),
};

const X86_64_LINUX_GNU_GCC: Predefines = Predefines {
    flavors: GCC,
    name: "<gcc-x86_64-linux-gnu-predefines>",
    source: include_str!("predefines/gcc-16.2.1_x86_64_linux_gnu.h"),
    ..X86_64_LINUX_GNU
};

const I686_LINUX_GNU_GCC: Predefines = Predefines {
    flavors: GCC,
    name: "<gcc-i686-linux-gnu-predefines>",
    source: include_str!("predefines/gcc-16.2.1_i686_linux_gnu.h"),
    ..I686_LINUX_GNU
};

const AARCH64_LINUX_GNU: Predefines = Predefines {
    flavors: CLANG_AND_MSVC,
    name: "<clang-aarch64-linux-gnu-predefines>",
    source: include_str!("predefines/clang-22.1.8_aarch64_linux_gnu.h"),
    defaults: include_str!("predefines/slate_aarch64_linux_defaults.h"),
    gnu_namespace: LINUX_GNU_NAMESPACE,
};

const AARCH64_LINUX_GNU_GCC: Predefines = Predefines {
    flavors: GCC,
    name: "<gcc-aarch64-linux-gnu-predefines>",
    source: include_str!("predefines/gcc-16.1.0_aarch64_linux_gnu.h"),
    ..AARCH64_LINUX_GNU
};

const ARM32_LINUX_DEFAULTS: &str = include_str!("predefines/slate_arm32_linux_defaults.h");

const ARM32_LINUX_GNU_GCC: Predefines = Predefines {
    flavors: GCC,
    name: "<gcc-armv7-linux-gnueabi-predefines>",
    source: include_str!("predefines/gcc-15.2.1_armv7_linux_gnueabihf.h"),
    defaults: ARM32_LINUX_DEFAULTS,
    gnu_namespace: LINUX_GNU_NAMESPACE,
};

pub const X86_64_LINUX_GNU_PROFILE: TargetProfile = TargetProfile {
    predefines: &[X86_64_LINUX_GNU, X86_64_LINUX_GNU_GCC],
    sysroot: SysrootLayout::Unix {
        multiarch: Some("x86_64-linux-gnu"),
    },
    clang_headers: ClangHeaders::Upstream,
    gcc_headers: Some(GccHeaders::X86),
    va_list: VaListKind::X86_64Sysv,
    convention: AbiConvention::SysV64,
};

const I686_LINUX_GNU_PROFILE: TargetProfile = TargetProfile {
    predefines: &[I686_LINUX_GNU, I686_LINUX_GNU_GCC],
    sysroot: SysrootLayout::Unix {
        multiarch: Some("i686-linux-gnu"),
    },
    clang_headers: ClangHeaders::Upstream,
    gcc_headers: Some(GccHeaders::X86),
    va_list: VaListKind::CharPointer,
    convention: AbiConvention::X86Cdecl,
};

const fn clang_unix(
    predefines: &'static [Predefines],
    va_list: VaListKind,
    convention: AbiConvention,
) -> TargetProfile {
    TargetProfile {
        predefines,
        sysroot: SysrootLayout::Unix { multiarch: None },
        clang_headers: ClangHeaders::Upstream,
        gcc_headers: None,
        va_list,
        convention,
    }
}

const fn arm32_linux(predefines: &'static [Predefines], multiarch: &'static str) -> TargetProfile {
    TargetProfile {
        predefines,
        sysroot: SysrootLayout::Unix {
            multiarch: Some(multiarch),
        },
        clang_headers: ClangHeaders::Upstream,
        gcc_headers: Some(GccHeaders::Arm),
        va_list: VaListKind::ArmAapcs,
        convention: AbiConvention::Aapcs32,
    }
}

const fn windows_msvc(
    predefines: &'static [Predefines],
    convention: AbiConvention,
) -> TargetProfile {
    TargetProfile {
        predefines,
        sysroot: SysrootLayout::WindowsKits,
        clang_headers: ClangHeaders::Upstream,
        gcc_headers: None,
        va_list: VaListKind::CharPointer,
        convention,
    }
}

pub const TARGETS: &[TargetSpec] = &[
    TargetSpec {
        triple: "x86_64-unknown-linux-gnu",
        layout: TargetInfo::default,
        profile: X86_64_LINUX_GNU_PROFILE,
    },
    TargetSpec {
        triple: "i686-unknown-linux-gnu",
        layout: TargetInfo::x86_linux,
        profile: I686_LINUX_GNU_PROFILE,
    },
    TargetSpec {
        triple: "aarch64-unknown-linux-gnu",
        layout: TargetInfo::aarch64_linux,
        profile: TargetProfile {
            predefines: &[AARCH64_LINUX_GNU, AARCH64_LINUX_GNU_GCC],
            sysroot: SysrootLayout::Unix {
                multiarch: Some("aarch64-linux-gnu"),
            },
            clang_headers: ClangHeaders::Upstream,
            gcc_headers: Some(GccHeaders::Aarch64),
            va_list: VaListKind::AArch64Aapcs,
            convention: AbiConvention::Aapcs64,
        },
    },
    TargetSpec {
        triple: "aarch64-apple-darwin",
        layout: TargetInfo::aarch64_apple_darwin,
        profile: TargetProfile {
            clang_headers: ClangHeaders::AppleFirst,
            ..clang_unix(
                &[clang_only(
                    "<clang-aarch64-apple-darwin-predefines>",
                    include_str!("predefines/clang-22.1.8_aarch64_apple_darwin.h"),
                )],
                VaListKind::CharPointer,
                AbiConvention::Aapcs64,
            )
        },
    },
    TargetSpec {
        triple: "x86_64-apple-darwin",
        layout: TargetInfo::x86_64_apple_darwin,
        profile: TargetProfile {
            clang_headers: ClangHeaders::AppleFirst,
            ..clang_unix(
                &[clang_only(
                    "<clang-x86_64-apple-darwin-predefines>",
                    include_str!("predefines/clang-22.1.8_x86_64_apple_darwin.h"),
                )],
                VaListKind::X86_64Sysv,
                AbiConvention::SysV64,
            )
        },
    },
    TargetSpec {
        triple: "aarch64-linux-android",
        layout: TargetInfo::aarch64_android,
        profile: clang_unix(
            &[clang_only(
                "<clang-aarch64-linux-android-predefines>",
                include_str!("predefines/clang-22.1.8_aarch64_linux_android.h"),
            )],
            VaListKind::AArch64Aapcs,
            AbiConvention::Aapcs64,
        ),
    },
    TargetSpec {
        triple: "x86_64-linux-android",
        layout: TargetInfo::x86_64_android,
        profile: clang_unix(
            &[clang_only(
                "<clang-x86_64-linux-android-predefines>",
                include_str!("predefines/clang-22.1.8_x86_64_linux_android.h"),
            )],
            VaListKind::X86_64Sysv,
            AbiConvention::SysV64,
        ),
    },
    TargetSpec {
        triple: "aarch64-unknown-freebsd",
        layout: TargetInfo::aarch64_freebsd,
        profile: clang_unix(
            &[clang_only(
                "<clang-aarch64-unknown-freebsd-predefines>",
                include_str!("predefines/clang-22.1.8_aarch64_unknown_freebsd.h"),
            )],
            VaListKind::AArch64Aapcs,
            AbiConvention::Aapcs64,
        ),
    },
    TargetSpec {
        triple: "x86_64-unknown-freebsd",
        layout: TargetInfo::x86_64_freebsd,
        profile: clang_unix(
            &[clang_only(
                "<clang-x86_64-unknown-freebsd-predefines>",
                include_str!("predefines/clang-22.1.8_x86_64_unknown_freebsd.h"),
            )],
            VaListKind::X86_64Sysv,
            AbiConvention::SysV64,
        ),
    },
    TargetSpec {
        triple: "armv7-unknown-linux-gnueabi",
        layout: TargetInfo::arm32_linux_gnueabi,
        profile: arm32_linux(
            &[
                Predefines {
                    flavors: CLANG_AND_MSVC,
                    name: "<clang-armv7-linux-gnueabi-predefines>",
                    source: include_str!("predefines/clang-22.1.8_armv7_linux_gnueabi.h"),
                    defaults: ARM32_LINUX_DEFAULTS,
                    gnu_namespace: LINUX_GNU_NAMESPACE,
                },
                ARM32_LINUX_GNU_GCC,
            ],
            "arm-linux-gnueabi",
        ),
    },
    TargetSpec {
        triple: "armv7-unknown-linux-gnueabihf",
        layout: TargetInfo::arm32_linux_gnueabihf,
        profile: arm32_linux(
            &[
                Predefines {
                    flavors: CLANG_AND_MSVC,
                    name: "<clang-armv7-linux-gnueabihf-predefines>",
                    source: include_str!("predefines/clang-22.1.8_armv7_linux_gnueabihf.h"),
                    defaults: ARM32_LINUX_DEFAULTS,
                    gnu_namespace: LINUX_GNU_NAMESPACE,
                },
                ARM32_LINUX_GNU_GCC,
            ],
            "arm-linux-gnueabihf",
        ),
    },
    TargetSpec {
        triple: "x86_64-pc-windows-msvc",
        layout: TargetInfo::x86_64_windows_msvc,
        profile: windows_msvc(
            &[
                Predefines {
                    flavors: MSVC,
                    ..clang_only(
                        "<msvc-x86_64-windows-predefines>",
                        include_str!("predefines/msvc_19.51.36257_x86_64_windows.h"),
                    )
                },
                clang_only(
                    "<clang-x86_64-windows-msvc-predefines>",
                    include_str!("predefines/clang-22.1.8_x86_64_windows_msvc.h"),
                ),
            ],
            AbiConvention::Win64,
        ),
    },
    TargetSpec {
        triple: "i686-pc-windows-msvc",
        layout: TargetInfo::i686_windows_msvc,
        profile: windows_msvc(
            &[
                Predefines {
                    flavors: MSVC,
                    ..clang_only(
                        "<msvc-x86-windows-predefines>",
                        include_str!("predefines/msvc_19.51.36257_x86_windows.h"),
                    )
                },
                Predefines {
                    gnu_namespace: include_str!("predefines/slate_gnu_namespace_i686_windows.h"),
                    ..clang_only(
                        "<clang-i686-windows-msvc-predefines>",
                        include_str!("predefines/clang-22.1.8_i686_windows_msvc.h"),
                    )
                },
            ],
            AbiConvention::X86Win32,
        ),
    },
    TargetSpec {
        triple: "aarch64-pc-windows-msvc",
        layout: TargetInfo::aarch64_windows_msvc,
        profile: windows_msvc(
            &[
                Predefines {
                    flavors: MSVC,
                    ..clang_only(
                        "<msvc-aarch64-windows-predefines>",
                        include_str!("predefines/msvc_19.51.36257_aarch64_windows.h"),
                    )
                },
                clang_only(
                    "<clang-aarch64-windows-msvc-predefines>",
                    include_str!("predefines/clang-22.1.8_aarch64_windows_msvc.h"),
                ),
            ],
            AbiConvention::WinArm64,
        ),
    },
    TargetSpec {
        triple: "thumbv7a-pc-windows-msvc",
        layout: TargetInfo::thumbv7a_windows_msvc,
        profile: windows_msvc(
            &[
                Predefines {
                    flavors: MSVC,
                    ..clang_only(
                        "<msvc-arm-windows-predefines>",
                        include_str!("predefines/msvc-19.44.35228_arm_windows.h"),
                    )
                },
                clang_only(
                    "<clang-thumbv7a-windows-msvc-predefines>",
                    include_str!("predefines/clang-22.1.8_thumbv7a_windows_msvc.h"),
                ),
            ],
            AbiConvention::Aapcs32,
        ),
    },
];
