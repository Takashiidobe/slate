#include "shared.h"

// @check-fn-begin
int mode_value(struct Holder *holder) {
  return holder->mode == MODE_READY ? 42 : 0;
}
// @check-fn-end
