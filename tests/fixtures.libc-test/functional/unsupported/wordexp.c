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

#define _GNU_SOURCE 1
#include <wordexp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define CHK_X(c, fatal) do { \
	errno = 0; \
	if (!(c)) { \
		t_error("%s failed: %s\n", #c, strerror(errno)); \
		if (fatal) exit(t_status); \
	} \
} while(0)

#define CHK(c) CHK_X(c,0)
#define CHK_FATAL(c) CHK_X(c,1)

static const struct flag {
	int val;
	char *str;
} wrde_flags[] = {
#define C(x) {x, #x},
	C(WRDE_DOOFFS)
	C(WRDE_APPEND)
	C(WRDE_NOCMD)
	C(WRDE_REUSE)
	C(WRDE_SHOWERR)
	C(WRDE_UNDEF)
#undef C
	{ 0, NULL },
};

static char *flagstr(int flags)
{
	const struct flag *p;
	static char buf[256];
	char *sep = "";
	int n = 0;
	for (p = wrde_flags; p->str; p++)
		if((flags & p->val) == p->val) {
			n += sprintf(buf+n, "%s%s", sep, p->str);
			sep = "|";
			flags &= ~p->val;
		}
	if (flags || !*sep)
		sprintf(buf+n, "%s%#x", sep, flags);
	return buf;
}

static char *retstr(int r)
{
	switch (r) {
	case 0: return "0";
#define C(x) case WRDE_##x: return "WRDE_"#x;
	C(NOSPACE)
	C(BADCHAR)
	C(BADVAL)
	C(CMDSUB)
	C(SYNTAX)
#undef C
	}
	return "WRDE_?";
}

static char *join(wordexp_t *we)
{
	static char buf[512];
	size_t i, n = 0;

	buf[0] = 0;
	for (i = 0; i < we->we_wordc && n < sizeof buf; i++)
		n += snprintf(buf+n, sizeof buf-n, "%s|", we->we_wordv[we->we_offs+i]);
	return buf;
}

static int wordexp_q(const char *s, wordexp_t *we, int flags, char *buf, size_t sz)
{
	static FILE *err;
	int r;

	if (!err) {
		err = tmpfile();
		CHK_FATAL(err);
		CHK_FATAL(dup2(fileno(err), 2) >= 0);
	}
	r = wordexp(s, we, flags);
	CHK(fseek(err, 0, SEEK_SET) == 0);
	if (!fgets(buf, sz, err))
		buf[0] = 0;
	CHK(fseek(err, 0, SEEK_SET) == 0);
	CHK(ftruncate(fileno(err), 0) == 0);
	return r;
}

static void glob_ok(const char *pat, const char *out)
{
	wordexp_t we;
	char *got;
	int r = wordexp(pat, &we, 0);

	if (r) {
		t_error("wordexp(\"%s\") returned %s\n", pat, retstr(r));
		return;
	}
	got = join(&we);
	if (strcmp(got, out))
		t_error("wordexp(\"%s\") gave [%s], want [%s]\n", pat, got, out);
	wordfree(&we);
}

