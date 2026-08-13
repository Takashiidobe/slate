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

/* unnamed semaphore sanity check */
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>

#define T(f)                                                                   \
  if (f)                                                                       \
  t_error(#f " failed: %s\n", strerror(errno))
#define T2(r, f)                                                               \
  if ((r = (f)))                                                               \
  t_error(#f " failed: %s\n", strerror(r))

static void *start(void *arg) {
  struct timespec ts;
  sem_t          *s = arg;
  T(clock_gettime(CLOCK_REALTIME, &ts));
  ts.tv_sec += 1;
  T(sem_post(s));
  T(sem_timedwait(s + 1, &ts));
  return 0;
}

static void many_waiters() {
  pthread_t t[3];
  sem_t     s[2];
  int       r;
  void     *p;

  T(sem_init(s, 0, 0));
  T(sem_init(s + 1, 0, 0));
  T2(r, pthread_create(t, 0, start, s));
  T2(r, pthread_create(t + 1, 0, start, s));
  T2(r, pthread_create(t + 2, 0, start, s));
  T(sem_wait(s));
  T(sem_wait(s));
  T(sem_wait(s));
  T(sem_getvalue(s, &r));
  if (r)
    t_error("sem value should be 0, got %d\n", r);
  T(sem_post(s + 1));
  T(sem_post(s + 1));
  T(sem_post(s + 1));
  T2(r, pthread_join(t[0], &p));
  T2(r, pthread_join(t[1], &p));
  T2(r, pthread_join(t[2], &p));
  T(sem_getvalue(s + 1, &r));
  if (r)
    t_error("sem value should be 0, got %d\n", r);
  T(sem_destroy(s));
  T(sem_destroy(s + 1));
}

static void single_thread() {
  struct timespec ts;
  sem_t           s;
  int             r;

  T(sem_init(&s, 0, 1));
  T(sem_wait(&s));
  T(sem_getvalue(&s, &r));
  if (r)
    t_error("sem value should be 0, got %d\n", r);
  if (sem_trywait(&s) != -1 || errno != EAGAIN)
    t_error("sem_trywait should fail with EAGAIN, got %s\n", strerror(errno));
  errno = 0;
  T(clock_gettime(CLOCK_REALTIME, &ts));
  if (sem_timedwait(&s, &ts) != -1 || errno != ETIMEDOUT)
    t_error("sem_timedwait should fail with ETIMEDOUT, got %s\n",
            strerror(errno));
  T(sem_destroy(&s));
}

int main(void) {
  single_thread();
  many_waiters();
  return t_status;
}
