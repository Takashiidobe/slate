// @lowering-fn-begin
int square(int x) { return x * x; }
// @lowering-fn-end

// @lowering-fn-begin
int cube(int x) { return square(x) * x; }
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub extern "C" fn square({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub extern "C" fn cube({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = square({{arg[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{arg[0-9]+}};
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering
