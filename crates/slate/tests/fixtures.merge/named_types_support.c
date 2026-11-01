// merge-extra-targets: android-x86_64 x86_64-linux-android clang
#define _GNU_SOURCE 1
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>
#ifdef _WIN32
#include <corecrt.h>
#include <corecrt_wctype.h>
#else
#include <sys/socket.h>
#include <wchar.h>
#endif

intmax_t f_intmax;
uintmax_t f_uintmax;
wint_t f_wint;
time_t f_time;
clock_t f_clock;
dev_t f_dev;
ino_t f_ino;
#ifndef _WIN32
off64_t f_off64;
pid_t f_pid;
uid_t f_uid;
gid_t f_gid;
mode_t f_mode;
nlink_t f_nlink;
socklen_t f_socklen;
#endif

int main(void) { return 0; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   named_types_support.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   named_types_support.c: [] 1 compiled
// CHECK-NEXT: target android-x86_64: 1 compiles
// CHECK-NEXT:   named_types_support.c: [] 1 compiled
// CHECK-NEXT: total: 3 compiles across 3 targets
// CHECK-NEXT: unit named_types_support: 31 items, 14 gated, 0 duplicated functions
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-extra-targets: android-x86_64 x86_64-linux-android clang
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "android")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::clock_t;
// CHECK-NEXT: use slate_support::dev_t;
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use slate_support::gid_t;
// CHECK-NEXT: use slate_support::ino_t;
// CHECK-NEXT: use slate_support::intmax_t;
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use slate_support::mode_t;
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use slate_support::nlink_t;
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use slate_support::off64_t;
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use slate_support::pid_t;
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use slate_support::socklen_t;
// CHECK-NEXT: use slate_support::time_t;
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use slate_support::uid_t;
// CHECK-NEXT: use slate_support::uintmax_t;
// CHECK-NEXT: use slate_support::wint_t;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_intmax: intmax_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_uintmax: uintmax_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_wint: wint_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_time: time_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_clock: clock_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_dev: dev_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_ino: ino_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_off64: off64_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_pid: pid_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_uid: uid_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_gid: gid_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_mode: mode_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_nlink: nlink_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut f_socklen: socklen_t = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
