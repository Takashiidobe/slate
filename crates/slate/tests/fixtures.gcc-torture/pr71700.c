struct S {
  signed   f0 : 16;
  unsigned f1 : 1;
};

int             b;
static struct S c[] = {{-1, 0}, {-1, 0}};
struct S        d;

int main() {
  struct S e = c[0];
  d          = e;
  if (d.f1 != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         [u8;3],
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
// CHECK-NEXT:         pub f0: i32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub f1: u32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: [S; 2] = [
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_f0(-(1 as i32));
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_f1((0 as i32) as u32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].__slate_bits_0.__set_f0(-(1 as i32));
// CHECK-NEXT:         [[T_1]].__slate_bits_0.__set_f1((0 as i32) as u32);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     },
// CHECK-NEXT: ];
// CHECK-NEXT: static mut d: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut e: S = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(c) as *mut S }.offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         d = e;
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_f1() }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
