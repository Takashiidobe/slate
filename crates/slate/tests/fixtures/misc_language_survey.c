#include <stdio.h>

void scale(int n, int arr[*]);

void scale(int n, int arr[n]) {
  for (int i = 0; i < n; ++i) {
    arr[i] *= 2;
  }
}

int add(int, int);
int add(int a, int b) { return a + b; }

int main(void) {
  int values[3] = {1, 2, 3};
  scale(3, values);

  double separated      = 1'000.5;
  char   escape_e       = '\e';
  char   escape_unknown = '\%';

  printf("%d %d %d\n", values[0], values[1], values[2]);
  printf("%f\n", separated);
  printf("%d %d\n", (int)escape_e, (int)escape_unknown);
  printf("%zu\n", sizeof(add));
  printf("%d\n", add(3, 4));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn scale(n: i32, arr: *mut i32) {
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *mut i32 = unsafe { arr.offset(i as isize) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_0]] = *[[V_0]] * 2;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn add(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: [i32; 3] = [1, 2, 3];
// CHECK-NEXT:     scale(3i32, values.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     let separated: f64 = 1000.5f64;
// CHECK-NEXT:     let escape_e: i8 = 27;
// CHECK-NEXT:     let escape_unknown: i8 = 37;
// CHECK-NEXT:     println!("{} {} {}", values[0], values[1], values[2]);
// CHECK-NEXT:     unsafe { printf(c"%f\n".as_ptr(), separated) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{} {}", escape_e as i32, escape_unknown as i32);
// CHECK-NEXT:     unsafe { printf(c"%zu\n".as_ptr(), 1u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", add(3i32, 4i32));
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
