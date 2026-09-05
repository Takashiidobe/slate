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
// COMMON-LOWERING-NEXT: struct Mixed {
// COMMON-LOWERING-NEXT:     code: u8,
// COMMON-LOWERING-NEXT:     ratio: f32,
// COMMON-LOWERING-NEXT:     total: f64,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union Scalar {
// COMMON-LOWERING-NEXT:     code: u8,
// COMMON-LOWERING-NEXT:     ratio: f32,
// COMMON-LOWERING-NEXT:     total: f64,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = struct_values();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 4.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = union_double({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = union_char({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn struct_values() -> i32 {
// COMMON-LOWERING-NEXT:     let mut m: Mixed = Mixed {
// COMMON-LOWERING-NEXT:         tag: 0,
// COMMON-LOWERING-NEXT:         code: 0,
// COMMON-LOWERING-NEXT:         ratio: 0.0,
// COMMON-LOWERING-NEXT:         total: 0.0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     m.tag = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 200;
// COMMON-LOWERING-NEXT:     m.code = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.5;
// COMMON-LOWERING-NEXT:     m.ratio = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.25;
// COMMON-LOWERING-NEXT:     m.total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = m.code;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = m.ratio;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = m.total;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = m.code;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn union_double({{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let mut s: Scalar = unsafe { std::mem::zeroed::<Scalar>() };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         s.total = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { s.total };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut s: Scalar = unsafe { std::mem::zeroed::<Scalar>() };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         s.tag = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     tag: i8,
// LOWERING-X86_64-GNU-NEXT:     tag: i8,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 90;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 65;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = m.tag;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT: fn union_char({{arg[0-9]+}}: i8) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { s.tag };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     tag: u8,
// LOWERING-AARCH64-GNU-NEXT:     tag: u8,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%f\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 90;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 65;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%c\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = m.tag;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%u\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%f\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%f\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT: fn union_char({{arg[0-9]+}}: u8) -> i32 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { s.tag };
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
// COMMON-REWRITES-NEXT: struct Mixed {
// COMMON-REWRITES-NEXT:     code: u8,
// COMMON-REWRITES-NEXT:     ratio: f32,
// COMMON-REWRITES-NEXT:     total: f64,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union Scalar {
// COMMON-REWRITES-NEXT:     code: u8,
// COMMON-REWRITES-NEXT:     ratio: f32,
// COMMON-REWRITES-NEXT:     total: f64,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), struct_values()) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%f\n".as_ptr(), union_double(4.5)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), union_char(90)) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn struct_values() -> i32 {
// COMMON-REWRITES-NEXT:     let mut m: Mixed = Mixed {
// COMMON-REWRITES-NEXT:         tag: 0,
// COMMON-REWRITES-NEXT:         code: 0,
// COMMON-REWRITES-NEXT:         ratio: 0.0,
// COMMON-REWRITES-NEXT:         total: 0.0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     m.tag = 65;
// COMMON-REWRITES-NEXT:     m.code = 200;
// COMMON-REWRITES-NEXT:     m.ratio = 1.5;
// COMMON-REWRITES-NEXT:     m.total = 2.25;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%c\n".as_ptr(), m.tag as i32) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%u\n".as_ptr(), m.code as i32) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%f\n".as_ptr(), m.ratio as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = m.total;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%f\n".as_ptr(), {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     m.code as i32
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn union_double({{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let mut s: Scalar = unsafe { std::mem::zeroed::<Scalar>() };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         s.total = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { s.total }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let mut s: Scalar = unsafe { std::mem::zeroed::<Scalar>() };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         s.tag = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     (unsafe { s.tag }) as i32
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     tag: i8,
// REWRITES-X86_64-GNU-NEXT:     tag: i8,
// REWRITES-X86_64-GNU-NEXT: fn union_char({{arg[0-9]+}}: i8) -> i32 {
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     tag: u8,
// REWRITES-AARCH64-GNU-NEXT:     tag: u8,
// REWRITES-AARCH64-GNU-NEXT: fn union_char({{arg[0-9]+}}: u8) -> i32 {
// SLATE-FILECHECK-END rewrites-aarch64-gnu
