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
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <fcntl.h>

#define TEST(c, ...) ((c) ? 1 : (t_error(#c" failed: " __VA_ARGS__),0))
#define TESTE(c) (errno=0, TEST(c, "errno = %s\n", strerror(errno)))

int main(void)
{
	struct sockaddr_in sa = { .sin_family = AF_INET };
	int s, c, t;
	char buf[100];

	TESTE((s=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP))>=0);
	TESTE(bind(s, (void *)&sa, sizeof sa)==0);
	TESTE(getsockname(s, (void *)&sa, (socklen_t[]){sizeof sa})==0);

	TESTE(setsockopt(s, SOL_SOCKET, SO_RCVTIMEO,
		&(struct timeval){.tv_usec=1}, sizeof(struct timeval))==0);

	TESTE((c=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP))>=0);
	sa.sin_addr.s_addr = htonl(0x7f000001);
	TESTE(sendto(c, "x", 1, 0, (void *)&sa, sizeof sa)==1);
	TESTE(recvfrom(s, buf, sizeof buf, 0, (void *)&sa, (socklen_t[]){sizeof sa})==1);
	TEST(buf[0]=='x', "'%c'\n", buf[0]);

	close(c);
	close(s);

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	TESTE((s=socket(PF_INET, SOCK_STREAM|SOCK_CLOEXEC, IPPROTO_TCP))>=0);
	TEST(fcntl(s, F_GETFD)&FD_CLOEXEC, "SOCK_CLOEXEC did not work\n");
	TESTE(bind(s, (void *)&sa, sizeof sa)==0);
	TESTE(getsockname(s, (void *)&sa, (socklen_t[]){sizeof sa})==0);
	sa.sin_addr.s_addr = htonl(0x7f000001);

	TESTE(listen(s, 1)==0);

	TESTE((c=socket(PF_INET, SOCK_STREAM|SOCK_NONBLOCK, IPPROTO_TCP))>=0);
	TEST(fcntl(c, F_GETFL)&O_NONBLOCK, "SOCK_NONBLOCK did not work\n");

	TESTE(connect(c, (void *)&sa, sizeof sa)==0 || errno==EINPROGRESS);

	TESTE((t=accept(s, (void *)&sa, &(socklen_t){sizeof sa}))>=0);

	close(t);
	close(c);
	close(s);

	return t_status;
}
