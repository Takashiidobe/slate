/* PR tree-optimization/86492 */

union U {
  unsigned int r;
  struct S {
    unsigned int a : 12;
    unsigned int b : 4;
    unsigned int c : 16;
  } f;
};

__attribute__((noipa)) unsigned int foo(unsigned int x) {
  union U u;
  u.r   = 0;
  u.f.c = x;
  u.f.b = 0xe;
  return u.r;
}

int main() {
  union U u;
  if (__CHAR_BIT__ * __SIZEOF_INT__ != 32 || sizeof(u.r) != sizeof(u.f))
    return 0;
  u.r = foo(0x72);
  if (u.f.a != 0 || u.f.b != 0xe || u.f.c != 0x72)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/86492
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union U {
// CHECK-NEXT:     r: u32,
// CHECK-NEXT:     f: S,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub c: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut u: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.r = (0 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.f.__slate_bits_0.__set_c(x);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.f.__slate_bits_0.__set_b((14 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { u.r }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut u: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if (8 as i32) * (4 as i32) != (32 as i32) || (4 as u64) != (4 as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.r = foo((114 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { u.f.__slate_bits_0.__get_a() }) as i32) != (0 as i32)
// CHECK-NEXT:         || ((unsafe { u.f.__slate_bits_0.__get_b() }) as i32) != (14 as i32)
// CHECK-NEXT:         || ((unsafe { u.f.__slate_bits_0.__get_c() }) as i32) != (114 as i32)
// CHECK-NEXT:     {
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
