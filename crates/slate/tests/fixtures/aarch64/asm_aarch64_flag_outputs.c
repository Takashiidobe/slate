#include <stdio.h>

static int compare_flags(int left, int right) {
  int equal;
  int higher;
  int less;
  int carry;

  __asm__("cmp %w4, %w5"
          : "=@cceq"(equal), "=@cchi"(higher), "=@cclt"(less), "=@cccs"(carry)
          : "r"(left), "r"(right));
  return equal * 1000 + higher * 100 + less * 10 + carry;
}

static int sign_zero(int value) {
  unsigned char minus;
  _Bool nonzero;

  __asm__("cmp %w2, #0" : "=@ccmi"(minus), "=@ccne"(nonzero) : "r"(value));
  return minus * 10 + nonzero;
}

int main(void) {
  printf("%d %d %d\n", compare_flags(5, 5), compare_flags(9, 4),
         compare_flags(-1, 1));
  printf("%d %d %d\n", sign_zero(-3), sign_zero(0), sign_zero(7));
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn compare_flags(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut equal: i32 = 0;
// CHECK-NEXT:     let mut higher: i32 = 0;
// CHECK-NEXT:     let mut less: i32 = 0;
// CHECK-NEXT:     let mut carry: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {4:w}, {5:w}\ncset {0:w}, eq\ncset {1:w}, hi\ncset {2:w}, lt\ncset {3:w}, cs",
// CHECK-NEXT:             lateout(reg) [[T_0]], lateout(reg) [[T_1]], lateout(reg) [[T_2]], lateout(reg) [[T_3]],
// CHECK-NEXT:             in (reg) left as u32, in (reg) right as u32, options(pure, nomem, nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         equal = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:         higher = unsafe { std::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:         less = unsafe { std::mem::transmute::<u32, i32>([[T_2]] as u32) };
// CHECK-NEXT:         carry = unsafe { std::mem::transmute::<u32, i32>([[T_3]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     equal * 1000 + higher * 100 + less * 10 + carry
// CHECK-NEXT: }
// CHECK-NEXT: fn sign_zero(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut minus: u8 = 0;
// CHECK-NEXT:     let mut nonzero: bool = false;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         let [[T_1]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {2:w}, #0\ncset {0:w}, mi\ncset {1:w}, ne", lateout(reg) [[T_0]],
// CHECK-NEXT:             lateout(reg) [[T_1]], in (reg) value as u32, options(pure, nomem, nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         minus = unsafe { std::mem::transmute::<u8, u8>([[T_0]] as u8) };
// CHECK-NEXT:         nonzero = ([[T_1]] as u8) != 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     ((minus as u32) as i32) * 10 + (nonzero as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", compare_flags(5i32, 5i32), compare_flags(9i32, 4i32), compare_flags(-
// CHECK-NEXT:         1i32, 1i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {} {}", sign_zero(- 3i32), sign_zero(0i32), sign_zero(7i32));
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
