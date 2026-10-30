#include <fenv.h>
#include <stdio.h>

int main(void) {
  int original = fegetround();
  int modes[] = {FE_TONEAREST, FE_TOWARDZERO, FE_UPWARD, FE_DOWNWARD};
  for (unsigned i = 0; i < 4; ++i) {
    if (fesetround(modes[i])) return 1;
    printf("%d\n", __builtin_flt_rounds());
  }
  if (fesetround(original)) return 2;
  printf("%d\n", __builtin_flt_rounds());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fegetround() -> i32;
// CHECK-NEXT:     fn fesetround([[ARG_0:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT:     fn printf([[ARG_0]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_flt_rounds() -> i32;
// CHECK-NEXT:     fn __slate_fenv_flt_rounds() -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let original: i32 = unsafe { fegetround() };
// CHECK-NEXT:     let modes: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         0i32, 3072i32, 2048i32, 1024i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     for i in 0u32..4u32 {
// CHECK-NEXT:         if (unsafe { fesetround(modes.0[i as usize]) }) != 0 {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{}", unsafe { __slate_fenv_flt_rounds() });
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { fesetround(original) }) != 0 {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", unsafe { __slate_fenv_flt_rounds() });
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
