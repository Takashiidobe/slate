/* PR tree-optimization/58209 */

extern void             abort(void);
typedef __INTPTR_TYPE__ T;
T                       buf[1024];

T *foo(T n) {
  if (n == 0)
    return (T *)buf;
  T s = (T)foo(n - 1);
  return (T *)(s + sizeof(T));
}

T *bar(T n) {
  if (n == 0)
    return buf;
  return foo(n - 1) + 1;
}

int main() {
  int i;
  for (i = 0; i < 27; i++)
    if (foo(i) != buf + i || bar(i) != buf + i)
      abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/58209
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut buf: __SlateAlign16<[i64; 1024]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut n: i64) -> *mut i64 {
// CHECK-NEXT:     if n == ((0 as i32) as i64) {
// CHECK-NEXT:         return unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i64 };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut s: i64 = foo(n - ((1 as i32) as i64)) as i64;
// CHECK-NEXT:     (s as u64).wrapping_add(8 as u64) as *mut i64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut n: i64) -> *mut i64 {
// CHECK-NEXT:     if n == ((0 as i32) as i64) {
// CHECK-NEXT:         return unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i64 };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { foo(n - ((1 as i32) as i64)).offset((1 as i32) as isize) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (27 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:         if foo(i as i64)
// CHECK-NEXT:             != unsafe { unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i64 }.offset(i as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             [[V_0]] = true as bool;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_0]] = bar(i as i64)
// CHECK-NEXT:                 != unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i64 }.offset(i as isize)
// CHECK-NEXT:                 };
// CHECK-NEXT:         }
// CHECK-NEXT:         if [[V_0]] {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
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
