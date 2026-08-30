#include "shared.h"

// @lowering-fn-begin
// @rewrite-fn-begin
int read_pair(struct Pair *pair) { return pair->left + pair->right; }
// @rewrite-fn-end
// @lowering-fn-end
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub unsafe extern "C" fn read_pair({{arg[0-9]+}}: *mut Pair) -> i32 {
// LOWERING-DAG: let mut pair: *mut Pair = std::ptr::null_mut();
// LOWERING-DAG: let mut __retval: i32 = 0;
// LOWERING-DAG: pair = {{arg[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut Pair = pair;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).left };
// LOWERING-DAG: let {{_v[0-9]+}}: *mut Pair = pair;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).right };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: __retval = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C" fn read_pair({{arg[0-9]+}}: *mut Pair) -> i32 {
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: __retval = (unsafe { (*{{arg[0-9]+}}).left }) + unsafe { (*{{arg[0-9]+}}).right };
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
