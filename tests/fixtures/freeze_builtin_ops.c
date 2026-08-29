#include <stdio.h>

static int freeze_probe(int seed) {
  int value = seed * 2 + 1;
#if __has_builtin(__builtin_nondeterministic_value)
  int frozen = __builtin_nondeterministic_value(value);
  return frozen == frozen;
#else
  return value == value;
#endif
}

int main(void) {
  volatile int input = 20;
  printf("%d\n", freeze_probe(input));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn freeze_probe(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut seed: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut frozen: i32 = 0;
// LOWERING-NEXT:     seed = arg0;
// LOWERING-NEXT:     let _v0: i32 = seed;
// LOWERING-NEXT:     let _v1: i32 = 2;
// LOWERING-NEXT:     let _v2: i32 = _v0 * _v1;
// LOWERING-NEXT:     let _v3: i32 = 1;
// LOWERING-NEXT:     let _v4: i32 = _v2 + _v3;
// LOWERING-NEXT:     value = _v4;
// LOWERING-NEXT:     let _v5: i32 = 0;
// LOWERING-NEXT:     let _v6: i32 = _v5;
// LOWERING-NEXT:     frozen = _v6;
// LOWERING-NEXT:     let _v7: i32 = frozen;
// LOWERING-NEXT:     let _v8: i32 = frozen;
// LOWERING-NEXT:     let _v9: bool = _v7 == _v8;
// LOWERING-NEXT:     let _v10: i32 = _v9 as i32;
// LOWERING-NEXT:     __retval = _v10;
// LOWERING-NEXT:     let _v11: i32 = __retval;
// LOWERING-NEXT:     return _v11;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut input: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 20;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), _v1) };
// LOWERING-NEXT:     let _v2: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let _v4: i32 = freeze_probe(_v3);
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v2 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: i32 = 0;
// LOWERING-NEXT:     __retval = _v6;
// LOWERING-NEXT:     let _v7: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v7 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn freeze_probe(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut seed: i32 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut value: i32 = 0;
// REWRITES-NEXT: let mut frozen: i32 = 0;
// REWRITES-NEXT: let _v1: i32 = 2;
// REWRITES-NEXT: let _v3: i32 = 1;
// REWRITES-NEXT: value = seed * _v1 + _v3;
// REWRITES-NEXT: frozen = 0;
// REWRITES-NEXT: __retval = (frozen == frozen) as i32;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut input: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: i32 = 20;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), _v1) };
// REWRITES-NEXT: let _v2: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: let _v4: i32 = freeze_probe(_v3);
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v2 as *const i8, _v4) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
