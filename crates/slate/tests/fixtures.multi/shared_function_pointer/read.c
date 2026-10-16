#include "shared.h"

// @rewrite-fn-begin
int call_handler(struct Callback *callback) { return callback->handler(41); }
// @rewrite-fn-end
