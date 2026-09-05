#include <stdio.h>

static volatile char   marker = 65;
static volatile double gain   = 1.5;

struct VolatileFields {
  volatile int    count;
  volatile double ratio;
};

static volatile int bump_return(int value) { return value + 1; }

static double read_volatile_param(volatile double value) { return value + 0.5; }

static double use_volatile_fields(double input) {
  struct VolatileFields fields;
  fields.count = bump_return(4);
  fields.ratio = input + gain;
  return fields.ratio + fields.count;
}

int main(void) {
  marker = marker + 1;
  gain   = read_volatile_param(gain);
  printf("%c\n", marker);
  printf("%f\n", gain);
  printf("%f\n", use_volatile_fields(2.0));
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
// LOWERING-NEXT: struct VolatileFields {
// LOWERING-NEXT:     count: i32,
// LOWERING-NEXT:     ratio: f64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut gain: f64 = 1.5;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut marker: i8 = 65;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = {{_v[0-9]+}} as i8;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(marker), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = read_volatile_param({{_v[0-9]+}});
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(gain), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 2.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = use_volatile_fields({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn read_volatile_param({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let mut value: f64 = 0.0;
// LOWERING-NEXT:     value = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(value)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn use_volatile_fields({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let mut fields: VolatileFields = VolatileFields {
// LOWERING-NEXT:         count: 0,
// LOWERING-NEXT:         ratio: 0.0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = bump_return({{_v[0-9]+}});
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(fields.count), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(fields.ratio), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.ratio)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.count)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn bump_return({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
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
// REWRITES-NEXT: struct VolatileFields {
// REWRITES-NEXT:     count: i32,
// REWRITES-NEXT:     ratio: f64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut gain: f64 = 1.5;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut marker: i8 = 65;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(marker), (({{_v[0-9]+}} as i32) + 1) as i8) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(gain), read_volatile_param({{_v[0-9]+}})) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%c\n".as_ptr(),
// REWRITES-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) }) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(c"%f\n".as_ptr(), unsafe {
// REWRITES-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(gain))
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { printf(c"%f\n".as_ptr(), use_volatile_fields(2.0)) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn read_volatile_param({{arg[0-9]+}}: f64) -> f64 {
// REWRITES-NEXT:     let mut value: f64 = 0.0;
// REWRITES-NEXT:     value = {{arg[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(value)) };
// REWRITES-NEXT:     {{_v[0-9]+}} + 0.5
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn use_volatile_fields({{arg[0-9]+}}: f64) -> f64 {
// REWRITES-NEXT:     let mut fields: VolatileFields = VolatileFields {
// REWRITES-NEXT:         count: 0,
// REWRITES-NEXT:         ratio: 0.0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(fields.count), bump_return(4)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(fields.ratio), {{arg[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.ratio)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.count)) };
// REWRITES-NEXT:     {{_v[0-9]+}} + ({{_v[0-9]+}} as f64)
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn bump_return({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} + 1
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
