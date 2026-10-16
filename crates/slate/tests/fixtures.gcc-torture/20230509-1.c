int __attribute__((noipa)) f(unsigned a, int b) {
  if (a < 0)
    __builtin_unreachable();
  if (a > 30)
    __builtin_unreachable();
  int t = a;
  if (b)
    t = 100;
  else if (a != 0)
    t = a;
  else
    t = 1;
  return t;
}

int main(void) {
  if (f(0, 0) != 1)
    __builtin_abort();
  if (f(1, 0) != 1)
    __builtin_abort();
  if (f(0, 1) != 100)
    __builtin_abort();
  if (f(1, 0) != 1)
    __builtin_abort();
  if (f(30, 0) != 30)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_unreachable();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut a: u32, mut b: i32) -> i32 {
// CHECK-NEXT:     if a < ((0 as i32) as u32) {
// CHECK-NEXT:         unsafe { std::hint::unreachable_unchecked() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if a > ((30 as i32) as u32) {
// CHECK-NEXT:         unsafe { std::hint::unreachable_unchecked() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut t: i32 = a as i32;
// CHECK-NEXT:     if b != (0 as i32) {
// CHECK-NEXT:         t = 100 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if a != ((0 as i32) as u32) {
// CHECK-NEXT:             t = a as i32;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             t = 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f((0 as i32) as u32, 0 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f((1 as i32) as u32, 0 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f((0 as i32) as u32, 1 as i32) != (100 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f((1 as i32) as u32, 0 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f((30 as i32) as u32, 0 as i32) != (30 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
