static int base = 100;
static int local(int x) { return x * base; }

int compute(int x) { return local(x) + 1; }
