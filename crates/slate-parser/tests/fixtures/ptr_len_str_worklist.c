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
  int                 score   = forward_text(bytes, 3);
  printf("%d\n", score);
  return 0;
}

// REWRITES: std::str::from_utf8_unchecked(

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
