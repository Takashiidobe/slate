
int a;
int b;

int main() {
  int d = b + 30;
  {
    int t;
    if (d < 29)
      t = 29;
    else
      t = (d > 28) ? 28 : d;
    a = t;
  }
  volatile int t = a;
  if (a != 28)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: i32 = (unsafe { b }) + (30 as i32);
// CHECK-NEXT:     let mut t: i32 = 0 as i32;
// CHECK-NEXT:     if d < (29 as i32) {
// CHECK-NEXT:         t = 29 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         t = if d > (28 as i32) { 28 as i32 } else { d };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = t;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut t: i32 = unsafe { a };
// CHECK-NEXT:     if (unsafe { a }) != (28 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
