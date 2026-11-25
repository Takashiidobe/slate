unsigned char _rotl8(unsigned char value, unsigned char count);
unsigned _rotr(unsigned value, int count);

int main(void) {
  return _rotl8(8, 2) != 10 || _rotr(9, 3) != 6;
}

unsigned char _rotl8(unsigned char value, unsigned char count) {
  return value + count;
}

unsigned _rotr(unsigned value, int count) {
  return value - count;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     ((_rotl8(8u8, 2u8) as i32) != 10 || _rotr(9u32, 3i32) != 6) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn _rotl8(value: u8, count: u8) -> u8 {
// CHECK-NEXT:     ((value as i32) + (count as i32)) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn _rotr(value: u32, count: i32) -> u32 {
// CHECK-NEXT:     value.wrapping_sub(count as u32)
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
