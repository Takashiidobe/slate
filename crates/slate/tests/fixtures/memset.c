#include <stdio.h>
#include <string.h>

static int get_count(void) { return 4; }

int main(void) {
  unsigned char zero_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memset(zero_buf, 0, sizeof(zero_buf));

  unsigned char value_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memset(value_buf, 0x41, sizeof(value_buf));

  unsigned char partial_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memset(partial_buf, 9, 4);

  unsigned char dynamic_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  int           n              = get_count();
  memset(dynamic_buf, 9, n);

  for (int i = 0; i < 8; i++)
    printf("%d ", zero_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", value_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", partial_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dynamic_buf[i]);
  printf("\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: fn get_count() -> i32 {
// CHECK-NEXT:     4
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut zero_buf: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// CHECK-NEXT:     unsafe { zero_buf[0usize..8usize].fill(0u8) };
// CHECK-NEXT:     let mut value_buf: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// CHECK-NEXT:     unsafe { value_buf[0usize..8usize].fill(65u8) };
// CHECK-NEXT:     let mut partial_buf: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// CHECK-NEXT:     unsafe { partial_buf[0usize..4usize].fill(9u8) };
// CHECK-NEXT:     let mut dynamic_buf: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// CHECK-NEXT:     let n: i32 = get_count();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             dynamic_buf.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             9u8,
// CHECK-NEXT:             ((n as i64) as u64) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         print!("{} ", (zero_buf[i as usize] as u32) as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         print!("{} ", (value_buf[i as usize] as u32) as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         print!("{} ", (partial_buf[i as usize] as u32) as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         print!("{} ", (dynamic_buf[i as usize] as u32) as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
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
