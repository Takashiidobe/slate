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

#include <wchar.h>

#define N(s, sub) { \
	wchar_t *p = s; \
	wchar_t *q = wcsstr(p, sub); \
	if (q) \
		t_error("wcsstr(%s,%s) returned str+%d, wanted 0\n", #s, #sub, q-p); \
}

#define T(s, sub, n) { \
	wchar_t *p = s; \
	wchar_t *q = wcsstr(p, sub); \
	if (q == 0) \
		t_error("wcsstr(%s,%s) returned 0, wanted str+%d\n", #s, #sub, n); \
	else if (q - p != n) \
		t_error("wcsstr(%s,%s) returned str+%d, wanted str+%d\n", #s, #sub, q-p, n); \
}

int main(void)
{
	N(L"", L"a")
	N(L"a", L"aa")
	N(L"a", L"b")
	N(L"aa", L"ab")
	N(L"aa", L"aaa")
	N(L"abba", L"aba")
	N(L"abc abc", L"abcd")
	N(L"0-1-2-3-4-5-6-7-8-9", L"-3-4-56-7-8-")
	N(L"0-1-2-3-4-5-6-7-8-9", L"-3-4-5+6-7-8-")
	N(L"_ _ _\xff_ _ _", L"_\x7f_")
	N(L"_ _ _\x7f_ _ _", L"_\xff_")

	T(L"", L"", 0)
	T(L"abcd", L"", 0)
	T(L"abcd", L"a", 0)
	T(L"abcd", L"b", 1)
	T(L"abcd", L"c", 2)
	T(L"abcd", L"d", 3)
	T(L"abcd", L"ab", 0)
	T(L"abcd", L"bc", 1)
	T(L"abcd", L"cd", 2)
	T(L"ababa", L"baba", 1)
	T(L"ababab", L"babab", 1)
	T(L"abababa", L"bababa", 1)
	T(L"abababab", L"bababab", 1)
	T(L"ababababa", L"babababa", 1)
	T(L"abbababab", L"bababa", 2)
	T(L"abbababab", L"ababab", 3)
	T(L"abacabcabcab", L"abcabcab", 4)
	T(L"nanabanabanana", L"aba", 3)
	T(L"nanabanabanana", L"ban", 4)
	T(L"nanabanabanana", L"anab", 1)
	T(L"nanabanabanana", L"banana", 8)
	T(L"_ _\xff_ _", L"_\xff_", 2)

	return t_status;
}
