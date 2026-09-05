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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn sum_values({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-DAG:     let mut len: i32 = 0;
// LOWERING-DAG:     let mut sum: i32 = 0;
// LOWERING-DAG:     values = {{arg[0-9]+}};
// LOWERING-DAG:     len = {{arg[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     sum = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = len;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = values;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = sum;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:             sum = {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = sum;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn bump_values({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) {
// LOWERING-DAG:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-DAG:     let mut len: i32 = 0;
// LOWERING-DAG:     values = {{arg[0-9]+}};
// LOWERING-DAG:     len = {{arg[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = len;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = values;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     return;
// LOWERING-DAG: }
// LOWERING-DAG: fn sum_prefix({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-DAG:     let mut len: i32 = 0;
// LOWERING-DAG:     let mut sum: i32 = 0;
// LOWERING-DAG:     values = {{arg[0-9]+}};
// LOWERING-DAG:     len = {{arg[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     sum = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = len;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = values;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = sum;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:             sum = {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = sum;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn score_text({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut text: *mut u8 = std::ptr::null_mut();
// LOWERING-DAG:     let mut len: i32 = 0;
// LOWERING-DAG:     let mut score: i32 = 0;
// LOWERING-DAG:     text = {{arg[0-9]+}};
// LOWERING-DAG:     len = {{arg[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = text;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     score = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = len;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut u8 = text;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-DAG:             let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = score;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:             score = {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = score;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn maybe_consume({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-DAG:     let mut len: i32 = 0;
// LOWERING-DAG:     let mut release: i32 = 0;
// LOWERING-DAG:     let mut sum: i32 = 0;
// LOWERING-DAG:     values = {{arg[0-9]+}};
// LOWERING-DAG:     len = {{arg[0-9]+}};
// LOWERING-DAG:     release = {{arg[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     sum = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = len;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = values;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = sum;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:             sum = {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = release;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = values;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:             unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = sum;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn sum_values({{arg[0-9]+}}: &[i32]) -> i32 {
// REWRITES-DAG:     let mut values: *mut i32 = {{arg[0-9]+}}.as_ptr() as *mut i32;
// REWRITES-DAG:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-DAG:     let mut sum: i32 = 0;
// REWRITES-DAG:     for i in 0..len {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = values;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-DAG:         sum += unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:     }
// REWRITES-DAG:     sum
// REWRITES-DAG: }
// REWRITES-DAG: fn bump_values({{arg[0-9]+}}: &mut [i32]) {
// REWRITES-DAG:     let mut values: *mut i32 = {{arg[0-9]+}}.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-DAG:     for i in 0..len {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = values;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             *{{__v[0-9]+}} = (unsafe { *{{__v[0-9]+}} }) + {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     return;
// REWRITES-DAG: }
// REWRITES-DAG: fn sum_prefix({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let __arg0_view = unsafe { std::slice::from_raw_parts({{arg[0-9]+}} as *const i32, {{arg[0-9]+}} as usize) };
// REWRITES-DAG:     let mut values: *mut i32 = {{arg[0-9]+}};
// REWRITES-DAG:     let mut len: i32 = {{arg[0-9]+}};
// REWRITES-DAG:     let mut sum: i32 = 0;
// REWRITES-DAG:     let mut i: i32 = 0;
// REWRITES-DAG:     while i < len {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = values;
// REWRITES-DAG:         unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-DAG:         sum += unsafe { __arg0_view[(i as usize)] };
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     sum
// REWRITES-DAG: }
// REWRITES-DAG: fn score_text({{arg[0-9]+}}: &str) -> i32 {
// REWRITES-DAG:     let mut text: *mut u8 = {{arg[0-9]+}}.as_ptr() as *mut u8;
// REWRITES-DAG:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-DAG:     let mut score: i32 = 0;
// REWRITES-DAG:     (unsafe { {{arg[0-9]+}}.len() as u64 }) as u64;
// REWRITES-DAG:     for i in 0..len {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut u8 = text;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-DAG:         score += (unsafe { *{{__v[0-9]+}} }) as i32;
// REWRITES-DAG:     }
// REWRITES-DAG:     score
// REWRITES-DAG: }
// REWRITES-DAG: fn maybe_consume(mut values: *mut i32, mut len: i32, mut {{__v[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let mut sum: i32 = 0;
// REWRITES-DAG:     for i in 0..len {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = values;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-DAG:         sum += unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { free(values as *mut core::ffi::c_void) };
// REWRITES-DAG:     }
// REWRITES-DAG:     sum
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
