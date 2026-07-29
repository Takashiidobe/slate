#ifdef SYMBOL_PRAGMAS
#pragma weak conditional_alias = conditional_target
#pragma redefine_extname conditional_name conditional_actual
#endif

int conditional_target(void) { return 41; }
int conditional_name(void) { return 43; }

int main(void) { return conditional_name() == 43 ? 0 : 1; }
