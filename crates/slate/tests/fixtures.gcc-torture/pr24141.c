// reduced testcase, compile with -O2. Also, with --disable-checking
// gcc produces wrong code.

void abort(void);
int  i;

void g(void) { i = 1; }

void f(int a, int b) {
  int c = 0;
  if (a == 0)
    c = 1;
  if (c)
    return;
  if (c == 1)
    c = 0;
  if (b == 0)
    c = 1;
  if (c)
    g();
}

int main(void) {
  f(1, 0);
  if (i != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! reduced testcase, compile with -O2. Also, with --disable-checking
// CHECK-NEXT: //!
// CHECK-NEXT: //! gcc produces wrong code.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn g() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut a: i32, mut b: i32) {
// CHECK-NEXT:     let mut c: i32 = 0 as i32;
// CHECK-NEXT:     if a == (0 as i32) {
// CHECK-NEXT:         c = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if c != (0 as i32) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     if c == (1 as i32) {
// CHECK-NEXT:         c = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if b == (0 as i32) {
// CHECK-NEXT:         c = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if c != (0 as i32) {
// CHECK-NEXT:         g();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f(1 as i32, 0 as i32);
// CHECK-NEXT:     if (unsafe { i }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
