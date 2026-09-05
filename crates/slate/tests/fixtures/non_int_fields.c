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
// LOWERING-X86_64-GNU-NEXT:     tag: i8,
// LOWERING-AARCH64-GNU-NEXT:     tag: u8,
// LOWERING-NEXT:     code: u8,
// LOWERING-NEXT:     ratio: f32,
// LOWERING-NEXT:     total: f64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union Scalar {
// LOWERING-X86_64-GNU-NEXT:     tag: i8,
// LOWERING-AARCH64-GNU-NEXT:     tag: u8,
// LOWERING-NEXT:     code: u8,
// LOWERING-NEXT:     ratio: f32,
// LOWERING-NEXT:     total: f64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = struct_values();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%f\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 4.5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = union_double({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 90;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 90;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = union_char({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn struct_values() -> i32 {
// LOWERING-NEXT:     let mut m: Mixed = Mixed {
// LOWERING-NEXT:         tag: 0,
// LOWERING-NEXT:         code: 0,
// LOWERING-NEXT:         ratio: 0.0,
// LOWERING-NEXT:         total: 0.0,
// LOWERING-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 65;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 65;
// LOWERING-NEXT:     m.tag = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 200;
// LOWERING-NEXT:     m.code = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.5;
// LOWERING-NEXT:     m.ratio = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.25;
// LOWERING-NEXT:     m.total = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = m.tag;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%c\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = m.tag;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%u\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = m.code;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%f\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = m.ratio;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%f\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = m.total;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = m.code;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn union_double({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let mut s: Scalar = unsafe { std::mem::zeroed::<Scalar>() };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         s.total = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { s.total };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn union_char({{arg[0-9]+}}: i8) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn union_char({{arg[0-9]+}}: u8) -> i32 {
// LOWERING-NEXT:     let mut s: Scalar = unsafe { std::mem::zeroed::<Scalar>() };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         s.tag = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { s.tag };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { s.tag };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{__v[0-9]+}};
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
// REWRITES-NEXT: struct Mixed {
// REWRITES-X86_64-GNU-NEXT:     tag: i8,
// REWRITES-AARCH64-GNU-NEXT:     tag: u8,
// REWRITES-NEXT:     code: u8,
// REWRITES-NEXT:     ratio: f32,
// REWRITES-NEXT:     total: f64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union Scalar {
// REWRITES-X86_64-GNU-NEXT:     tag: i8,
// REWRITES-AARCH64-GNU-NEXT:     tag: u8,
// REWRITES-NEXT:     code: u8,
// REWRITES-NEXT:     ratio: f32,
// REWRITES-NEXT:     total: f64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), struct_values()) };
// REWRITES-NEXT:     unsafe { printf(c"%f\n".as_ptr(), union_double(4.5)) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), union_char(90)) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn struct_values() -> i32 {
// REWRITES-NEXT:     let mut m: Mixed = Mixed {
// REWRITES-NEXT:         tag: 0,
// REWRITES-NEXT:         code: 0,
// REWRITES-NEXT:         ratio: 0.0,
// REWRITES-NEXT:         total: 0.0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     m.tag = 65;
// REWRITES-NEXT:     m.code = 200;
// REWRITES-NEXT:     m.ratio = 1.5;
// REWRITES-NEXT:     m.total = 2.25;
// REWRITES-NEXT:     unsafe { printf(c"%c\n".as_ptr(), m.tag as i32) };
// REWRITES-NEXT:     unsafe { printf(c"%u\n".as_ptr(), m.code as i32) };
// REWRITES-NEXT:     unsafe { printf(c"%f\n".as_ptr(), m.ratio as f64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = m.total;
// REWRITES-NEXT:     unsafe { printf(c"%f\n".as_ptr(), {{__v[0-9]+}}) };
// REWRITES-NEXT:     m.code as i32
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn union_double({{arg[0-9]+}}: f64) -> f64 {
// REWRITES-NEXT:     let mut s: Scalar = unsafe { std::mem::zeroed::<Scalar>() };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         s.total = {{arg[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { s.total }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn union_char({{arg[0-9]+}}: i8) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn union_char({{arg[0-9]+}}: u8) -> i32 {
// REWRITES-NEXT:     let mut s: Scalar = unsafe { std::mem::zeroed::<Scalar>() };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         s.tag = {{arg[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     (unsafe { s.tag }) as i32
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
