int main(void) {
  int storage[4] = {0};
  int *pointer = storage;
  unsigned char *bytes = (unsigned char *)&storage;

  int *old = __atomic_fetch_add(&pointer, 1, __ATOMIC_RELAXED);
  int failed_add = old != storage || (unsigned char *)pointer != bytes + 1;

  pointer = storage + 2;
  int *updated = __atomic_sub_fetch(&pointer, 1, __ATOMIC_RELAXED);
  int failed_sub = (unsigned char *)updated != bytes + 2 * sizeof(int) - 1 ||
                   updated != pointer;
  return failed_add + 2 * failed_sub;
}
