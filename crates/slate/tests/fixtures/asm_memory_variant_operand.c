#include <stdio.h>

static int read_via_o(int x) {
  int result;
  // @lowering-begin
  // @rewrite-begin
  __asm__("movl %1, %0" : "=r"(result) : "o"(x));
  // @rewrite-end
  // @lowering-end
  return result;
}

static int incr_via_V(int x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("incl %0" : "+V"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

static int store_via_o(void) {
  int x;
  // @lowering-begin
  // @rewrite-begin
  __asm__("movl $42, %0" : "=o"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

int main(void) {
  printf("%d %d %d\n", read_via_o(7), incr_via_V(9), store_via_o());
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("movl ({1}), {0:e}", lateout(reg) {{__v[0-9]+}}, in(reg) std::ptr::addr_of_mut!(x), options(att_syntax));
// LOWERING-DAG: }
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("incl ({0})\n/* {1} */", in(reg) std::ptr::addr_of_mut!(x), in(reg) std::ptr::addr_of_mut!(x), options(att_syntax));
// LOWERING-DAG: }
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("movl $42, ({0})", in(reg) std::ptr::addr_of_mut!(x), options(att_syntax));
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("movl ({1}), {0:e}", lateout(reg) {{__v[0-9]+}}, in(reg) std::ptr::addr_of_mut!(x), options(att_syntax));
// REWRITES-DAG: }
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("incl ({0})\n/* {1} */", in(reg) std::ptr::addr_of_mut!(x), in(reg) std::ptr::addr_of_mut!(x), options(att_syntax));
// REWRITES-DAG: }
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("movl $42, ({0})", in(reg) std::ptr::addr_of_mut!(x), options(att_syntax));
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
