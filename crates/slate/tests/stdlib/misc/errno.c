#include <errno.h>
#include <stdio.h>
#include <string.h>
int main(void) {
  errno = 0;
  printf("%d\n", errno);
  return 0;
}
