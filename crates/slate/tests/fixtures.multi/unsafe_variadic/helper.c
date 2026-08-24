int bump(int value, ...) { return value + 1; }
// LOWERING-LABEL: {{^}}pub unsafe extern "C" fn bump(
// LOWERING: {{^}}}
