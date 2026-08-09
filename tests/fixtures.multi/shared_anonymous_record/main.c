#include "shared.h"

int read_payload(struct Box *box);

int main(void) {
  struct Box box = {{42}};
  return read_payload(&box) != 42;
}
