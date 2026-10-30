// { dg-options "-std=gnu23" }
#include <stdio.h>

int main(void) {
  const volatile int qualified   = 9;
  typeof_unqual(qualified) copy  = qualified;
  typeof(qualified) preserved    = 12;
  int              *pointer      = nullptr;
  constexpr int     width        = 7;
  unsigned _BitInt(width) narrow = 100;
  int unqualified = __builtin_types_compatible_p(typeof(copy), int);
  int still_qualified =
      __builtin_types_compatible_p(typeof(preserved), const volatile int);
  printf("%d %d %d %d %d\n", copy, preserved, pointer == nullptr, (int)narrow,
         unqualified + still_qualified);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-std=gnu23" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let qualified: i32 = 9;
// CHECK-NEXT:     let copy: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(qualified)) };
// CHECK-NEXT:     let preserved: i32 = 12;
// CHECK-NEXT:     let pointer: *mut i32 = std::ptr::null_mut::<i32>();
// CHECK-NEXT:     let narrow: u8 = 100u8;
// CHECK-NEXT:     let unqualified: i32 = 1;
// CHECK-NEXT:     let still_qualified: i32 = 1;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {}",
// CHECK-NEXT:         copy,
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(preserved)) },
// CHECK-NEXT:         pointer.is_null() as i32,
// CHECK-NEXT:         (narrow as u32) as i32,
// CHECK-NEXT:         unqualified + still_qualified
// CHECK-NEXT:     );
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
