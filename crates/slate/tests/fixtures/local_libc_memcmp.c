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

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memcmp(__s1: *const (), __s2: *const (), __n: u64) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn cmp_bytes(a: *const u8, alen: i32, b: *const u8, blen: i32) -> i32 {
// CHECK-NEXT:     let mut sa: i32 = 0;
// CHECK-NEXT:     let mut sb: i32 = 0;
// CHECK-NEXT:     for i in 0..alen {
// CHECK-NEXT:         sa += ((unsafe { *unsafe { a.offset(i as isize) } }) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0..blen {
// CHECK-NEXT:         sb += ((unsafe { *unsafe { b.offset(i as isize) } }) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let order: i32 = unsafe { memcmp(a as *const (), b as *const (), 3 as u64) };
// CHECK-NEXT:     let sign: i32 = ((order > 0) as i32) - ((order < 0) as i32);
// CHECK-NEXT:     sign * 1000 + (sa - sb)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: [u8; 3] = [1, 2, 3];
// CHECK-NEXT:     let mut y: [u8; 3] = [1, 2, 4];
// CHECK-NEXT:     let mut z: [u8; 3] = [1, 2, 3];
// CHECK-NEXT:     let __slate_printf_arg0 = cmp_bytes(
// CHECK-NEXT:         x.as_mut_ptr() as *const u8,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         y.as_mut_ptr() as *const u8,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let __slate_printf_arg1 = cmp_bytes(
// CHECK-NEXT:         y.as_mut_ptr() as *const u8,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         x.as_mut_ptr() as *const u8,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let __slate_printf_arg2 = cmp_bytes(
// CHECK-NEXT:         x.as_mut_ptr() as *const u8,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         z.as_mut_ptr() as *const u8,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {} {}", __slate_printf_arg0, __slate_printf_arg1, __slate_printf_arg2);
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
