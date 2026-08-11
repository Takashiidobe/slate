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

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

static const char path[] = ".";
static const int id = 'h';

#define T(f) do{ \
	if ((f)+1 == 0) \
		t_error("%s failed: %s\n", #f, strerror(errno)); \
}while(0)

#define EQ(a,b,fmt) do{ \
	if ((a) != (b)) \
		t_error("%s == %s failed: " fmt "\n", #a, #b, a, b); \
}while(0)

static void set()
{
	time_t t;
	key_t k;
	int shmid;
	struct shmid_ds shmid_ds;
	void *p;

	T(t = time(0));
	T(k = ftok(path, id));

	/* make sure we get a clean shared memory id */
	T(shmid = shmget(k, 100, IPC_CREAT|0666));
	T(shmctl(shmid, IPC_RMID, 0));
	T(shmid = shmget(k, 100, IPC_CREAT|IPC_EXCL|0666));

	if (t_status)
		exit(t_status);

	/* check IPC_EXCL */
	errno = 0;
	if (shmget(k, 100, IPC_CREAT|IPC_EXCL|0666) != -1 || errno != EEXIST)
		t_error("shmget(IPC_CREAT|IPC_EXCL) should have failed with EEXIST, got %s\n", strerror(errno));

	/* check if shmget initilaized the msshmid_ds structure correctly */
	T(shmctl(shmid, IPC_STAT, &shmid_ds));
	EQ(shmid_ds.shm_perm.cuid, geteuid(), "got %d, want %d");
	EQ(shmid_ds.shm_perm.uid, geteuid(), "got %d, want %d");
	EQ(shmid_ds.shm_perm.cgid, getegid(), "got %d, want %d");
	EQ(shmid_ds.shm_perm.gid, getegid(), "got %d, want %d");
	EQ(shmid_ds.shm_perm.mode & 0x1ff, 0666, "got %o, want %o");
	EQ(shmid_ds.shm_segsz, 100, "got %d, want %d");
	EQ(shmid_ds.shm_lpid, 0, "got %d, want %d");
	EQ(shmid_ds.shm_cpid, getpid(), "got %d, want %d");
	EQ((int)shmid_ds.shm_nattch, 0, "got %d, want %d");
	EQ((long long)shmid_ds.shm_atime, 0, "got %lld, want %d");
	EQ((long long)shmid_ds.shm_dtime, 0, "got %lld, want %d");
	if (shmid_ds.shm_ctime < t)
		t_error("shmid_ds.shm_ctime >= t failed: got %lld, want >= %lld\n", (long long)shmid_ds.shm_ctime, (long long)t);
	if (shmid_ds.shm_ctime > t+5)
		t_error("shmid_ds.shm_ctime <= t+5 failed: got %lld, want <= %lld\n", (long long)shmid_ds.shm_ctime, (long long)t+5);

	/* test attach */
	if ((p=shmat(shmid, 0, 0)) == 0)
		t_error("shmat failed: %s\n", strerror(errno));
	T(shmctl(shmid, IPC_STAT, &shmid_ds));
	EQ((int)shmid_ds.shm_nattch, 1, "got %d, want %d");
	EQ(shmid_ds.shm_lpid, getpid(), "got %d, want %d");
	if (shmid_ds.shm_atime < t)
		t_error("shm_atime is %lld want >= %lld\n", (long long)shmid_ds.shm_atime, (long long)t);
	if (shmid_ds.shm_atime > t+5)
		t_error("shm_atime is %lld want <= %lld\n", (long long)shmid_ds.shm_atime, (long long)t+5);
	strcpy(p, "test data");
	T(shmdt(p));
}

static void get()
{
	key_t k;
	int shmid;
	void *p;

	T(k = ftok(path, id));
	T(shmid = shmget(k, 0, 0));

	errno = 0;
	if ((p=shmat(shmid, 0, SHM_RDONLY)) == 0)
		t_error("shmat failed: %s\n", strerror(errno));

	if (strcmp(p, "test data") != 0)
		t_error("reading shared mem failed: got \"%.100s\" want \"test data\"\n", p);

	/* cleanup */
	T(shmdt(p));
	T(shmctl(shmid, IPC_RMID, 0));
}

int main(void)
{
	int p;
	int status;

	set();
	p = fork();
	if (p == -1)
		t_error("fork failed: %s\n", strerror(errno));
	else if (p == 0)
		get();
	else {
		T(waitpid(p, &status, 0));
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
			t_error("child exit status: %d\n", status);
	}
	return t_status;
}
