int bump(int value, ...);

int main(void) { return bump(41, 0) != 42; }
// LOWERING-LABEL: {{^}}fn main() {
// LOWERING-DAG: unsafe { bump(
// LOWERING: {{^}}}
