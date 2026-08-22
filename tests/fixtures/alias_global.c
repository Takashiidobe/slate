int        real_global = 12;
extern int alias_global __attribute__((alias("real_global")));

int main(void) { return alias_global; }
