void abort(void);
void exit(int);

struct s {
  unsigned long long a : 8, b : 32;
};

struct s f(struct s x) {
  x.b = 0xcdef1234;
  return x;
}

int main(void) {
  static struct s i;
  i.a = 12;
  i   = f(i);
  if (i.a != 12 || i.b != 0xcdef1234)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         [u8;5],
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
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub a: u64,
// CHECK-NEXT:         #[bits(32)]
// CHECK-NEXT:         pub b: u64,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut x: s) -> s {
// CHECK-NEXT:     x.__slate_bits_0.__set_b((3454997044 as u32) as u64);
// CHECK-NEXT:     return x;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(i.__slate_bits_0)).__set_a(((12 as i32) as i64) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = f(unsafe { i });
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { (*std::ptr::addr_of!(i.__slate_bits_0)).__get_a() }) as u32) as i32)
// CHECK-NEXT:         != (12 as i32)
// CHECK-NEXT:         || (unsafe { (*std::ptr::addr_of!(i.__slate_bits_0)).__get_b() })
// CHECK-NEXT:             != ((3454997044 as u32) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut i: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
