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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Callback {
// LOWERING-NEXT:     handler: Option<unsafe extern "C" fn(*mut i32) -> *mut i32>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn add_one({{arg[0-9]+}}: *mut i32) -> *mut i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{arg[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return {{arg[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut callback: Callback = Callback { handler: None };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:     value = {{_v[0-9]+}};
// LOWERING-NEXT:     callback = Callback { handler: None };
// LOWERING-NEXT:     callback.handler = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(*mut i32) -> *mut i32>>(add_one as *const ()) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(*mut i32) -> *mut i32> = callback.handler;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.unwrap()(std::ptr::addr_of_mut!(value)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Callback {
// REWRITES-NEXT:     handler: Option<unsafe extern "C" fn(*mut i32) -> *mut i32>,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn add_one({{arg[0-9]+}}: *mut i32) -> *mut i32 {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *{{arg[0-9]+}} = (unsafe { *{{arg[0-9]+}} }) + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: return {{arg[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut value: i32 = 0;
// REWRITES-NEXT: let mut callback: Callback = Callback { handler: None };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: value = 41;
// REWRITES-NEXT: callback = Callback { handler: None };
// REWRITES-NEXT: callback.handler = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(*mut i32) -> *mut i32>>(add_one as *const ()) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = unsafe { callback.handler.unwrap()(std::ptr::addr_of_mut!(value)) };
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, unsafe { *{{_v[0-9]+}} }) };
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
