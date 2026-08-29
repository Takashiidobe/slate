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
// LOWERING-NEXT: fn add_const_param(arg3: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut local_bias: i32 = 0;
// LOWERING-NEXT:     value = arg3;
// LOWERING-NEXT:     let _v0: i32 = 5;
// LOWERING-NEXT:     local_bias = _v0;
// LOWERING-NEXT:     let _v1: i32 = value;
// LOWERING-NEXT:     let _v2: i32 = local_bias;
// LOWERING-NEXT:     let _v3: i32 = _v1 + _v2;
// LOWERING-NEXT:     let _v4: i32 = unsafe { global_bias };
// LOWERING-NEXT:     let _v5: i32 = _v3 + _v4;
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: i32 = __retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_restrict_pointers(arg1: *mut i32, arg2: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut lhs: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut rhs: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     lhs = arg1;
// LOWERING-NEXT:     rhs = arg2;
// LOWERING-NEXT:     let _v0: *mut i32 = lhs;
// LOWERING-NEXT:     let _v1: i32 = unsafe { *_v0 };
// LOWERING-NEXT:     let _v2: *mut i32 = rhs;
// LOWERING-NEXT:     let _v3: i32 = unsafe { *_v2 };
// LOWERING-NEXT:     let _v4: i32 = _v1 + _v3;
// LOWERING-NEXT:     __retval = _v4;
// LOWERING-NEXT:     let _v5: i32 = __retval;
// LOWERING-NEXT:     return _v5;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_atomic_value(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut local: i32 = 0;
// LOWERING-NEXT:     value = arg0;
// LOWERING-NEXT:     let _v0: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v1: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(atomic_counter)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     local = _v2;
// LOWERING-NEXT:     let _v3: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(local)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     __retval = _v3;
// LOWERING-NEXT:     let _v4: i32 = __retval;
// LOWERING-NEXT:     return _v4;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut left: i32 = 0;
// LOWERING-NEXT:     let mut right: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 7;
// LOWERING-NEXT:     left = _v1;
// LOWERING-NEXT:     let _v2: i32 = 11;
// LOWERING-NEXT:     right = _v2;
// LOWERING-NEXT:     let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: i32 = 2;
// LOWERING-NEXT:     let _v5: i32 = add_const_param(_v4);
// LOWERING-NEXT:     let _v6: i32 = unsafe { printf(_v3 as *const i8, _v5) };
// LOWERING-NEXT:     let _v7: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v8: i32 = add_restrict_pointers(std::ptr::addr_of_mut!(left), std::ptr::addr_of_mut!(right));
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v7 as *const i8, _v8) };
// LOWERING-NEXT:     let _v10: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: i32 = 6;
// LOWERING-NEXT:     let _v12: i32 = add_atomic_value(_v11);
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v10 as *const i8, _v12) };
// LOWERING-NEXT:     let _v14: i32 = 0;
// LOWERING-NEXT:     __retval = _v14;
// LOWERING-NEXT:     let _v15: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v15 as i32);
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
// REWRITES-NEXT: fn add_const_param(arg3: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = arg3;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut local_bias: i32 = 0;
// REWRITES-NEXT: local_bias = 5;
// REWRITES-NEXT: __retval = value + local_bias + unsafe { global_bias };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_restrict_pointers(arg1: &i32, arg2: &i32) -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = (unsafe { *((arg1 as *const i32) as *mut i32) }) + unsafe { *((arg2 as *const i32) as *mut i32) };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_atomic_value(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut local: i32 = 0;
// REWRITES-NEXT: let _v0: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v1: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(atomic_counter)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: local = _v0 + _v1;
// REWRITES-NEXT: let _v3: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(local)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: __retval = _v3;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut left: i32 = 0;
// REWRITES-NEXT: let mut right: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: left = 7;
// REWRITES-NEXT: right = 11;
// REWRITES-NEXT: let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: i32 = 2;
// REWRITES-NEXT: let _v5: i32 = add_const_param(_v4);
// REWRITES-NEXT: let _v6: i32 = unsafe { printf(_v3 as *const i8, _v5) };
// REWRITES-NEXT: let _v7: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i32 = add_restrict_pointers(unsafe { &(*std::ptr::addr_of_mut!(left)) }, unsafe { &(*std::ptr::addr_of_mut!(right)) });
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v7 as *const i8, _v8) };
// REWRITES-NEXT: let _v10: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v11: i32 = 6;
// REWRITES-NEXT: let _v12: i32 = add_atomic_value(_v11);
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v10 as *const i8, _v12) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
