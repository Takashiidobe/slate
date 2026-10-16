#include "shared.h"

// @check-fn-begin
int deref_and_add(int *p) { return *p + 1; }
// @check-fn-end

int call_handler(struct Callback *cb, int *p) { return cb->handler(p); }
