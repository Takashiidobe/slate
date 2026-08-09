#include "shared.h"

int read_marker(struct Box *box);

int main(void) {
  struct Box box = {1.5L, 42};
  return read_marker(&box) != 42;
}
