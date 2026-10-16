extern void abort(void);

long long __attribute__((noinline, noclone)) foo(long long t) {
  while (t > -4)
    t -= 2;

  return t;
}

int main(void) {
  if (foo(0) != -4)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut t: i64) -> i64 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while t > (-(4 as i32) as i64) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i64 = t;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i64 = [[V_0]] - ((2 as i32) as i64);
// CHECK-NEXT:         t = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo((0 as i32) as i64) != (-(4 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
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
