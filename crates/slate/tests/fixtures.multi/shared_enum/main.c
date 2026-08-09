#include "shared.h"

int mode_value(struct Holder *holder);

int main(void) {
  struct Holder holder = {MODE_READY};
  return mode_value(&holder) != 42;
}
