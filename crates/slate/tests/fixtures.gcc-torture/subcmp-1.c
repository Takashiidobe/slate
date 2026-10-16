#define func(vol, op1, op2, op3)                                               \
  _Bool op1##_##op2##_##op3##_##vol(int a, int b) {                            \
    vol _Bool x = op_##op1(a, b);                                              \
    vol _Bool y = op_##op2(a, b);                                              \
    return op_##op3(x - y, 0);                                                 \
  }

#define op_lt(a, b) ((a) < (b))
#define op_le(a, b) ((a) <= (b))
#define op_gt(a, b) ((a) > (b))
#define op_ge(a, b) ((a) >= (b))

#define funcs(a)                                                               \
  a(gt, lt, lt) a(gt, lt, le) a(gt, lt, gt) a(gt, lt, ge)                      \
                                                                               \
      a(ge, le, lt) a(ge, le, le) a(ge, le, gt) a(ge, le, ge)                  \
                                                                               \
          a(lt, gt, lt) a(lt, gt, le) a(lt, gt, gt) a(lt, gt, ge)              \
                                                                               \
              a(le, ge, lt) a(le, ge, le) a(le, ge, gt) a(le, ge, ge)

#define funcs1(a, b, c) func(, a, b, c) func(volatile, a, b, c)

funcs(funcs1)

#define test(op1, op2, op3)                                                    \
  do {                                                                         \
    if (op1##_##op2##_##op3##_(x, y) != op1##_##op2##_##op3##_volatile(x, y))  \
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
// CHECK-NEXT: fn gt_lt_lt_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) - (y as i32) < (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_lt_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         < (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_le_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) - (y as i32) <= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_le_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         <= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_gt_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) - (y as i32) > (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_gt_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         > (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_ge_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) - (y as i32) >= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_ge_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         >= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_lt_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) - (y as i32) < (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_lt_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         < (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_le_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) - (y as i32) <= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_le_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         <= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_gt_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) - (y as i32) > (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_gt_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         > (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_ge_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) - (y as i32) >= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_ge_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         >= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_lt_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) - (y as i32) < (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_lt_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         < (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_le_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) - (y as i32) <= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_le_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         <= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_gt_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) - (y as i32) > (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_gt_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         > (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_ge_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) - (y as i32) >= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_ge_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         >= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_lt_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) - (y as i32) < (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_lt_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         < (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_le_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) - (y as i32) <= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_le_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         <= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_gt_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) - (y as i32) > (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_gt_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         > (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_ge_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) - (y as i32) >= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_ge_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         - ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         >= (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = -(10 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while x < (10 as i32) {
// CHECK-NEXT:         let mut y: i32 = -(10 as i32);
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while y < (10 as i32) {
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_lt_lt_(x, y) as i32) != (gt_lt_lt_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_lt_le_(x, y) as i32) != (gt_lt_le_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_lt_gt_(x, y) as i32) != (gt_lt_gt_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_lt_ge_(x, y) as i32) != (gt_lt_ge_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_le_lt_(x, y) as i32) != (ge_le_lt_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_le_le_(x, y) as i32) != (ge_le_le_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_le_gt_(x, y) as i32) != (ge_le_gt_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_le_ge_(x, y) as i32) != (ge_le_ge_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_gt_lt_(x, y) as i32) != (lt_gt_lt_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_11:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_gt_le_(x, y) as i32) != (lt_gt_le_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_12:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_gt_gt_(x, y) as i32) != (lt_gt_gt_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_13:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_gt_ge_(x, y) as i32) != (lt_gt_ge_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_14:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_ge_lt_(x, y) as i32) != (le_ge_lt_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_15:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_ge_le_(x, y) as i32) != (le_ge_le_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_16:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_ge_gt_(x, y) as i32) != (le_ge_gt_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_17:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_ge_ge_(x, y) as i32) != (le_ge_ge_volatile(x, y) as i32) {
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
