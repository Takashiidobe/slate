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
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TEST(r, f, x, m)                                                       \
  (((r) = (f)) == (x) || (t_error("%s failed (" m ")\n", #f, r, x), 0))

#define TEST_E(f)                                                              \
  ((errno = 0), (f) || (t_error("%s failed (errno = %d)\n", #f, errno), 0))

#define TEST_S(s, x, m)                                                        \
  (!strcmp((s), (x)) || (t_error("[%s] != [%s] (%s)\n", s, x, m), 0))

static sig_atomic_t got_sig;

static void handler(int sig) { got_sig = 1; }

int main(void) {
  int   i;
  char  foo[6];
  char  cmd[64];
  FILE *f;

  TEST_E(f = popen("echo hello", "r"));
  if (f) {
    TEST_E(fgets(foo, sizeof foo, f));
    TEST_S(foo, "hello", "child process did not say hello");
    TEST(i, pclose(f), 0, "exit status %04x != %04x");
  }

  signal(SIGUSR1, handler);
  snprintf(cmd, sizeof cmd, "read a ; test \"x$a\" = xhello && kill -USR1 %d",
           getpid());
  TEST_E(f = popen(cmd, "w"));
  if (f) {
    TEST_E(fputs("hello", f) >= 0);
    TEST(i, pclose(f), 0, "exit status %04x != %04x");
    TEST(i, got_sig, 1, "child process did not send signal");
  }
  signal(SIGUSR1, SIG_DFL);
  return t_status;
}
