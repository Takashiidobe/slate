int weak_global __attribute__((weak)) = 13;

// @rewrite-fn-begin
int __attribute__((weak)) fallback_value(void) { return weak_global; }
// @rewrite-fn-end

// LOWERING-DAG: #![feature(linkage)]
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: #[linkage = "weak"]
// LOWERING-DAG: pub static mut weak_global: i32 = 13;
// LOWERING-LABEL: {{^}}pub extern "C" fn fallback_value(
// LOWERING: {{^}}}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: #[linkage = "weak"]
// REWRITES-DAG: pub extern "C" fn fallback_value() -> i32 {
// REWRITES-DAG:     return unsafe { weak_global };
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
