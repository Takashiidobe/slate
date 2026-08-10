int run_a(void) {
  struct Item {
    int value;
    int weight;
  };

  struct Item items[] = {{1, 10}, {2, 20}};

  int total = 0;
  for (int i = 0; i < 2; i++) {
    total += items[i].value * items[i].weight;
  }
  return total;
}
