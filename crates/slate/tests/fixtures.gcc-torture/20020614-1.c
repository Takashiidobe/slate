/* PR c/6677 */
/* Verify that GCC doesn't perform illegal simplifications
   when folding constants.  */

#include <limits.h>

extern void abort(void);
extern void exit(int);

int main(void) {
  int           i;
  signed char   j;
  unsigned char k;

  i = SCHAR_MAX;

  j = ((signed char)(i << 1)) / 2;

  if (j != -1)
    abort();

  j = ((signed char)(i * 2)) / 2;

  if (j != -1)
    abort();

  i = UCHAR_MAX;

  k = ((unsigned char)(i << 1)) / 2;

  if (k != UCHAR_MAX / 2)
    abort();

  k = ((unsigned char)(i * 2)) / 2;

  if (k != UCHAR_MAX / 2)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/6677
// CHECK-NEXT: //!
// CHECK-NEXT: //! Verify that GCC doesn't perform illegal simplifications
// CHECK-NEXT: //! when folding constants.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 127 as i32;
// CHECK-NEXT:     let mut j: i8 = ((((i << (1 as i32)) as i8) as i32) / (2 as i32)) as i8;
// CHECK-NEXT:     let mut k: u8 = 0 as u8;
// CHECK-NEXT:     if (j as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     j = ((((i * (2 as i32)) as i8) as i32) / (2 as i32)) as i8;
// CHECK-NEXT:     if (j as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     i = (127 as i32) * (2 as i32) + (1 as i32);
// CHECK-NEXT:     k = (((((((i << (1 as i32)) as i8) as u8) as u32) as i32) / (2 as i32)) as i8) as u8;
// CHECK-NEXT:     if ((k as u32) as i32) != ((127 as i32) * (2 as i32) + (1 as i32)) / (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     k = (((((((i * (2 as i32)) as i8) as u8) as u32) as i32) / (2 as i32)) as i8) as u8;
// CHECK-NEXT:     if ((k as u32) as i32) != ((127 as i32) * (2 as i32) + (1 as i32)) / (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
