#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int global_values[4] = {2, 4, 6, 8};

// @check-fn-begin
static int sum_values(const int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  return sum;
}
// @check-fn-end

// @check-fn-begin
static void bump_values(int *values, int len) {
  for (int i = 0; i < len; ++i)
    values[i] += 1;
}
// @check-fn-end

// @check-fn-begin
static int sum_prefix(const int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  return sum;
}
// @check-fn-end

// @check-fn-begin
static int score_text(const unsigned char *text, int len) {
  (void)strlen((const char *)text);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += text[i];
  return score;
}
// @check-fn-end

// @check-fn-begin
static int maybe_consume(int *values, int len, int release) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  if (release)
    free(values);
  return sum;
}
// @check-fn-end

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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn sum_values(values: *const i32, len: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         sum += unsafe { *unsafe { values.offset(i as isize) } };
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn bump_values(values: *mut i32, len: i32) {
// CHECK-NEXT:     let __values_view = unsafe {
// CHECK-NEXT:         std::slice::from_raw_parts_mut(values as *mut i32, len as usize)
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __values_view[i as usize] = (unsafe { __values_view[i as usize] }) + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn sum_prefix(values: *const i32, len: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         sum += unsafe { *unsafe { values.offset(i as isize) } };
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn score_text(text: *const u8, len: i32) -> i32 {
// CHECK-NEXT:     unsafe { strlen(text as *const i8) };
// CHECK-NEXT:     let mut score: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         score += ((unsafe { *unsafe { text.offset(i as isize) } }) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     score
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn maybe_consume(values: *mut i32, len: i32, release: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         sum += unsafe { *unsafe { values.offset(i as isize) } };
// CHECK-NEXT:     }
// CHECK-NEXT:     if release != 0 {
// CHECK-NEXT:         unsafe { free(values as *mut ()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK: {{^}}}
// SLATE-FILECHECK-END
