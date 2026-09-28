use crate::install::staging_path;
use crate::{CompilerHeaders, Paths, Target, windows_msvc};
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
            &["stdarg.h", "stddef.h", "immintrin.h"] as &[&str],
        ),
        (
            CompilerHeaders::Gcc,
            &[
                "stdarg.h",
                "stddef.h",
                "stdint.h",
                "limits.h",
                "syslimits.h",
                "unwind.h",
            ],
        ),
    ] {
        let include = paths.compiler_header_path(compiler);
        assert!(paths.resolve_compiler_headers(compiler).is_err());
        fs::create_dir_all(&include).unwrap();
        for name in files {
            fs::write(include.join(name), "").unwrap();
        }
        let licenses: &[&str] = match compiler {
            CompilerHeaders::Clang => &["LICENSE.TXT"],
            CompilerHeaders::AppleClang => unreachable!(),
            CompilerHeaders::Gcc => &["COPYING3", "COPYING.RUNTIME"],
            CompilerHeaders::Msvc(_) => unreachable!(),
        };
        for license in licenses {
            fs::write(include.parent().unwrap().join(license), "").unwrap();
        }
        assert!(paths.doctor_compiler_headers(compiler)[0].present);
        assert_eq!(paths.resolve_compiler_headers(compiler).unwrap(), include);
        assert_eq!(
            paths.include_paths_with_compiler(target, compiler).unwrap(),
            vec![include.clone(), libc.clone()]
        );
        fs::remove_file(include.join("stdarg.h")).unwrap();
        assert!(!paths.doctor_compiler_headers(compiler)[0].present);
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
    zip.start_file("android-ndk-r27d/source.properties", options)
        .unwrap();
    zip.write_all(b"Pkg.Revision = 27.3.13750724\n").unwrap();
    zip.start_file("android-ndk-r27d/NOTICE", options).unwrap();
    zip.write_all(b"license").unwrap();
    let prefix = "android-ndk-r27d/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include";
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
    assert!(
        !paths
            .data
            .join(format!("sysroots/{}.lock", target.triple()))
            .exists()
    );
    assert_eq!(
        fs::read_dir(paths.data.join("sysroots")).unwrap().count(),
        0
    );
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
