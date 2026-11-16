__attribute__((section("slate_cgu_data"))) int alpha_value = 3;
extern int alpha_alias __attribute__((alias("alpha_value")));

int alpha_read(void) { return alpha_alias; }
