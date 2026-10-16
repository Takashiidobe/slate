void abort(void);
void exit(int);

long a = 1;

int foo(void) {
  switch (a % 2 % 2 % 2 % 2 % 2 % 2 % 2 % 2) {
  case 0:
    return 0;
  case 1:
    return 1;
  default:
    return -1;
  }
}

int main(void) {
  if (foo() != 1)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i64 = (1 as i32) as i64;
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     match (unsafe { a }) % ((2 as i32) as i64) % ((2 as i32) as i64)
// CHECK-NEXT:         % ((2 as i32) as i64) % ((2 as i32) as i64) % ((2 as i32) as i64)
// CHECK-NEXT:         % ((2 as i32) as i64) % ((2 as i32) as i64) % ((2 as i32) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             return 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return -(1 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo() != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
