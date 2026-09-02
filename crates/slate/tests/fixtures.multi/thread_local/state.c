_Thread_local int shared_value = 5;

// @rewrite-fn-begin
int read_shared_value(void) { return shared_value; }
// @rewrite-fn-end

// LOWERING: #[thread_local]
// LOWERING-NEXT: pub static mut shared_value: i32 = 5;

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C" fn read_shared_value() -> i32 {
// REWRITES-DAG:     return unsafe { shared_value };
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
