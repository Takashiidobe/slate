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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <errno.h>

#define set(k,v) do{ \
	e = hsearch((ENTRY){.key = k, .data = (void*)v}, ENTER); \
	if (!e || strcmp(e->key, k) != 0) \
		t_error("hsearch ENTER %s %d failed\n", k, v); \
}while(0)

#define get(k) hsearch((ENTRY){.key = k, .data = 0}, FIND)

#define getdata(e) ((intptr_t)(e)->data)

int main()
{
	ENTRY *e;

	if (hcreate(-1) || errno != ENOMEM)
		t_error("hcreate((size_t)-1) should fail with ENOMEM got %s\n", strerror(errno));
	if (!hcreate(13))
		t_error("hcreate(13) failed\n");
	set("", 0);
	set("a", 1);
	set("b", 2);
	set("abc", 3);
	set("cd", 4);
	set("e", 5);
	set("ef", 6);
	set("g", 7);
	set("h", 8);
	set("iiiiiiiiii", 9);
	if (!get("a"))
		t_error("hsearch FIND a failed\n");
	if (get("c"))
		t_error("hsearch FIND c should fail\n");
	set("g", 10);
	if (e && getdata(e) != 7)
		t_error("hsearch ENTER g 10 returned data %d, wanted 7\n", getdata(e));
	set("g", 10);
	if (e && getdata(e) != 7)
		t_error("hsearch ENTER g 10 returned data %d, wanted 7\n", getdata(e));
	set("j", 10);
	if (e && getdata(e) != 10)
		t_error("hsearch ENTER j 10 returned data %d, wanted 10\n", getdata(e));
	hdestroy();
	return t_status;
}
