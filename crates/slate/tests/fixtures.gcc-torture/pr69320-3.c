#include <stdlib.h>

static int a[40] = {7, 5, 3, 3, 0, 0, 3};
short      b;
int        c = 5;
int        main() {
  b = 0;
  for (; b <= 3; b++)
    if (a[b + 6] ^ (0 || c))
      ;
    else
      break;
  if (b != 4)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut a: __SlateAlign16<[i32; 40]> = __SlateAlign16({
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [i32; 40] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = 7 as i32;
// CHECK-NEXT:     [[T_0]][1] = 5 as i32;
// CHECK-NEXT:     [[T_0]][2] = 3 as i32;
// CHECK-NEXT:     [[T_0]][3] = 3 as i32;
// CHECK-NEXT:     [[T_0]][4] = 0 as i32;
// CHECK-NEXT:     [[T_0]][5] = 0 as i32;
// CHECK-NEXT:     [[T_0]][6] = 3 as i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-NEXT: static mut b: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i32 = 5 as i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = (0 as i32) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((unsafe { b }) as i32) <= (3 as i32) {
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                     .offset((((unsafe { b }) as i32) + (6 as i32)) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) ^ (((0 as i32) != (0 as i32) || (unsafe { c }) != (0 as i32)) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             {}
// CHECK-NEXT:         } else {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i16 = unsafe { b };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i16 = (([[V_0]] as i32) + (1 as i32)) as i16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { b }) as i32) != (4 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
