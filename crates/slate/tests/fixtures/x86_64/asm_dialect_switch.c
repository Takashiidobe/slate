#include <stdio.h>

// @check-fn-begin
static int intel_add(int value) {
  __asm__ volatile(".intel_syntax noprefix\n\t"
                   "add %V0, 7\n\t"
                   ".att_syntax prefix"
                   : "+r"(value));
  return value;
}
// @check-fn-end

static int compare_intel_equal(int left, int right) {
  int equal;

  __asm__(".intel_syntax noprefix\n\t"
          "cmp %V1, %V2\n\t"
          ".att_syntax prefix"
          : "=@ccz"(equal)
          : "r"(left), "r"(right));
  return equal;
}

static long intel_shift_wide(long value, long amount) {
  __asm__(".intel_syntax noprefix\n\t"
          "lea %V0, [%V0 + %V1 * 4]\n\t"
          ".att_syntax prefix"
          : "+r"(value)
          : "r"(amount));
  return value;
}

// @check-fn-begin
static int att_subtract(int value) {
  __asm__ volatile("subl $2, %0" : "+r"(value));
  return value;
}
// @check-fn-end

int main(void) {
  printf("%d %d %d %d %ld\n", intel_add(5), att_subtract(5),
         compare_intel_equal(7, 7), compare_intel_equal(7, 8),
         intel_shift_wide(0x100000000L, 3));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn intel_add(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             ".intel_syntax noprefix\n\tadd eax, 7\n\t.att_syntax prefix", inlateout("ax")
// CHECK-NEXT:             value as u32 => [[T_0]], options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn att_subtract(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "subl $2, {0:e}", inlateout(reg) value as u32 => [[T_0]], options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK: {{^}}}
// SLATE-FILECHECK-END
