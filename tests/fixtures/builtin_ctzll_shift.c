#include <stdio.h>
#include <stddef.h>

size_t lowest_bit(unsigned long long x) {
    return (size_t)1 << (size_t)__builtin_ctzll(x);
}

int main(void) {
    unsigned long long x = 0;
    scanf("%llu", &x);
    size_t n = lowest_bit(x);
    printf("%zu\n", n);
    return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn scanf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn lowest_bit(arg0: u64) -> u64 {
// LOWERING-NEXT:     let mut x: u64 = 0;
// LOWERING-NEXT:     let mut __retval: u64 = 0;
// LOWERING-NEXT:     x = arg0;
// LOWERING-NEXT:     let _v0: u64 = 1;
// LOWERING-NEXT:     let _v1: u64 = x;
// LOWERING-NEXT:     let _v2: u64 = _v1.trailing_zeros() as u64;
// LOWERING-NEXT:     let _v3: i32 = _v2 as i32;
// LOWERING-NEXT:     let _v4: u64 = _v3 as u64;
// LOWERING-NEXT:     let _v5: u64 = _v0 << _v4;
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: u64 = __retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: u64 = 0;
// LOWERING-NEXT:     let mut n: u64 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u64 = 0;
// LOWERING-NEXT:     x = _v1;
// LOWERING-NEXT:     let _v2: *mut i8 = b"%llu\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: i32 = unsafe { scanf(_v2 as *const i8, std::ptr::addr_of_mut!(x)) };
// LOWERING-NEXT:     let _v4: u64 = x;
// LOWERING-NEXT:     let _v5: u64 = lowest_bit(_v4);
// LOWERING-NEXT:     n = _v5;
// LOWERING-NEXT:     let _v6: *mut i8 = b"%zu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: u64 = n;
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v6 as *const i8, _v7) };
// LOWERING-NEXT:     let _v9: i32 = 0;
// LOWERING-NEXT:     __retval = _v9;
// LOWERING-NEXT:     let _v10: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v10 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn scanf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn lowest_bit(arg0: u64) -> u64 {
// REWRITES-NEXT: let mut x: u64 = arg0;
// REWRITES-NEXT: let mut __retval: u64 = 0;
// REWRITES-NEXT: let _v0: u64 = 1;
// REWRITES-NEXT: let _v1: u64 = x;
// REWRITES-NEXT: __retval = _v0 << (((_v1.trailing_zeros() as u64) as i32) as u64);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: u64 = 0;
// REWRITES-NEXT: let mut n: u64 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: x = 0;
// REWRITES-NEXT: let _v2: *mut i8 = b"%llu\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: i32 = unsafe { scanf(_v2 as *const i8, std::ptr::addr_of_mut!(x)) };
// REWRITES-NEXT: n = lowest_bit(x);
// REWRITES-NEXT: let _v6: *mut i8 = b"%zu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v6 as *const i8, n) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
