#include <stdio.h>

static unsigned char inc_byte_tied_via_Q(unsigned char x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("incb %b0" : "+Q"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

static unsigned char add_byte_input_via_Q(unsigned char x, unsigned char y) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("addb %b1, %b0" : "+Q"(x) : "Q"(y));
  // @rewrite-end
  // @lowering-end
  return x;
}

int main(void) {
  printf("%d %d\n", inc_byte_tied_via_Q(0xFF), add_byte_input_via_Q(5, 9));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("incb {0:l}", inlateout(reg_abcd) ({{arg[0-9]+}} as u8) as i32 => {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("addb {1:l}, {0:l}", inlateout(reg_abcd) ({{arg[0-9]+}} as u8) as i32 => {{__v[0-9]+}}, in(reg_abcd) ({{arg[0-9]+}} as u8) as i32, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("incb {0:l}", inlateout(reg_abcd) ({{arg[0-9]+}} as u8) as i32 => {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("addb {1:l}, {0:l}", inlateout(reg_abcd) ({{arg[0-9]+}} as u8) as i32 => {{__v[0-9]+}}, in(reg_abcd) ({{arg[0-9]+}} as u8) as i32, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
