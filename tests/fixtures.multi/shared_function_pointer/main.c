#include "shared.h"

int call_handler(struct Callback *callback);

static int add_one(int value) { return value + 1; }

int main(void) {
  struct Callback callback = {0};
  callback.handler         = add_one;
  return call_handler(&callback) != 42;
}
