#include "shared.h"

int mode_value(struct Holder *holder) {
  return holder->mode == MODE_READY ? 42 : 0;
}
