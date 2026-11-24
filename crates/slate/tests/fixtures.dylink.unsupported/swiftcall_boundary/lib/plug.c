#include "../abi.h"
#include "dylink.h"

EXPORT int call_cb(callback_fn *f) {
  int context = 23;
  return f(19, &context, ordinary_argument);
}
