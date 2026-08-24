int read_ptr(int *p) { return *p; }
// LOWERING-LABEL: {{^}}pub unsafe extern "C" fn read_ptr(
// LOWERING-DAG: unsafe { *_v{{[0-9]+}} }
// LOWERING: {{^}}}
