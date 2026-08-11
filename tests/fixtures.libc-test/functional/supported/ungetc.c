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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

#define TEST(r, f, x, m) ( \
	errno = 0, ((r) = (f)) == (x) || \
	(t_error("%s failed (" m ")\n", #f, r, x, strerror(errno)), 0) )

#define TEST_S(s, x, m) ( \
	!strcmp((s),(x)) || \
	(t_error("[%s] != [%s] (%s)\n", s, x, m), 0) )

int main(void)
{
	int i;
	char a[100];
	FILE *f;

	TEST(i, !(f = tmpfile()), 0, "failed to create temp file %d!=%d (%s)");

	if (!f) return t_status;

	TEST(i, fprintf(f, "hello, world\n"), 13, "%d != %d (%m)");
	TEST(i, fseek(f, 0, SEEK_SET), 0, "%d != %d (%m)");

	TEST(i, feof(f), 0, "%d != %d");
	TEST(i, fgetc(f), 'h', "'%c' != '%c'");
	TEST(i, ftell(f), 1, "%d != %d");
	TEST(i, ungetc('x', f), 'x', "%d != %d");
	TEST(i, ftell(f), 0, "%d != %d");
	TEST(i, fscanf(f, "%[h]", a), 0, "got %d fields, expected %d");
	TEST(i, ftell(f), 0, "%d != %d");
	TEST(i, fgetc(f), 'x', "'%c' != '%c'");
	TEST(i, ftell(f), 1, "%d != %d");

	TEST(i, fseek(f, 0, SEEK_SET), 0, "%d != %d");
	TEST(i, ungetc('x', f), 'x', "%d != %d");
	TEST(i, fread(a, 1, sizeof a, f), 14, "read %d, expected %d");
	a[14] = 0;
	TEST_S(a, "xhello, world\n", "mismatch reading ungot character");

	TEST(i, fseek(f, 0, SEEK_SET), 0, "%d != %d");
	TEST(i, fscanf(f, "%[x]", a), 0, "got %d fields, expected %d");
	TEST(i, ungetc('x', f), 'x', "unget failed after fscanf: %d != %d");
	TEST(i, fgetc(f), 'x', "'%c' != '%c'");
	TEST(i, fgetc(f), 'h', "'%c' != '%c'");

	fclose(f);
	return t_status;
}
