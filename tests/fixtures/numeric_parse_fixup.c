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

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     whole = [52, 50, 0];
// COMMON-REWRITES-DAG:     whole_long = [45, 49, 50, 51, 52, 53, 0];
// COMMON-REWRITES-DAG:     whole_unsigned = [55, 55, 0];
// COMMON-REWRITES-DAG:     leading = [32, 32, 45, 49, 55, 116, 97, 105, 108, 0];
// COMMON-REWRITES-DAG:     empty = [0; 1];
// COMMON-REWRITES-DAG:         57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
// COMMON-REWRITES-DAG:         57, 57, 57, 57, 57, 57, 57, 0,
// COMMON-REWRITES-DAG:     ];
// COMMON-REWRITES-DAG:     flt = [32, 32, 45, 51, 46, 53, 101, 50, 114, 101, 115, 116, 0];
// COMMON-REWRITES-DAG:     end_source = [49, 50, 116, 97, 105, 108, 0];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-REWRITES-DAG:         strtol(
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-DAG:             10 as i32,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u64 = unsafe {
// COMMON-REWRITES-DAG:         strtoul(
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-DAG:             10 as i32,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-REWRITES-DAG:         strtol(
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-DAG:             10 as i32,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u64 = unsafe {
// COMMON-REWRITES-DAG:         strtoul(
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-DAG:             10 as i32,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         printf(
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             unsafe {
// COMMON-REWRITES-DAG:                 strtod(
// COMMON-REWRITES-DAG:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-DAG:                     {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-DAG:                 )
// COMMON-REWRITES-DAG:             },
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         printf(
// COMMON-REWRITES-DAG:             c"%ld %c\n".as_ptr(),
// COMMON-REWRITES-DAG:             unsafe {
// COMMON-REWRITES-DAG:                 strtol(
// COMMON-REWRITES-DAG:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-DAG:                     std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-DAG:                     10 as i32,
// COMMON-REWRITES-DAG:                 )
// COMMON-REWRITES-DAG:             },
// COMMON-REWRITES-DAG:             (unsafe { *end }) as i32,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     std::process::exit(0 as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:     let mut whole: [i8; 3] = [0; 3];
// REWRITES-X86_64-GNU-DAG:     let mut whole_long: [i8; 7] = [0; 7];
// REWRITES-X86_64-GNU-DAG:     let mut whole_unsigned: [i8; 3] = [0; 3];
// REWRITES-X86_64-GNU-DAG:     let mut leading: [i8; 10] = [0; 10];
// REWRITES-X86_64-GNU-DAG:     let mut empty: [i8; 1] = [0; 1];
// REWRITES-X86_64-GNU-DAG:     let mut large: aligned::Aligned<aligned::A16, [i8; 31]> = aligned::Aligned([0; 31]);
// REWRITES-X86_64-GNU-DAG:     let mut flt: [i8; 13] = [0; 13];
// REWRITES-X86_64-GNU-DAG:     let mut end_source: [i8; 7] = [0; 7];
// REWRITES-X86_64-GNU-DAG:     let mut end: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     *large = [
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = c"%d %ld %lu %ld %ld %lu %.1f\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = whole.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = unsafe { __slate_atoi({{__v[0-9]+}} as *const i8) };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = whole_long.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = whole_unsigned.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = leading.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i64 = unsafe { __slate_atol({{__v[0-9]+}} as *const i8) };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = large.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = flt.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = end_source.as_mut_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:     let mut whole: [u8; 3] = [0; 3];
// REWRITES-AARCH64-GNU-DAG:     let mut whole_long: [u8; 7] = [0; 7];
// REWRITES-AARCH64-GNU-DAG:     let mut whole_unsigned: [u8; 3] = [0; 3];
// REWRITES-AARCH64-GNU-DAG:     let mut leading: [u8; 10] = [0; 10];
// REWRITES-AARCH64-GNU-DAG:     let mut empty: [u8; 1] = [0; 1];
// REWRITES-AARCH64-GNU-DAG:     let mut large: [u8; 31] = [0; 31];
// REWRITES-AARCH64-GNU-DAG:     let mut flt: [u8; 13] = [0; 13];
// REWRITES-AARCH64-GNU-DAG:     let mut end_source: [u8; 7] = [0; 7];
// REWRITES-AARCH64-GNU-DAG:     let mut end: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-DAG:     large = [
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = c"%d %ld %lu %ld %ld %lu %.1f\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = whole.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = unsafe { __slate_atoi({{__v[0-9]+}} as *const u8) };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = whole_long.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = whole_unsigned.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = leading.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i64 = unsafe { __slate_atol({{__v[0-9]+}} as *const u8) };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = large.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = empty.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = flt.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = end_source.as_mut_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
