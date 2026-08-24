#ifdef HIDDEN_API
#pragma GCC visibility push(hidden)
#endif

int visibility_after_endif(void) { return 37; }

#ifdef HIDDEN_API
#pragma GCC visibility pop
#endif

int main(void) { return visibility_after_endif() == 37 ? 0 : 1; }
// DIRECTIVES-DAG: #[cfg(feature = "hidden_api")]
// DIRECTIVES-DAG: compile_error!("unsupported semantic directive #pragma at line 2: GCC visibility push(hidden)");
