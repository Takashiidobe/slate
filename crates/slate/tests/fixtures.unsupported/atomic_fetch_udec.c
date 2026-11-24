static int check(unsigned start, unsigned limit, unsigned expected) {
  unsigned value = start;
  unsigned old = __atomic_fetch_udec(&value, limit, __ATOMIC_RELAXED);
  return old != start || value != expected;
}

int main(void) {
  return check(0, 7, 7) || check(8, 7, 7) || check(7, 7, 6) ||
         check(1, 7, 0) || check(0xffffffffu, 0x80000000u, 0x80000000u);
}
