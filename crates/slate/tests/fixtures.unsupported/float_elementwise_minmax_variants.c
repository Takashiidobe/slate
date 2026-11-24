static float minimum(float a, float b) {
  return __builtin_elementwise_minimum(a, b);
}

static float maximum(float a, float b) {
  return __builtin_elementwise_maximum(a, b);
}

static float minimum_number(float a, float b) {
  return __builtin_elementwise_minimumnum(a, b);
}

static float maximum_number(float a, float b) {
  return __builtin_elementwise_maximumnum(a, b);
}

int main(void) {
  float nan = __builtin_nanf("");
  int bad_min = !__builtin_isnan(minimum(nan, 2.0f)) ||
                !__builtin_signbit(minimum(0.0f, -0.0f));
  int bad_max = !__builtin_isnan(maximum(2.0f, nan)) ||
                __builtin_signbit(maximum(-0.0f, 0.0f));
  int bad_minnum = minimum_number(nan, 2.0f) != 2.0f ||
                   !__builtin_signbit(minimum_number(0.0f, -0.0f));
  int bad_maxnum = maximum_number(2.0f, nan) != 2.0f ||
                   __builtin_signbit(maximum_number(-0.0f, 0.0f));
  return bad_min + 2 * bad_max + 4 * bad_minnum + 8 * bad_maxnum;
}
