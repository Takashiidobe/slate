/* PR rtl-optimization/83565 */
/* Testcase by Sergei Trofimovich <slyfox@inbox.ru> */

extern void abort(void);

typedef __UINT32_TYPE__ u32;

u32 bug(u32 *result) __attribute__((noinline));
u32 bug(u32 *result) {
  volatile u32 ss = 0xFFFFffff;
  volatile u32 d  = 0xEEEEeeee;
  u32          tt = d & 0x00800000;
  u32          r  = tt << 8;

  r = (r >> 31) | (r << 1);

  u32 u   = r ^ ss;
  u32 off = u >> 1;

  *result = tt;
  return off;
}

int main(void) {
  u32 l;
  u32 off = bug(&l);
  if (off != 0x7fffffff)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/83565
// CHECK-NEXT: //!
// CHECK-NEXT: //! Testcase by Sergei Trofimovich <slyfox@inbox.ru>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bug(mut result: &mut u32) -> u32 {
// CHECK-NEXT:     let mut ss: u32 = 4294967295 as u32;
// CHECK-NEXT:     let mut d: u32 = 4008636142 as u32;
// CHECK-NEXT:     let mut tt: u32 =
// CHECK-NEXT:         (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) }) & ((8388608 as i32) as u32);
// CHECK-NEXT:     let mut r: u32 = tt << (8 as i32);
// CHECK-NEXT:     r = r >> (31 as i32) | r << (1 as i32);
// CHECK-NEXT:     let mut u: u32 = r ^ unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ss)) };
// CHECK-NEXT:     let mut off: u32 = u >> (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(result as *mut u32) = tt;
// CHECK-NEXT:     }
// CHECK-NEXT:     off
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut l: u32 = 0 as u32;
// CHECK-NEXT:     let mut off: u32 = bug(unsafe { &mut (*std::ptr::addr_of_mut!(l)) });
// CHECK-NEXT:     if off != ((2147483647 as i32) as u32) {
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
