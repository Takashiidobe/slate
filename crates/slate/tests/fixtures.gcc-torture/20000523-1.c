void abort(void);
void exit(int);

int main(void) {
  long long x;
  int       n;

  if (sizeof(long long) < 8)
    exit(0);

  n = 9;
  x = (((long long)n) << 55) / 0xff;

  if (x == 0)
    abort();

  x = (((long long)9) << 55) / 0xff;

  if (x == 0)
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
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i64 = 0 as i64;
// CHECK-NEXT:     let mut n: i32 = 9 as i32;
// CHECK-NEXT:     if (8 as u64) < (((8 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x = ((n as i64) << (55 as i32)) / ((255 as i32) as i64);
// CHECK-NEXT:     if x == ((0 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     x = (((9 as i32) as i64) << (55 as i32)) / ((255 as i32) as i64);
// CHECK-NEXT:     if x == ((0 as i32) as i64) {
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
