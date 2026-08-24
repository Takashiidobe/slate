#ifdef FAIL_BUILD
#error selected failure
#endif

int main(void) { return 0; }
// DIRECTIVES-DAG: #[cfg(feature = "fail_build")]
// DIRECTIVES-DAG: compile_error!("selected failure");
