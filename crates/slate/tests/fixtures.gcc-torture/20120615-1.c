extern void abort(void);

void __attribute__((noinline, noclone)) test1(int i) {
  if (i == 12)
    return;
  if (i != 17) {
    if (i == 15)
      return;
    abort();
  }
}

int main() {
  test1(15);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn test1(mut i: i32) {
// CHECK-NEXT:     if i == (12 as i32) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     if i != (17 as i32) {
// CHECK-NEXT:         if i == (15 as i32) {
// CHECK-NEXT:             return;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test1(15 as i32);
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
