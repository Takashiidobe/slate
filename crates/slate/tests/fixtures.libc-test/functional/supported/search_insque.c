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
#include <stdlib.h>

struct q {
  struct q *n;
  struct q *p;
  int       i;
};

static struct q *new (int i) {
  struct q *q = malloc(sizeof *q);
  q->i        = i;
  return q;
}

int main() {
  struct q *q = new (0);
  struct q *p;
  int       i;

  insque(q, 0);
  for (i = 1; i < 10; i++) {
    insque(new (i), q);
    q = q->n;
  }
  p = q;
  while (q) {
    if (q->i != --i)
      t_error("walking queue: got %d, wanted %d\n", q->i, i);
    q = q->p;
  }
  remque(p->p);
  if (p->p->i != p->i - 2)
    t_error("remque: got %d, wanted %d\n", p->p->i, p->i - 2);
  if (p->p->n->i != p->i)
    t_error("remque: got %d, wanted %d\n", p->p->n->i, p->i);
  return t_status;
}
