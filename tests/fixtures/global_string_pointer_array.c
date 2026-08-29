#include <stdio.h>

static char *names[] = {"alpha", "beta"};

int main(void) { printf("%s %s\n", names[0], names[1]); }
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut names: aligned::Aligned<aligned::A16, [*mut i8; 2]> = aligned::Aligned([b"alpha\0".as_ptr() as *mut i8, b"beta\0".as_ptr() as *mut i8]);
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%s %s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i64 = 0;
// LOWERING-NEXT:     let _v3: *mut i8 = unsafe { (*names)[(_v2 as usize)] };
// LOWERING-NEXT:     let _v4: i64 = 1;
// LOWERING-NEXT:     let _v5: *mut i8 = unsafe { (*names)[(_v4 as usize)] };
// LOWERING-NEXT:     let _v6: i32 = unsafe { printf(_v1 as *const i8, _v3, _v5) };
// LOWERING-NEXT:     let _v7: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v7 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut names: aligned::Aligned<aligned::A16, [*mut i8; 2]> = aligned::Aligned([b"alpha\0".as_ptr() as *mut i8, b"beta\0".as_ptr() as *mut i8]);
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%s %s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i64 = 0;
// REWRITES-NEXT: let _v4: i64 = 1;
// REWRITES-NEXT: let _v6: i32 = unsafe { printf(_v1 as *const i8, unsafe { (*names)[(_v2 as usize)] }, unsafe { (*names)[(_v4 as usize)] }) };
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
