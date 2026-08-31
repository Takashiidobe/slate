mod support;

use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use support::libc_shim::compile_test_program_with_args;
use support::libc_shim::{
    Architecture, FREEBSD_VERSION_NUMBER, LibcVariant, TestConfig, compile_test_program,
};

fn bionic_headers() -> Vec<String> {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/bionic-basic-headers.txt");
    fs::read_to_string(manifest)
        .expect("read Bionic header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn bionic_stdio_locale_headers() -> Vec<String> {
    let manifest =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/bionic-stdio-locale-headers.txt");
    fs::read_to_string(manifest)
        .expect("read Bionic stdio/locale header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn msvc_headers() -> Vec<String> {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/msvc-basic-headers.txt");
    fs::read_to_string(manifest)
        .expect("read MSVC header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn macos_headers() -> Vec<String> {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/macos-basic-headers.txt");
    fs::read_to_string(manifest)
        .expect("read macOS header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn macos_stdio_locale_headers() -> Vec<String> {
    let manifest =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/macos-stdio-locale-headers.txt");
    fs::read_to_string(manifest)
        .expect("read macOS stdio/locale header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn freebsd_headers() -> Vec<String> {
    let manifest =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/freebsd-basic-headers.txt");
    fs::read_to_string(manifest)
        .expect("read FreeBSD header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn freebsd_stdio_locale_headers() -> Vec<String> {
    let manifest =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/freebsd-stdio-locale-headers.txt");
    fs::read_to_string(manifest)
        .expect("read FreeBSD stdio/locale header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn freebsd_filesystem_headers() -> Vec<String> {
    let manifest =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/freebsd-filesystem-headers.txt");
    fs::read_to_string(manifest)
        .expect("read FreeBSD filesystem header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn macos_filesystem_headers() -> Vec<String> {
    let manifest =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/macos-filesystem-headers.txt");
    fs::read_to_string(manifest)
        .expect("read macOS filesystem header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn macos_time_signal_headers() -> Vec<String> {
    let manifest =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/macos-time-signal-headers.txt");
    fs::read_to_string(manifest)
        .expect("read macOS time/signal header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn macos_pthread_headers() -> Vec<String> {
    let manifest =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/macos-pthread-headers.txt");
    fs::read_to_string(manifest)
        .expect("read macOS pthread header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn header_program(headers: &[String]) -> String {
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    format!(
        "{includes}\nint main(void) {{ assert(sizeof(wchar_t) == 2); return errno == 0 ? 0 : 0; }}\n"
    )
}

fn clang() -> PathBuf {
    std::env::var_os("SLATE_CLANG").map_or_else(
        || {
            PathBuf::from(std::env::var_os("HOME").unwrap_or_default())
                .join("llvm-project/build-cir/bin/clang")
        },
        PathBuf::from,
    )
}

fn xwin_include_dirs() -> Option<[PathBuf; 2]> {
    let root = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/msvc-sysroot");
    let crt = root.join("crt/include");
    let ucrt = root.join("sdk/include/ucrt");
    (crt.is_dir() && ucrt.is_dir()).then_some([crt, ucrt])
}

fn compile_xwin_header(header: &str, include_dirs: &[PathBuf; 2]) -> Result<(), String> {
    let cache = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/test-cache");
    fs::create_dir_all(&cache).map_err(|error| format!("create cache: {error}"))?;
    let stem = header.replace(['/', '.'], "_");
    let source = cache.join(format!("msvc_header_{stem}.c"));
    let object = cache.join(format!("msvc_header_{stem}.o"));
    fs::write(
        &source,
        format!("#include <{header}>\nint main(void) {{ return 0; }}\n"),
    )
    .map_err(|error| format!("write {}: {error}", source.display()))?;
    let output = Command::new(clang())
        .args([
            "-xc",
            "-c",
            "-nostdlibinc",
            "--target=x86_64-pc-windows-msvc",
        ])
        .arg("-isystem")
        .arg(&include_dirs[0])
        .arg("-isystem")
        .arg(&include_dirs[1])
        .arg("-o")
        .arg(object)
        .arg(source)
        .output()
        .map_err(|error| format!("spawn {}: {error}", clang().display()))?;
    output
        .status
        .success()
        .then_some(())
        .ok_or_else(|| String::from_utf8_lossy(&output.stderr).into_owned())
}

fn compile_macos_sdk_header(header: &str, sdk: &Path) -> Result<(), String> {
    let cache = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/test-cache");
    fs::create_dir_all(&cache).map_err(|error| format!("create cache: {error}"))?;
    let stem = header.replace(['/', '.'], "_");
    let source = cache.join(format!("macos_header_{stem}.c"));
    let object = cache.join(format!("macos_header_{stem}.o"));
    fs::write(
        &source,
        format!("#include <{header}>\nint main(void) {{ return 0; }}\n"),
    )
    .map_err(|error| format!("write {}: {error}", source.display()))?;
    let output = Command::new(clang())
        .args(["-xc", "-c", "--target=arm64-apple-macos11.0", "-isysroot"])
        .arg(sdk)
        .arg("-o")
        .arg(object)
        .arg(source)
        .output()
        .map_err(|error| format!("spawn {}: {error}", clang().display()))?;
    output
        .status
        .success()
        .then_some(())
        .ok_or_else(|| String::from_utf8_lossy(&output.stderr).into_owned())
}

#[test]
fn msvc_basic_header_manifest_compiles() {
    let headers = msvc_headers();
    for forbidden in ["pthread.h", "sys/ioctl.h", "sys/socket.h", "unistd.h"] {
        assert!(!headers.iter().any(|header| header == forbidden));
    }
    let config = TestConfig::new(Architecture::X86_64, LibcVariant::Msvc);
    compile_test_program(&config, &header_program(&headers)).unwrap();
}

#[test]
fn msvc_manifest_headers_compile_standalone() {
    let headers = msvc_headers();
    let config = TestConfig::new(Architecture::X86_64, LibcVariant::Msvc);
    let xwin = xwin_include_dirs();
    let failures = headers
        .iter()
        .filter_map(|header| {
            let source = format!("#include <{header}>\nint main(void) {{ return 0; }}\n");
            if let Err(error) = compile_test_program(&config, &source) {
                return Some(format!("{header} against shim:\n{error}"));
            }
            if let Some(include_dirs) = &xwin
                && let Err(error) = compile_xwin_header(header, include_dirs)
            {
                return Some(format!("{header} against xwin:\n{error}"));
            }
            None
        })
        .collect::<Vec<_>>();
    assert!(
        failures.is_empty(),
        "MSVC manifest headers failed standalone compilation:\n{}",
        failures.join("\n\n")
    );
}

#[test]
fn bionic_basic_header_manifest_compiles_for_64_bit_targets() {
    let headers = bionic_headers();
    for arch in [Architecture::Aarch64, Architecture::X86_64] {
        let config = TestConfig::new(arch, LibcVariant::Bionic);
        let includes = headers
            .iter()
            .map(|header| format!("#include <{header}>\n"))
            .collect::<String>();
        let source = format!(
            "{includes}\n_Static_assert(sizeof(wchar_t) == 4, \"wchar_t\");\n_Static_assert(__SLATE_ANDROID_API__ == 21, \"API\");\nint main(void) {{ return 0; }}\n"
        );
        compile_test_program(&config, &source).unwrap();
    }
}

#[test]
fn bionic_stdio_locale_header_manifest_compiles_for_64_bit_targets() {
    let headers = bionic_stdio_locale_headers();
    for arch in [Architecture::Aarch64, Architecture::X86_64] {
        let config = TestConfig::new(arch, LibcVariant::Bionic);
        let includes = headers
            .iter()
            .map(|header| format!("#include <{header}>\n"))
            .collect::<String>();
        let source = format!("{includes}\nint main(void) {{ return 0; }}\n");
        compile_test_program(&config, &source).unwrap();
    }
}

#[test]
fn freebsd_basic_header_manifest_compiles_for_x86_64_and_aarch64() {
    let headers = freebsd_headers();
    for forbidden in [
        "dirent.h",
        "pthread.h",
        "signal.h",
        "sys/socket.h",
        "sys/stat.h",
        "unistd.h",
    ] {
        assert!(!headers.iter().any(|header| header == forbidden));
    }
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!(
        "{includes}\n#include <sys/types.h>\n_Static_assert(sizeof(wchar_t) == 4, \"wchar_t\");\n_Static_assert(sizeof(long double) == 16, \"long double\");\n_Static_assert(sizeof(mode_t) == 2, \"mode_t\");\n_Static_assert(sizeof(dev_t) == 8, \"dev_t\");\n_Static_assert(sizeof(nlink_t) == 8, \"nlink_t\");\n_Static_assert(__SLATE_FREEBSD_VERSION__ == {FREEBSD_VERSION_NUMBER}, \"release\");\nint main(void) {{ return 0; }}\n"
    );
    for arch in [Architecture::X86_64, Architecture::Aarch64] {
        let config = TestConfig::new(arch, LibcVariant::FreeBsd);
        compile_test_program(&config, &source).unwrap();
    }
}

#[test]
fn freebsd_stdio_locale_header_manifest_compiles_for_x86_64_and_aarch64() {
    let headers = freebsd_stdio_locale_headers();
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!(
        "{includes}\n#include <stddef.h>\n_Static_assert(sizeof(FILE) == 312, \"FILE size\");\n_Static_assert(offsetof(FILE, _mbstate) == 176, \"FILE mbstate offset\");\n_Static_assert(LC_ALL == 0, \"LC_ALL\");\n_Static_assert(LC_MESSAGES == 6, \"LC_MESSAGES\");\n_Static_assert(LC_MONETARY_MASK == (1 << 2), \"LC_MONETARY_MASK\");\n_Static_assert(LC_MESSAGES_MASK == (1 << 5), \"LC_MESSAGES_MASK\");\n_Static_assert(CODESET == 0, \"CODESET\");\n_Static_assert(D_MD_ORDER == 57, \"D_MD_ORDER\");\nint main(void) {{ FILE *f = stdin; return f == stdout || f == stderr; }}\n"
    );
    for arch in [Architecture::X86_64, Architecture::Aarch64] {
        let config = TestConfig::new(arch, LibcVariant::FreeBsd);
        compile_test_program(&config, &source).unwrap();
    }
}

#[test]
fn freebsd_filesystem_header_manifest_compiles_for_x86_64_and_aarch64() {
    let headers = freebsd_filesystem_headers();
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!(
        "{includes}\n#include <stddef.h>\n\
         _Static_assert(sizeof(struct stat) == 224, \"stat size\");\n\
         _Static_assert(offsetof(struct stat, st_ino) == 8, \"st_ino offset\");\n\
         _Static_assert(offsetof(struct stat, st_mode) == 24, \"st_mode offset\");\n\
         _Static_assert(offsetof(struct stat, st_rdev) == 40, \"st_rdev offset\");\n\
         _Static_assert(offsetof(struct stat, st_atim) == 48, \"st_atim offset\");\n\
         _Static_assert(offsetof(struct stat, st_birthtim) == 96, \"st_birthtim offset\");\n\
         _Static_assert(offsetof(struct stat, st_size) == 112, \"st_size offset\");\n\
         _Static_assert(offsetof(struct stat, st_blksize) == 128, \"st_blksize offset\");\n\
         _Static_assert(offsetof(struct stat, st_gen) == 136, \"st_gen offset\");\n\
         _Static_assert(offsetof(struct stat, st_filerev) == 144, \"st_filerev offset\");\n\
         _Static_assert(offsetof(struct stat, st_spare) == 152, \"st_spare offset\");\n\
         _Static_assert(sizeof(((struct stat *)0)->st_blksize) == 4, \"st_blksize width\");\n\
         _Static_assert(sizeof(((struct stat *)0)->st_blocks) == 8, \"st_blocks width\");\n\
         _Static_assert(sizeof(struct dirent) == 280, \"dirent size\");\n\
         _Static_assert(offsetof(struct dirent, d_off) == 8, \"d_off offset\");\n\
         _Static_assert(offsetof(struct dirent, d_namlen) == 20, \"d_namlen offset\");\n\
         _Static_assert(offsetof(struct dirent, d_name) == 24, \"d_name offset\");\n\
         _Static_assert(O_CREAT == 0x0200, \"O_CREAT\");\n\
         _Static_assert(O_NOCTTY == 0x8000, \"O_NOCTTY\");\n\
         _Static_assert(O_DIRECTORY == 0x00020000, \"O_DIRECTORY\");\n\
         _Static_assert(O_CLOEXEC == 0x00100000, \"O_CLOEXEC\");\n\
         _Static_assert(AT_FDCWD == -100, \"AT_FDCWD\");\n\
         _Static_assert(F_DUPFD_CLOEXEC == 17, \"F_DUPFD_CLOEXEC\");\n\
         _Static_assert(F_GETLK == 11, \"F_GETLK\");\n\
         int main(void) {{ struct stat s; struct dirent d; (void)s; (void)d; return 0; }}\n"
    );
    for arch in [Architecture::X86_64, Architecture::Aarch64] {
        let config = TestConfig::new(arch, LibcVariant::FreeBsd);
        compile_test_program(&config, &source).unwrap();
    }
}

#[test]
fn macos_basic_header_manifest_compiles_for_aarch64() {
    let headers = macos_headers();
    for forbidden in [
        "dirent.h",
        "pthread.h",
        "signal.h",
        "sys/socket.h",
        "sys/stat.h",
        "unistd.h",
    ] {
        assert!(!headers.iter().any(|header| header == forbidden));
    }
    let config = TestConfig::new(Architecture::Aarch64, LibcVariant::Darwin);
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!(
        "{includes}\n_Static_assert(sizeof(wchar_t) == 4, \"wchar_t\");\n_Static_assert(sizeof(long double) == 8, \"long double\");\n_Static_assert(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ == 110000, \"deployment\");\nint main(void) {{ return 0; }}\n"
    );
    compile_test_program(&config, &source).unwrap();
}

#[test]
fn macos_stdio_locale_header_manifest_compiles_for_aarch64() {
    let headers = macos_stdio_locale_headers();
    let config = TestConfig::new(Architecture::Aarch64, LibcVariant::Darwin);
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!("{includes}\nint main(void) {{ return 0; }}\n");
    compile_test_program(&config, &source).unwrap();

    let sdk = std::env::var_os("SLATE_MACOS_SDK")
        .filter(|path| !path.is_empty())
        .map(PathBuf::from);
    let failures = headers
        .iter()
        .filter_map(|header| {
            let source = format!("#include <{header}>\nint main(void) {{ return 0; }}\n");
            if let Err(error) = compile_test_program(&config, &source) {
                return Some(format!("{header} against shim:\n{error}"));
            }
            if let Some(sdk) = &sdk
                && let Err(error) = compile_macos_sdk_header(header, sdk)
            {
                return Some(format!("{header} against macOS SDK:\n{error}"));
            }
            None
        })
        .collect::<Vec<_>>();
    assert!(
        failures.is_empty(),
        "macOS stdio/locale manifest headers failed standalone compilation:\n{}",
        failures.join("\n\n")
    );
}

#[test]
fn macos_filesystem_header_manifest_compiles_for_aarch64() {
    let headers = macos_filesystem_headers();
    let config = TestConfig::new(Architecture::Aarch64, LibcVariant::Darwin);
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!("{includes}\nint main(void) {{ return 0; }}\n");
    compile_test_program(&config, &source).unwrap();

    let sdk = std::env::var_os("SLATE_MACOS_SDK")
        .filter(|path| !path.is_empty())
        .map(PathBuf::from);
    let failures = headers
        .iter()
        .filter_map(|header| {
            let source = format!("#include <{header}>\nint main(void) {{ return 0; }}\n");
            if let Err(error) = compile_test_program(&config, &source) {
                return Some(format!("{header} against shim:\n{error}"));
            }
            if let Some(sdk) = &sdk
                && let Err(error) = compile_macos_sdk_header(header, sdk)
            {
                return Some(format!("{header} against macOS SDK:\n{error}"));
            }
            None
        })
        .collect::<Vec<_>>();
    assert!(
        failures.is_empty(),
        "macOS filesystem manifest headers failed standalone compilation:\n{}",
        failures.join("\n\n")
    );
}

#[test]
fn macos_time_signal_header_manifest_compiles_for_aarch64() {
    let headers = macos_time_signal_headers();
    assert!(!headers.iter().any(|header| header == "ucontext.h"));
    let config = TestConfig::new(Architecture::Aarch64, LibcVariant::Darwin);
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!("{includes}\nint main(void) {{ return 0; }}\n");
    compile_test_program(&config, &source).unwrap();

    let sdk = std::env::var_os("SLATE_MACOS_SDK")
        .filter(|path| !path.is_empty())
        .map(PathBuf::from);
    let failures = headers
        .iter()
        .filter_map(|header| {
            let source = format!("#include <{header}>\nint main(void) {{ return 0; }}\n");
            if let Err(error) = compile_test_program(&config, &source) {
                return Some(format!("{header} against shim:\n{error}"));
            }
            if let Some(sdk) = &sdk
                && let Err(error) = compile_macos_sdk_header(header, sdk)
            {
                return Some(format!("{header} against macOS SDK:\n{error}"));
            }
            None
        })
        .collect::<Vec<_>>();
    assert!(
        failures.is_empty(),
        "macOS time/signal manifest headers failed standalone compilation:\n{}",
        failures.join("\n\n")
    );

    let error = compile_test_program(&config, "#include <ucontext.h>\n").unwrap_err();
    assert!(error.contains("process-context interfaces are unavailable"));
}

#[test]
fn macos_pthread_header_manifest_compiles_for_aarch64() {
    let headers = macos_pthread_headers();
    let config = TestConfig::new(Architecture::Aarch64, LibcVariant::Darwin);
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!("{includes}\nint main(void) {{ return 0; }}\n");
    compile_test_program(&config, &source).unwrap();

    let sdk = std::env::var_os("SLATE_MACOS_SDK")
        .filter(|path| !path.is_empty())
        .map(PathBuf::from);
    let failures = headers
        .iter()
        .filter_map(|header| {
            let source = format!("#include <{header}>\nint main(void) {{ return 0; }}\n");
            if let Err(error) = compile_test_program(&config, &source) {
                return Some(format!("{header} against shim:\n{error}"));
            }
            if let Some(sdk) = &sdk
                && let Err(error) = compile_macos_sdk_header(header, sdk)
            {
                return Some(format!("{header} against macOS SDK:\n{error}"));
            }
            None
        })
        .collect::<Vec<_>>();
    assert!(
        failures.is_empty(),
        "macOS pthread manifest headers failed standalone compilation:\n{}",
        failures.join("\n\n")
    );

    let error = compile_test_program(&config, "#include <threads.h>\n").unwrap_err();
    assert!(error.contains("threads.h") && error.contains("unavailable"));
}

#[test]
fn macos_feature_modes_select_darwin_namespaces() {
    let fixture =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures/macos/feature_modes.c");
    let source = fs::read_to_string(fixture).expect("read macOS feature mode fixture");
    let config = TestConfig::new(Architecture::Aarch64, LibcVariant::Darwin);
    for args in [
        vec!["-DEXPECT_DARWIN_FULL"],
        vec!["-D_POSIX_C_SOURCE=200112L", "-DEXPECT_POSIX_2001"],
        vec!["-D_POSIX_C_SOURCE=200809L", "-DEXPECT_POSIX_2008"],
        vec![
            "-D_POSIX_C_SOURCE=200809L",
            "-D_DARWIN_C_SOURCE",
            "-DEXPECT_DARWIN_FULL",
        ],
    ] {
        compile_test_program_with_args(&config, &source, &args).unwrap();
    }
}

#[test]
fn macos_redirect_fact_fixture_compiles() {
    let fixture =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures/macos/redirect_facts.c");
    let source = fs::read_to_string(fixture).expect("read macOS redirect fixture");
    let config = TestConfig::new(Architecture::Aarch64, LibcVariant::Darwin);
    compile_test_program(&config, &source).unwrap();
}

#[test]
fn macos_sdk_oracle_probes_or_skips_explicitly() {
    let script = Path::new(env!("CARGO_MANIFEST_DIR")).join("tools/test-macos-sdk-oracle.sh");
    let output = Command::new("bash")
        .arg(script)
        .output()
        .expect("run macOS SDK oracle test");
    assert!(
        output.status.success(),
        "macOS SDK oracle failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    if std::env::var_os("SLATE_MACOS_SDK").is_none() && !Path::new("/usr/bin/xcrun").is_file() {
        assert!(String::from_utf8_lossy(&output.stdout).contains("SKIP macOS SDK oracle"));
    }
}

#[test]
fn macos_sdk_probe_fixture_compiles_against_shim() {
    let fixture = Path::new(env!("CARGO_MANIFEST_DIR")).join("tools/probes/macos-libc.c");
    let source = fs::read_to_string(fixture).expect("read macOS SDK probe fixture");
    let config = TestConfig::new(Architecture::Aarch64, LibcVariant::Darwin);
    compile_test_program(&config, &source).unwrap();
}

#[test]
fn msvc_linux_headers_are_rejected() {
    let config = TestConfig::new(Architecture::X86_64, LibcVariant::Msvc);
    for header in ["pthread.h", "sys/ioctl.h", "sys/socket.h", "unistd.h"] {
        let source = format!("#include <{header}>\n");
        let error = compile_test_program(&config, &source).unwrap_err();
        assert!(
            error.contains("is unavailable for the MSVC libc profile"),
            "unexpected diagnostic for {header}:\n{error}"
        );
    }
}
