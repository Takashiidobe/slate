#include <stdio.h>

static int check_target(const char *ptr, const char *end, int *tokPtr) {
  int upper = 0;
  *tokPtr = 7;
  if (end - ptr != 3)
    return 1;
  switch (ptr[0]) {
  case 'x':
    break;
  case 'X':
    upper = 1;
    break;
  default:
    return 1;
  }
  if (upper)
    *tokPtr = 9;
  return 0;
}

int main(void) {
  int tok = 0;
  const char *s = "xyz";
  int r = check_target(s, s + 3, &tok);
  printf("%d %d\n", r, tok);
  return 0;
}
