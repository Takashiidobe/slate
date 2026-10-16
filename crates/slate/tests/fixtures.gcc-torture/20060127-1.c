void abort();

void f(long long a) {
  if ((a & 0xffffffffLL) != 0)
    abort();
}

long long a = 0x1234567800000000LL;

int main() {
  f(a);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut [[A_0:a_[0-9]+]]: i64) {
// CHECK-NEXT:     if [[A_0]] & (4294967295 as i64) != ((0 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i64 = 1311768464867721216 as i64;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f(unsafe { a });
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
