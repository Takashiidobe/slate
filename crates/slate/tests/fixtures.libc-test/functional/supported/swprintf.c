#define _GNU_SOURCE 1

#include <stdint.h>
#include <unistd.h>

/* TODO: not thread-safe nor fork-safe */
extern volatile int t_status;

#define T_LOC2(l)    __FILE__ ":" #l
#define T_LOC1(l)    T_LOC2(l)
#define t_error(...) t_printf(T_LOC1(__LINE__) ": " __VA_ARGS__)

int t_printf(const char *s, ...);

int t_vmfill(void **, size_t *, int);
int t_memfill(void);

void t_fdfill(void);

void     t_randseed(uint64_t s);
uint64_t t_randn(uint64_t n);
uint64_t t_randint(uint64_t a, uint64_t b);
void     t_shuffle(uint64_t *p, size_t n);
void     t_randrange(uint64_t *p, size_t n);
int      t_choose(uint64_t n, size_t k, uint64_t *p);

char *t_pathrel(char *buf, size_t n, char *argv0, char *p);

int t_setrlim(int r, long lim);

int t_setutf8(void);

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

volatile int t_status = 0;

int t_printf(const char *s, ...) {
  va_list ap;
  char    buf[512];
  int     n;

  t_status = 1;
  va_start(ap, s);
  n = vsnprintf(buf, sizeof buf, s, ap);
  va_end(ap);
  if (n < 0)
    n = 0;
  else if (n >= sizeof buf) {
    n          = sizeof buf;
    buf[n - 1] = '\n';
    buf[n - 2] = '.';
    buf[n - 3] = '.';
    buf[n - 4] = '.';
  }
  return write(1, buf, n);
}

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#include <errno.h>
#include <langinfo.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define TEST(r, f, x, m)                                                       \
  (((r) = (f)) == (x) || (t_error("%s failed (" m ")\n", #f, r, x), 0))

#define TEST_S(s, x, m)                                                        \
  (!wcscmp((s), (x)) || (t_error("[%ls] != [%ls] (%s)\n", s, x, m), 0))

static const struct {
  const wchar_t *fmt;
  int            i;
  const wchar_t *expect;
} int_tests[] = {
    /* width, precision, alignment */
    {L"%04d", 12, L"0012"},
    {L"%.3d", 12, L"012"},
    {L"%3d", 12, L" 12"},
    {L"%-3d", 12, L"12 "},
    {L"%+3d", 12, L"+12"},
    {L"%+-5d", 12, L"+12  "},
    {L"%+- 5d", 12, L"+12  "},
    {L"%- 5d", 12, L" 12  "},
    {L"% d", 12, L" 12"},
    {L"%0-5d", 12, L"12   "},
    {L"%-05d", 12, L"12   "},

    /* ...explicit precision of 0 shall be no characters except for alt-octal.
     */
    {L"%.0d", 0, L""},
    {L"%.0o", 0, L""},
    {L"%#.0d", 0, L""},
    {L"%#.0o", 0, L"0"},
    {L"%#.0x", 0, L""},

    /* hex: test alt form and case */
    {L"%x", 63, L"3f"},
    {L"%#x", 63, L"0x3f"},
    {L"%X", 63, L"3F"},

    /* octal: test alt form */
    {L"%o", 15, L"17"},
    {L"%#o", 15, L"017"},

    {NULL, 0.0, NULL}};

static const struct {
  const wchar_t *fmt;
  double         f;
  const wchar_t *expect;
} fp_tests[] = {
    /* basic form, handling of exponent/precision for 0 */
    {L"%e", 0.0, L"0.000000e+00"},
    {L"%f", 0.0, L"0.000000"},
    {L"%g", 0.0, L"0"},
    {L"%#g", 0.0, L"0.00000"},

    /* rounding */
    {L"%f", 1.1, L"1.100000"},
    {L"%f", 1.2, L"1.200000"},
    {L"%f", 1.3, L"1.300000"},
    {L"%f", 1.4, L"1.400000"},
    {L"%f", 1.5, L"1.500000"},

    /* correctness in DBL_DIG places */
    {L"%.15g", 1.23456789012345, L"1.23456789012345"},

    /* correct choice of notation for %g */
    {L"%g", 0.0001, L"0.0001"},
    {L"%g", 0.00001, L"1e-05"},
    {L"%g", 123456, L"123456"},
    {L"%g", 1234567, L"1.23457e+06"},
    {L"%.7g", 1234567, L"1234567"},
    {L"%.7g", 12345678, L"1.234568e+07"},

    /* pi in double precision, printed to a few extra places */
    {L"%.15f", M_PI, L"3.141592653589793"},
    {L"%.18f", M_PI, L"3.141592653589793116"},

    /* exact conversion of large integers */
    {L"%.0f", 340282366920938463463374607431768211456.0,
     L"340282366920938463463374607431768211456"},

    {NULL, 0.0, NULL}};

int main(void) {
  int     i, j;
  wchar_t b[500];

  (void)(setlocale(LC_CTYPE, "en_US.UTF-8") ||
         setlocale(LC_CTYPE, "en_GB.UTF-8") ||
         setlocale(LC_CTYPE, "en.UTF-8") ||
         setlocale(LC_CTYPE, "POSIX.UTF-8") || setlocale(LC_CTYPE, "C.UTF-8") ||
         setlocale(LC_CTYPE, "UTF-8") || setlocale(LC_CTYPE, ""));

  TEST(i, strcmp(nl_langinfo(CODESET), "UTF-8"), 0,
       "no UTF-8 locale; tests might fail");

  TEST(i, swprintf(0, 0, L"%d", 123456) < 0, 1, "%d != %d");

  TEST(i, swprintf(b, 2, L"%lc", 0xc0), 1, "%d != %d");
  TEST(i, b[0], 0xc0, "wrong character %x != %x");
  TEST(i, swprintf(b, 2, L"%lc", 0x20ac), 1, "%d != %d");
  TEST(i, b[0], 0x20ac, "wrong character %x != %x");
  TEST(i, swprintf(b, 3, L"%s", "\xc3\x80!"), 2, "%d != %d");
  TEST(i, b[0], 0xc0, "wrong character %x != %x");
  TEST(i, swprintf(b, 2, L"%.1s", "\xc3\x80!"), 1, "%d != %d");
  TEST(i, b[0], 0xc0, "wrong character %x != %x");

  wcscpy(b, L"xxxxxxxx");
  TEST(i, swprintf(b, 4, L"%d", 123456) < 0, 1, "%d != %d");
  TEST_S(b, L"123", "incorrect output");
  TEST(i, b[5], 'x', "buffer overrun");

  for (j = 0; int_tests[j].fmt; j++) {
    i = swprintf(b, sizeof b, int_tests[j].fmt, int_tests[j].i);
    if (i != wcslen(int_tests[j].expect)) {
      t_error("swprintf(b, sizeof b, \"%ls\", %d) returned %d wanted %d\n",
              int_tests[j].fmt, int_tests[j].i, i, wcslen(int_tests[j].expect));
    }
    if (wcscmp(b, int_tests[j].expect) != 0)
      t_error("bad integer conversion: got \"%ls\", want \"%ls\"\n", b,
              int_tests[j].expect);
  }

  for (j = 0; fp_tests[j].fmt; j++) {
    i = swprintf(b, sizeof b, fp_tests[j].fmt, fp_tests[j].f);
    if (i != wcslen(fp_tests[j].expect)) {
      t_error("swprintf(b, sizeof b, \"%ls\", %f) returned %d wanted %d\n",
              fp_tests[j].fmt, fp_tests[j].f, i, wcslen(fp_tests[j].expect));
    }
    if (wcscmp(b, fp_tests[j].expect) != 0)
      t_error("bad floating-point conversion: got \"%ls\", want \"%ls\"\n", b,
              fp_tests[j].expect);
  }
  return t_status;
}
