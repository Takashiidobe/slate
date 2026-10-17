#include <stdio.h>

int main(void) {
  static const unsigned char data[] = {
#embed "c23_embed.bin"
  };
  static const unsigned char framed[] = {0xAA,
#embed "c23_embed.bin" prefix(0xBB, ) suffix(, 0xCC)
                                         , 0xDD};
  static const unsigned char empty[] = {
#embed "c23_embed_empty.bin" if_empty(0xEE)
  };
  for (size_t i = 0; i < sizeof(data); i++) {
    putchar(data[i]);
  }
  int framed_ok = sizeof(framed) == 8 && framed[0] == 0xAA &&
                  framed[1] == 0xBB && framed[2] == 'C' && framed[3] == '2' &&
                  framed[4] == '3' && framed[5] == '\n' && framed[6] == 0xCC &&
                  framed[7] == 0xDD;
  int empty_ok  = sizeof(empty) == 1 && empty[0] == 0xEE;
  return sizeof(data) == 4 && data[0] == 'C' && data[1] == '2' &&
                 data[2] == '3' && data[3] == '\n' && framed_ok && empty_ok
             ? 0
             : 1;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn putchar(__c: i32) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     for i in 0 as u64..4 as u64 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { putchar(((unsafe { data[i as usize] }) as u32) as i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let framed_ok: i32 = ((8 as u64) == (8 as u64)
// CHECK-NEXT:         && (((unsafe { framed[0] }) as u32) as i32) == 170
// CHECK-NEXT:         && (((unsafe { framed[1] }) as u32) as i32) == 187
// CHECK-NEXT:         && (((unsafe { framed[2] }) as u32) as i32) == 67
// CHECK-NEXT:         && (((unsafe { framed[3] }) as u32) as i32) == 50
// CHECK-NEXT:         && (((unsafe { framed[4] }) as u32) as i32) == 51
// CHECK-NEXT:         && (((unsafe { framed[5] }) as u32) as i32) == 10
// CHECK-NEXT:         && (((unsafe { framed[6] }) as u32) as i32) == 204
// CHECK-NEXT:         && (((unsafe { framed[7] }) as u32) as i32) == 221) as i32;
// CHECK-NEXT:     let empty_ok: i32 = ((1 as u64) == (1 as u64)
// CHECK-NEXT:         && (((unsafe { empty[0] }) as u32) as i32) == 238) as i32;
// CHECK-NEXT:     if (4 as u64) == (4 as u64) && (((unsafe { data[0] }) as u32) as i32) == 67
// CHECK-NEXT:         && (((unsafe { data[1] }) as u32) as i32) == 50
// CHECK-NEXT:         && (((unsafe { data[2] }) as u32) as i32) == 51
// CHECK-NEXT:         && (((unsafe { data[3] }) as u32) as i32) == 10 && framed_ok != 0
// CHECK-NEXT:         && empty_ok != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         1
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut data: [u8; 4] = [
// CHECK-NEXT:     ((67 as i32) as i8) as u8,
// CHECK-NEXT:     ((50 as i32) as i8) as u8,
// CHECK-NEXT:     ((51 as i32) as i8) as u8,
// CHECK-NEXT:     ((10 as i32) as i8) as u8,
// CHECK-NEXT: ];
// CHECK-NEXT: static mut framed: [u8; 8] = [
// CHECK-NEXT:     ((170 as i32) as i8) as u8,
// CHECK-NEXT:     ((187 as i32) as i8) as u8,
// CHECK-NEXT:     ((67 as i32) as i8) as u8,
// CHECK-NEXT:     ((50 as i32) as i8) as u8,
// CHECK-NEXT:     ((51 as i32) as i8) as u8,
// CHECK-NEXT:     ((10 as i32) as i8) as u8,
// CHECK-NEXT:     ((204 as i32) as i8) as u8,
// CHECK-NEXT:     ((221 as i32) as i8) as u8,
// CHECK-NEXT: ];
// CHECK-NEXT: static mut empty: [u8; 1] = [((238 as i32) as i8) as u8];
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
