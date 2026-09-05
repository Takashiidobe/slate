#include <stdio.h>

static const int   global_bias    = 3;
static _Atomic int atomic_counter = 4;

static int add_const_param(const int value) {
  const int local_bias = 5;
  return value + local_bias + global_bias;
}

static int add_restrict_pointers(int *restrict lhs, int *restrict rhs) {
  return *lhs + *rhs;
}

static int add_atomic_value(_Atomic int value) {
  _Atomic int local = value + atomic_counter;
  return local;
}

int main(void) {
  int left  = 7;
  int right = 11;
  printf("%d\n", add_const_param(2));
  printf("%d\n", add_restrict_pointers(&left, &right));
  printf("%d\n", add_atomic_value(6));
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
// LOWERING-NEXT: static mut atomic_counter: i32 = 4;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut global_bias: i32 = 3;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut left: i32 = 0;
// LOWERING-NEXT:     let mut right: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     left = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 11;
// LOWERING-NEXT:     right = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = add_const_param({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 =
// LOWERING-NEXT:         add_restrict_pointers(std::ptr::addr_of_mut!(left), std::ptr::addr_of_mut!(right));
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = add_atomic_value({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_const_param({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { global_bias };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_restrict_pointers({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_atomic_value({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut local: i32 = 0;
// LOWERING-NEXT:     value = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(atomic_counter))
// LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     local = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(local))
// LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
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
// REWRITES-NEXT: static mut atomic_counter: i32 = 4;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut global_bias: i32 = 3;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut left: i32 = 7;
// REWRITES-NEXT:     let mut right: i32 = 11;
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), add_const_param(2)) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d\n".as_ptr(),
// REWRITES-NEXT:             add_restrict_pointers(unsafe { &(*std::ptr::addr_of_mut!(left)) }, unsafe {
// REWRITES-NEXT:                 &(*std::ptr::addr_of_mut!(right))
// REWRITES-NEXT:             }),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), add_atomic_value(6)) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_const_param({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} + 5 + unsafe { global_bias }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_restrict_pointers({{arg[0-9]+}}: &i32, {{arg[0-9]+}}: &i32) -> i32 {
// REWRITES-NEXT:     (unsafe { *({{arg[0-9]+}} as *const i32) }) + unsafe { *({{arg[0-9]+}} as *const i32) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_atomic_value(mut value: i32) -> i32 {
// REWRITES-NEXT:     let mut local: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(atomic_counter))
// REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     local = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(local))
// REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
