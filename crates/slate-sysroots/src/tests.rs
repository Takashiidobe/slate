use crate::install::staging_path;
use crate::{CompilerHeaders, GccFamily, Paths, Target, windows_msvc};
use std::env;
use std::fs;
use std::io::Write;
use std::os::unix::fs::PermissionsExt;
use std::path::{Path, PathBuf};

struct TestDir(PathBuf);

impl TestDir {
    fn new() -> Self {
        let path = staging_path(&env::temp_dir(), "slate-sysroots-test").unwrap();
        fs::create_dir(&path).unwrap();
        Self(path)
    }

    fn script_named(&self, name: &str, body: &str) -> PathBuf {
        let path = self.0.join(name);
        fs::write(&path, body).unwrap();
        fs::set_permissions(&path, fs::Permissions::from_mode(0o700)).unwrap();
        path
    }

    fn paths(&self) -> Paths {
        Paths {
            data: self.0.join("data"),
            cache: self.0.join("cache"),
        }
    }
}

impl Drop for TestDir {
    fn drop(&mut self) {
        let _ = fs::remove_dir_all(&self.0);
    }
}

fn fake_splat(root: &Path, arch: &str) {
    for dir in [
        "crt/include",
        "sdk/include/ucrt",
        "sdk/include/um",
        "sdk/include/shared",
        "sdk/include/winrt",
        "sdk/include/cppwinrt",
    ] {
        fs::create_dir_all(root.join(dir)).unwrap();
    }
    fs::create_dir_all(root.join(format!("crt/lib/{arch}"))).unwrap();
    for file in [
        "crt/include/vcruntime.h",
        "crt/include/yvals_core.h",
        "sdk/include/ucrt/stdio.h",
        "sdk/include/ucrt/stdlib.h",
        "sdk/include/ucrt/corecrt.h",
        "sdk/include/um/windows.h",
    ] {
        fs::write(root.join(file), "").unwrap();
    }
    fs::write(root.join(format!("crt/lib/{arch}/msvcrt.lib")), "").unwrap();
}

fn write_pinned_manifest(paths: &Paths, target: Target) {
    let root = paths.sysroot_path(target);
    fs::write(
        root.join("SYSROOT-MANIFEST.txt"),
        crate::pinned_manifest(target).unwrap(),
    )
    .unwrap();
}

fn versions() -> windows_msvc::Versions {
    windows_msvc::Versions {
        crt: "14.51.36244".into(),
        sdk: "10.0.26100".into(),
    }
}

#[test]
fn installs_windows_msvc_targets() {
    for (target, arch) in [
        (Target::I686PcWindowsMsvc, "x86"),
        (Target::X86_64PcWindowsMsvc, "x86_64"),
        (Target::Aarch64PcWindowsMsvc, "aarch64"),
        (Target::Thumbv7aPcWindowsMsvc, "aarch"),
    ] {
        let temp = TestDir::new();
        let paths = temp.paths();
        let installed = windows_msvc::install_with(&paths, target, |root| {
            fake_splat(root, arch);
            Ok(versions())
        })
        .unwrap();

        assert_eq!(installed, paths.sysroot_path(target));
        assert_eq!(paths.resolve(target).unwrap(), installed);
        assert_eq!(paths.include_paths(target).unwrap().len(), 6);
        assert_eq!(
            paths
                .install_compiler_headers(CompilerHeaders::Msvc(target))
                .unwrap(),
            installed.join("crt/include")
        );
        assert_eq!(
            paths
                .include_paths_with_compiler(target, CompilerHeaders::Msvc(target))
                .unwrap(),
            paths.include_paths(target).unwrap()
        );
        assert!(paths.doctor(target).iter().all(|check| check.present));
        let manifest = fs::read_to_string(installed.join("SYSROOT-MANIFEST.txt")).unwrap();
        assert!(manifest.contains("MSVC CRT: 14.51.36244"));
        assert!(manifest.contains("Windows SDK: 10.0.26100"));
        assert_eq!(
            windows_msvc::install_with(&paths, target, |_| Err(std::io::Error::other("refetched")))
                .unwrap(),
            installed
        );

        fs::remove_file(installed.join("sdk/include/ucrt/stdio.h")).unwrap();
        let checks = paths.doctor(target);
        assert!(checks[0].present);
        assert!(!checks[1].present);
        assert!(checks[2].present);
        assert!(checks[3].present);
        assert!(paths.resolve(target).is_err());
    }
}

