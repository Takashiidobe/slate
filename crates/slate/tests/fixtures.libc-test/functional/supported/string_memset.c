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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define N 500
static char buf[N];
static char buf2[N];

static void *(*volatile pmemset)(void *, int, size_t);

static char *aligned(void *p) { return (char *)(((uintptr_t)p + 63) & -64); }

static void test_align(int align, int len) {
  char *s    = aligned(buf + 64) + align;
  char *want = aligned(buf2 + 64) + align;
  char *p;
  int   i;

  if (len + 64 > buf + N - s || len + 64 > buf2 + N - want)
    abort();
  for (i = 0; i < N; i++)
    buf[i] = buf2[i] = ' ';
  for (i = 0; i < len; i++)
    want[i] = '#';
  p = pmemset(s, '#', len);
  if (p != s)
    t_error("memset(%p,...) returned %p\n", s, p);
  for (i = -64; i < len + 64; i++)
    if (s[i] != want[i]) {
      t_error("memset(align %d, '#', %d) failed at pos %d\n", align, len, i);
      t_printf("got : '%.*s'\n", len + 128, s - 64);
      t_printf("want: '%.*s'\n", len + 128, want - 64);
      break;
    }
}

static void test_value(int c) {
  int i;

  pmemset(buf, c, 10);
  for (i = 0; i < 10; i++)
    if ((unsigned char)buf[i] != (unsigned char)c) {
      t_error("memset(%d) failed: got %d\n", c, buf[i]);
      break;
    }
}

int main(void) {
  int i, j, k;

  pmemset = memset;

  for (i = 0; i < 64; i++)
    for (j = 0; j < N - 256; j++)
      test_align(i, j);

  test_value('c');
  test_value(0);
  test_value(-1);
  test_value(-5);
  test_value(0xab);
  return t_status;
}
