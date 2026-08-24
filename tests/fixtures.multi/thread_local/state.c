_Thread_local int shared_value = 5;

int read_shared_value(void) { return shared_value; }
// LOWERING: #[thread_local]
// LOWERING-NEXT: pub static mut shared_value: i32 = 5;
