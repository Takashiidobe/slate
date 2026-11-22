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
// CHECK-NEXT:     let alpha: &str = "alpha";
// CHECK-NEXT:     let beta: &str = "beta";
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     total += (alpha == "alpha") as i32;
// CHECK-NEXT:     total += (beta == "alpha") as i32;
// CHECK-NEXT:     total
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn same_prefix() -> i32 {
// CHECK-NEXT:     let a: &str = "abx";
// CHECK-NEXT:     if a.starts_with("ab") {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn long_word() -> i32 {
// CHECK-NEXT:     let s: &str = "lengthy";
// CHECK-NEXT:     (((s.len() as u64) as i32) > 4) as i32
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn spans() -> i32 {
// CHECK-NEXT:     let s: &str = "  word";
// CHECK-NEXT:     ((slate_support::__slate_strspn(s, " ") as u64) as i32)
// CHECK-NEXT:         + ((slate_support::__slate_strcspn(s, "r") as u64) as i32)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn parsed() -> i32 {
// CHECK-NEXT:     3 + "12".parse::<i32>().unwrap() * 2
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn embedded(n: i32) -> i32 {
// CHECK-NEXT:     let s: &str = "ab";
// CHECK-NEXT:     ((slate_support::__slate_strspn(s, "abcd") as u64) as i32)
// CHECK-NEXT:         + (s
// CHECK-NEXT:             .bytes()
// CHECK-NEXT:             .take((n as u64) as usize)
// CHECK-NEXT:             .eq("abz".bytes().take((n as u64) as usize)) as i32)
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
