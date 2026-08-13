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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* r = place to store result
 * f = function call to test (or any expression)
 * x = expected result
 * m = message to print on failure (with formats for r & x)
 */

#define TEST(r, f, x, m)                                                       \
  (((r) = (f)) == (x) || (t_error("%s failed (" m ")\n", #f, r, x, r - x), 0))

int main(void) {
  int    i;
  double d, d2;
  char   buf[1000];

  for (i = 0; i < 100; i++) {
    d = sin(i);
    snprintf(buf, sizeof buf, "%.300f", d);
    TEST(d2, strtod(buf, 0), d, "round trip fail %a != %a (%a)");
  }

  TEST(d, strtod("0x1p4", 0), 16.0, "hex float %a != %a");
  TEST(d, strtod("0x1.1p4", 0), 17.0, "hex float %a != %a");
  return t_status;
}
