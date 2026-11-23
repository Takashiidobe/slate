typedef unsigned int U32;

// @check-fn-begin
static int cpuid_leaf1_nonzero(void) {
  U32 f1a, f1c, f1d;
  __asm__("cpuid\n\t" : "=a"(f1a), "=c"(f1c), "=d"(f1d) : "a"(1) : "ebx");
  return (int)((f1c | f1d) != 0);
}
// @check-fn-end

int main(void) { return cpuid_leaf1_nonzero(); }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn cpuid_leaf1_nonzero() -> i32 {
// CHECK-NEXT:     let mut f1a: u32 = 0;
// CHECK-NEXT:     let mut f1c: u32 = 0;
// CHECK-NEXT:     let mut f1d: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov %rbx, {0:r}\ncpuid\n\t\nmov {0:r}, %rbx",
// CHECK-NEXT:             out(reg) _,
// CHECK-NEXT:             lateout("ax") f1a,
// CHECK-NEXT:             lateout("cx") f1c,
// CHECK-NEXT:             lateout("dx") f1d,
// CHECK-NEXT:             in("ax") 1i32 as u32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     (f1c | f1d != 0) as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
