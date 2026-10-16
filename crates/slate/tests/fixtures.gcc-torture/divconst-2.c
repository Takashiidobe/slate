void abort(void);
void exit(int);

long f(long x) { return x / (-0x7fffffffL - 1L); }

long r(long x) { return x % (-0x7fffffffL - 1L); }

/* Since we have a negative divisor, this equation must hold for the
   results of / and %; no specific results are guaranteed.  */
long std_eqn(long num, long denom, long quot, long rem) {
  /* For completeness, a check for "ABS (rem) < ABS (denom)" belongs here,
     but causes trouble on 32-bit machines and isn't worthwhile.  */
  return quot * (-0x7fffffffL - 1L) + rem == num;
}

long nums[] = {-1L, 0x7fffffffL, -0x7fffffffL - 1L};

int main(void) {
  int i;

  for (i = 0; i < sizeof(nums) / sizeof(nums[0]); i++)
    if (std_eqn(nums[i], -0x7fffffffL - 1L, f(nums[i]), r(nums[i])) == 0)
      abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut x: i64) -> i64 {
// CHECK-NEXT:     x / (-(2147483647 as i64) - (1 as i64))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn r(mut x: i64) -> i64 {
// CHECK-NEXT:     x % (-(2147483647 as i64) - (1 as i64))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// Since we have a negative divisor, this equation must hold for the
// CHECK-NEXT: /// results of / and %; no specific results are guaranteed.
// CHECK-NEXT: fn std_eqn(mut num: i64, mut denom: i64, mut quot: i64, mut rem: i64) -> i64 {
// CHECK-NEXT:     // For completeness, a check for "ABS (rem) < ABS (denom)" belongs here,
// CHECK-NEXT:     // but causes trouble on 32-bit machines and isn't worthwhile.
// CHECK-NEXT:     (quot * (-(2147483647 as i64) - (1 as i64)) + rem == num) as i64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut nums: __SlateAlign16<[i64; 3]> = __SlateAlign16([
// CHECK-NEXT:     -(1 as i64),
// CHECK-NEXT:     2147483647 as i64,
// CHECK-NEXT:     -(2147483647 as i64) - (1 as i64),
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (24 as u64) / (8 as u64) {
// CHECK-NEXT:         if std_eqn(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { std::ptr::addr_of_mut!(nums.0) as *mut i64 }.offset(i as isize) }
// CHECK-NEXT:             },
// CHECK-NEXT:             -(2147483647 as i64) - (1 as i64),
// CHECK-NEXT:             f(unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { std::ptr::addr_of_mut!(nums.0) as *mut i64 }.offset(i as isize) }
// CHECK-NEXT:             }),
// CHECK-NEXT:             r(unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { std::ptr::addr_of_mut!(nums.0) as *mut i64 }.offset(i as isize) }
// CHECK-NEXT:             }),
// CHECK-NEXT:         ) == ((0 as i32) as i64)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
