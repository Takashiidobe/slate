/* Problem originally visible on ia64.

   There is a partial redundancy of "in + 1" that makes GCSE want to
   transform the final while loop to

     p = in + 1;
     tmp = p;
     ...
     goto start;
   top:
     tmp = tmp + 1;
   start:
     in = tmp;
     if (in < p) goto top;

   We miscalculate the number of loop iterations as (p - tmp) = 0
   instead of (p - in) = 1, which results in overflow in the doloop
   optimization.  */

static const char *test(const char *in, char *out) {
  while (1) {
    if (*in == 'a') {
      const char *p = in + 1;
      while (*p == 'x')
        ++p;
      if (*p == 'b')
        return p;
      while (in < p)
        *out++ = *in++;
    }
  }
}

int main() {
  char out[4];
  test("aab", out);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Problem originally visible on ia64.
// CHECK-NEXT: //!
// CHECK-NEXT: //! There is a partial redundancy of "in + 1" that makes GCSE want to
// CHECK-NEXT: //! transform the final while loop to
// CHECK-NEXT: //!
// CHECK-NEXT: //!   p = in + 1;
// CHECK-NEXT: //!   tmp = p;
// CHECK-NEXT: //!   ...
// CHECK-NEXT: //!   goto start;
// CHECK-NEXT: //! top:
// CHECK-NEXT: //!   tmp = tmp + 1;
// CHECK-NEXT: //! start:
// CHECK-NEXT: //!   in = tmp;
// CHECK-NEXT: //!   if (in < p) goto top;
// CHECK-NEXT: //!
// CHECK-NEXT: //! We miscalculate the number of loop iterations as (p - tmp) = 0
// CHECK-NEXT: //! instead of (p - in) = 1, which results in overflow in the doloop
// CHECK-NEXT: //! optimization.
// CHECK-NEXT: fn test(mut r#in: *const i8, mut out: *mut i8) -> *const i8 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:         if ((unsafe { *r#in }) as i32) == (97 as i32) {
// CHECK-NEXT:             let mut p: *const i8 = unsafe { r#in.offset((1 as i32) as isize) };
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while ((unsafe { *p }) as i32) == (120 as i32) {
// CHECK-NEXT:                 let mut [[V_0:__v[0-9]+]]: *const i8 = p;
// CHECK-NEXT:                 let mut [[V_1:__v[0-9]+]]: *const i8 = unsafe { [[V_0]].offset((1 as i32) as isize) };
// CHECK-NEXT:                 p = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:             if ((unsafe { *p }) as i32) == (98 as i32) {
// CHECK-NEXT:                 return p;
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while r#in < p {
// CHECK-NEXT:                 let mut [[V_2:__v[0-9]+]]: *const i8 = r#in;
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: *const i8 = unsafe { [[V_2]].offset((1 as i32) as isize) };
// CHECK-NEXT:                 r#in = [[V_3]];
// CHECK-NEXT:                 let mut [[V_4:__v[0-9]+]]: *mut i8 = out;
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: *mut i8 = unsafe { [[V_4]].offset((1 as i32) as isize) };
// CHECK-NEXT:                 out = [[V_5]];
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *[[V_4]] = unsafe { *[[V_2]] };
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut out: [i8; 4] = [0 as i8; 4];
// CHECK-NEXT:     test(c"aab".as_ptr() as *const i8, out.as_mut_ptr() as *mut i8);
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
