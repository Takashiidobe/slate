#ifndef _SLATE_STDIO_H
#define _SLATE_STDIO_H

#include <features.h>

#define __NEED_FILE
#define __NEED_size_t
#if defined(__SLATE_LIBC_MSVC)
#define __NEED_wchar_t
#define __NEED_wint_t
#endif

#if __STDC_VERSION__ < 201112L
#define __NEED_struct__IO_FILE
#endif

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE) ||  \
    defined(__SLATE_LIBC_DARWIN)
#define __NEED_ssize_t
#define __NEED_off_t
#define __NEED_wchar_t
#endif

#if defined(__SLATE_LIBC_BIONIC)
#define __NEED_off_t
#endif

#define __NEED_NULL
#define __NEED_va_list
#include <bits/types.h>

#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/stdio.h>
#else

#undef EOF
#define EOF (-1)

#undef SEEK_SET
#undef SEEK_CUR
#undef SEEK_END
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#define BUFSIZ       1024
#define FILENAME_MAX 4096
#define FOPEN_MAX    1000
#define TMP_MAX      10000
#define L_tmpnam     20

#if defined(__SLATE_LIBC_DARWIN)
typedef __fpos_t fpos_t;

#include <bits/darwin/stdio.h>

extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;

#define stdin  __stdinp
#define stdout __stdoutp
#define stderr __stderrp

#undef FOPEN_MAX
#undef FILENAME_MAX
#undef TMP_MAX
#undef L_tmpnam
#define FOPEN_MAX    20
#define FILENAME_MAX 1024
#define TMP_MAX      308915776
#define L_tmpnam     1024
#elif defined(__SLATE_LIBC_FREEBSD)
typedef __off_t fpos_t;

#include <bits/freebsd/stdio.h>

extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;

#define stdin  __stdinp
#define stdout __stdoutp
#define stderr __stderrp
#elif defined(__SLATE_LIBC_BIONIC)
typedef off_t fpos_t;

extern FILE *const stdin;
extern FILE *const stdout;
extern FILE *const stderr;

#define stdin  (stdin)
#define stdout (stdout)
#define stderr (stderr)
#else
typedef union _G_fpos64_t {
  char      __opaque[16];
  long long __lldata;
  double    __align;
} fpos_t;

extern FILE *const stdin;
extern FILE *const stdout;
extern FILE *const stderr;

#define stdin  (stdin)
#define stdout (stdout)
#define stderr (stderr)
#endif

#if defined(__SLATE_LIBC_DARWIN) && defined(_DARWIN_C_SOURCE)
FILE *fopen(const char *__restrict, const char *__restrict) __DARWIN_EXTSN(fopen);
#elif defined(__SLATE_LIBC_DARWIN)
FILE *fopen(const char *__restrict, const char *__restrict) __DARWIN_ALIAS(fopen);
#else
FILE *fopen(const char *__restrict, const char *__restrict);
#endif
FILE *freopen(const char *__restrict, const char *__restrict, FILE *__restrict);
int   fclose(FILE *);

int remove(const char *);
int rename(const char *, const char *);

int  feof(FILE *);
int  ferror(FILE *);
int  fflush(FILE *);
void clearerr(FILE *);

int  fseek(FILE *, long, int);
long ftell(FILE *);
void rewind(FILE *);

int fgetpos(FILE *__restrict, fpos_t *__restrict);
int fsetpos(FILE *, const fpos_t *);

size_t fread(void *__restrict, size_t, size_t, FILE *__restrict);
size_t fwrite(const void *__restrict, size_t, size_t, FILE *__restrict);

int fgetc(FILE *);
int getc(FILE *);
int getchar(void);
int ungetc(int, FILE *);

int fputc(int, FILE *);
int putc(int, FILE *);
int putchar(int);

char *fgets(char *__restrict, int, FILE *__restrict);
#if __STDC_VERSION__ < 201112L
char *gets(char *);
#endif

int fputs(const char *__restrict, FILE *__restrict);
int puts(const char *);

int printf(const char *__restrict, ...);
int fprintf(FILE *__restrict, const char *__restrict, ...);
int sprintf(char *__restrict, const char *__restrict, ...);
int snprintf(char *__restrict, size_t, const char *__restrict, ...);

int vprintf(const char *__restrict, va_list);
int vfprintf(FILE *__restrict, const char *__restrict, va_list);
int vsprintf(char *__restrict, const char *__restrict, va_list);
int vsnprintf(char *__restrict, size_t, const char *__restrict, va_list);

int scanf(const char *__restrict, ...);
int fscanf(FILE *__restrict, const char *__restrict, ...);
int sscanf(const char *__restrict, const char *__restrict, ...);
int vscanf(const char *__restrict, va_list);
int vfscanf(FILE *__restrict, const char *__restrict, va_list);
int vsscanf(const char *__restrict, const char *__restrict, va_list);

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE) ||  \
    defined(__SLATE_LIBC_DARWIN)
