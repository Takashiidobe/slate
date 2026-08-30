#include "shared.h"

// @lowering-fn-begin
// @rewrite-fn-begin
int read_payload(struct Box *box) { return box->payload.value; }
// @rewrite-fn-end
// @lowering-fn-end
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub unsafe extern "C" fn read_payload({{arg[0-9]+}}: *mut Box) -> i32 {
// LOWERING-DAG: let mut r#box: *mut Box = std::ptr::null_mut();
// LOWERING-DAG: let mut __retval: i32 = 0;
// LOWERING-DAG: r#box = {{arg[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut Box = r#box;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).payload.value };
// LOWERING-DAG: __retval = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C" fn read_payload({{arg[0-9]+}}: *mut Box) -> i32 {
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: __retval = unsafe { (*{{arg[0-9]+}}).payload.value };
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
