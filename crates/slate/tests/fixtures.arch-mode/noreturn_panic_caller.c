__attribute__((noreturn)) void panic(const char *fmt, ...);

static int slots[4];
static int count;

int push_slot(int value) {
  if (count == 4)
    panic("slots full: %d\n", value);
  slots[count++] = value;
  return count;
}
