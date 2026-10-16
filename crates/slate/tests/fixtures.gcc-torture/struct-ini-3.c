void abort(void);
void exit(int);

struct {
  unsigned int f1 : 1, f2 : 1, f3 : 3, f4 : 3, f5 : 2, f6 : 1, f7 : 1;
} result = {1, 1, 7, 7, 3, 1, 1};

int main(void) {
  if ((result.f3 & ~7) != 0 || (result.f4 & ~7) != 0)
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
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
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
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub f1: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub f2: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub f3: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub f4: u32,
// CHECK-NEXT:         #[bits(2)]
// CHECK-NEXT:         pub f5: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub f6: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub f7: u32,
// CHECK-NEXT:         #[bits(4, access = na)]
// CHECK-NEXT:         pub __slate_pad_7: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut result: [[SLATERECORD_0]] = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_f1((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_f2((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_f3((7 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_f4((7 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_f5((3 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_f6((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_f7((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((unsafe { (*std::ptr::addr_of!(result.__slate_bits_0)).__get_f3() }) as i32)
// CHECK-NEXT:         & !(7 as i32) != (0 as i32)
// CHECK-NEXT:         || ((unsafe { (*std::ptr::addr_of!(result.__slate_bits_0)).__get_f4() }) as i32)
// CHECK-NEXT:             & !(7 as i32) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
