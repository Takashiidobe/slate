// @lowering-fn-begin
int square(int x) { return x * x; }
// @lowering-fn-end

// @lowering-fn-begin
int cube(int x) { return square(x) * x; }
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub extern "C-unwind" fn square({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub extern "C-unwind" fn cube({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = square({{arg[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{arg[0-9]+}};
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// LOWERING-X86_64-GNU-DAG: pub extern "C-unwind" fn square({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG: }
// LOWERING-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// LOWERING-X86_64-GNU-DAG: pub extern "C-unwind" fn cube({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = square({{arg[0-9]+}});
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{arg[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// LOWERING-AARCH64-GNU-DAG: pub extern "C-unwind" fn square({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG: }
// LOWERING-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// LOWERING-AARCH64-GNU-DAG: pub extern "C-unwind" fn cube({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = square({{arg[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END lowering-aarch64-gnu
