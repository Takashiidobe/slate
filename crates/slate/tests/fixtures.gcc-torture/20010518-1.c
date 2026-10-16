/* Leaf functions with many arguments.  */

void abort(void);
void exit(int);

int add(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j,
        int k, int l, int m) {
  return a + b + c + d + e + f + g + h + i + j + k + l + m;
}

int main(void) {
  if (add(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13) != 91)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Leaf functions with many arguments.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn add(
// CHECK-NEXT:     mut a: i32,
// CHECK-NEXT:     mut b: i32,
// CHECK-NEXT:     mut c: i32,
// CHECK-NEXT:     mut d: i32,
// CHECK-NEXT:     mut e: i32,
// CHECK-NEXT:     mut f: i32,
// CHECK-NEXT:     mut g: i32,
// CHECK-NEXT:     mut h: i32,
// CHECK-NEXT:     mut i: i32,
// CHECK-NEXT:     mut j: i32,
// CHECK-NEXT:     mut k: i32,
// CHECK-NEXT:     mut l: i32,
// CHECK-NEXT:     mut m: i32,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     a + b + c + d + e + f + g + h + i + j + k + l + m
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if add(
// CHECK-NEXT:         1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32, 8 as i32, 9 as i32,
// CHECK-NEXT:         10 as i32, 11 as i32, 12 as i32, 13 as i32,
// CHECK-NEXT:     ) != (91 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
