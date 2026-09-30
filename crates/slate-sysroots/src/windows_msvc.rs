use crate::install::install_staged;
use crate::{DoctorCheck, Paths, Target};
use std::env;
use std::fs;
use std::io::{self, BufRead, Write};
use std::path::{Path, PathBuf};
use std::sync::Arc;
use xwin::manifest::PackageManifest;
use xwin::util::ProgressTarget;

const MANIFEST_VERSION: u8 = 18;
const LICENSE_URL: &str = "https://go.microsoft.com/fwlink/?LinkId=2086102";

pub(crate) struct Versions {
    pub(crate) crt: String,
    pub(crate) sdk: String,
}

pub(crate) fn install(paths: &Paths, target: Target) -> io::Result<PathBuf> {
    install_with(paths, target, |root| {
        accept_license()?;
        splat(&paths.cache.join("xwin"), root, target)
    })
}

pub(crate) fn install_with(
    paths: &Paths,
    target: Target,
    fetch: impl FnOnce(&Path) -> io::Result<Versions>,
) -> io::Result<PathBuf> {
    install_staged(
        paths,
        target,
        |root| validate(root, target),
        |root| {
            let versions = fetch(root)?;
            validate(root, target)?;
            fs::write(
                root.join("SYSROOT-MANIFEST.txt"),
                format!(
                    "Target: {}\nOperating system: Windows\nABI: MSVC\nSource: Microsoft CRT and Windows SDK, assembled by xwin\nVisual Studio manifest: {MANIFEST_VERSION}\nMSVC CRT: {}\nWindows SDK: {}\nUniversal CRT: from the Windows SDK\nContents: headers and libraries; local use only\n",
                    target.triple(),
                    versions.crt,
                    versions.sdk,
                ),
            )?;
            Ok(())
        },
    )
}

fn accept_license() -> io::Result<()> {
    if env::var_os("XWIN_ACCEPT_LICENSE").is_some() {
        return Ok(());
    }
    eprint!("Do you accept the license at {LICENSE_URL} (yes | no)? ");
    io::stderr().flush()?;
    let mut answer = String::new();
    io::stdin().lock().read_line(&mut answer)?;
    match answer.trim() {
        "yes" => Ok(()),
        _ => Err(io::Error::new(
            io::ErrorKind::PermissionDenied,
            format!("the Microsoft license at {LICENSE_URL} was not accepted"),
        )),
    }
}

fn splat(cache: &Path, root: &Path, target: Target) -> io::Result<Versions> {
    splat_xwin(cache, root, target).map_err(|error| io::Error::other(format!("xwin: {error:#}")))
}

fn splat_xwin(cache: &Path, root: &Path, target: Target) -> anyhow::Result<Versions> {
    let utf8 = |path: &Path| {
        xwin::PathBuf::from_path_buf(path.to_path_buf())
            .map_err(|path| anyhow::anyhow!("{} is not a valid utf-8 path", path.display()))
    };
    let tls = xwin::ureq::tls::TlsConfig::builder()
        .root_certs(xwin::ureq::tls::RootCerts::PlatformVerifier)
        .build();
    let client = xwin::ureq::config::Config::builder()
        .tls_config(tls)
        .build()
        .new_agent();
    let draw_target = ProgressTarget::Hidden;
    let ctx = Arc::new(xwin::Ctx::with_dir(utf8(cache)?, draw_target, client, 0)?);
    let progress = indicatif::ProgressBar::hidden();
    let manifest =
        xwin::manifest::get_manifest(&ctx, MANIFEST_VERSION, "release", progress.clone())?;
    let mut packages = xwin::manifest::get_package_manifest(&ctx, &manifest, progress)?;
    use_sdk_ucrt(&mut packages);
    let arch = xwin_arch(target) as u32;
    let variant = xwin::Variant::Desktop as u32;
    let (crt_version, sdk_version) = pinned_versions(target);
    let pruned = xwin::prune_pkg_list(
        &packages,
        arch,
        variant,
        false,
        false,
        Some(sdk_version.into()),
        Some(crt_version.into()),
    )?;
    let versions = Versions {
        crt: pruned.crt_version.clone(),
        sdk: pruned.sdk_version.clone(),
    };
    let work = pruned
        .payloads
        .into_iter()
        .map(|payload| xwin::WorkItem {
            payload: Arc::new(payload),
            progress: indicatif::ProgressBar::hidden(),
        })
        .collect();
    let splat = xwin::SplatConfig {
        include_debug_libs: false,
        include_debug_symbols: false,
        enable_symlinks: true,
        preserve_ms_arch_notation: false,
        use_winsysroot_style: false,
        output: utf8(root)?,
        map: None,
        copy: false,
    };
    ctx.execute(
        packages.packages,
        work,
        pruned.crt_version,
        pruned.sdk_version,
        pruned.vcr_version,
        arch,
        variant,
        xwin::Ops::Splat(splat),
    )?;
    Ok(versions)
}

