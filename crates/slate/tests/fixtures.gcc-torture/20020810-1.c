/* PR target/7559
   This testcase was miscompiled on x86-64, because classify_argument
   wrongly computed the offset of nested structure fields.  */

extern void abort(void);

struct A {
  long x;
};

struct R {
  struct A a, b;
};

struct R R = {100, 200};

void f(struct R r) {
  if (r.a.x != R.a.x || r.b.x != R.b.x)
    abort();
}

struct R g(void) { return R; }

int main(void) {
  struct R r;
  f(R);
  r = g();
  if (r.a.x != R.a.x || r.b.x != R.b.x)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/7559
// CHECK-NEXT: //! This testcase was miscompiled on x86-64, because classify_argument
// CHECK-NEXT: //! wrongly computed the offset of nested structure fields.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     x: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct R {
// CHECK-NEXT:     a: A,
// CHECK-NEXT:     b: A,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut R: R = R {
// CHECK-NEXT:     a: A {
// CHECK-NEXT:         x: (100 as i32) as i64,
// CHECK-NEXT:     },
// CHECK-NEXT:     b: A {
// CHECK-NEXT:         x: (200 as i32) as i64,
// CHECK-NEXT:     },
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut r: R) {
// CHECK-NEXT:     if r.a.x != unsafe { R.a.x } || r.b.x != unsafe { R.b.x } {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn g() -> R {
// CHECK-NEXT:     return unsafe { R };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r: R = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     f(unsafe { R });
// CHECK-NEXT:     r = g();
// CHECK-NEXT:     if r.a.x != unsafe { R.a.x } || r.b.x != unsafe { R.b.x } {
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