#[test]
fn compiler_header_bundles_resolve_with_target_includes() {
    let temp = TestDir::new();
    let paths = temp.paths();
    let target = Target::X86_64UnknownLinuxGnu;
    let sysroot = paths.sysroot_path(target);
    let libc = sysroot.join("usr/x86_64-linux-gnu/include");
    let libs = sysroot.join("usr/x86_64-linux-gnu/lib");
    fs::create_dir_all(libc.join("linux")).unwrap();
    fs::create_dir_all(&libs).unwrap();
    for name in ["stdio.h", "stdlib.h", "linux/version.h"] {
        fs::write(libc.join(name), "").unwrap();
    }
    for name in ["libc.a", "crt1.o"] {
        fs::write(libs.join(name), "").unwrap();
    }

    for (compiler, files) in [
        (
            CompilerHeaders::Clang,
            crate::compiler_headers::CLANG_REQUIRED_HEADERS,
        ),
        (
            CompilerHeaders::Gcc(GccFamily::X86),
            &[
                "stdarg.h",
                "stddef.h",
                "stdint.h",
                "limits.h",
                "syslimits.h",
                "unwind.h",
                "immintrin.h",
                "cpuid.h",
                "mm_malloc.h",
            ],
        ),
    ] {
        let include = paths.compiler_header_path(compiler);
        assert!(paths.resolve_compiler_headers(compiler).is_err());
        fs::create_dir_all(&include).unwrap();
        for name in files {
            fs::write(include.join(name), "").unwrap();
        }
        let (licenses, license_dir): (&[&str], _) = match compiler {
            CompilerHeaders::Clang => (&["LICENSE.TXT"], include.parent().unwrap()),
            CompilerHeaders::Gcc(_) => (
                &["COPYING3", "COPYING.RUNTIME"],
                include.parent().unwrap().parent().unwrap(),
            ),
            CompilerHeaders::AppleClang | CompilerHeaders::Msvc(_) => unreachable!(),
        };
        for license in licenses {
            fs::write(license_dir.join(license), "").unwrap();
        }
        assert!(paths.doctor_compiler_headers(compiler)[0].present);
        assert_eq!(paths.resolve_compiler_headers(compiler).unwrap(), include);
        assert_eq!(
            paths.include_paths_with_compiler(target, compiler).unwrap(),
            vec![include.clone(), libc.clone()]
        );
        for name in files {
            fs::remove_file(include.join(name)).unwrap();
            assert!(!paths.doctor_compiler_headers(compiler)[0].present);
            fs::write(include.join(name), "").unwrap();
        }
    }

    assert!(
        paths
            .include_paths_with_compiler(
                target,
                CompilerHeaders::Msvc(Target::Aarch64PcWindowsMsvc)
            )
            .is_err()
    );
    assert!(
        paths
            .include_paths_with_compiler(target, CompilerHeaders::Gcc(GccFamily::Aarch64))
            .is_err()
    );
    assert!(
        paths
            .install_compiler_headers(CompilerHeaders::Msvc(target))
            .is_err()
    );
}

