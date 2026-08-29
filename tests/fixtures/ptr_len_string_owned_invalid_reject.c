#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int consume_bin(char *buf, int len) {
  (void)strlen(buf);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += (unsigned char)buf[i];
  free(buf);
  return score;
}

static int forward_bin(char *buf, int len) {
  return consume_bin(buf, len);
}

int main(void) {
  int len = 2;
  char *buf = malloc(len * sizeof(char));
  memcpy(buf, "\xff", len);
  int score = forward_bin(buf, len);
  printf("%d\n", score);
  return 0;
}

// REWRITES: fn consume_bin(arg{{[0-9]+}}: *mut i8, arg{{[0-9]+}}: i32) -> i32
// REWRITES-NOT: String::from_raw_parts(
// REWRITES-NOT: mut arg{{[0-9]+}}: String
