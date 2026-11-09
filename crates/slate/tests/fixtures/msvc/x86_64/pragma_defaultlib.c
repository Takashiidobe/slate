/* { dg-options "/Zc:preprocessor /O2 /MT /TC" } */
#include <windows.h>
void abort(void);

#pragma comment(linker, "/defaultlib:user32.lib")

int main(void) {
  if (GetSystemMetrics(SM_CXSCREEN) < 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor /O2 /MT /TC" }
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn GetSystemMetrics([[ARG_0:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::abort;
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".section .drectve,\"yni\"\n.ascii \" /defaultlib:user32.lib\"");
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe { GetSystemMetrics(0i32) }) < 0 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
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
