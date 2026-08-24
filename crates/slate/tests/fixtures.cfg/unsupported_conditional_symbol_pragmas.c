#ifdef SYMBOL_PRAGMAS
#pragma weak conditional_alias = conditional_target
#pragma redefine_extname conditional_name conditional_actual
#endif

int conditional_target(void) { return 41; }
int conditional_name(void) { return 43; }

int main(void) { return conditional_name() == 43 ? 0 : 1; }
// DIRECTIVES-DAG: #[cfg(feature = "symbol_pragmas")]
// DIRECTIVES-DAG: compile_error!("unsupported semantic directive #pragma at line 2: weak conditional_alias = conditional_target");
// DIRECTIVES-DAG: compile_error!("unsupported semantic directive #pragma at line 3: redefine_extname conditional_name conditional_actual");
