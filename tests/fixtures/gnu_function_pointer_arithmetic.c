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
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// LOWERING-NEXT:             (Some(target).unwrap() as *const u8).add(3)
// LOWERING-NEXT:         })
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe {
// LOWERING-NEXT:         ({{_v[0-9]+}}.unwrap() as *const u8).offset_from(Some(target).unwrap() as *const u8) as i64
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// LOWERING-NEXT:             (Some(target).unwrap() as *const u8).offset({{_v[0-9]+}} as isize)
// LOWERING-NEXT:         })
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe {
// LOWERING-NEXT:         ({{_v[0-9]+}}.unwrap() as *const u8).offset_from(Some(target).unwrap() as *const u8) as i64
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// LOWERING-NEXT:             (Some(target).unwrap() as *const u8).add(3)
// LOWERING-NEXT:         })
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe {
// LOWERING-NEXT:         (Some(target).unwrap() as *const u8).offset_from({{_v[0-9]+}}.unwrap() as *const u8) as i64
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// LOWERING-NEXT:             (Some(target).unwrap() as *const u8).add(0)
// LOWERING-NEXT:         })
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}
// LOWERING-NEXT:         == unsafe {
// LOWERING-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn() -> i32>>(
// LOWERING-NEXT:                 target as *const (),
// LOWERING-NEXT:             )
// LOWERING-NEXT:         };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%td %td %td %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn target() -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 42;
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
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// REWRITES-NEXT:             (Some(target).unwrap() as *const u8).add(3)
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe {
// REWRITES-NEXT:         ({{_v[0-9]+}}.unwrap() as *const u8).offset_from(Some(target).unwrap() as *const u8) as i64
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// REWRITES-NEXT:             (Some(target).unwrap() as *const u8).offset((-2 as i32) as isize)
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe {
// REWRITES-NEXT:         ({{_v[0-9]+}}.unwrap() as *const u8).offset_from(Some(target).unwrap() as *const u8) as i64
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// REWRITES-NEXT:             (Some(target).unwrap() as *const u8).add(3)
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe {
// REWRITES-NEXT:         (Some(target).unwrap() as *const u8).offset_from({{_v[0-9]+}}.unwrap() as *const u8) as i64
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(unsafe {
// REWRITES-NEXT:             (Some(target).unwrap() as *const u8).add(0)
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}
// REWRITES-NEXT:         == unsafe {
// REWRITES-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn() -> i32>>(
// REWRITES-NEXT:                 target as *const (),
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:     unsafe { printf(c"%td %td %td %d\n".as_ptr(), {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn target() -> i32 {
// REWRITES-NEXT:     42
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
