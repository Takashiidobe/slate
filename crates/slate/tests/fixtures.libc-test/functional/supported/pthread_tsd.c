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

#include <pthread.h>
#include <string.h>

#define TESTC(c, m) ((c) || (t_error("%s failed (" m ")\n", #c), 0))
#define TESTR(r, f, m)                                                         \
  (((r) = (f)) == 0 || (t_error("%s failed: %s (" m ")\n", #f, strerror(r)), 0))

static pthread_key_t k1, k2;

static void dtor(void *p) { *(int *)p = 1; }

static void *start(void *arg) {
  int *p = arg;
  if (pthread_setspecific(k1, p) || pthread_setspecific(k2, p + 1))
    return arg;
  return 0;
}

int main(void) {
  pthread_t td;
  int       r;
  void     *res;
  int       foo[2], bar[2];

  /* Test POSIX thread-specific data */
  TESTR(r, pthread_key_create(&k1, dtor), "failed to create key");
  TESTR(r, pthread_key_create(&k2, dtor), "failed to create key");
  foo[0] = foo[1] = 0;
  TESTR(r, pthread_setspecific(k1, bar), "failed to set tsd");
  TESTR(r, pthread_setspecific(k2, bar + 1), "failed to set tsd");
  TESTR(r, pthread_create(&td, 0, start, foo), "failed to create thread");
  TESTR(r, pthread_join(td, &res), "failed to join");
  TESTC(res == 0, "pthread_setspecific failed in thread");
  TESTC(foo[0] == 1, "dtor failed to run");
  TESTC(foo[1] == 1, "dtor failed to run");
  TESTC(pthread_getspecific(k1) == bar, "tsd corrupted");
  TESTC(pthread_getspecific(k2) == bar + 1, "tsd corrupted");
  TESTR(r, pthread_setspecific(k1, 0), "failed to clear tsd");
  TESTR(r, pthread_setspecific(k2, 0), "failed to clear tsd");
  TESTR(r, pthread_key_delete(k1), "failed to destroy key");
  TESTR(r, pthread_key_delete(k2), "failed to destroy key");
  return t_status;
}
