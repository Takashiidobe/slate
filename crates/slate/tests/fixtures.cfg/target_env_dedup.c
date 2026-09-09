#include <stdio.h>
#include <sys/syscall.h>

int main() { printf("%d\n", SYS_preadv); }

// SLATE-TARGETS: aarch64-linux-gnu,x86_64-linux-gnu,i686-linux-gnu,arm-linux-gnueabihf,arm-linux-musleabihf,arm-linux-musleabihf
// COMMON: unsafe extern "C" {
// COMMON-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-NEXT: }
// COMMON-EMPTY:
// COMMON-NEXT: #[cfg(all(target_arch = "aarch64", target_os = "linux", target_env = "gnu"))]
// COMMON-NEXT: fn main() -> std::process::ExitCode {
// COMMON-NEXT:     println!("{}", 69 as i32);
// COMMON-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// COMMON-NEXT:     return std::process::ExitCode::SUCCESS;
// COMMON-NEXT: }
// COMMON-EMPTY:
// COMMON-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// COMMON-NEXT: fn main() -> std::process::ExitCode {
// COMMON-NEXT:     println!("{}", 295 as i32);
// COMMON-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// COMMON-NEXT:     return std::process::ExitCode::SUCCESS;
// COMMON-NEXT: }
// COMMON-EMPTY:
// COMMON-NEXT: #[cfg(all(target_arch = "x86", target_os = "linux", target_env = "gnu"))]
// COMMON-NEXT: fn main() -> std::process::ExitCode {
// COMMON-NEXT:     println!("{}", 333 as i32);
// COMMON-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// COMMON-NEXT:     return std::process::ExitCode::SUCCESS;
// COMMON-NEXT: }
// COMMON-EMPTY:
// COMMON-NEXT: #[cfg(all(
// COMMON-NEXT:     target_arch = "arm",
// COMMON-NEXT:     target_os = "linux",
// COMMON-NEXT:     any(target_env = "gnu", target_env = "musl")
// COMMON-NEXT: ))]
// COMMON-NEXT: fn main() -> std::process::ExitCode {
// COMMON-NEXT:     println!("{}", 361 as i32);
// COMMON-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// COMMON-NEXT:     return std::process::ExitCode::SUCCESS;
// COMMON-NEXT: }
