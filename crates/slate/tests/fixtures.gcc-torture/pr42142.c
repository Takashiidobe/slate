int __attribute__((noinline, noclone)) sort(int L) {
  int end[2] =
      {
          10,
          10,
      },
      i = 0, R;
  while (i < 2) {
    R = end[i];
    if (L < R) {
      end[i + 1] = 1;
      end[i]     = 10;
      ++i;
    } else
      break;
  }
  return i;
}
extern void abort(void);
int         main() {
  if (sort(5) != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn sort(mut L: i32) -> i32 {
// CHECK-NEXT:     let mut end: [i32; 2] = [10 as i32, 10 as i32];
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut R: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         R = unsafe { *unsafe { (end.as_mut_ptr() as *mut i32).offset(i as isize) } };
// CHECK-NEXT:         if L < R {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     (end.as_mut_ptr() as *mut i32).offset((i + (1 as i32)) as isize)
// CHECK-NEXT:                 } = 1 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (end.as_mut_ptr() as *mut i32).offset(i as isize) } = 10
// CHECK-NEXT:                     as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             i = [[V_1]];
// CHECK-NEXT:         } else {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     i
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if sort(5 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
