typedef unsigned int U32;

// @check-fn-begin
int cpuid_leaf7_ebx_nonzero(void) {
  U32 f7a, f7b, f7c;
  __asm__("cpuid" : "=a"(f7a), "=b"(f7b), "=c"(f7c) : "a"(7), "c"(0) : "edx");
  return (int)(f7b != 0 || f7a != 0 || f7c != 0);
}
// @check-fn-end

int main(void) { return cpuid_leaf7_ebx_nonzero(); }

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn cpuid_leaf7_ebx_nonzero() -> i32 {
// CHECK-NEXT:     let mut f7a: u32 = 0 as u32;
// CHECK-NEXT:     let mut f7b: u32 = 0 as u32;
// CHECK-NEXT:     let mut f7c: u32 = 0 as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "xchg {0:r}, %rbx\ncpuid\nxchg {0:r}, %rbx", out(reg) [[T_1]], lateout("ax")
// CHECK-NEXT:             [[T_0]], lateout("cx") [[T_2]], in ("ax") (7 as i32) as u32, in ("cx") (0 as i32)
// CHECK-NEXT:             as u32, lateout("dx") _, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         f7a = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:         f7b = unsafe { std::mem::transmute::<u32, u32>([[T_1]] as u32) };
// CHECK-NEXT:         f7c = unsafe { std::mem::transmute::<u32, u32>([[T_2]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     (f7b != ((0 as i32) as u32) || f7a != ((0 as i32) as u32)
// CHECK-NEXT:         || f7c != ((0 as i32) as u32)) as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
