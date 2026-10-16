extern void abort(void);

unsigned int a, b = 1, c;

void __attribute__((noinline)) foo(int x) {
  if (x != 5)
    abort();
}

int main() {
  unsigned int d, e;
  for (d = 1; d < 5; d++)
    if (c)
      a = b;
  a = b;
  e = a << 1;
  if (e)
    e = (e << 1) ^ 1;
  foo(e);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: u32 = (1 as i32) as u32;
// CHECK-NEXT: static mut c: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo(mut x: i32) {
// CHECK-NEXT:     if x != (5 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: u32 = (1 as i32) as u32;
// CHECK-NEXT:     let mut e: u32 = 0 as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while d < ((5 as i32) as u32) {
// CHECK-NEXT:         if (unsafe { c }) != (0 as u32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 a = unsafe { b };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = d;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         d = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = unsafe { b };
// CHECK-NEXT:     }
// CHECK-NEXT:     e = (unsafe { a }) << (1 as i32);
// CHECK-NEXT:     if e != (0 as u32) {
// CHECK-NEXT:         e = e << (1 as i32) ^ ((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(e as i32);
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
