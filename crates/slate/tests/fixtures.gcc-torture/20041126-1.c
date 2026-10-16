extern int  abs(int);
extern void abort(void);

void check(int *p) {
  int i;
  for (i = 0; i < 5; ++i)
    if (p[i])
      abort();
  for (; i < 10; ++i)
    if (p[i] != i + 1)
      abort();
}

int main(void) {
  int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int i;

  for (i = -5; i < 0; i++)
    a[abs(i - 10) - 11] = 0;
  check(a);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abs([[V_0:__v[0-9]+]]: i32) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn check(mut p: *mut i32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (5 as i32) {
// CHECK-NEXT:         if (unsafe { *unsafe { p.offset(i as isize) } }) != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (10 as i32) {
// CHECK-NEXT:         if (unsafe { *unsafe { p.offset(i as isize) } }) != i + (1 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i32; 10]> = __SlateAlign16([
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         6 as i32,
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:         8 as i32,
// CHECK-NEXT:         9 as i32,
// CHECK-NEXT:         10 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut i: i32 = -(5 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 (a.0.as_mut_ptr() as *mut i32)
// CHECK-NEXT:                     .offset(((unsafe { abs(i - (10 as i32)) }) - (11 as i32)) as isize)
// CHECK-NEXT:             } = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     check(a.0.as_mut_ptr() as *mut i32);
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
