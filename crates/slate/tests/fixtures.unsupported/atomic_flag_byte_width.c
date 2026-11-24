int main(void) {
  unsigned word = 0;
  unsigned char *bytes = (unsigned char *)&word;
  bytes[1] = 1;

  int old = __atomic_test_and_set(&word, __ATOMIC_RELAXED);
  int failed_set = old != 0 || bytes[0] == 0 || bytes[1] != 1;

  bytes[0] = 1;
  bytes[1] = 1;
  __atomic_clear(&word, __ATOMIC_RELAXED);
  int failed_clear = bytes[0] != 0 || bytes[1] != 1;
  return failed_set + 2 * failed_clear;
}
