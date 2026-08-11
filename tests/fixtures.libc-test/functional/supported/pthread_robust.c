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
#include <string.h>
#include <errno.h>

#define TEST(r, f, m) ( \
	((r) = (f)) == 0 || (t_error("%s failed: (pshared==%d, pi==%d) %s (" m ")\n", #f, pshared, pi, strerror(r)), 0) )
#define TESTX(r, f, x, m) ( \
	((r) = (f)) == (x) || (t_error("%s failed: (pshared==%d, pi==%d) got %d \"%s\" want %d \"%s\" (" m ")\n", #f, pshared, pi, r, strerror(r), x, strerror(x)), 0) )

static void *start_lock(void *arg)
{
	pthread_mutex_lock(arg);
	return 0;
}

static void *start_wait(void *arg)
{
	void **args = arg;
	pthread_mutex_lock(args[1]);
	pthread_barrier_wait(args[0]);
	nanosleep(&(struct timespec){ .tv_nsec = 10000000 }, 0);
	return 0;
}

void f(int pshared, int pi)
{
	pthread_t td;
	int r;
	void *res;
	pthread_barrier_t barrier2;
	pthread_mutexattr_t mtx_a;
	pthread_mutex_t mtx;

	TEST(r, pthread_barrier_init(&barrier2, 0, 2), "creating barrier");

	/* Robust mutexes */
	TEST(r, pthread_mutexattr_init(&mtx_a), "initializing mutex attr");
	TEST(r, pthread_mutexattr_setrobust(&mtx_a, PTHREAD_MUTEX_ROBUST), "setting robust attribute");
	if (pshared)
		TEST(r, pthread_mutexattr_setpshared(&mtx_a, PTHREAD_PROCESS_SHARED), "setting pshared attribute");
	if (pi)
		TEST(r, pthread_mutexattr_setprotocol(&mtx_a, PTHREAD_PRIO_INHERIT), "setting PI attribute");
	TEST(r, pthread_mutex_init(&mtx, &mtx_a), "initializing robust mutex");
	TEST(r, pthread_mutex_lock(&mtx), "locking robust mutex");
	TEST(r, pthread_mutex_unlock(&mtx), "unlocking robust mutex");
	TEST(r, pthread_create(&td, 0, start_lock, &mtx), "failed to create thread");
	TEST(r, pthread_join(td, &res), "joining thread");
	TESTX(r, pthread_mutex_lock(&mtx), EOWNERDEAD, "locking orphaned robust mutex");
	TEST(r, pthread_mutex_unlock(&mtx), "unlocking orphaned robust mutex");
	TESTX(r, pthread_mutex_lock(&mtx), ENOTRECOVERABLE, "re-locking orphaned robust mutex");
	TEST(r, pthread_mutex_destroy(&mtx), "destroying unrecoverable mutex");

	TEST(r, pthread_mutex_init(&mtx, &mtx_a), "initializing robust mutex");
	TEST(r, pthread_create(&td, 0, start_lock, &mtx), "failed to create thread");
	TEST(r, pthread_join(td, &res), "joining thread");
	TESTX(r, pthread_mutex_lock(&mtx), EOWNERDEAD, "locking orphaned robust mutex");
	TEST(r, pthread_mutex_consistent(&mtx), "");
	TEST(r, pthread_mutex_unlock(&mtx), "unlocking orphaned robust mutex");
	TEST(r, pthread_mutex_lock(&mtx), "re-locking orphaned robust mutex");
	TEST(r, pthread_mutex_destroy(&mtx), "destroying mutex");

	TEST(r, pthread_mutex_init(&mtx, &mtx_a), "");
	TEST(r, pthread_create(&td, 0, start_wait, (void *[]){ &barrier2, &mtx }), "");
	r = pthread_barrier_wait(&barrier2);
	if (r && r != PTHREAD_BARRIER_SERIAL_THREAD)
		t_error("pthread_barrier_wait failed: got %d \"%s\", wanted either 0 or %d\n",
			r, strerror(r), PTHREAD_BARRIER_SERIAL_THREAD);
	TEST(r, pthread_barrier_destroy(&barrier2), "");
	TESTX(r, pthread_mutex_lock(&mtx), EOWNERDEAD, "");
	TEST(r, pthread_join(td, &res), "");
	TEST(r, pthread_mutex_consistent(&mtx), "");
	TEST(r, pthread_mutex_unlock(&mtx), "");
	TEST(r, pthread_mutex_destroy(&mtx), "");
}

int main(void)
{
	for (int pshared=0; pshared<=1; pshared++)
		for (int pi=0; pi<=1; pi++)
			f(pshared, pi);
	return t_status;
}
