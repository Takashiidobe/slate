int foo(int a) {
  int b = a == 0;
  return (a & b);
}

#define function(vol, cst)                                                     \
  __attribute__((noipa)) _Bool func_##cst##_##vol(vol int a) {                 \
    vol int b = a == cst;                                                      \
    return (a & b);                                                            \
  }

#define funcdefs(cst) function(, cst) function(volatile, cst)

#define funcs(f) f(0) f(1) f(5)

funcs(funcdefs)

#define test(cst)                                                              \
  do {                                                                         \
    if (func_##cst##_(a) != func_##cst##_volatile(a))                          \
      __builtin_abort();                                                       \
  } while (0);
    int main(void) {
  for (int a = -10; a <= 10; a++) {
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
// CHECK-NEXT: fn foo(mut a: i32) -> i32 {
// CHECK-NEXT:     let mut b: i32 = (a == (0 as i32)) as i32;
// CHECK-NEXT:     a & b
// CHECK-NEXT: }
// CHECK-NEXT: fn func_0_(mut a: i32) -> bool {
// CHECK-NEXT:     let mut b: i32 = (a == (0 as i32)) as i32;
// CHECK-NEXT:     a & b != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn func_0_volatile(mut a: i32) -> bool {
// CHECK-NEXT:     let mut b: i32 = ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) })
// CHECK-NEXT:         == (0 as i32)) as i32;
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) })
// CHECK-NEXT:         & unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) } != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn func_1_(mut a: i32) -> bool {
// CHECK-NEXT:     let mut b: i32 = (a == (1 as i32)) as i32;
// CHECK-NEXT:     a & b != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn func_1_volatile(mut a: i32) -> bool {
// CHECK-NEXT:     let mut b: i32 = ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) })
// CHECK-NEXT:         == (1 as i32)) as i32;
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) })
// CHECK-NEXT:         & unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) } != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn func_5_(mut a: i32) -> bool {
// CHECK-NEXT:     let mut b: i32 = (a == (5 as i32)) as i32;
// CHECK-NEXT:     a & b != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn func_5_volatile(mut a: i32) -> bool {
// CHECK-NEXT:     let mut b: i32 = ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) })
// CHECK-NEXT:         == (5 as i32)) as i32;
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) })
// CHECK-NEXT:         & unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) } != (0 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = -(10 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while a <= (10 as i32) {
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             if (func_0_(a) as i32) != (func_0_volatile(a) as i32) {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             if (func_1_(a) as i32) != (func_1_volatile(a) as i32) {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             if (func_5_(a) as i32) != (func_5_volatile(a) as i32) {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         a = [[V_1]];
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
