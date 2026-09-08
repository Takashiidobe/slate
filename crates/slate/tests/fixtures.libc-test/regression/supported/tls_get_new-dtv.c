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

// __tls_get_new did not allocate new dtv for threads properly
#include <pthread.h>
#include <dlfcn.h>

#define N 10

#define T(c) ((c) || (t_error(#c " failed\n"),0))

static pthread_barrier_t b;
static void *mod;

static void *start(void *a)
{
	void *(*f)(void);

	pthread_barrier_wait(&b);
	T(f = dlsym(mod, "f"));
	f();
	return 0;
}

int main()
{
	pthread_t td[N];
	int i;

	pthread_barrier_init(&b, 0, N+1);
	for (i=0; i<N; i++)
		T(!pthread_create(td+i, 0, start, 0));

	T(mod = dlopen("tls_get_new-dtv_dso.so", RTLD_NOW));
	pthread_barrier_wait(&b);

	for (i=0; i<N; i++)
		T(!pthread_join(td[i], 0));
	return t_status;
}
