static int calls = 10;

static int helper(int x) { return x * 2; }

int ng_free_value(int x) {
  calls = calls + 1;
  return helper(x) + calls;
}
