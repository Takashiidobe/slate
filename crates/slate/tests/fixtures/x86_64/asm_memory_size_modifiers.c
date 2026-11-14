#include <stdio.h>

static unsigned short control_word(void) {
  unsigned short cw;
  __asm__("fstcw %w0" : "=m"(cw));
  return cw;
}

static void set_control_word(unsigned short cw) { __asm__ volatile("fldcw %w0" : : "m"(cw)); }

int main(void) {
  unsigned short saved = control_word();
  printf("cw %04x\n", saved & 0x1f3f);
  set_control_word((unsigned short)((saved & ~0x0c00) | 0x0c00));
  printf("truncating %04x\n", control_word() & 0x0c00);
  set_control_word(saved);
  printf("restored %d\n", control_word() == saved);

  unsigned char byte = 0x5a;
  unsigned int word = 0x12345678;
  unsigned long long quad = 0x0102030405060708ull;
  unsigned short half = 0x0f0f;
  __asm__("notb %b0\n\tnotl %k1\n\tnegq %q2\n\tincw %h3" : "+m"(byte), "+m"(word), "+m"(quad), "+m"(half));
  printf("%02x %08x %016llx %04x\n", byte, word, quad, half);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn control_word() -> u16 {
// CHECK-NEXT:     let mut cw: u16 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fstcw ({0:r})", in (reg) std::ptr::addr_of_mut!(cw), options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     cw
// CHECK-NEXT: }
// CHECK-NEXT: fn set_control_word(mut cw: u16) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fldcw ({0:r})", in (reg) std::ptr::addr_of!(cw), options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let saved: u16 = control_word();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"cw %04x\n".as_ptr(), (saved as i32) & 7999) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     set_control_word(((saved as i32) & !3072 | 3072) as u16);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"truncating %04x\n".as_ptr(),
// CHECK-NEXT:             (control_word() as i32) & 3072,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     set_control_word(saved);
// CHECK-NEXT:     println!("restored {}", ((control_word() as i32) == (saved as i32)) as i32);
// CHECK-NEXT:     let mut byte: u8 = 90;
// CHECK-NEXT:     let mut word: u32 = 305419896;
// CHECK-NEXT:     let mut quad: u64 = 72623859790382856;
// CHECK-NEXT:     let mut half: u16 = 3855;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "notb ({0:r})\n\tnotl ({1:r})\n\tnegq ({2:r})\n\tincw ({3:r})", in (reg)
// CHECK-NEXT:             std::ptr::addr_of_mut!(byte), in (reg) std::ptr::addr_of_mut!(word), in (reg)
// CHECK-NEXT:             std::ptr::addr_of_mut!(quad), in (reg) std::ptr::addr_of_mut!(half),
// CHECK-NEXT:             options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%02x %08x %016llx %04x\n".as_ptr(),
// CHECK-NEXT:             byte as i32,
// CHECK-NEXT:             word,
// CHECK-NEXT:             quad,
// CHECK-NEXT:             half as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
