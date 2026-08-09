#include "shared.h"

int call_handler(struct Callback *callback) { return callback->handler(41); }
