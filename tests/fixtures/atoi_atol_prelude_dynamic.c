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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn strcpy(_0: *mut core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn atoi(_0: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT:     fn atol(_0: *const core::ffi::c_char) -> i64;
// COMMON-LOWERING-NEXT:     fn atoll(_0: *const core::ffi::c_char) -> i64;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         strcpy(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:         strcpy(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:         strcpy(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { atoi({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { atol({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { atoll({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// LOWERING-X86_64-GNU-NEXT:     let mut b: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// LOWERING-X86_64-GNU-NEXT:     let mut c: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"  -42abc\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"+1000000000000zzz\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"9000000000000\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%ld\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%lld\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c.as_mut_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut a: [u8; 32] = [0; 32];
// LOWERING-AARCH64-GNU-NEXT:     let mut b: [u8; 32] = [0; 32];
// LOWERING-AARCH64-GNU-NEXT:     let mut c: [u8; 32] = [0; 32];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"  -42abc\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"+1000000000000zzz\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"9000000000000\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%ld\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%lld\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c.as_mut_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn strcpy(_0: *mut core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn atoi(_0: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT:     fn atol(_0: *const core::ffi::c_char) -> i64;
// COMMON-REWRITES-NEXT:     fn atoll(_0: *const core::ffi::c_char) -> i64;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let bytes = unsafe { std::ffi::CStr::from_ptr(s) }.to_bytes();
// COMMON-REWRITES-NEXT:     let n = bytes.len();
// COMMON-REWRITES-NEXT:     let mut i = 0usize;
// COMMON-REWRITES-NEXT:     while i < n && (bytes[i].is_ascii_whitespace() || bytes[i] == 11u8) {
// COMMON-REWRITES-NEXT:         i += 1usize;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let start = i;
// COMMON-REWRITES-NEXT:     if i < n && (bytes[i] == 43u8 || bytes[i] == 45u8) {
// COMMON-REWRITES-NEXT:         i += 1usize;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let digits = i;
// COMMON-REWRITES-NEXT:     while i < n && bytes[i].is_ascii_digit() {
// COMMON-REWRITES-NEXT:         i += 1usize;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     if i == digits {
// COMMON-REWRITES-NEXT:         return 0i32;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return std::str::from_utf8(&bytes[(start..i)])
// COMMON-REWRITES-NEXT:         .unwrap()
// COMMON-REWRITES-NEXT:         .parse()
// COMMON-REWRITES-NEXT:         .unwrap_or(0i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let bytes = unsafe { std::ffi::CStr::from_ptr(s) }.to_bytes();
// COMMON-REWRITES-NEXT:     let n = bytes.len();
// COMMON-REWRITES-NEXT:     let mut i = 0usize;
// COMMON-REWRITES-NEXT:     while i < n && (bytes[i].is_ascii_whitespace() || bytes[i] == 11u8) {
// COMMON-REWRITES-NEXT:         i += 1usize;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let start = i;
// COMMON-REWRITES-NEXT:     if i < n && (bytes[i] == 43u8 || bytes[i] == 45u8) {
// COMMON-REWRITES-NEXT:         i += 1usize;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let digits = i;
// COMMON-REWRITES-NEXT:     while i < n && bytes[i].is_ascii_digit() {
// COMMON-REWRITES-NEXT:         i += 1usize;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     if i == digits {
// COMMON-REWRITES-NEXT:         return 0i64;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return std::str::from_utf8(&bytes[(start..i)])
// COMMON-REWRITES-NEXT:         .unwrap()
// COMMON-REWRITES-NEXT:         .parse()
// COMMON-REWRITES-NEXT:         .unwrap_or(0i64);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     (unsafe {
// COMMON-REWRITES-NEXT:         strcpy(
// COMMON-REWRITES-NEXT:             a.as_mut_ptr() as *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             c"  -42abc".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     (unsafe {
// COMMON-REWRITES-NEXT:         strcpy(
// COMMON-REWRITES-NEXT:             b.as_mut_ptr() as *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             c"+1000000000000zzz".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     (unsafe {
// COMMON-REWRITES-NEXT:         strcpy(
// COMMON-REWRITES-NEXT:             c.as_mut_ptr() as *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             c"9000000000000".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf({{__v[0-9]+}} as *const core::ffi::c_char, unsafe {
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf({{__v[0-9]+}} as *const core::ffi::c_char, unsafe {
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf({{__v[0-9]+}} as *const core::ffi::c_char, unsafe {
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: fn __slate_atoi(s: *const i8) -> i32 {
// REWRITES-X86_64-GNU-NEXT: fn __slate_atol(s: *const i8) -> i64 {
// REWRITES-X86_64-GNU-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// REWRITES-X86_64-GNU-NEXT:     let mut b: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// REWRITES-X86_64-GNU-NEXT:     let mut c: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// REWRITES-X86_64-GNU-NEXT:     }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:             __slate_atoi({{__v[0-9]+}} as *const i8)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%ld\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:             __slate_atol({{__v[0-9]+}} as *const i8)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%lld\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:             __slate_atol({{__v[0-9]+}} as *const i8)
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: fn __slate_atoi(s: *const u8) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn __slate_atol(s: *const u8) -> i64 {
// REWRITES-AARCH64-GNU-NEXT:     let mut a: [u8; 32] = [0; 32];
// REWRITES-AARCH64-GNU-NEXT:     let mut b: [u8; 32] = [0; 32];
// REWRITES-AARCH64-GNU-NEXT:     let mut c: [u8; 32] = [0; 32];
// REWRITES-AARCH64-GNU-NEXT:     }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:             __slate_atoi({{__v[0-9]+}} as *const u8)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%ld\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:             __slate_atol({{__v[0-9]+}} as *const u8)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%lld\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:             __slate_atol({{__v[0-9]+}} as *const u8)
// SLATE-FILECHECK-END rewrites-aarch64-gnu
