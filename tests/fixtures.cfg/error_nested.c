#ifdef OUTER_FAILURE
#ifdef INNER_FAILURE
#error nested failure
#endif
#endif

int main(void) { return 0; }
// DIRECTIVES-DAG: #[cfg(all(feature = "outer_failure", feature = "inner_failure"))]
// DIRECTIVES-DAG: compile_error!("nested failure");
