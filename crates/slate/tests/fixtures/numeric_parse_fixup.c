#include <stdio.h>
#include <stdlib.h>

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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn atoi(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn strtol(_0: *const i8, _1: *mut *mut i8, _2: i32) -> i64;
// LOWERING-NEXT:     fn strtoul(_0: *const i8, _1: *mut *mut i8, _2: i32) -> u64;
// LOWERING-NEXT:     fn atol(_0: *const i8) -> i64;
// LOWERING-NEXT:     fn strtod(_0: *const i8, _1: *mut *mut i8) -> f64;
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
// LOWERING-NEXT:     *large = [57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 0];
// LOWERING-NEXT:     flt = [32, 32, 45, 51, 46, 53, 101, 50, 114, 101, 115, 116, 0];
// LOWERING-NEXT:     end_source = [49, 50, 116, 97, 105, 108, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     end = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %ld %lu %ld %ld %lu %.1f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = whole.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { atoi({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = whole_long.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { strtol({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *mut *mut i8, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = whole_unsigned.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { strtoul({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *mut *mut i8, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = leading.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { atol({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = large.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { strtol({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *mut *mut i8, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { strtoul({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *mut *mut i8, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = flt.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { strtod({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *mut *mut i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = end_source.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { strtol({{_v[0-9]+}} as *const i8, std::ptr::addr_of_mut!(end) as *mut *mut i8, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%ld %c\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = end;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: unsafe { strtol(
