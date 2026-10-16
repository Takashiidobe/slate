
#define func_name(outer, inner, cst)   outer##inner##_##cst
#define func_name_v(outer, inner, cst) outer##inner##_##cst##_v

#define func_decl(outer, inner, cst)                                           \
  int outer##inner##_##cst(int) __attribute__((noipa));                        \
  int outer##inner##_##cst(int a) { return (a op_##inner cst)op_##outer a; }   \
  int outer##inner##_##cst##_v(int) __attribute__((noipa));                    \
  int outer##inner##_##cst##_v(volatile int a) {                               \
    return (a op_##inner cst)op_##outer a;                                     \
  }

#define functions_n(outer, inner)                                              \
  func_decl(outer, inner, 0) func_decl(outer, inner, 1)                        \
      func_decl(outer, inner, 2)

#define functions()                                                            \
  functions_n(eq, eq) functions_n(eq, ne) functions_n(ne, eq)                  \
      functions_n(ne, ne)

#define op_ne !=
#define op_eq ==

#define test(inner, outer, cst, arg)                                           \
  func_name_v(inner, outer, cst)(arg) != func_name(inner, outer, cst)(arg)

functions()

#define tests_n(inner, outer, arg)                                             \
  if (test(inner, outer, 0, arg))                                              \
    __builtin_abort();                                                         \
  if (test(inner, outer, 1, arg))                                              \
    __builtin_abort();                                                         \
  if (test(inner, outer, 2, arg))                                              \
    __builtin_abort();

#define tests(arg)                                                             \
  tests_n(eq, eq, arg) tests_n(eq, ne, arg) tests_n(ne, eq, arg)               \
      tests_n(ne, ne, arg)

    int main() {
  for (int n = -1; n <= 2; n++) {
    tests(n)
  }
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn eqeq_0(mut a: i32) -> i32 {
// CHECK-NEXT:     (((a == (0 as i32)) as i32) == a) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn eqeq_0_v(mut a: i32) -> i32 {
// CHECK-NEXT:     ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) == (0 as i32)) as i32)
// CHECK-NEXT:         == unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn eqeq_1(mut a: i32) -> i32 {
// CHECK-NEXT:     (((a == (1 as i32)) as i32) == a) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn eqeq_1_v(mut a: i32) -> i32 {
// CHECK-NEXT:     ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) == (1 as i32)) as i32)
// CHECK-NEXT:         == unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn eqeq_2(mut a: i32) -> i32 {
// CHECK-NEXT:     (((a == (2 as i32)) as i32) == a) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn eqeq_2_v(mut a: i32) -> i32 {
// CHECK-NEXT:     ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) == (2 as i32)) as i32)
// CHECK-NEXT:         == unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn eqne_0(mut a: i32) -> i32 {
// CHECK-NEXT:     (((a != (0 as i32)) as i32) == a) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn eqne_0_v(mut a: i32) -> i32 {
// CHECK-NEXT:     ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) != (0 as i32)) as i32)
// CHECK-NEXT:         == unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn eqne_1(mut a: i32) -> i32 {
// CHECK-NEXT:     (((a != (1 as i32)) as i32) == a) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn eqne_1_v(mut a: i32) -> i32 {
// CHECK-NEXT:     ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) != (1 as i32)) as i32)
// CHECK-NEXT:         == unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn eqne_2(mut a: i32) -> i32 {
// CHECK-NEXT:     (((a != (2 as i32)) as i32) == a) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn eqne_2_v(mut a: i32) -> i32 {
// CHECK-NEXT:     ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) != (2 as i32)) as i32)
// CHECK-NEXT:         == unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn neeq_0(mut a: i32) -> i32 {
// CHECK-NEXT:     (((a == (0 as i32)) as i32) != a) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn neeq_0_v(mut a: i32) -> i32 {
// CHECK-NEXT:     ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) == (0 as i32)) as i32)
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn neeq_1(mut a: i32) -> i32 {
// CHECK-NEXT:     (((a == (1 as i32)) as i32) != a) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn neeq_1_v(mut a: i32) -> i32 {
// CHECK-NEXT:     ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) == (1 as i32)) as i32)
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn neeq_2(mut a: i32) -> i32 {
// CHECK-NEXT:     (((a == (2 as i32)) as i32) != a) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn neeq_2_v(mut a: i32) -> i32 {
// CHECK-NEXT:     ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) == (2 as i32)) as i32)
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn nene_0(mut a: i32) -> i32 {
// CHECK-NEXT:     (((a != (0 as i32)) as i32) != a) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn nene_0_v(mut a: i32) -> i32 {
// CHECK-NEXT:     ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) != (0 as i32)) as i32)
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn nene_1(mut a: i32) -> i32 {
// CHECK-NEXT:     (((a != (1 as i32)) as i32) != a) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn nene_1_v(mut a: i32) -> i32 {
// CHECK-NEXT:     ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) != (1 as i32)) as i32)
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn nene_2(mut a: i32) -> i32 {
// CHECK-NEXT:     (((a != (2 as i32)) as i32) != a) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn nene_2_v(mut a: i32) -> i32 {
// CHECK-NEXT:     ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) != (2 as i32)) as i32)
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut n: i32 = -(1 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while n <= (2 as i32) {
// CHECK-NEXT:         if eqeq_0_v(n) != eqeq_0(n) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if eqeq_1_v(n) != eqeq_1(n) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if eqeq_2_v(n) != eqeq_2(n) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if eqne_0_v(n) != eqne_0(n) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if eqne_1_v(n) != eqne_1(n) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if eqne_2_v(n) != eqne_2(n) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if neeq_0_v(n) != neeq_0(n) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if neeq_1_v(n) != neeq_1(n) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if neeq_2_v(n) != neeq_2(n) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if nene_0_v(n) != nene_0(n) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if nene_1_v(n) != nene_1(n) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if nene_2_v(n) != nene_2(n) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         n = [[V_1]];
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