static struct {
	const char *in;
	int flags;
	int ret;
	int n;
	const char *out;
	int altret; /* if multiple err codes are acceptable */
} tests[] = {
	{ "one two three",   0, 0, 3, "one|two|three|" },
	{ "",                0, 0, 0, "" },
	{ "   ",             0, 0, 0, "" },
	{ "  a  b  ",        0, 0, 2, "a|b|" },
	{ "a $FOO b",        0, 0, 4, "a|bar|baz|b|" },
	{ "$FOO$FOO",        0, 0, 3, "bar|bazbar|baz|" },
	{ "$FOO $FOO",       0, 0, 4, "bar|baz|bar|baz|" },
	{ "a\tb",            0, 0, 2, "a|b|" },

	{ "'a b'",           0, 0, 1, "a b|" },
	{ "\"a b\"",         0, 0, 1, "a b|" },
	{ "a' 'b",           0, 0, 1, "a b|" },
	{ "''",              0, 0, 1, "|" },
	{ "\"\"",            0, 0, 1, "|" },
	{ "a''b",            0, 0, 1, "ab|" },
	{ "a\"\"b",          0, 0, 1, "ab|" },
	{ "'\"'",            0, 0, 1, "\"|" },
	{ "\"'\"",           0, 0, 1, "'|" },
	{ "a\\ b",           0, 0, 1, "a b|" },
	{ "\\$FOO",          0, 0, 1, "$FOO|" },
	{ "\"a\"'b'\"c\"",   0, 0, 1, "abc|" },
	{ "''\"\"",          0, 0, 1, "|" },

	{ "$FOO",            0, 0, 2, "bar|baz|" },
	{ "\"$FOO\"",        0, 0, 1, "bar baz|" },
	{ "a$FOO-b",         0, 0, 2, "abar|baz-b|" },
	{ "${FOO}x",         0, 0, 2, "bar|bazx|" },
	{ "$UNSET",          0, 0, 0, "" },
	{ "${UNSET:-def}",   0, 0, 1, "def|" },
	{ "${UNSET-def}",    0, 0, 1, "def|" },
	{ "${FOO:+y}",       0, 0, 1, "y|" },
	{ "${#FOO}",         0, 0, 1, "7|" },
	{ "${FOO%% *}",      0, 0, 1, "bar|" },
	{ "${FOO#* }",       0, 0, 1, "baz|" },
	{ "${UNSET:=x}",     0, 0, 1, "x|" },
	{ "${UNSET:+x}",     0, 0, 0, "" },
	{ "${UNSET-}",       0, 0, 0, "" },
	{ "${FOO:?}",        0, 0, 2, "bar|baz|" },
	{ "${FOO:-}",        0, 0, 2, "bar|baz|" },
	{ "${FOO=x}",        0, 0, 2, "bar|baz|" },

	{ "$((3*4))",        0, 0, 1, "12|" },
	{ "$((1+2*3))",      0, 0, 1, "7|" },
	{ "x$((2*8))y",      0, 0, 1, "x16y|" },
	{ "$(((1+2)*3))",    0, 0, 1, "9|" },
	{ "$((10%3))",       0, 0, 1, "1|" },
	{ "$((-5+3))",       0, 0, 1, "-2|" },

	{ "$(echo hi there)", 0, 0, 2, "hi|there|" },
	{ "`echo q`",        0, 0, 1, "q|" },
	{ "$(echo)",         0, 0, 0, "" },
	{ "\"$(echo a b)\"", 0, 0, 1, "a b|" },
	{ "$(echo 'a b')",   0, 0, 2, "a|b|" },
	{ "a$(echo b)c",     0, 0, 1, "abc|" },
	{ "$(echo a)$(echo b)", 0, 0, 1, "ab|" },
	{ "a`echo b`c",      0, 0, 1, "abc|" },
	{ "$(echo $(echo x))", 0, 0, 1, "x|" },
	{ "$(printf 'a\\nb')", 0, 0, 2, "a|b|" },
	{ "$(echo a; echo b)", 0, 0, 2, "a|b|" },
	{ "$(true)",         0, 0, 0, "" },

	{ "~",               0, 0, 1, "/home/syd|" },
	{ "~/sub",           0, 0, 1, "/home/syd/sub|" },
	{ "a~",              0, 0, 1, "a~|" },
	{ "\"~\"",           0, 0, 1, "~|" },

	{ "$1",              0, 0, 0, "" },
	{ "$2",              0, 0, 0, "" },
	{ "$*",              0, 0, 0, "" },
	{ "$@",              0, 0, 0, "" },
	{ "$#",              0, 0, 1, "0|" },

	{ "{",               0, 0, 1, "{|", WRDE_BADCHAR },
	{ "}",               0, 0, 1, "}|", WRDE_BADCHAR },
	{ "{",               WRDE_NOCMD, WRDE_BADCHAR },
	{ "}",               WRDE_NOCMD, WRDE_BADCHAR },
	{ ")",               0, WRDE_SYNTAX },
	{ "(",               0, WRDE_SYNTAX },
	{ "$(",              0, WRDE_SYNTAX },
	{ "$(echo",          0, WRDE_SYNTAX },
	{ "${",              0, WRDE_SYNTAX },
	{ "`",               0, WRDE_SYNTAX },
	{ "'unterminated",   0, WRDE_SYNTAX },
	{ "\"unterminated",  0, WRDE_SYNTAX },

	{ "$UNSET ) $UNSET",               WRDE_UNDEF, WRDE_SYNTAX },
	{ "$UNSET ( $UNSET",               WRDE_UNDEF, WRDE_SYNTAX },
	{ "$UNSET $( $UNSET",              WRDE_UNDEF, WRDE_SYNTAX },
	{ "$UNSET $(echo $UNSET",          WRDE_UNDEF, WRDE_SYNTAX },
	{ "$UNSET ${ $UNSET",              WRDE_UNDEF, WRDE_SYNTAX },
	{ "$UNSET ` $UNSET",               WRDE_UNDEF, WRDE_SYNTAX },
	{ "$UNSET 'unterminated $UNSET",   WRDE_UNDEF, WRDE_SYNTAX },
	{ "$UNSET \"unterminated $UNSET",  WRDE_UNDEF, WRDE_SYNTAX },

	{ "$UNSET ) $UNSET",               WRDE_UNDEF|WRDE_NOCMD, WRDE_BADCHAR },
	{ "$UNSET ( $UNSET",               WRDE_UNDEF|WRDE_NOCMD, WRDE_BADCHAR },
	{ "$UNSET $( $UNSET",              WRDE_UNDEF|WRDE_NOCMD, WRDE_CMDSUB },
	{ "$UNSET $(echo $UNSET",          WRDE_UNDEF|WRDE_NOCMD, WRDE_CMDSUB },
	{ "$UNSET ${ $UNSET",              WRDE_UNDEF|WRDE_NOCMD, WRDE_SYNTAX, 0, 0, WRDE_BADCHAR },
	{ "$UNSET ` $UNSET",               WRDE_UNDEF|WRDE_NOCMD, WRDE_CMDSUB },
	{ "$UNSET 'unterminated $UNSET",   WRDE_UNDEF|WRDE_NOCMD, WRDE_SYNTAX },
	{ "$UNSET \"unterminated $UNSET",  WRDE_UNDEF|WRDE_NOCMD, WRDE_SYNTAX },

	{ "#!@$^%&*(_+=<<-0>[{`${",        WRDE_NOCMD, 0, 0, 0 },
	{ "#'\necho x\\'",   WRDE_NOCMD, WRDE_BADCHAR },

	{ "$(echo x)",       WRDE_NOCMD, WRDE_CMDSUB },
	{ "`echo x`",        WRDE_NOCMD, WRDE_CMDSUB },
	{ "a$(echo x)b",     WRDE_NOCMD, WRDE_CMDSUB },
	{ "a`b",             WRDE_NOCMD, WRDE_CMDSUB },
	{ "\"$(echo x)\"",   WRDE_NOCMD, WRDE_CMDSUB },
	{ "$((echo foo) )",  WRDE_NOCMD, WRDE_CMDSUB, 0, 0, WRDE_SYNTAX },
	{ "$(case $A in a) echo x ;; *) echo y ;; esac)", 0, 0, 1, "x|" },
	{ "$(case $A in a) echo x ;; *) echo y ;; esac)", WRDE_NOCMD, WRDE_CMDSUB },
	{ "$((case $A in a) echo x ;; *) echo y ;; esac))", WRDE_NOCMD, WRDE_CMDSUB },
	{ "$((echo a);(echo b))", WRDE_NOCMD, WRDE_CMDSUB, 0, 0, WRDE_SYNTAX },
	/* Using $' from posix 2024, result may depend on sh version. */
	{ "$'\\''$(echo bad)\\'", WRDE_NOCMD, WRDE_CMDSUB, 0, 0, WRDE_SYNTAX },
	{ "$'\\''$(echo bad)\\'", 0, 0, 1, "'bad'|", WRDE_SYNTAX },
	{ "$'\\'$(echo bad)'\\'", WRDE_NOCMD, 0, 1, "'$(echo bad)'|", WRDE_SYNTAX },
	{ "$'\\n'$(echo bad)", WRDE_NOCMD, WRDE_CMDSUB },

	{ "a|b",             WRDE_NOCMD, WRDE_BADCHAR },
	{ "a&b",             WRDE_NOCMD, WRDE_BADCHAR },
	{ "a;b",             WRDE_NOCMD, WRDE_BADCHAR },
	{ "a<b",             WRDE_NOCMD, WRDE_BADCHAR },
	{ "a>b",             WRDE_NOCMD, WRDE_BADCHAR },
	{ "a{b",             WRDE_NOCMD, WRDE_BADCHAR },
	{ "a}b",             WRDE_NOCMD, WRDE_BADCHAR },
	{ "a\nb",            WRDE_NOCMD, WRDE_BADCHAR },
	{ "(",               WRDE_NOCMD, WRDE_BADCHAR },

	{ "$"
	"(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((("
	"(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((("
	"(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((("
	"(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((("
	"(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((("
	"1+$(((${X-1}+1)))"
	"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"
	"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"
	"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"
	"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"
	"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))", WRDE_NOCMD, 0, 1, "3|" },

	/* unquoted ${...} expansion */
	{ "${X:=${X:=x}$X}", 0, 0, 1, "xx|" },
	{ "${X:-x${NULL:-y${A:-z}}}", 0, 0, 1, "xya|" },
	{ "${X:-x}",         0, 0, 1, "x|" },
	{ "${X-x}",          0, 0, 1, "x|" },
	{ "${X:=x} $X",      0, 0, 2, "x|x|" },
	{ "${X=x} $X",       0, 0, 2, "x|x|" },
	{ "${X:?x}",         WRDE_SHOWERR, WRDE_SYNTAX },
	{ "${X?x}",          WRDE_SHOWERR, WRDE_SYNTAX },
	{ "${X:+x}",         0, 0, 0, "" },
	{ "${X+x}",          0, 0, 0, "" },
	{ "${#X}",           0, 0, 1, "0|" },
	{ "${A:-x}",         0, 0, 1, "a|" },
	{ "${A-x}",          0, 0, 1, "a|" },
	{ "${A:=x} $A",      0, 0, 2, "a|a|" },
	{ "${A=x} $A",       0, 0, 2, "a|a|" },
	{ "${A:?x}",         0, 0, 1, "a|" },
	{ "${A?x}",          0, 0, 1, "a|" },
	{ "${A:+x}",         0, 0, 1, "x|" },
	{ "${A+x}" ,         0, 0, 1, "x|" },
	{ "${#A}",           0, 0, 1, "1|" },
	{ "${NULL:-x}",      0, 0, 1, "x|" },
	{ "${NULL-x}",       0, 0, 0, "" },
	{ "${NULL:=x} $NULL", 0, 0, 2, "x|x|" },
	{ "${NULL=x} $NULL", 0, 0, 0, "" },
	{ "${NULL:?x}",      WRDE_SHOWERR, WRDE_SYNTAX },
	{ "${NULL?x}",       0, 0, 0, "" },
	{ "${NULL:+x}",      0, 0, 0, "" },
	{ "${NULL+x}",       0, 0, 1, "x|" },
	{ "${#NULL}",        0, 0, 1, "0|" },
	{ "${X:=x%} ${X%\\%}", 0, 0, 2, "x%|x|" },
	{ "${X:=#x} ${X#\\#}", 0, 0, 2, "#x|x|" },
	{ "${AB%*b}",        0, 0, 1, "a|" },
	{ "${AB%%*b}",       0, 0, 0, "" },
	{ "${AB#$A*}",       0, 0, 1, "b|" },
	{ "${AB##$A*}",      0, 0, 0, "" },

	{ "${X-\\'}",        0, 0, 1, "'|" },
	{ "${X-\\\"}",       0, 0, 1, "\"|" },
	{ "${X-\\}}",        0, 0, 1, "}|" },
	{ "${X-{}",          0, 0, 1, "{|" },
	{ "${X-\\{}",        0, 0, 1, "{|" },
	{ "${X-\\$A}",       0, 0, 1, "$A|" },
	{ "${X-\\`}",        0, 0, 1, "`|" },
	{ "${X-'}'}",        0, 0, 1, "}|" },
	{ "${X-'\"'}",       0, 0, 1, "\"|" },
	{ "${X-'}\"'}",      0, 0, 1, "}\"|" },
	{ "${X-'$'A}",       0, 0, 1, "$A|" },
	{ "${X-\"'\"}",      0, 0, 1, "'|" },
	{ "${X-\"$\"A}",     0, 0, 1, "$A|" },
	{ "${X-\"$\"{A}B}",  0, 0, 1, "${AB}|" },
	{ "${X-${Y-\\'}}",   0, 0, 1, "'|" },
	{ "${X-${Y-\\\"}}",  0, 0, 1, "\"|" },

	{ "${X-\\'}",        WRDE_NOCMD, 0, 1, "'|", WRDE_SYNTAX },
	{ "${X-\\\"}",       WRDE_NOCMD, 0, 1, "\"|", WRDE_SYNTAX },
	{ "${X-\\}}",        WRDE_NOCMD, 0, 1, "}|", WRDE_SYNTAX },
	{ "${X-{}",          WRDE_NOCMD, 0, 1, "{|" },
	{ "${X-\\{}",        WRDE_NOCMD, 0, 1, "{|" },
	{ "${X-\\$A}",       WRDE_NOCMD, 0, 1, "$A|", WRDE_SYNTAX },
	{ "${X-\\`}",        WRDE_NOCMD, 0, 1, "`|", WRDE_SYNTAX },
	{ "${X-'}'}",        WRDE_NOCMD, 0, 1, "}|", WRDE_SYNTAX },
	{ "${X-'\"'}",       WRDE_NOCMD, 0, 1, "\"|", WRDE_SYNTAX },
	{ "${X-'}\"'}",      WRDE_NOCMD, 0, 1, "}\"|", WRDE_SYNTAX },
	{ "${X-'$'A}",       WRDE_NOCMD, 0, 1, "$A|", WRDE_SYNTAX },
	{ "${X-\"'\"}",      WRDE_NOCMD, 0, 1, "'|", WRDE_SYNTAX },
	{ "${X-\"$\"A}",     WRDE_NOCMD, 0, 1, "$A|", WRDE_SYNTAX },
	{ "${X-\"$\"{A}B\\}", WRDE_NOCMD, 0, 1, "${AB}|", WRDE_SYNTAX },
	{ "${X-\"$\"{A}B}",  WRDE_NOCMD, WRDE_BADCHAR, 0, 0, WRDE_SYNTAX },
	{ "${X-${Y-\\'}}",   WRDE_NOCMD, 0, 1, "'|", WRDE_SYNTAX },
	{ "${X-${Y-\\\"}}",  WRDE_NOCMD, 0, 1, "\"|", WRDE_SYNTAX },

	/* quoted "${...}" expansion */
	{ "\"${X-\\'}\"",    0, 0, 1, "\\'|" },
	{ "\"${X-\\\"}\"",   0, 0, 1, "\"|" },
	{ "\"${X-\\}}\"",    0, 0, 1, "}|" },
	{ "\"${X-\\$A}\"",   0, 0, 1, "$A|" },
	{ "\"${X-\\`}\"",    0, 0, 1, "`|" },
	{ "\"${X-\"'\"}\"",  0, 0, 1, "'|" },
	{ "\"${X-${Y-\\'}}\"",  0, 0, 1, "\\'|" },
	{ "\"${X-${Y-\\\"}}\"", 0, 0, 1, "\"|" },

	{ "\"${X-\\'}\"",    WRDE_NOCMD, 0, 1, "\\'|", WRDE_SYNTAX },
	{ "\"${X-\\\"}\"",   WRDE_NOCMD, 0, 1, "\"|", WRDE_SYNTAX },
	{ "\"${X-\\}}\"",    WRDE_NOCMD, 0, 1, "}|", WRDE_SYNTAX },
	{ "\"${X-{}\"",      WRDE_NOCMD, 0, 1, "{|" },
	{ "\"${X-\\{}\"",    WRDE_NOCMD, 0, 1, "\\{|" },
	{ "\"${X-\\$A}\"",   WRDE_NOCMD, 0, 1, "$A|", WRDE_SYNTAX },
	{ "\"${X-\\`}\"",    WRDE_NOCMD, 0, 1, "`|", WRDE_SYNTAX },
	{ "\"${X-\"'\"}\"",  WRDE_NOCMD, 0, 1, "'|", WRDE_SYNTAX },
	{ "\"${X-${Y-\\'}}\"",  WRDE_NOCMD, 0, 1, "\\'|", WRDE_SYNTAX },
	{ "\"${X-${Y-\\\"}}\"", WRDE_NOCMD, 0, 1, "\"|", WRDE_SYNTAX },

	/* bash bug */
	{ "\"${X-\"$\"A}\"", WRDE_NOCMD, 0, 1, "$A|", WRDE_SYNTAX },
	{ "\"${X-\"$\"{A}B}\"", WRDE_NOCMD, 0, 1, "${AB}|", WRDE_SYNTAX },
	{ "\"${X-\"$\"Y}\"", WRDE_NOCMD|WRDE_UNDEF, 0, 1, "$Y|", WRDE_SYNTAX },
	{ "\"${X-\"$\"{Y\"}\"}\"", WRDE_NOCMD|WRDE_UNDEF, 0, 1, "${Y}|", WRDE_SYNTAX },
	{ "${X-\"$\"(echo x)}", WRDE_NOCMD, 0, 2, "$(echo|x)|", WRDE_SYNTAX },
	{ "\"${X-\"$\"(echo x)}\"", WRDE_NOCMD, 0, 1, "$(echo x)|", WRDE_SYNTAX },
	{ "$((${X-\"$\"(echo + 2)}))", WRDE_NOCMD, 0, 1, "2|", WRDE_SYNTAX },
	{ "$((${X-\"$\"((1+1))}))", WRDE_NOCMD, 0, 1, "$((1+1))|", WRDE_SYNTAX },
	{ "\"${X-'\"'}\"}\"\"'$(echo bad)'\"'\"\\'", 0, 0, 1, "''}'bad'\"\\|" },
#if 0
	/* both valid */
	{ "a\\",             WRDE_NOCMD, 0, 1, "a\\|", WRDE_SYNTAX },
	{ "a\\",             WRDE_NOCMD, 0, 1, "a|", WRDE_SYNTAX },

	/* broken on bash.. */
	{ "\"${X-\"$\"A}\"", 0, 0, 1, "$A|" },
	{ "\"${X-\"$\"{A}B}\"", 0, 0, 1, "${AB}|" },

	{ "\"${Y=x\\}} ${Y%${X-'}'}}\"", WRDE_NOCMD, 0, 1, "x} x|" },

	/* single-quote handling across shells is not portable. */
	{ "\"${X-'}'}\"",       WRDE_NOCMD, 0, 1, "''}|", WRDE_SYNTAX },
	{ "\"${X-'\\\"'}\"",    WRDE_NOCMD, 0, 1, "'\"'|", WRDE_SYNTAX },
	{ "\"${X-'}\\\"'}\"",   WRDE_NOCMD, 0, 1, "'\"'}|", WRDE_SYNTAX },
	{ "\"${X-'$'A}\"",      WRDE_NOCMD, 0, 1, "$A|", WRDE_SYNTAX },
	{ "\"${X-'$'A}\"",      WRDE_NOCMD, 0, 1, "$A|", WRDE_SYNTAX },
	{ "\"${X-'\"'}\"}\"'$(echo bad)\"\\'", WRDE_NOCMD, 0, 1, "''}$(echo bad)\"\\|", WRDE_SYNTAX },

	{ "$\"\"",              WRDE_NOCMD, 0, 1, "$|" }, // mksh
	{ "${X=1} $((${X-))}))", WRDE_NOCMD, 0, 2, "1|1|" },
	{ "$((cat<<x\n)\nx\n))",  WRDE_SHOWERR, 0, 1, ")|", WRDE_SYNTAX },
	{ "\"$((${x-'1'}))\"",  WRDE_NOCMD, 0, 1, "1|" },

	{ "${X=1} $((${X-\"${X-))}\"}))", WRDE_NOCMD, 0, 2, "1|1|" },
	{ "${X=2} $((${X-\"))\"}+1))", WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} $((${X-\")\"}+1))", WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} $((${X-\"')'\"}+1))", WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} \"$((${X-\"))\"}+1))\"", WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} \"$((${X-\")\"}+1))\"", WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} \"$((${X-\"')'\"}+1))\"", WRDE_NOCMD, 0, 2, "2|3|" },

	/* undef with cmd sub */
	{ "$(echo x$UNSET)y",     0, 0, 1, "xy|" },
	{ "$(echo x$UNSET)y",     WRDE_UNDEF, 0, 1, "y|", WRDE_BADVAL },
	{ "\"$(echo x$UNSET)y\"", 0, 0, 1, "xy|" },
	{ "\"$(echo x$UNSET)y\"", WRDE_UNDEF, 0, 1, "y|", WRDE_BADVAL },
	{ "`echo x$UNSET`y",      0, 0, 1, "xy|" },
	{ "`echo x$UNSET`y",      WRDE_UNDEF, 0, 1, "y|", WRDE_BADVAL },
	{ "\"`echo x$UNSET`y\"",  0, 0, 1, "xy|" },
	{ "\"`echo x$UNSET`y\"",  WRDE_UNDEF, 0, 1, "y|", WRDE_BADVAL },
#endif

	{ "\t @\t $\t %\t []",  WRDE_NOCMD, 0, 4, "@|$|%|[]|" },
	{ "${X} ${Y}",          WRDE_NOCMD, 0, 0, 0 },
	{ "$\\$",               WRDE_NOCMD, 0, 1, "$$|" },
	{ "$'$'",               WRDE_NOCMD, 0, 1, "$|" },
	{ "$)",                 WRDE_NOCMD, WRDE_BADCHAR },
	{ "$}",                 WRDE_NOCMD, WRDE_BADCHAR },
	{ "$: $~ $. $, $",      WRDE_NOCMD, 0, 5, "$:|$~|$.|$,|$|" },
	{ "\"$) $} $\\ $\"",    WRDE_NOCMD, 0, 1, "$) $} $\\ $|" },
	{ "${X-$''}",           WRDE_NOCMD, WRDE_SYNTAX },
	{ "${X-\"$'$(cmd)'y\"}", WRDE_NOCMD, WRDE_CMDSUB },
	{ "\"${X-$'$(cmd)'y}\"", WRDE_NOCMD, WRDE_SYNTAX },
	{ "$((${X-$'$(cmd)'y}))", WRDE_NOCMD, WRDE_SYNTAX },
	{ "$'\\'",              WRDE_NOCMD, WRDE_SYNTAX },
	{ "$'x'",               WRDE_NOCMD, 0, 1, "x|" },
	{ "\"$'x'\"",           WRDE_NOCMD, 0, 1, "$'x'|" },
	{ "${X=1} $((${X-'}))", WRDE_NOCMD, WRDE_SYNTAX },
	{ "${X=1} $((${X%'}))$(cmd)\'}))", WRDE_NOCMD, 0, 2, "1|1|" },
	{ "${X=12} $((${X%\"'$(cmd)'\"}))", WRDE_NOCMD, WRDE_CMDSUB },
	{ "${X=12} $((${X%\"2\"}))", WRDE_NOCMD, 0, 2, "12|1|" },
	{ "\"${X-'$'A}\"",      WRDE_NOCMD, WRDE_SYNTAX },
	{ "$((${X-\"$\"((1+1))}))", WRDE_NOCMD, WRDE_SYNTAX },
	{ "$((cmd #(\n)\n))",   WRDE_NOCMD, WRDE_SYNTAX, 0, 0, WRDE_CMDSUB },
	{ "$((cmd #(\n)) )",    WRDE_NOCMD, WRDE_SYNTAX, 0, 0, WRDE_CMDSUB },
	{ "$((cmd #(\n))\n)",   WRDE_NOCMD, WRDE_SYNTAX, 0, 0, WRDE_CMDSUB },
	{ "$((echo + 1 #)\n))", WRDE_NOCMD, WRDE_SYNTAX, 0, 0, WRDE_CMDSUB },
	{ "$((\"1\"))",         WRDE_NOCMD, WRDE_SYNTAX },
	{ "$((echo + 1 ) )",    WRDE_NOCMD, WRDE_CMDSUB },
	{ "$((case x in x) echo a;; esac) )", WRDE_NOCMD, WRDE_CMDSUB },
	{ "\"${X-))}\"",        WRDE_NOCMD, 0, 1, "))|" },
	{ "${X%${X-'}}",        WRDE_NOCMD, WRDE_SYNTAX },
	{ "${Y=x\\}} ${Y%${X-'}'}}", WRDE_NOCMD, 0, 2, "x}|x|" },
	{ "\"${AB%${X-'$(echo b)'}}\"", WRDE_NOCMD, 0, 1, "ab|" },
	{ "\"${AB%${X-'b'}}\"", WRDE_NOCMD, 0, 1, "a|" },

	{ "$((1+1))",             WRDE_NOCMD, 0, 1, "2|" },
	{ "${X=2} $((X+1))",      WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} $((((X)+(1))))", WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} $((X(+)1))",    WRDE_NOCMD, WRDE_SYNTAX },
	{ "${X=2} $(($X+1))",     WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} $((${X}+1))",   WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} $((${X-))}+1))", WRDE_NOCMD, 0, 2, "2|3|", WRDE_SYNTAX },
	{ "${X=2} $((${X-\\)\\)}+1))", WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} $((${X-$((}))}+1))", WRDE_NOCMD, 0, 2, "2|3|", WRDE_BADCHAR }, /* } in $(())*/
	{ "${X=2} $((${X-')'}+1))", WRDE_NOCMD, 0, 2, "2|3|", WRDE_SYNTAX },
	{ "$((${X-2}+1))",        WRDE_NOCMD, 0, 1, "3|" },
	{ "$(($((1+2))+3))",      WRDE_NOCMD, 0, 1, "6|" },
	{ "$(($((1+${X-2}))+3))", WRDE_NOCMD, 0, 1, "6|" },
	{ "$(($((1+${X-$((1+1))}))+3))", WRDE_NOCMD, 0, 1, "6|" },
	{ "$((cat<<x\n)(\nx\n))", WRDE_NOCMD, WRDE_CMDSUB, 0, 0, WRDE_BADCHAR },
	{ "$((echo x #)\n))",     WRDE_NOCMD, WRDE_SYNTAX, 0, 0, WRDE_CMDSUB },

	{ "\"$((1+1))\"",         WRDE_NOCMD, 0, 1, "2|" },
	{ "${X=2} \"$((X+1))\"",  WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} \"$(($X+1))\"", WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} \"$((((X)+(1))))\"", WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} \"$((X(+)1))\"", WRDE_NOCMD, WRDE_SYNTAX },
	{ "${X=2} \"$(($X+1))\"", WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} \"$((${X}+1))\"", WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} \"$((${X-))}+1))\"", WRDE_NOCMD, 0, 2, "2|3|", WRDE_SYNTAX },
	{ "${X=2} \"$((${X-\\)\\)}+1))\"", WRDE_NOCMD, 0, 2, "2|3|" },
	{ "${X=2} \"$((${X-$((}))}+1))\"", WRDE_NOCMD, 0, 2, "2|3|", WRDE_BADCHAR },
	{ "${X=2} \"$((${X-')'}+1))\"", WRDE_NOCMD, 0, 2, "2|3|", WRDE_SYNTAX },
	{ "\"$((${X-2}+1))\"",    WRDE_NOCMD, 0, 1, "3|" },
	{ "\"$(($((1+2))+3))\"",  WRDE_NOCMD, 0, 1, "6|" },
	{ "\"$(($((1+${X-2}))+3))\"", WRDE_NOCMD, 0, 1, "6|" },
	{ "\"$(($((1+${X-$((1+1))}))+3))\"", WRDE_NOCMD, 0, 1, "6|" },

	{ "${X- #x}",        WRDE_NOCMD, 0, 1, "#x|" },
	{ "${X-x }#${Y-y}",  WRDE_NOCMD, 0, 2, "x|#y|" },
	{ "x \\\n\\\n#);",   WRDE_NOCMD, 0, 1, "x|" },
	{ "$((1 + #x))\n1))", WRDE_NOCMD, WRDE_SYNTAX },
	{ "\" #x\"",         WRDE_NOCMD, 0, 1, " #x|" },
	{ " \\ #x",          WRDE_NOCMD, 0, 1, " #x|" },

	{ "'a;b'",           WRDE_NOCMD, 0, 1, "a;b|" },
	{ "\"a;b\"",         WRDE_NOCMD, 0, 1, "a;b|" },
	{ "a\\;b",           WRDE_NOCMD, 0, 1, "a;b|" },
	{ "\"a;b\"",         WRDE_NOCMD, 0, 1, "a;b|" },
	{ "'a&b'",           WRDE_NOCMD, 0, 1, "a&b|" },
	{ "a\\&b",           WRDE_NOCMD, 0, 1, "a&b|" },
	{ "'$(echo x)'",     WRDE_NOCMD, 0, 1, "$(echo x)|" },
	{ "$((1+1))",        WRDE_NOCMD, 0, 1, "2|" },
	{ "$((1+${X-$((1+(1)))}))", WRDE_NOCMD, 0, 1, "3|", WRDE_SYNTAX },
	{ "${FOO}",          WRDE_NOCMD, 0, 2, "bar|baz|", WRDE_SYNTAX },
	{ "~",               WRDE_NOCMD, 0, 1, "/home/syd|" },
	{ "/~",              WRDE_NOCMD, 0, 1, "/~|" },
	{ "~/",              WRDE_NOCMD, 0, 1, "/home/syd/|" },
	{ "${X-~}",          WRDE_NOCMD, 0, 1, "/home/syd|" },
	{ "\"${X-~}\"",      WRDE_NOCMD, 0, 1, "~|" },
	{ "$FOO",            WRDE_NOCMD, 0, 2, "bar|baz|" },

	{ "$FOO",            WRDE_SHOWERR, 0, 2, "bar|baz|" },
	{ "${UNSET:?unset}", WRDE_SHOWERR, WRDE_SYNTAX },

	{ "x $UNSET y",      WRDE_UNDEF, WRDE_BADVAL },
	{ "x ${UNSET} y",    WRDE_UNDEF, WRDE_BADVAL },
	{ "x \"$UNSET y\"",  WRDE_UNDEF, WRDE_BADVAL },
	{ "$((UNSET+1))",    WRDE_UNDEF, 0, 1, "1|", WRDE_BADVAL },
	{ "$1",              WRDE_UNDEF, WRDE_BADVAL },
	{ "$FOO",            WRDE_UNDEF, 0, 2, "bar|baz|" },
	{ "${FOO:-x}",       WRDE_UNDEF, 0, 2, "bar|baz|" },
	{ "${UNSET:-d}",     WRDE_UNDEF, 0, 1, "d|" },
	{ "${UNSET:-$FOO}",  WRDE_UNDEF, 0, 2, "bar|baz|" },
	{ "$(echo hi)",      WRDE_UNDEF, 0, 1, "hi|" },
	{ "\"$FOO\"",        WRDE_UNDEF, 0, 1, "bar baz|" },
	{ "",                WRDE_UNDEF, 0, 0, "" },
	{0}
};

