static int used_only __attribute__((used)) = 11;
static int used_and_retained __attribute__((used, retain)) = 22;
static int retain_only __attribute__((retain)) = 33;

int main(void) { return 0; }
