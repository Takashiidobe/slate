
#include <stdio.h>

__asm__(".text\n"
        ".globl gnu_basic_asm_function\n"
        ".type gnu_basic_asm_function,@function\n"
        "gnu_basic_asm_function:\n"
        "movl $37, %eax\n"
        "ret\n"
        ".size gnu_basic_asm_function,.-gnu_basic_asm_function\n");

extern int gnu_basic_asm_function(void);

static int gnu_basic_asm_value;

// @check-fn-begin
int gnu_function_basic_asm(void) {
  __asm__ volatile("movl $23, gnu_basic_asm_value(%rip)");
  return gnu_basic_asm_value;
}
// @check-fn-end

int main(void) {
  printf("%d %d\n", gnu_basic_asm_function(), gnu_function_basic_asm());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn gnu_function_basic_asm() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl $23, {0}(%rip)", sym gnu_basic_asm_value, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { gnu_basic_asm_value }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
