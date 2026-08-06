#include <stdint.h>
#include <stdio.h>

static int cache_prefetch_probe(int x) {
  char bytes[16] = {0};
  bytes[0]       = (char)x;
  __builtin___clear_cache(bytes, bytes + sizeof(bytes));
  __builtin_prefetch(bytes + 1, 0, 3);
  return bytes[0] + 1;
}

static int frame_probe(void) {
  void *frame = __builtin_frame_address(0);
  return frame != 0;
}

int main(void) {
  volatile int input = 7;
  printf("%d %d\n", cache_prefetch_probe(input), frame_probe());
  return 0;
}
