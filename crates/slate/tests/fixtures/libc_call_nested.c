#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *words[] = {"alpha", "beta", "alpha"};
static int cursor;

static const char *next_word(void) { return words[cursor++ % 3]; }

// @check-fn-begin
static int count_alpha(void) {
  const char *alpha = "alpha";
  const char *beta = "beta";
  int total = 0;
  total += strcmp(alpha, "alpha") == 0;
  total += strcmp(beta, "alpha") == 0;
  return total;
}
// @check-fn-end

// @check-fn-begin
static int same_prefix(void) {
  const char *a = "abx";
  if (strncmp(a, "aby", 2) == 0) return 1;
  return 0;
}
// @check-fn-end

// @check-fn-begin
static int long_word(void) {
  const char *s = "lengthy";
  return (int)strlen(s) > 4 ? 1 : 0;
}
// @check-fn-end

// @check-fn-begin
static int spans(void) {
  const char *s = "  word";
  return (int)strspn(s, " ") + (int)strcspn(s, "r");
}
// @check-fn-end

// @check-fn-begin
static int parsed(void) {
  return 3 + atoi("12") * 2;
}
// @check-fn-end

// @check-fn-begin
static int embedded(int n) {
  const char *s = "ab\0cd";
  return (int)strspn(s, "abcd") + (strncmp(s, "abz", n) == 0);
}
// @check-fn-end

// @check-fn-begin
static int effectful(void) {
  if (strcmp(next_word(), "beta") == 0) return 7;
  return 8;
}
// @check-fn-end

int main(void) {
  printf("%d %d %d %d %d\n", count_alpha(), same_prefix(), long_word(), spans(), parsed());
  printf("%d %d\n", effectful(), effectful());
  printf("%d %d %d\n", embedded(1), embedded(2), embedded(3));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn count_alpha() -> i32 {
// CHECK-NEXT:     let alpha: *const i8 = c"alpha".as_ptr();
// CHECK-NEXT:     let beta: *const i8 = c"beta".as_ptr();
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     total += (0i32 == 0) as i32;
// CHECK-NEXT:     total += (1i32 == 0) as i32;
// CHECK-NEXT:     total
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn same_prefix() -> i32 {
// CHECK-NEXT:     let a: *const i8 = c"abx".as_ptr();
// CHECK-NEXT:     if 0i32 == 0 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn long_word() -> i32 {
// CHECK-NEXT:     let s: *const i8 = c"lengthy".as_ptr();
// CHECK-NEXT:     ((7u64 as i32) > 4) as i32
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn spans() -> i32 {
// CHECK-NEXT:     let s: *const i8 = c"  word".as_ptr();
// CHECK-NEXT:     (2u64 as i32) + (4u64 as i32)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn parsed() -> i32 {
// CHECK-NEXT:     3 + 12i32 * 2
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn embedded(n: i32) -> i32 {
// CHECK-NEXT:     let s: *const i8 = b"ab\0cd\0".as_ptr() as *const i8;
// CHECK-NEXT:     (2u64 as i32)
// CHECK-NEXT:         + (((b"ab"[0usize..((n as u64) as usize).min(2usize)]
// CHECK-NEXT:             .cmp(&b"abz"[0usize..((n as u64) as usize).min(3usize)]) as i32) == 0)
// CHECK-NEXT:             as i32)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn effectful() -> i32 {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strcmp(
// CHECK-NEXT:             next_word() as *const core::ffi::c_char,
// CHECK-NEXT:             c"beta".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     }) == 0 {
// CHECK-NEXT:         return 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     8
// CHECK: {{^}}}
// SLATE-FILECHECK-END
