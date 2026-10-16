#include <stdio.h>

// @check-fn-begin
static int inc_byte_view_of_int(int x) {
  __asm__("incb %b0" : "+r"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static int inc_word_view_of_int(int x) {
  __asm__("incw %w0" : "+r"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static long inc_dword_view_of_long(long x) {
  __asm__("incl %k0" : "+r"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static long inc_qword_view_of_long(long x) {
  __asm__("incq %q0" : "+r"(x));
  return x;
}
// @check-fn-end

int main(void) {
  printf("%d %d %ld %ld\n", inc_byte_view_of_int(0xFF), inc_word_view_of_int(0xFFFF),
         inc_dword_view_of_long(1), inc_qword_view_of_long(1));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn inc_byte_view_of_int(mut x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incb {0:l}", inlateout(reg) x as u32 => [[T_0]], options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn inc_word_view_of_int(mut x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incw {0:x}", inlateout(reg) x as u32 => [[T_0]], options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn inc_dword_view_of_long(mut x: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incl {0:e}", inlateout(reg) x as u64 => [[T_0]], options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn inc_qword_view_of_long(mut x: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incq {0:r}", inlateout(reg) x as u64 => [[T_0]], options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// SLATE-FILECHECK-END
