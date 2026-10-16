/* PR tree-optimization/65418 */

__attribute__((noinline, noclone)) int foo(int x) {
  if (x == -216 || x == -211 || x == -218 || x == -205 || x == -223)
    return 1;
  return 0;
}

int main() {
  volatile int i;
  for (i = -230; i < -200; i++)
    if (foo(i) !=
        (i == -216 || i == -211 || i == -218 || i == -205 || i == -223))
      __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/65418
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) -> i32 {
// CHECK-NEXT:     if x == -(216 as i32)
// CHECK-NEXT:         || x == -(211 as i32)
// CHECK-NEXT:         || x == -(218 as i32)
// CHECK-NEXT:         || x == -(205 as i32)
// CHECK-NEXT:         || x == -(223 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(i), -(230 as i32)) };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) })
// CHECK-NEXT:         < -(200 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         if foo(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) })
// CHECK-NEXT:             != (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) }) == -(216 as i32)
// CHECK-NEXT:                 || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) }) == -(211 as i32)
// CHECK-NEXT:                 || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) }) == -(218 as i32)
// CHECK-NEXT:                 || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) }) == -(205 as i32)
// CHECK-NEXT:                 || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) }) == -(223 as i32))
// CHECK-NEXT:                 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(i), [[V_1]]) };
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
