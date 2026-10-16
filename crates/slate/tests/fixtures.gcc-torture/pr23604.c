extern void abort(void);

int g(int i, int j) {
  if (i > -1)
    if (i < 2) {
      if (i != j) {
        if (j != 0)
          return 0;
      }
    }
  return 1;
}

int main(void) {
  if (!g(1, 0))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn g(mut i: i32, mut j: i32) -> i32 {
// CHECK-NEXT:     if i > -(1 as i32) {
// CHECK-NEXT:         if i < (2 as i32) {
// CHECK-NEXT:             if i != j {
// CHECK-NEXT:                 if j != (0 as i32) {
// CHECK-NEXT:                     return 0 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if !(g(1 as i32, 0 as i32) != (0 as i32)) {
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
