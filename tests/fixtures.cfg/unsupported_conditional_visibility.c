#ifdef HIDDEN_API
#pragma GCC visibility push(hidden)
#endif

int visibility_after_endif(void) { return 37; }

#ifdef HIDDEN_API
#pragma GCC visibility pop
#endif

int main(void) { return visibility_after_endif() == 37 ? 0 : 1; }