#[test]
fn apple_clang_headers_install_from_local_toolchain() {
    let temp = TestDir::new();
    let paths = temp.paths();
    let resource = temp.0.join("resource");
    fs::create_dir_all(resource.join("include")).unwrap();
    for name in ["stdarg.h", "stddef.h", "immintrin.h"] {
        fs::write(resource.join("include").join(name), "header").unwrap();
    }
    let clang = temp.script_named(
        "apple-clang-mock",
        &format!(
            "#!/bin/sh\ncase \"$1\" in\n  -print-resource-dir) printf '%s\\n' '{}' ;;\n  --version) printf '%s\\n' 'Apple clang version 17.0.0 (clang-1700.0.13.5)' ;;\n  *) exit 2 ;;\nesac\n",
            resource.display()
        ),
    );
    let xcrun = temp.script_named(
        "xcrun-apple-clang-mock",
        &format!(
            "#!/bin/sh\n[ \"$1\" = --find ]\n[ \"$2\" = clang ]\nprintf '%s\\n' '{}'\n",
            clang.display()
        ),
    );
    assert!(
        crate::compiler_headers::install_apple_clang(&paths, "linux", xcrun.as_os_str()).is_err()
    );
    let include =
        crate::compiler_headers::install_apple_clang(&paths, "macos", xcrun.as_os_str()).unwrap();
    assert_eq!(
        include,
        paths.compiler_header_path(CompilerHeaders::AppleClang)
    );
    assert!(paths.doctor_compiler_headers(CompilerHeaders::AppleClang)[0].present);
    assert_eq!(
        paths
            .resolve_compiler_headers(CompilerHeaders::AppleClang)
            .unwrap(),
        include
    );
    let bundle = paths
        .data
        .join("compiler-headers/apple-clang-17.0.0-clang-1700.0.13.5");
    assert!(bundle.join("include/stdarg.h").is_file());
    assert!(bundle.join("COMPILER-HEADERS-MANIFEST.txt").is_file());
    assert_eq!(
        crate::compiler_headers::install_apple_clang(&paths, "macos", xcrun.as_os_str()).unwrap(),
        include
    );
    assert!(
        paths
            .include_paths_with_compiler(Target::X86_64UnknownLinuxGnu, CompilerHeaders::AppleClang)
            .is_err()
    );
}

#[test]
fn darwin_targets_link_existing_sdk() {
    let temp = TestDir::new();
    let sdk = temp.0.join("MacOSX.sdk");
    fs::create_dir_all(sdk.join("usr/include/sys")).unwrap();
    fs::write(sdk.join("usr/include/stdio.h"), "").unwrap();
    fs::write(sdk.join("usr/include/sys/types.h"), "").unwrap();
    let paths = temp.paths();
    for target in [Target::X86_64AppleDarwin, Target::Aarch64AppleDarwin] {
        assert_eq!(target.triple().parse::<Target>().unwrap(), target);
        let root = paths.install_darwin_with_sdk(target, &sdk).unwrap();
        assert_eq!(paths.resolve(target).unwrap(), root);
        assert_eq!(
            paths.include_paths(target).unwrap(),
            vec![root.join("SDK/usr/include")]
        );
        assert!(paths.doctor(target).iter().all(|check| check.present));
        assert!(
            fs::symlink_metadata(root.join("SDK"))
                .unwrap()
                .file_type()
                .is_symlink()
        );
        assert_eq!(paths.install_darwin_with_sdk(target, &sdk).unwrap(), root);
    }
    fs::remove_file(sdk.join("usr/include/stdio.h")).unwrap();
    assert!(
        paths
            .doctor(Target::X86_64AppleDarwin)
            .iter()
            .any(|check| !check.present)
    );
    assert!(paths.resolve(Target::Aarch64AppleDarwin).is_err());
}

#[test]
fn darwin_sdk_discovery_reports_host_and_guides_mac_installation() {
    let temp = TestDir::new();
    let sdk = temp.0.join("MacOSX.sdk");
    let called = temp.0.join("xcrun-called");
    let xcrun = temp.script_named(
        "xcrun",
        &format!(
            "#!/bin/sh\nset -eu\ntouch '{}'\n[ \"$1\" = --sdk ]\n[ \"$2\" = macosx ]\n[ \"$3\" = --show-sdk-path ]\nprintf '%s\\n' '{}'\n",
            called.display(),
            sdk.display()
        ),
    );
    let error = crate::darwin::find_sdk("linux", None, xcrun.as_os_str()).unwrap_err();
    assert!(error.to_string().contains("unavailable on linux"));
    assert!(error.to_string().contains("--sdk <path>"));
    assert!(!called.exists());

    assert_eq!(
        crate::darwin::find_sdk("linux", Some(&sdk), xcrun.as_os_str()).unwrap(),
        sdk
    );
    assert!(!called.exists());

    assert_eq!(
        crate::darwin::find_sdk("macos", None, xcrun.as_os_str()).unwrap(),
        sdk
    );
    assert!(called.exists());

    let failed_xcrun = temp.script_named("xcrun-failed", "#!/bin/sh\nexit 17\n");
    let error = crate::darwin::find_sdk("macos", None, failed_xcrun.as_os_str()).unwrap_err();
    assert!(error.to_string().contains("xcode-select --install"));
}

