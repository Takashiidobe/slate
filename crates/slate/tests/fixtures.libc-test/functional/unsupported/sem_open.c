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

/* named semaphore sanity check */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#define TEST(c, ...) \
	( (c) || (t_error(#c " failed: " __VA_ARGS__),0) )

int main(void)
{
	char buf[100];
	struct timespec ts;
	sem_t *sem, *sem2;
	int val;

	clock_gettime(CLOCK_REALTIME, &ts);
	snprintf(buf, sizeof buf, "/testsuite-%d-%d", (int)getpid(), (int)ts.tv_nsec);

	TEST((sem=sem_open(buf, O_CREAT|O_EXCL, 0700, 1)) != SEM_FAILED,
		"could not open sem: %s\n", strerror(errno));
	errno = 0;
	TEST(sem_open(buf, O_CREAT|O_EXCL, 0700, 1) == SEM_FAILED,
		"reopening should fail with O_EXCL\n");
	TEST(errno == EEXIST,
		"after reopen failure errno is \"%s\" (%d); want EEXIST (%d)\n", strerror(errno), errno, EEXIST);

	TEST(sem_getvalue(sem, &val) == 0, "failed to get sem value\n");
	TEST(val == 1, "wrong initial semaphore value: %d\n", val);

	TEST((sem2=sem_open(buf, 0)) == sem,
		"could not reopen sem: got %p, want %p\n", sem2, sem);

	errno = 0;
	TEST(sem_wait(sem) == 0, "%s\n", strerror(errno));
	TEST(sem_getvalue(sem2, &val) == 0, "%s\n", strerror(errno));
	TEST(val == 0, "wrong semaphore value on second handle: %d\n", val);

	errno = 0;
	TEST(sem_trywait(sem) == -1 && errno == EAGAIN,
		"trywait on locked sem: got errno \"%s\" (%d), want EAGAIN (%d)\n", strerror(errno), errno, EAGAIN);

	TEST(sem_post(sem) == 0, "%s\n", strerror(errno));
	TEST(sem_getvalue(sem2, &val) == 0, "%s\n", strerror(errno));
	TEST(val == 1, "wrong semaphore value on second handle: %d\n", val);

	TEST(sem_close(sem) == 0, "%s\n", strerror(errno));
	TEST(sem_close(sem) == 0, "%s\n", strerror(errno));
	TEST(sem_unlink(buf) == 0, "%s\n", strerror(errno));
	return t_status;
}
