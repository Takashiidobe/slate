#include <stdio.h>
#include <stdlib.h>

// @rewrite-fn-begin
int main(void) {
  char  whole[]          = "42";
  char  whole_long[]     = "-12345";
  char  whole_unsigned[] = "77";
  char  leading[]        = "  -17tail";
  char  empty[]          = "";
  char  large[]          = "999999999999999999999999999999";
  char  flt[]            = "  -3.5e2rest";
  char  end_source[]     = "12tail";
  char *end              = 0;

  printf("%d %ld %lu %ld %ld %lu %.1f\n", atoi(whole),
         strtol(whole_long, 0, 10), strtoul(whole_unsigned, 0, 10),
         atol(leading), strtol(large, 0, 10), strtoul(empty, 0, 10),
         strtod(flt, 0));

  long raw = strtol(end_source, &end, 10);
  printf("%ld %c\n", raw, *end);
  return 0;
}
// @rewrite-fn-end

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
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn atoi(_0: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn strtol(_0: *const core::ffi::c_char, _1: *mut *mut core::ffi::c_char, _2: i32) -> i64;
// LOWERING-NEXT:     fn strtoul(_0: *const core::ffi::c_char, _1: *mut *mut core::ffi::c_char, _2: i32) -> u64;
// LOWERING-NEXT:     fn atol(_0: *const core::ffi::c_char) -> i64;
// LOWERING-NEXT:     fn strtod(_0: *const core::ffi::c_char, _1: *mut *mut core::ffi::c_char) -> f64;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut whole: [i8; 3] = [0; 3];
// LOWERING-NEXT:     let mut whole_long: [i8; 7] = [0; 7];
// LOWERING-NEXT:     let mut whole_unsigned: [i8; 3] = [0; 3];
// LOWERING-NEXT:     let mut leading: [i8; 10] = [0; 10];
// LOWERING-NEXT:     let mut empty: [i8; 1] = [0; 1];
// LOWERING-NEXT:     let mut large: aligned::Aligned<aligned::A16, [i8; 31]> = aligned::Aligned([0; 31]);
// LOWERING-NEXT:     let mut flt: [i8; 13] = [0; 13];
// LOWERING-NEXT:     let mut end_source: [i8; 7] = [0; 7];
// LOWERING-NEXT:     let mut end: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     whole = [52, 50, 0];
// LOWERING-NEXT:     whole_long = [45, 49, 50, 51, 52, 53, 0];
// LOWERING-NEXT:     whole_unsigned = [55, 55, 0];
// LOWERING-NEXT:     leading = [32, 32, 45, 49, 55, 116, 97, 105, 108, 0];
// LOWERING-NEXT:     empty = [0];
// LOWERING-NEXT:     *large = [
// LOWERING-NEXT:         57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
// LOWERING-NEXT:         57, 57, 57, 57, 57, 57, 57, 0,
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     flt = [32, 32, 45, 51, 46, 53, 101, 50, 114, 101, 115, 116, 0];
// LOWERING-NEXT:     end_source = [49, 50, 116, 97, 105, 108, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     end = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %ld %lu %ld %ld %lu %.1f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = whole.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { atoi({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = whole_long.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe {
// LOWERING-NEXT:         strtol(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = whole_unsigned.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe {
// LOWERING-NEXT:         strtoul(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = leading.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { atol({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = large.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe {
// LOWERING-NEXT:         strtol(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe {
// LOWERING-NEXT:         strtoul(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = flt.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe {
// LOWERING-NEXT:         strtod(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = end_source.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe {
// LOWERING-NEXT:         strtol(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%ld %c\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = end;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut whole: [i8; 3] = [52, 50, 0];
// REWRITES-DAG:     let mut whole_long: [i8; 7] = [45, 49, 50, 51, 52, 53, 0];
// REWRITES-DAG:     let mut whole_unsigned: [i8; 3] = [55, 55, 0];
// REWRITES-DAG:     let mut leading: [i8; 10] = [32, 32, 45, 49, 55, 116, 97, 105, 108, 0];
// REWRITES-DAG:     let mut empty: [i8; 1] = [0];
// REWRITES-DAG:     let mut large: aligned::Aligned<aligned::A16, [i8; 31]> = aligned::Aligned([0; 31]);
// REWRITES-DAG:     let mut flt: [i8; 13] = [0; 13];
// REWRITES-DAG:     let mut end_source: [i8; 7] = [0; 7];
// REWRITES-DAG:     let mut end: *mut i8 = std::ptr::null_mut();
// REWRITES-DAG:     *large = [
// REWRITES-DAG:         57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
// REWRITES-DAG:         57, 57, 57, 57, 57, 57, 57, 0,
// REWRITES-DAG:     ];
// REWRITES-DAG:     flt = [32, 32, 45, 51, 46, 53, 101, 50, 114, 101, 115, 116, 0];
// REWRITES-DAG:     end_source = [49, 50, 116, 97, 105, 108, 0];
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i8 = c"%d %ld %lu %ld %ld %lu %.1f\n".as_ptr() as *mut i8;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i8 = whole.as_mut_ptr() as *mut i8;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = unsafe { __slate_atoi({{_v[0-9]+}} as *const i8) };
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i8 = whole_long.as_mut_ptr() as *mut i8;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// REWRITES-DAG:     let {{_v[0-9]+}}: i64 = unsafe {
// REWRITES-DAG:         strtol(
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:             {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// REWRITES-DAG:             10 as i32,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i8 = whole_unsigned.as_mut_ptr() as *mut i8;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// REWRITES-DAG:     let {{_v[0-9]+}}: u64 = unsafe {
// REWRITES-DAG:         strtoul(
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:             {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// REWRITES-DAG:             10 as i32,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i8 = leading.as_mut_ptr() as *mut i8;
// REWRITES-DAG:     let {{_v[0-9]+}}: i64 = unsafe { __slate_atol({{_v[0-9]+}} as *const i8) };
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i8 = large.as_mut_ptr() as *mut i8;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// REWRITES-DAG:     let {{_v[0-9]+}}: i64 = unsafe {
// REWRITES-DAG:         strtol(
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:             {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// REWRITES-DAG:             10 as i32,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// REWRITES-DAG:     let {{_v[0-9]+}}: u64 = unsafe {
// REWRITES-DAG:         strtoul(
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:             {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// REWRITES-DAG:             10 as i32,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i8 = flt.as_mut_ptr() as *mut i8;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         printf(
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:             {{_v[0-9]+}},
// REWRITES-DAG:             {{_v[0-9]+}},
// REWRITES-DAG:             {{_v[0-9]+}},
// REWRITES-DAG:             {{_v[0-9]+}},
// REWRITES-DAG:             {{_v[0-9]+}},
// REWRITES-DAG:             {{_v[0-9]+}},
// REWRITES-DAG:             unsafe {
// REWRITES-DAG:                 strtod(
// REWRITES-DAG:                     {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:                     {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// REWRITES-DAG:                 )
// REWRITES-DAG:             },
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i8 = end_source.as_mut_ptr() as *mut i8;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         printf(
// REWRITES-DAG:             c"%ld %c\n".as_ptr(),
// REWRITES-DAG:             unsafe {
// REWRITES-DAG:                 strtol(
// REWRITES-DAG:                     {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:                     std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// REWRITES-DAG:                     10 as i32,
// REWRITES-DAG:                 )
// REWRITES-DAG:             },
// REWRITES-DAG:             (unsafe { *end }) as i32,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