#[test]
fn android_targets_copy_only_their_architecture_headers() {
    let temp = TestDir::new();
    let archive = temp.0.join("ndk.zip");
    let mut zip = zip::ZipWriter::new(fs::File::create(&archive).unwrap());
    let options =
        zip::write::SimpleFileOptions::default().compression_method(zip::CompressionMethod::Stored);
    zip.start_file("android-ndk-r30/source.properties", options)
        .unwrap();
    zip.write_all(b"Pkg.Revision = 30.0.16248370\n").unwrap();
    zip.start_file("android-ndk-r30/NOTICE", options).unwrap();
    zip.write_all(b"license").unwrap();
    let prefix = "android-ndk-r30/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include";
    for name in ["stdio.h", "stdlib.h"] {
        zip.start_file(format!("{prefix}/{name}"), options).unwrap();
        zip.write_all(b"header").unwrap();
    }
    for arch in [
        "x86_64-linux-android",
        "aarch64-linux-android",
        "i686-linux-android",
    ] {
        zip.start_file(format!("{prefix}/{arch}/asm/types.h"), options)
            .unwrap();
        zip.write_all(b"arch header").unwrap();
    }
    zip.finish().unwrap();
    let paths = temp.paths();
    for target in [Target::X86_64LinuxAndroid, Target::Aarch64LinuxAndroid] {
        assert_eq!(target.triple().parse::<Target>().unwrap(), target);
        let root = paths.sysroot_path(target);
        crate::android_bionic::extract_headers(&archive, target, &root).unwrap();
        write_pinned_manifest(&paths, target);
        assert_eq!(paths.resolve(target).unwrap(), root);
        assert!(paths.doctor(target).iter().all(|check| check.present));
        assert_eq!(
            paths.include_paths(target).unwrap(),
            vec![
                root.join("usr/include").join(target.triple()),
                root.join("usr/include")
            ]
        );
        assert!(!root.join("usr/include/i686-linux-android").exists());
        let other = if target == Target::X86_64LinuxAndroid {
            "aarch64-linux-android"
        } else {
            "x86_64-linux-android"
        };
        assert!(!root.join("usr/include").join(other).exists());
    }
}

#[test]
fn freebsd_targets_have_header_only_layout() {
    let temp = TestDir::new();
    let paths = temp.paths();
    for target in [Target::X86_64UnknownFreebsd, Target::Aarch64UnknownFreebsd] {
        assert_eq!(target.triple().parse::<Target>().unwrap(), target);
        let root = paths.sysroot_path(target);
        fs::create_dir_all(root.join("usr/include/sys")).unwrap();
        fs::write(root.join("usr/include/stdio.h"), "").unwrap();
        fs::write(root.join("usr/include/sys/types.h"), "").unwrap();
        fs::write(root.join("COPYRIGHT"), "").unwrap();
        write_pinned_manifest(&paths, target);
        assert_eq!(paths.resolve(target).unwrap(), root);
        assert_eq!(
            paths.include_paths(target).unwrap(),
            vec![root.join("usr/include")]
        );
        assert!(paths.doctor(target).iter().all(|check| check.present));
        fs::remove_file(root.join("COPYRIGHT")).unwrap();
        assert!(paths.resolve(target).is_err());
    }
}

