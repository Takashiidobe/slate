#include <crypt.h>
#include <stdio.h>

int main(void) {
  char *hash = crypt("hunter2", "ab");
  printf("%s\n", hash);
  return 0;
}
