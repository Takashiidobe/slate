#ifdef FAIL_BUILD
#error selected failure
#endif

int main(void) { return 0; }

// COMMON-NOT: compile_error!
