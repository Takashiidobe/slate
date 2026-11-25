typedef signed short i16x4 __attribute__((vector_size(8)));
typedef unsigned short u16x3 __attribute__((ext_vector_type(3)));
typedef signed _BitInt(17) i17;
typedef unsigned _BitInt(17) u17;
typedef signed _BitInt(97) i97;
typedef unsigned _BitInt(97) u97;

// @check-fn-begin
static unsigned char reverse_u8(unsigned char value) {
  return __builtin_elementwise_bitreverse(value);
}
// @check-fn-end

// @check-fn-begin
static signed char reverse_i8(signed char value) {
  return __builtin_elementwise_bitreverse(value);
}
// @check-fn-end

// @check-fn-begin
static int check_vectors(i16x4 signed_lanes, u16x3 unsigned_lanes) {
  i16x4 s = __builtin_elementwise_bitreverse(signed_lanes);
  u16x3 u = __builtin_elementwise_bitreverse(unsigned_lanes);
  return s[0] != -32768 || s[1] != 32767 || s[2] != 0x2c48 || s[3] != 0x4000 ||
         u[0] != 0xf7b3 || u[1] != 0x0001 || u[2] != 0x8000;
}
// @check-fn-end

// @check-fn-begin
static int check_bitints(i17 narrow_signed, u17 narrow_unsigned,
                         i97 wide_signed, u97 wide_unsigned) {
  i17 s = __builtin_elementwise_bitreverse(narrow_signed);
  u17 u = __builtin_elementwise_bitreverse(narrow_unsigned);
  i97 wide_s = __builtin_elementwise_bitreverse(wide_signed);
  u97 wide_u = __builtin_elementwise_bitreverse(wide_unsigned);
  return s != -65536 || u != 0x18000 ||
         wide_s != (i97)(((u97)1 << 96) - 1) ||
         wide_u != (((u97)1 << 96) | ((u97)1 << 32) | 1);
}
// @check-fn-end

