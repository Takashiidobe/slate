int call_root_helper(void);

int root_helper(void) { return 42; }

int main(void) { return call_root_helper() != 42; }
