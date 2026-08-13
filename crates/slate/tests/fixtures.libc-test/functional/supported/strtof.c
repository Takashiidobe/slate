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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define length(x) (sizeof(x) / sizeof *(x))

static struct {
  char *s;
  float f;
} t[] = {
    // 2^-149 * 0.5 - eps
    {".700649232162408535461864791644958065640130970938257885878534141944895541"
     "3429303e-45",
     0},
    // 2^-149 * 0.5 + eps
    {".700649232162408535461864791644958065640130970938257885878534141944895541"
     "3429304e-45",
     0x1p-149},
    // 2^-149 * 0.5 - eps
    {".210194769648722560638559437493487419692039291281477365763560242583468662"
     "4028790e-44",
     0x1p-149},
    // 2^-149 * 0.5 + eps
    {".210194769648722560638559437493487419692039291281477365763560242583468662"
     "4028791e-44",
     0x1p-148},
    // 2^-126 + 2^-150 - eps
    {".117549442088721072420959008340872484231447212078518461533454029413183145"
     "3944281e-37",
     0x1p-126},
    // 2^-126 + 2^-150 + eps
    {".117549442088721072420959008340872484231447212078518461533454029413183145"
     "3944282e-37",
     0x1.000002p-126},
    // 2^128 - 2^103 - eps
    {"340282356779733661637539395458142568447.9999999999999999999",
     0x1.fffffep127},
    // 2^128 - 2^103
    {"340282356779733661637539395458142568448", INFINITY},
};

int main(void) {
  int   i;
  float x;
  char *p;

  for (i = 0; i < length(t); i++) {
    x = strtof(t[i].s, &p);
    if (x != t[i].f)
      t_error("strtof(\"%s\") want %a got %a\n", t[i].s, t[i].f, x);
  }
  return t_status;
}
