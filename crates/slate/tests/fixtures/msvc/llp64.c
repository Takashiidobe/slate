#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

_Static_assert(sizeof(size_t) == 8, "size_t");
_Static_assert(sizeof(ptrdiff_t) == 8, "ptrdiff_t");
_Static_assert(sizeof(intptr_t) == 8, "intptr_t");
_Static_assert(sizeof(uintptr_t) == 8, "uintptr_t");
_Static_assert(sizeof(wchar_t) == 2, "wchar_t");
_Static_assert(sizeof(long) == 4, "long");
_Static_assert(sizeof(long long) == 8, "long long");
_Static_assert(sizeof(long double) == 8, "long double");

extern long long imported_msvc(size_t, ptrdiff_t, intptr_t, uintptr_t, wchar_t,
                               long, long long, long double);

long long call_imported_msvc(size_t size, ptrdiff_t difference,
                             intptr_t  signed_pointer,
                             uintptr_t unsigned_pointer, wchar_t wide,
                             long narrow_long, long long wide_long,
                             long double real) {
  return imported_msvc(size, difference, signed_pointer, unsigned_pointer, wide,
                       narrow_long, wide_long, real);
}

int main(void) {
  char buffer[8];
  strcpy(buffer, "slate");
  printf("%zu %d\n", strlen(buffer), isdigit('7') != 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering-msvc
// LOWERING-MSVC: #![feature(c_variadic)]
// LOWERING-MSVC-NEXT: #![allow(
// LOWERING-MSVC-NEXT:     dead_code,
// LOWERING-MSVC-NEXT:     unused,
// LOWERING-MSVC-NEXT:     non_camel_case_types,
// LOWERING-MSVC-NEXT:     non_snake_case,
// LOWERING-MSVC-NEXT:     non_upper_case_globals,
// LOWERING-MSVC-NEXT:     arithmetic_overflow,
// LOWERING-MSVC-NEXT:     unconditional_panic,
// LOWERING-MSVC-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-MSVC-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-MSVC-NEXT:     unused_comparisons
// LOWERING-MSVC-NEXT: )]
// LOWERING-MSVC-EMPTY:
// LOWERING-MSVC-NEXT: unsafe extern "C" {
// LOWERING-MSVC-NEXT:     fn imported_msvc(
// LOWERING-MSVC-NEXT:         _0: usize,
// LOWERING-MSVC-NEXT:         _1: isize,
// LOWERING-MSVC-NEXT:         _2: isize,
// LOWERING-MSVC-NEXT:         _3: usize,
// LOWERING-MSVC-NEXT:         _4: u16,
// LOWERING-MSVC-NEXT:         _5: i32,
// LOWERING-MSVC-NEXT:         _6: i64,
// LOWERING-MSVC-NEXT:         _7: f64,
// LOWERING-MSVC-NEXT:     ) -> i64;
// LOWERING-MSVC-NEXT:     fn strcpy(_0: *mut core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// LOWERING-MSVC-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-MSVC-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// LOWERING-MSVC-NEXT:     fn isdigit(_0: i32) -> i32;
// LOWERING-MSVC-NEXT: }
// LOWERING-MSVC-EMPTY:
// LOWERING-MSVC-NEXT: fn call_imported_msvc(
// LOWERING-MSVC-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-MSVC-NEXT:     {{arg[0-9]+}}: i64,
// LOWERING-MSVC-NEXT:     {{arg[0-9]+}}: i64,
// LOWERING-MSVC-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-MSVC-NEXT:     {{arg[0-9]+}}: u16,
// LOWERING-MSVC-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-MSVC-NEXT:     {{arg[0-9]+}}: i64,
// LOWERING-MSVC-NEXT:     {{arg[0-9]+}}: f64,
// LOWERING-MSVC-NEXT: ) -> i64 {
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// LOWERING-MSVC-NEXT:         imported_msvc(
// LOWERING-MSVC-NEXT:             {{arg[0-9]+}} as usize,
// LOWERING-MSVC-NEXT:             {{arg[0-9]+}} as isize,
// LOWERING-MSVC-NEXT:             {{arg[0-9]+}} as isize,
// LOWERING-MSVC-NEXT:             {{arg[0-9]+}} as usize,
// LOWERING-MSVC-NEXT:             {{arg[0-9]+}} as u16,
// LOWERING-MSVC-NEXT:             {{arg[0-9]+}} as i32,
// LOWERING-MSVC-NEXT:             {{arg[0-9]+}} as i64,
// LOWERING-MSVC-NEXT:             {{arg[0-9]+}} as f64,
// LOWERING-MSVC-NEXT:         )
// LOWERING-MSVC-NEXT:     };
// LOWERING-MSVC-NEXT:     return {{__v[0-9]+}};
// LOWERING-MSVC-NEXT: }
// LOWERING-MSVC-EMPTY:
// LOWERING-MSVC-NEXT: fn main() {
// LOWERING-MSVC-NEXT:     let mut buffer: [i8; 8] = [0; 8];
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: *mut i8 = buffer.as_mut_ptr() as *mut i8;
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-MSVC-NEXT:         strcpy(
// LOWERING-MSVC-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-MSVC-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-MSVC-NEXT:         )
// LOWERING-MSVC-NEXT:     }) as *mut i8;
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %d\n\0".as_ptr() as *mut i8;
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: *mut i8 = buffer.as_mut_ptr() as *mut i8;
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: i32 = 55;
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isdigit({{__v[0-9]+}} as i32) };
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-MSVC-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-MSVC-NEXT: }
// SLATE-FILECHECK-END lowering-msvc

// SLATE-FILECHECK-BEGIN rewrites-msvc
// REWRITES-MSVC: #![feature(c_variadic)]
// REWRITES-MSVC-NEXT: #![allow(
// REWRITES-MSVC-NEXT:     dead_code,
// REWRITES-MSVC-NEXT:     unused,
// REWRITES-MSVC-NEXT:     non_camel_case_types,
// REWRITES-MSVC-NEXT:     non_snake_case,
// REWRITES-MSVC-NEXT:     non_upper_case_globals,
// REWRITES-MSVC-NEXT:     arithmetic_overflow,
// REWRITES-MSVC-NEXT:     unconditional_panic,
// REWRITES-MSVC-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-MSVC-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-MSVC-NEXT:     unused_comparisons
// REWRITES-MSVC-NEXT: )]
// REWRITES-MSVC-EMPTY:
// REWRITES-MSVC-NEXT: unsafe extern "C" {
// REWRITES-MSVC-NEXT:     fn imported_msvc(
// REWRITES-MSVC-NEXT:         _0: usize,
// REWRITES-MSVC-NEXT:         _1: isize,
// REWRITES-MSVC-NEXT:         _2: isize,
// REWRITES-MSVC-NEXT:         _3: usize,
// REWRITES-MSVC-NEXT:         _4: u16,
// REWRITES-MSVC-NEXT:         _5: i32,
// REWRITES-MSVC-NEXT:         _6: i64,
// REWRITES-MSVC-NEXT:         _7: f64,
// REWRITES-MSVC-NEXT:     ) -> i64;
// REWRITES-MSVC-NEXT:     fn strcpy(_0: *mut core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// REWRITES-MSVC-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-MSVC-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// REWRITES-MSVC-NEXT:     fn isdigit(_0: i32) -> i32;
// REWRITES-MSVC-NEXT: }
// REWRITES-MSVC-EMPTY:
// REWRITES-MSVC-NEXT: fn call_imported_msvc(
// REWRITES-MSVC-NEXT:     {{arg[0-9]+}}: u64,
// REWRITES-MSVC-NEXT:     {{arg[0-9]+}}: i64,
// REWRITES-MSVC-NEXT:     {{arg[0-9]+}}: i64,
// REWRITES-MSVC-NEXT:     {{arg[0-9]+}}: u64,
// REWRITES-MSVC-NEXT:     {{arg[0-9]+}}: u16,
// REWRITES-MSVC-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-MSVC-NEXT:     {{arg[0-9]+}}: i64,
// REWRITES-MSVC-NEXT:     {{arg[0-9]+}}: f64,
// REWRITES-MSVC-NEXT: ) -> i64 {
// REWRITES-MSVC-NEXT:     unsafe {
// REWRITES-MSVC-NEXT:         imported_msvc(
// REWRITES-MSVC-NEXT:             {{arg[0-9]+}} as usize,
// REWRITES-MSVC-NEXT:             {{arg[0-9]+}} as isize,
// REWRITES-MSVC-NEXT:             {{arg[0-9]+}} as isize,
// REWRITES-MSVC-NEXT:             {{arg[0-9]+}} as usize,
// REWRITES-MSVC-NEXT:             {{arg[0-9]+}} as u16,
// REWRITES-MSVC-NEXT:             {{arg[0-9]+}} as i32,
// REWRITES-MSVC-NEXT:             {{arg[0-9]+}} as i64,
// REWRITES-MSVC-NEXT:             {{arg[0-9]+}} as f64,
// REWRITES-MSVC-NEXT:         )
// REWRITES-MSVC-NEXT:     }
// REWRITES-MSVC-NEXT: }
// REWRITES-MSVC-EMPTY:
// REWRITES-MSVC-NEXT: fn main() {
// REWRITES-MSVC-NEXT:     let mut buffer: [i8; 8] = [0; 8];
// REWRITES-MSVC-NEXT:     (unsafe {
// REWRITES-MSVC-NEXT:         strcpy(
// REWRITES-MSVC-NEXT:             buffer.as_mut_ptr() as *mut core::ffi::c_char,
// REWRITES-MSVC-NEXT:             c"slate".as_ptr(),
// REWRITES-MSVC-NEXT:         )
// REWRITES-MSVC-NEXT:     }) as *mut i8;
// REWRITES-MSVC-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%zu %d\n".as_ptr() as *mut i8;
// REWRITES-MSVC-NEXT:     let {{__v[0-9]+}}: *mut i8 = buffer.as_mut_ptr() as *mut i8;
// REWRITES-MSVC-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// REWRITES-MSVC-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isdigit(55 as i32) };
// REWRITES-MSVC-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, ({{__v[0-9]+}} != 0) as i32) };
// REWRITES-MSVC-NEXT:     std::process::exit(0 as i32);
// REWRITES-MSVC-NEXT: }
// SLATE-FILECHECK-END rewrites-msvc
