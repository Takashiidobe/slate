#define _GNU_SOURCE 1

#include <stdint.h>
#include <unistd.h>

/* TODO: not thread-safe nor fork-safe */
extern volatile int t_status;

#define T_LOC2(l) __FILE__ ":" #l
#define T_LOC1(l) T_LOC2(l)
#define t_error(...) t_printf(T_LOC1(__LINE__) ": " __VA_ARGS__)

int t_printf(const char *s, ...);

int t_vmfill(void **, size_t *, int);
int t_memfill(void);

void t_fdfill(void);

void t_randseed(uint64_t s);
uint64_t t_randn(uint64_t n);
uint64_t t_randint(uint64_t a, uint64_t b);
void t_shuffle(uint64_t *p, size_t n);
void t_randrange(uint64_t *p, size_t n);
int t_choose(uint64_t n, size_t k, uint64_t *p);

char *t_pathrel(char *buf, size_t n, char *argv0, char *p);

int t_setrlim(int r, long lim);

int t_setutf8(void);

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

volatile int t_status = 0;

int t_printf(const char *s, ...)
{
	va_list ap;
	char buf[512];
	int n;

	t_status = 1;
	va_start(ap, s);
	n = vsnprintf(buf, sizeof buf, s, ap);
	va_end(ap);
	if (n < 0)
		n = 0;
	else if (n >= sizeof buf) {
		n = sizeof buf;
		buf[n - 1] = '\n';
		buf[n - 2] = '.';
		buf[n - 3] = '.';
		buf[n - 4] = '.';
	}
	return write(1, buf, n);
}

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>

/* naive statistical checks */

/* error p ~ 1.6e-6 */
static int chkmissing(long *x)
{
	int d[8] = {0};
	int i;
	for (i = 0; i < 100; i++)
		d[x[i]%8]++;
	for (i = 0; i < 8; i++)
		if (d[i]==0)
			return 1;
	return 0;
}

/* error p ~ 4e-6 */
static int chkrepeat(long *x)
{
	int i, j;
	for (i = 0; i < 100; i++)
		for (j = 0; j < i; j++)
			if (x[i] == x[j])
				return 1;
	return 0;
}

/* error p ~ 1e-6 */
static unsigned orx;
static int chkones(long *x)
{
	int i;
	orx = 0;
	for (i = 0; i < 20; i++)
		orx |= x[i];
	return orx != 0x7fffffff;
}

void checkseed(unsigned seed, long *x)
{
	int i;
	srandom(seed);
	for (i = 0; i < 100; i++)
		x[i] = random();
	if (chkmissing(x))
		t_error("weak seed %d, missing pattern in low bits\n", seed);
	if (chkrepeat(x))
		t_error("weak seed %d, exact repeats\n", seed);
	if (chkones(x))
		t_error("weak seed %d, or pattern: 0x%08x\n", seed, orx);
}

int main()
{
	long x[100];
	long y,z;
	int i;
	char state[128];
	char *p;
	char *q;

	for (i = 0; i < 100; i++)
		x[i] = random();
	p = initstate(1, state, sizeof state);
	for (i = 0; i < 100; i++)
		if (x[i] != (y = random()))
			t_error("initstate(1) is not default: (%d) default: %ld, seed1: %ld\n", i, x[i], y);
	for (i = 0; i < 10; i++) {
		z = random();
		q = setstate(p);
		if (z != (y = random()))
			t_error("setstate failed (%d) orig: %ld, reset: %ld\n", i, z, y);
		p = setstate(q);
	}
	srandom(1);
	for (i = 0; i < 100; i++)
		if (x[i] != (y = random()))
			t_error("srandom(1) is not default: (%d) default: %ld, seed1: %ld\n", i, x[i], y);
	checkseed(0x7fffffff, x);
	for (i = 0; i < 10; i++)
		checkseed(i, x);
	return t_status;
}
