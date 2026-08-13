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
#include <errno.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define TEST(f, x)                                                             \
  (void)((r = (f)) == (x) || t_error("%s failed, got %d want %d\n", #f, r, x))

#define TEST_E(f)                                                              \
  (void)((errno = 0), (f) || t_error("%s failed (errno = %d \"%s\")\n", #f,    \
                                     errno, strerror(errno)))

int main(void) {
  int                        r;
  char                       foo[10];
  int                        p[2];
  pid_t                      pid;
  int                        status;
  posix_spawn_file_actions_t fa;

  TEST_E(!pipe(p));
  TEST(posix_spawn_file_actions_init(&fa), 0);
  TEST(posix_spawn_file_actions_addclose(&fa, p[0]), 0);
  TEST(posix_spawn_file_actions_adddup2(&fa, p[1], 1), 0);
  TEST(posix_spawn_file_actions_addclose(&fa, p[1]), 0);
  TEST(posix_spawnp(&pid, "echo", &fa, 0, (char *[]){"echo", "hello", 0}, 0),
       0);
  close(p[1]);
  TEST(waitpid(pid, &status, 0), pid);
  TEST(read(p[0], foo, sizeof foo), 6);
  close(p[0]);
  TEST(posix_spawn_file_actions_destroy(&fa), 0);
  return t_status;
}
