int f(int k, int i1, int j1) {
  int *f1;
  if (k)
    f1 = &i1;
  else
    f1 = &j1;
  i1 = 0;
  return *f1;
}

extern void abort(void);

int main() {
  if (f(1, 1, 2) != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut k: i32, mut i1: i32, mut j1: i32) -> i32 {
// CHECK-NEXT:     let mut f1: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if k != (0 as i32) {
// CHECK-NEXT:         f1 = std::ptr::addr_of_mut!(i1);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         f1 = std::ptr::addr_of_mut!(j1);
// CHECK-NEXT:     }
// CHECK-NEXT:     i1 = 0 as i32;
// CHECK-NEXT:     unsafe { *f1 }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f(1 as i32, 1 as i32, 2 as i32) != (0 as i32) {
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
