#include "shared.h"

// @lowering-fn-begin
// @rewrite-fn-begin
int read_payload(struct Box *box) { return box->payload.value; }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub unsafe extern "C-unwind" fn read_payload({{arg[0-9]+}}: *mut Box) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = unsafe { (*{{arg[0-9]+}}).payload.value };
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C-unwind" fn read_payload({{arg[0-9]+}}: *mut Box) -> i32 {
// REWRITES-DAG:     unsafe { (*{{arg[0-9]+}}).payload.value }
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
