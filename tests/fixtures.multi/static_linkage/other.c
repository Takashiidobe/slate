static int base = 100;
static int local(int x) { return x * base; }

// @rewrite-fn-begin
int compute(int x) { return local(x) + 1; }
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn local({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: return {{arg[0-9]+}} * unsafe { base };
// REWRITES-DAG: }
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C" fn compute({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = local({{arg[0-9]+}});
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-DAG: return {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// LOWERING-DAG: {{^}}static mut base: i32 = 100;
// LOWERING-NOT: pub static mut base
// LOWERING-LABEL: {{^}}fn local(
// LOWERING: {{^}}}
// LOWERING-LABEL: {{^}}pub extern "C" fn compute(
// LOWERING: {{^}}}
