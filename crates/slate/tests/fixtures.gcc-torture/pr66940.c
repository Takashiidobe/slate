long long __attribute__((noinline, noclone)) foo(long long ival) {
  if (ival <= 0)
    return -0x7fffffffffffffffL - 1;

  return 0x7fffffffffffffffL;
}

int main(void) {
  if (foo(-1) != (-0x7fffffffffffffffL - 1))
    __builtin_abort();

  if (foo(1) != 0x7fffffffffffffffL)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut ival: i64) -> i64 {
// CHECK-NEXT:     if ival <= ((0 as i32) as i64) {
// CHECK-NEXT:         return -(9223372036854775807 as i64) - ((1 as i32) as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     9223372036854775807 as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(-(1 as i32) as i64) != -(9223372036854775807 as i64) - ((1 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo((1 as i32) as i64) != (9223372036854775807 as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
