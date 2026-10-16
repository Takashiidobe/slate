/* PR tree-optimization/56051 */

extern void abort(void);

int main() {
  unsigned char x1[1] = {0};
  unsigned int  s1    = __CHAR_BIT__;
  int           a1    = x1[0] < (unsigned char)(1 << s1);
  unsigned char y1    = (unsigned char)(1 << s1);
  int           b1    = x1[0] < y1;
  if (a1 != b1)
    abort();
#if __SIZEOF_LONG_LONG__ > __SIZEOF_INT__
  unsigned long long x2[1] = {2ULL << (sizeof(int) * __CHAR_BIT__)};
  unsigned int       s2    = sizeof(int) * __CHAR_BIT__ - 1;
  int                a2    = x2[0] >= (unsigned long long)(1 << s2);
  unsigned long long y2    = 1 << s2;
  int                b2    = x2[0] >= y2;
  if (a2 != b2)
    abort();
  unsigned long long x3[1] = {2ULL << (sizeof(int) * __CHAR_BIT__)};
  unsigned int       s3    = sizeof(int) * __CHAR_BIT__ - 1;
  int                a3    = x3[0] >= (unsigned long long)(1U << s3);
  unsigned long long y3    = 1U << s3;
  int                b3    = x3[0] >= y3;
  if (a3 != b3)
    abort();
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/56051
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x1: [u8; 1] = [((0 as i32) as i8) as u8];
// CHECK-NEXT:     let mut s1: u32 = (8 as i32) as u32;
// CHECK-NEXT:     let mut a1: i32 = ((((unsafe {
// CHECK-NEXT:         *unsafe { (x1.as_mut_ptr() as *mut u8).offset((0 as i32) as isize) }
// CHECK-NEXT:     }) as u32) as i32)
// CHECK-NEXT:         < ((((((1 as i32) << s1) as i8) as u8) as u32) as i32)) as i32;
// CHECK-NEXT:     let mut y1: u8 = (((1 as i32) << s1) as i8) as u8;
// CHECK-NEXT:     let mut b1: i32 = ((((unsafe {
// CHECK-NEXT:         *unsafe { (x1.as_mut_ptr() as *mut u8).offset((0 as i32) as isize) }
// CHECK-NEXT:     }) as u32) as i32)
// CHECK-NEXT:         < ((y1 as u32) as i32)) as i32;
// CHECK-NEXT:     if a1 != b1 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut x2: [u64; 1] = [(2 as u64) << (4 as u64).wrapping_mul(((8 as i32) as i64) as u64)];
// CHECK-NEXT:     let mut s2: u32 = (4 as u64)
// CHECK-NEXT:         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_sub(((1 as i32) as i64) as u64) as u32;
// CHECK-NEXT:     let mut a2: i32 =
// CHECK-NEXT:         ((unsafe { *unsafe { (x2.as_mut_ptr() as *mut u64).offset((0 as i32) as isize) } })
// CHECK-NEXT:             >= ((((1 as i32) << s2) as i64) as u64)) as i32;
// CHECK-NEXT:     let mut y2: u64 = (((1 as i32) << s2) as i64) as u64;
// CHECK-NEXT:     let mut b2: i32 =
// CHECK-NEXT:         ((unsafe { *unsafe { (x2.as_mut_ptr() as *mut u64).offset((0 as i32) as isize) } }) >= y2)
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     if a2 != b2 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut x3: [u64; 1] = [(2 as u64) << (4 as u64).wrapping_mul(((8 as i32) as i64) as u64)];
// CHECK-NEXT:     let mut s3: u32 = (4 as u64)
// CHECK-NEXT:         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_sub(((1 as i32) as i64) as u64) as u32;
// CHECK-NEXT:     let mut a3: i32 =
// CHECK-NEXT:         ((unsafe { *unsafe { (x3.as_mut_ptr() as *mut u64).offset((0 as i32) as isize) } })
// CHECK-NEXT:             >= (((1 as u32) << s3) as u64)) as i32;
// CHECK-NEXT:     let mut y3: u64 = ((1 as u32) << s3) as u64;
// CHECK-NEXT:     let mut b3: i32 =
// CHECK-NEXT:         ((unsafe { *unsafe { (x3.as_mut_ptr() as *mut u64).offset((0 as i32) as isize) } }) >= y3)
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     if a3 != b3 {
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
