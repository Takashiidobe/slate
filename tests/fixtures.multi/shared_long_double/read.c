#include "shared.h"

// @lowering-fn-begin
// @rewrite-fn-begin
int read_marker(struct Box *box) { return box->marker; }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub unsafe extern "C" fn read_marker({{arg[0-9]+}}: *mut Box) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = unsafe { (*{{arg[0-9]+}}).marker };
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C" fn read_marker({{arg[0-9]+}}: *mut Box) -> i32 {
// REWRITES-DAG:     unsafe { (*{{arg[0-9]+}}).marker }
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
