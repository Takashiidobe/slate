#include <stdio.h>

struct GNUEmpty {};

static size_t empty_size(struct GNUEmpty value) { return sizeof(value); }

int main(void) {
  struct GNUEmpty value = {};
  printf("%zu %zu\n", sizeof(struct GNUEmpty), empty_size(value));
  return 0;
}
