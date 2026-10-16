/* The fp-bit.c function __floatunsisf had a latent bug where guard bits
   could be lost leading to incorrect rounding.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */

extern void abort(void);
extern void exit(int);
#if __INT_MAX__ >= 0x7fffffff
volatile unsigned u = 0x80000081;
#else
volatile unsigned long u = 0x80000081;
#endif
volatile float f1, f2;
int            main(void) {
  f1 = (float)u;
  f2 = (float)0x80000081;
  if (f1 != f2)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! The fp-bit.c function __floatunsisf had a latent bug where guard bits
// CHECK-NEXT: //! could be lost leading to incorrect rounding.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Origin: Joseph Myers <joseph@codesourcery.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut u: u32 = 2147483777 as u32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut f1: f32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f2: f32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(f1),
// CHECK-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(u)) }) as f32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f2), (2147483777 as u32) as f32) };
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f1)) })
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f2)) }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
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
