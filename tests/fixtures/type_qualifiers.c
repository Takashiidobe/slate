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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut atomic_counter: i32 = 4;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut global_bias: i32 = 3;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_const_param({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { global_bias };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_restrict_pointers({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_atomic_value({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut local: i32 = 0;
// LOWERING-NEXT:     value = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(atomic_counter)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     local = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(local)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut left: i32 = 0;
// LOWERING-NEXT:     let mut right: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     left = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 11;
// LOWERING-NEXT:     right = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = add_const_param({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = add_restrict_pointers(std::ptr::addr_of_mut!(left), std::ptr::addr_of_mut!(right));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = add_atomic_value({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut atomic_counter: i32 = 4;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut global_bias: i32 = 3;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_const_param({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT: return {{arg[0-9]+}} + {{_v[0-9]+}} + unsafe { global_bias };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_restrict_pointers({{arg[0-9]+}}: &i32, {{arg[0-9]+}}: &i32) -> i32 {
// REWRITES-NEXT: return (unsafe { *({{arg[0-9]+}} as *const i32) }) + unsafe { *({{arg[0-9]+}} as *const i32) };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_atomic_value({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut local: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(atomic_counter)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: local = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(local)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: return {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut left: i32 = 0;
// REWRITES-NEXT: let mut right: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: left = 7;
// REWRITES-NEXT: right = 11;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = add_const_param({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = add_restrict_pointers(unsafe { &(*std::ptr::addr_of_mut!(left)) }, unsafe { &(*std::ptr::addr_of_mut!(right)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 6;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = add_atomic_value({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
