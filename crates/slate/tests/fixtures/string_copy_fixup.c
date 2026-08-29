#include <stdio.h>
#include <string.h>

int main(void) {
  char copy[16]         = {0};
  char append[16]       = "foo";
  char trunc_copy[16]   = {0};
  char trunc_append[16] = "pre";

  strcpy(copy, "abc");
  strcat(append, "bar");
  strncpy(trunc_copy, "abcdef", 3);
  strncat(trunc_append, "suffix", 3);

  printf("%s %s %s %s %zu\n", copy, append, trunc_copy, trunc_append,
         strlen(trunc_append));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn strcpy(_0: *mut i8, _1: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn strcat(_0: *mut i8, _1: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn strncpy(_0: *mut i8, _1: *const i8, _2: usize) -> *mut i8;
// LOWERING-NEXT:     fn strncat(_0: *mut i8, _1: *const i8, _2: usize) -> *mut i8;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn strlen(_0: *const i8) -> usize;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut copy: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut append: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut trunc_copy: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut trunc_append: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     *copy = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *append = [102, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *trunc_copy = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *trunc_append = [112, 114, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     let _v1: *mut i8 = copy.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: *mut i8 = unsafe { strcpy(_v1 as *mut i8, _v2 as *const i8) };
// LOWERING-NEXT:     let _v4: *mut i8 = append.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: *mut i8 = b"bar\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: *mut i8 = unsafe { strcat(_v4 as *mut i8, _v5 as *const i8) };
// LOWERING-NEXT:     let _v7: *mut i8 = trunc_copy.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v8: *mut i8 = b"abcdef\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v9: u64 = 3;
// LOWERING-NEXT:     let _v10: *mut i8 = unsafe { strncpy(_v7 as *mut i8, _v8 as *const i8, _v9 as usize) };
// LOWERING-NEXT:     let _v11: *mut i8 = trunc_append.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: *mut i8 = b"suffix\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v13: u64 = 3;
// LOWERING-NEXT:     let _v14: *mut i8 = unsafe { strncat(_v11 as *mut i8, _v12 as *const i8, _v13 as usize) };
// LOWERING-NEXT:     let _v15: *mut i8 = b"%s %s %s %s %zu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: *mut i8 = copy.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: *mut i8 = append.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v18: *mut i8 = trunc_copy.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: *mut i8 = trunc_append.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v20: *mut i8 = trunc_append.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v21: u64 = (unsafe { strlen(_v20 as *const i8) }) as u64;
// LOWERING-NEXT:     let _v22: i32 = unsafe { printf(_v15 as *const i8, _v16, _v17, _v18, _v19, _v21) };
// LOWERING-NEXT:     let _v23: i32 = 0;
// LOWERING-NEXT:     __retval = _v23;
// LOWERING-NEXT:     let _v24: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v24 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn strcpy(_0: *mut i8, _1: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn strcat(_0: *mut i8, _1: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn strncpy(_0: *mut i8, _1: *const i8, _2: usize) -> *mut i8;
// REWRITES-NEXT:     fn strncat(_0: *mut i8, _1: *const i8, _2: usize) -> *mut i8;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn strlen(_0: *const i8) -> usize;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut copy: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut append: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut trunc_copy: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut trunc_append: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: *copy = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: *append = [102, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: *trunc_copy = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: *trunc_append = [112, 114, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: let _v1: *mut i8 = copy.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: *mut i8 = unsafe { strcpy(_v1 as *mut i8, _v2 as *const i8) };
// REWRITES-NEXT: let _v4: *mut i8 = append.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: *mut i8 = b"bar\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: *mut i8 = unsafe { strcat(_v4 as *mut i8, _v5 as *const i8) };
// REWRITES-NEXT: let _v7: *mut i8 = trunc_copy.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: *mut i8 = b"abcdef\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v9: u64 = 3;
// REWRITES-NEXT: let _v10: *mut i8 = unsafe { strncpy(_v7 as *mut i8, _v8 as *const i8, _v9 as usize) };
// REWRITES-NEXT: let _v11: *mut i8 = trunc_append.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: *mut i8 = b"suffix\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v13: u64 = 3;
// REWRITES-NEXT: let _v14: *mut i8 = unsafe { strncat(_v11 as *mut i8, _v12 as *const i8, _v13 as usize) };
// REWRITES-NEXT: let _v15: *mut i8 = b"%s %s %s %s %zu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: *mut i8 = copy.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v17: *mut i8 = append.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v18: *mut i8 = trunc_copy.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: *mut i8 = trunc_append.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v20: *mut i8 = trunc_append.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v21: u64 = (unsafe { strlen(_v20 as *const i8) }) as u64;
// REWRITES-NEXT: let _v22: i32 = unsafe { printf(_v15 as *const i8, _v16, _v17, _v18, _v19, _v21) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
