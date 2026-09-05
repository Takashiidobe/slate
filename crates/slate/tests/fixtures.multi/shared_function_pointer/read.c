#include "shared.h"

// @lowering-fn-begin
// @rewrite-fn-begin
int call_handler(struct Callback *callback) { return callback->handler(41); }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub unsafe extern "C-unwind" fn call_handler({{arg[0-9]+}}: *mut Callback) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe { (*{{arg[0-9]+}}).handler };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 41;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}) };
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C-unwind" fn call_handler({{arg[0-9]+}}: *mut Callback) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe { (*{{arg[0-9]+}}).handler };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 41;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}) };
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
