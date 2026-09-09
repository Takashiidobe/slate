#include <stdio.h>
#include <sys/syscall.h>

int main() { printf("%d\n", SYS_preadv); }

// SLATE-TARGETS: x86_64-linux-gnu,aarch64-linux-gnu
// COMMON: unsafe extern "C" {
// COMMON-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-NEXT: }
// COMMON-EMPTY:
// COMMON-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// COMMON-NEXT: fn main() -> std::process::ExitCode {
// COMMON-NEXT:     println!("{}", 295 as i32);
// COMMON-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// COMMON-NEXT:     return std::process::ExitCode::SUCCESS;
// COMMON-NEXT: }
// COMMON-EMPTY:
// COMMON-NEXT: #[cfg(all(target_arch = "aarch64", target_os = "linux", target_env = "gnu"))]
// COMMON-NEXT: fn main() -> std::process::ExitCode {
// COMMON-NEXT:     println!("{}", 69 as i32);
// COMMON-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// COMMON-NEXT:     return std::process::ExitCode::SUCCESS;
// COMMON-NEXT: }
