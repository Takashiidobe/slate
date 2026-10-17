#include <stdio.h>

__attribute__((visibility("internal"))) extern const unsigned char opmodes[4];
__attribute__((visibility("hidden"))) extern const char *const typenames[3];
__attribute__((visibility("protected"))) int counter;

const unsigned char opmodes[4] = {1, 2, 4, 8};
const char *const typenames[3] = {"nil", "boolean", "number"};

int main(void) {
  for (int i = 0; i < 4; i++) {
    counter += opmodes[i];
  }
  printf("%d %s %s\n", counter, typenames[0], typenames[2]);
  return counter == 15 ? 0 : 1;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut typenames: __SlateAlign16<[*const i8; 3]> = __SlateAlign16([
// CHECK-NEXT:     (b"nil\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     (b"boolean\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     (b"number\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut counter: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut opmodes: [u8; 4] = [
// CHECK-NEXT:     ((1 as i32) as i8) as u8,
// CHECK-NEXT:     ((2 as i32) as i8) as u8,
// CHECK-NEXT:     ((4 as i32) as i8) as u8,
// CHECK-NEXT:     ((8 as i32) as i8) as u8,
// CHECK-NEXT: ];
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     for i in 0 as i32..4 as i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter = (unsafe { counter })
// CHECK-NEXT:                 + (((unsafe { opmodes[i as usize] }) as u32) as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %s %s\n".as_ptr(),
// CHECK-NEXT:             unsafe { counter },
// CHECK-NEXT:             unsafe { typenames.0[0] },
// CHECK-NEXT:             unsafe { typenames.0[2] },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     if (unsafe { counter }) == 15 { 0 } else { 1 }
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
