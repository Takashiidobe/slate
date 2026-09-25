use crate::install::staging_path;
use crate::{Paths, Target};
use std::env;
use std::ffi::OsStr;
use std::fs;
use std::os::unix::fs::PermissionsExt;
use std::path::PathBuf;

struct TestDir(PathBuf);

impl TestDir {
    fn new() -> Self {
        let path = staging_path(&env::temp_dir(), "slate-sysroots-test").unwrap();
        fs::create_dir(&path).unwrap();
        Self(path)
    }

    fn script(&self, body: &str) -> PathBuf {
        let path = self.0.join("xwin");
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

#[test]
fn installs_both_windows_msvc_targets() {
    for (target, arch) in [
        (Target::X86_64PcWindowsMsvc, "x86_64"),
        (Target::Aarch64PcWindowsMsvc, "aarch64"),
    ] {
        let temp = TestDir::new();
        let xwin = temp.script(&format!(
            "#!/bin/sh\nset -eu\n[ \"$1\" = --arch ]\n[ \"$2\" = {arch} ]\n[ \"$3\" = --cache-dir ]\n[ \"$5\" = splat ]\n[ \"$6\" = --output ]\nmkdir -p \"$7/crt/include\" \"$7/sdk/include/ucrt\" \"$7/sdk/include/um\" \"$7/sdk/include/shared\" \"$7/sdk/include/winrt\" \"$7/sdk/include/cppwinrt\" \"$7/crt/lib/{arch}\"\ntouch \"$7/crt/include/vcruntime.h\" \"$7/crt/include/yvals_core.h\" \"$7/sdk/include/ucrt/stdio.h\" \"$7/sdk/include/ucrt/stdlib.h\" \"$7/sdk/include/um/windows.h\" \"$7/crt/lib/{arch}/msvcrt.lib\"\n"
        ));
        let paths = temp.paths();
        let installed = paths.install_with_xwin(target, xwin.as_os_str()).unwrap();

        assert_eq!(installed, paths.sysroot_path(target));
        assert_eq!(paths.resolve(target).unwrap(), installed);
        assert_eq!(paths.include_paths(target).unwrap().len(), 6);
        assert!(paths.doctor(target).iter().all(|check| check.present));
        assert!(installed.join("SYSROOT-MANIFEST.txt").is_file());
        assert!(paths.cache.join("xwin").is_dir());
        assert_eq!(
            paths
                .install_with_xwin(target, OsStr::new("missing-xwin"))
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
    let xwin = temp.script("#!/bin/sh\nexit 17\n");
    let paths = temp.paths();
    let target = Target::Aarch64PcWindowsMsvc;

    assert!(paths.install_with_xwin(target, xwin.as_os_str()).is_err());
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
        assert!(paths.install_with_xwin(target, OsStr::new("xwin")).is_err());
    }
}
