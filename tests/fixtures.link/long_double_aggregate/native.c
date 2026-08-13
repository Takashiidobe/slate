struct ld_pair {
  long double a;
  long double b;
};

struct ld_pair ext_pair_roundtrip(struct ld_pair p) {
  p.a += 1.0L;
  p.b += 2.0L;
  return p;
}