#[test]
fn doctor_checks_target_specific_libraries() {
    let temp = TestDir::new();
    let paths = temp.paths();
    let root = paths.sysroot_path(Target::Aarch64PcWindowsMsvc);
    for dir in [
        "crt/include",
        "sdk/include/ucrt",
        "sdk/include/um",
        "sdk/include/shared",
        "sdk/include/winrt",
        "sdk/include/cppwinrt",
        "crt/lib/x86_64",
    ] {
        fs::create_dir_all(root.join(dir)).unwrap();
    }
    for file in [
        "crt/include/vcruntime.h",
        "crt/include/yvals_core.h",
        "sdk/include/ucrt/stdio.h",
        "sdk/include/ucrt/stdlib.h",
        "sdk/include/um/windows.h",
        "crt/lib/x86_64/msvcrt.lib",
    ] {
        fs::write(root.join(file), "").unwrap();
    }

    let checks = paths.doctor(Target::Aarch64PcWindowsMsvc);
    assert!(checks[..3].iter().all(|check| check.present));
    assert!(!checks[3].present);
    assert_eq!(checks[3].path, root.join("crt/lib/aarch64"));
}

#[test]
fn failed_xwin_leaves_no_installation() {
    let temp = TestDir::new();
    let paths = temp.paths();
    let target = Target::Aarch64PcWindowsMsvc;

    assert!(
        windows_msvc::install_with(&paths, target, |_| Err(std::io::Error::other("xwin"))).is_err()
    );
    assert!(paths.doctor(target).iter().all(|check| !check.present));
    assert!(!paths.sysroot_path(target).exists());
    assert_eq!(
        fs::read_dir(paths.data.join("sysroots"))
            .unwrap()
            .map(|entry| entry.unwrap().file_name())
            .collect::<Vec<_>>(),
        vec![std::ffi::OsString::from(format!(
            "{}.lock",
            target.triple()
        ))]
    );
    windows_msvc::install_with(&paths, target, |root| {
        fake_splat(root, "aarch64");
        Ok(versions())
    })
    .unwrap();
}

#[test]
fn linux_targets_resolve_prebuilt_header_layouts() {
    let temp = TestDir::new();
    let paths = temp.paths();
    for (target, include, library) in [
        (
            Target::X86_64UnknownLinuxGnu,
            "usr/x86_64-linux-gnu/include",
            "usr/x86_64-linux-gnu/lib",
        ),
        (
            Target::Aarch64UnknownLinuxGnu,
            "usr/aarch64-linux-gnu/include",
            "usr/aarch64-linux-gnu/lib",
        ),
        (
            Target::I686UnknownLinuxGnu,
            "usr/i686-linux-gnu/include",
            "usr/i686-linux-gnu/lib",
        ),
        (
            Target::Armv7UnknownLinuxGnueabi,
            "usr/arm-linux-gnueabi/include",
            "usr/arm-linux-gnueabi/lib",
        ),
        (
            Target::Armv7UnknownLinuxGnueabihf,
            "usr/arm-linux-gnueabihf/include",
            "usr/arm-linux-gnueabihf/lib",
        ),
        (Target::X86_64UnknownLinuxMusl, "usr/include", "usr/lib"),
        (Target::Aarch64UnknownLinuxMusl, "usr/include", "usr/lib"),
    ] {
        assert_eq!(target.triple().parse::<Target>().unwrap(), target);
        let root = paths.sysroot_path(target);
        fs::create_dir_all(root.join(include).join("linux")).unwrap();
        fs::create_dir_all(root.join(library)).unwrap();
        for name in ["stdio.h", "stdlib.h", "linux/version.h"] {
            fs::write(root.join(include).join(name), "").unwrap();
        }
        for name in ["libc.a", "crt1.o"] {
            fs::write(root.join(library).join(name), "").unwrap();
        }
        if target == Target::X86_64UnknownLinuxMusl || target == Target::Aarch64UnknownLinuxMusl {
            let arch = if target == Target::X86_64UnknownLinuxMusl {
                "x86_64"
            } else {
                "aarch64"
            };
            fs::create_dir_all(root.join("lib")).unwrap();
            fs::write(root.join(format!("lib/ld-musl-{arch}.so.1")), "").unwrap();
        }
        write_pinned_manifest(&paths, target);

        assert!(paths.doctor(target).iter().all(|check| check.present));
        assert_eq!(paths.resolve(target).unwrap(), root);
        assert_eq!(
            paths.include_paths(target).unwrap(),
            vec![root.join(include)]
        );
    }
}

