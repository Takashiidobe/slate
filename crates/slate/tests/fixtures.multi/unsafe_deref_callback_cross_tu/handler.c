#include "shared.h"

// @rewrite-fn-begin
int deref_and_add(int *p) { return *p + 1; }
// @rewrite-fn-end

int call_handler(struct Callback *cb, int *p) { return cb->handler(p); }

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C" fn deref_and_add({{arg[0-9]+}}: *mut i32) -> i32 {
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-DAG: __retval = (unsafe { *{{arg[0-9]+}} }) + {{_v[0-9]+}};
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// LOWERING-LABEL: {{^}}pub unsafe extern "C" fn deref_and_add(
// LOWERING-DAG: unsafe { *_v{{[0-9]+}} }
// LOWERING: {{^}}}
