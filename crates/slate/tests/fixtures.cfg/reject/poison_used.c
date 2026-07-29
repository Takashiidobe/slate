#pragma GCC poison forbidden_identifier

int forbidden_identifier(void) { return 47; }

int main(void) { return forbidden_identifier(); }
