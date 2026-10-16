#include "shared.h"

// @check-fn-begin
int call_handler(struct Callback *callback) { return callback->handler(41); }
// @check-fn-end
