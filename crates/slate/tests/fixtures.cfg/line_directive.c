#line 700 "virtual-line.c"
#ifdef LINE_FEATURE
int line_value(void) { return __LINE__; }
int expected_line(void) { return 701; }
#else
int line_value(void) { return __LINE__; }
int expected_line(void) { return 704; }
#endif

int main(void) { return line_value() == expected_line() ? 0 : 1; }
