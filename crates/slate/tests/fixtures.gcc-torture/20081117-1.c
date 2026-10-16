/* { dg-require-effective-target int32plus } */
extern void abort(void);

struct s {
  unsigned long long a : 16;
  unsigned long long b : 32;
  unsigned long long c : 16;
};

__attribute__((noinline)) unsigned f(struct s s, unsigned i) {
  return s.b == i;
}

struct s s = {1, 0x87654321u, 2};

int main() {
  if (!f(s, 0x87654321u))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-require-effective-target int32plus }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u64,
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
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub a: u64,
// CHECK-NEXT:         #[bits(32)]
// CHECK-NEXT:         pub b: u64,
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub c: u64,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut [[S_0:s_[0-9]+]]: s, mut i: u32) -> u32 {
// CHECK-NEXT:     ([[S_0]].__slate_bits_0.__get_b() == (i as u64)) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: s = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(((1 as i32) as i64) as u64);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((2271560481 as u32) as u64);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(((2 as i32) as i64) as u64);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if !(f(unsafe { s }, 2271560481 as u32) != (0 as u32)) {
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
