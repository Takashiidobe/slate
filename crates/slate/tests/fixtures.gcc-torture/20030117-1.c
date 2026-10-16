void abort(void);
void exit(int);

int foo(int, int, int);
int bar(int, int, int);

int main(void) {
  if (foo(5, 10, 21) != 12)
    abort();

  if (bar(9, 12, 15) != 150)
    abort();

  exit(0);
}

int foo(int x, int y, int z) { return (x + y + z) / 3; }

int bar(int x, int y, int z) { return foo(x * x, y * y, z * z); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(5 as i32, 10 as i32, 21 as i32) != (12 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(9 as i32, 12 as i32, 15 as i32) != (150 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32, mut z: i32) -> i32 {
// CHECK-NEXT:     (x + y + z) / (3 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut x: i32, mut y: i32, mut z: i32) -> i32 {
// CHECK-NEXT:     foo(x * x, y * y, z * z)
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
