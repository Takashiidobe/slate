struct sizes {
  unsigned char long_bytes;
  unsigned char long_double_bytes;
  unsigned char pointer_bytes;
};

long scale(long value, long factor) { return value * factor; }

long double average(long double a, long double b) { return (a + b) / 2; }

struct sizes data_model(void) {
  struct sizes s = { sizeof(long), sizeof(long double), sizeof(void *) };
  return s;
}
