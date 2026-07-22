#ifdef OUTER_FAILURE
#ifdef INNER_FAILURE
#error nested failure
#endif
#endif

int main(void) { return 0; }
