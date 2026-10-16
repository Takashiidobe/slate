extern void abort(void);

int main() {
  int           i, j;
  unsigned long u, r1, r2;

  i = -16;
  j = 1;
  u = i + j;

  /* no sign extension upon shift */
  r1 = u >> 1;
  /* sign extension upon shift, but there shouldn't be */
  r2 = ((unsigned long)(i + j)) >> 1;

  if (r1 != r2)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = -(16 as i32);
// CHECK-NEXT:     let mut j: i32 = 1 as i32;
// CHECK-NEXT:     let mut u: u64 = ((i + j) as i64) as u64;
// CHECK-NEXT:     let mut r1: u64 = 0 as u64;
// CHECK-NEXT:     let mut r2: u64 = 0 as u64;
// CHECK-NEXT:     // no sign extension upon shift
// CHECK-NEXT:     r1 = u >> (1 as i32);
// CHECK-NEXT:     // sign extension upon shift, but there shouldn't be
// CHECK-NEXT:     r2 = (((i + j) as i64) as u64) >> (1 as i32);
// CHECK-NEXT:     if r1 != r2 {
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
