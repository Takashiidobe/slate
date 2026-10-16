#include "shared.h"

// @check-fn-begin
int read_payload(struct Box *box) { return box->payload.value; }
// @check-fn-end
