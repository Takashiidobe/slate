int  array[1000];
void test(int a) {
  if (__builtin_expect(a > 3, 1))
    return;
  for (int i = 0; i < a; i++)
    array[i] = i;
}
void test2(int a) {
  if (__builtin_expect(a > 10, 1))
    return;
  for (int i = 0; i < a; i++)
    array[i] = i;
}
int main() {
  test(1);
  test(2);
  test(3);
  test2(10);
  if (array[9] != 9)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_expect([[V_0:__v[0-9]+]]: i64, [[V_1:__v[0-9]+]]: i64) -> i64;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut array: __SlateAlign16<[i32; 1000]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn test(mut a: i32) {
// CHECK-NEXT:     if ((a > (3 as i32)) as i64) != (0 as i64) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < a {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(array.0) as *mut i32 }.offset(i as isize)
// CHECK-NEXT:             } = i;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test2(mut a: i32) {
// CHECK-NEXT:     if ((a > (10 as i32)) as i64) != (0 as i64) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < a {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(array.0) as *mut i32 }.offset(i as isize)
// CHECK-NEXT:             } = i;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test(1 as i32);
// CHECK-NEXT:     test(2 as i32);
// CHECK-NEXT:     test(3 as i32);
// CHECK-NEXT:     test2(10 as i32);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array.0) as *mut i32 }
// CHECK-NEXT:                 .offset((9 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (9 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
