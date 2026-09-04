#include <crypt.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  struct crypt_data data;
  memset(&data, 0, sizeof(data));
  char *hash = crypt_r("hunter2", "ab", &data);
  printf("%s\n", hash);
  return 0;
}
