/* PR tree-optimization/119707 */
/* { dg-do run { target bitint } } */

#if __BITINT_MAXWIDTH__ >= 256
__attribute__((noipa)) unsigned _BitInt(256)
    foo(unsigned _BitInt(256) x, _BitInt(129) y) {
  // @lowering-begin
  // @rewrite-begin
  return x + (unsigned _BitInt(255))y;
  // @rewrite-end
  // @lowering-end
}
#endif

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
#if __BITINT_MAXWIDTH__ >= 256
  if (foo(0, -1) !=
      0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffuwb)
    __builtin_abort();
#endif
}
// @rewrite-fn-end
// @lowering-fn-end
