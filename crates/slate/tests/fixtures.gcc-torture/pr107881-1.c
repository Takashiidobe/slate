#define func(vol, op1, op2, op3)                                               \
  _Bool op1##_##op2##_##op3##_##vol(int a, int b) {                            \
    vol _Bool x = op_##op1(a, b);                                              \
    vol _Bool y = op_##op2(a, b);                                              \
    return op_##op3(x, y);                                                     \
  }

#define op_lt(a, b)  ((a) < (b))
#define op_le(a, b)  ((a) <= (b))
#define op_eq(a, b)  ((a) == (b))
#define op_ne(a, b)  ((a) != (b))
#define op_gt(a, b)  ((a) > (b))
#define op_ge(a, b)  ((a) >= (b))
#define op_xor(a, b) ((a) ^ (b))

#define funcs(a)                                                               \
  a(lt, lt, ne) a(lt, lt, eq) a(lt, lt, xor) a(lt, le, ne) a(lt, le, eq) a(    \
      lt, le, xor) a(lt, gt, ne) a(lt, gt, eq) a(lt, gt, xor) a(lt, ge, ne)    \
      a(lt, ge, eq) a(lt, ge, xor) a(lt, eq, ne) a(lt, eq, eq) a(lt, eq, xor)  \
          a(lt, ne, ne) a(lt, ne, eq) a(lt, ne, xor)                           \
                                                                               \
              a(le, lt, ne) a(le, lt, eq) a(le, lt, xor) a(le, le, ne)         \
                  a(le, le, eq) a(le, le, xor) a(le, gt, ne) a(le, gt, eq) a(  \
                      le, gt, xor) a(le, ge, ne) a(le, ge, eq) a(le, ge, xor)  \
                      a(le, eq, ne) a(le, eq, eq) a(le, eq, xor) a(le, ne, ne) \
                          a(le, ne, eq) a(le, ne, xor)                         \
                                                                               \
                              a(gt, lt, ne) a(gt, lt, eq) a(gt, lt, xor)       \
                                  a(gt, le, ne) a(gt, le, eq) a(               \
                                      gt, le, xor) a(gt, gt, ne) a(gt, gt, eq) \
                                      a(gt, gt, xor) a(gt, ge, ne) a(          \
                                          gt, ge, eq) a(gt, ge, xor)           \
                                          a(gt, eq, ne) a(gt, eq, eq) a(       \
                                              gt, eq,                          \
                                              xor) a(gt, ne,                   \
                                                     ne) a(gt, ne,             \
                                                           eq) a(gt, ne, xor)  \
                                                                               \
                                              a(ge, lt, ne) a(ge, lt, eq) a(   \
                                                  ge, lt,                      \
                                                  xor) a(ge, le,               \
                                                         ne) a(ge, le, eq)     \
                                                  a(ge, le, xor) a(ge, gt, ne) \
                                                      a(ge, gt, eq) a(         \
                                                          ge, gt,              \
                                                          xor) a(ge, ge, ne)   \
                                                          a(ge, ge, eq) a(     \
                                                              ge, ge,          \
                                                              xor) a(ge, eq,   \
                                                                     ne) a(ge, \
                                                                           eq, \
                                                                           eq) \
                                                              a(ge, eq,        \
                                                                xor) a(ge, ne, \
                                                                       ne)     \
                                                                  a(ge, ne,    \
                                                                    eq) a(ge,  \
                                                                          ne,  \
                                                                          xor)

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
// CHECK-NEXT: fn lt_lt_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_lt_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_lt_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_lt_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_lt_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_lt_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_le_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_le_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_le_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_le_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_le_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_le_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_gt_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_ge_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_ge_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_ge_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_ge_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_ge_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_ge_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_eq_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_eq_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_eq_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_eq_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_eq_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_eq_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_ne_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_ne_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_ne_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_ne_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_ne_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn lt_ne_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a < b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_lt_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_lt_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_lt_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_lt_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_lt_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_lt_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_le_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_le_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_le_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_le_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_le_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_le_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_gt_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_gt_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_gt_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_gt_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_gt_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_gt_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ge_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_eq_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_eq_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_eq_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_eq_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_eq_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_eq_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ne_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ne_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ne_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ne_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ne_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn le_ne_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a <= b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_lt_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_le_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_le_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_le_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_le_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_le_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_le_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_gt_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_gt_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_gt_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_gt_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_gt_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_gt_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_ge_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_ge_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_ge_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_ge_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_ge_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_ge_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_eq_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_eq_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_eq_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_eq_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_eq_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_eq_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_ne_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_ne_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_ne_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_ne_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_ne_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gt_ne_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a > b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_lt_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_lt_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_lt_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_lt_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_lt_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_lt_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a < b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_le_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a <= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_gt_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_gt_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_gt_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_gt_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_gt_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_gt_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a > b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_ge_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_ge_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_ge_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_ge_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_ge_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_ge_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a >= b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_eq_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_eq_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_eq_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_eq_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_eq_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_eq_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a == b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_ne_ne_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     (x as i32) != (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_ne_ne_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         != ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_ne_eq_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     (x as i32) == (y as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_ne_eq_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         == ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_ne_xor_(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     (x as i32) ^ (y as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ge_ne_xor_volatile(mut a: i32, mut b: i32) -> bool {
// CHECK-NEXT:     let mut x: bool = a >= b;
// CHECK-NEXT:     let mut y: bool = a != b;
// CHECK-NEXT:     ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) as i32)
// CHECK-NEXT:         ^ ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = -(10 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while x < (10 as i32) {
// CHECK-NEXT:         let mut y: i32 = -(10 as i32);
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while y < (10 as i32) {
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_lt_ne_(x, y) as i32) != (lt_lt_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_lt_eq_(x, y) as i32) != (lt_lt_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_lt_xor_(x, y) as i32) != (lt_lt_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_le_ne_(x, y) as i32) != (lt_le_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_le_eq_(x, y) as i32) != (lt_le_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_le_xor_(x, y) as i32) != (lt_le_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_gt_ne_(x, y) as i32) != (lt_gt_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_gt_eq_(x, y) as i32) != (lt_gt_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_gt_xor_(x, y) as i32) != (lt_gt_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_11:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_ge_ne_(x, y) as i32) != (lt_ge_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_12:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_ge_eq_(x, y) as i32) != (lt_ge_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_13:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_ge_xor_(x, y) as i32) != (lt_ge_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_14:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_eq_ne_(x, y) as i32) != (lt_eq_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_15:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_eq_eq_(x, y) as i32) != (lt_eq_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_16:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_eq_xor_(x, y) as i32) != (lt_eq_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_17:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_ne_ne_(x, y) as i32) != (lt_ne_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_18:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_ne_eq_(x, y) as i32) != (lt_ne_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_19:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (lt_ne_xor_(x, y) as i32) != (lt_ne_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_20:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_lt_ne_(x, y) as i32) != (le_lt_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_21:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_lt_eq_(x, y) as i32) != (le_lt_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_22:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_lt_xor_(x, y) as i32) != (le_lt_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_23:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_le_ne_(x, y) as i32) != (le_le_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_24:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_le_eq_(x, y) as i32) != (le_le_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_25:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_le_xor_(x, y) as i32) != (le_le_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_26:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_gt_ne_(x, y) as i32) != (le_gt_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_27:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_gt_eq_(x, y) as i32) != (le_gt_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_28:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_gt_xor_(x, y) as i32) != (le_gt_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_29:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_ge_ne_(x, y) as i32) != (le_ge_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_30:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_ge_eq_(x, y) as i32) != (le_ge_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_31:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_ge_xor_(x, y) as i32) != (le_ge_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_32:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_eq_ne_(x, y) as i32) != (le_eq_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_33:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_eq_eq_(x, y) as i32) != (le_eq_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_34:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_eq_xor_(x, y) as i32) != (le_eq_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_35:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_ne_ne_(x, y) as i32) != (le_ne_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_36:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_ne_eq_(x, y) as i32) != (le_ne_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_37:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (le_ne_xor_(x, y) as i32) != (le_ne_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_38:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_lt_ne_(x, y) as i32) != (gt_lt_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_39:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_lt_eq_(x, y) as i32) != (gt_lt_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_40:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_lt_xor_(x, y) as i32) != (gt_lt_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_41:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_le_ne_(x, y) as i32) != (gt_le_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_42:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_le_eq_(x, y) as i32) != (gt_le_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_43:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_le_xor_(x, y) as i32) != (gt_le_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_44:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_gt_ne_(x, y) as i32) != (gt_gt_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_45:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_gt_eq_(x, y) as i32) != (gt_gt_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_46:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_gt_xor_(x, y) as i32) != (gt_gt_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_47:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_ge_ne_(x, y) as i32) != (gt_ge_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_48:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_ge_eq_(x, y) as i32) != (gt_ge_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_49:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_ge_xor_(x, y) as i32) != (gt_ge_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_50:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_eq_ne_(x, y) as i32) != (gt_eq_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_51:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_eq_eq_(x, y) as i32) != (gt_eq_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_52:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_eq_xor_(x, y) as i32) != (gt_eq_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_53:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_ne_ne_(x, y) as i32) != (gt_ne_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_54:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_ne_eq_(x, y) as i32) != (gt_ne_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_55:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (gt_ne_xor_(x, y) as i32) != (gt_ne_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_56:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_lt_ne_(x, y) as i32) != (ge_lt_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_57:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_lt_eq_(x, y) as i32) != (ge_lt_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_58:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_lt_xor_(x, y) as i32) != (ge_lt_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_59:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_le_ne_(x, y) as i32) != (ge_le_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_60:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_le_eq_(x, y) as i32) != (ge_le_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_61:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_le_xor_(x, y) as i32) != (ge_le_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_62:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_gt_ne_(x, y) as i32) != (ge_gt_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_63:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_gt_eq_(x, y) as i32) != (ge_gt_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_64:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_gt_xor_(x, y) as i32) != (ge_gt_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_65:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_ge_ne_(x, y) as i32) != (ge_ge_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_66:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_ge_eq_(x, y) as i32) != (ge_ge_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_67:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_ge_xor_(x, y) as i32) != (ge_ge_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_68:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_eq_ne_(x, y) as i32) != (ge_eq_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_69:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_eq_eq_(x, y) as i32) != (ge_eq_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_70:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_eq_xor_(x, y) as i32) != (ge_eq_xor_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_71:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_ne_ne_(x, y) as i32) != (ge_ne_ne_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_72:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_ne_eq_(x, y) as i32) != (ge_ne_eq_volatile(x, y) as i32) {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_73:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if (ge_ne_xor_(x, y) as i32) != (ge_ne_xor_volatile(x, y) as i32) {
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
