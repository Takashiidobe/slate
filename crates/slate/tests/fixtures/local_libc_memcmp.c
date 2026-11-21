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
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::memcmp;
// CHECK-EMPTY:
// CHECK-NEXT: fn cmp_bytes(a: &[u8], alen: i32, b: &[u8], blen: i32) -> i32 {
// CHECK-NEXT:     let mut sa: i32 = 0;
// CHECK-NEXT:     let mut sb: i32 = 0;
// CHECK-NEXT:     for i in 0..alen {
// CHECK-NEXT:         sa += a[i as usize] as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0..blen {
// CHECK-NEXT:         sb += b[i as usize] as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let order: i32 = unsafe {
// CHECK-NEXT:         a[0usize..(3u64 as usize) as usize].cmp(&b[0usize..(3u64 as usize) as usize])
// CHECK-NEXT:             as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let sign: i32 = ((order > 0) as i32) - ((order < 0) as i32);
// CHECK-NEXT:     sign * 1000 + (sa - sb)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: [u8; 3] = [1, 2, 3];
// CHECK-NEXT:     let mut y: [u8; 3] = [1, 2, 4];
// CHECK-NEXT:     let mut z: [u8; 3] = [1, 2, 3];
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         cmp_bytes(x.as_slice(), 3i32, y.as_slice(), 3i32),
// CHECK-NEXT:         cmp_bytes(y.as_slice(), 3i32, x.as_slice(), 3i32),
// CHECK-NEXT:         cmp_bytes(x.as_slice(), 3i32, z.as_slice(), 3i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
