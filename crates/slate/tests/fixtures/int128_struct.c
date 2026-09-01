#include <stdio.h>

struct Wide {
  int               tag;
  __int128          value;
  unsigned __int128 uvalue;
};

int main(void) {
  struct Wide w;
  w.tag     = 7;
  w.value   = -1234567890123456789;
  w.uvalue  = 12345678901234567890ULL;
  w.value  += 1;
  w.uvalue *= 2;

  printf("%d\n", w.tag);
  printf("%llu\n", (unsigned long long)(w.value >> 64));
  printf("%llu\n", (unsigned long long)w.value);
  printf("%llu\n", (unsigned long long)(w.uvalue >> 64));
  printf("%llu\n", (unsigned long long)w.uvalue);
  printf("%zu\n", sizeof(struct Wide));
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
// LOWERING-NEXT: struct Wide {
// LOWERING-NEXT:     tag: i32,
// LOWERING-NEXT:     value: i128,
// LOWERING-NEXT:     uvalue: u128,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut w: Wide = Wide {
// LOWERING-NEXT:         tag: 0,
// LOWERING-NEXT:         value: 0,
// LOWERING-NEXT:         uvalue: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     w.tag = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = -1234567890123456789i128;
// LOWERING-NEXT:     w.value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = 12345678901234567890u128;
// LOWERING-NEXT:     w.uvalue = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = w.value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     w.value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = w.uvalue;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     w.uvalue = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = w.tag;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = w.value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = {{_v[0-9]+}} >> {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = w.value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = w.uvalue;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = {{_v[0-9]+}} >> {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = w.uvalue;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%zu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::size_of::<Wide>() as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: struct Wide {
// REWRITES-NEXT:     tag: i32,
// REWRITES-NEXT:     value: i128,
// REWRITES-NEXT:     uvalue: u128,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut w: Wide = Wide {
// REWRITES-NEXT:         tag: 0,
// REWRITES-NEXT:         value: 0,
// REWRITES-NEXT:         uvalue: 0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     w.tag = 7;
// REWRITES-NEXT:     w.value = -1234567890123456789i128;
// REWRITES-NEXT:     w.uvalue = 12345678901234567890u128;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i128 = 1;
// REWRITES-NEXT:     w.value = w.value + {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: u128 = 2;
// REWRITES-NEXT:     w.uvalue = w.uvalue * {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = w.tag;
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{_v[0-9]+}}) };
// REWRITES-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), (w.value >> (64 as i32)) as u64) };
// REWRITES-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), w.value as u64) };
// REWRITES-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), (w.uvalue >> (64 as i32)) as u64) };
// REWRITES-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), w.uvalue as u64) };
// REWRITES-NEXT:     unsafe { printf(c"%zu\n".as_ptr(), std::mem::size_of::<Wide>() as u64) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
