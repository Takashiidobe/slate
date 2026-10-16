/* PR tree-optimization/49073 */

extern void abort(void);
int         a[] = {1, 2, 3, 4, 5, 6, 7}, c;

int main() {
  int   d = 1, i = 1;
  _Bool f = 0;
  do {
    d = a[i];
    if (f && d == 4) {
      ++c;
      break;
    }
    i++;
    f = (d == 3);
  } while (d < 7);
  if (c != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/49073
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: __SlateAlign16<[i32; 7]> = __SlateAlign16([
// CHECK-NEXT:     1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: i32 = 1 as i32;
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     let mut f: bool = (0 as i32) != (0 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         d = unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }.offset(i as isize) }
// CHECK-NEXT:         };
// CHECK-NEXT:         if f && d == (4 as i32) {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 c = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:         f = d == (3 as i32);
// CHECK-NEXT:         if !(d < (7 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { c }) != (1 as i32) {
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
