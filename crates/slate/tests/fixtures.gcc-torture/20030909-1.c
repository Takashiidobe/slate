void abort();
void exit(int);

void test(int x, int y) {
  if (x == y)
    abort();
}

void foo(int x, int y) {
  if (x == y)
    goto a;
  else {
  a:;
    if (x == y)
      goto b;
    else {
    b:;
      if (x != y)
        test(x, y);
    }
  }
}

int main(void) {
  foo(0, 0);

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn test(mut x: i32, mut y: i32) {
// CHECK-NEXT:     if x == y {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: { if x == y {} }
// CHECK-NEXT:         {}
// CHECK-NEXT:         if x == y {}
// CHECK-NEXT:         {}
// CHECK-NEXT:         if x != y {
// CHECK-NEXT:             test(x, y);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(0 as i32, 0 as i32);
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
