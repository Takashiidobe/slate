void abort(void);
void exit(int);

int g1, g2;

void foo(int x) {
  int y;

  if (x)
    y = 793;
  else
    y = 793;
  g1 = 7930 / y;
  g2 = 7930 / x;
}

int main() {
  foo(793);
  if (g1 != 10 || g2 != 10)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut g1: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut g2: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo(mut x: i32) {
// CHECK-NEXT:     let mut y: i32 = 0 as i32;
// CHECK-NEXT:     if x != (0 as i32) {
// CHECK-NEXT:         y = 793 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         y = 793 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g1 = (7930 as i32) / y;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g2 = (7930 as i32) / x;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(793 as i32);
// CHECK-NEXT:     if (unsafe { g1 }) != (10 as i32) || (unsafe { g2 }) != (10 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
