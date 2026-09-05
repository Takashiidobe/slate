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
// COMMON-LOWERING-NEXT: union BitIntOrArray {
// COMMON-LOWERING-NEXT:     bits: bitint::BInt<65, 2, 16>,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct NestedBitInt {
// COMMON-LOWERING-NEXT:     inner: {{_unnamed_at_[0-9A-Za-z_]+}},
// COMMON-LOWERING-NEXT:     tail: i16,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-LOWERING-NEXT:     prefix: i32,
// COMMON-LOWERING-NEXT:     value: bitint::BInt<65, 2, 16>,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct {{anon_[0-9]+}} {
// COMMON-LOWERING-NEXT:     prefix: i32,
// COMMON-LOWERING-NEXT:     value: bitint::BInt<65, 2, 16>,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut values: aligned::Aligned<aligned::A16, [NestedBitInt; 2]> = aligned::Aligned([
// COMMON-LOWERING-NEXT:     NestedBitInt {
// COMMON-LOWERING-NEXT:         tag: 1,
// COMMON-LOWERING-NEXT:         inner:
// COMMON-LOWERING-NEXT:             {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-LOWERING-NEXT:                 prefix: 2,
// COMMON-LOWERING-NEXT:                 value: bitint::BInt::<65, 2, 16>::from_decimal_str("333"),
// COMMON-LOWERING-NEXT:             },
// COMMON-LOWERING-NEXT:         tail: 4,
// COMMON-LOWERING-NEXT:     },
// COMMON-LOWERING-NEXT:     NestedBitInt {
// COMMON-LOWERING-NEXT:         tag: 5,
// COMMON-LOWERING-NEXT:         inner:
// COMMON-LOWERING-NEXT:             {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-LOWERING-NEXT:                 prefix: 6,
// COMMON-LOWERING-NEXT:                 value: bitint::BInt::<65, 2, 16>::from_decimal_str("777"),
// COMMON-LOWERING-NEXT:             },
// COMMON-LOWERING-NEXT:         tail: 8,
// COMMON-LOWERING-NEXT:     },
// COMMON-LOWERING-NEXT: ]);
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: BitIntOrArray = BitIntOrArray {
// COMMON-LOWERING-NEXT:         bytes: [
// COMMON-LOWERING-NEXT:             97, 98, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// COMMON-LOWERING-NEXT:         ],
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*values)[({{__v[0-9]+}} as usize)].inner.prefix };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BInt<65, 2, 16> = unsafe { (*values)[({{__v[0-9]+}} as usize)].inner.value };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}}.to_i128() as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i16 = unsafe { (*values)[({{__v[0-9]+}} as usize)].tail };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     bytes: [i8; 20],
// LOWERING-X86_64-GNU-NEXT:     tag: i8,
// LOWERING-X86_64-GNU-NEXT:     __slate_anon_1: [u8; 4],
// LOWERING-X86_64-GNU-NEXT:     let mut item: BitIntOrArray = unsafe { std::mem::zeroed::<BitIntOrArray>() };
// LOWERING-X86_64-GNU-NEXT:     item = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu %d %d %lld %d %c%c%c\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = 24;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = 40;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { (*values)[({{__v[0-9]+}} as usize)].tag };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     bytes: [u8; 20],
// LOWERING-AARCH64-GNU-NEXT:     tag: u8,
// LOWERING-AARCH64-GNU-NEXT:     let mut item: aligned::Aligned<aligned::A16, BitIntOrArray> =
// LOWERING-AARCH64-GNU-NEXT:         aligned::Aligned(unsafe { std::mem::zeroed::<BitIntOrArray>() });
// LOWERING-AARCH64-GNU-NEXT:     *item = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu %d %d %lld %d %c%c%c\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = 64;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { (*values)[({{__v[0-9]+}} as usize)].tag };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
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
// COMMON-REWRITES-NEXT: union BitIntOrArray {
// COMMON-REWRITES-NEXT:     bits: bitint::BInt<65, 2, 16>,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct NestedBitInt {
// COMMON-REWRITES-NEXT:     inner: {{_unnamed_at_[0-9A-Za-z_]+}},
// COMMON-REWRITES-NEXT:     tail: i16,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-REWRITES-NEXT:     prefix: i32,
// COMMON-REWRITES-NEXT:     value: bitint::BInt<65, 2, 16>,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct {{anon_[0-9]+}} {
// COMMON-REWRITES-NEXT:     prefix: i32,
// COMMON-REWRITES-NEXT:     value: bitint::BInt<65, 2, 16>,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut values: aligned::Aligned<aligned::A16, [NestedBitInt; 2]> = aligned::Aligned([
// COMMON-REWRITES-NEXT:     NestedBitInt {
// COMMON-REWRITES-NEXT:         tag: 1,
// COMMON-REWRITES-NEXT:         inner:
// COMMON-REWRITES-NEXT:             {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-REWRITES-NEXT:                 prefix: 2,
// COMMON-REWRITES-NEXT:                 value: bitint::BInt::<65, 2, 16>::from_decimal_str("333"),
// COMMON-REWRITES-NEXT:             },
// COMMON-REWRITES-NEXT:         tail: 4,
// COMMON-REWRITES-NEXT:     },
// COMMON-REWRITES-NEXT:     NestedBitInt {
// COMMON-REWRITES-NEXT:         tag: 5,
// COMMON-REWRITES-NEXT:         inner:
// COMMON-REWRITES-NEXT:             {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-REWRITES-NEXT:                 prefix: 6,
// COMMON-REWRITES-NEXT:                 value: bitint::BInt::<65, 2, 16>::from_decimal_str("777"),
// COMMON-REWRITES-NEXT:             },
// COMMON-REWRITES-NEXT:         tail: 8,
// COMMON-REWRITES-NEXT:     },
// COMMON-REWRITES-NEXT: ]);
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:         bytes: [
// COMMON-REWRITES-NEXT:             97, 98, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// COMMON-REWRITES-NEXT:         ],
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*values)[1].inner.prefix };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<65, 2, 16> = unsafe { (*values)[1].inner.value };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%zu %zu %d %d %lld %d %c%c%c\n".as_ptr(),
// COMMON-REWRITES-NEXT:             (unsafe { (*values)[1].tag }) as i32,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}.to_i128() as i64,
// COMMON-REWRITES-NEXT:             (unsafe { (*values)[1].tail }) as i32,
// COMMON-REWRITES-NEXT:             (unsafe { item.bytes[0] }) as i32,
// COMMON-REWRITES-NEXT:             (unsafe { item.bytes[1] }) as i32,
// COMMON-REWRITES-NEXT:             (unsafe { item.bytes[2] }) as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     bytes: [i8; 20],
// REWRITES-X86_64-GNU-NEXT:     tag: i8,
// REWRITES-X86_64-GNU-NEXT:     __slate_anon_1: [u8; 4],
// REWRITES-X86_64-GNU-NEXT:     let mut item: BitIntOrArray = unsafe { std::mem::zeroed::<BitIntOrArray>() };
// REWRITES-X86_64-GNU-NEXT:     item = BitIntOrArray {
// REWRITES-X86_64-GNU-NEXT:             24 as u64,
// REWRITES-X86_64-GNU-NEXT:             40 as u64,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     bytes: [u8; 20],
// REWRITES-AARCH64-GNU-NEXT:     tag: u8,
// REWRITES-AARCH64-GNU-NEXT:     let mut item: aligned::Aligned<aligned::A16, BitIntOrArray> =
// REWRITES-AARCH64-GNU-NEXT:         aligned::Aligned(unsafe { std::mem::zeroed::<BitIntOrArray>() });
// REWRITES-AARCH64-GNU-NEXT:     *item = BitIntOrArray {
// REWRITES-AARCH64-GNU-NEXT:             32 as u64,
// REWRITES-AARCH64-GNU-NEXT:             64 as u64,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