int main(void)
{
	wordexp_t we;
	int i, r;

	CHK(unsetenv("X") == 0);
	CHK(unsetenv("Y") == 0);
	CHK(setenv("NULL", "", 1) == 0);
	CHK(setenv("A", "a", 1) == 0);
	CHK(setenv("AB", "ab", 1) == 0);
	CHK(setenv("FOO", "bar baz", 1) == 0);
	CHK(setenv("HOME", "/home/syd", 1) == 0);
	CHK(unsetenv("UNSET") == 0);

	for (i = 0; tests[i].in; i++) {
		const char *in = tests[i].in;
		int flags = tests[i].flags;
		char err[256];

		int want = tests[i].ret;
		int want_alt = tests[i].altret;
		const char *want_out = tests[i].out ? tests[i].out : "";
		size_t want_n = tests[i].n;

		r = wordexp_q(in, &we, flags, err, sizeof err);
		if (!(flags & WRDE_SHOWERR) && err[0])
			t_error("wordexp(\"%s\", %s) wrote to stderr: %s\n",
				in, flagstr(flags), err);
		if ((flags & WRDE_SHOWERR) && want && !err[0])
			t_error("wordexp(\"%s\", %s) did not write to stderr.\n",
				in, flagstr(flags));
		if (r != want && r != want_alt)
			t_error("wordexp(\"%s\", %s) returned %s, want %s\n",
				in, flagstr(flags), retstr(r), retstr(want));
//		else if (r && r == want_alt)
//			dprintf(1, "alt warn: \"%s\" %s got %s want %s\n",
//				in, flagstr(flags), retstr(r), retstr(want));
		if (!r) {
			char *got = join(&we);
			if (we.we_wordc != want_n || strcmp(got, want_out))
				t_error("wordexp(\"%s\", %s) gave %zu [%s], want %zu [%s]\n",
					in, flagstr(flags), we.we_wordc, got, want_n, want_out);
			wordfree(&we);
		}
	}

	we.we_offs = 2;
	r = wordexp("a b", &we, WRDE_DOOFFS);
	if (r)
		t_error("wordexp(WRDE_DOOFFS) returned %s\n", retstr(r));
	else {
		if (we.we_wordc != 2)
			t_error("WRDE_DOOFFS we_wordc %zu, want 2\n", we.we_wordc);
		if (we.we_wordv[0] || we.we_wordv[1])
			t_error("WRDE_DOOFFS leading slots not null\n");
		if (strcmp(we.we_wordv[2], "a") || strcmp(we.we_wordv[3], "b"))
			t_error("WRDE_DOOFFS words wrong: [%s] [%s]\n",
				we.we_wordv[2], we.we_wordv[3]);
		if (we.we_wordv[4])
			t_error("WRDE_DOOFFS not null terminated\n");
		wordfree(&we);
	}

	r = wordexp("a b", &we, 0);
	if (!r)
		r = wordexp("c d", &we, WRDE_APPEND);
	if (r)
		t_error("wordexp(WRDE_APPEND) returned %s\n", retstr(r));
	else {
		if (we.we_wordc != 4 || strcmp(join(&we), "a|b|c|d|"))
			t_error("WRDE_APPEND gave [%s] (%zu)\n", join(&we), we.we_wordc);
		wordfree(&we);
	}

	we.we_offs = 1;
	r = wordexp("a", &we, WRDE_DOOFFS);
	if (!r)
		r = wordexp("b", &we, WRDE_DOOFFS | WRDE_APPEND);
	if (r)
		t_error("wordexp(WRDE_DOOFFS|WRDE_APPEND) returned %s\n", retstr(r));
	else {
		if (we.we_wordc != 2 || we.we_wordv[0] ||
		    strcmp(we.we_wordv[1], "a") || strcmp(we.we_wordv[2], "b"))
			t_error("WRDE_DOOFFS|WRDE_APPEND layout wrong (%zu)\n", we.we_wordc);
		wordfree(&we);
	}

	r = wordexp("a b", &we, 0);
	if (!r)
		r = wordexp("c d e", &we, WRDE_REUSE);
	if (r)
		t_error("wordexp(WRDE_REUSE) returned %s\n", retstr(r));
	else {
		if (we.we_wordc != 3 || strcmp(join(&we), "c|d|e|"))
			t_error("WRDE_REUSE gave [%s] (%zu)\n", join(&we), we.we_wordc);
		wordfree(&we);
	}

	char dir[] = "/tmp/wordexpXXXXXX";
	CHK_FATAL(mkdtemp(dir) == dir);
	CHK_FATAL(chdir(dir) == 0);

	const char *files[] = {
		"aa",
		"aa*",
		"ab",
		"b b",
		"~",
		0};
	for (i = 0; files[i]; i++) {
		FILE *f;
		CHK((f = fopen(files[i], "w")) != 0);
		if (f) fclose(f);
	}

	glob_ok("a*", "aa|aa*|ab|");
	glob_ok("a?", "aa|ab|");
	glob_ok("*b", "ab|b b|");
	glob_ok("?b", "ab|");
	glob_ok("./?b", "./ab|");
	glob_ok("a[ab]", "aa|ab|");
	glob_ok("a[!a]", "ab|");
	glob_ok("[a]a", "aa|");
	glob_ok("a[b-z]", "ab|");
	glob_ok("aa", "aa|");
	glob_ok("zz*", "zz*|");
	glob_ok("?", "~|");
	glob_ok("./~*", "./~|");
	glob_ok("b\\ b*", "b b|");
	glob_ok("a* a?", "aa|aa*|ab|aa|ab|");

	for (i = 0; files[i]; i++)
		CHK(unlink(files[i]) == 0);
	CHK(rmdir(dir) == 0);

	return t_status;
}
