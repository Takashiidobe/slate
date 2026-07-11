#include <errno.h>
#include <string.h>
#include <stdio.h>
int main(void) {
  char *a = strerror(ENOENT);
  char *b = strerror(0);
  printf("%d %d\n", strstr(a, "No such") != 0, strlen(b) > 0);
  return 0;
}
