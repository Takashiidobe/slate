#include "shared.h"

// @rewrite-fn-begin
int mode_value(struct Holder *holder) {
  return holder->mode == MODE_READY ? 42 : 0;
}
// @rewrite-fn-end
