#include <stdio.h>

__attribute__((noinline)) static unsigned swap_low_bytes(unsigned x) {
  __asm__("xchgb %b0, %h0" : "+q"(x));
  return x;
}

__attribute__((noinline)) static unsigned high_byte(unsigned x) {
  unsigned char out;
  __asm__("movb %h1, %0" : "=r"(out) : "r"(x));
  return out;
}

__attribute__((noinline)) static unsigned mix(unsigned a, unsigned b) {
  __asm__("addb %h1, %h0\n\txorb %b1, %b0\n\tincw %w0" : "+q"(a) : "q"(b));
  return a;
}

int main(void) {
  unsigned values[] = {0x12345678u, 0xffff00ffu, 0x0000ab00u, 0x80017f80u};
  for (int i = 0; i < 4; i++)
    printf("%08x %02x %08x\n", swap_low_bytes(values[i]), high_byte(values[i]), mix(values[i], values[3 - i]));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn swap_low_bytes(mut x: u32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "xchgb {0:l}, {0:h}", inlateout(reg_abcd) x as u32 => [[T_0]], options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn high_byte(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut out: u8 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movb {1:h}, {0:l}", lateout(reg) [[T_0]], in (reg_abcd) x as u32, options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         out = unsafe { std::mem::transmute::<u8, u8>([[T_0]] as u8) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out as u32
// CHECK-NEXT: }
// CHECK-NEXT: fn mix(mut a: u32, mut b: u32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addb {1:h}, {0:h}\n\txorb {1:l}, {0:l}\n\tincw {0:x}", inlateout(reg_abcd) a
// CHECK-NEXT:             as u32 => [[T_0]], in (reg_abcd) b as u32, options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         a = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let values: __SlateAlign16<[u32; 4]> = __SlateAlign16([
// CHECK-NEXT:         305419896u32, 4294902015u32, 43776u32, 2147581824u32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%08x %02x %08x\n".as_ptr(),
// CHECK-NEXT:                 swap_low_bytes(values.0[i as usize]),
// CHECK-NEXT:                 high_byte(values.0[i as usize]),
// CHECK-NEXT:                 mix(values.0[i as usize], values.0[(3 - i) as usize]),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
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
