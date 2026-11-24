static int check_rotate(unsigned _BitInt(17) value, unsigned long long count) {
  unsigned left  = __builtin_stdc_rotate_left(value, count);
  unsigned right = __builtin_stdc_rotate_right(value, count);
  return (left != 1) + 2 * (right != 1);
}

int main(void) {
  return check_rotate(1, 0xffffffffffffffffULL);
}
