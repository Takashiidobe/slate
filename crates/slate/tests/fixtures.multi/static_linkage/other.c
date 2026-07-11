// same names as in `main.c`, but internal linkage keeps them distinct.
static int base = 100;
static int local(int x) { return x * base; }

// external-linkage function: this is the one `main.c` imports.
int compute(int x) { return local(x) + 1; }
