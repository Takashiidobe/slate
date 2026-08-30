#include "shared.h"

// @lowering-fn-begin
// @rewrite-fn-begin
int call_handler(struct Callback *callback) { return callback->handler(41); }
// @rewrite-fn-end
// @lowering-fn-end
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub unsafe extern "C" fn call_handler({{arg[0-9]+}}: *mut Callback) -> i32 {
// LOWERING-DAG: let mut callback: *mut Callback = std::ptr::null_mut();
// LOWERING-DAG: let mut __retval: i32 = 0;
// LOWERING-DAG: callback = {{arg[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut Callback = callback;
// LOWERING-DAG: let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { (*{{_v[0-9]+}}).handler };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 41;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-DAG: __retval = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C" fn call_handler({{arg[0-9]+}}: *mut Callback) -> i32 {
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 41;
// REWRITES-DAG: __retval = unsafe { unsafe { (*{{arg[0-9]+}}).handler }.unwrap()({{_v[0-9]+}}) };
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
