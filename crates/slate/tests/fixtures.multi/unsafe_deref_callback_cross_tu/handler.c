#include "shared.h"

// @lowering-fn-begin
// @rewrite-fn-begin
int deref_and_add(int *p) { return *p + 1; }
// @rewrite-fn-end
// @lowering-fn-end

int call_handler(struct Callback *cb, int *p) { return cb->handler(p); }

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub unsafe extern "C" fn deref_and_add({{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C" fn deref_and_add({{arg[0-9]+}}: *mut i32) -> i32 {
// REWRITES-DAG:     (unsafe { *{{arg[0-9]+}} }) + 1
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
