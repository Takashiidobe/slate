static _BitInt(65) signed_max(_BitInt(65) left, _BitInt(65) right) {
  return __builtin_elementwise_max(left, right);
}

static unsigned _BitInt(65) unsigned_min(unsigned _BitInt(65) left,
                                         unsigned _BitInt(65) right) {
  return __builtin_elementwise_min(left, right);
}

int main(void) {
  _BitInt(65) signed_high = (_BitInt(65))1 << 63;
  unsigned _BitInt(65) unsigned_high = (unsigned _BitInt(65))1 << 64;
  return signed_max(-1, signed_high) != signed_high ||
         unsigned_min(unsigned_high, 1) != 1;
}
