/* PR target/9348 */

void abort(void);

#define u_l_l unsigned long long
#define l_l   long long

l_l mpy_res;

u_l_l mpy(long a, long b) { return (u_l_l)a * (u_l_l)b; }

int main(void) {
  mpy_res = mpy(1, -1);
  if (mpy_res != -1LL)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/9348
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut mpy_res: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn mpy(mut a: i64, mut b: i64) -> u64 {
// CHECK-NEXT:     (a as u64).wrapping_mul(b as u64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         mpy_res = mpy((1 as i32) as i64, -(1 as i32) as i64) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { mpy_res }) != -(1 as i64) {
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
