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

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define TEST(r, f, x, m) ( \
((r) = (f)) == (x) || \
(t_error("%s failed (" m ")\n", #f, r, x), 0) )

#define TEST_E(f) ( (errno = 0), (f) || \
(t_error("%s failed (errno = %d)\n", #f, errno), 0) )

#define TEST_S(s, x, m) ( \
!strcmp((s),(x)) || \
(t_error("[%s] != [%s] (%s)\n", s, x, m), 0) )

#define TEST_M(s, x, n, m) ( \
!memcmp((s),(x),(n)) || \
(t_error("[%s] != [%s] (%s)\n", s, x, m), 0) )

int main(void)
{
	FILE *f;
	char *s;
	size_t l;
	char buf[100];
	int i;

	s = 0;
	TEST_E(f = open_memstream(&s, &l));
	TEST_E(putc('a', f) == 'a');
	TEST_E(putc('b', f) == 'b');
	TEST_E(putc('c', f) == 'c');
	TEST_E(!fflush(f));
	fclose(f);
	if (s) TEST_S(s, "abc", "wrong output");
	free(s);

	s = 0;
	TEST_E(f = open_memstream(&s, &l));
	TEST_E(fseek(f,1,SEEK_CUR)>=0);
	TEST_E(putc('q', f) == 'q');
	TEST_E(!fflush(f));
	if (s) TEST_M(s, "\0q", 3, "wrong output");
	TEST(i, fseek(f,-3,SEEK_CUR), -1, "invalid seek allowed");
	TEST(i, errno, EINVAL, "%d != %d");
	TEST(i, ftell(f), 2, "%d != %d");
	TEST_E(fseek(f,-2,SEEK_CUR)>=0);
	TEST_E(putc('e', f) == 'e');
	TEST_E(!fflush(f));
	if (s) TEST_S(s, "eq", "wrong output");
	fclose(f);
	free(s);

	TEST_E(f = fmemopen(buf, 10, "r+"));
	TEST_E(fputs("hello", f) >= 0);
	TEST_E(fputc(0, f)==0);
	TEST_E(fseek(f, 0, SEEK_SET)>=0);
	i=0;
	TEST_E(fscanf(f, "hello%n", &i)==0);
	TEST(i, i, 5, "%d != %d");
	TEST(i, ftell(f), 5, "%d != %d");
	errno = 0;
	TEST(i, fseek(f, 6, SEEK_CUR)<0, 1, "");
	TEST(i, errno!=0, 1, "");
	TEST(i, ftell(f), 5, "%d != %d");
	TEST_S(buf, "hello", "");
	fclose(f);

	TEST_E(f = fmemopen(buf, 10, "a+"));
	TEST(i, ftell(f), 5, "%d != %d");
	TEST_E(fseek(f, 0, SEEK_SET)>=0);
	TEST(i, getc(f), 'h', "%d != %d");
	TEST(i, getc(f), 'e', "%d != %d");
	TEST(i, getc(f), 'l', "%d != %d");
	TEST(i, getc(f), 'l', "%d != %d");
	TEST(i, getc(f), 'o', "%d != %d");
	TEST(i, getc(f), EOF, "%d != %d");
	TEST_E(fseek(f, 6, SEEK_SET)>=0);
	TEST(i, ftell(f), 6, "%d != %d");
	TEST(i, getc(f), EOF, "%d != %d");
	TEST(i, ftell(f), 6, "%d != %d");
	TEST_E(fseek(f, 0, SEEK_SET)>=0);
	TEST(i, getc(f), 'h', "%d != %d");
	TEST_E(fseek(f, 0, SEEK_CUR)>=0);
	buf[7] = 'x';
	TEST_E(fprintf(f, "%d", i)==3);
	TEST_E(fflush(f)==0);
	TEST(i, ftell(f), 8, "%d != %d");
	TEST_S(buf, "hello104", "");
	fclose(f);
	return t_status;
}
