static int base = 100;
static int local(int x) { return x * base; }

// @rewrite-fn-begin
int compute(int x) { return local(x) + 1; }
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn local({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: let mut x: i32 = {{arg[0-9]+}};
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: __retval = x * unsafe { base };
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C" fn compute({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: let mut x: i32 = {{arg[0-9]+}};
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = local(x);
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-DAG: __retval = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// LOWERING-DAG: {{^}}static mut base: i32 = 100;
// LOWERING-NOT: pub static mut base
// LOWERING-LABEL: {{^}}fn local(
// LOWERING: {{^}}}
// LOWERING-LABEL: {{^}}pub extern "C" fn compute(
// LOWERING: {{^}}}
