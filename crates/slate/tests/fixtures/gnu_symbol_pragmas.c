
// LOWERING-DAG: .weak pragma_weak_alias\n.set pragma_weak_alias, pragma_weak_target
// LOWERING-DAG: fn pragma_weak_alias(_0: i32) -> i32;
// LOWERING-DAG: fn pragma_actual(arg0: i32) -> i32 {
// LOWERING-NOT: pragma_renamed

#include <stdio.h>

int pragma_weak_target(int value) { return value + 7; }

#pragma weak pragma_weak_alias = pragma_weak_target
extern int   pragma_weak_alias(int);

#pragma redefine_extname pragma_renamed pragma_actual
int pragma_renamed(int value) { return value * 3; }

int main(void) {
  printf("%d %d %d\n", pragma_weak_alias(29),
         pragma_weak_alias == pragma_weak_target, pragma_renamed(13));
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
// COMMON-LOWERING-NEXT: core::arch::global_asm!(
// COMMON-LOWERING-NEXT:     ".weak pragma_weak_alias\n.set pragma_weak_alias, pragma_weak_target",
// COMMON-LOWERING-NEXT: );
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn pragma_weak_alias(_0: i32) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[unsafe(no_mangle)]
// COMMON-LOWERING-NEXT: pub extern "C-unwind" fn pragma_weak_target({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn pragma_actual({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 29;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { pragma_weak_alias({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = (unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-LOWERING-NEXT:             pragma_weak_alias as *const (),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     .unwrap() as *const u8)
// COMMON-LOWERING-NEXT:         == (Some(pragma_weak_target).unwrap() as *const u8);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 13;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = pragma_actual({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: #[cfg(target_arch = "x86_64")]
// LOWERING-X86_64-GNU-NEXT:     options(att_syntax, raw)
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: #[cfg(target_arch = "aarch64")]
// LOWERING-AARCH64-GNU-NEXT:     options(raw)
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: core::arch::global_asm!(
// COMMON-REWRITES-NEXT:     ".weak pragma_weak_alias\n.set pragma_weak_alias, pragma_weak_target",
// COMMON-REWRITES-NEXT: );
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn pragma_weak_alias(_0: i32) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[unsafe(no_mangle)]
// COMMON-REWRITES-NEXT: pub extern "C-unwind" fn pragma_weak_target({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + 7
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn pragma_actual({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} * 3
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { pragma_weak_alias(29 as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = (unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-REWRITES-NEXT:             pragma_weak_alias as *const (),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     .unwrap() as *const u8)
// COMMON-REWRITES-NEXT:         == (Some(pragma_weak_target).unwrap() as *const u8);
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-REWRITES-NEXT:             pragma_actual(13),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: #[cfg(target_arch = "x86_64")]
// REWRITES-X86_64-GNU-NEXT:     options(att_syntax, raw)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: #[cfg(target_arch = "aarch64")]
// REWRITES-AARCH64-GNU-NEXT:     options(raw)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
