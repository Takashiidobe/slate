#include <stdio.h>

static int add_const_n(int value) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("addl $%c[amount], %[value]"
          : [value] "+r"(value)
          : [amount] "n"(4)
          : "cc");
  // @rewrite-end
  // @lowering-end
  return value;
}

static int shift_const_i_letter(int value) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("sall $%c[amount], %[value]"
          : [value] "+r"(value)
          : [amount] "I"(3)
          : "cc");
  // @rewrite-end
  // @lowering-end
  return value;
}

int main(void) {
  printf("%d %d\n", add_const_n(10), shift_const_i_letter(2));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("addl ${1}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 4, options(att_syntax));
// LOWERING-DAG: }
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("sall ${1}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 3, options(att_syntax));
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("addl ${1}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 4, options(att_syntax));
// REWRITES-DAG: }
// REWRITES-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("sall ${1}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 3, options(att_syntax));
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
