#include <stddef.h>
#include <stdio.h>

static int target(void) { return 42; }

int main(void) {
  int       (*base)(void) = target;
  ptrdiff_t forward       = (base + 3) - base;
  ptrdiff_t backward      = (base - 2) - base;
  ptrdiff_t difference    = base - (base + 3);
  int       unchanged     = base + 0 == base;
  printf("%td %td %td %d\n", forward, backward, difference, unchanged);
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
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// COMMON-LOWERING-NEXT:             (Some(target).unwrap() as *const u8).add(3)
// COMMON-LOWERING-NEXT:         })
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-LOWERING-NEXT:         ({{__v[0-9]+}}.unwrap() as *const u8).offset_from(Some(target).unwrap() as *const u8) as i64
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// COMMON-LOWERING-NEXT:             (Some(target).unwrap() as *const u8).offset({{__v[0-9]+}} as isize)
// COMMON-LOWERING-NEXT:         })
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-LOWERING-NEXT:         ({{__v[0-9]+}}.unwrap() as *const u8).offset_from(Some(target).unwrap() as *const u8) as i64
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// COMMON-LOWERING-NEXT:             (Some(target).unwrap() as *const u8).add(3)
// COMMON-LOWERING-NEXT:         })
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-LOWERING-NEXT:         (Some(target).unwrap() as *const u8).offset_from({{__v[0-9]+}}.unwrap() as *const u8) as i64
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// COMMON-LOWERING-NEXT:             (Some(target).unwrap() as *const u8).add(0)
// COMMON-LOWERING-NEXT:         })
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:         == unsafe {
// COMMON-LOWERING-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn() -> i32>>(
// COMMON-LOWERING-NEXT:                 target as *const (),
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn target() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%td %td %td %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%td %td %td %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// COMMON-REWRITES-NEXT:             (Some(target).unwrap() as *const u8).add(3)
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-REWRITES-NEXT:         ({{__v[0-9]+}}.unwrap() as *const u8).offset_from(Some(target).unwrap() as *const u8) as i64
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// COMMON-REWRITES-NEXT:             (Some(target).unwrap() as *const u8).offset((-2 as i32) as isize)
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-REWRITES-NEXT:         ({{__v[0-9]+}}.unwrap() as *const u8).offset_from(Some(target).unwrap() as *const u8) as i64
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// COMMON-REWRITES-NEXT:             (Some(target).unwrap() as *const u8).add(3)
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-REWRITES-NEXT:         (Some(target).unwrap() as *const u8).offset_from({{__v[0-9]+}}.unwrap() as *const u8) as i64
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// COMMON-REWRITES-NEXT:             (Some(target).unwrap() as *const u8).add(0)
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         == unsafe {
// COMMON-REWRITES-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn() -> i32>>(
// COMMON-REWRITES-NEXT:                 target as *const (),
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%td %td %td %d\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}} as i32) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn target() -> i32 {
// COMMON-REWRITES-NEXT:     42
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
