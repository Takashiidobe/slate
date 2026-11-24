static int calls;
static unsigned seen;

static double value(double result) {
  ++calls;
  return result;
}

static int code(int result) {
  seen |= 1u << result;
  return result;
}

static int lessgreater(void) {
  return __builtin_islessgreater(value(1.0), value(2.0));
}

static int inf_sign(void) {
  return __builtin_isinf_sign(value(-__builtin_inf()));
}

static int classify(void) {
  return __builtin_fpclassify(code(1), code(2), code(3), code(4), code(5),
                              value(1.0));
}

int main(void) {
  int result = lessgreater();
  int failed = !result || calls != 2;
  calls = 0;
  result = inf_sign();
  failed |= 2 * (result != -1 || calls != 1);
  calls = 0;
  result = classify();
  failed |= 4 * (result != 3 || calls != 1 || seen != 62);
  return failed;
}
