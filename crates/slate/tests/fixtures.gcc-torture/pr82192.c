/* PR rtl-optimization/82192 */

unsigned long long int a = 0x95dd3d896f7422e2ULL;
struct S {
  unsigned int m : 13;
} b;

__attribute__((noinline, noclone)) void foo(void) {
  b.m = ((unsigned)a) >>
        (0x644eee9667723bf7LL | a & ~0xdee27af8U) - 0x644eee9667763bd8LL;
}

int main() {
  if (__INT_MAX__ != 0x7fffffffULL)
    return 0;
  foo();
  if (b.m != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/82192
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: u64 = 10798855141994013410 as u64;
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u16,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(13)]
// CHECK-NEXT:         pub m: u32,
// CHECK-NEXT:         #[bits(3, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_m(
// CHECK-NEXT:             ((unsafe { a }) as u32)
// CHECK-NEXT:                 >> (((7227976781724269559 as i64) as u64)
// CHECK-NEXT:                     | (unsafe { a }) & (!(3739384568 as u32) as u64))
// CHECK-NEXT:                     .wrapping_sub((7227976781724531672 as i64) as u64),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (((2147483647 as i32) as i64) as u64) != (2147483647 as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     foo();
// CHECK-NEXT:     if ((unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_m() }) as i32) != (0 as i32) {
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