int vfwprintf(FILE *__restrict, const wchar_t *__restrict, va_list);
int vswprintf(wchar_t *__restrict, size_t, const wchar_t *__restrict, va_list);
int vwprintf(const wchar_t *__restrict, va_list);
int vfwscanf(FILE *__restrict, const wchar_t *__restrict, va_list);
int vswscanf(const wchar_t *__restrict, const wchar_t *__restrict, va_list);
int vwscanf(const wchar_t *__restrict, va_list);
#endif

void perror(const char *);

int  setvbuf(FILE *__restrict, char *__restrict, int, size_t);
void setbuf(FILE *__restrict, char *__restrict);

char *tmpnam(char *);
FILE *tmpfile(void);

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE) ||  \
    defined(__SLATE_LIBC_DARWIN)
FILE   *fmemopen(void *__restrict, size_t, const char *__restrict);
FILE   *open_memstream(char **, size_t *);
FILE   *fdopen(int, const char *);
FILE   *popen(const char *, const char *);
int     pclose(FILE *);
int     fileno(FILE *);
int     fseeko(FILE *, off_t, int);
off_t   ftello(FILE *);
int     dprintf(int, const char *__restrict, ...);
int     vdprintf(int, const char *__restrict, va_list);
void    flockfile(FILE *);
int     ftrylockfile(FILE *);
void    funlockfile(FILE *);
int     getc_unlocked(FILE *);
int     getchar_unlocked(void);
int     putc_unlocked(int, FILE *);
int     putchar_unlocked(int);
ssize_t getdelim(char **__restrict, size_t *__restrict, int, FILE *__restrict);
ssize_t getline(char **__restrict, size_t *__restrict, FILE *__restrict);
int     renameat(int, const char *, int, const char *);
char   *ctermid(char *);
#if defined(__SLATE_LIBC_DARWIN)
#define L_ctermid 1024
#else
#define L_ctermid 20
#endif
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE) ||  \
    defined(__SLATE_LIBC_DARWIN)
#if defined(__SLATE_LIBC_DARWIN)
#define P_tmpdir "/var/tmp/"
#else
#define P_tmpdir "/tmp"
#endif
char *tempnam(const char *, const char *);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define L_cuserid 20
char  *cuserid(char *);
void   setlinebuf(FILE *);
void   setbuffer(FILE *, char *, size_t);
int    fgetc_unlocked(FILE *);
int    fputc_unlocked(int, FILE *);
int    fflush_unlocked(FILE *);
size_t fread_unlocked(void *, size_t, size_t, FILE *);
size_t fwrite_unlocked(const void *, size_t, size_t, FILE *);
void   clearerr_unlocked(FILE *);
int    feof_unlocked(FILE *);
int    ferror_unlocked(FILE *);
int    fileno_unlocked(FILE *);
int    getw(FILE *);
int    putw(int, FILE *);
char  *fgetln(FILE *, size_t *);
int    asprintf(char **, const char *, ...);
int    vasprintf(char **, const char *, va_list);
#endif

#if defined(__SLATE_LIBC_DARWIN)
int         getw(FILE *);
int         putw(int, FILE *);
char       *fgetln(FILE *, size_t *);
int         asprintf(char **__restrict, const char *__restrict, ...);
int         vasprintf(char **__restrict, const char *__restrict, va_list);
char       *ctermid_r(char *);
const char *fmtcheck(const char *, const char *);
int         fpurge(FILE *);
void        setbuffer(FILE *, char *, int);
int         setlinebuf(FILE *);
FILE       *funopen(const void *, int (*)(void *, char *, int),
                    int (*)(void *, const char *, int),
                    fpos_t (*)(void *, fpos_t, int), int (*)(void *));
#endif

#ifdef _GNU_SOURCE
char *fgets_unlocked(char *, int, FILE *);
int   fputs_unlocked(const char *, FILE *);

typedef ssize_t(cookie_read_function_t)(void *, char *, size_t);
typedef ssize_t(cookie_write_function_t)(void *, const char *, size_t);
typedef int(cookie_seek_function_t)(void *, off_t *, int);
typedef int(cookie_close_function_t)(void *);

typedef struct _IO_cookie_io_functions_t {
  cookie_read_function_t  *read;
  cookie_write_function_t *write;
  cookie_seek_function_t  *seek;
  cookie_close_function_t *close;
} cookie_io_functions_t;

FILE *fopencookie(void *, const char *, cookie_io_functions_t);
#endif

#if defined(_LARGEFILE64_SOURCE)
#define tmpfile64 tmpfile
#define fopen64   fopen
#define freopen64 freopen
#define fseeko64  fseeko
#define ftello64  ftello
#define fgetpos64 fgetpos
#define fsetpos64 fsetpos
#define fpos64_t  fpos_t
#define off64_t   off_t
#endif

#endif

#endif
