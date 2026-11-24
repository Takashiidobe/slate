static int calls;

static int ordering(void) {
  ++calls;
  return __ATOMIC_RELAXED;
}

static int fetch_add(int *value) {
  return __atomic_fetch_add(value, 2, ordering());
}

static int load(int *value) {
  return __atomic_load_n(value, ordering());
}

int main(void) {
  int value = 7;
  int before = fetch_add(&value);
  int failed_update = before != 7 || calls != 1;
  int after = load(&value);
  int failed_load = after != 9 || calls != 2;
  return failed_update + 2 * failed_load;
}
