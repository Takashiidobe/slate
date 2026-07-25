#include <stdio.h>
#include <string.h>
int main(void) {
  const char *volatile a = "abcx";
  const char *volatile b = "abcy";
  printf("%d\n", strncmp((char *)a, (char *)b, 3));
  return 0;
}
