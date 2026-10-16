void abort(void);

int bar(void) { return -1; }

unsigned long foo() {
  unsigned long retval;
  retval = bar();
  if (retval == -1)
    return 0;
  return 3;
}

int main(void) {
  if (foo() != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn bar() -> i32 {
// CHECK-NEXT:     -(1 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn foo() -> u64 {
// CHECK-NEXT:     let mut retval: u64 = (bar() as i64) as u64;
// CHECK-NEXT:     if retval == ((-(1 as i32) as i64) as u64) {
// CHECK-NEXT:         return ((0 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     ((3 as i32) as i64) as u64
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo() != (((0 as i32) as i64) as u64) {
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
