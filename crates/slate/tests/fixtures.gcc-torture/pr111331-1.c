int a;
int b;
int c(int d, int e, int f) {
  if (d < e)
    return e;
  if (d > f)
    return f;
  return d;
}
int main() {
  int g          = -1;
  a              = c(b + 30, 29, g + 29);
  volatile int t = a;
  if (t != 28)
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
// CHECK-NEXT: fn c(mut d: i32, mut e: i32, mut f: i32) -> i32 {
// CHECK-NEXT:     if d < e {
// CHECK-NEXT:         return e;
// CHECK-NEXT:     }
// CHECK-NEXT:     if d > f {
// CHECK-NEXT:         return f;
// CHECK-NEXT:     }
// CHECK-NEXT:     d
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut g: i32 = -(1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = c((unsafe { b }) + (30 as i32), 29 as i32, g + (29 as i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut t: i32 = unsafe { a };
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(t)) }) != (28 as i32) {
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
