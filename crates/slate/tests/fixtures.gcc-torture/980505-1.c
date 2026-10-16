void abort(void);
void exit(int);

static int f(int) __attribute__((const));
int        main() {
  int f1, f2, x;
  x  = 1;
  f1 = f(x);
  x  = 2;
  f2 = f(x);
  if (f1 != 1 || f2 != 2)
    abort();
  exit(0);
}
static int f(int x) { return x; }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f1: i32 = 0 as i32;
// CHECK-NEXT:     let mut f2: i32 = 0 as i32;
// CHECK-NEXT:     let mut x: i32 = 1 as i32;
// CHECK-NEXT:     f1 = f(x);
// CHECK-NEXT:     x = 2 as i32;
// CHECK-NEXT:     f2 = f(x);
// CHECK-NEXT:     if f1 != (1 as i32) || f2 != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut x: i32) -> i32 {
// CHECK-NEXT:     x
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
