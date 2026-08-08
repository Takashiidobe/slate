use crate::{ArchPart, Kernel, Parsable, Triple};

pub fn get_host_triple() -> Triple {
    let arch = ArchPart::reduce(target_arch_str())
        .expect("host architecture is supported by triplers")
        .0;

    let kernel = Kernel::reduce(target_os_str())
        .expect("host OS is supported by triplers")
        .0;

    Triple {
        arch,
        vendor: None,
        kernel,
        env: None,
        obj: None,
    }
}

#[cfg(feature = "std")]
fn target_arch_str() -> &'static str {
    std::env::consts::ARCH
}

#[cfg(not(feature = "std"))]
fn target_arch_str() -> &'static str {
    if cfg!(target_arch = "x86") {
        "x86"
    } else if cfg!(target_arch = "x86_64") {
        "x86_64"
    } else if cfg!(target_arch = "arm") {
        "arm"
    } else if cfg!(target_arch = "aarch64") {
        "aarch64"
    } else if cfg!(target_arch = "mips") {
        "mips"
    } else if cfg!(target_arch = "mips64") {
        "mips64"
    } else if cfg!(target_arch = "powerpc") {
        "powerpc"
    } else if cfg!(target_arch = "powerpc64") {
        "powerpc64"
    } else if cfg!(target_arch = "wasm32") {
        "wasm32"
    } else if cfg!(target_arch = "wasm64") {
        "wasm64"
    } else {
        "unknown"
    }
}

#[cfg(feature = "std")]
fn target_os_str() -> &'static str {
    std::env::consts::OS
}

#[cfg(not(feature = "std"))]
fn target_os_str() -> &'static str {
    if cfg!(target_os = "linux") {
        "linux"
    } else if cfg!(target_os = "windows") {
        "windows"
    } else if cfg!(target_os = "macos") {
        "macos"
    } else if cfg!(target_os = "ios") {
        "ios"
    } else if cfg!(target_os = "tvos") {
        "tvos"
    } else if cfg!(target_os = "watchos") {
        "watchos"
    } else if cfg!(target_os = "freebsd") {
        "freebsd"
    } else if cfg!(target_os = "netbsd") {
        "netbsd"
    } else if cfg!(target_os = "openbsd") {
        "openbsd"
    } else if cfg!(target_os = "dragonfly") {
        "dragonfly"
    } else if cfg!(target_os = "android") {
        "android"
    } else if cfg!(target_os = "fuchsia") {
        "fuchsia"
    } else if cfg!(target_os = "hermit") {
        "hermit"
    } else {
        "none"
    }
}
