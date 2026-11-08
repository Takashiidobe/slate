#include <stdio.h>

int counter = 41;

static unsigned long long add64(unsigned long long a, unsigned long long b) {
  unsigned long long result;
  __asm__("adds %Q0, %Q1, %Q2\n\t"
          "adc %R0, %R1, %R2"
          : "=&r"(result)
          : "r"(a), "r"(b)
          : "cc");
  return result;
}

static long long increment(long long value) {
  __asm__("adds %Q0, %Q0, #1\n\t"
          "adc %R0, %R0, #0"
          : "+r"(value)
          :
          : "cc");
  return value;
}

static unsigned long long swap_halves(unsigned long long value) {
  unsigned long long result;
  __asm__("mov %0, %H2\n\t"
          "mov %H0, %2"
          : "=&r"(result)
          : "0"(value >> 1), "r"(value));
  return result;
}

static unsigned long long exclusive_load(unsigned long long *pointer) {
  unsigned long long result;
  __asm__ volatile("ldrexd %0, %H0, [%1]\n\t"
                   "clrex"
                   : "=&r"(result)
                   : "r"(pointer)
                   : "memory");
  return result;
}

static double double_from_pair(double value) {
  double result;
  __asm__("vmov %P0, %Q1, %R1" : "=w"(result) : "r"(value));
  return result;
}

static int modifiers(int *pointer) {
  int result;
  __asm__("mov %0, #%B1\n\t"
          "movw r1, #%L2\n\t"
          "add %0, %0, r1\n\t"
          "ldr r1, [%m3]\n\t"
          "add %0, %0, r1\n\t"
          "@ %4"
          : "=&r"(result)
          : "i"(-16), "i"(0x12345), "m"(*pointer), "i"(&counter)
          : "r1");
  return result;
}

int main(void) {
  unsigned long long pair = 0x123456789abcdef0ULL;
  int value = 7;
  printf("%llx\n", add64(0xffffffffULL, 0x100000001ULL));
  printf("%lld %lld\n", increment(-1), increment(0xffffffffLL));
  printf("%llx\n", swap_halves(pair));
  printf("%llx\n", exclusive_load(&pair));
  printf("%.2f\n", double_from_pair(3.25));
  printf("%d %d\n", modifiers(&value), counter);
  return 0;
}

// SLATE-FILECHECK-BEGIN armv7-unknown-linux-gnueabihf
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut counter: i32 = 41;
// CHECK-NEXT: fn add64(a: u64, b: u64) -> u64 {
// CHECK-NEXT:     let mut result: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u64 = a as u64;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: u64 = b as u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "adds r0, r2, r4\n\tadc r1, r3, r5", out("r0") [[T_0]], out("r1") [[T_1]], in
// CHECK-NEXT:             ("r2") [[T_2]] as u32, in ("r3") ([[T_2]] >> 32) as u32, in ("r4") [[T_3]] as u32, in
// CHECK-NEXT:             ("r5") ([[T_3]] >> 32) as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe {
// CHECK-NEXT:             std::mem::transmute::<u64, u64>((([[T_1]] as u64) << 32 | ([[T_0]] as u64)) as u64)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn increment(mut value: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64 = value as u64;
// CHECK-NEXT:         let [[T_1]]: u32;
// CHECK-NEXT:         let [[T_2]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "adds r0, r0, #1\n\tadc r1, r1, #0", inlateout("r0") [[T_0]] as u32 => [[T_1]],
// CHECK-NEXT:             inlateout("r1") ([[T_0]] >> 32) as u32 => [[T_2]], options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe {
// CHECK-NEXT:             std::mem::transmute::<u64, i64>((([[T_2]] as u64) << 32 | ([[T_1]] as u64)) as u64)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn swap_halves(value: u64) -> u64 {
// CHECK-NEXT:     let mut result: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64 = (value >> 1i32) as u64;
// CHECK-NEXT:         let [[T_1]]: u32;
// CHECK-NEXT:         let [[T_2]]: u32;
// CHECK-NEXT:         let [[T_3]]: u64 = value as u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov r0, r3\n\tmov r1, r2", inout("r0") [[T_0]] as u32 => [[T_1]], inout("r1")
// CHECK-NEXT:             ([[T_0]] >> 32) as u32 => [[T_2]], in ("r2") [[T_3]] as u32, in ("r3") ([[T_3]] >> 32) as
// CHECK-NEXT:             u32, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe {
// CHECK-NEXT:             std::mem::transmute::<u64, u64>((([[T_2]] as u64) << 32 | ([[T_1]] as u64)) as u64)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn exclusive_load(mut pointer: &mut u64) -> u64 {
// CHECK-NEXT:     let mut result: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         let [[T_1]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "ldrexd r0, r1, [{0}]\n\tclrex", in (reg) (pointer as * mut u64) as u32,
// CHECK-NEXT:             out("r0") [[T_0]], out("r1") [[T_1]], options(nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe {
// CHECK-NEXT:             std::mem::transmute::<u64, u64>((([[T_1]] as u64) << 32 | ([[T_0]] as u64)) as u64)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn double_from_pair(value: f64) -> f64 {
// CHECK-NEXT:     let mut result: f64 = 0 as f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         let [[T_1]]: u64 = unsafe { std::mem::transmute::<f64, u64>(value) };
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "vmov {0}, r0, r1", lateout(dreg_low16) [[T_0]], in ("r0") [[T_1]] as u32, in
// CHECK-NEXT:             ("r1") ([[T_1]] >> 32) as u32, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u64, f64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn modifiers(mut pointer: &i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {0}, #15\n\tmovw r1, #9029\n\tadd {0}, {0}, r1\n\tldr r1, [{3}]\n\tadd {0}, {0}, r1\n\t@ {4}\n@ {1}\n@ {2}",
// CHECK-NEXT:             out(reg) [[T_0]], const - 16 as i32, const 74565 as i32, in (reg) pointer as *
// CHECK-NEXT:             const i32, sym counter, out("r1") _, options(pure, readonly, nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut pair: u64 = 1311768467463790320;
// CHECK-NEXT:     let mut value: i32 = 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"%llx\n".as_ptr(), add64(4294967295u64, 4294967297u64))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lld %lld\n".as_ptr(),
// CHECK-NEXT:             increment(-1i64),
// CHECK-NEXT:             increment(4294967295i64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%llx\n".as_ptr(), swap_halves(pair)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%llx\n".as_ptr(),
// CHECK-NEXT:             exclusive_load(&mut (*std::ptr::addr_of_mut!(pair))),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%.2f\n".as_ptr(), double_from_pair(3.25f64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", modifiers(unsafe { & (* std::ptr::addr_of_mut!(value)) }), unsafe {
// CHECK-NEXT:         counter }
// CHECK-NEXT:     );
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
