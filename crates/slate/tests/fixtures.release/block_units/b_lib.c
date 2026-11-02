// { dg-options "-fblocks" }
typedef int (^intfn)(int);

extern void *_Block_copy(const void *);

int apply_twice(intfn f, int x) { return f(f(x)); }

intfn make_offset(int k) {
  return _Block_copy(^(int x) { return x + k; });
}

void for_each_squared(int n, void (^visit)(int)) {
  for (int i = 1; i <= n; i++) visit(i * i);
}
