struct Bits {
  unsigned long long field : 33;
};

static unsigned long long increment(const struct Bits *value) {
  return value->field + 1;
}

static unsigned long long decrement(const struct Bits *value) {
  return value->field - 1;
}

int main(void) {
  struct Bits full = {0x1ffffffffULL};
  struct Bits empty = {0};
  return (increment(&full) != 0) + 2 * (decrement(&empty) != 0x1ffffffffULL);
}
