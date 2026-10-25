/* { dg-options "/Zc:preprocessor /Gz /std:c11" } */
void __cdecl abort(void);

static int add(int a, int b) { return a + b; }

int main(void) {
  if (_Generic(&add, int(__stdcall *)(int, int): 1, default: 0) != 1)
    abort();
  if (add(2, 3) != 5)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor /Gz /std:c11" }
// CHECK-NEXT: extern "stdcall-unwind" fn add(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if false {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if add(2i32, 3i32) != 5 {
// CHECK-NEXT:         unsafe { abort() };
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
