#include <stdio.h>
#include <string.h>

static int cmp_texts(const char *a, int alen, const char *b, int blen) {
  int sa = 0, sb = 0;
  for (int i = 0; i < alen; i++)
    sa += a[i];
  for (int i = 0; i < blen; i++)
    sb += b[i];
  int order = strcmp(a, b);
  int sign  = (order > 0) - (order < 0);
  int eq    = strcmp(a, b) == 0;
  return sign * 1000 + eq * 100 + (sa - sb);
}

int main(void) {
  const char x[] = "abc";
  const char y[] = "abd";
  printf("%d %d %d\n", cmp_texts(x, 3, y, 3), cmp_texts(y, 3, x, 3),
         cmp_texts(x, 3, x, 3));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::strcmp;
// CHECK-EMPTY:
// CHECK-NEXT: fn cmp_texts(a: *const i8, alen: i32, b: *const i8, blen: i32) -> i32 {
// CHECK-NEXT:     let mut sa: i32 = 0;
// CHECK-NEXT:     let mut sb: i32 = 0;
// CHECK-NEXT:     for i in 0..alen {
// CHECK-NEXT:         sa += (unsafe { *a.offset(i as isize) }) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0..blen {
// CHECK-NEXT:         sb += (unsafe { *b.offset(i as isize) }) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let order: i32 = unsafe {
// CHECK-NEXT:         strcmp(a as *const core::ffi::c_char, b as *const core::ffi::c_char) as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let sign: i32 = ((order > 0) as i32) - ((order < 0) as i32);
// CHECK-NEXT:     let eq: i32 = ((unsafe {
// CHECK-NEXT:         strcmp(a as *const core::ffi::c_char, b as *const core::ffi::c_char) as i32
// CHECK-NEXT:     }) == 0) as i32;
// CHECK-NEXT:     sign * 1000 + eq * 100 + (sa - sb)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: [i8; 4] = [97, 98, 99, 0];
// CHECK-NEXT:     let mut y: [i8; 4] = [97, 98, 100, 0];
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         cmp_texts(x.as_ptr() as *const i8, 3i32, y.as_ptr() as *const i8, 3i32),
// CHECK-NEXT:         cmp_texts(y.as_ptr() as *const i8, 3i32, x.as_ptr() as *const i8, 3i32),
// CHECK-NEXT:         cmp_texts(x.as_ptr() as *const i8, 3i32, x.as_ptr() as *const i8, 3i32)
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
