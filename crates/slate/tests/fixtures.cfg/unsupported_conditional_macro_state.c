#define MACRO_STATE_VALUE 5

#ifdef NESTED_MACRO_STATE
#pragma push_macro("MACRO_STATE_VALUE")
#undef MACRO_STATE_VALUE
#define MACRO_STATE_VALUE 13
#endif

static int value_after_endif = MACRO_STATE_VALUE;

#ifdef NESTED_MACRO_STATE
#pragma pop_macro("MACRO_STATE_VALUE")
#endif

int main(void) { return value_after_endif == MACRO_STATE_VALUE ? 0 : 1; }
