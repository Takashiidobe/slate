#include <stdio.h>

static volatile char   marker = 65;
static volatile double gain   = 1.5;

struct VolatileFields {
  volatile int    count;
  volatile double ratio;
};

static volatile int bump_return(int value) { return value + 1; }

static double read_volatile_param(volatile double value) { return value + 0.5; }

static double use_volatile_fields(double input) {
  struct VolatileFields fields;
  fields.count = bump_return(4);
  fields.ratio = input + gain;
  return fields.ratio + fields.count;
}

int main(void) {
  marker = marker + 1;
  gain   = read_volatile_param(gain);
  printf("%c\n", marker);
  printf("%f\n", gain);
  printf("%f\n", use_volatile_fields(2.0));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct VolatileFields {
// LOWERING-NEXT:     count: i32,
// LOWERING-NEXT:     ratio: f64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut gain: f64 = 1.5;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut marker: i8 = 65;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn read_volatile_param(arg2: f64) -> f64 {
// LOWERING-NEXT:     let mut value: f64 = 0.0;
// LOWERING-NEXT:     let mut __retval: f64 = 0.0;
// LOWERING-NEXT:     value = arg2;
// LOWERING-NEXT:     let _v0: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(value)) };
// LOWERING-NEXT:     let _v1: f64 = 0.5;
// LOWERING-NEXT:     let _v2: f64 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: f64 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn bump_return(arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     value = arg1;
// LOWERING-NEXT:     let _v0: i32 = value;
// LOWERING-NEXT:     let _v1: i32 = 1;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn use_volatile_fields(arg0: f64) -> f64 {
// LOWERING-NEXT:     let mut input: f64 = 0.0;
// LOWERING-NEXT:     let mut __retval: f64 = 0.0;
// LOWERING-NEXT:     let mut fields: VolatileFields = VolatileFields { count: 0, ratio: 0.0 };
// LOWERING-NEXT:     input = arg0;
// LOWERING-NEXT:     let _v0: i32 = 4;
// LOWERING-NEXT:     let _v1: i32 = bump_return(_v0);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(fields.count), _v1) };
// LOWERING-NEXT:     let _v2: f64 = input;
// LOWERING-NEXT:     let _v3: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// LOWERING-NEXT:     let _v4: f64 = _v2 + _v3;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(fields.ratio), _v4) };
// LOWERING-NEXT:     let _v5: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.ratio)) };
// LOWERING-NEXT:     let _v6: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.count)) };
// LOWERING-NEXT:     let _v7: f64 = _v6 as f64;
// LOWERING-NEXT:     let _v8: f64 = _v5 + _v7;
// LOWERING-NEXT:     __retval = _v8;
// LOWERING-NEXT:     let _v9: f64 = __retval;
// LOWERING-NEXT:     return _v9;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// LOWERING-NEXT:     let _v2: i32 = _v1 as i32;
// LOWERING-NEXT:     let _v3: i32 = 1;
// LOWERING-NEXT:     let _v4: i32 = _v2 + _v3;
// LOWERING-NEXT:     let _v5: i8 = _v4 as i8;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(marker), _v5) };
// LOWERING-NEXT:     let _v6: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// LOWERING-NEXT:     let _v7: f64 = read_volatile_param(_v6);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(gain), _v7) };
// LOWERING-NEXT:     let _v8: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v9: i8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// LOWERING-NEXT:     let _v10: i32 = _v9 as i32;
// LOWERING-NEXT:     let _v11: i32 = unsafe { printf(_v8 as *const i8, _v10) };
// LOWERING-NEXT:     let _v12: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v13: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// LOWERING-NEXT:     let _v14: i32 = unsafe { printf(_v12 as *const i8, _v13) };
// LOWERING-NEXT:     let _v15: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: f64 = 2.0;
// LOWERING-NEXT:     let _v17: f64 = use_volatile_fields(_v16);
// LOWERING-NEXT:     let _v18: i32 = unsafe { printf(_v15 as *const i8, _v17) };
// LOWERING-NEXT:     let _v19: i32 = 0;
// LOWERING-NEXT:     __retval = _v19;
// LOWERING-NEXT:     let _v20: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v20 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct VolatileFields {
// REWRITES-NEXT:     count: i32,
// REWRITES-NEXT:     ratio: f64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut gain: f64 = 1.5;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut marker: i8 = 65;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn read_volatile_param(arg2: f64) -> f64 {
// REWRITES-NEXT: let mut value: f64 = 0.0;
// REWRITES-NEXT: let mut __retval: f64 = 0.0;
// REWRITES-NEXT: value = arg2;
// REWRITES-NEXT: let _v0: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(value)) };
// REWRITES-NEXT: let _v1: f64 = 0.5;
// REWRITES-NEXT: __retval = _v0 + _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn bump_return(arg1: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v1: i32 = 1;
// REWRITES-NEXT: __retval = value + _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn use_volatile_fields(arg0: f64) -> f64 {
// REWRITES-NEXT: let mut input: f64 = 0.0;
// REWRITES-NEXT: let mut __retval: f64 = 0.0;
// REWRITES-NEXT: let mut fields: VolatileFields = VolatileFields { count: 0, ratio: 0.0 };
// REWRITES-NEXT: input = arg0;
// REWRITES-NEXT: let _v0: i32 = 4;
// REWRITES-NEXT: let _v1: i32 = bump_return(_v0);
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(fields.count), _v1) };
// REWRITES-NEXT: let _v2: f64 = input;
// REWRITES-NEXT: let _v3: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(fields.ratio), _v2 + _v3) };
// REWRITES-NEXT: let _v5: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.ratio)) };
// REWRITES-NEXT: let _v6: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.count)) };
// REWRITES-NEXT: __retval = _v5 + (_v6 as f64);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: i8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// REWRITES-NEXT: let _v3: i32 = 1;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(marker), ((_v1 as i32) + _v3) as i8) };
// REWRITES-NEXT: let _v6: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// REWRITES-NEXT: let _v7: f64 = read_volatile_param(_v6);
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(gain), _v7) };
// REWRITES-NEXT: let _v8: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v9: i8 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) };
// REWRITES-NEXT: let _v11: i32 = unsafe { printf(_v8 as *const i8, _v9 as i32) };
// REWRITES-NEXT: let _v12: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v13: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) };
// REWRITES-NEXT: let _v14: i32 = unsafe { printf(_v12 as *const i8, _v13) };
// REWRITES-NEXT: let _v15: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: f64 = 2.0;
// REWRITES-NEXT: let _v17: f64 = use_volatile_fields(_v16);
// REWRITES-NEXT: let _v18: i32 = unsafe { printf(_v15 as *const i8, _v17) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
