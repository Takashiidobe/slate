/* Check that MAX_EXPR and MIN_EXPR are working properly.  */

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

extern void abort(void);

int main() {
  int ll_bitsize, ll_bitpos;
  int rl_bitsize, rl_bitpos;
  int end_bit;

  ll_bitpos  = 32;
  ll_bitsize = 32;
  rl_bitpos  = 0;
  rl_bitsize = 32;

  end_bit = MAX(ll_bitpos + ll_bitsize, rl_bitpos + rl_bitsize);
  if (end_bit != 64)
    abort();
  end_bit = MAX(rl_bitpos + rl_bitsize, ll_bitpos + ll_bitsize);
  if (end_bit != 64)
    abort();
  end_bit = MIN(ll_bitpos + ll_bitsize, rl_bitpos + rl_bitsize);
  if (end_bit != 32)
    abort();
  end_bit = MIN(rl_bitpos + rl_bitsize, ll_bitpos + ll_bitsize);
  if (end_bit != 32)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Check that MAX_EXPR and MIN_EXPR are working properly.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut ll_bitsize: i32 = 32 as i32;
// CHECK-NEXT:     let mut ll_bitpos: i32 = 32 as i32;
// CHECK-NEXT:     let mut rl_bitsize: i32 = 32 as i32;
// CHECK-NEXT:     let mut rl_bitpos: i32 = 0 as i32;
// CHECK-NEXT:     let mut end_bit: i32 = if ll_bitpos + ll_bitsize > rl_bitpos + rl_bitsize {
// CHECK-NEXT:         ll_bitpos + ll_bitsize
// CHECK-NEXT:     } else {
// CHECK-NEXT:         rl_bitpos + rl_bitsize
// CHECK-NEXT:     };
// CHECK-NEXT:     if end_bit != (64 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     end_bit = if rl_bitpos + rl_bitsize > ll_bitpos + ll_bitsize {
// CHECK-NEXT:         rl_bitpos + rl_bitsize
// CHECK-NEXT:     } else {
// CHECK-NEXT:         ll_bitpos + ll_bitsize
// CHECK-NEXT:     };
// CHECK-NEXT:     if end_bit != (64 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     end_bit = if ll_bitpos + ll_bitsize < rl_bitpos + rl_bitsize {
// CHECK-NEXT:         ll_bitpos + ll_bitsize
// CHECK-NEXT:     } else {
// CHECK-NEXT:         rl_bitpos + rl_bitsize
// CHECK-NEXT:     };
// CHECK-NEXT:     if end_bit != (32 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     end_bit = if rl_bitpos + rl_bitsize < ll_bitpos + ll_bitsize {
// CHECK-NEXT:         rl_bitpos + rl_bitsize
// CHECK-NEXT:     } else {
// CHECK-NEXT:         ll_bitpos + ll_bitsize
// CHECK-NEXT:     };
// CHECK-NEXT:     if end_bit != (32 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
