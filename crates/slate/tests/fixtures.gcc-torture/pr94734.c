/* PR tree-optimization/94734 */

__attribute__((noipa)) int foo(int n) {
  int arr[16], s = 0;
  for (int i = 0; i < n; i++) {
    if (i < 16)
      arr[i] = i;
  }
  for (int i = 0; i < 16; i++)
    s += arr[i];
  return s;
}

__attribute__((noipa)) int bar(int n, int x, unsigned long y, unsigned long z) {
  int arr[16], s = 0;
  arr[4] = 42;
  for (int i = 0; i < n; i++) {
    if (x == (i & 0x25))
      arr[y] = i;
  }
  return arr[z];
}

__attribute__((noipa)) int baz(int n, int x, unsigned long z) {
  int arr[16], s = 0;
  arr[12] = 42;
  for (int i = 0; i < n; i++) {
    if (x == (i & 0x25))
      arr[7] = i;
  }
  return arr[z];
}

int main() {
  if (foo(10374) != 15 * 16 / 2)
    __builtin_abort();
  if (bar(25, 0x25, (unsigned long)0xdeadbeefbeefdeadULL, 4) != 42)
    __builtin_abort();
  if (bar(25, 4, 15, 15) != 22)
    __builtin_abort();
  if (baz(25, 0x25, 12) != 42)
    __builtin_abort();
  if (baz(25, 4, 7) != 22)
    __builtin_abort();
  if (baz(25, 4, 12) != 42)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/94734
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut arr: __SlateAlign16<[i32; 16]> = __SlateAlign16([0 as i32; 16]);
// CHECK-NEXT:     let mut s: i32 = 0 as i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < n {
// CHECK-NEXT:             if i < (16 as i32) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { (arr.0.as_mut_ptr() as *mut i32).offset(i as isize) } = i;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             i = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (16 as i32) {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = s;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 =
// CHECK-NEXT:             [[V_2]] + unsafe { *unsafe { (arr.0.as_mut_ptr() as *mut i32).offset(i as isize) } };
// CHECK-NEXT:         s = [[V_3]];
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut n: i32, mut x: i32, mut y: u64, mut z: u64) -> i32 {
// CHECK-NEXT:     let mut arr: __SlateAlign16<[i32; 16]> = __SlateAlign16([0 as i32; 16]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (arr.0.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) } = 42 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < n {
// CHECK-NEXT:         if x == i & (37 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (arr.0.as_mut_ptr() as *mut i32).offset(y as isize) } = i;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         i = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *unsafe { (arr.0.as_mut_ptr() as *mut i32).offset(z as isize) } }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut n: i32, mut x: i32, mut z: u64) -> i32 {
// CHECK-NEXT:     let mut arr: __SlateAlign16<[i32; 16]> = __SlateAlign16([0 as i32; 16]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (arr.0.as_mut_ptr() as *mut i32).offset((12 as i32) as isize) } = 42 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < n {
// CHECK-NEXT:         if x == i & (37 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (arr.0.as_mut_ptr() as *mut i32).offset((7 as i32) as isize) } = i;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:         i = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *unsafe { (arr.0.as_mut_ptr() as *mut i32).offset(z as isize) } }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(10374 as i32) != (15 as i32) * (16 as i32) / (2 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(
// CHECK-NEXT:         25 as i32,
// CHECK-NEXT:         37 as i32,
// CHECK-NEXT:         16045690984300797613 as u64,
// CHECK-NEXT:         ((4 as i32) as i64) as u64,
// CHECK-NEXT:     ) != (42 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(
// CHECK-NEXT:         25 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         ((15 as i32) as i64) as u64,
// CHECK-NEXT:         ((15 as i32) as i64) as u64,
// CHECK-NEXT:     ) != (22 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if baz(25 as i32, 37 as i32, ((12 as i32) as i64) as u64) != (42 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if baz(25 as i32, 4 as i32, ((7 as i32) as i64) as u64) != (22 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if baz(25 as i32, 4 as i32, ((12 as i32) as i64) as u64) != (42 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
