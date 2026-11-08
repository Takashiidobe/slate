#include <stdio.h>

static int pinned_core(int a, int b) {
  register int first __asm__("r0") = a;
  register int second __asm__("r4") = b;
  register int result __asm__("lr");
  __asm__("add %0, %1, %2" : "=r"(result) : "r"(first), "r"(second));
  return result;
}

static float pinned_single(float value) {
  register float in __asm__("s3") = value;
  float result;
  __asm__("vadd.f32 %0, %1, %1" : "=t"(result) : "t"(in));
  return result;
}

static double double_register(double value) {
  double result;
  __asm__("vadd.f64 %P0, %P1, %P1" : "=w"(result) : "w"(value));
  return result;
}

static int reserved_clobbers(int value) {
  __asm__ volatile("mov r6, #0\n\t"
                   "add %0, %0, #2"
                   : "+r"(value)
                   :
                   : "r6");
  return value;
}

static int core_clobbers(int value) {
  __asm__ volatile("mov r1, #7\n\t"
                   "mov ip, #9\n\t"
                   "add %0, %0, r1\n\t"
                   "add %0, %0, ip"
                   : "+r"(value)
                   :
                   : "r1", "ip");
  return value;
}

static double vfp_clobbers(double value) {
  __asm__ volatile("vmov.f64 d0, #1.0\n\t"
                   "vmov.f32 s5, #2.0\n\t"
                   "vmov.f64 d7, #3.0"
                   :
                   :
                   : "d0", "s5", "s4", "q3", "d6");
  return value * 2.0;
}

static int immediates(int value) {
  __asm__("add %0, %0, %1 @ %c1 %2" : "+r"(value) : "I"(255), "M"(4));
  return value;
}

static int unused_operands(int value) {
  int result;
  float f = 1.0f;
  double d = 2.0;
  __asm__("mov %0, %1" : "=r"(result) : "r"(value), "t"(f), "w"(d), "m"(value), "i"(3));
  return result;
}

int main(void) {
  printf("%d %.1f %.1f\n", pinned_core(30, 12), pinned_single(1.25f), double_register(2.5));
  printf("%d %d %.1f\n", reserved_clobbers(40), core_clobbers(1), vfp_clobbers(4.0));
  printf("%d %d\n", immediates(1), unused_operands(5));
  return 0;
}

// SLATE-FILECHECK-BEGIN armv7-unknown-linux-gnueabihf
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn pinned_core(mut first: i32, mut second: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "add r14, r0, r4", lateout("r14") [[T_0]], in ("r0") first as u32, in ("r4")
// CHECK-NEXT:             second as u32, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn pinned_single(mut r#in: f32) -> f32 {
// CHECK-NEXT:     let mut result: f32 = 0 as f32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "vadd.f32 {0}, s3, s3", lateout(sreg) [[T_0]], in ("s3") unsafe {
// CHECK-NEXT:             std::mem::transmute::< f32, u32 > (r#in) }, options(pure, nomem, nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, f32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn double_register(mut value: f64) -> f64 {
// CHECK-NEXT:     let mut result: f64 = 0 as f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "vadd.f64 {0}, {1}, {1}", lateout(dreg_low16) [[T_0]], in (dreg_low16) unsafe {
// CHECK-NEXT:             std::mem::transmute::< f64, u64 > (value) }, options(pure, nomem, nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u64, f64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn reserved_clobbers(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {1}, r6\nmov r6, #0\n\tadd {0}, {0}, #2\nmov r6, {1}", inlateout(reg)
// CHECK-NEXT:             value as u32 => [[T_0]], out(reg) _, options(nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn core_clobbers(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov r1, #7\n\tmov ip, #9\n\tadd {0}, {0}, r1\n\tadd {0}, {0}, ip",
// CHECK-NEXT:             inlateout(reg) value as u32 => [[T_0]], lateout("r1") _, lateout("r12") _,
// CHECK-NEXT:             options(nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn vfp_clobbers(value: f64) -> f64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "vmov.f64 d0, #1.0\n\tvmov.f32 s5, #2.0\n\tvmov.f64 d7, #3.0", lateout("d0")
// CHECK-NEXT:             _, lateout("d2") _, lateout("d6") _, lateout("d7") _, options(nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     value * 2.0f64
// CHECK-NEXT: }
// CHECK-NEXT: fn immediates(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "add {0}, {0}, #{1} @ {1} #{2}", inlateout(reg) value as u32 => [[T_0]], const
// CHECK-NEXT:             255 as i32, const 4 as i32, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn unused_operands(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     let mut f: f32 = 1.0f32;
// CHECK-NEXT:     let mut d: f64 = 2.0f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {0}, {1}\n@ {2}\n@ {3}\n@ {4}\n@ {5}", lateout(reg) [[T_0]], in (reg) value
// CHECK-NEXT:             as u32, in (sreg) unsafe { std::mem::transmute::< f32, u32 > (f) }, in
// CHECK-NEXT:             (dreg_low16) unsafe { std::mem::transmute::< f64, u64 > (d) }, in (reg)
// CHECK-NEXT:             std::ptr::addr_of!(value), const 3 as i32, options(pure, readonly, nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %.1f %.1f\n".as_ptr(),
// CHECK-NEXT:             pinned_core(30i32, 12i32),
// CHECK-NEXT:             pinned_single(1.25f32) as f64,
// CHECK-NEXT:             double_register(2.5f64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %.1f\n".as_ptr(),
// CHECK-NEXT:             reserved_clobbers(40i32),
// CHECK-NEXT:             core_clobbers(1i32),
// CHECK-NEXT:             vfp_clobbers(4.0f64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{} {}", immediates(1i32), unused_operands(5i32));
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
