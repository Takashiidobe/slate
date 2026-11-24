_Alignas(16) static char buffer[16];
static int calls;

static long alignment_offset(void) {
  ++calls;
  return 1;
}

int main(void) {
  char *interior = buffer + 1;
  char *result = __builtin_assume_aligned(interior, 16, alignment_offset());
  return (result != interior) + 2 * (calls != 1);
}
