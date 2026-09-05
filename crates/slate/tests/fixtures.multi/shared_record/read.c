#include "shared.h"

// @lowering-fn-begin
// @rewrite-fn-begin
int read_pair(struct Pair *pair) { return pair->left + pair->right; }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub unsafe extern "C-unwind" fn read_pair({{arg[0-9]+}}: *mut Pair) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { (*{{arg[0-9]+}}).left };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { (*{{arg[0-9]+}}).right };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C-unwind" fn read_pair({{arg[0-9]+}}: *mut Pair) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { (*{{arg[0-9]+}}).left };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { (*{{arg[0-9]+}}).right };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
