extern void abort(void);

int test1(int x) { return x / 10 == 2; }

int test2(int x) { return x / 10 == 0; }

int test3(int x) { return x / 10 == -2; }

int test4(int x) { return x / -10 == 2; }

int test5(int x) { return x / -10 == 0; }

int test6(int x) { return x / -10 == -2; }

int main() {
  if (test1(19) != 0)
    abort();
  if (test1(20) != 1)
    abort();
  if (test1(29) != 1)
    abort();
  if (test1(30) != 0)
    abort();

  if (test2(-10) != 0)
    abort();
  if (test2(-9) != 1)
    abort();
  if (test2(9) != 1)
    abort();
  if (test2(10) != 0)
    abort();

  if (test3(-30) != 0)
    abort();
  if (test3(-29) != 1)
    abort();
  if (test3(-20) != 1)
    abort();
  if (test3(-19) != 0)
    abort();

  if (test4(-30) != 0)
    abort();
  if (test4(-29) != 1)
    abort();
  if (test4(-20) != 1)
    abort();
  if (test4(-19) != 0)
    abort();

  if (test5(-10) != 0)
    abort();
  if (test5(-9) != 1)
    abort();
  if (test5(9) != 1)
    abort();
  if (test5(10) != 0)
    abort();

  if (test6(19) != 0)
    abort();
  if (test6(20) != 1)
    abort();
  if (test6(29) != 1)
    abort();
  if (test6(30) != 0)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn test1(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / (10 as i32) == (2 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test2(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / (10 as i32) == (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test3(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / (10 as i32) == -(2 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test4(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) == (2 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test5(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) == (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test6(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) == -(2 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if test1(19 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test1(20 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test1(29 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test1(30 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(-(10 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(-(9 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(9 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(10 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(-(30 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(-(29 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(-(20 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(-(19 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(-(30 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(-(29 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(-(20 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(-(19 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test5(-(10 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test5(-(9 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test5(9 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test5(10 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6(19 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6(20 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6(29 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6(30 as i32) != (0 as i32) {
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
