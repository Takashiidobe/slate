#include "shared.h"

#include <stdio.h>

int main(void) {
  struct stat info = {0};
  if (stat("/dev/null", &info) != 0)
    return 1;
  printf("%lld\n", read_mtime(&info));
}