#[test]
fn doctor_rejects_ucrt_older_than_crt() {
    let temp = TestDir::new();
    let paths = temp.paths();
    let target = Target::X86_64PcWindowsMsvc;
    let root = paths.sysroot_path(target);
    fake_splat(&root, "x86_64");
    assert!(paths.doctor(target).iter().all(|check| check.present));

    fs::write(
        root.join("crt/include/threads.h"),
        "_UCRT_DISABLE_CLANG_WARNINGS\n",
    )
    .unwrap();
    let stale = paths.doctor(target);
    assert!(!stale[4].present);
    assert_eq!(stale[4].path, root.join("sdk/include/ucrt/corecrt.h"));
    assert!(paths.resolve(target).is_err());

    fs::write(
        root.join("sdk/include/ucrt/corecrt.h"),
        "#define _UCRT_DISABLE_CLANG_WARNINGS\n",
    )
    .unwrap();
    assert!(paths.doctor(target).iter().all(|check| check.present));
}

#[test]
fn doctor_reports_sysroot_older_than_pinned_versions() {
    let temp = TestDir::new();
    let paths = temp.paths();
    let target = Target::X86_64UnknownFreebsd;
    let root = paths.sysroot_path(target);
    fs::create_dir_all(root.join("usr/include/sys")).unwrap();
    fs::write(root.join("usr/include/stdio.h"), "").unwrap();
    fs::write(root.join("usr/include/sys/types.h"), "").unwrap();
    fs::write(root.join("COPYRIGHT"), "").unwrap();

    fs::write(root.join("SYSROOT-MANIFEST.txt"), "Target: old pin\n").unwrap();
    let checks = paths.doctor(target);
    let failing: Vec<_> = checks.iter().filter(|check| !check.present).collect();
    assert_eq!(failing.len(), 1);
    assert!(failing[0].label.contains("out of date"));

    write_pinned_manifest(&paths, target);
    assert!(paths.doctor(target).iter().all(|check| check.present));
}

#[test]
fn install_replaces_sysroot_whose_manifest_is_out_of_date() {
    let temp = TestDir::new();
    let output = temp.0.join("data/sysroots/target");
    fs::create_dir_all(&output).unwrap();
    fs::write(output.join("SYSROOT-MANIFEST.txt"), "old").unwrap();
    fs::write(output.join("old-only.h"), "").unwrap();

    let installed = crate::install::install_pinned_at(
        &output,
        Some("new"),
        |_| Ok(()),
        |root| {
            fs::create_dir(root)?;
            fs::write(root.join("SYSROOT-MANIFEST.txt"), "new")?;
            fs::write(root.join("new-only.h"), "")
        },
    )
    .unwrap();

    assert_eq!(installed, output);
    assert_eq!(
        fs::read_to_string(output.join("SYSROOT-MANIFEST.txt")).unwrap(),
        "new"
    );
    assert!(output.join("new-only.h").exists());
    assert!(!output.join("old-only.h").exists());
    assert_eq!(
        fs::read_dir(output.parent().unwrap())
            .unwrap()
            .filter_map(Result::ok)
            .filter(|entry| entry.file_name().to_string_lossy().starts_with('.'))
            .count(),
        0
    );
}

#[test]
fn install_keeps_sysroot_whose_manifest_is_current() {
    let temp = TestDir::new();
    let output = temp.0.join("data/sysroots/target");
    fs::create_dir_all(&output).unwrap();
    fs::write(output.join("SYSROOT-MANIFEST.txt"), "same").unwrap();

    crate::install::install_pinned_at(
        &output,
        Some("same"),
        |_| Ok(()),
        |_| Err(std::io::Error::other("rebuilt a current sysroot")),
    )
    .unwrap();
}

