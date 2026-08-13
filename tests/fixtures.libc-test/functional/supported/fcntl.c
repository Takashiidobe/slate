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
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define TEST(c, ...) ((c) ? 1 : (t_error(#c " failed: " __VA_ARGS__), 0))
#define TESTE(c)     (errno = 0, TEST(c, "errno = %s\n", strerror(errno)))

int main(void) {
  struct flock fl = {0};
  FILE        *f;
  int          fd;
  pid_t        pid;
  int          status;

  if (!TESTE(f = tmpfile()))
    return t_status;
  fd = fileno(f);

  fl.l_type   = F_WRLCK;
  fl.l_whence = SEEK_SET;
  fl.l_start  = 0;
  fl.l_len    = 0;
  TESTE(fcntl(fd, F_SETLK, &fl) == 0);

  pid = fork();
  if (!pid) {
    fl.l_type = F_RDLCK;
    _exit(fcntl(fd, F_SETLK, &fl) == 0 || (errno != EAGAIN && errno != EACCES));
  }
  while (waitpid(pid, &status, 0) < 0 && errno == EINTR)
    ;
  TEST(status == 0, "lock failed to work\n");

  pid = fork();
  if (!pid) {
    fl.l_type = F_WRLCK;
    _exit(fcntl(fd, F_GETLK, &fl) || fl.l_pid != getppid());
  }
  while (waitpid(pid, &status, 0) < 0 && errno == EINTR)
    ;
  TEST(status == 0, "child failed to detect lock held by parent\n");

  fclose(f);

  return t_status;
}
