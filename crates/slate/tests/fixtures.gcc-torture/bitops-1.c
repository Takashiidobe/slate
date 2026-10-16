/* PR tree-optimization/110726 */

#define DECLS(n, VOL)                                                          \
  __attribute__((noinline, noclone)) int h##n(VOL int A, VOL int B) {          \
    return (A | B) & (A == B);                                                 \
  }                                                                            \
  __attribute__((noinline, noclone)) int i##n(VOL int A, VOL int B) {          \
    return A | (A == B);                                                       \
  }                                                                            \
  __attribute__((noinline, noclone)) int k##n(VOL int A, VOL int B) {          \
    return (A & B) | (A == B);                                                 \
  }

DECLS(0, )
DECLS(1, volatile)

int values[]  = {0, 1, 2, 3, -1, -2, -3, 0x10080};
int numvalues = sizeof(values) / sizeof(values[0]);

int main() {
  for (int A = 0; A < numvalues; A++)
    for (int B = 0; B < numvalues; B++) {
      int a = values[A];
      int b = values[B];
      if (h0(a, b) != h1(a, b))
        __builtin_abort();
      if (i0(a, b) != i1(a, b))
        __builtin_abort();
      if (k0(a, b) != k1(a, b))
        __builtin_abort();
    }
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/110726
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn h0(mut A: i32, mut B: i32) -> i32 {
// CHECK-NEXT:     (A | B) & ((A == B) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn i0(mut A: i32, mut B: i32) -> i32 {
// CHECK-NEXT:     A | ((A == B) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn k0(mut A: i32, mut B: i32) -> i32 {
// CHECK-NEXT:     A & B | ((A == B) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn h1(mut A: i32, mut B: i32) -> i32 {
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(A)) })
// CHECK-NEXT:         | unsafe { std::ptr::read_volatile(std::ptr::addr_of!(B)) })
// CHECK-NEXT:         & (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(A)) })
// CHECK-NEXT:             == unsafe { std::ptr::read_volatile(std::ptr::addr_of!(B)) }) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn i1(mut A: i32, mut B: i32) -> i32 {
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(A)) })
// CHECK-NEXT:         | (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(A)) })
// CHECK-NEXT:             == unsafe { std::ptr::read_volatile(std::ptr::addr_of!(B)) }) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn k1(mut A: i32, mut B: i32) -> i32 {
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(A)) })
// CHECK-NEXT:         & unsafe { std::ptr::read_volatile(std::ptr::addr_of!(B)) }
// CHECK-NEXT:         | (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(A)) })
// CHECK-NEXT:             == unsafe { std::ptr::read_volatile(std::ptr::addr_of!(B)) }) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut values: __SlateAlign16<[i32; 8]> = __SlateAlign16([
// CHECK-NEXT:     0 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     3 as i32,
// CHECK-NEXT:     -(1 as i32),
// CHECK-NEXT:     -(2 as i32),
// CHECK-NEXT:     -(3 as i32),
// CHECK-NEXT:     65664 as i32,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut numvalues: i32 = (((32 as u64) / (4 as u64)) as u32) as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut A: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while A < unsafe { numvalues } {
// CHECK-NEXT:         let mut B: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while B < unsafe { numvalues } {
// CHECK-NEXT:             let mut a: i32 = unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(values.0) as *mut i32 }.offset(A as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:             let mut b: i32 = unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(values.0) as *mut i32 }.offset(B as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:             if h0(a, b) != h1(a, b) {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             if i0(a, b) != i1(a, b) {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             if k0(a, b) != k1(a, b) {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = B;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             B = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = A;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         A = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
