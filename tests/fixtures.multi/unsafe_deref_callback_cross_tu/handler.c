#include "shared.h"

// @lowering-fn-begin
// @rewrite-fn-begin
int deref_and_add(int *p) { return *p + 1; }
// @rewrite-fn-end
// @lowering-fn-end

int call_handler(struct Callback *cb, int *p) { return cb->handler(p); }

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub unsafe extern "C-unwind" fn deref_and_add({{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C-unwind" fn deref_and_add({{arg[0-9]+}}: *mut i32) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// LOWERING-X86_64-GNU-DAG: pub unsafe extern "C-unwind" fn deref_and_add({{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// LOWERING-AARCH64-GNU-DAG: pub unsafe extern "C-unwind" fn deref_and_add({{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-X86_64-GNU-DAG: pub unsafe extern "C-unwind" fn deref_and_add({{arg[0-9]+}}: *mut i32) -> i32 {
// REWRITES-X86_64-GNU-DAG:     (unsafe { *{{arg[0-9]+}} }) + 1
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-AARCH64-GNU-DAG: pub unsafe extern "C-unwind" fn deref_and_add({{arg[0-9]+}}: *mut i32) -> i32 {
// REWRITES-AARCH64-GNU-DAG:     (unsafe { *{{arg[0-9]+}} }) + 1
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
