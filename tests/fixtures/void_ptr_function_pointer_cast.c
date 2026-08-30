#include <stdio.h>

typedef int (*Callback)(int);

static int add_one(int x) { return x + 1; }

union FnBox {
  Callback fn;
  void    *ptr;
};

int main(void) {
  union FnBox box;
  box.fn        = add_one;
  void    *slot = box.ptr;
  Callback cb   = (Callback)slot;
  Callback none = (Callback)0;
  printf("%d %d\n", cb(41), none != NULL);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union FnBox {
// LOWERING-NEXT:     r#fn: Option<unsafe extern "C" fn(i32) -> i32>,
// LOWERING-NEXT:     ptr: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn add_one({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     x = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut r#box: FnBox = unsafe { std::mem::zeroed::<FnBox>() };
// LOWERING-NEXT:     let mut slot: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut cb: Option<unsafe extern "C" fn(i32) -> i32> = None;
// LOWERING-NEXT:     let mut none: Option<unsafe extern "C" fn(i32) -> i32> = None;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         r#box.r#fn = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(add_one as *const ()) };
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { r#box.ptr };
// LOWERING-NEXT:     slot = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = slot;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { std::mem::transmute::<usize, Option<unsafe extern "C" fn(i32) -> i32>>({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     cb = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = None;
// LOWERING-NEXT:     none = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = cb;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = none;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = None;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union FnBox {
// REWRITES-NEXT:     r#fn: Option<unsafe extern "C" fn(i32) -> i32>,
// REWRITES-NEXT:     ptr: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn add_one({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: __retval = x + {{_v[0-9]+}};
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut r#box: FnBox = unsafe { std::mem::zeroed::<FnBox>() };
// REWRITES-NEXT: let mut slot: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut cb: Option<unsafe extern "C" fn(i32) -> i32> = None;
// REWRITES-NEXT: let mut none: Option<unsafe extern "C" fn(i32) -> i32> = None;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         r#box.r#fn = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(add_one as *const ()) };
// REWRITES-NEXT: }
// REWRITES-NEXT: slot = unsafe { r#box.ptr };
// REWRITES-NEXT: let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { std::mem::transmute::<usize, Option<unsafe extern "C" fn(i32) -> i32>>(slot as usize) };
// REWRITES-NEXT: cb = {{_v[0-9]+}};
// REWRITES-NEXT: none = None;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 41;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { cb.unwrap()({{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = None;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, (none != {{_v[0-9]+}}) as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
