/* PR rtl-optimization/81423 */

extern void abort(void);

unsigned long long int ll   = 0;
unsigned long long int ull1 = 1ULL;
unsigned long long int ull2 = 12008284144813806346ULL;
unsigned long long int ull3;

unsigned long long int __attribute__((noinline)) foo(void) {
  ll = -5597998501375493990LL;

  ll = (unsigned int)(5677365550390624949LL - ll) - (ull1 > 0);
  unsigned long long int ull3;
  ull3 = (unsigned int)(2067854353LL
                        << (((ll + -2129105131LL) ^ 10280750144413668236ULL) -
                            10280750143997242009ULL)) >>
         ((2873442921854271231ULL | ull2) - 12098357307243495419ULL);

  return ull3;
}

int main(void) {
  /* We need a long long of exactly 64 bits and int of exactly 32 bits
     for this test.  */
  if (__SIZEOF_LONG_LONG__ * __CHAR_BIT__ != 64 ||
      __SIZEOF_INT__ * __CHAR_BIT__ != 32)
    return 0;

  ull3 = foo();
  if (ull3 != 3998784)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/81423
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut ll: u64 = ((0 as i32) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ull1: u64 = 1 as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ull2: u64 = 12008284144813806346 as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ull3: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> u64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ll = -(5597998501375493990 as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ll = (((5677365550390624949 as i64) as u64).wrapping_sub(unsafe { ll }) as u32)
// CHECK-NEXT:             .wrapping_sub((((unsafe { ull1 }) > (((0 as i32) as i64) as u64)) as i32) as u32)
// CHECK-NEXT:             as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[ULL3_0:ull[0-9]+_[0-9]+]]: u64 = (((((2067854353 as i64)
// CHECK-NEXT:         << (unsafe { ll }.wrapping_add(-(2129105131 as i64) as u64)
// CHECK-NEXT:             ^ (10280750144413668236 as u64))
// CHECK-NEXT:             .wrapping_sub(10280750143997242009 as u64)) as i32) as u32)
// CHECK-NEXT:         >> ((2873442921854271231 as u64) | unsafe { ull2 })
// CHECK-NEXT:             .wrapping_sub(12098357307243495419 as u64)) as u64;
// CHECK-NEXT:     [[ULL3_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // We need a long long of exactly 64 bits and int of exactly 32 bits
// CHECK-NEXT:     // for this test.
// CHECK-NEXT:     if (8 as i32) * (8 as i32) != (64 as i32) || (4 as i32) * (8 as i32) != (32 as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ull3 = foo();
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ull3 }) != (((3998784 as i32) as i64) as u64) {
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
