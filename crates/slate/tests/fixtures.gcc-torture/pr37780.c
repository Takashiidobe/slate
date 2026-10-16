/* PR middle-end/37780.  */

#define VAL (8 * sizeof(int))

int __attribute__((noinline, noclone)) fooctz(int i) {
  return (i == 0) ? VAL : __builtin_ctz(i);
}

int __attribute__((noinline, noclone)) fooctz2(int i) {
  return (i != 0) ? __builtin_ctz(i) : VAL;
}

unsigned int __attribute__((noinline, noclone)) fooctz3(unsigned int i) {
  return (i > 0) ? __builtin_ctz(i) : VAL;
}

int __attribute__((noinline, noclone)) fooclz(int i) {
  return (i == 0) ? VAL : __builtin_clz(i);
}

int __attribute__((noinline, noclone)) fooclz2(int i) {
  return (i != 0) ? __builtin_clz(i) : VAL;
}

unsigned int __attribute__((noinline, noclone)) fooclz3(unsigned int i) {
  return (i > 0) ? __builtin_clz(i) : VAL;
}

int main(void) {
  if (fooctz(0) != VAL || fooctz2(0) != VAL || fooctz3(0) != VAL ||
      fooclz(0) != VAL || fooclz2(0) != VAL || fooclz3(0) != VAL)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/37780.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fooctz(mut i: i32) -> i32 {
// CHECK-NEXT:     ((if i == (0 as i32) {
// CHECK-NEXT:         (((8 as i32) as i64) as u64).wrapping_mul(4 as u64)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (((i as u32).trailing_zeros() as i32) as i64) as u64
// CHECK-NEXT:     }) as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fooctz2(mut i: i32) -> i32 {
// CHECK-NEXT:     ((if i != (0 as i32) {
// CHECK-NEXT:         (((i as u32).trailing_zeros() as i32) as i64) as u64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (((8 as i32) as i64) as u64).wrapping_mul(4 as u64)
// CHECK-NEXT:     }) as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fooctz3(mut i: u32) -> u32 {
// CHECK-NEXT:     (if i > ((0 as i32) as u32) {
// CHECK-NEXT:         ((i.trailing_zeros() as i32) as i64) as u64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (((8 as i32) as i64) as u64).wrapping_mul(4 as u64)
// CHECK-NEXT:     }) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fooclz(mut i: i32) -> i32 {
// CHECK-NEXT:     ((if i == (0 as i32) {
// CHECK-NEXT:         (((8 as i32) as i64) as u64).wrapping_mul(4 as u64)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (((i as u32).leading_zeros() as i32) as i64) as u64
// CHECK-NEXT:     }) as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fooclz2(mut i: i32) -> i32 {
// CHECK-NEXT:     ((if i != (0 as i32) {
// CHECK-NEXT:         (((i as u32).leading_zeros() as i32) as i64) as u64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (((8 as i32) as i64) as u64).wrapping_mul(4 as u64)
// CHECK-NEXT:     }) as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fooclz3(mut i: u32) -> u32 {
// CHECK-NEXT:     (if i > ((0 as i32) as u32) {
// CHECK-NEXT:         ((i.leading_zeros() as i32) as i64) as u64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (((8 as i32) as i64) as u64).wrapping_mul(4 as u64)
// CHECK-NEXT:     }) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if ((fooctz(0 as i32) as i64) as u64) != (((8 as i32) as i64) as u64).wrapping_mul(4 as u64) {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = ((fooctz2(0 as i32) as i64) as u64)
// CHECK-NEXT:             != (((8 as i32) as i64) as u64).wrapping_mul(4 as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = (fooctz3((0 as i32) as u32) as u64)
// CHECK-NEXT:             != (((8 as i32) as i64) as u64).wrapping_mul(4 as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = ((fooclz(0 as i32) as i64) as u64)
// CHECK-NEXT:             != (((8 as i32) as i64) as u64).wrapping_mul(4 as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         [[V_3]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = ((fooclz2(0 as i32) as i64) as u64)
// CHECK-NEXT:             != (((8 as i32) as i64) as u64).wrapping_mul(4 as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_3]] {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = (fooclz3((0 as i32) as u32) as u64)
// CHECK-NEXT:             != (((8 as i32) as i64) as u64).wrapping_mul(4 as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_4]] {
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
