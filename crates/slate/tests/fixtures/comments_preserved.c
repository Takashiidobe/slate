/* preserved-file-comment */
#include <stdio.h>
#include "comments_preserved.h"
#include "comments_preserved.h"
struct Value {
  /* preserved-field-comment */
  int n;
  /* preserved-field-trailing-comment */
};
enum Kind {
  /* preserved-enumerator-comment */
  KIND = 3
};
/* preserved-switch-function-comment */
static int selected(int n) {
  switch (n) {
    /* preserved-before-case-comment */
    case 0:
      /* preserved-between-cases-comment */
    case 1:
      return 3;
      /* preserved-after-return-comment */
    default:
      return KIND;
  }
}
/* preserved-goto-function-comment */
static int jumped(int n) {
  /* preserved-before-goto-comment */
  goto done;
  /* preserved-unreachable-comment */
  n++;
done:
  /* preserved-label-comment */
  return n;
}
/* preserved-function-comment */
static int value(int n) {
  /* preserved-local-comment */
  int result = n;
  while (result < 3) {
    // preserved-loop-comment
    result++;
  }
  /* preserved-return-comment */
  return result;
}
int main(void) {
  struct HeaderValue h = { jumped(selected(1)) };
  struct Value v = { value(h.n) };
  // preserved-call-comment
  printf("%d\n", v.n);
  return 0;
}
/* preserved-trailing-comment */
