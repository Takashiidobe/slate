static _BitInt(17) abs17(_BitInt(17) value) {
  return __builtin_elementwise_abs(value);
}

static _BitInt(65) abs65(_BitInt(65) value) {
  return __builtin_elementwise_abs(value);
}

int main(void) {
  _BitInt(65) high = (_BitInt(65))1 << 63;
  return abs17(-65535) != 65535 || abs17(-65536) != -65536 ||
         abs65(-high) != high;
}
