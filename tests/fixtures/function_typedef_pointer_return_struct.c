#include <stdio.h>

typedef int *pointer_returning_fn(int *);

struct Callback {
  pointer_returning_fn *handler;
};

static int *add_one(int *value) {
  *value += 1;
  return value;
}

int main(void) {
  int             value    = 41;
  struct Callback callback = {0};
  callback.handler         = add_one;
  printf("%d\n", *callback.handler(&value));
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
// LOWERING-NEXT: struct Callback {
// LOWERING-NEXT:     handler: Option<unsafe extern "C-unwind" fn(*mut i32) -> *mut i32>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut callback: Callback = Callback { handler: None };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:     value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Callback = Callback { handler: None };
// LOWERING-NEXT:     callback = {{_v[0-9]+}};
// LOWERING-NEXT:     callback.handler = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(*mut i32) -> *mut i32>>(
// LOWERING-NEXT:             add_one as *const (),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut i32) -> *mut i32> = callback.handler;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.unwrap()(std::ptr::addr_of_mut!(value)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn add_one({{arg[0-9]+}}: *mut i32) -> *mut i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{arg[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return {{arg[0-9]+}};
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
// REWRITES-NEXT: struct Callback {
// REWRITES-NEXT:     handler: Option<unsafe extern "C-unwind" fn(*mut i32) -> *mut i32>,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut value: i32 = 0;
// REWRITES-NEXT:     let mut callback: Callback = Callback { handler: None };
// REWRITES-NEXT:     value = 41;
// REWRITES-NEXT:     callback = Callback { handler: None };
// REWRITES-NEXT:     callback.handler = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(*mut i32) -> *mut i32>>(
// REWRITES-NEXT:             add_one as *const (),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { callback.handler.unwrap()(std::ptr::addr_of_mut!(value)) };
// REWRITES-NEXT:     unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, unsafe { *{{_v[0-9]+}} }) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn add_one({{arg[0-9]+}}: *mut i32) -> *mut i32 {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{arg[0-9]+}} = (unsafe { *{{arg[0-9]+}} }) + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     {{arg[0-9]+}}
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
