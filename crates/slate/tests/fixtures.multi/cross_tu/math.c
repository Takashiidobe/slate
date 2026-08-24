int square(int x) { return x * x; }

int cube(int x) { return square(x) * x; }
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: let mut __retval
// LOWERING-LABEL: {{^}}pub extern "C" fn square(
// LOWERING-DAG: _v{{[0-9]+}} * _v{{[0-9]+}}
// LOWERING: {{^}}}
// LOWERING-LABEL: {{^}}pub extern "C" fn cube(
// LOWERING-DAG: square(_v{{[0-9]+}})
// LOWERING: {{^}}}
