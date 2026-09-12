#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int global_values[4] = {2, 4, 6, 8};

static int sum_values(const int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  return sum;
}

static void bump_values(int *values, int len) {
  for (int i = 0; i < len; ++i)
    values[i] += 1;
}

static int sum_prefix(const int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  return sum;
}

static int score_text(const unsigned char *text, int len) {
  (void)strlen((const char *)text);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += text[i];
  return score;
}

static int maybe_consume(int *values, int len, int release) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  if (release)
    free(values);
  return sum;
}

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



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
