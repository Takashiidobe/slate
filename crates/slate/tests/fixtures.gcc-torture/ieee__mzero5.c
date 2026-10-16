/* { dg-do run } */
/* Test gcse handling of IEEE 0/-0 rules.  */
void          abort(void);
static double zero = 0.0;

int negzero_check(double d) {
  if (d == 0)
    return !!__builtin_memcmp((void *)&zero, (void *)&d, sizeof(double));
  return 0;
}

int sub(double d, double e) {
  if (d == 0.0 && e == 0.0 && negzero_check(d) == 0 && negzero_check(e) == 0)
    return 1;
  else
    return 0;
}

int main(void) {
  double minus_zero = -0.0;
  if (sub(minus_zero, 0))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-do run }
// CHECK-NEXT: //
// CHECK-NEXT: // Test gcse handling of IEEE 0/-0 rules.
// CHECK-NEXT: static mut zero: f64 = 0.0f64;
// CHECK-EMPTY:
// CHECK-NEXT: fn negzero_check(mut d: f64) -> i32 {
// CHECK-NEXT:     if d == ((0 as i32) as f64) {
// CHECK-NEXT:         return !(!((unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 ((unsafe { std::ptr::addr_of_mut!(zero) }) as *mut ()) as *const (),
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(d) as *mut ()) as *const (),
// CHECK-NEXT:                 8 as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32))) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sub(mut d: f64, mut e: f64) -> i32 {
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if d == 0.0f64 && e == 0.0f64 {
// CHECK-NEXT:         [[V_3]] = negzero_check(d) == (0 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_3]] {
// CHECK-NEXT:         [[V_4]] = negzero_check(e) == (0 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_4]] {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut minus_zero: f64 = -0.0f64;
// CHECK-NEXT:     if sub(minus_zero, (0 as i32) as f64) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
