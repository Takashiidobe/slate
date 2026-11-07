static int calls;

static int helper(int x) { return x + 1; }

int free_value(int x) {
  calls = calls + 1;
  return helper(x) + calls;
}
