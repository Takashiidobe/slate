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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union BitIntOrArray {
// LOWERING-NEXT:     bits: bitint::BInt<65, 2, 16>,
// LOWERING-X86_64-GNU-NEXT:     bytes: [i8; 20],
// LOWERING-AARCH64-GNU-NEXT:     bytes: [u8; 20],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct NestedBitInt {
// LOWERING-X86_64-GNU-NEXT:     tag: i8,
// LOWERING-AARCH64-GNU-NEXT:     tag: u8,
// LOWERING-NEXT:     inner: {{_unnamed_at_[0-9A-Za-z_]+}},
// LOWERING-NEXT:     tail: i16,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// LOWERING-NEXT:     prefix: i32,
// LOWERING-NEXT:     value: bitint::BInt<65, 2, 16>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_[0-9]+}} {
// LOWERING-NEXT:     prefix: i32,
// LOWERING-X86_64-GNU-NEXT:     __slate_anon_1: [u8; 4],
// LOWERING-NEXT:     value: bitint::BInt<65, 2, 16>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut values: aligned::Aligned<aligned::A16, [NestedBitInt; 2]> = aligned::Aligned([
// LOWERING-NEXT:     NestedBitInt {
// LOWERING-NEXT:         tag: 1,
// LOWERING-NEXT:         inner:
// LOWERING-NEXT:             {{_unnamed_at_[0-9A-Za-z_]+}} {
// LOWERING-NEXT:                 prefix: 2,
// LOWERING-NEXT:                 value: bitint::BInt::<65, 2, 16>::from_decimal_str("333"),
// LOWERING-NEXT:             },
// LOWERING-NEXT:         tail: 4,
// LOWERING-NEXT:     },
// LOWERING-NEXT:     NestedBitInt {
// LOWERING-NEXT:         tag: 5,
// LOWERING-NEXT:         inner:
// LOWERING-NEXT:             {{_unnamed_at_[0-9A-Za-z_]+}} {
// LOWERING-NEXT:                 prefix: 6,
// LOWERING-NEXT:                 value: bitint::BInt::<65, 2, 16>::from_decimal_str("777"),
// LOWERING-NEXT:             },
// LOWERING-NEXT:         tail: 8,
// LOWERING-NEXT:     },
// LOWERING-NEXT: ]);
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let mut item: BitIntOrArray = unsafe { std::mem::zeroed::<BitIntOrArray>() };
// LOWERING-AARCH64-GNU-NEXT:     let mut item: aligned::Aligned<aligned::A16, BitIntOrArray> =
// LOWERING-AARCH64-GNU-NEXT:         aligned::Aligned(unsafe { std::mem::zeroed::<BitIntOrArray>() });
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: BitIntOrArray = BitIntOrArray {
// LOWERING-NEXT:         bytes: [
// LOWERING-NEXT:             97, 98, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// LOWERING-NEXT:         ],
// LOWERING-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     item = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu %d %d %lld %d %c%c%c\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = 24;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = 40;
// LOWERING-AARCH64-GNU-NEXT:     *item = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu %d %d %lld %d %c%c%c\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = 64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { (*values)[({{__v[0-9]+}} as usize)].tag };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { (*values)[({{__v[0-9]+}} as usize)].tag };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*values)[({{__v[0-9]+}} as usize)].inner.prefix };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BInt<65, 2, 16> = unsafe { (*values)[({{__v[0-9]+}} as usize)].inner.value };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}}.to_i128() as i64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i16 = unsafe { (*values)[({{__v[0-9]+}} as usize)].tail };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union BitIntOrArray {
// REWRITES-NEXT:     bits: bitint::BInt<65, 2, 16>,
// REWRITES-X86_64-GNU-NEXT:     bytes: [i8; 20],
// REWRITES-AARCH64-GNU-NEXT:     bytes: [u8; 20],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct NestedBitInt {
// REWRITES-X86_64-GNU-NEXT:     tag: i8,
// REWRITES-AARCH64-GNU-NEXT:     tag: u8,
// REWRITES-NEXT:     inner: {{_unnamed_at_[0-9A-Za-z_]+}},
// REWRITES-NEXT:     tail: i16,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// REWRITES-NEXT:     prefix: i32,
// REWRITES-NEXT:     value: bitint::BInt<65, 2, 16>,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{anon_[0-9]+}} {
// REWRITES-NEXT:     prefix: i32,
// REWRITES-X86_64-GNU-NEXT:     __slate_anon_1: [u8; 4],
// REWRITES-NEXT:     value: bitint::BInt<65, 2, 16>,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut values: aligned::Aligned<aligned::A16, [NestedBitInt; 2]> = aligned::Aligned([
// REWRITES-NEXT:     NestedBitInt {
// REWRITES-NEXT:         tag: 1,
// REWRITES-NEXT:         inner:
// REWRITES-NEXT:             {{_unnamed_at_[0-9A-Za-z_]+}} {
// REWRITES-NEXT:                 prefix: 2,
// REWRITES-NEXT:                 value: bitint::BInt::<65, 2, 16>::from_decimal_str("333"),
// REWRITES-NEXT:             },
// REWRITES-NEXT:         tail: 4,
// REWRITES-NEXT:     },
// REWRITES-NEXT:     NestedBitInt {
// REWRITES-NEXT:         tag: 5,
// REWRITES-NEXT:         inner:
// REWRITES-NEXT:             {{_unnamed_at_[0-9A-Za-z_]+}} {
// REWRITES-NEXT:                 prefix: 6,
// REWRITES-NEXT:                 value: bitint::BInt::<65, 2, 16>::from_decimal_str("777"),
// REWRITES-NEXT:             },
// REWRITES-NEXT:         tail: 8,
// REWRITES-NEXT:     },
// REWRITES-NEXT: ]);
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     let mut item: BitIntOrArray = unsafe { std::mem::zeroed::<BitIntOrArray>() };
// REWRITES-X86_64-GNU-NEXT:     item = BitIntOrArray {
// REWRITES-AARCH64-GNU-NEXT:     let mut item: aligned::Aligned<aligned::A16, BitIntOrArray> =
// REWRITES-AARCH64-GNU-NEXT:         aligned::Aligned(unsafe { std::mem::zeroed::<BitIntOrArray>() });
// REWRITES-AARCH64-GNU-NEXT:     *item = BitIntOrArray {
// REWRITES-NEXT:         bytes: [
// REWRITES-NEXT:             97, 98, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// REWRITES-NEXT:         ],
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*values)[1].inner.prefix };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<65, 2, 16> = unsafe { (*values)[1].inner.value };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%zu %zu %d %d %lld %d %c%c%c\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:             24 as u64,
// REWRITES-X86_64-GNU-NEXT:             40 as u64,
// REWRITES-AARCH64-GNU-NEXT:             32 as u64,
// REWRITES-AARCH64-GNU-NEXT:             64 as u64,
// REWRITES-NEXT:             (unsafe { (*values)[1].tag }) as i32,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}}.to_i128() as i64,
// REWRITES-NEXT:             (unsafe { (*values)[1].tail }) as i32,
// REWRITES-NEXT:             (unsafe { item.bytes[0] }) as i32,
// REWRITES-NEXT:             (unsafe { item.bytes[1] }) as i32,
// REWRITES-NEXT:             (unsafe { item.bytes[2] }) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
