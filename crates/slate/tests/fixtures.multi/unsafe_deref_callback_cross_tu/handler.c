#include "shared.h"

int deref_and_add(int *p) { return *p + 1; }

int call_handler(struct Callback *cb, int *p) { return cb->handler(p); }
