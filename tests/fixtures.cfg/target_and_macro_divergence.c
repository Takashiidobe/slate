#include <stdio.h>
#include <sys/syscall.h>

#ifdef MY_FEATURE
int feature_code(void) { return 10; }
#else
int feature_code(void) { return 20; }
#endif

int main(void) {
  printf("%d %d\n", feature_code(), (int)SYS_preadv);
  return 0;
}

// SLATE-TARGETS: x86_64-linux-gnu,aarch64-linux-gnu
// COMMON: #[cfg(feature = "my_feature")]
// COMMON-NEXT: fn feature_code() -> i32 {
// COMMON-NEXT:     10
// COMMON-NEXT: }
// COMMON-EMPTY:
// COMMON-NEXT: #[cfg(not(feature = "my_feature"))]
// COMMON-NEXT: fn feature_code() -> i32 {
// COMMON-NEXT:     20
// COMMON-NEXT: }
// COMMON-EMPTY:
// COMMON-NEXT: unsafe extern "C" {
// COMMON-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-NEXT: }
// COMMON-EMPTY:
// COMMON-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// COMMON-NEXT: fn main() -> std::process::ExitCode {
// COMMON-NEXT:     println!("{} {}", feature_code(), 295 as i32);
// COMMON-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// COMMON-NEXT:     return std::process::ExitCode::SUCCESS;
// COMMON-NEXT: }
// COMMON-EMPTY:
// COMMON-NEXT: #[cfg(all(target_arch = "aarch64", target_os = "linux", target_env = "gnu"))]
// COMMON-NEXT: fn main() -> std::process::ExitCode {
// COMMON-NEXT:     println!("{} {}", feature_code(), 69 as i32);
// COMMON-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// COMMON-NEXT:     return std::process::ExitCode::SUCCESS;
// COMMON-NEXT: }
