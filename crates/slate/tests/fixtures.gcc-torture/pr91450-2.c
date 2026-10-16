/* PR middle-end/91450 */

__attribute__((noipa)) void foo(int a, int b) {
  unsigned long long r;
  if (__builtin_mul_overflow(a, b, &r))
    __builtin_abort();
  if (r != 0)
    __builtin_abort();
}

__attribute__((noipa)) void bar(int a, int b) {
  unsigned long long r;
  if (a >= 0)
    return;
  if (__builtin_mul_overflow(a, b, &r))
    __builtin_abort();
  if (r != 0)
    __builtin_abort();
}

__attribute__((noipa)) void baz(int a, int b) {
  unsigned long long r;
  if (b >= 0)
    return;
  if (__builtin_mul_overflow(a, b, &r))
    __builtin_abort();
  if (r != 0)
    __builtin_abort();
}

__attribute__((noipa)) void qux(int a, int b) {
  unsigned long long r;
  if (a >= 0)
    return;
  if (b < 0)
    return;
  if (__builtin_mul_overflow(a, b, &r))
    __builtin_abort();
  if (r != 0)
    __builtin_abort();
}

__attribute__((noipa)) void quux(int a, int b) {
  unsigned long long r;
  if (a < 0)
    return;
  if (b >= 0)
    return;
  if (__builtin_mul_overflow(a, b, &r))
    __builtin_abort();
  if (r != 0)
    __builtin_abort();
}

int main() {
  foo(-4, 0);
  foo(0, -4);
  foo(0, 0);
  bar(-4, 0);
  baz(0, -4);
  qux(-4, 0);
  quux(0, -4);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/91450
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut a: i32, mut b: i32) {
// CHECK-NEXT:     let mut r: u64 = 0 as u64;
// CHECK-NEXT:     if {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_mul([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_2]] as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_mul([[T_1]]).is_none() || (([[T_2]] as u64) as i128) != [[T_2]]
// CHECK-NEXT:     } {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if r != (((0 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut a: i32, mut b: i32) {
// CHECK-NEXT:     let mut r: u64 = 0 as u64;
// CHECK-NEXT:     if a >= (0 as i32) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     if {
// CHECK-NEXT:         let [[T_0]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_mul([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_2]] as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_mul([[T_1]]).is_none() || (([[T_2]] as u64) as i128) != [[T_2]]
// CHECK-NEXT:     } {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if r != (((0 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut a: i32, mut b: i32) {
// CHECK-NEXT:     let mut r: u64 = 0 as u64;
// CHECK-NEXT:     if b >= (0 as i32) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     if {
// CHECK-NEXT:         let [[T_0]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_mul([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_2]] as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_mul([[T_1]]).is_none() || (([[T_2]] as u64) as i128) != [[T_2]]
// CHECK-NEXT:     } {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if r != (((0 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn qux(mut a: i32, mut b: i32) {
// CHECK-NEXT:     let mut r: u64 = 0 as u64;
// CHECK-NEXT:     if a >= (0 as i32) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     if b < (0 as i32) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     if {
// CHECK-NEXT:         let [[T_0]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_mul([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_2]] as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_mul([[T_1]]).is_none() || (([[T_2]] as u64) as i128) != [[T_2]]
// CHECK-NEXT:     } {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if r != (((0 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn quux(mut a: i32, mut b: i32) {
// CHECK-NEXT:     let mut r: u64 = 0 as u64;
// CHECK-NEXT:     if a < (0 as i32) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     if b >= (0 as i32) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     if {
// CHECK-NEXT:         let [[T_0]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_mul([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_2]] as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_mul([[T_1]]).is_none() || (([[T_2]] as u64) as i128) != [[T_2]]
// CHECK-NEXT:     } {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if r != (((0 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(-(4 as i32), 0 as i32);
// CHECK-NEXT:     foo(0 as i32, -(4 as i32));
// CHECK-NEXT:     foo(0 as i32, 0 as i32);
// CHECK-NEXT:     bar(-(4 as i32), 0 as i32);
// CHECK-NEXT:     baz(0 as i32, -(4 as i32));
// CHECK-NEXT:     qux(-(4 as i32), 0 as i32);
// CHECK-NEXT:     quux(0 as i32, -(4 as i32));
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
