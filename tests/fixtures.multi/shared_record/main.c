#include "shared.h"

int read_pair(struct Pair *pair);

int main(void) {
  struct Pair pair = {20, 22};
  return read_pair(&pair) != 42;
}
