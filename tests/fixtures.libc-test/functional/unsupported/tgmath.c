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

#include <tgmath.h>

int main(void) {
  long i;

  i = lrint(123456789.1f) & 0x7fffffff;
  if (i != 123456792)
    t_error("lrint(123456789.1f)&0x7fffffff want 123456792 got %ld\n", i);
  i = lrint(123456789.1) & 0x7fffffff;
  if (i != 123456789)
    t_error("lrint(123456789.1)&0x7fffffff want 123456789 got %ld\n", i);

  if (sqrt(2.0f) != 1.41421353816986083984375)
    t_error("sqrt(2.0f) want 0x1.6a09e6p+0 got %a\n", sqrt(2.0f));
  if (sqrt(2.0) != 1.414213562373095145474621858738828450441360)
    t_error("sqrt(2.0) want 0x1.6a09e667f3bcdp+0 got %a\n", sqrt(2.0));
  if (sqrt(2) != 1.414213562373095145474621858738828450441360)
    t_error("sqrt(2) want 0x1.6a09e667f3bcdp+0 got %a\n", sqrt(2.0));

  if (sizeof pow(sqrt(8), 0.5f) != sizeof(double))
    t_error("sizeof pow(sqrt(8),0.5f) want %d got %d\n", (int)sizeof(double),
            (int)sizeof pow(sqrt(8), 0.5f));
  if (sizeof pow(2.0, 0.5) != sizeof(double))
    t_error("sizeof pow(2.0,0.5) want %d got %d\n", (int)sizeof(double),
            (int)sizeof pow(2.0, 0.5));
  if (sizeof pow(2.0f, 0.5f) != sizeof(float))
    t_error("sizeof pow(2.0f,0.5f) want %d got %d\n", (int)sizeof(float),
            (int)sizeof pow(2.0f, 0.5f));
  if (sizeof pow(2.0, 0.5 + 0 * I) != sizeof(double complex))
    t_error("sizeof pow(2.0,0.5+0*I) want %d got %d\n",
            (int)sizeof(double complex), (int)sizeof pow(2.0, 0.5 + 0 * I));

  if (pow(2.0, 0.5) != 1.414213562373095145474621858738828450441360)
    t_error("pow(2.0,0.5) want 0x1.6a09e667f3bcdp+0 got %a\n", pow(2.0, 0.5));
  if (pow(2, 0.5) != 1.414213562373095145474621858738828450441360)
    t_error("pow(2,0.5) want 0x1.6a09e667f3bcdp+0 got %a\n", pow(2, 0.5));
  if (pow(2, 0.5f) != 1.414213562373095145474621858738828450441360)
    t_error("pow(2,0.5f) want 0x1.6a09e667f3bcdp+0 got %a\n", pow(2, 0.5f));

  return t_status;
}
