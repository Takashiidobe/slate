#include <stdio.h>

static int inc_high_byte_of_wide_operand(int x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("incb %h0" : "+Q"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

int main(void) {
  printf("%d\n", inc_high_byte_of_wide_operand(0x1234));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("incb {0:h}", inlateout(reg_abcd) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("incb {0:h}", inlateout(reg_abcd) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