#[test]
fn failed_rebuild_keeps_the_previous_sysroot() {
    let temp = TestDir::new();
    let output = temp.0.join("data/sysroots/target");
    fs::create_dir_all(&output).unwrap();
    fs::write(output.join("SYSROOT-MANIFEST.txt"), "old").unwrap();

    let result = crate::install::install_pinned_at(
        &output,
        Some("new"),
        |_| Ok(()),
        |_| Err(std::io::Error::other("download failed")),
    );

    assert!(result.is_err());
    assert_eq!(
        fs::read_to_string(output.join("SYSROOT-MANIFEST.txt")).unwrap(),
        "old"
    );
}

#[test]
fn clang_package_extracts_built_headers_and_license_only() {
    let temp = TestDir::new();
    let mut tar = tar::Builder::new(Vec::new());
    for (name, contents) in [
        (
            "./usr/lib/llvm-22/lib/clang/22/include/arm_neon.h",
            "generated neon",
        ),
        (
            "./usr/lib/llvm-22/lib/clang/22/include/arm_sve.h",
            "generated sve",
        ),
        (
            "./usr/lib/llvm-22/lib/clang/22/include/subdir/header.h",
            "nested",
        ),
        (
            "./usr/share/doc/libclang-common-22-dev/copyright",
            "license",
        ),
        ("./usr/bin/unwanted", "executable"),
    ] {
        let mut header = tar::Header::new_gnu();
        header.set_mode(0o644);
        header.set_size(contents.len() as u64);
        header.set_cksum();
        tar.append_data(&mut header, name, contents.as_bytes())
            .unwrap();
    }
    let mut compressed = xz2::write::XzEncoder::new(Vec::new(), 6);
    compressed.write_all(&tar.into_inner().unwrap()).unwrap();
    let compressed = compressed.finish().unwrap();
    let package = temp.0.join("headers.deb");
    let mut ar = ar::Builder::new(fs::File::create(&package).unwrap());
    ar.append(
        &ar::Header::new(b"data.tar.xz".to_vec(), compressed.len() as u64),
        compressed.as_slice(),
    )
    .unwrap();
    drop(ar);
    let root = temp.0.join("root");
    crate::compiler_headers::extract_clang_headers(&package, &root).unwrap();
    assert_eq!(
        fs::read_to_string(root.join("include/arm_neon.h")).unwrap(),
        "generated neon"
    );
    assert_eq!(
        fs::read_to_string(root.join("include/arm_sve.h")).unwrap(),
        "generated sve"
    );
    assert_eq!(
        fs::read_to_string(root.join("include/subdir/header.h")).unwrap(),
        "nested"
    );
    assert_eq!(
        fs::read_to_string(root.join("LICENSE.TXT")).unwrap(),
        "license"
    );
    assert!(!root.join("usr").exists());
}

#[test]
fn installation_lock_child() {
    let Some(base) = env::var_os("SLATE_SYSROOT_LOCK_TEST") else {
        return;
    };
    let base = PathBuf::from(base);
    let paths = Paths {
        data: base.join("data"),
        cache: base.join("cache"),
    };
    crate::install::install_staged(
        &paths,
        Target::X86_64UnknownLinuxGnu,
        Some("new"),
        |_| Ok(()),
        |_| {
            fs::write(base.join("ready"), "")?;
            let mut input = String::new();
            std::io::stdin().read_line(&mut input)?;
            Err(std::io::Error::other("child should be killed"))
        },
    )
    .unwrap();
}

