int pragma_weak_target(int value) { return value + 7; }

#pragma weak pragma_weak_alias = pragma_weak_target
extern int   pragma_weak_alias(int);

#pragma redefine_extname pragma_renamed pragma_actual
int pragma_renamed(int value) { return value * 3; }
// LOWERING-DAG: .weak pragma_weak_alias\n.set pragma_weak_alias, pragma_weak_target
// LOWERING-LABEL: {{^}}pub extern "C" fn pragma_weak_target(
// LOWERING: {{^}}}
// LOWERING-LABEL: {{^}}pub extern "C" fn pragma_actual(
// LOWERING: {{^}}}
