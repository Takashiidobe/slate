int counter __attribute__((section(".slate_data"))) = 0;

int unreferenced_global = 7;

void __attribute__((section(".slate_fn"))) bump(int by) { counter += by; }
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING: #[unsafe(link_section = ".slate_data")]
// LOWERING-NEXT: pub static mut counter: i32 = 0;
// LOWERING-DAG: pub static mut unreferenced_global: i32 = 7;
// LOWERING: #[unsafe(link_section = ".slate_fn")]
// LOWERING-NEXT: pub extern "C" fn bump(
