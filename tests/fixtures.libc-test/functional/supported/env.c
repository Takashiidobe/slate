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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

int main() {
  char *s;
  int   r;

  if (!environ)
    t_error("environ is NULL\n");
  if (clearenv() || (environ && *environ))
    t_error("clrearenv: %s\n", strerror(errno));
  if (putenv("TEST=1"))
    t_error("putenv: %s\n", strerror(errno));
  if (strcmp(environ[0], "TEST=1") != 0)
    t_error("putenv failed: environ[0]: %s, wanted \"TEST=1\"\n", environ[0]);
  if ((s = environ[1]))
    t_error("environ[1]: %p, wanted 0\n", s);
  if (!(s = getenv("TEST")))
    t_error("getenv(\"TEST\"): 0, wanted \"1\"\n");
  if (strcmp(s, "1") != 0)
    t_error("getenv(\"TEST\"): \"%s\", wanted \"1\"\n", s);
  if (unsetenv("TEST"))
    t_error("unsetenv: %s\n", strerror(errno));
  if ((s = *environ))
    t_error("*environ: %p != 0\n", s);
  if ((s = getenv("TEST")))
    t_error("getenv(\"TEST\"): %p, wanted 0\n", s);
  errno = 0;
  if (setenv("TEST", "2", 0))
    t_error("setenv: %s\n", strerror(errno));
  if (strcmp(s = getenv("TEST"), "2") != 0)
    t_error("getenv(\"TEST\"): \"%s\", wanted \"2\"\n", s);
  if (strcmp(environ[0], "TEST=2") != 0)
    t_error("setenv failed: environ[0]: %s, wanted \"TEST=2\"\n", environ[0]);
  errno = 0;
  if (setenv("TEST", "3", 0))
    t_error("setenv: %s\n", strerror(errno));
  if (strcmp(s = getenv("TEST"), "2") != 0)
    t_error("getenv(\"TEST\"): \"%s\", wanted \"2\"\n", s);
  errno = 0;
  if (setenv("TEST", "3", 1))
    t_error("setenv: %s\n", strerror(errno));
  if (strcmp(s = getenv("TEST"), "3") != 0)
    t_error("getenv(\"TEST\"): \"%s\", wanted \"3\"\n", s);
  /* test failures */
  errno = 0;
  if ((r = setenv("", "", 0)) != -1 || errno != EINVAL)
    t_error("setenv(\"\",\"\",0): %d, errno: %d (%s), wanted -1, %d (EINVAL)\n",
            r, errno, strerror(errno), EINVAL);
  return t_status;
}
