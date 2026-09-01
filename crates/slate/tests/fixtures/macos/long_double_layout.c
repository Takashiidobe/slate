#include <stddef.h>
#include <stdio.h>

struct ld_box {
  char        tag;
  long double value;
  int         tail;
};

union ld_union {
  long double ld;
  char        bytes[sizeof(long double)];
};

int main(void) {
  printf("%zu %zu\n", sizeof(long double), _Alignof(long double));
  printf("%zu %zu %zu %zu\n", sizeof(struct ld_box), _Alignof(struct ld_box),
         offsetof(struct ld_box, value), offsetof(struct ld_box, tail));
  printf("%zu %zu\n", sizeof(union ld_union), _Alignof(union ld_union));
  return 0;
}
// REWRITES-MACOS-DAG: value: f64
// REWRITES-MACOS-DAG: ld: f64
// REWRITES-MACOS: let _v{{[0-9]+}}: u64 = 8;
// REWRITES-MACOS-NEXT: let _v{{[0-9]+}}: u64 = 8;
// REWRITES-MACOS-DAG: std::mem::offset_of!(ld_box, value) as u64
// REWRITES-MACOS-NOT: LongDouble

// SLATE-FILECHECK-BEGIN lowering-macos
// LOWERING-MACOS: #![feature(c_variadic)]
// LOWERING-MACOS-NEXT: #![allow(
// LOWERING-MACOS-NEXT:     dead_code,
// LOWERING-MACOS-NEXT:     unused,
// LOWERING-MACOS-NEXT:     non_camel_case_types,
// LOWERING-MACOS-NEXT:     non_snake_case,
// LOWERING-MACOS-NEXT:     non_upper_case_globals,
// LOWERING-MACOS-NEXT:     arithmetic_overflow,
// LOWERING-MACOS-NEXT:     unconditional_panic,
// LOWERING-MACOS-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-MACOS-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-MACOS-NEXT:     unused_comparisons
// LOWERING-MACOS-NEXT: )]
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: #[repr(C)]
// LOWERING-MACOS-NEXT: #[derive(Clone, Copy)]
// LOWERING-MACOS-NEXT: struct ld_box {
// LOWERING-MACOS-NEXT:     tag: i8,
// LOWERING-MACOS-NEXT:     value: f64,
// LOWERING-MACOS-NEXT:     tail: i32,
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: #[repr(C)]
// LOWERING-MACOS-NEXT: #[derive(Clone, Copy)]
// LOWERING-MACOS-NEXT: union ld_union {
// LOWERING-MACOS-NEXT:     ld: f64,
// LOWERING-MACOS-NEXT:     bytes: [i8; 8],
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: unsafe extern "C" {
// LOWERING-MACOS-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn main() {
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%zu %zu %zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_box>() as u64;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_box>() as u64;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::offset_of!(ld_box, value) as u64;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::offset_of!(ld_box, tail) as u64;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_union>() as u64;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_union>() as u64;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-MACOS-NEXT: }
// SLATE-FILECHECK-END lowering-macos
