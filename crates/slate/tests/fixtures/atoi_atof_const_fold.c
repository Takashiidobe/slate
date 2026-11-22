#include <stdio.h>
#include <stdlib.h>

int main(void) {
  const char *s = "-7xyz";
  printf("%d\n", atoi("42abc"));
  printf("%d\n", atoi("  +13 "));
  printf("%d\n", atoi(s));
  printf("%d\n", atoi("nope"));
  printf("%ld\n", atol("1000000000000"));
  printf("%lld\n", atoll("-9000000000000"));
  printf("%.2f\n", atof("  3.14  "));
  printf("%.1f\n", atof("2"));
  printf("%.2f\n", atof("6.5garbage"));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::atof;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let s: *const i8 = c"-7xyz".as_ptr();
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::__slate_atoi(
// CHECK-NEXT:                 (c"42abc".as_ptr() as *const core::ffi::c_char) as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::__slate_atoi(
// CHECK-NEXT:                 (c"  +13 ".as_ptr() as *const core::ffi::c_char) as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::__slate_atoi((s as *const core::ffi::c_char) as *const i8)
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::__slate_atoi(
// CHECK-NEXT:                 (c"nope".as_ptr() as *const core::ffi::c_char) as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"%ld\n".as_ptr(), "1000000000000".parse::<i64>().unwrap())
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lld\n".as_ptr(),
// CHECK-NEXT:             "-9000000000000".parse::<i64>().unwrap(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%.2f\n".as_ptr(),
// CHECK-NEXT:             atof(c"  3.14  ".as_ptr() as *const core::ffi::c_char) as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%.1f\n".as_ptr(), "2".parse::<f64>().unwrap()) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%.2f\n".as_ptr(),
// CHECK-NEXT:             atof(c"6.5garbage".as_ptr() as *const core::ffi::c_char) as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
