static int calls;

static long expected(void) {
  ++calls;
  return 1;
}

static long plain(long value) {
  return __builtin_expect(value, expected());
}

static long probability(long value) {
  return __builtin_expect_with_probability(value, expected(), 0.5);
}

int main(void) {
  long value = plain(42);
  int plain_failed = value != 42 || calls != 1;
  value = probability(43);
  int probability_failed = value != 43 || calls != 2;
  return plain_failed + 2 * probability_failed;
}
