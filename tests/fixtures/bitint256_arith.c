void abort(void);

static _BitInt(256) add256(_BitInt(256) a, _BitInt(256) b) { return a + b; }

static unsigned _BitInt(256)
    mul256(unsigned _BitInt(256) a, unsigned _BitInt(256) b) {
  return a * b;
}

int main(void) {
  _BitInt(256) x   = 12345;
  _BitInt(256) y   = -300;
  _BitInt(256) sum = add256(x, y);
  if (sum != 12045)
    abort();

  unsigned _BitInt(256) p    = 1000000000000ULL;
  unsigned _BitInt(256) q    = 3;
  unsigned _BitInt(256) prod = mul256(p, q);
  if (prod != 3000000000000ULL)
    abort();

  _BitInt(256) neg = -x;
  if (neg != -12345)
    abort();

  _BitInt(256) diff = x - y;
  if (diff != 12645)
    abort();

  if (!(y < x))
    abort();
  if (!(x > y))
    abort();

  return 0;
}
