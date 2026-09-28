/* PR tree-optimization/92618 */

typedef long long __m128i
    __attribute__((__vector_size__(2 * sizeof(long long)), __may_alias__));

double             a[4];
unsigned long long b[4];

__attribute__((noipa)) __m128i bar(void) {
  static int cnt;
  cnt += 2;
  return (__m128i){cnt, cnt + 1};
}

#if __SIZEOF_LONG_LONG__ == __SIZEOF_DOUBLE__
typedef double __m128d
    __attribute__((__vector_size__(2 * sizeof(double)), __may_alias__));

__attribute__((noipa)) __m128i qux(void) {
  static double cnt;
  cnt += 2.0;
  return (__m128i)(__m128d){cnt, cnt + 1.0};
}
#endif

void foo(unsigned long long *x) {
  __m128i c         = bar();
  __m128i d         = bar();
  *(__m128i *)&b[0] = c;
  *(__m128i *)&b[2] = d;
  *x                = b[0] + b[1] + b[2] + b[3];
}

void baz(double *x) {
#if __SIZEOF_LONG_LONG__ == __SIZEOF_DOUBLE__
  __m128i c         = qux();
  __m128i d         = qux();
  *(__m128i *)&a[0] = c;
  *(__m128i *)&a[2] = d;
  *x                = a[0] + a[1] + a[2] + a[3];
#endif
}

int main() {
  unsigned long long c = 0;
  foo(&c);
  if (c != 2 + 3 + 4 + 5)
    __builtin_abort();
#if __SIZEOF_LONG_LONG__ == __SIZEOF_DOUBLE__
  double d = 0.0;
  baz(&d);
  if (d != 2.0 + 3.0 + 4.0 + 5.0)
    __builtin_abort();
#endif
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: ⚠ unknown attribute 'noipa' ignored
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/pr92618.c:9:16]
// DEFAULT: 8 │
// DEFAULT: 9 │ __attribute__((noipa)) __m128i bar(void) {
// DEFAULT: ·                ─────
// DEFAULT: 10 │   static int cnt;
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: target builtin type
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/pr92618.c:9:1]
// DEFAULT: 8 │
// DEFAULT: 9 │ ╭─▶ __attribute__((noipa)) __m128i bar(void) {
// DEFAULT: 10 │ │     static int cnt;
// DEFAULT: 11 │ │     cnt += 2;
// DEFAULT: 12 │ │     return (__m128i){cnt, cnt + 1};
// DEFAULT: 13 │ ╰─▶ }
// DEFAULT: 14 │
// DEFAULT: ╰────
// DEFAULT: ⚠ unknown attribute 'noipa' ignored
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/pr92618.c:19:16]
// DEFAULT: 18 │
// DEFAULT: 19 │ __attribute__((noipa)) __m128i qux(void) {
// DEFAULT: ·                ─────
// DEFAULT: 20 │   static double cnt;
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: target builtin type
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/pr92618.c:19:1]
// DEFAULT: 18 │
// DEFAULT: 19 │ ╭─▶ __attribute__((noipa)) __m128i qux(void) {
// DEFAULT: 20 │ │     static double cnt;
// DEFAULT: 21 │ │     cnt += 2.0;
// DEFAULT: 22 │ │     return (__m128i)(__m128d){cnt, cnt + 1.0};
// DEFAULT: 23 │ ╰─▶ }
// DEFAULT: 24 │     #endif
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
