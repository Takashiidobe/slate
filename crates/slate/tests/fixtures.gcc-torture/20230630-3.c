struct S {
  __INT32_TYPE__ i  : 24;
  char           c1 : 1;
  char           c2 : 1;
  char           c3 : 1;
  char           c4 : 1;
  char           c5 : 1;
  char           c6 : 1;
  char           c7 : 1;
  char           c8 : 1;
};

int main(void) {
  struct S s0 = {1193046, 1, 1, 1, 1, 1, 1, 1, 1};
  char    *p  = (char *)&s0;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  if (*p != 86)
    __builtin_abort();
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  if (*p != 18)
    __builtin_abort();
#endif

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
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
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub i: i32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub c1: i8,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub c2: i8,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub c3: i8,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub c4: i8,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub c5: i8,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub c6: i8,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub c7: i8,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub c8: i8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s0: S = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_i(1193046 as i32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_c1((1 as i32) as i8);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_c2((1 as i32) as i8);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_c3((1 as i32) as i8);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_c4((1 as i32) as i8);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_c5((1 as i32) as i8);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_c6((1 as i32) as i8);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_c7((1 as i32) as i8);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_c8((1 as i32) as i8);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut p: *mut i8 = std::ptr::addr_of_mut!(s0) as *mut i8;
// CHECK-NEXT:     if ((unsafe { *p }) as i32) != (86 as i32) {
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
