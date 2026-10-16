#include "shared.h"

// @rewrite-fn-begin
int read_payload(struct Box *box) { return box->payload.value; }
// @rewrite-fn-end
