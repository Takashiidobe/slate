#include <stdio.h>

union BitIntOrArray {
  _BitInt(65) bits;
  char bytes[20];
};

struct NestedBitInt {
  char tag;
  struct {
    int prefix;
    _BitInt(65) value;
  } inner;
  short tail;
};

static struct NestedBitInt values[2] = {
    {.tag = 1, .inner = {.prefix = 2, .value = 333}, .tail = 4},
    {.tag = 5, .inner = {.prefix = 6, .value = 777}, .tail = 8},
};

int main(void) {
  union BitIntOrArray item = {.bytes = {'a', 'b', 'c'}};
  printf("%zu %zu %d %d %lld %d %c%c%c\n", sizeof(item), sizeof(values[0]),
         values[1].tag, values[1].inner.prefix,
         (long long)values[1].inner.value, values[1].tail, item.bytes[0],
         item.bytes[1], item.bytes[2]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union BitIntOrArray {
// CHECK-NEXT:     bits: bitint::BInt<65, 2, 16, bitint::A8>,
// CHECK-NEXT:     bytes: [i8; 20],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct NestedBitInt {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     inner: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT:     tail: i16,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     prefix: i32,
// CHECK-NEXT:     value: bitint::BInt<65, 2, 16, bitint::A8>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut values: __SlateAlign16<[NestedBitInt; 2]> = __SlateAlign16([
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: NestedBitInt = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].tag = (1 as i32) as i8;
// CHECK-NEXT:         [[T_0]].inner = {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].prefix = 2 as i32;
// CHECK-NEXT:             [[T_1]].value = <bitint::BInt<
// CHECK-NEXT:                 65,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_i128((333 as i32) as i128);
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]].tail = (4 as i32) as i16;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: NestedBitInt = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].tag = (5 as i32) as i8;
// CHECK-NEXT:         [[T_2]].inner = {
// CHECK-NEXT:             let mut [[T_3:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_3]].prefix = 6 as i32;
// CHECK-NEXT:             [[T_3]].value = <bitint::BInt<
// CHECK-NEXT:                 65,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_i128((777 as i32) as i128);
// CHECK-NEXT:             [[T_3]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_2]].tail = (8 as i32) as i16;
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let item: BitIntOrArray = {
// CHECK-NEXT:         let mut [[T_0]]: BitIntOrArray = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].bytes = {
// CHECK-NEXT:             let mut [[T_1]]: [i8; 20] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]][0] = 97 as i8;
// CHECK-NEXT:             [[T_1]][1] = 98 as i8;
// CHECK-NEXT:             [[T_1]][2] = 99 as i8;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %zu %d %d %lld %d %c%c%c\n".as_ptr(),
// CHECK-NEXT:             24 as u64,
// CHECK-NEXT:             40 as u64,
// CHECK-NEXT:             (unsafe { values.0[1].tag }) as i32,
// CHECK-NEXT:             unsafe { values.0[1].inner.prefix },
// CHECK-NEXT:             unsafe { values.0[1].inner.value }.to_i128() as i64,
// CHECK-NEXT:             (unsafe { values.0[1].tail }) as i32,
// CHECK-NEXT:             (unsafe { item.bytes[0] }) as i32,
// CHECK-NEXT:             (unsafe { item.bytes[1] }) as i32,
// CHECK-NEXT:             (unsafe { item.bytes[2] }) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
