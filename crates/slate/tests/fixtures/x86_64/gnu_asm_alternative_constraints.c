#include <stdio.h>

static int add_ri(int a, int b) {
  int result;
  // @lowering-begin
  // @rewrite-begin
  __asm__("addl %2, %0" : "=r"(result) : "0"(a), "ri"(b) : "cc");
  // @rewrite-end
  // @lowering-end
  return result;
}

static int add_rm(int a, int *p) {
  int result;
  // @lowering-begin
  // @rewrite-begin
  __asm__("addl %2, %0" : "=r"(result) : "0"(a), "rm"(*p) : "cc");
  // @rewrite-end
  // @lowering-end
  return result;
}

int main(void) {
  int x = 4;
  printf("%d %d\n", add_ri(3, 5), add_rm(3, &x));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("addl {1:e}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("addl {1:e}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("addl {1:e}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("addl {1:e}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, in(reg) unsafe { *({{arg[0-9]+}} as *const i32) }, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
