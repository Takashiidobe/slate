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

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static char buf[512];

static void *(*volatile pmemcpy)(void *restrict, const void *restrict, size_t);

static void *aligned(void *p) {
	return (void*)(((uintptr_t)p + 63) & -64);
}

#define N 80
static void test_align(int dalign, int salign, int len)
{
	char *src = aligned(buf);
	char *dst = aligned(buf + 128);
	char *want = aligned(buf + 256);
	char *p;
	int i;

	if (salign + len > N || dalign + len > N)
		abort();
	for (i = 0; i < N; i++) {
		src[i] = '#';
		dst[i] = want[i] = ' ';
	}
	for (i = 0; i < len; i++)
		src[salign+i] = want[dalign+i] = '0'+i;
	p = pmemcpy(dst+dalign, src+salign, len);
	if (p != dst+dalign)
		t_error("memcpy(%p,...) returned %p\n", dst+dalign, p);
	for (i = 0; i < N; i++)
		if (dst[i] != want[i]) {
			t_error("memcpy(align %d, align %d, %d) failed\n", dalign, salign, len);
			t_printf("got : %.*s\n", dalign+len+1, dst);
			t_printf("want: %.*s\n", dalign+len+1, want);
			break;
		}
}

int main(void)
{
	int i,j,k;

	pmemcpy = memcpy;

	for (i = 0; i < 16; i++)
		for (j = 0; j < 16; j++)
			for (k = 0; k < 64; k++)
				test_align(i,j,k);

	return t_status;
}
