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

static char buf[512];

static void *aligned(void *p)
{
	return (void*)(((uintptr_t)p + 63) & -64);
}

static void *aligncpy(void *p, size_t len, size_t a)
{
	return memcpy((char*)aligned(buf)+a, p, len);
}

#define N(s, c) { \
	int align; \
	for (align=0; align<8; align++) { \
		char *p = aligncpy(s, sizeof s, align); \
		char *q = strchr(p, c); \
		if (q) \
			t_error("strchr(%s,%s) with align=%d returned str+%d, wanted 0\n", #s, #c, align, q-p); \
	} \
}

#define T(s, c, n) { \
	int align; \
	for (align=0; align<8; align++) { \
		char *p = aligncpy(s, sizeof s, align); \
		char *q = strchr(p, c); \
		if (q == 0) \
			t_error("strchr(%s,%s) with align=%d returned 0, wanted str+%d\n", #s, #c, align, n); \
		else if (q - p != n) \
			t_error("strchr(%s,%s) with align=%d returned str+%d, wanted str+%d\n", #s, #c, align, q-p, n); \
	} \
}

int main(void)
{
	int i;
	char a[128];
	char s[256];

	for (i = 0; i < 128; i++)
		a[i] = (i+1) & 127;
	for (i = 0; i < 256; i++)
		*((unsigned char*)s+i) = i+1;

	N("\0aaa", 'a')
	N("a\0bb", 'b')
	N("ab\0c", 'c')
	N("abc\0d", 'd')
	N("abc abc\0x", 'x')
	N(a, 128)
	N(a, 255)

	T("", 0, 0)
	T("a", 'a', 0)
	T("a", 'a'+256, 0)
	T("a", 0, 1)
	T("abb", 'b', 1)
	T("aabb", 'b', 2)
	T("aaabb", 'b', 3)
	T("aaaabb", 'b', 4)
	T("aaaaabb", 'b', 5)
	T("aaaaaabb", 'b', 6)
	T("abc abc", 'c', 2)
	T(s, 1, 0)
	T(s, 2, 1)
	T(s, 10, 9)
	T(s, 11, 10)
	T(s, 127, 126)
	T(s, 128, 127)
	T(s, 255, 254)
	T(s, 0, 255)

	return t_status;
}
