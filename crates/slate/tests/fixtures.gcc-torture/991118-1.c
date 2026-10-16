void abort(void);
void exit(int);

struct tmp {
  long long int pad   : 12;
  long long int field : 52;
};

struct tmp2 {
  long long int field : 52;
  long long int pad   : 12;
};

struct tmp3 {
  long long int pad   : 11;
  long long int field : 53;
};

struct tmp4 {
  long long int field : 53;
  long long int pad   : 11;
};

struct tmp sub(struct tmp tmp) {
  tmp.field ^= 0x0008765412345678LL;
  return tmp;
}

struct tmp2 sub2(struct tmp2 tmp2) {
  tmp2.field ^= 0x0008765412345678LL;
  return tmp2;
}

struct tmp3 sub3(struct tmp3 tmp3) {
  tmp3.field ^= 0x0018765412345678LL;
  return tmp3;
}

struct tmp4 sub4(struct tmp4 tmp4) {
  tmp4.field ^= 0x0018765412345678LL;
  return tmp4;
}

struct tmp  tmp  = {0x123, 0x123456789ABCDLL};
struct tmp2 tmp2 = {0x123456789ABCDLL, 0x123};
struct tmp3 tmp3 = {0x123, 0x1FFFF00000000LL};
struct tmp4 tmp4 = {0x1FFFF00000000LL, 0x123};

int main(void) {

  if (sizeof(long long) != 8)
    exit(0);

  tmp  = sub(tmp);
  tmp2 = sub2(tmp2);

  if (tmp.pad != 0x123 || tmp.field != 0xFFF9551175BDFDB5LL)
    abort();
  if (tmp2.pad != 0x123 || tmp2.field != 0xFFF9551175BDFDB5LL)
    abort();

  tmp3 = sub3(tmp3);
  tmp4 = sub4(tmp4);
  if (tmp3.pad != 0x123 || tmp3.field != 0xFFF989AB12345678LL)
    abort();
  if (tmp4.pad != 0x123 || tmp4.field != 0xFFF989AB12345678LL)
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tmp {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tmp2 {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tmp3 {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tmp4 {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
// CHECK-NEXT: }
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
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub pad: i64,
// CHECK-NEXT:         #[bits(52)]
// CHECK-NEXT:         pub field: i64,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(52)]
// CHECK-NEXT:         pub field: i64,
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub pad: i64,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits2U0 {
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub pad: i64,
// CHECK-NEXT:         #[bits(53)]
// CHECK-NEXT:         pub field: i64,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits3U0 {
// CHECK-NEXT:         #[bits(53)]
// CHECK-NEXT:         pub field: i64,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub pad: i64,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn sub(mut [[TMP_0:tmp_[0-9]+]]: tmp) -> tmp {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64 = [[TMP_0]].__slate_bits_0.__get_field();
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i64 = [[V_1]] ^ (2381903268435576 as i64);
// CHECK-NEXT:     [[TMP_0]].__slate_bits_0.__set_field([[V_2]]);
// CHECK-NEXT:     return [[TMP_0]];
// CHECK-NEXT: }
// CHECK-NEXT: fn sub2(mut [[TMP2_0:tmp[0-9]+_[0-9]+]]: tmp2) -> tmp2 {
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i64 = [[TMP2_0]].__slate_bits_0.__get_field();
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i64 = [[V_3]] ^ (2381903268435576 as i64);
// CHECK-NEXT:     [[TMP2_0]].__slate_bits_0.__set_field([[V_4]]);
// CHECK-NEXT:     return [[TMP2_0]];
// CHECK-NEXT: }
// CHECK-NEXT: fn sub3(mut [[TMP3_0:tmp[0-9]+_[0-9]+]]: tmp3) -> tmp3 {
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i64 = [[TMP3_0]].__slate_bits_0.__get_field();
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: i64 = [[V_5]] ^ (6885502895806072 as i64);
// CHECK-NEXT:     [[TMP3_0]].__slate_bits_0.__set_field([[V_6]]);
// CHECK-NEXT:     return [[TMP3_0]];
// CHECK-NEXT: }
// CHECK-NEXT: fn sub4(mut [[TMP4_0:tmp[0-9]+_[0-9]+]]: tmp4) -> tmp4 {
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: i64 = [[TMP4_0]].__slate_bits_0.__get_field();
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: i64 = [[V_7]] ^ (6885502895806072 as i64);
// CHECK-NEXT:     [[TMP4_0]].__slate_bits_0.__set_field([[V_8]]);
// CHECK-NEXT:     return [[TMP4_0]];
// CHECK-NEXT: }
// CHECK-NEXT: static mut tmp: tmp = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: tmp = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_pad((291 as i32) as i64);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_field(320255973501901 as i64);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut tmp2: tmp2 = {
// CHECK-NEXT:     let mut [[T_0]]: tmp2 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_field(320255973501901 as i64);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_pad((291 as i32) as i64);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut tmp3: tmp3 = {
// CHECK-NEXT:     let mut [[T_0]]: tmp3 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_pad((291 as i32) as i64);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_field(562945658454016 as i64);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut tmp4: tmp4 = {
// CHECK-NEXT:     let mut [[T_0]]: tmp4 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_field(562945658454016 as i64);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_pad((291 as i32) as i64);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (8 as u64) != (((8 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         tmp = sub(unsafe { tmp });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         tmp2 = sub2(unsafe { tmp2 });
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { (*std::ptr::addr_of!(tmp.__slate_bits_0)).__get_pad() }) as i32)
// CHECK-NEXT:         != (291 as i32)
// CHECK-NEXT:         || ((unsafe { (*std::ptr::addr_of!(tmp.__slate_bits_0)).__get_field() }) as u64)
// CHECK-NEXT:             != (18444867282350767541 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { (*std::ptr::addr_of!(tmp2.__slate_bits_0)).__get_pad() }) as i32)
// CHECK-NEXT:         != (291 as i32)
// CHECK-NEXT:         || ((unsafe { (*std::ptr::addr_of!(tmp2.__slate_bits_0)).__get_field() }) as u64)
// CHECK-NEXT:             != (18444867282350767541 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         tmp3 = sub3(unsafe { tmp3 });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         tmp4 = sub4(unsafe { tmp4 });
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { (*std::ptr::addr_of!(tmp3.__slate_bits_0)).__get_pad() }) as i32)
// CHECK-NEXT:         != (291 as i32)
// CHECK-NEXT:         || ((unsafe { (*std::ptr::addr_of!(tmp3.__slate_bits_0)).__get_field() }) as u64)
// CHECK-NEXT:             != (18444925116710409848 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { (*std::ptr::addr_of!(tmp4.__slate_bits_0)).__get_pad() }) as i32)
// CHECK-NEXT:         != (291 as i32)
// CHECK-NEXT:         || ((unsafe { (*std::ptr::addr_of!(tmp4.__slate_bits_0)).__get_field() }) as u64)
// CHECK-NEXT:             != (18444925116710409848 as u64)
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
