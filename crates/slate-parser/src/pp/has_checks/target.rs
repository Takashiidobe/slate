#[derive(Debug, Clone, Copy)]
pub(in crate::pp) enum TargetCheck {
    Arch,
    Vendor,
    Os,
    Environment,
    VariantOs,
    VariantEnvironment,
}

impl TargetCheck {
    pub(in crate::pp) fn from_macro(name: &str) -> Option<Self> {
        Some(match name {
            "__is_target_arch" => Self::Arch,
            "__is_target_vendor" => Self::Vendor,
            "__is_target_os" => Self::Os,
            "__is_target_environment" => Self::Environment,
            "__is_target_variant_os" => Self::VariantOs,
            "__is_target_variant_environment" => Self::VariantEnvironment,
            _ => return None,
        })
    }

    pub(in crate::pp) fn matches(self, triple: &str, name: &str) -> bool {
        let triple = Triple::parse(triple);
        let name = name.to_ascii_lowercase();
        match self {
            Self::Arch => arch_matches(triple.arch, &name),
            Self::Vendor => triple.vendor.eq_ignore_ascii_case(&name),
            Self::Os => os_matches(triple.os, &name),
            Self::Environment => environment_matches(triple.environment, &name),
            Self::VariantOs | Self::VariantEnvironment => false,
        }
    }
}

struct Triple<'a> {
    arch: &'a str,
    vendor: &'a str,
    os: &'a str,
    environment: &'a str,
}

impl<'a> Triple<'a> {
    fn parse(triple: &'a str) -> Self {
        let parts: Vec<&str> = triple.split('-').collect();
        let part = |index: usize| parts.get(index).copied().unwrap_or("");
        let vendorless = parts.len() == 3 && is_os_name(part(1));
        let (vendor, os, environment) = if vendorless {
            ("unknown", part(1), part(2))
        } else {
            (part(1), part(2), part(3))
        };
        Self {
            arch: part(0),
            vendor: if vendor.is_empty() { "unknown" } else { vendor },
            os,
            environment,
        }
    }
}

fn is_os_name(name: &str) -> bool {
    matches!(name, "linux" | "windows" | "freebsd" | "darwin")
}

#[derive(PartialEq, Eq)]
struct Arch<'a> {
    family: &'a str,
    subarch: &'a str,
}

fn parse_arch(name: &str) -> Option<Arch<'_>> {
    let family = |family| Arch {
        family,
        subarch: "",
    };
    match name {
        "x86_64" | "amd64" => return Some(family("x86_64")),
        "i386" | "i486" | "i586" | "i686" | "x86" => return Some(family("x86")),
        "aarch64" | "arm64" => return Some(family("aarch64")),
        _ => {}
    }
    let (family_name, version) = if let Some(rest) = name.strip_prefix("thumb") {
        ("thumb", rest)
    } else {
        ("arm", name.strip_prefix("arm")?)
    };
    let subarch = match version {
        "" => "",
        "v7" | "v7a" => "v7",
        "v6" | "v6k" => "v6",
        "v8" | "v8a" => "v8",
        _ => return None,
    };
    Some(Arch {
        family: family_name,
        subarch,
    })
}

fn arch_matches(target: &str, name: &str) -> bool {
    let (Some(target), Some(name)) = (parse_arch(target), parse_arch(name)) else {
        return false;
    };
    let subarch_matches = name.subarch.is_empty() || name.subarch == target.subarch;
    subarch_matches
        && (name.family == target.family || (target.family == "thumb" && name.family == "arm"))
}

fn os_matches(target: &str, name: &str) -> bool {
    let canonical = |os: &str| match os {
        "darwin" | "macos" | "macosx" => Some("darwin"),
        "windows" | "win32" => Some("windows"),
        "linux" => Some("linux"),
        "freebsd" => Some("freebsd"),
        _ => None,
    };
    matches!((canonical(target), canonical(name)), (Some(target), Some(name)) if target == name)
}

const ENVIRONMENTS: &[&str] = &[
    "gnu",
    "gnueabi",
    "gnueabihf",
    "gnuabi64",
    "gnux32",
    "musl",
    "musleabi",
    "musleabihf",
    "android",
    "androideabi",
    "eabi",
    "eabihf",
    "msvc",
    "itanium",
    "cygnus",
    "coreclr",
    "simulator",
    "macabi",
];

fn environment_matches(target: &str, name: &str) -> bool {
    if name == "unknown" {
        return target.is_empty();
    }
    ENVIRONMENTS.contains(&name) && target == name
}
