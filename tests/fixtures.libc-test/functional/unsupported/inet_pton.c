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

// inet_addr, inet_ntoa, inet_pton and inet_ntop tests with roundtrip check
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

static int digit(int c)
{
	c-='0';
	if (c>9) c-='a'-'0'-10;
	return c;
}

static void tobin(void *d, char *s)
{
	int i;
	unsigned char *p = d;
	for (i=0; s[2*i]; i++) p[i] = digit(s[2*i])*16+digit(s[2*i+1]);
}

static void tohex(char *d, void *s, int n)
{
	int i;
	unsigned char *p = s;
	for (i=0; i<n; i++) sprintf(d+2*i, "%02x", p[i]);
}

#define V6(src,ret,hex) do{\
	int r; \
	char binaddr[16]={0}; \
	char hexaddr[40]={0}; \
	char txtaddr[60]={0}; \
	\
	r=inet_pton(AF_INET6,src,binaddr); \
	if (r!=ret) \
		t_error("inet_pton(AF_INET6, "#src", addr) returned %d, want %d\n", r, ret); \
	if (ret!=1) break; \
	tohex(hexaddr,binaddr,16); \
	if (strcmp(hexaddr,hex)) \
		t_error("inet_pton(AF_INET6, "#src", addr) got addr %s, want %s\n", hexaddr, hex); \
	\
	tobin(binaddr,hex); \
	if (inet_ntop(AF_INET6,binaddr,txtaddr,sizeof txtaddr)!=txtaddr) \
		t_error("inet_ntop(AF_INET6, <"#hex">, buf, size) did not return buf\n"); \
	if (inet_pton(AF_INET6,txtaddr,binaddr)!=1) \
		t_error("inet_ntop(AF_INET6, <"#hex">, buf, size) got %s, it is rejected by inet_pton\n", txtaddr); \
	tohex(hexaddr,binaddr,16); \
	if (strcmp(hexaddr,hex)) \
		t_error("inet_ntop(AF_INET6, <"#hex">, buf, size) got %s that is %s, want %s\n", txtaddr, hexaddr, hex); \
	if (strncmp(hex,"00000000000000000000ffff",24)==0 && !strchr(txtaddr,'.')) \
		t_error("inet_ntop(AF_INET6, <"#hex">, buf, size) got %s, should be ipv4 mapped\n", txtaddr); \
}while(0);

// ret and hex are the results of inet_pton and inet_addr respectively
#define V4(src,ret,hex) do{\
	int r; \
	uint32_t a; \
	struct in_addr in; \
	char buf[20]={0}; \
	char *p; \
	\
	a=inet_addr(src); \
	tohex(buf,&a,4); \
	if (strcmp(buf,hex)) \
		t_error("inet_addr("#src") returned %s, want %s\n", buf, hex); \
	\
	r=inet_pton(AF_INET,src,&a); \
	if (r!=ret) \
		t_error("inet_pton(AF_INET, "#src", addr) returned %d, want %d\n", r, ret); \
	\
	if (ret!=1) break; \
	\
	tohex(buf,&a,4); \
	if (strcmp(buf,hex)) \
		t_error("inet_pton(AF_INET, "#src", addr) got addr %s, want %s\n", buf, hex); \
	\
	tobin(&a,hex); \
	if (inet_ntop(AF_INET,&a,buf,sizeof buf)!=buf) \
		t_error("inet_ntop(AF_INET, <"#hex">, buf, size) did not return buf\n"); \
	if (strcmp(buf,src)) \
		t_error("inet_ntop(AF_INET, <"#hex">, buf, size) got %s, want %s\n", buf, src); \
	\
	in.s_addr = a; \
	p=inet_ntoa(in); \
	if (strcmp(p,src)) \
		t_error("inet_ntoa(<"#hex">) returned %s, want %s\n", p, src); \
}while(0);

int main(void)
{

// errors
if (inet_pton(12345, "", 0) != -1 || errno != EAFNOSUPPORT)
	t_error("inet_pton(12345,,) should fail with EAFNOSUPPORT, got %s\n", strerror(errno));
errno=0;
if (inet_ntop(AF_INET,"xxxx","",0) != 0 || errno != ENOSPC)
	t_error("inet_ntop(,,0,0) should fail with ENOSPC, got %s\n", strerror(errno));
errno=0;

// dotted-decimal notation
V4("0.0.0.0", 1, "00000000")
V4("127.0.0.1", 1, "7f000001")
V4("10.0.128.31", 1, "0a00801f")
V4("255.255.255.255", 1, "ffffffff")

// numbers-and-dots notation, but not dotted-decimal
V4("1.2.03.4", 0, "01020304")
V4("1.2.0x33.4", 0, "01023304")
V4("1.2.0XAB.4", 0, "0102ab04")
V4("1.2.0xabcd", 0, "0102abcd")
V4("1.0xabcdef", 0, "01abcdef")
V4("00377.0x0ff.65534", 0, "fffffffe")

// invalid
V4(".1.2.3", 0, "ffffffff")
V4("1..2.3", 0, "ffffffff")
V4("1.2.3.", 0, "ffffffff")
V4("1.2.3.4.5", 0, "ffffffff")
V4("1.2.3.a", 0, "ffffffff")
V4("1.256.2.3", 0, "ffffffff")
V4("1.2.4294967296.3", 0, "ffffffff")
V4("1.2.-4294967295.3", 0, "ffffffff")
V4("1.2. 3.4", 0, "ffffffff")

// ipv6
V6(":", 0, "")
V6("::", 1, "00000000000000000000000000000000")
V6("::1", 1, "00000000000000000000000000000001")
V6(":::", 0, "")
V6("192.168.1.1", 0, "")
V6(":192.168.1.1", 0, "")
V6("::192.168.1.1", 1, "000000000000000000000000c0a80101")
V6("0:0:0:0:0:0:192.168.1.1", 1, "000000000000000000000000c0a80101")
V6("0:0::0:0:0:192.168.1.1", 1, "000000000000000000000000c0a80101")
V6("::012.34.56.78", 0, "")
V6(":ffff:192.168.1.1", 0, "")
V6("::ffff:192.168.1.1", 1, "00000000000000000000ffffc0a80101")
V6(".192.168.1.1", 0, "")
V6(":.192.168.1.1", 0, "")
V6("a:0b:00c:000d:E:F::", 1, "000a000b000c000d000e000f00000000")
V6("a:0b:00c:000d:0000e:f::", 0, "")
V6("1:2:3:4:5:6::", 1, "00010002000300040005000600000000")
V6("1:2:3:4:5:6:7::", 1, "00010002000300040005000600070000")
V6("1:2:3:4:5:6:7:8::", 0, "")
V6("1:2:3:4:5:6:7::9", 0, "")
V6("::1:2:3:4:5:6", 1, "00000000000100020003000400050006")
V6("::1:2:3:4:5:6:7", 1, "00000001000200030004000500060007")
V6("::1:2:3:4:5:6:7:8", 0, "")
V6("a:b::c:d:e:f", 1, "000a000b00000000000c000d000e000f")
V6("ffff:c0a8:5e4", 0, "")
V6(":ffff:c0a8:5e4", 0, "")
V6("0:0:0:0:0:ffff:c0a8:5e4", 1, "00000000000000000000ffffc0a805e4")
V6("0:0:0:0:ffff:c0a8:5e4", 0, "")
V6("0::ffff:c0a8:5e4", 1, "00000000000000000000ffffc0a805e4")
V6("::0::ffff:c0a8:5e4", 0, "")
V6("c0a8", 0, "")

return t_status;
}
