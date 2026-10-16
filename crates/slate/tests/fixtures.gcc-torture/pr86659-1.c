#define ENDIANBIG __attribute((scalar_storage_order("big-endian")))

typedef struct ENDIANBIG {
  unsigned long long field0 : 29;
  unsigned long long field1 : 4;
  unsigned long long field2 : 31;
} struct1;

int main(void) {
  int          value1 = 0;
  int          value2 = 0;
  int          value3 = 0;
  unsigned int flag;
  struct1      var1;
  var1.field0 = 23;

  flag   = var1.field0;
  value1 = ((var1.field0) ? 10 : 20);
  if (var1.field0) {
    value2 = 10;
  } else {
    value2 = 20;
  }

  value3 = ((flag) ? 10 : 20);

  if (value1 != 10)
    __builtin_abort();

  if (value2 != 10)
    __builtin_abort();

  if (value3 != 10)
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
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
// CHECK-NEXT:         #[bits(29)]
// CHECK-NEXT:         pub field0: u64,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub field1: u64,
// CHECK-NEXT:         #[bits(31)]
// CHECK-NEXT:         pub field2: u64,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value1: i32 = 0 as i32;
// CHECK-NEXT:     let mut value2: i32 = 0 as i32;
// CHECK-NEXT:     let mut value3: i32 = 0 as i32;
// CHECK-NEXT:     let mut flag: u32 = 0 as u32;
// CHECK-NEXT:     let mut var1: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     var1.__slate_bits_0.__set_field0(((23 as i32) as i64) as u64);
// CHECK-NEXT:     flag = ((var1.__slate_bits_0.__get_field0() as u32) as i32) as u32;
// CHECK-NEXT:     value1 = if ((var1.__slate_bits_0.__get_field0() as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         10 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         20 as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     if ((var1.__slate_bits_0.__get_field0() as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         value2 = 10 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         value2 = 20 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     value3 = if flag != (0 as u32) { 10 as i32 } else { 20 as i32 };
// CHECK-NEXT:     if value1 != (10 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if value2 != (10 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if value3 != (10 as i32) {
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
