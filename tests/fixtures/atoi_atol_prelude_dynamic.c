#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char a[32];
    char b[32];
    char c[32];
    strcpy(a, "  -42abc");
    strcpy(b, "+1000000000000zzz");
    strcpy(c, "9000000000000");
    printf("%d\n", atoi(a));
    printf("%ld\n", atol(b));
    printf("%lld\n", atoll(c));
    return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn strcpy(_0: *mut i8, _1: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn atoi(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn atol(_0: *const i8) -> i64;
// LOWERING-NEXT:     fn atoll(_0: *const i8) -> i64;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// LOWERING-NEXT:     let mut b: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// LOWERING-NEXT:     let mut c: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"  -42abc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { strcpy({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"+1000000000000zzz\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { strcpy({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = c.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"9000000000000\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { strcpy({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { atoi({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%ld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { atol({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%lld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = c.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { atoll({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn strcpy(_0: *mut i8, _1: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn atoi(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn atol(_0: *const i8) -> i64;
// REWRITES-NEXT:     fn atoll(_0: *const i8) -> i64;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn __slate_atoi(s: *const i8) -> i32 {
// REWRITES-NEXT: let bytes = unsafe { std::ffi::CStr::from_ptr(s) }.to_bytes();
// REWRITES-NEXT: let n = bytes.len();
// REWRITES-NEXT: let mut i = 0usize;
// REWRITES-NEXT: while i < n && (bytes[i].is_ascii_whitespace() || bytes[i] == 11u8) {
// REWRITES-NEXT:     i += 1usize;
// REWRITES-NEXT: }
// REWRITES-NEXT: let start = i;
// REWRITES-NEXT: if i < n && (bytes[i] == 43u8 || bytes[i] == 45u8) {
// REWRITES-NEXT:     i += 1usize;
// REWRITES-NEXT: }
// REWRITES-NEXT: let digits = i;
// REWRITES-NEXT: while i < n && bytes[i].is_ascii_digit() {
// REWRITES-NEXT:     i += 1usize;
// REWRITES-NEXT: }
// REWRITES-NEXT: if i == digits {
// REWRITES-NEXT:     return 0i32;
// REWRITES-NEXT: }
// REWRITES-NEXT: return std::str::from_utf8(&bytes[(start..i)]).unwrap().parse().unwrap_or(0i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn __slate_atol(s: *const i8) -> i64 {
// REWRITES-NEXT: let bytes = unsafe { std::ffi::CStr::from_ptr(s) }.to_bytes();
// REWRITES-NEXT: let n = bytes.len();
// REWRITES-NEXT: let mut i = 0usize;
// REWRITES-NEXT: while i < n && (bytes[i].is_ascii_whitespace() || bytes[i] == 11u8) {
// REWRITES-NEXT:     i += 1usize;
// REWRITES-NEXT: }
// REWRITES-NEXT: let start = i;
// REWRITES-NEXT: if i < n && (bytes[i] == 43u8 || bytes[i] == 45u8) {
// REWRITES-NEXT:     i += 1usize;
// REWRITES-NEXT: }
// REWRITES-NEXT: let digits = i;
// REWRITES-NEXT: while i < n && bytes[i].is_ascii_digit() {
// REWRITES-NEXT:     i += 1usize;
// REWRITES-NEXT: }
// REWRITES-NEXT: if i == digits {
// REWRITES-NEXT:     return 0i64;
// REWRITES-NEXT: }
// REWRITES-NEXT: return std::str::from_utf8(&bytes[(start..i)]).unwrap().parse().unwrap_or(0i64);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut a: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// REWRITES-NEXT: let mut b: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// REWRITES-NEXT: let mut c: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"  -42abc\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { strcpy({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"+1000000000000zzz\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { strcpy({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = c.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"9000000000000\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { strcpy({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { __slate_atoi({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%ld\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { __slate_atol({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%lld\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = c.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { __slate_atol({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
