/* PR tree-optimization/126503 */
/* { dg-do run { target bitint575 } } */

[[gnu::noipa]] unsigned _BitInt(400) foo(_BitInt(257) a) {
  // @lowering-begin
  // @rewrite-begin
  return (unsigned _BitInt(400))(-a);
  // @rewrite-end
  // @lowering-end
}

[[gnu::noipa]] unsigned _BitInt(300) bar(_BitInt(7) a, unsigned _BitInt(17) b) {
  // @lowering-begin
  // @rewrite-begin
  _BitInt(257) x = (_BitInt(257))a + -1;
  return ~((unsigned _BitInt(300))x ^ (unsigned _BitInt(300))b);
  // @rewrite-end
  // @lowering-end
}

[[gnu::noipa]] _BitInt(129) baz(_BitInt(129) x) {
  // @lowering-begin
  // @rewrite-begin
  return x - 24;
  // @rewrite-end
  // @lowering-end
}

[[gnu::noipa]] int qux(_BitInt(129) x, _BitInt(129) y) {
  // @lowering-begin
  // @rewrite-begin
  return x == y;
  // @rewrite-end
  // @lowering-end
}

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
  if (foo(-1wb) != 1uwb)
    __builtin_abort();
  if (bar(1wb, 3uwb) != (unsigned _BitInt(300)) - 4wb)
    __builtin_abort();
  if (!qux(baz(100), 76))
    __builtin_abort();
}
// @rewrite-fn-end
// @lowering-fn-end
