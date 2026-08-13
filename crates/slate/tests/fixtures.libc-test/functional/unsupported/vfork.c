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

#define _GNU_SOURCE
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define TEST(c, ...) ((c) || (t_error(#c " failed: " __VA_ARGS__), 0))

static int w(pid_t pid) {
  int r, s;
  r = waitpid(pid, &s, 0);
  if (r == -1)
    t_error("waitpid failed: %s\n", strerror(errno));
  else if (r != pid)
    t_error("child pid was %d, waitpid returned %d\n", pid, r);
  else
    return s;
  return -1;
}

static void test_exit(int code) {
  pid_t pid;
  if ((pid = vfork()) == 0) {
    _exit(code);
    t_error("exit failed: %s\n", strerror(errno));
  }
  if (pid == -1) {
    t_error("vfork failed: %s\n", strerror(errno));
    return;
  }
  int r = w(pid);
  TEST(WIFEXITED(r), "child terminated abnormally\n");
  TEST(WEXITSTATUS(r) == code, "child exited with %d, expected %d\n",
       WEXITSTATUS(r), code);
}

static int sh(const char *cmd) {
  pid_t pid;
  if ((pid = vfork()) == 0) {
    execl("/bin/sh", "/bin/sh", "-c", cmd, (char *)0);
    t_error("execl failed: %s\n", strerror(errno));
    _exit(1);
  }
  if (pid == -1) {
    t_error("vfork failed: %s\n", strerror(errno));
    return -1;
  }
  return w(pid);
}

static void test_shell_exit(const char *cmd, int code) {
  int r = sh(cmd);
  TEST(WIFEXITED(r), "child terminated abnormally\n");
  TEST(WEXITSTATUS(r) == code, "child exited with %d, expected %d\n",
       WEXITSTATUS(r), code);
}

static void test_shell_kill(const char *cmd, int sig) {
  int r = sh(cmd);
  TEST(WIFSIGNALED(r), "child did not get killed\n");
  TEST(WTERMSIG(r) == sig, "child is killed by %d, expected %d\n", WTERMSIG(r),
       sig);
}

int main() {
  test_exit(0);
  test_exit(1);
  test_shell_exit("exit 0", 0);
  test_shell_exit("exit 1", 1);
  test_shell_kill("kill -9 $$", 9);
  return t_status;
}
