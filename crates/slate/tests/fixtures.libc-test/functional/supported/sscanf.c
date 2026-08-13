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

#include <limits.h>
#include <stdio.h>
#include <string.h>

#define TEST(r, f, x, m)                                                       \
  (((r) = (f)) == (x) || (t_error("%s failed (" m ")\n", #f, r, x), 0))

#define TEST_S(s, x, m)                                                        \
  (!strcmp((s), (x)) || (t_error("[%s] != [%s] (%s)\n", s, x, m), 0))

#define TEST_F(x)                                                              \
  (TEST(i, sscanf(#x, "%lf", &d), 1, "got %d fields, expected %d"),            \
   TEST(t, d, (double)x, "%g != %g"))

int main(void) {
  int    i;
  char   a[100], b[100];
  int    x, y, z, u, v;
  double d, t;

  TEST(i, sscanf("hello, world\n", "%s %s", a, b), 2,
       "only %d fields, expected %d");
  TEST_S(a, "hello,", "");
  TEST_S(b, "world", "");

  TEST(i, sscanf("hello, world\n", "%[hel]%s", a, b), 2,
       "only %d fields, expected %d");
  TEST_S(a, "hell", "");
  TEST_S(b, "o,", "");

  TEST(i, sscanf("hello, world\n", "%[hel] %s", a, b), 2,
       "only %d fields, expected %d");
  TEST_S(a, "hell", "");
  TEST_S(b, "o,", "");

  a[8] = 'X';
  a[9] = 0;
  TEST(i, sscanf("hello, world\n", "%8c%8c", a, b), 1,
       "%d fields, expected %d");
  TEST_S(a, "hello, wX", "");

  TEST(i, sscanf("56789 0123 56a72", "%2d%d%*d %[0123456789]\n", &x, &y, a), 3,
       "only %d fields, expected %d");
  TEST(i, x, 56, "%d != %d");
  TEST(i, y, 789, "%d != %d");
  TEST_S(a, "56", "");

  TEST(i,
       sscanf("011 0x100 11 0x100 100", "%i %i %o %x %x\n", &x, &y, &z, &u, &v),
       5, "only %d fields, expected %d");
  TEST(i, x, 9, "%d != %d");
  TEST(i, y, 256, "%d != %d");
  TEST(i, z, 9, "%d != %d");
  TEST(i, u, 256, "%d != %d");
  TEST(i, v, 256, "%d != %d");

  TEST(i, sscanf("20 xyz", "%d %d\n", &x, &y), 1,
       "only %d fields, expected %d");
  TEST(i, x, 20, "%d != %d");

  TEST(i, sscanf("xyz", "%d %d\n", &x, &y), 0,
       "got %d fields, expected no match (%d)");

  TEST(i, sscanf("", "%d %d\n", &x, &y), -1,
       "got %d fields, expected input failure (%d)");

  TEST(i, sscanf(" 12345 6", "%2d%d%d", &x, &y, &z), 3,
       "only %d fields, expected %d");
  TEST(i, x, 12, "%d != %d");
  TEST(i, y, 345, "%d != %d");
  TEST(i, z, 6, "%d != %d");

  TEST(i, sscanf(" 0x12 0x34", "%5i%2i", &x, &y), 1,
       "got %d fields, expected %d");
  TEST(i, x, 0x12, "%d != %d");

  TEST_F(123);
  TEST_F(123.0);
  TEST_F(123.0e+0);
  TEST_F(123.0e+4);
  TEST_F(1.234e1234);
  TEST_F(1.234e-1234);
  TEST_F(1.234e56789);
  TEST_F(1.234e-56789);
  TEST_F(-0.5);
  TEST_F(0.1);
  TEST_F(0.2);
  TEST_F(0.1e-10);
  TEST_F(0x1234p56);

  TEST(i, sscanf("10e", "%lf", &d), 0, "got %d fields, expected no match (%d)");
  TEST(i, sscanf("", "%lf\n", &d), -1,
       "got %d fields, expected input failure (%d)");
  return t_status;
}
