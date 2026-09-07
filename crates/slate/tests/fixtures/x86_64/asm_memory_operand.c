#include <stdio.h>

static int read_via_memory(int x) {
  int result;
  // @lowering-begin
  // @rewrite-begin
  __asm__("movl %1, %0" : "=r"(result) : "m"(x));
  // @rewrite-end
  // @lowering-end
  return result;
}

static int increment_in_place(int x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("incl %0" : "+m"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

static int store_via_memory(void) {
  int x;
  // @lowering-begin
  // @rewrite-begin
  __asm__("movl $42, %0" : "=m"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

int main(void) {
  printf("%d %d %d\n", read_via_memory(7), increment_in_place(9),
         store_via_memory());
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("movl ({1}), {0:e}", lateout(reg) {{__v[0-9]+}}, in(reg) std::ptr::addr_of_mut!(x), options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("incl ({0})\n/* {1} */", in(reg) std::ptr::addr_of_mut!(x), in(reg) std::ptr::addr_of_mut!(x), options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("movl $42, ({0})", in(reg) std::ptr::addr_of_mut!(x), options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("movl ({1}), {0:e}", lateout(reg) {{__v[0-9]+}}, in(reg) std::ptr::addr_of_mut!(x), options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("incl ({0})\n/* {1} */", in(reg) std::ptr::addr_of_mut!(x), in(reg) std::ptr::addr_of_mut!(x), options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("movl $42, ({0})", in(reg) std::ptr::addr_of_mut!(x), options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
