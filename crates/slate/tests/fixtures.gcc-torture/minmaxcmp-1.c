#define func(vol, op1, op2)                                                    \
  _Bool op1##_##op2##_##vol(int a, int b) {                                    \
    vol int x = op_##op1(a, b);                                                \
    return op_##op2(x, a);                                                     \
  }

#define op_lt(a, b)  ((a) < (b))
#define op_le(a, b)  ((a) <= (b))
#define op_eq(a, b)  ((a) == (b))
#define op_ne(a, b)  ((a) != (b))
#define op_gt(a, b)  ((a) > (b))
#define op_ge(a, b)  ((a) >= (b))
#define op_min(a, b) ((a) < (b) ? (a) : (b))
#define op_max(a, b) ((a) > (b) ? (a) : (b))

#define funcs(a)                                                               \
  a(min, lt) a(max, lt) a(min, gt) a(max, gt) a(min, le) a(max, le) a(min, ge) \
      a(max, ge) a(min, ne) a(max, ne) a(min, eq) a(max, eq)

#define funcs1(a, b) func(, a, b) func(volatile, a, b)

funcs(funcs1)

#define test(op1, op2)                                                         \
  do {                                                                         \
    if (op1##_##op2##_(x, y) != op1##_##op2##_volatile(x, y))                  \
      __builtin_abort();                                                       \
  } while (0);

    int main() {
  for (int x = -10; x < 10; x++)
    for (int y = -10; y < 10; y++) {
      funcs(test)
    }
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn min_lt_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a < b { a } else { b };
// CHECK-NEXT:     x < a
// CHECK-NEXT: }
// CHECK-NEXT: fn min_lt_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a < b { a } else { b };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) < a
// CHECK-NEXT: }
// CHECK-NEXT: fn max_lt_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a > b { a } else { b };
// CHECK-NEXT:     x < a
// CHECK-NEXT: }
// CHECK-NEXT: fn max_lt_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a > b { a } else { b };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) < a
// CHECK-NEXT: }
// CHECK-NEXT: fn min_gt_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a < b { a } else { b };
// CHECK-NEXT:     x > a
// CHECK-NEXT: }
// CHECK-NEXT: fn min_gt_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a < b { a } else { b };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) > a
// CHECK-NEXT: }
// CHECK-NEXT: fn max_gt_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a > b { a } else { b };
// CHECK-NEXT:     x > a
// CHECK-NEXT: }
// CHECK-NEXT: fn max_gt_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a > b { a } else { b };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) > a
// CHECK-NEXT: }
// CHECK-NEXT: fn min_le_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a < b { a } else { b };
// CHECK-NEXT:     x <= a
// CHECK-NEXT: }
// CHECK-NEXT: fn min_le_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a < b { a } else { b };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) <= a
// CHECK-NEXT: }
// CHECK-NEXT: fn max_le_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a > b { a } else { b };
// CHECK-NEXT:     x <= a
// CHECK-NEXT: }
// CHECK-NEXT: fn max_le_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a > b { a } else { b };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) <= a
// CHECK-NEXT: }
// CHECK-NEXT: fn min_ge_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a < b { a } else { b };
// CHECK-NEXT:     x >= a
// CHECK-NEXT: }
// CHECK-NEXT: fn min_ge_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a < b { a } else { b };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) >= a
// CHECK-NEXT: }
// CHECK-NEXT: fn max_ge_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a > b { a } else { b };
// CHECK-NEXT:     x >= a
// CHECK-NEXT: }
// CHECK-NEXT: fn max_ge_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a > b { a } else { b };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) >= a
// CHECK-NEXT: }
// CHECK-NEXT: fn min_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a < b { a } else { b };
// CHECK-NEXT:     x != a
// CHECK-NEXT: }
// CHECK-NEXT: fn min_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a < b { a } else { b };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) != a
// CHECK-NEXT: }
// CHECK-NEXT: fn max_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a > b { a } else { b };
// CHECK-NEXT:     x != a
// CHECK-NEXT: }
// CHECK-NEXT: fn max_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a > b { a } else { b };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) != a
// CHECK-NEXT: }
// CHECK-NEXT: fn min_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a < b { a } else { b };
// CHECK-NEXT:     x == a
// CHECK-NEXT: }
// CHECK-NEXT: fn min_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a < b { a } else { b };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) == a
// CHECK-NEXT: }
// CHECK-NEXT: fn max_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a > b { a } else { b };
// CHECK-NEXT:     x == a
// CHECK-NEXT: }
// CHECK-NEXT: fn max_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: i32 = if a > b { a } else { b };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) == a
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = -(10 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while x < (10 as i32) {
// CHECK-NEXT:         let mut y: i32 = -(10 as i32);
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while y < (10 as i32) {
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (min_lt_(x, y) as i32) != (min_lt_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (max_lt_(x, y) as i32) != (max_lt_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (min_gt_(x, y) as i32) != (min_gt_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (max_gt_(x, y) as i32) != (max_gt_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (min_le_(x, y) as i32) != (min_le_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (max_le_(x, y) as i32) != (max_le_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (min_ge_(x, y) as i32) != (min_ge_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (max_ge_(x, y) as i32) != (max_ge_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (min_ne_(x, y) as i32) != (min_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_11:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (max_ne_(x, y) as i32) != (max_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_12:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (min_eq_(x, y) as i32) != (min_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_13:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (max_eq_(x, y) as i32) != (max_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = y;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             y = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = x;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         x = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
