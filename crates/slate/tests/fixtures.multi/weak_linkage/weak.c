int weak_global __attribute__((weak)) = 13;

int __attribute__((weak)) fallback_value(void) { return weak_global; }
// LOWERING-DAG: #![feature(linkage)]
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: #[linkage = "weak"]
// LOWERING-DAG: pub static mut weak_global: i32 = 13;
// LOWERING-LABEL: {{^}}pub extern "C" fn fallback_value(
// LOWERING: {{^}}}
