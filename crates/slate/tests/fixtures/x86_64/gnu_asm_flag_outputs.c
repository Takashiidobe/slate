#include <stdio.h>

// @check-fn-begin
static int compare_flags(int left, int right) {
  int equal;
  int above;
  int less;

  __asm__("cmpl %4, %3"
          : "=@cce"(equal), "=@cca"(above), "=@ccl"(less)
          : "r"(left), "r"(right));
  return equal * 100 + above * 10 + less;
}
// @check-fn-end

// @check-fn-begin
static int compare_flag_widths(int value) {
  unsigned short sign;
  unsigned long  nonzero;

  __asm__("testl %2, %2" : "=@ccs"(sign), "=@ccne"(nonzero) : "r"(value));
  return (int)(sign * 10 + nonzero);
}
// @check-fn-end

static int carry_bool(unsigned a, unsigned b) {
  _Bool carry;
  unsigned char zero;

  __asm__("addl %3, %2" : "=@ccc"(carry), "=@ccz"(zero), "+r"(a) : "r"(b));
  return carry * 10 + zero;
}

int main(void) {
  printf("%d %d %d %d\n", compare_flags(5, 5), compare_flags(9, 4),
         compare_flags(-1, 1), compare_flag_widths(-1));
  printf("%d %d %d\n", carry_bool(0xffffffffu, 2), carry_bool(0xffffffffu, 1),
         carry_bool(1, 2));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn compare_flags(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut equal: i32 = 0;
// CHECK-NEXT:     let mut above: i32 = 0;
// CHECK-NEXT:     let mut less: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u8;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u8;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u8;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmpl {4:e}, {3:e}\nsete {0}\nseta {1}\nsetl {2}", lateout(reg_byte) [[T_0]],
// CHECK-NEXT:             lateout(reg_byte) [[T_1]], lateout(reg_byte) [[T_2]], in (reg) left as u32, in
// CHECK-NEXT:             (reg) right as u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         equal = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:         above = unsafe { std::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:         less = unsafe { std::mem::transmute::<u32, i32>([[T_2]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     equal * 100 + above * 10 + less
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn compare_flag_widths(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut sign: u16 = 0;
// CHECK-NEXT:     let mut nonzero: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u8;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u8;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "testl {2:e}, {2:e}\nsets {0}\nsetne {1}", lateout(reg_byte) [[T_0]],
// CHECK-NEXT:             lateout(reg_byte) [[T_1]], in (reg) value as u32, options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         sign = unsafe { std::mem::transmute::<u16, u16>([[T_0]] as u16) };
// CHECK-NEXT:         nonzero = unsafe { std::mem::transmute::<u64, u64>([[T_1]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     ((((((sign as u32) as i32) * 10) as i64) as u64).wrapping_add(nonzero) as u32) as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
