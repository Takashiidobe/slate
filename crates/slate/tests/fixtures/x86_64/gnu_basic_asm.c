
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

// @lowering-fn-begin
// @rewrite-fn-begin
int gnu_function_basic_asm(void) {
  __asm__ volatile("movl $23, gnu_basic_asm_value(%rip)");
  return gnu_basic_asm_value;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d %d\n", gnu_basic_asm_function(), gnu_function_basic_asm());
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: fn gnu_function_basic_asm() -> i32 {
// LOWERING-X86_64-GNU-DAG:     unsafe {
// LOWERING-X86_64-GNU-DAG:         core::arch::asm!(
// LOWERING-X86_64-GNU-DAG:             "movl $23, gnu_basic_asm_value(%rip)",
// LOWERING-X86_64-GNU-DAG:             options(att_syntax, raw)
// LOWERING-X86_64-GNU-DAG:         );
// LOWERING-X86_64-GNU-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = unsafe { gnu_basic_asm_value };
// LOWERING-X86_64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: fn gnu_function_basic_asm() -> i32 {
// REWRITES-X86_64-GNU-DAG:     unsafe {
// REWRITES-X86_64-GNU-DAG:         core::arch::asm!(
// REWRITES-X86_64-GNU-DAG:             "movl $23, gnu_basic_asm_value(%rip)",
// REWRITES-X86_64-GNU-DAG:             options(att_syntax, raw)
// REWRITES-X86_64-GNU-DAG:         );
// REWRITES-X86_64-GNU-DAG:     }
// REWRITES-X86_64-GNU-DAG:     unsafe { gnu_basic_asm_value }
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
