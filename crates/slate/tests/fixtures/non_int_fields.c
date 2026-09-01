#include <stdio.h>

struct Mixed {
  char          tag;
  unsigned char code;
  float         ratio;
  double        total;
};

union Scalar {
  char          tag;
  unsigned char code;
  float         ratio;
  double        total;
};

static int struct_values(void) {
  struct Mixed m;
  m.tag   = 'A';
  m.code  = 200;
  m.ratio = 1.5f;
  m.total = 2.25;
  printf("%c\n", m.tag);
  printf("%u\n", m.code);
  printf("%f\n", m.ratio);
  printf("%f\n", m.total);
  return m.code;
}

static double union_double(double value) {
  union Scalar s;
  s.total = value;
  return s.total;
}

static int union_char(char value) {
  union Scalar s;
  s.tag = value;
  return s.tag;
}

int main(void) {
  printf("%d\n", struct_values());
  printf("%f\n", union_double(4.5));
  printf("%d\n", union_char('Z'));
  return 0;
}

// REWRITES-DAG: let _v{{[0-9]+}}: f64 = m.total;
// REWRITES-DAG: unsafe { s.total }
// REWRITES-DAG: (unsafe { s.tag }) as i32

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
// LOWERING-NEXT: struct Mixed {
// LOWERING-NEXT:     tag: i8,
// LOWERING-NEXT:     code: u8,
// LOWERING-NEXT:     ratio: f32,
// LOWERING-NEXT:     total: f64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union Scalar {
// LOWERING-NEXT:     tag: i8,
// LOWERING-NEXT:     code: u8,
// LOWERING-NEXT:     ratio: f32,
// LOWERING-NEXT:     total: f64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn struct_values() -> i32 {
// LOWERING-NEXT:     let mut m: Mixed = Mixed {
// LOWERING-NEXT:         tag: 0,
// LOWERING-NEXT:         code: 0,
// LOWERING-NEXT:         ratio: 0.0,
// LOWERING-NEXT:         total: 0.0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 65;
// LOWERING-NEXT:     m.tag = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 200;
// LOWERING-NEXT:     m.code = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.5;
// LOWERING-NEXT:     m.ratio = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 2.25;
// LOWERING-NEXT:     m.total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = m.tag;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = m.code;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = m.ratio;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = m.total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = m.code;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn union_double({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let mut s: Scalar = unsafe { std::mem::zeroed::<Scalar>() };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         s.total = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { s.total };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn union_char({{arg[0-9]+}}: i8) -> i32 {
// LOWERING-NEXT:     let mut s: Scalar = unsafe { std::mem::zeroed::<Scalar>() };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         s.tag = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { s.tag };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = struct_values();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 4.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = union_double({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 90;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = union_char({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering
