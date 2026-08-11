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
#include <errno.h>
#include <iconv.h>

int main(void)
{
	iconv_t cd;
	char buf[100];
	char *inbuf = "test";
	char *outbuf = buf;
	size_t inlen = strlen(inbuf);
	size_t outlen;
	size_t r;
	char *bad = "bad-codeset";

	cd = iconv_open(bad, bad);
	if (cd != (iconv_t)-1)
		t_error("iconv_open(\"%s\",\"%s\") didn't fail\n", bad, bad);
	if (errno != EINVAL)
		t_error("iconv_open(\"%s\",\"%s\") did not fail with EINVAL, got %s\n", bad, bad, strerror(errno));

	errno = 0;
	cd = iconv_open("UTF-8", "UTF-8");
	if (cd == (iconv_t)-1)
		t_error("iconv_open(\"\",\"\") failed: %s\n", strerror(errno));

	errno = 0;
	outlen = 0;
	r = iconv(cd, &inbuf, &inlen, &outbuf, &outlen);
	if (r != -1)
		t_error("iconv() did not fail with 0 output length\n");
	if (errno != E2BIG)
		t_error("iconv() did not fail with E2BIG, got %s\n", strerror(errno));

	errno = 0;
	outlen = sizeof buf;
	r = iconv(cd, &inbuf, &inlen, &outbuf, &outlen);
	if (r)
		t_error("iconv() did not return 0 when converting utf8 to utf8, got %d\n", r);
	if (errno)
		t_error("iconv() failed: %s\n", strerror(errno));

	r = iconv_close(cd);
	if (r == -1)
		t_error("iconv_close failed: %s\n", strerror(errno));

	return t_status;
}
