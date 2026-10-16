/* { dg-xfail-if "ABI specifies bitfields cannot exceed 32 bits" { mcore-*-* } }
 */

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

struct tmp sub(struct tmp tmp) {
  tmp.field |= 0x0008765412345678LL;
  return tmp;
}

struct tmp2 sub2(struct tmp2 tmp2) {
  tmp2.field |= 0x0008765412345678LL;
  return tmp2;
}

int main(void) {
  struct tmp  tmp  = {0x123, 0xFFF000FFF000FLL};
  struct tmp2 tmp2 = {0xFFF000FFF000FLL, 0x123};

  tmp  = sub(tmp);
  tmp2 = sub2(tmp2);

  if (tmp.pad != 0x123 || tmp.field != 0xFFFFFF541FFF567FLL)
    abort();
  if (tmp2.pad != 0x123 || tmp2.field != 0xFFFFFF541FFF567FLL)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-xfail-if "ABI specifies bitfields cannot exceed 32 bits" { mcore-*-* } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tmp {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tmp2 {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
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
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sub(mut tmp: tmp) -> tmp {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64 = tmp.__slate_bits_0.__get_field();
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i64 = [[V_1]] | (2381903268435576 as i64);
// CHECK-NEXT:     tmp.__slate_bits_0.__set_field([[V_2]]);
// CHECK-NEXT:     return tmp;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sub2(mut tmp2: tmp2) -> tmp2 {
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i64 = tmp2.__slate_bits_0.__get_field();
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i64 = [[V_3]] | (2381903268435576 as i64);
// CHECK-NEXT:     tmp2.__slate_bits_0.__set_field([[V_4]]);
// CHECK-NEXT:     return tmp2;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut tmp: tmp = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: tmp = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_pad((291 as i32) as i64);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_field(4502500384112655 as i64);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut tmp2: tmp2 = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: tmp2 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].__slate_bits_0.__set_field(4502500384112655 as i64);
// CHECK-NEXT:         [[T_1]].__slate_bits_0.__set_pad((291 as i32) as i64);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     tmp = sub(tmp);
// CHECK-NEXT:     tmp2 = sub2(tmp2);
// CHECK-NEXT:     if (tmp.__slate_bits_0.__get_pad() as i32) != (291 as i32)
// CHECK-NEXT:         || (tmp.__slate_bits_0.__get_field() as u64) != (18446743335512004223 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (tmp2.__slate_bits_0.__get_pad() as i32) != (291 as i32)
// CHECK-NEXT:         || (tmp2.__slate_bits_0.__get_field() as u64) != (18446743335512004223 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
