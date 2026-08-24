static int base = 100;
static int local(int x) { return x * base; }

int compute(int x) { return local(x) + 1; }
// LOWERING-DAG: {{^}}static mut base: i32 = 100;
// LOWERING-NOT: pub static mut base
// LOWERING-LABEL: {{^}}fn local(
// LOWERING: {{^}}}
// LOWERING-LABEL: {{^}}pub extern "C" fn compute(
// LOWERING: {{^}}}
