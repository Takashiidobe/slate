/* Origin: PR target/11535 from H. J. Lu <hjl@lucon.org> */
/* { dg-require-effective-target return_address } */

void vararg(int i, ...) { (void)i; }

int i0[0], i1;

void test1(void) {
  int a = (int)(long long)__builtin_return_address(0);
  vararg(0, a);
}

void test2(void) { i0[0] = (int)(long long)__builtin_return_address(0); }

void test3(void) { i1 = (int)(long long)__builtin_return_address(0); }

void test4(void) {
  volatile long long a = (long long)__builtin_return_address(0);
  i0[0]                = (int)a;
}

int main(void) { return 0; }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Origin: PR target/11535 from H. J. Lu <hjl@lucon.org>
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target return_address }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.returnaddress.p0"]
// CHECK-NEXT:     fn __slate_intrinsic_ded315013ea6523([[ARG_0:arg[0-9]+]]: i32) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn vararg(mut i: i32, mut __va_args: ...) {
// CHECK-NEXT:     i;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut i0: [i32; 0] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i1: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn test1() {
// CHECK-NEXT:     let mut a: i32 =
// CHECK-NEXT:         ((unsafe { __slate_intrinsic_ded315013ea6523(const { ((0 as i32) as u32) as i32 }) })
// CHECK-NEXT:             as i64) as i32;
// CHECK-NEXT:     unsafe { vararg(0 as i32, a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { std::ptr::addr_of_mut!(i0) as *mut i32 }.offset((0 as i32) as isize) } =
// CHECK-NEXT:             ((unsafe { __slate_intrinsic_ded315013ea6523(const { ((0 as i32) as u32) as i32 }) })
// CHECK-NEXT:                 as i64) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i1 = ((unsafe { __slate_intrinsic_ded315013ea6523(const { ((0 as i32) as u32) as i32 }) })
// CHECK-NEXT:             as i64) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test4() {
// CHECK-NEXT:     let mut a: i64 =
// CHECK-NEXT:         (unsafe { __slate_intrinsic_ded315013ea6523(const { ((0 as i32) as u32) as i32 }) }) as i64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { std::ptr::addr_of_mut!(i0) as *mut i32 }.offset((0 as i32) as isize) } =
// CHECK-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
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
