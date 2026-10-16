/* PR rtl-optimization/28970 */
/* Origin: Peter Bergner <bergner@vnet.ibm.com> */

extern void abort(void);

int tar(long i) {
  if (i != 36863)
    abort();

  return -1;
}

void bug(int q, long bcount) {
  int j     = 0;
  int outgo = 0;

  while (j != -1) {
    outgo++;
    if (outgo > q - 1)
      outgo = q - 1;
    j = tar(outgo * bcount);
  }
}

int main(void) {
  bug(5, 36863);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/28970
// CHECK-NEXT: //!
// CHECK-NEXT: //! Origin: Peter Bergner <bergner@vnet.ibm.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn tar(mut i: i64) -> i32 {
// CHECK-NEXT:     if i != ((36863 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     -(1 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bug(mut q: i32, mut bcount: i64) {
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut outgo: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while j != -(1 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = outgo;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         outgo = [[V_1]];
// CHECK-NEXT:         if outgo > q - (1 as i32) {
// CHECK-NEXT:             outgo = q - (1 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         j = tar((outgo as i64) * bcount);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     bug(5 as i32, (36863 as i32) as i64);
// CHECK-NEXT:     0 as i32
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
