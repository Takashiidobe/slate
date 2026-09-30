#include <stdio.h>
#include <string.h>

static long double global_pi = 3.141592653589793238462643383279L;

static void dump80(const char *name, long double value) {
  unsigned char bytes[10];
  memcpy(bytes, &value, 10);
  printf("%s", name);
  for (int i = 0; i < 10; ++i)
    printf("%02x", bytes[i]);
  printf("\n");
}

int main(void) {
  long double local_pi = 3.141592653589793238462643383279L;
  dump80("local", local_pi);
  dump80("global", global_pi);
  return 0;
}
