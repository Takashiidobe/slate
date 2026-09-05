#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int global_values[4] = {2, 4, 6, 8};

// @lowering-fn-begin
// @rewrite-fn-begin
static int sum_values(const int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  return sum;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static void bump_values(int *values, int len) {
  for (int i = 0; i < len; ++i)
    values[i] += 1;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static int sum_prefix(const int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  return sum;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static int score_text(const unsigned char *text, int len) {
  (void)strlen((const char *)text);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += text[i];
  return score;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static int maybe_consume(int *values, int len, int release) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  if (release)
    free(values);
  return sum;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  int           local_values[4] = {1, 3, 5, 7};
  unsigned char text[]          = "abc";
  int           total           = sum_values(global_values, 4);
  bump_values(local_values, 4);
  int score    = score_text(text, 3);
  int borrowed = maybe_consume(local_values, 4, 0);
  int prefix   = sum_prefix(local_values, 3);
  printf("%d %d %d %d %d\n", total, local_values[3], score, borrowed, prefix);
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn sum_values({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let mut values: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-DAG:     let mut len: i32 = 0;
// COMMON-LOWERING-DAG:     let mut sum: i32 = 0;
// COMMON-LOWERING-DAG:     values = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     len = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     sum = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let mut i: i32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         loop {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = len;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                 break;
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = sum;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             sum = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG:             i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = sum;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn bump_values({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) {
// COMMON-LOWERING-DAG:     let mut values: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-DAG:     let mut len: i32 = 0;
// COMMON-LOWERING-DAG:     values = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     len = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let mut i: i32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         loop {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = len;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                 break;
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             unsafe {
// COMMON-LOWERING-DAG:                 *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG:             i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     return;
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn sum_prefix({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let mut values: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-DAG:     let mut len: i32 = 0;
// COMMON-LOWERING-DAG:     let mut sum: i32 = 0;
// COMMON-LOWERING-DAG:     values = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     len = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     sum = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let mut i: i32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         loop {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = len;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                 break;
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = sum;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             sum = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG:             i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = sum;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn score_text({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let mut text: *mut u8 = std::ptr::null_mut();
// COMMON-LOWERING-DAG:     let mut len: i32 = 0;
// COMMON-LOWERING-DAG:     let mut score: i32 = 0;
// COMMON-LOWERING-DAG:     text = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     len = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = text;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     score = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let mut i: i32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         loop {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = len;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                 break;
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut u8 = text;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = score;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             score = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG:             i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = score;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn maybe_consume({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let mut values: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-DAG:     let mut len: i32 = 0;
// COMMON-LOWERING-DAG:     let mut release: i32 = 0;
// COMMON-LOWERING-DAG:     let mut sum: i32 = 0;
// COMMON-LOWERING-DAG:     values = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     len = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     release = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     sum = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let mut i: i32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         loop {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = len;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                 break;
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = sum;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             sum = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG:             i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = release;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:             unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = sum;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn sum_values({{arg[0-9]+}}: &[i32]) -> i32 {
// COMMON-REWRITES-DAG:     let mut values: *mut i32 = {{arg[0-9]+}}.as_ptr() as *mut i32;
// COMMON-REWRITES-DAG:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// COMMON-REWRITES-DAG:     let mut sum: i32 = 0;
// COMMON-REWRITES-DAG:     for i in 0..len {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-DAG:         sum += unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     sum
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn bump_values({{arg[0-9]+}}: &mut [i32]) {
// COMMON-REWRITES-DAG:     let mut values: *mut i32 = {{arg[0-9]+}}.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-DAG:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// COMMON-REWRITES-DAG:     for i in 0..len {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-DAG:         unsafe {
// COMMON-REWRITES-DAG:             *{{__v[0-9]+}} = (unsafe { *{{__v[0-9]+}} }) + {{__v[0-9]+}};
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     return;
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn sum_prefix({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-DAG:     let __arg0_view = unsafe { std::slice::from_raw_parts({{arg[0-9]+}} as *const i32, {{arg[0-9]+}} as usize) };
// COMMON-REWRITES-DAG:     let mut values: *mut i32 = {{arg[0-9]+}};
// COMMON-REWRITES-DAG:     let mut len: i32 = {{arg[0-9]+}};
// COMMON-REWRITES-DAG:     let mut sum: i32 = 0;
// COMMON-REWRITES-DAG:     let mut i: i32 = 0;
// COMMON-REWRITES-DAG:     while i < len {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-REWRITES-DAG:         unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-DAG:         sum += unsafe { __arg0_view[(i as usize)] };
// COMMON-REWRITES-DAG:         i += 1;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     sum
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn score_text({{arg[0-9]+}}: &str) -> i32 {
// COMMON-REWRITES-DAG:     let mut text: *mut u8 = {{arg[0-9]+}}.as_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// COMMON-REWRITES-DAG:     let mut score: i32 = 0;
// COMMON-REWRITES-DAG:     (unsafe { {{arg[0-9]+}}.len() as u64 }) as u64;
// COMMON-REWRITES-DAG:     for i in 0..len {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: *mut u8 = text;
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-DAG:         score += (unsafe { *{{__v[0-9]+}} }) as i32;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     score
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn maybe_consume(mut values: *mut i32, mut len: i32, mut {{__v[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-DAG:     let mut sum: i32 = 0;
// COMMON-REWRITES-DAG:     for i in 0..len {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-DAG:         sum += unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         unsafe { free(values as *mut core::ffi::c_void) };
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     sum
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
