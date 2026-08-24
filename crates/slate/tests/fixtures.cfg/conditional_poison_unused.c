#ifdef STRICT_NAMES
#pragma GCC poison forbidden_identifier
#endif

int main(void) { return 0; }
// DIRECTIVES-NOT: compile_error!
