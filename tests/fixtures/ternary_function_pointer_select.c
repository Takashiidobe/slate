#include <stdio.h>

typedef int (*BinaryOp)(int, int);

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }

static int apply(int useAdd, int a, int b) {
  return (useAdd ? add : sub)(a, b);
}

int main(void) {
  int      useAdd = 1;
  BinaryOp op     = useAdd ? add : sub;
  printf("%d %d %d %d\n", op(10, 3), apply(0, 10, 3), apply(1, 4, 4),
         (useAdd ? sub : add)(9, 2));
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
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> =
// LOWERING-NEXT:         if {{__v[0-9]+}} { Some(add) } else { Some(sub) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = apply({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = apply({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> =
// LOWERING-NEXT:         if {{__v[0-9]+}} { Some(sub) } else { Some(add) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 9;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn sub({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} - {{arg[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn apply({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> =
// LOWERING-NEXT:         if {{__v[0-9]+}} { Some(add) } else { Some(sub) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{arg[0-9]+}}, {{arg[0-9]+}}) };
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
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> =
// REWRITES-NEXT:         if {{__v[0-9]+}} { Some(add) } else { Some(sub) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d %d\n".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d %d\n".as_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()(10 as i32, 3 as i32) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = apply(0, 10, 3);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = apply(1, 4, 4);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> =
// REWRITES-NEXT:         if {{__v[0-9]+}} { Some(sub) } else { Some(add) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             unsafe { {{__v[0-9]+}}.unwrap()(9 as i32, 2 as i32) },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn sub({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} - {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn apply({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> =
// REWRITES-NEXT:         if {{__v[0-9]+}} { Some(add) } else { Some(sub) };
// REWRITES-NEXT:     unsafe { {{__v[0-9]+}}.unwrap()({{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
