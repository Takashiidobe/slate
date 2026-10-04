struct mixed {
  char tag : 4;
  int value : 12;
  char flags : 2;
};

int mixed_size(void) { return (int)sizeof(struct mixed); }

int pack(int tag, int value, int flags) {
  struct mixed m = { tag, value, flags };
  return m.tag + m.value + m.flags;
}
