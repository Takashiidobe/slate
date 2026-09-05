
// REWRITES-DAG: #[linkage = "extern_weak"]
// REWRITES-DAG: static abs: Option<unsafe extern "C-unwind" fn(i32) -> i32>;
// REWRITES-NOT: fn weakref_alias

#include <stdio.h>
#include <stdlib.h>

int weakref_target(int value) { return value + 7; }

static int weakref_alias(int) __attribute__((weakref("weakref_target")));

static int weakref_external(int) __attribute__((weakref("abs")));

int main(void) {
  printf("%d %d %d\n", weakref_alias(35), weakref_target(41),
         weakref_external(-53));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(linkage)]
// LOWERING-NEXT: #![feature(c_variadic)]
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
// LOWERING-NEXT:     #[linkage = "extern_weak"]
// LOWERING-NEXT:     static abs: Option<unsafe extern "C-unwind" fn(i32) -> i32>;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn weakref_target({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 35;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = weakref_target({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = weakref_target({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -53;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { abs.unwrap()({{__v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(linkage)]
// REWRITES-NEXT: #![feature(c_variadic)]
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
// REWRITES-NEXT:     #[linkage = "extern_weak"]
// REWRITES-NEXT:     static abs: Option<unsafe extern "C-unwind" fn(i32) -> i32>;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn weakref_target({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} + 7
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// REWRITES-NEXT:             weakref_target(35 as i32),
// REWRITES-NEXT:             weakref_target(41),
// REWRITES-NEXT:             unsafe { abs.unwrap()(-53 as i32) },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
