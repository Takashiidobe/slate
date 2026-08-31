#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

struct FixedPair {
  int16_t  left;
  uint32_t right;
  size_t   count;
};

static uint32_t global_u32  = 4000000000u;
static size_t   global_size = 7;

static int32_t add_i32(int32_t a, int16_t b) {
  int32_t sum = a + b;
  return sum;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct FixedPair {
// LOWERING-NEXT:     left: i16,
// LOWERING-NEXT:     right: u32,
// LOWERING-NEXT:     count: u64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut global_size: u64 = 7;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut global_u32: u32 = 4000000000;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_i32({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i16) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: /// REWRITES-DAG: println!("{}
// LOWERING-NEXT: fn widen_u32({{arg[0-9]+}}: u32) -> u64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { global_size };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn use_fixed_types() -> i32 {
// LOWERING-NEXT:     let mut pair: FixedPair = FixedPair { left: 0, right: 0, count: 0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = -5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 250;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = 1200;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = 65000;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 100000;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = add_i32({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { global_u32 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = widen_u32({{_v[0-9]+}});
// LOWERING-NEXT:     pair.left = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { global_u32 };
// LOWERING-NEXT:     pair.right = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { global_size };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     pair.count = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = pair.left;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = pair.count;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = 20;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = add_i32({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%lu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = widen_u32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = use_fixed_types();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: println!("{}

static uint64_t widen_u32(uint32_t value) {
  uint64_t wide = value + global_size;
  return wide;
}

static int use_fixed_types(void) {
  int8_t           small        = -5;
  uint8_t          byte         = 250;
  int16_t          short_value  = 1200;
  uint16_t         ushort_value = 65000;
  int32_t          signed_value = add_i32(100000, short_value);
  uint64_t         wide         = widen_u32(global_u32);
  struct FixedPair pair;
  pair.left  = short_value;
  pair.right = global_u32;
  pair.count = global_size + 3;
  return small + byte + pair.left + signed_value + ushort_value + pair.count +
         wide;
}

int main(void) {
  printf("%d\n", add_i32(10, 20));
  printf("%lu\n", widen_u32(5));
  printf("%d\n", use_fixed_types());
  return 0;
}
