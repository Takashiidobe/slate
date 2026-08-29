#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>

static int bounded_cmp(const char *a, const char *b, size_t n) {
  return strncmp(a, b, n);
}

static size_t bounded_len(const char *s, size_t n) { return strnlen(s, n); }

static size_t spans(const char *s, const char *set) {
  return strspn(s, set) * 100 + strcspn(s, set);
}

int main(void) {
  const char abc[]    = "abc";
  const char abd[]    = "abd";
  const char text[]   = "abcdef";
  const char span[]   = "abacad";
  const char accept[] = "ab";
  const char empty[]  = "";
  const char reject[] = "cd";
  printf("%d %d %d %zu %zu %zu %zu %zu %zu\n",
         bounded_cmp(abc, abd, 2) == 0, bounded_cmp(abc, abd, 3) < 0,
         bounded_cmp(abc, abd, 0) == 0, bounded_len(abc, 99),
         bounded_len(text, 3), bounded_len(empty, 7), spans(span, accept),
         spans(span, empty), spans(empty, reject));
  return 0;
}
// REWRITES-NOT: unsafe { strncmp(
// REWRITES-NOT: unsafe { strnlen(
// REWRITES-NOT: unsafe { strspn(
// REWRITES-NOT: unsafe { strcspn(
