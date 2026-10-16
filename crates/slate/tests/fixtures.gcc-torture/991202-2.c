void abort(void);
void exit(int);

int f1() {
  unsigned long x, y = 1;

  x = ((y * 8192) - 216) % 16;
  return x;
}

int main() {
  if (f1() != 8)
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
// CHECK-NEXT: fn f1() -> i32 {
// CHECK-NEXT:     let mut x: u64 = 0 as u64;
// CHECK-NEXT:     let mut y: u64 = ((1 as i32) as i64) as u64;
// CHECK-NEXT:     x = y
// CHECK-NEXT:         .wrapping_mul(((8192 as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_sub(((216 as i32) as i64) as u64) % (((16 as i32) as i64) as u64);
// CHECK-NEXT:     (x as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f1() != (8 as i32) {
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
