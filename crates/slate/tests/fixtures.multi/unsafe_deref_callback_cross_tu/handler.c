#include "shared.h"

// @rewrite-fn-begin
int deref_and_add(int *p) { return *p + 1; }
// @rewrite-fn-end

int call_handler(struct Callback *cb, int *p) { return cb->handler(p); }
