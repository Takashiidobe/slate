// @lowering-fn-begin
// @rewrite-fn-begin
int π(int value) { return value * 2 + 1; }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(export_name = "\u{3c0}")]
// LOWERING-DAG: pub extern "C-unwind" fn π({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(export_name = "\u{3c0}")]
// REWRITES-DAG: pub extern "C-unwind" fn π({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     {{arg[0-9]+}} * 2 + 1
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: #[unsafe(export_name = "\u{3c0}")]
// LOWERING-X86_64-GNU-DAG: pub extern "C-unwind" fn π({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: #[unsafe(export_name = "\u{3c0}")]
// LOWERING-AARCH64-GNU-DAG: pub extern "C-unwind" fn π({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} * {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: #[unsafe(export_name = "\u{3c0}")]
// REWRITES-X86_64-GNU-DAG: pub extern "C-unwind" fn π({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-X86_64-GNU-DAG:     {{arg[0-9]+}} * 2 + 1
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: #[unsafe(export_name = "\u{3c0}")]
// REWRITES-AARCH64-GNU-DAG: pub extern "C-unwind" fn π({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-AARCH64-GNU-DAG:     {{arg[0-9]+}} * 2 + 1
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
