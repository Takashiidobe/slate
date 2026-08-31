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

fn freebsd_time_signal_headers() -> Vec<String> {
    let manifest =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/freebsd-time-signal-headers.txt");
    fs::read_to_string(manifest)
        .expect("read FreeBSD time/signal header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn freebsd_pthread_headers() -> Vec<String> {
    let manifest =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/freebsd-pthread-headers.txt");
    fs::read_to_string(manifest)
        .expect("read FreeBSD pthread header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn freebsd_net_headers() -> Vec<String> {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/freebsd-net-headers.txt");
    fs::read_to_string(manifest)
        .expect("read FreeBSD net header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn freebsd_process_headers() -> Vec<String> {
    let manifest =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/freebsd-process-headers.txt");
    fs::read_to_string(manifest)
        .expect("read FreeBSD process header manifest")
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
fn freebsd_time_signal_header_manifest_compiles_for_x86_64_and_aarch64() {
    let headers = freebsd_time_signal_headers();
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!(
        "{includes}\n#include <stddef.h>\n\
         _Static_assert(CLOCKS_PER_SEC == 128, \"CLOCKS_PER_SEC\");\n\
         _Static_assert(CLOCK_REALTIME == 0, \"CLOCK_REALTIME\");\n\
         _Static_assert(CLOCK_MONOTONIC == 4, \"CLOCK_MONOTONIC\");\n\
         _Static_assert(CLOCK_UPTIME == 5, \"CLOCK_UPTIME\");\n\
         _Static_assert(CLOCK_UPTIME_PRECISE == 7, \"CLOCK_UPTIME_PRECISE\");\n\
         _Static_assert(CLOCK_UPTIME_FAST == 8, \"CLOCK_UPTIME_FAST\");\n\
         _Static_assert(CLOCK_SECOND == 13, \"CLOCK_SECOND\");\n\
         _Static_assert(CLOCK_THREAD_CPUTIME_ID == 14, \"CLOCK_THREAD_CPUTIME_ID\");\n\
         _Static_assert(CLOCK_PROCESS_CPUTIME_ID == 15, \"CLOCK_PROCESS_CPUTIME_ID\");\n\
         _Static_assert(SIGHUP == 1, \"SIGHUP\");\n\
         _Static_assert(SIGKILL == 9, \"SIGKILL\");\n\
         _Static_assert(SIGBUS == 10, \"SIGBUS\");\n\
         _Static_assert(SIGUSR1 == 30, \"SIGUSR1\");\n\
         _Static_assert(SIGUSR2 == 31, \"SIGUSR2\");\n\
         _Static_assert(SIGRTMIN == 65, \"SIGRTMIN\");\n\
         _Static_assert(SIGRTMAX == 126, \"SIGRTMAX\");\n\
         _Static_assert(sizeof(sigset_t) == 16, \"sigset_t size\");\n\
         _Static_assert(sizeof(siginfo_t) == 80, \"siginfo_t size\");\n\
         _Static_assert(offsetof(siginfo_t, si_pid) == 12, \"si_pid offset\");\n\
         _Static_assert(offsetof(siginfo_t, si_uid) == 16, \"si_uid offset\");\n\
         _Static_assert(offsetof(siginfo_t, si_addr) == 24, \"si_addr offset\");\n\
         _Static_assert(offsetof(siginfo_t, si_value) == 32, \"si_value offset\");\n\
         _Static_assert(sizeof(struct sigaction) == 32, \"sigaction size\");\n\
         _Static_assert(offsetof(struct sigaction, sa_flags) == 8, \"sa_flags offset\");\n\
         _Static_assert(offsetof(struct sigaction, sa_mask) == 12, \"sa_mask offset\");\n\
         _Static_assert(ILL_ILLOPN == 2, \"ILL_ILLOPN\");\n\
         _Static_assert(ILL_ILLTRP == 4, \"ILL_ILLTRP\");\n\
         int main(void) {{ sigset_t s; struct sigaction sa; siginfo_t si; (void)s; (void)sa; (void)si; return 0; }}\n"
    );
    for arch in [Architecture::X86_64, Architecture::Aarch64] {
        let config = TestConfig::new(arch, LibcVariant::FreeBsd);
        compile_test_program(&config, &source).unwrap();
    }
}

#[test]
fn freebsd_net_header_manifest_compiles_for_x86_64_and_aarch64() {
    let headers = freebsd_net_headers();
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!(
        "{includes}\n#include <stddef.h>\n\
         _Static_assert(sizeof(struct sockaddr) == 16, \"sockaddr size\");\n\
         _Static_assert(offsetof(struct sockaddr, sa_family) == 1, \"sa_family offset\");\n\
         _Static_assert(offsetof(struct sockaddr, sa_data) == 2, \"sa_data offset\");\n\
         _Static_assert(sizeof(struct sockaddr_storage) == 128, \"sockaddr_storage size\");\n\
         _Static_assert(sizeof(struct sockaddr_in) == 16, \"sockaddr_in size\");\n\
         _Static_assert(offsetof(struct sockaddr_in, sin_family) == 1, \"sin_family offset\");\n\
         _Static_assert(offsetof(struct sockaddr_in, sin_port) == 2, \"sin_port offset\");\n\
         _Static_assert(offsetof(struct sockaddr_in, sin_addr) == 4, \"sin_addr offset\");\n\
         _Static_assert(sizeof(struct sockaddr_in6) == 28, \"sockaddr_in6 size\");\n\
         _Static_assert(offsetof(struct sockaddr_in6, sin6_addr) == 8, \"sin6_addr offset\");\n\
         _Static_assert(sizeof(struct sockaddr_un) == 106, \"sockaddr_un size\");\n\
         _Static_assert(offsetof(struct sockaddr_un, sun_path) == 2, \"sun_path offset\");\n\
         _Static_assert(AF_INET6 == 28, \"AF_INET6\");\n\
         _Static_assert(AF_INET == 2, \"AF_INET\");\n\
         _Static_assert(AF_UNIX == 1, \"AF_UNIX\");\n\
         _Static_assert(SOL_SOCKET == 0xffff, \"SOL_SOCKET\");\n\
         _Static_assert(SO_REUSEADDR == 0x0004, \"SO_REUSEADDR\");\n\
         _Static_assert(SO_ERROR == 0x1007, \"SO_ERROR\");\n\
         _Static_assert(SOCK_STREAM == 1, \"SOCK_STREAM\");\n\
         _Static_assert(SOCK_DGRAM == 2, \"SOCK_DGRAM\");\n\
         _Static_assert(IPPROTO_TCP == 6, \"IPPROTO_TCP\");\n\
         _Static_assert(IPPROTO_UDP == 17, \"IPPROTO_UDP\");\n\
         _Static_assert(INADDR_LOOPBACK == 0x7f000001, \"INADDR_LOOPBACK\");\n\
         _Static_assert(sizeof(struct addrinfo) == 48, \"addrinfo size\");\n\
         _Static_assert(offsetof(struct addrinfo, ai_canonname) == 24, \"ai_canonname offset\");\n\
         _Static_assert(offsetof(struct addrinfo, ai_addr) == 32, \"ai_addr offset\");\n\
         _Static_assert(EAI_FAMILY == 5, \"EAI_FAMILY\");\n\
         _Static_assert(sizeof(struct ifaddrs) == 56, \"ifaddrs size\");\n\
         int main(void) {{ struct sockaddr sa; struct sockaddr_in sin; struct sockaddr_in6 sin6; struct sockaddr_un sun; struct addrinfo ai; struct ifaddrs ifa; (void)sa; (void)sin; (void)sin6; (void)sun; (void)ai; (void)ifa; return 0; }}\n"
    );
    for arch in [Architecture::X86_64, Architecture::Aarch64] {
        let config = TestConfig::new(arch, LibcVariant::FreeBsd);
        compile_test_program(&config, &source).unwrap();
    }
}

#[test]
fn freebsd_process_header_manifest_compiles_for_x86_64_and_aarch64() {
    let headers = freebsd_process_headers();
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!(
        "#define _BSD_SOURCE\n{includes}\n#include <stddef.h>\n\
         _Static_assert(sizeof(nfds_t) == 4, \"nfds_t size\");\n\
         _Static_assert(POLLIN == 0x0001, \"POLLIN\");\n\
         _Static_assert(POLLWRBAND == 0x0100, \"POLLWRBAND\");\n\
         _Static_assert(sizeof(struct termios) == 44, \"termios size\");\n\
         _Static_assert(offsetof(struct termios, c_cc) == 16, \"c_cc offset\");\n\
         _Static_assert(offsetof(struct termios, c_ispeed) == 36, \"c_ispeed offset\");\n\
         _Static_assert(VINTR == 8, \"VINTR\");\n\
         _Static_assert(TCSANOW == 0, \"TCSANOW\");\n\
         _Static_assert(TCIFLUSH == 1, \"TCIFLUSH\");\n\
         _Static_assert(sizeof(struct passwd) == 80, \"passwd size\");\n\
         _Static_assert(offsetof(struct passwd, pw_change) == 24, \"pw_change offset\");\n\
         _Static_assert(offsetof(struct passwd, pw_gecos) == 40, \"pw_gecos offset\");\n\
         _Static_assert(sizeof(posix_spawnattr_t) == 8, \"posix_spawnattr_t size\");\n\
         _Static_assert(POSIX_SPAWN_SETSIGDEF == 0x10, \"POSIX_SPAWN_SETSIGDEF\");\n\
         _Static_assert(WNOHANG == 1, \"WNOHANG\");\n\
         _Static_assert(WCONTINUED == 4, \"WCONTINUED\");\n\
         _Static_assert(P_PID == 0, \"P_PID\");\n\
         _Static_assert(P_ALL == 7, \"P_ALL\");\n\
         _Static_assert((long)RTLD_DEFAULT == -2, \"RTLD_DEFAULT\");\n\
         _Static_assert(RTLD_NOLOAD == 0x02000, \"RTLD_NOLOAD\");\n\
         _Static_assert(sizeof(regex_t) == 32, \"regex_t size\");\n\
         _Static_assert(REG_NEWLINE == 0010, \"REG_NEWLINE\");\n\
         _Static_assert(GLOB_APPEND == 0x0001, \"GLOB_APPEND\");\n\
         _Static_assert(GLOB_NOSPACE == -1, \"GLOB_NOSPACE\");\n\
         _Static_assert(FNM_PATHNAME == 0x02, \"FNM_PATHNAME\");\n\
         _Static_assert(FNM_NOESCAPE == 0x01, \"FNM_NOESCAPE\");\n\
         _Static_assert(sizeof(struct kevent) == 64, \"kevent size\");\n\
         _Static_assert(offsetof(struct kevent, data) == 16, \"kevent data offset\");\n\
         _Static_assert(EVFILT_READ == -1, \"EVFILT_READ\");\n\
         _Static_assert(CTL_KERN == 1, \"CTL_KERN\");\n\
         _Static_assert(HW_NCPU == 3, \"HW_NCPU\");\n\
         int main(void) {{ struct pollfd pfd; fd_set fds; struct kevent kev; (void)pfd; (void)fds; (void)kev; return 0; }}\n"
    );
    for arch in [Architecture::X86_64, Architecture::Aarch64] {
        let config = TestConfig::new(arch, LibcVariant::FreeBsd);
        compile_test_program(&config, &source).unwrap();
    }
}

#[test]
fn freebsd_pthread_header_manifest_compiles_for_x86_64_and_aarch64() {
    let headers = freebsd_pthread_headers();
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!(
        "{includes}\n#include <stddef.h>\n\
         _Static_assert(sizeof(pthread_t) == 8, \"pthread_t size\");\n\
         _Static_assert(sizeof(pthread_mutex_t) == 8, \"pthread_mutex_t size\");\n\
         _Static_assert(sizeof(pthread_attr_t) == 8, \"pthread_attr_t size\");\n\
         _Static_assert(sizeof(pthread_cond_t) == 8, \"pthread_cond_t size\");\n\
         _Static_assert(sizeof(pthread_rwlock_t) == 8, \"pthread_rwlock_t size\");\n\
         _Static_assert(sizeof(pthread_once_t) == 16, \"pthread_once_t size\");\n\
         _Static_assert(sizeof(sem_t) == 16, \"sem_t size\");\n\
         _Static_assert(sizeof(struct sched_param) == 4, \"sched_param size\");\n\
         _Static_assert(PTHREAD_MUTEX_ERRORCHECK == 1, \"PTHREAD_MUTEX_ERRORCHECK\");\n\
         _Static_assert(PTHREAD_MUTEX_RECURSIVE == 2, \"PTHREAD_MUTEX_RECURSIVE\");\n\
         _Static_assert(PTHREAD_MUTEX_NORMAL == 3, \"PTHREAD_MUTEX_NORMAL\");\n\
         _Static_assert(PTHREAD_MUTEX_DEFAULT == PTHREAD_MUTEX_ERRORCHECK, \"PTHREAD_MUTEX_DEFAULT\");\n\
         _Static_assert(PTHREAD_CREATE_JOINABLE == 0, \"PTHREAD_CREATE_JOINABLE\");\n\
         _Static_assert(PTHREAD_CREATE_DETACHED == 0x1, \"PTHREAD_CREATE_DETACHED\");\n\
         _Static_assert(SCHED_FIFO == 1, \"SCHED_FIFO\");\n\
         _Static_assert(SCHED_OTHER == 2, \"SCHED_OTHER\");\n\
         _Static_assert(SCHED_RR == 3, \"SCHED_RR\");\n\
         int main(void) {{ pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; pthread_once_t o = PTHREAD_ONCE_INIT; sem_t s; (void)m; (void)o; (void)s; return 0; }}\n"
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
