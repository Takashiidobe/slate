/* { dg-options "-ftree-loop-distribution" } */
extern void  abort(void);
extern void *memset(void *s, int c, __SIZE_TYPE__ n);
extern int   memcmp(const void *s1, const void *s2, __SIZE_TYPE__ n);
/*extern int printf(const char *format, ...);*/

int main() {
  char A[30], B[30], C[30];
  int  i;

  /* prepare arrays */
  memset(A, 1, 30);
  memset(B, 1, 30);

  for (i = 20; i-- > 10;) {
    A[i] = 0;
    B[i] = 0;
  }

  /* expected result */
  memset(C, 1, 30);
  memset(C + 10, 0, 10);

  /* show result */
  /*  for (i = 0; i < 30; i++)
      printf("%d %d %d\n", A[i], B[i], C[i]); */

  /* compare results */
  if (memcmp(A, C, 30) || memcmp(B, C, 30))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn memset(s: *mut (), c: i32, n: u64) -> *mut ();
// CHECK-NEXT:     fn memcmp(s1: *const (), s2: *const (), n: u64) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: // extern int printf(const char *format, ...);
// CHECK-NEXT: // { dg-options "-ftree-loop-distribution" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut A: __SlateAlign16<[i8; 30]> = __SlateAlign16([0 as i8; 30]);
// CHECK-NEXT:     let mut B: __SlateAlign16<[i8; 30]> = __SlateAlign16([0 as i8; 30]);
// CHECK-NEXT:     let mut C: __SlateAlign16<[i8; 30]> = __SlateAlign16([0 as i8; 30]);
// CHECK-NEXT:     let mut i: i32 = 20 as i32;
// CHECK-NEXT:     // prepare arrays
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             A.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             ((30 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             B.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             ((30 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] - (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:         if !([[V_0]] > (10 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (A.0.as_mut_ptr() as *mut i8).offset(i as isize) } = (0 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (B.0.as_mut_ptr() as *mut i8).offset(i as isize) } = (0 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     // expected result
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             C.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             ((30 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { (C.0.as_mut_ptr() as *mut i8).offset((10 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((10 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     // show result
// CHECK-NEXT:     //
// CHECK-NEXT:     //  for (i = 0; i < 30; i++)
// CHECK-NEXT:     // printf("%d %d %d\n", A[i], B[i], C[i]);
// CHECK-NEXT:     // compare results
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             A.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             C.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             ((30 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = (unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 B.0.as_mut_ptr() as *const (),
// CHECK-NEXT:                 C.0.as_mut_ptr() as *const (),
// CHECK-NEXT:                 ((30 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_2]] {
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
