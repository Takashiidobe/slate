#include "shared.h"

int deref_and_add(int *p) { return *p + 1; }

int call_handler(struct Callback *cb, int *p) { return cb->handler(p); }
// LOWERING-LABEL: {{^}}pub unsafe extern "C" fn deref_and_add(
// LOWERING-DAG: unsafe { *_v{{[0-9]+}} }
// LOWERING: {{^}}}
