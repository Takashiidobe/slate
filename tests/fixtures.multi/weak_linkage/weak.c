int weak_global __attribute__((weak)) = 13;

// @rewrite-fn-begin
int __attribute__((weak)) fallback_value(void) { return weak_global; }
// @rewrite-fn-end

// LOWERING-DAG: #![feature(linkage)]
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: #[linkage = "weak"]
// LOWERING-DAG: pub static mut weak_global: i32 = 13;
// LOWERING-LABEL: {{^}}pub extern "C-unwind" fn fallback_value(
// LOWERING: {{^}}}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: #[linkage = "weak"]
// REWRITES-DAG: pub extern "C-unwind" fn fallback_value() -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { weak_global };
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-X86_64-GNU-DAG: #[linkage = "weak"]
// REWRITES-X86_64-GNU-DAG: pub extern "C-unwind" fn fallback_value() -> i32 {
// REWRITES-X86_64-GNU-DAG:     unsafe { weak_global }
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-AARCH64-GNU-DAG: #[linkage = "weak"]
// REWRITES-AARCH64-GNU-DAG: pub extern "C-unwind" fn fallback_value() -> i32 {
// REWRITES-AARCH64-GNU-DAG:     unsafe { weak_global }
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
