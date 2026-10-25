/* { dg-options "/Zc:preprocessor /MTd /RTC1 /W4 /sdl" } */
void abort(void);

int main(void) {
#if !defined(_DEBUG) || defined(_DLL) || defined(_MSVC_MT)
  abort();
#endif
#if !defined(__MSVC_RUNTIME_CHECKS) || _MSVC_WARNING_LEVEL != 4
  abort();
#endif
#ifndef _GUARDOVERFLOW_CRT_ALLOCATORS
  abort();
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc i686-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor /MTd /RTC1 /W4 /sdl" }
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