#[test]
fn interrupted_installer_releases_lock_for_install_and_remove() {
    struct Child(std::process::Child);
    impl Drop for Child {
        fn drop(&mut self) {
            let _ = self.0.kill();
            let _ = self.0.wait();
        }
    }
    let temp = TestDir::new();
    let paths = temp.paths();
    let target = Target::X86_64UnknownLinuxGnu;
    let root = paths.sysroot_path(target);
    fs::create_dir_all(&root).unwrap();
    fs::write(root.join("SYSROOT-MANIFEST.txt"), "old").unwrap();
    let lock = root.with_extension("lock");
    fs::write(&lock, "stale pid").unwrap();
    let mut child = Child(
        std::process::Command::new(env::current_exe().unwrap())
            .args(["--exact", "tests::installation_lock_child", "--nocapture"])
            .env("SLATE_SYSROOT_LOCK_TEST", &temp.0)
            .stdin(std::process::Stdio::piped())
            .stdout(std::process::Stdio::null())
            .spawn()
            .unwrap(),
    );
    let deadline = std::time::Instant::now() + std::time::Duration::from_secs(10);
    while !temp.0.join("ready").exists() {
        assert!(
            child.0.try_wait().unwrap().is_none(),
            "child exited before acquiring lock"
        );
        assert!(
            std::time::Instant::now() < deadline,
            "child did not acquire lock"
        );
        std::thread::sleep(std::time::Duration::from_millis(10));
    }
    let error = crate::install::install_staged(
        &paths,
        target,
        Some("new"),
        |_| Ok(()),
        |_| panic!("concurrent installer entered build"),
    )
    .unwrap_err();
    assert_eq!(error.kind(), std::io::ErrorKind::WouldBlock);
    assert!(
        error
            .to_string()
            .contains("installation already in progress")
    );
    assert_eq!(
        paths.remove(target).unwrap_err().kind(),
        std::io::ErrorKind::WouldBlock
    );
    child.0.kill().unwrap();
    child.0.wait().unwrap();
    assert!(lock.is_file());
    crate::install::install_staged(
        &paths,
        target,
        Some("new"),
        |_| Ok(()),
        |root| {
            fs::create_dir(root)?;
            fs::write(root.join("SYSROOT-MANIFEST.txt"), "new")
        },
    )
    .unwrap();
    assert_eq!(
        fs::read_to_string(root.join("SYSROOT-MANIFEST.txt")).unwrap(),
        "new"
    );
    paths.remove(target).unwrap();
    assert!(!root.exists());
    assert!(lock.is_file());
}

#[test]
fn darwin_sdk_registration_replaces_changed_and_missing_sdks() {
    let temp = TestDir::new();
    let paths = temp.paths();
    let sdks: Vec<_> = ["MacOSX-A.sdk", "MacOSX-B.sdk", "MacOSX-C.sdk"]
        .into_iter()
        .map(|name| {
            let sdk = temp.0.join(name);
            fs::create_dir_all(sdk.join("usr/include/sys")).unwrap();
            fs::write(sdk.join("usr/include/stdio.h"), name).unwrap();
            fs::write(sdk.join("usr/include/sys/types.h"), "").unwrap();
            fs::canonicalize(sdk).unwrap()
        })
        .collect();
    for target in [Target::X86_64AppleDarwin, Target::Aarch64AppleDarwin] {
        let root = paths.install_darwin_with_sdk(target, &sdks[0]).unwrap();
        fs::write(root.join("keep-on-repeat"), "").unwrap();
        paths
            .install_darwin_with_sdk(target, &sdks[0].join("."))
            .unwrap();
        assert!(root.join("keep-on-repeat").is_file());
        paths.install_darwin_with_sdk(target, &sdks[1]).unwrap();
        assert_eq!(fs::read_link(root.join("SDK")).unwrap(), sdks[1]);
        assert!(!root.join("keep-on-repeat").exists());
        assert!(
            fs::read_to_string(root.join("SYSROOT-MANIFEST.txt"))
                .unwrap()
                .contains(&format!("SDK path: {}\n", sdks[1].display()))
        );
        let invalid = temp.0.join("invalid.sdk");
        fs::create_dir_all(invalid.join("usr/include")).unwrap();
        fs::write(invalid.join("usr/include/stdio.h"), "").unwrap();
        assert!(paths.install_darwin_with_sdk(target, &invalid).is_err());
        assert_eq!(fs::read_link(root.join("SDK")).unwrap(), sdks[1]);
        assert!(paths.doctor(target).iter().all(|check| check.present));
    }
    fs::remove_dir_all(&sdks[1]).unwrap();
    for target in [Target::X86_64AppleDarwin, Target::Aarch64AppleDarwin] {
        assert!(paths.resolve(target).is_err());
        let root = paths.install_darwin_with_sdk(target, &sdks[2]).unwrap();
        assert_eq!(fs::read_link(root.join("SDK")).unwrap(), sdks[2]);
        assert!(paths.doctor(target).iter().all(|check| check.present));
    }
}
