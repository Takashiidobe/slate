int test1(int b, int c) {
  char x;
  if (b)
    return x / c;
  else
    return 1;
}
int test2(int b, int c) {
  int x;
  if (b)
    return x * c;
  else
    return 1;
}
int test3(int b, int c) {
  int x;
  if (b)
    return x % c;
  else
    return 1;
}
int test4(int b, int c) {
  char x;
  if (b)
    return x == c;
  else
    return 1;
}

extern void abort(void);
int         main() {
  if (test1(1, 1000) != 0)
    abort();
  if (test2(1, 0) != 0)
    abort();
  if (test3(1, 1) != 0)
    abort();
  if (test4(1, 1000) != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn test1(mut b: i32, mut c: i32) -> i32 {
// CHECK-NEXT:     let mut x: i8 = 0 as i8;
// CHECK-NEXT:     if b != (0 as i32) {
// CHECK-NEXT:         return (x as i32) / c;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn test2(mut b: i32, mut c: i32) -> i32 {
// CHECK-NEXT:     let mut x: i32 = 0 as i32;
// CHECK-NEXT:     if b != (0 as i32) {
// CHECK-NEXT:         return x * c;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn test3(mut b: i32, mut c: i32) -> i32 {
// CHECK-NEXT:     let mut x: i32 = 0 as i32;
// CHECK-NEXT:     if b != (0 as i32) {
// CHECK-NEXT:         return x % c;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn test4(mut b: i32, mut c: i32) -> i32 {
// CHECK-NEXT:     let mut x: i8 = 0 as i8;
// CHECK-NEXT:     if b != (0 as i32) {
// CHECK-NEXT:         return ((x as i32) == c) as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if test1(1 as i32, 1000 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(1 as i32, 0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(1 as i32, 1 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(1 as i32, 1000 as i32) != (0 as i32) {
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
