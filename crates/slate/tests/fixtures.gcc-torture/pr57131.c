/* PR rtl-optimization/57131 */

extern void abort(void);

int main() {
  volatile int       x1 = 0;
  volatile long long x2 = 0;
  volatile int       x3 = 0;
  volatile int       x4 = 1;
  volatile int       x5 = 1;
  volatile long long x6 = 1;
  long long          t  = ((x1 * (x2 << x3)) / (x4 * x5)) + x6;

  if (t != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/57131
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x1: i32 = 0 as i32;
// CHECK-NEXT:     let mut x2: i64 = (0 as i32) as i64;
// CHECK-NEXT:     let mut x3: i32 = 0 as i32;
// CHECK-NEXT:     let mut x4: i32 = 1 as i32;
// CHECK-NEXT:     let mut x5: i32 = 1 as i32;
// CHECK-NEXT:     let mut x6: i64 = (1 as i32) as i64;
// CHECK-NEXT:     let mut t: i64 = ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x1)) }) as i64)
// CHECK-NEXT:         * ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x2)) })
// CHECK-NEXT:             << unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x3)) })
// CHECK-NEXT:         / (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x4)) })
// CHECK-NEXT:             * unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x5)) }) as i64)
// CHECK-NEXT:         + unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x6)) };
// CHECK-NEXT:     if t != ((1 as i32) as i64) {
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
