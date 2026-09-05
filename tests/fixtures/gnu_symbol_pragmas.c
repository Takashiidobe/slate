
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
// LOWERING-X86_64-GNU-NEXT: #[cfg(target_arch = "x86_64")]
// LOWERING-AARCH64-GNU-NEXT: #[cfg(target_arch = "aarch64")]
// LOWERING-NEXT: core::arch::global_asm!(
// LOWERING-NEXT:     ".weak pragma_weak_alias\n.set pragma_weak_alias, pragma_weak_target",
// LOWERING-X86_64-GNU-NEXT:     options(att_syntax, raw)
// LOWERING-AARCH64-GNU-NEXT:     options(raw)
// LOWERING-NEXT: );
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn pragma_weak_alias(_0: i32) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[unsafe(no_mangle)]
// LOWERING-NEXT: pub extern "C-unwind" fn pragma_weak_target({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pragma_actual({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 29;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { pragma_weak_alias({{__v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = (unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// LOWERING-NEXT:             pragma_weak_alias as *const (),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }
// LOWERING-NEXT:     .unwrap() as *const u8)
// LOWERING-NEXT:         == (Some(pragma_weak_target).unwrap() as *const u8);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 13;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = pragma_actual({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
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
// REWRITES-X86_64-GNU-NEXT: #[cfg(target_arch = "x86_64")]
// REWRITES-AARCH64-GNU-NEXT: #[cfg(target_arch = "aarch64")]
// REWRITES-NEXT: core::arch::global_asm!(
// REWRITES-NEXT:     ".weak pragma_weak_alias\n.set pragma_weak_alias, pragma_weak_target",
// REWRITES-X86_64-GNU-NEXT:     options(att_syntax, raw)
// REWRITES-AARCH64-GNU-NEXT:     options(raw)
// REWRITES-NEXT: );
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn pragma_weak_alias(_0: i32) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[unsafe(no_mangle)]
// REWRITES-NEXT: pub extern "C-unwind" fn pragma_weak_target({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} + 7
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn pragma_actual({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} * 3
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d\n".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d\n".as_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { pragma_weak_alias(29 as i32) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = (unsafe {
// REWRITES-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// REWRITES-NEXT:             pragma_weak_alias as *const (),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }
// REWRITES-NEXT:     .unwrap() as *const u8)
// REWRITES-NEXT:         == (Some(pragma_weak_target).unwrap() as *const u8);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}} as i32,
// REWRITES-NEXT:             pragma_actual(13),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
