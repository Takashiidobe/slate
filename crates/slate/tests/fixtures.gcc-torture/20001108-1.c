void abort(void);
void exit(int);

long long signed_poly(long long sum, long x) {
  sum += (long long)(long)sum * (long long)x;
  return sum;
}

unsigned long long unsigned_poly(unsigned long long sum, unsigned long x) {
  sum += (unsigned long long)(unsigned long)sum * (unsigned long long)x;
  return sum;
}

int main(void) {
  if (signed_poly(2LL, -3) != -4LL)
    abort();

  if (unsigned_poly(2ULL, 3) != 8ULL)
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
// CHECK-NEXT: fn signed_poly(mut sum: i64, mut x: i64) -> i64 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64 = sum;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i64 = [[V_1]] + sum * x;
// CHECK-NEXT:     sum = [[V_2]];
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: fn unsigned_poly(mut sum: u64, mut x: u64) -> u64 {
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = sum;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u64 = [[V_3]].wrapping_add(sum.wrapping_mul(x));
// CHECK-NEXT:     sum = [[V_4]];
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if signed_poly(2 as i64, -(3 as i32) as i64) != -(4 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if unsigned_poly(2 as u64, ((3 as i32) as i64) as u64) != (8 as u64) {
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
