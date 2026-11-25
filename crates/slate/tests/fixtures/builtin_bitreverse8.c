static unsigned char reverse_byte(unsigned char value) {
  return __builtin_bitreverse8(value);
}

static unsigned short reverse_u16(unsigned short value) {
  return __builtin_bitreverse16(value);
}

static unsigned int reverse_u32(unsigned int value) {
  return __builtin_bitreverse32(value);
}

static unsigned long long reverse_u64(unsigned long long value) {
  return __builtin_bitreverse64(value);
}

int main(void) {
  return reverse_byte(0x12) != 0x48 ||
         reverse_u16(0x1234) != 0x2c48 ||
         reverse_u32(0x89abcdefU) != 0xf7b3d591U ||
         reverse_u64(0x0123456789abcdefULL) != 0xf7b3d591e6a2c480ULL;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu armv7-unknown-linux-gnueabihf
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn reverse_byte(value: u8) -> u8 {
// CHECK-NEXT:     u8::reverse_bits(value)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn reverse_u16(value: u16) -> u16 {
// CHECK-NEXT:     u16::reverse_bits(value)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn reverse_u32(value: u32) -> u32 {
// CHECK-NEXT:     u32::reverse_bits(value)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn reverse_u64(value: u64) -> u64 {
// CHECK-NEXT:     u64::reverse_bits(value)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = (reverse_byte(18u8) as i32) != 72
// CHECK-NEXT:         || (reverse_u16(4660u16) as i32) != 11336;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool = [[V_0]] || reverse_u32(2309737967u32) != 4155757969;
// CHECK-NEXT:     ([[V_1]] || reverse_u64(81985529216486895u64) != 17848844570815808640) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
