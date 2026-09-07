#include <stdio.h>

static int inc_wide_via_Q(int x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("incl %0" : "+Q"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

static unsigned char inc_byte_via_q(unsigned char x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("incb %0" : "+q"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

int main(void) {
  printf("%d %d\n", inc_wide_via_Q(41), inc_byte_via_q(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("incl {0:e}", inlateout(reg_abcd) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u8;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("incb {0}", inlateout(reg_byte) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("incl {0:e}", inlateout(reg_abcd) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: u8;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("incb {0}", inlateout(reg_byte) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
