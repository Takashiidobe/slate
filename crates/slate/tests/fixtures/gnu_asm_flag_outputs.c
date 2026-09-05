// { dg-do run { target x86_64-*-* } }
#include <stdio.h>

static int compare_flags(int left, int right) {
  int equal;
  int above;
  int less;

  // @lowering-begin
  // @rewrite-begin
  __asm__("cmpl %4, %3"
          : "=@cce"(equal), "=@cca"(above), "=@ccl"(less)
          : "r"(left), "r"(right));
  // @rewrite-end
  // @lowering-end
  return equal * 100 + above * 10 + less;
}

static int compare_intel_equal(int left, int right) {
  int equal;

  // @lowering-begin
  // @rewrite-begin
  __asm__(".intel_syntax noprefix\n\t"
          "cmp %V1, %V2\n\t"
          ".att_syntax prefix"
          : "=@ccz"(equal)
          : "r"(left), "r"(right));
  // @rewrite-end
  // @lowering-end
  return equal;
}

static int compare_flag_widths(int value) {
  unsigned short sign;
  unsigned long  nonzero;

  // @lowering-begin
  // @rewrite-begin
  __asm__("testl %2, %2" : "=@ccs"(sign), "=@ccne"(nonzero) : "r"(value));
  // @rewrite-end
  // @lowering-end
  return (int)(sign * 10 + nonzero);
}

int main(void) {
  printf("%d %d %d %d %d\n", compare_flags(5, 5), compare_flags(9, 4),
         compare_flags(-1, 1), compare_intel_equal(7, 7),
         compare_flag_widths(-1));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("cmpl {4:e}, {3:e}\n\tsete {0:l}\n\tmovzbl {0:l}, {0:e}\n\tseta {1:l}\n\tmovzbl {1:l}, {1:e}\n\tsetl {2:l}\n\tmovzbl {2:l}, {2:e}", lateout(reg) {{_v[0-9]+}}, lateout(reg) {{_v[0-9]+}}, lateout(reg) {{_v[0-9]+}}, in(reg) {{arg[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: i32;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("cmp {1:e}, {2:e}\n\tsetz {0:l}\n\tmovzx {0:e}, {0:l}", lateout(reg) {{_v[0-9]+}}, in(reg) {{arg[0-9]+}}, in(reg) {{arg[0-9]+}});
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: u16;
// LOWERING-DAG: let {{_v[0-9]+}}: u64;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("testl {2:e}, {2:e}\n\tsets {0:l}\n\tmovzbw {0:l}, {0:x}\n\tsetne {1:l}\n\tmovzbq {1:l}, {1:r}", lateout(reg) {{_v[0-9]+}}, lateout(reg) {{_v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: i32;
// REWRITES-DAG: let {{_v[0-9]+}}: i32;
// REWRITES-DAG: let {{_v[0-9]+}}: i32;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("cmpl {4:e}, {3:e}\n\tsete {0:l}\n\tmovzbl {0:l}, {0:e}\n\tseta {1:l}\n\tmovzbl {1:l}, {1:e}\n\tsetl {2:l}\n\tmovzbl {2:l}, {2:e}", lateout(reg) {{_v[0-9]+}}, lateout(reg) {{_v[0-9]+}}, lateout(reg) {{_v[0-9]+}}, in(reg) {{arg[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// REWRITES-DAG: }
// REWRITES-DAG: let {{_v[0-9]+}}: i32;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("cmp {1:e}, {2:e}\n\tsetz {0:l}\n\tmovzx {0:e}, {0:l}", lateout(reg) {{_v[0-9]+}}, in(reg) {{arg[0-9]+}}, in(reg) {{arg[0-9]+}});
// REWRITES-DAG: }
// REWRITES-DAG: let {{_v[0-9]+}}: u16;
// REWRITES-DAG: let {{_v[0-9]+}}: u64;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("testl {2:e}, {2:e}\n\tsets {0:l}\n\tmovzbw {0:l}, {0:x}\n\tsetne {1:l}\n\tmovzbq {1:l}, {1:r}", lateout(reg) {{_v[0-9]+}}, lateout(reg) {{_v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
