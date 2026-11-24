typedef unsigned v4u __attribute__((vector_size(16)));

static v4u leading(v4u value, v4u fallback) {
  return __builtin_elementwise_clzg(value, fallback);
}

static v4u trailing(v4u value, v4u fallback) {
  return __builtin_elementwise_ctzg(value, fallback);
}

int main(void) {
  v4u values = {0, 1, 0x80000000U, 0};
  v4u fallback = {11, 22, 33, 44};
  v4u left = leading(values, fallback);
  v4u right = trailing(values, fallback);
  int bad_left = left[0] != 11 || left[1] != 31 || left[2] != 0 || left[3] != 44;
  int bad_right = right[0] != 11 || right[1] != 0 || right[2] != 31 || right[3] != 44;
  return bad_left + 2 * bad_right;
}
