void abort(void);

int main() {
  int b = 0;

  struct {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int bit3 : 1;
    unsigned int bit4 : 1;
    unsigned int bit5 : 1;
    unsigned int bit6 : 1;
    unsigned int bit7 : 1;
  } sdata = {0x01};

  while (sdata.bit0-- > 0) {
    b++;
    if (b > 100)
      break;
  }

  if (b != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
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
// CHECK-NEXT:         pub bit0: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub bit1: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub bit2: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub bit3: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub bit4: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub bit5: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub bit6: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub bit7: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: i32 = 0 as i32;
// CHECK-NEXT:     let mut sdata: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_bit0((1 as i32) as u32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = sdata.__slate_bits_0.__get_bit0();
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = (([[V_0]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:         sdata.__slate_bits_0.__set_bit0([[V_1]]);
// CHECK-NEXT:         if !([[V_0]] > ((0 as i32) as u32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         b = [[V_3]];
// CHECK-NEXT:         if b > (100 as i32) {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if b != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
