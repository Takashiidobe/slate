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

static long long negate(long long value) {
  __asm__("mvn %R0, %R0\n\t"
          "rsbs %Q0, %Q0, #0\n\t"
          "adc %R0, %R0, #0"
          : "+r"(value)
          :
          : "cc");
  return value;
}

static int modifiers(int *pointer) {
  int result;
  __asm__("mov %0, #%B1\n\t"
          "movw r2, #%L2\n\t"
          "add %0, %0, r2\n\t"
          "ldr r2, [%m3]\n\t"
          "add %0, %0, r2\n\t"
          "@ %4 %c4"
          : "=&r"(result)
          : "i"(-16), "i"(0x12345), "m"(*pointer), "i"(&counter)
          : "r2");
  return result;
}

int main(void) {
  int value = 7;
  printf("%llx\n", add64(0xffffffffULL, 0x100000001ULL));
  printf("%lld %lld\n", negate(5), negate(-0x100000000LL));
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
// CHECK-NEXT: fn negate(mut value: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64 = value as u64;
// CHECK-NEXT:         let [[T_1]]: u32;
// CHECK-NEXT:         let [[T_2]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mvn r1, r1\n\trsbs r0, r0, #0\n\tadc r1, r1, #0", inlateout("r0") [[T_0]] as
// CHECK-NEXT:             u32 => [[T_1]], inlateout("r1") ([[T_0]] >> 32) as u32 => [[T_2]], options(pure,
// CHECK-NEXT:             nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe {
// CHECK-NEXT:             std::mem::transmute::<u64, i64>((([[T_2]] as u64) << 32 | ([[T_1]] as u64)) as u64)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn modifiers(mut pointer: &i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {0}, #15\n\tmovw r2, #9029\n\tadd {0}, {0}, r2\n\tldr r2, [{3}]\n\tadd {0}, {0}, r2\n\t@ #{4} {4}\n@ {1}\n@ {2}",
// CHECK-NEXT:             out(reg) [[T_0]], const - 16 as i32, const 74565 as i32, in (reg) pointer as *
// CHECK-NEXT:             const i32, sym counter, out("r2") _, options(pure, readonly, nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
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
// CHECK-NEXT:             negate(5i64),
// CHECK-NEXT:             negate(-4294967296i64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
