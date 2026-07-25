__float128 add(__float128 a, __float128 b) { return a + b; }

int main(void) {
  __float128 one = 1.0Q;
  __float128 tiny = 0x1p-100Q;
  __float128 sum = add(one, tiny);
  if (sum == one)
    return 1;
  if (sum - one != tiny)
    return 2;
  if ((__float128)42 != 42.0Q)
    return 3;
  if ((int)42.75Q != 42)
    return 4;
  if ((double)1.5Q != 1.5)
    return 5;
  if (sizeof(__float128) != 16)
    return 6;
  if (_Alignof(__float128) != 16)
    return 7;
  return 0;
}
