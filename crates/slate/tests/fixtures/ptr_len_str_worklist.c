#include <stdio.h>
#include <string.h>

static int score_text(const unsigned char *bytes, int len) {
  (void)strlen((const char *)bytes);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += bytes[i];
  return score;
}

static int forward_text(const unsigned char *bytes, int len) {
  return score_text(bytes, len);
}

int main(void) {
  const unsigned char bytes[] = "abc";
  int score = forward_text(bytes, 3);
  printf("%d\n", score);
  return 0;
}

// REWRITES-DAG: fn score_text(arg{{[0-9]+}}: &str) -> i32
// REWRITES-DAG: fn forward_text(arg{{[0-9]+}}: &str) -> i32
// REWRITES: std::str::from_utf8_unchecked(
