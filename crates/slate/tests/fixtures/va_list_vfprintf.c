#include <stdarg.h>
#include <stdio.h>

static void print_values(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
}

int main(void) {
  print_values("%d %s\n", 42, "forwarded");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn vfprintf(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: *mut slate_support::FILE,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: *const i8,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: core::ffi::VaList<'static>,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn print_values(format: *const i8, mut __va_args: ...) {
// CHECK-NEXT:     let mut args: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     args = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { vfprintf(slate_support::stdout(), format, args.clone()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         print_values(c"%d %s\n".as_ptr(), 42i32, c"forwarded".as_ptr() as *mut i8)
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
