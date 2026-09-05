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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct FixedPair {
// COMMON-LOWERING-NEXT:     left: i16,
// COMMON-LOWERING-NEXT:     right: u32,
// COMMON-LOWERING-NEXT:     count: u64,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut global_size: u64 = 7;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut global_u32: u32 = 4000000000;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i16 = 20;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = add_i32({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = widen_u32({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = use_fixed_types();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn add_i32({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i16) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: /// REWRITES-DAG: println!("{}
// COMMON-LOWERING-NEXT: fn widen_u32({{arg[0-9]+}}: u32) -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { global_size };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn use_fixed_types() -> i32 {
// COMMON-LOWERING-NEXT:     let mut pair: FixedPair = FixedPair {
// COMMON-LOWERING-NEXT:         left: 0,
// COMMON-LOWERING-NEXT:         right: 0,
// COMMON-LOWERING-NEXT:         count: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = -5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 250;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i16 = 1200;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = 65000;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 100000;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = add_i32({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { global_u32 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = widen_u32({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     pair.left = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { global_u32 };
// COMMON-LOWERING-NEXT:     pair.right = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { global_size };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     pair.count = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i16 = pair.left;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = pair.count;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%lu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%lu\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct FixedPair {
// COMMON-REWRITES-NEXT:     left: i16,
// COMMON-REWRITES-NEXT:     right: u32,
// COMMON-REWRITES-NEXT:     count: u64,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut global_size: u64 = 7;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut global_u32: u32 = 4000000000;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), add_i32(10, 20)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%lu\n".as_ptr(), widen_u32(5)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), use_fixed_types()) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn add_i32({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i16) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + ({{arg[0-9]+}} as i32)
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: /// REWRITES-DAG: println!("{}
// COMMON-REWRITES-NEXT: fn widen_u32({{arg[0-9]+}}: u32) -> u64 {
// COMMON-REWRITES-NEXT:     ({{arg[0-9]+}} as u64) + unsafe { global_size }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn use_fixed_types() -> i32 {
// COMMON-REWRITES-NEXT:     let mut pair: FixedPair = FixedPair {
// COMMON-REWRITES-NEXT:         left: 0,
// COMMON-REWRITES-NEXT:         right: 0,
// COMMON-REWRITES-NEXT:         count: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i16 = 1200;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = add_i32(100000, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = widen_u32(unsafe { global_u32 });
// COMMON-REWRITES-NEXT:     pair.left = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     pair.right = unsafe { global_u32 };
// COMMON-REWRITES-NEXT:     pair.count = (unsafe { global_size }) + 3;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (250 as u8) as i32;
// COMMON-REWRITES-NEXT:     (((((-5 as i8) as i32) + {{__v[0-9]+}} + (pair.left as i32) + {{__v[0-9]+}} + ((65000 as u16) as i32)) as u64)
// COMMON-REWRITES-NEXT:         + pair.count
// COMMON-REWRITES-NEXT:         + {{__v[0-9]+}}) as i32
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
