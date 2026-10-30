#include <stdio.h>
#include <string.h>

static int cmp_bytes(const unsigned char *a, int alen, const unsigned char *b,
                     int blen) {
  int sa = 0, sb = 0;
  for (int i = 0; i < alen; i++)
    sa += a[i];
  for (int i = 0; i < blen; i++)
    sb += b[i];
  int order = memcmp(a, b, 3);
  int sign  = (order > 0) - (order < 0);
  return sign * 1000 + (sa - sb);
}

int main(void) {
  unsigned char x[] = {1, 2, 3};
  unsigned char y[] = {1, 2, 4};
  unsigned char z[] = {1, 2, 3};
  printf("%d %d %d\n", cmp_bytes(x, 3, y, 3), cmp_bytes(y, 3, x, 3),
         cmp_bytes(x, 3, z, 3));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memcmp([[ARG_0]]: *const (), [[ARG_1:arg[0-9]+]]: *const (), [[ARG_2:arg[0-9]+]]: u64) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn cmp_bytes(a: &[u8], b: &[u8]) -> i32 {
// CHECK-NEXT:     let mut sa: i32 = 0;
// CHECK-NEXT:     let mut sb: i32 = 0;
// CHECK-NEXT:     for i in 0..a.len() as i32 {
// CHECK-NEXT:         sa += ((unsafe { a[i as usize] }) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0..b.len() as i32 {
// CHECK-NEXT:         sb += ((unsafe { b[i as usize] }) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let order: i32 = unsafe { a[0usize..3 as usize].cmp(&b[0usize..3 as usize]) as i32 };
// CHECK-NEXT:     let sign: i32 = ((order > 0) as i32) - ((order < 0) as i32);
// CHECK-NEXT:     sign * 1000 + (sa - sb)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x: [u8; 3] = [1, 2, 3];
// CHECK-NEXT:     let y: [u8; 3] = [1, 2, 4];
// CHECK-NEXT:     let z: [u8; 3] = [1, 2, 3];
// CHECK-NEXT:     println!("{} {} {}", cmp_bytes(& x, & y), cmp_bytes(& y, & x), cmp_bytes(& x, & z));
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
