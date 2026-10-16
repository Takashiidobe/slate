void abort(void);
void exit(int);

int f(long long x) {
  x >>= 8;
  return x & 0xff;
}

int main(void) {
  if (f(0x0123456789ABCDEFLL) != 0xCD)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut x: i64) -> i32 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64 = x;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i64 = [[V_1]] >> (8 as i32);
// CHECK-NEXT:     x = [[V_2]];
// CHECK-NEXT:     (x & ((255 as i32) as i64)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f(81985529216486895 as i64) != (205 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
