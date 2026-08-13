int run_b(void) {
  struct Item {
    const char *label;
    int         score;
  };

  struct Item items[] = {{"x", 3}, {"yy", 4}};

  int total = 0;
  for (int i = 0; i < 2; i++) {
    total += (int)items[i].label[0] * items[i].score;
  }
  return total;
}
