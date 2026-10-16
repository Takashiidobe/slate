struct c {
  double a;
} __attribute((packed)) __attribute((aligned));

extern void abort(void);

double g_expect = 32.25;

void f(unsigned x, struct c y) {
  if (x != 0)
    abort();

  if (y.a != g_expect)
    abort();
}

struct c e = {64.25};

int main(void) {
  struct c d = {32.25};
  f(0, d);

  g_expect = 64.25;
  f(0, e);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct c {
// CHECK-NEXT:     a: f64,
// CHECK-NEXT: }
// CHECK-NEXT: static mut g_expect: f64 = 32.25f64;
// CHECK-NEXT: fn f(mut x: u32, mut y: c) {
// CHECK-NEXT:     if x != ((0 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if y.a != unsafe { g_expect } {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut e: c = c { a: 64.25f64 };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: c = c { a: 32.25f64 };
// CHECK-NEXT:     f((0 as i32) as u32, d);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g_expect = 64.25f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     f((0 as i32) as u32, unsafe { e });
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
