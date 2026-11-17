#include <stdio.h>

unsigned short ds = 0x1234;
unsigned eax = 100;
unsigned counter = 40;

static unsigned long read_ds(void) {
  unsigned long r;
  __asm__ volatile(".intel_syntax noprefix\n\t"
                   "mov ax, ds\n\t"
                   "movzx %V0, ax\n\t"
                   ".att_syntax prefix"
                   : "=r"(r)
                   :
                   : "rax");
  return r;
}

static unsigned long add_three(unsigned long value) {
  __asm__(".intel_syntax noprefix\n\t"
          "mov eax, 3\n\t"
          "add %V0, rax\n\t"
          ".att_syntax prefix"
          : "+r"(value)
          :
          : "rax");
  return value;
}

static unsigned long add_counter(unsigned long value) {
  __asm__ volatile(".intel_syntax noprefix\n\t"
                   "mov eax, DWORD PTR counter[rip]\n\t"
                   "add %V0, rax\n\t"
                   ".att_syntax prefix"
                   : "+r"(value)
                   :
                   : "rax", "memory");
  return value;
}

int main(void) {
  printf("%d %lu %lu %u %u\n", read_ds() != ds, add_three(4), add_counter(2), eax, counter);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut ds: u16 = 4660;
// CHECK-NEXT: static mut eax: u32 = 100;
// CHECK-NEXT: static mut counter: u32 = 40;
// CHECK-NEXT: fn read_ds() -> u64 {
// CHECK-NEXT:     let mut r: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             ".intel_syntax noprefix\n\tmov ax, ds\n\tmovzx rcx, ax\n\t.att_syntax prefix",
// CHECK-NEXT:             lateout("cx") [[T_0]], out("ax") _, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = unsafe { core::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn add_three(mut value: u64) -> u64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             ".intel_syntax noprefix\n\tmov eax, 3\n\tadd rcx, rax\n\t.att_syntax prefix",
// CHECK-NEXT:             inlateout("cx") value as u64 => [[T_0]], out("ax") _, options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { core::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn add_counter(mut value: u64) -> u64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             ".intel_syntax noprefix\n\tmov eax, DWORD PTR {0}[rip]\n\tadd rcx, rax\n\t.att_syntax prefix",
// CHECK-NEXT:             sym counter, inlateout("cx") value as u64 => [[T_0]], out("ax") _,
// CHECK-NEXT:             options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { core::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %lu %lu %u %u\n".as_ptr(),
// CHECK-NEXT:             (read_ds() != ((ds as i32) as u64)) as i32,
// CHECK-NEXT:             add_three(4u64),
// CHECK-NEXT:             add_counter(2u64),
// CHECK-NEXT:             eax,
// CHECK-NEXT:             counter,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