fn use_sdk_ucrt(packages: &mut PackageManifest) {
    packages
        .packages
        .remove("Microsoft.Windows.UniversalCRT.HeadersLibsSources.Msi");
    for package in packages.packages.values_mut() {
        for payload in &mut package.payloads {
            payload.file_name = payload.file_name.replace('\\', "/");
        }
    }
}

pub(crate) fn include_paths(root: &Path) -> Vec<PathBuf> {
    [
        "crt/include",
        "sdk/include/ucrt",
        "sdk/include/shared",
        "sdk/include/um",
        "sdk/include/winrt",
        "sdk/include/cppwinrt",
    ]
    .into_iter()
    .map(|path| root.join(path))
    .collect()
}

pub(crate) fn validate(root: &Path, target: Target) -> io::Result<()> {
    if let Some(check) = doctor(root, target)
        .into_iter()
        .find(|check| !check.present)
    {
        return Err(io::Error::new(
            io::ErrorKind::NotFound,
            format!("missing {} at {}", check.label, check.path.display()),
        ));
    }
    Ok(())
}

pub(crate) fn doctor(root: &Path, target: Target) -> Vec<DoctorCheck> {
    let library_dir = format!("crt/lib/{}", arch(target));
    [
        (
            "MSVC compiler headers",
            "crt/include",
            &["vcruntime.h", "yvals_core.h"][..],
            &[][..],
        ),
        (
            "C runtime headers",
            "sdk/include/ucrt",
            &["stdio.h", "stdlib.h"][..],
            &[][..],
        ),
        (
            "Windows SDK headers",
            "sdk/include",
            &["um/windows.h"][..],
            &["shared", "winrt", "cppwinrt"][..],
        ),
        (
            "MSVC import libraries",
            library_dir.as_str(),
            &["msvcrt.lib"][..],
            &[][..],
        ),
    ]
    .into_iter()
    .map(|(label, relative, files, dirs)| {
        let path = root.join(relative);
        let present = path.is_dir()
            && files.iter().all(|name| path.join(name).is_file())
            && dirs.iter().all(|name| path.join(name).is_dir());
        DoctorCheck {
            label,
            path,
            present,
        }
    })
    .chain([ucrt_matches_crt(root)])
    .collect()
}

fn ucrt_matches_crt(root: &Path) -> DoctorCheck {
    let marker = "_UCRT_DISABLE_CLANG_WARNINGS";
    let mentions = |relative: &str, text: &str| {
        fs::read_to_string(root.join(relative)).is_ok_and(|contents| contents.contains(text))
    };
    let path = root.join("sdk/include/ucrt/corecrt.h");
    DoctorCheck {
        label: "Universal CRT as new as the MSVC CRT",
        present: path.is_file() && !mentions("crt/include/threads.h", marker)
            || mentions("sdk/include/ucrt/corecrt.h", &format!("#define {marker}")),
        path,
    }
}

fn pinned_versions(target: Target) -> (&'static str, &'static str) {
    match target {
        Target::Thumbv7aPcWindowsMsvc => ("14.44.17.14", "10.0.22621"),
        _ => ("14.51", "10.0.26100"),
    }
}

fn xwin_arch(target: Target) -> xwin::Arch {
    match target {
        Target::I686PcWindowsMsvc => xwin::Arch::X86,
        Target::X86_64PcWindowsMsvc => xwin::Arch::X86_64,
        Target::Aarch64PcWindowsMsvc => xwin::Arch::Aarch64,
        Target::Thumbv7aPcWindowsMsvc => xwin::Arch::Aarch,
        _ => unreachable!("Windows MSVC module received a non-Windows target"),
    }
}

fn arch(target: Target) -> &'static str {
    match target {
        Target::I686PcWindowsMsvc => "x86",
        Target::X86_64PcWindowsMsvc => "x86_64",
        Target::Aarch64PcWindowsMsvc => "aarch64",
        Target::Thumbv7aPcWindowsMsvc => "aarch",
        _ => unreachable!("Windows MSVC module received a non-Windows target"),
    }
}
