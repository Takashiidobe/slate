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
// COMMON-LOWERING-NEXT: struct VolatileFields {
// COMMON-LOWERING-NEXT:     count: i32,
// COMMON-LOWERING-NEXT:     ratio: f64,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut gain: f64 = 1.5;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(marker), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = read_volatile_param({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(gain), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = use_volatile_fields({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn read_volatile_param({{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let mut value: f64 = 0.0;
// COMMON-LOWERING-NEXT:     value = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(value)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn use_volatile_fields({{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let mut fields: VolatileFields = VolatileFields {
// COMMON-LOWERING-NEXT:         count: 0,
// COMMON-LOWERING-NEXT:         ratio: 0.0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = bump_return({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(fields.count), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{arg[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(fields.ratio), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.ratio)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.count)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn bump_return({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: static mut marker: i8 = 65;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = {{__v[0-9]+}} as i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: static mut marker: u8 = 65;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%c\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%f\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%f\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: struct VolatileFields {
// COMMON-REWRITES-NEXT:     count: i32,
// COMMON-REWRITES-NEXT:     ratio: f64,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut gain: f64 = 1.5;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(gain), read_volatile_param({{__v[0-9]+}})) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%c\n".as_ptr(),
// COMMON-REWRITES-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) }) as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(c"%f\n".as_ptr(), unsafe {
// COMMON-REWRITES-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(gain))
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%f\n".as_ptr(), use_volatile_fields(2.0)) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn read_volatile_param({{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let mut value: f64 = 0.0;
// COMMON-REWRITES-NEXT:     value = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(value)) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + 0.5
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn use_volatile_fields({{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let mut fields: VolatileFields = VolatileFields {
// COMMON-REWRITES-NEXT:         count: 0,
// COMMON-REWRITES-NEXT:         ratio: 0.0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(fields.count), bump_return(4)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(fields.ratio), {{arg[0-9]+}} + {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.ratio)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.count)) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + ({{__v[0-9]+}} as f64)
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn bump_return({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + 1
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: static mut marker: i8 = 65;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(marker), (({{__v[0-9]+}} as i32) + 1) as i8) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: static mut marker: u8 = 65;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// REWRITES-AARCH64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(marker), (({{__v[0-9]+}} as i32) + 1) as u8) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
