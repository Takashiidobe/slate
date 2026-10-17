typedef unsigned int U32;

// @check-fn-begin
static int cpuid_leaf1_nonzero(void) {
  U32 f1a, f1c, f1d;
  __asm__("cpuid\n\t" : "=a"(f1a), "=c"(f1c), "=d"(f1d) : "a"(1) : "ebx");
  return (int)((f1c | f1d) != 0);
}
// @check-fn-end

int main(void) { return cpuid_leaf1_nonzero(); }

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn cpuid_leaf1_nonzero() -> i32 {
// CHECK-NEXT:     let mut f1a: u32 = 0;
// CHECK-NEXT:     let mut f1c: u32 = 0;
// CHECK-NEXT:     let mut f1d: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov %rbx, {0:r}\ncpuid\n\t\nmov {0:r}, %rbx", out(reg) _, lateout("ax")
// CHECK-NEXT:             [[T_0]], lateout("cx") [[T_1]], lateout("dx") [[T_2]], in ("ax") (1 as i32) as u32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         f1a = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:         f1c = unsafe { std::mem::transmute::<u32, u32>([[T_1]] as u32) };
// CHECK-NEXT:         f1d = unsafe { std::mem::transmute::<u32, u32>([[T_2]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     (f1c | f1d != 0) as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
