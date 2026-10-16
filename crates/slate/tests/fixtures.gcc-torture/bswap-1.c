/* Test __builtin_bswap64 . */

unsigned long long g(unsigned long long a) __attribute__((noinline));
unsigned long long g(unsigned long long a) { return __builtin_bswap64(a); }

unsigned long long f(unsigned long long c) {
  union {
    unsigned long long a;
    unsigned char      b[8];
  } a, b;
  a.a    = c;
  b.b[0] = a.b[7];
  b.b[1] = a.b[6];
  b.b[2] = a.b[5];
  b.b[3] = a.b[4];
  b.b[4] = a.b[3];
  b.b[5] = a.b[2];
  b.b[6] = a.b[1];
  b.b[7] = a.b[0];
  return b.a;
}

int main(void) {
  unsigned long long i;
  /* The rest of the testcase assumes 8 byte long long. */
  if (sizeof(i) != sizeof(char) * 8)
    return 0;
  if (f(0x12) != g(0x12))
    __builtin_abort();
  if (f(0x1234) != g(0x1234))
    __builtin_abort();
  if (f(0x123456) != g(0x123456))
    __builtin_abort();
  if (f(0x12345678ull) != g(0x12345678ull))
    __builtin_abort();
  if (f(0x1234567890ull) != g(0x1234567890ull))
    __builtin_abort();
  if (f(0x123456789012ull) != g(0x123456789012ull))
    __builtin_abort();
  if (f(0x12345678901234ull) != g(0x12345678901234ull))
    __builtin_abort();
  if (f(0x1234567890123456ull) != g(0x1234567890123456ull))
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test __builtin_bswap64 .
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.bswap.i64"]
// CHECK-NEXT:     fn __slate_intrinsic_5c27152fac4a123f([[ARG_0:arg[0-9]+]]: u64) -> u64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: u64,
// CHECK-NEXT:     b: [u8; 8],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn g(mut a: u64) -> u64 {
// CHECK-NEXT:     unsafe { __slate_intrinsic_5c27152fac4a123f(a) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut c: u64) -> u64 {
// CHECK-NEXT:     let mut a: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.a = c;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { b.b.as_mut_ptr() as *mut u8 }.offset((0 as i32) as isize) } = unsafe {
// CHECK-NEXT:             *unsafe { unsafe { a.b.as_mut_ptr() as *mut u8 }.offset((7 as i32) as isize) }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { b.b.as_mut_ptr() as *mut u8 }.offset((1 as i32) as isize) } = unsafe {
// CHECK-NEXT:             *unsafe { unsafe { a.b.as_mut_ptr() as *mut u8 }.offset((6 as i32) as isize) }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { b.b.as_mut_ptr() as *mut u8 }.offset((2 as i32) as isize) } = unsafe {
// CHECK-NEXT:             *unsafe { unsafe { a.b.as_mut_ptr() as *mut u8 }.offset((5 as i32) as isize) }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { b.b.as_mut_ptr() as *mut u8 }.offset((3 as i32) as isize) } = unsafe {
// CHECK-NEXT:             *unsafe { unsafe { a.b.as_mut_ptr() as *mut u8 }.offset((4 as i32) as isize) }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { b.b.as_mut_ptr() as *mut u8 }.offset((4 as i32) as isize) } = unsafe {
// CHECK-NEXT:             *unsafe { unsafe { a.b.as_mut_ptr() as *mut u8 }.offset((3 as i32) as isize) }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { b.b.as_mut_ptr() as *mut u8 }.offset((5 as i32) as isize) } = unsafe {
// CHECK-NEXT:             *unsafe { unsafe { a.b.as_mut_ptr() as *mut u8 }.offset((2 as i32) as isize) }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { b.b.as_mut_ptr() as *mut u8 }.offset((6 as i32) as isize) } = unsafe {
// CHECK-NEXT:             *unsafe { unsafe { a.b.as_mut_ptr() as *mut u8 }.offset((1 as i32) as isize) }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { b.b.as_mut_ptr() as *mut u8 }.offset((7 as i32) as isize) } = unsafe {
// CHECK-NEXT:             *unsafe { unsafe { a.b.as_mut_ptr() as *mut u8 }.offset((0 as i32) as isize) }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { b.a }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // The rest of the testcase assumes 8 byte long long.
// CHECK-NEXT:     if (8 as u64) != (1 as u64).wrapping_mul(((8 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if f(((18 as i32) as i64) as u64) != g(((18 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f(((4660 as i32) as i64) as u64) != g(((4660 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f(((1193046 as i32) as i64) as u64) != g(((1193046 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f(305419896 as u64) != g(305419896 as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f(78187493520 as u64) != g(78187493520 as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f(20015998341138 as u64) != g(20015998341138 as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f(5124095575331380 as u64) != g(5124095575331380 as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f(1311768467284833366 as u64) != g(1311768467284833366 as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
