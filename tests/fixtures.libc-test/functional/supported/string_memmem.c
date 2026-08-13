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
#include <string.h>

#define N(s, tail, sub)                                                        \
  {                                                                            \
    char *p       = s tail;                                                    \
    char       *q = memmem(p, strlen(s), sub, strlen(sub));                    \
    if (q)                                                                     \
      t_error("memmem(" #s " " #tail ", %d, " #sub                             \
              ", %d) returned str+%d, wanted 0\n",                             \
              strlen(s), strlen(sub), q - p);                                  \
  }

#define T(s, sub, n)                                                           \
  {                                                                            \
    char *p = s;                                                               \
    char *q = memmem(p, strlen(p), sub, strlen(sub));                          \
    if (q == 0)                                                                \
      t_error("memmem(%s,%s) returned 0, wanted str+%d\n", #s, #sub, n);       \
    else if (q - p != n)                                                       \
      t_error("memmem(%s,%s) returned str+%d, wanted str+%d\n", #s, #sub,      \
              q - p, n);                                                       \
  }

int main(void) {
  N("", "a", "a")
  N("a", "a", "aa")
  N("a", "b", "b")
  N("aa", "b", "ab")
  N("aa", "a", "aaa")
  N("aba", "b", "bab")
  N("abba", "b", "bab")
  N("abba", "ba", "aba")
  N("abc abc", "d", "abcd")
  N("0-1-2-3-4-5-6-7-8-9", "", "-3-4-56-7-8-")
  N("0-1-2-3-4-5-6-7-8-9", "", "-3-4-5+6-7-8-")
  N("_ _ _\xff_ _ _", "\x7f_", "_\x7f_")
  N("_ _ _\x7f_ _ _", "\xff_", "_\xff_")

  T("", "", 0)
  T("abcd", "", 0)
  T("abcd", "a", 0)
  T("abcd", "b", 1)
  T("abcd", "c", 2)
  T("abcd", "d", 3)
  T("abcd", "ab", 0)
  T("abcd", "bc", 1)
  T("abcd", "cd", 2)
  T("ababa", "baba", 1)
  T("ababab", "babab", 1)
  T("abababa", "bababa", 1)
  T("abababab", "bababab", 1)
  T("ababababa", "babababa", 1)
  T("abbababab", "bababa", 2)
  T("abbababab", "ababab", 3)
  T("abacabcabcab", "abcabcab", 4)
  T("nanabanabanana", "aba", 3)
  T("nanabanabanana", "ban", 4)
  T("nanabanabanana", "anab", 1)
  T("nanabanabanana", "banana", 8)
  T("_ _\xff_ _", "_\xff_", 2)

  return t_status;
}
