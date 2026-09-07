#include <stdio.h>

static int atomic_increment(int *counter) {
  int old, status;
  // @lowering-begin
  // @rewrite-begin
  __asm__ volatile("1: ldxr %w0, %1\n"
                    "\tadd %w0, %w0, #1\n"
                    "\tstxr %w3, %w0, %2\n"
                    "\tcbnz %w3, 1b"
                    : "=&r"(old), "+Q"(*counter), "+Q"(*counter), "=&r"(status));
  // @rewrite-end
  // @lowering-end
  return old;
}

int main(void) {
  int counter = 41;
  int result = atomic_increment(&counter);
  printf("%d %d\n", result, counter);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     core::arch::asm!("1: ldxr {0:w}, [{1}]\n\tadd {0:w}, {0:w}, #1\n\tstxr {3:w}, {0:w}, [{2}]\n\tcbnz {3:w}, 1b\n/* {4} */\n/* {5} */", out(reg) {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, in(reg) {{arg[0-9]+}}, out(reg) {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, in(reg) {{arg[0-9]+}});
// LOWERING-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:     core::arch::asm!("1: ldxr {0:w}, [{1}]\n\tadd {0:w}, {0:w}, #1\n\tstxr {3:w}, {0:w}, [{2}]\n\tcbnz {3:w}, 1b\n/* {4} */\n/* {5} */", out(reg) {{__v[0-9]+}}, in(reg) {{arg[0-9]+}} as *const i32, in(reg) {{arg[0-9]+}} as *const i32, out(reg) {{__v[0-9]+}}, in(reg) {{arg[0-9]+}} as *const i32, in(reg) {{arg[0-9]+}} as *const i32);
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
