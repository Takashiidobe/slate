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

#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

#define TEST(c, ...) ((c) ? 1 : (t_error(#c" failed: " __VA_ARGS__),0))

int main(void)
{
	struct stat st;
	FILE *f;
	time_t t;

	if (TEST(stat(".",&st)==0, "errno = %s\n", strerror(errno))) {
		TEST(S_ISDIR(st.st_mode), "\n");
		TEST(st.st_nlink>0, "%ju\n", (uintmax_t)st.st_nlink);
		t = time(0);
		TEST(st.st_ctime<=t, "%jd > %jd\n", (intmax_t)st.st_ctime, (intmax_t)t);
		TEST(st.st_mtime<=t, "%jd > %jd\n", (intmax_t)st.st_mtime, (intmax_t)t);
		TEST(st.st_atime<=t, "%jd > %jd\n", (intmax_t)st.st_atime, (intmax_t)t);
	}

	if (TEST(stat("/dev/null",&st)==0, "errno = %s\n", strerror(errno))) {
		TEST(S_ISCHR(st.st_mode), "\n");
	}

	if ((f = tmpfile())) {
		fputs("hello", f);
		fflush(f);
		if (TEST(fstat(fileno(f),&st)==0, "errnp = %s\n", strerror(errno))) {
			TEST(st.st_uid==geteuid(), "%d vs %d\n", (int)st.st_uid, (int)geteuid());
			TEST(st.st_gid==getegid(), "%d vs %d\n", (int)st.st_gid, (int)getegid());
			TEST(st.st_size==5, "%jd vs 5\n", (intmax_t)st.st_size);
		}
		fclose(f);
	}

	return t_status;
}
