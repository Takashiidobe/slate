// { dg-do run { target x86_64-*-* } }

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
// LOWERING-DAG: fn gnu_function_basic_asm() -> i32 {
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         core::arch::asm!(
// LOWERING-DAG:             "movl $23, gnu_basic_asm_value(%rip)",
// LOWERING-DAG:             options(att_syntax, raw)
// LOWERING-DAG:         );
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { gnu_basic_asm_value };
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn gnu_function_basic_asm() -> i32 {
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         core::arch::asm!(
// REWRITES-DAG:             "movl $23, gnu_basic_asm_value(%rip)",
// REWRITES-DAG:             options(att_syntax, raw)
// REWRITES-DAG:         );
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { gnu_basic_asm_value }
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
