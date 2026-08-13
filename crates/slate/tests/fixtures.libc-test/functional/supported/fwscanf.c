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

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#define TEST(r, f, x, m)                                                       \
  (errno = 0,                                                                  \
   ((r) = (f)) == (x) ||                                                       \
       (t_error("%s failed (" m ")\n", #f, r, x, strerror(errno)), 0))

#define TEST_S(s, x, m)                                                        \
  (!strcmp((s), (x)) || (t_error("[%s] != [%s] (%s)\n", s, x, m), 0))

static FILE *writetemp(const char *data) {
  FILE  *f = tmpfile();
  size_t n = strlen(data);
  if (!f)
    return 0;
  if (write(fileno(f), data, n) != n) {
    t_error("write: %s\n", strerror(errno));
    fclose(f);
    return 0;
  }
  if (lseek(fileno(f), 0, SEEK_SET) != 0) {
    t_error("lseek: %s\n", strerror(errno));
    fclose(f);
    return 0;
  }
  return f;
}

int main(void) {
  int    i, x, y;
  double u;
  char   a[100], b[100];
  FILE  *f;

  TEST(i, !!(f = writetemp("      42")), 1, "failed to make temp file");
  if (f) {
    x = y = -1;
    TEST(i, fwscanf(f, L" %n%*d%n", &x, &y), 0, "%d != %d");
    TEST(i, x, 6, "%d != %d");
    TEST(i, y, 8, "%d != %d");
    TEST(i, ftell(f), 8, "%d != %d");
    TEST(i, !!feof(f), 1, "%d != %d");
    fclose(f);
  }

  TEST(i, !!(f = writetemp("[abc123]....x")), 1, "failed to make temp file");
  if (f) {
    x = y = -1;
    TEST(i, fwscanf(f, L"%10[^]]%n%10[].]%n", a, &x, b, &y), 2, "%d != %d");
    TEST_S(a, "[abc123", "wrong result for %[^]]");
    TEST_S(b, "]....", "wrong result for %[].]");
    TEST(i, x, 7, "%d != %d");
    TEST(i, y, 12, "%d != %d");
    TEST(i, ftell(f), 12, "%d != %d");
    TEST(i, feof(f), 0, "%d != %d");
    TEST(i, fgetwc(f), 'x', "%d != %d");
    fclose(f);
  }

  TEST(i, !!(f = writetemp("0x1p 12")), 1, "failed to make temp file");
  if (f) {
    x = y = -1;
    u     = -1;
    TEST(i, fwscanf(f, L"%lf%n %d", &u, &x, &y), 0, "%d != %d");
    TEST(u, u, -1.0, "%g != %g");
    TEST(i, x, -1, "%d != %d");
    TEST(i, y, -1, "%d != %d");
    TEST(i, ftell(f), 4, "%d != %d");
    TEST(i, feof(f), 0, "%d != %d");
    TEST(i, fgetwc(f), ' ', "%d != %d");
    rewind(f);
    TEST(i, fgetwc(f), '0', "%d != %d");
    TEST(i, fgetwc(f), 'x', "%d != %d");
    TEST(i, fwscanf(f, L"%lf%n%c %d", &u, &x, a, &y), 3, "%d != %d");
    TEST(u, u, 1.0, "%g != %g");
    TEST(i, x, 1, "%d != %d");
    TEST(i, a[0], 'p', "%d != %d");
    TEST(i, y, 12, "%d != %d");
    TEST(i, ftell(f), 7, "%d != %d");
    TEST(i, !!feof(f), 1, "%d != %d");
    fclose(f);
  }

  TEST(i, !!(f = writetemp("0x.1p4    012")), 1, "failed to make temp file");
  if (f) {
    x = y = -1;
    u     = -1;
    TEST(i, fwscanf(f, L"%lf%n %i", &u, &x, &y), 2, "%d != %d");
    TEST(u, u, 1.0, "%g != %g");
    TEST(i, x, 6, "%d != %d");
    TEST(i, y, 10, "%d != %d");
    TEST(i, ftell(f), 13, "%d != %d");
    TEST(i, !!feof(f), 1, "%d != %d");
    fclose(f);
  }

  TEST(i, !!(f = writetemp("0xx")), 1, "failed to make temp file");
  if (f) {
    x = y = -1;
    TEST(i, fwscanf(f, L"%x%n", &x, &y), 0, "%d != %d");
    TEST(i, x, -1, "%d != %d");
    TEST(i, y, -1, "%d != %d");
    TEST(i, ftell(f), 2, "%d != %d");
    TEST(i, feof(f), 0, "%d != %d");
    fclose(f);
  }

  return t_status;
}
