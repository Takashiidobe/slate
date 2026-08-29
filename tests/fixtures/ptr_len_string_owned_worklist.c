#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int consume_text(char *text, int len) {
  (void)strlen(text);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += (unsigned char)text[i];
  free(text);
  return score;
}

static int forward_text_owner(char *text, int len) {
  return consume_text(text, len);
}

int main(void) {
  int len = 4;
  char *text = malloc(len * sizeof(char));
  memcpy(text, "abc", len);
  int score = forward_text_owner(text, len);
  printf("%d\n", score);
  return 0;
}

// REWRITES-DAG: fn consume_text(mut arg{{[0-9]+}}: String) -> i32
// REWRITES-DAG: fn forward_text_owner(mut arg{{[0-9]+}}: String) -> i32
// REWRITES: String::from_raw_parts(
// REWRITES-NOT: unsafe { free(
