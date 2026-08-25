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
#include <search.h>
#include <string.h>

#define W 80
static char   tab[100][W];
static size_t nel;

#define set(k)                                                                 \
  do {                                                                         \
    char *r =                                                                  \
        lsearch(k, tab, &nel, W, (int (*)(const void *, const void *))strcmp); \
    if (strcmp(r, k) != 0)                                                     \
      t_error("lsearch %s failed\n", #k);                                      \
  } while (0)

#define get(k)                                                                 \
  lfind(k, tab, &nel, W, (int (*)(const void *, const void *))strcmp)

int main() {
  size_t n;

  set("");
  set("a");
  set("b");
  set("abc");
  set("cd");
  set("e");
  set("ef");
  set("g");
  set("h");
  set("iiiiiiiiii");
  if (!get("a"))
    t_error("lfind a failed\n");
  if (get("c"))
    t_error("lfind c should fail\n");
  n = nel;
  set("g");
  if (nel != n)
    t_error("lsearch g should not modify the table size (%d, was %d)\n", nel,
            n);
  n = nel;
  set("j");
  if (nel != n + 1)
    t_error("lsearch j should increase the table size (%d, was %d)\n", nel, n);
  return t_status;
}
