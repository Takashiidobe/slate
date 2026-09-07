#include <stdio.h>

static int global_value = 42;

static int read_via_address(void) {
  int result;
  // @lowering-begin
  // @rewrite-begin
  __asm__("movl %a1, %0" : "=r"(result) : "p"(&global_value));
  // @rewrite-end
  // @lowering-end
  return result;
}

int main(void) {
  printf("%d\n", read_via_address());
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("movl ({1}), {0:e}", lateout(reg) {{__v[0-9]+}}, in(reg) std::ptr::addr_of_mut!(global_value), options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("movl ({1}), {0:e}", lateout(reg) {{__v[0-9]+}}, in(reg) std::ptr::addr_of_mut!(global_value), options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