int main(void) {
  unsigned char operands[2] = {0x12, 0x80};
  int index = 0;
  if (reverse_u8(operands[0]) != 0x48 || reverse_i8(-2) != 127 ||
      __builtin_elementwise_bitreverse(operands[index++]) != 0x48 || index != 1)
    return 1;
  if (check_vectors((i16x4){1, -2, 0x1234, 2}, (u16x3){0xcdef, 0x8000, 1}))
    return 2;
  return 3 * check_bitints(1, 3, -2, ((u97)1 << 96) | ((u97)1 << 64) | 1);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu armv7-unknown-linux-gnueabihf
// CHECK-LABEL: {{^}}fn reverse_u8(value: u8) -> u8 {
// CHECK-NEXT:     u8::reverse_bits(value)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn reverse_i8(value: i8) -> i8 {
// CHECK-NEXT:     i8::reverse_bits(value)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn check_vectors(
// CHECK-NEXT:     signed_lanes: core::simd::Simd<i16, 4>,
// CHECK-NEXT:     unsigned_lanes: __slate_vector::__SlateVector<u16, 3, __slate_vector::Align8>,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let s: core::simd::Simd<i16, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:         signed_lanes.to_array().map(i16::reverse_bits),
// CHECK-NEXT:     );
// CHECK-NEXT:     let u: __slate_vector::__SlateVector<u16, 3, __slate_vector::Align8> = __slate_vector::__SlateVector::from_array(
// CHECK-NEXT:         unsigned_lanes.to_array().map(u16::reverse_bits),
// CHECK-NEXT:     );
// CHECK-NEXT:     ((s[0] as i32) != -32768 || (s[1] as i32) != 32767 || (s[2] as i32) != 11336
// CHECK-NEXT:         || (s[3] as i32) != 16384 || (u[0] as i32) != 63411 || (u[1] as i32) != 1
// CHECK-NEXT:         || (u[2] as i32) != 32768) as i32
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn check_bitints(
// CHECK-NEXT:     narrow_signed: i32,
// CHECK-NEXT:     narrow_unsigned: u32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     wide_signed: bitint::BInt<97, 2, 16, bitint::A8>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     wide_unsigned: bitint::BUint<97, 2, 16, bitint::A8>,
// I686-UNKNOWN-LINUX-GNU-NEXT:     wide_signed: bitint::BInt<97, 2, 16, bitint::A4>,
// I686-UNKNOWN-LINUX-GNU-NEXT:     wide_unsigned: bitint::BUint<97, 2, 16, bitint::A4>,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     wide_signed: bitint::BInt<97, 2, 16, bitint::A16>,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     wide_unsigned: bitint::BUint<97, 2, 16, bitint::A16>,
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:     wide_signed: bitint::BInt<97, 2, 16, bitint::A8>,
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:     wide_unsigned: bitint::BUint<97, 2, 16, bitint::A8>,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let s: i32 = i32::reverse_bits(narrow_signed) >> 15;
// CHECK-NEXT:     let u: u32 = u32::reverse_bits(narrow_unsigned) >> 15;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let wide_s: bitint::BInt<97, 2, 16, bitint::A8> = wide_signed.reverse_bits();
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let wide_u: bitint::BUint<97, 2, 16, bitint::A8> = wide_unsigned.reverse_bits();
// I686-UNKNOWN-LINUX-GNU-NEXT:     let wide_s: bitint::BInt<97, 2, 16, bitint::A4> = wide_signed.reverse_bits();
// I686-UNKNOWN-LINUX-GNU-NEXT:     let wide_u: bitint::BUint<97, 2, 16, bitint::A4> = wide_unsigned.reverse_bits();
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let wide_s: bitint::BInt<97, 2, 16, bitint::A16> = wide_signed.reverse_bits();
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let wide_u: bitint::BUint<97, 2, 16, bitint::A16> = wide_unsigned.reverse_bits();
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:     let wide_s: bitint::BInt<97, 2, 16, bitint::A8> = wide_signed.reverse_bits();
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:     let wide_u: bitint::BUint<97, 2, 16, bitint::A8> = wide_unsigned.reverse_bits();
// CHECK-NEXT:     ((s as i32) != -65536 || ((u as u32) as i32) != 98304
// CHECK-NEXT:         || wide_s
// CHECK-NEXT:             .compare(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 <bitint::BInt<97, 2, 16, bitint::A8>>::from_buint(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     <bitint::BUint<97, 2, 16, bitint::A8>>::from_bint(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             <bitint::BInt<97, 2, 16, bitint::A8>>::from_i128(1 as i128),
// I686-UNKNOWN-LINUX-GNU-NEXT:                 <bitint::BInt<97, 2, 16, bitint::A4>>::from_buint(
// I686-UNKNOWN-LINUX-GNU-NEXT:                     <bitint::BUint<97, 2, 16, bitint::A4>>::from_bint(
// I686-UNKNOWN-LINUX-GNU-NEXT:                             <bitint::BInt<97, 2, 16, bitint::A4>>::from_i128(1 as i128),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 <bitint::BInt<97, 2, 16, bitint::A16>>::from_buint(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     <bitint::BUint<97, 2, 16, bitint::A16>>::from_bint(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                             <bitint::BInt<97, 2, 16, bitint::A16>>::from_i128(1 as i128),
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:                 <bitint::BInt<97, 2, 16, bitint::A8>>::from_buint(
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:                     <bitint::BUint<97, 2, 16, bitint::A8>>::from_bint(
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:                             <bitint::BInt<97, 2, 16, bitint::A8>>::from_i128(1 as i128),
// CHECK-NEXT:                         )
// CHECK-NEXT:                         .shl_bits(96 as usize)
// CHECK-NEXT:                         .wrapping_sub(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             <bitint::BUint<97, 2, 16, bitint::A8>>::from_u128(1),
// I686-UNKNOWN-LINUX-GNU-NEXT:                             <bitint::BUint<97, 2, 16, bitint::A4>>::from_u128(1),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                             <bitint::BUint<97, 2, 16, bitint::A16>>::from_u128(1),
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:                             <bitint::BUint<97, 2, 16, bitint::A8>>::from_u128(1),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                 ),
// CHECK-NEXT:             ) != 0
// CHECK-NEXT:         || wide_u
// CHECK-NEXT:             .compare(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 <bitint::BUint<97, 2, 16, bitint::A8>>::from_bint(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         <bitint::BInt<97, 2, 16, bitint::A8>>::from_i128(1 as i128),
// I686-UNKNOWN-LINUX-GNU-NEXT:                 <bitint::BUint<97, 2, 16, bitint::A4>>::from_bint(
// I686-UNKNOWN-LINUX-GNU-NEXT:                         <bitint::BInt<97, 2, 16, bitint::A4>>::from_i128(1 as i128),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 <bitint::BUint<97, 2, 16, bitint::A16>>::from_bint(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                         <bitint::BInt<97, 2, 16, bitint::A16>>::from_i128(1 as i128),
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:                 <bitint::BUint<97, 2, 16, bitint::A8>>::from_bint(
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:                         <bitint::BInt<97, 2, 16, bitint::A8>>::from_i128(1 as i128),
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .shl_bits(96 as usize)
// CHECK-NEXT:                     .bitor_bits(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         <bitint::BUint<97, 2, 16, bitint::A8>>::from_bint(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                                 <bitint::BInt<97, 2, 16, bitint::A8>>::from_i128(1 as i128),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         <bitint::BUint<97, 2, 16, bitint::A4>>::from_bint(
// I686-UNKNOWN-LINUX-GNU-NEXT:                                 <bitint::BInt<97, 2, 16, bitint::A4>>::from_i128(1 as i128),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                         <bitint::BUint<97, 2, 16, bitint::A16>>::from_bint(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                                 <bitint::BInt<97, 2, 16, bitint::A16>>::from_i128(1 as i128),
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:                         <bitint::BUint<97, 2, 16, bitint::A8>>::from_bint(
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:                                 <bitint::BInt<97, 2, 16, bitint::A8>>::from_i128(1 as i128),
// CHECK-NEXT:                             )
// CHECK-NEXT:                             .shl_bits(32 as usize),
// CHECK-NEXT:                     )
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     .bitor_bits(<bitint::BUint<97, 2, 16, bitint::A8>>::from_u128(1)),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     .bitor_bits(<bitint::BUint<97, 2, 16, bitint::A4>>::from_u128(1)),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     .bitor_bits(<bitint::BUint<97, 2, 16, bitint::A16>>::from_u128(1)),
// ARMV7-UNKNOWN-LINUX-GNUEABIHF-NEXT:                     .bitor_bits(<bitint::BUint<97, 2, 16, bitint::A8>>::from_u128(1)),
// CHECK-NEXT:             ) != 0) as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
