static int base = 100;
static int local(int x) { return x * base; }

// @rewrite-fn-begin
int compute(int x) { return local(x) + 1; }
// @rewrite-fn-end

// LOWERING-DAG: {{^}}static mut base: i32 = 100;
// LOWERING-NOT: pub static mut base
// LOWERING-LABEL: {{^}}fn local(
// LOWERING: {{^}}}
// LOWERING-LABEL: {{^}}pub extern "C-unwind" fn compute(
// LOWERING: {{^}}}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C-unwind" fn compute({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = local({{arg[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-X86_64-GNU-DAG: pub extern "C-unwind" fn compute({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = local({{arg[0-9]+}});
// REWRITES-X86_64-GNU-DAG:     {{__v[0-9]+}} + 1
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-AARCH64-GNU-DAG: pub extern "C-unwind" fn compute({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = local({{arg[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:     {{__v[0-9]+}} + 1
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
