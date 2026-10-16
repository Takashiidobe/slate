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

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn sum_values(mut values: *const i32, mut len: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < len {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = sum;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + unsafe { *unsafe { values.offset(i as isize) } };
// CHECK-NEXT:         sum = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn bump_values(mut values: *mut i32, mut len: i32) {
// CHECK-NEXT:     let __values_view = unsafe {
// CHECK-NEXT:         std::slice::from_raw_parts_mut(values as *mut i32, len as usize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < len {
// CHECK-NEXT:         unsafe { values.offset(i as isize) };
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { __values_view[(i as usize)] };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __values_view[(i as usize)] = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn sum_prefix(mut values: *const i32, mut len: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < len {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = sum;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + unsafe { *unsafe { values.offset(i as isize) } };
// CHECK-NEXT:         sum = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn score_text(mut text: *const u8, mut len: i32) -> i32 {
// CHECK-NEXT:     unsafe { strlen(text as *const i8) };
// CHECK-NEXT:     let mut score: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < len {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = score;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]]
// CHECK-NEXT:             + (((unsafe { *unsafe { text.offset(i as isize) } }) as u32) as i32);
// CHECK-NEXT:         score = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     score
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn maybe_consume(mut values: *mut i32, mut len: i32, mut release: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < len {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = sum;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + unsafe { *unsafe { values.offset(i as isize) } };
// CHECK-NEXT:         sum = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if release != (0 as i32) {
// CHECK-NEXT:         unsafe { free(values as *mut ()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK: {{^}}}
// SLATE-FILECHECK-END
