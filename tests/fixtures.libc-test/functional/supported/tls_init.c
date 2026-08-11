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

#include <pthread.h>

__thread int tls_fix = 23;
__thread int tls_zero;

static void *f(void *arg)
{
	if (tls_fix != 23)
		t_error("fixed init failed: want 23 got %d\n", tls_fix);
	if (tls_zero != 0)
		t_error("zero init failed: want 0 got %d\n", tls_zero);
	tls_fix++;
	tls_zero++;
	return 0;
}

#define CHECK(f) do{ if(f) t_error("%s failed.\n", #f); }while(0)
#define length(a) (sizeof(a)/sizeof*(a))

int main()
{
	pthread_t t[5];
	int i, j;

	if (tls_fix != 23)
		t_error("fixed init failed: want 23 got %d\n", tls_fix);
	if (tls_zero != 0)
		t_error("zero init failed: want 0 got %d\n", tls_zero);

	for (j = 0; j < 2; j++) {
		for (i = 0; i < length(t); i++) {
			CHECK(pthread_create(t+i, 0, f, 0));
			tls_fix++;
			tls_zero++;
		}
		for (i = 0; i < length(t); i++)
			CHECK(pthread_join(t[i], 0));
	}

	return t_status;
}
