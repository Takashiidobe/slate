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

// SPDX-License-Identifier: MIT

#define _DEFAULT_SOURCE // for getmntent_r

#include <errno.h>
#include <mntent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT(x)                                                              \
  do {                                                                         \
    if (!(x)) {                                                                \
      t_error(#x " failed\n");                                                 \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0);

#define ERR(fmt, ...)                                                          \
  do {                                                                         \
    t_error(fmt ": %s\n", ##__VA_ARGS__, strerror(errno));                     \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

void test_getmntent_empty(void) {
  char  fstab[] = "\n";
  FILE *f       = fmemopen((void *)fstab, sizeof fstab - 1, "r");
  if (!f)
    ERR("fmemopen");
  ASSERT(!getmntent(f));
  ASSERT(endmntent(f) == 1);
}

void test_getmntent(void) {
  // Checks that the fifth and sixth fields default to 0.
  char  fstab[] = "none /proc proc defaults\n";
  FILE *f       = fmemopen((void *)fstab, sizeof fstab - 1, "r");
  if (!f)
    ERR("fmemopen");
  struct mntent *m = getmntent(f);
  ASSERT(m);
  ASSERT(!strcmp(m->mnt_fsname, "none"));
  ASSERT(!strcmp(m->mnt_dir, "/proc"));
  ASSERT(!strcmp(m->mnt_type, "proc"));
  ASSERT(!strcmp(m->mnt_opts, "defaults"));
  ASSERT(m->mnt_freq == 0);
  ASSERT(m->mnt_passno == 0);
  ASSERT(endmntent(f) == 1);
}

void test_getmntent_r(void) {
  struct mntent m, *r;
  char          fstab[] = "/dev/sda\t/\text4\trw,nosuid\t2\t1\n";
  char          buf[sizeof(fstab)];

  FILE *f = fmemopen((void *)fstab, sizeof fstab - 1, "r");
  if (!f)
    ERR("fmemopen");

  r = getmntent_r(f, &m, buf, sizeof buf);
  ASSERT(r == &m);
  ASSERT(!strcmp(m.mnt_fsname, "/dev/sda"));
  ASSERT(!strcmp(m.mnt_dir, "/"));
  ASSERT(!strcmp(m.mnt_type, "ext4"));
  ASSERT(!strcmp(m.mnt_opts, "rw,nosuid"));
  ASSERT(m.mnt_freq == 2);
  ASSERT(m.mnt_passno == 1);
  ASSERT(endmntent(f) == 1);
}

int main(void) {
  test_getmntent_empty();
  test_getmntent();
  test_getmntent_r();
}
