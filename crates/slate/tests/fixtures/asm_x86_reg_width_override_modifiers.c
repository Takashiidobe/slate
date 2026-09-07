#include <stdio.h>

static int inc_byte_view_of_int(int x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("incb %b0" : "+r"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

static int inc_word_view_of_int(int x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("incw %w0" : "+r"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

static long inc_dword_view_of_long(long x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("incl %k0" : "+r"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

static long inc_qword_view_of_long(long x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("incq %q0" : "+r"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

int main(void) {
  printf("%d %d %ld %ld\n", inc_byte_view_of_int(0xFF), inc_word_view_of_int(0xFFFF),
         inc_dword_view_of_long(1), inc_qword_view_of_long(1));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("incb {0:l}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("incw {0:x}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i64;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("incl {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i64;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("incq {0:r}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("incb {0:l}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("incw {0:x}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: i64;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("incl {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: i64;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("incq {0:r}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
