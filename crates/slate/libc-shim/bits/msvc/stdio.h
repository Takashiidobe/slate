#ifndef _SLATE_BITS_MSVC_STDIO_H
#define _SLATE_BITS_MSVC_STDIO_H

#define BUFSIZ       512
#define _NSTREAM_    512
#define _NFILE       _NSTREAM_
#define _IOB_ENTRIES 3

#define EOF (-1)

#define _IOFBF 0x0000
#define _IOLBF 0x0040
#define _IONBF 0x0004

#define L_tmpnam 260

#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0

#define FILENAME_MAX 260
#define FOPEN_MAX    20
#define _SYS_OPEN    20
#define TMP_MAX      2147483647

typedef long long fpos_t;

FILE *__acrt_iob_func(unsigned int);

#define stdin  (__acrt_iob_func(0))
#define stdout (__acrt_iob_func(1))
#define stderr (__acrt_iob_func(2))

errno_t _get_stream_buffer_pointers(FILE *, char ***, char ***, int **);

void  clearerr(FILE *);
int   fclose(FILE *);
int   _fcloseall(void);
FILE *_fdopen(int, const char *);
int   feof(FILE *);
int   ferror(FILE *);
int   fflush(FILE *);
int   fgetc(FILE *);
int   _fgetchar(void);
int   fgetpos(FILE *, fpos_t *);
char *fgets(char *, int, FILE *);
int   _fileno(FILE *);
int   _flushall(void);
FILE *fopen(const char *, const char *);
int   fputc(int, FILE *);
int   _fputchar(int);
int   fputs(const char *, FILE *);
size_t fread(void *, size_t, size_t, FILE *);
FILE  *freopen(const char *, const char *, FILE *);
FILE  *_fsopen(const char *, const char *, int);
int    fsetpos(FILE *, const fpos_t *);
int    fseek(FILE *, long, int);
int    _fseeki64(FILE *, long long, int);
long   ftell(FILE *);
long long _ftelli64(FILE *);
size_t fwrite(const void *, size_t, size_t, FILE *);
int    getc(FILE *);
int    getchar(void);
int    _getmaxstdio(void);
int    _getw(FILE *);
void   perror(const char *);
int    _pclose(FILE *);
FILE  *_popen(const char *, const char *);
int    putc(int, FILE *);
int    putchar(int);
int    puts(const char *);
int    _putw(int, FILE *);
int    remove(const char *);
int    rename(const char *, const char *);
int    _unlink(const char *);
void   rewind(FILE *);
int    _rmtmp(void);
void   setbuf(FILE *, char *);
int    _setmaxstdio(int);
int    setvbuf(FILE *, char *, int, size_t);
char  *_tempnam(const char *, const char *);
FILE  *tmpfile(void);
char  *tmpnam(char *);
int    ungetc(int, FILE *);

void   _lock_file(FILE *);
void   _unlock_file(FILE *);
int    _fclose_nolock(FILE *);
int    _fflush_nolock(FILE *);
int    _fgetc_nolock(FILE *);
int    _fputc_nolock(int, FILE *);
size_t _fread_nolock(void *, size_t, size_t, FILE *);
int    _fseek_nolock(FILE *, long, int);
int    _fseeki64_nolock(FILE *, long long, int);
long   _ftell_nolock(FILE *);
long long _ftelli64_nolock(FILE *);
size_t _fwrite_nolock(const void *, size_t, size_t, FILE *);
int    _getc_nolock(FILE *);
int    _putc_nolock(int, FILE *);
int    _ungetc_nolock(int, FILE *);

#define _getchar_nolock()    _getc_nolock(stdin)
#define _putchar_nolock(ch)  _putc_nolock((ch), stdout)

int *__p__commode(void);
#define _commode (*__p__commode())

int printf(const char *, ...);
int fprintf(FILE *, const char *, ...);
int sprintf(char *, const char *, ...);
int snprintf(char *, size_t, const char *, ...);

int vprintf(const char *, va_list);
int vfprintf(FILE *, const char *, va_list);
int vsprintf(char *, const char *, va_list);
int vsnprintf(char *, size_t, const char *, va_list);

int scanf(const char *, ...);
int fscanf(FILE *, const char *, ...);
int sscanf(const char *, const char *, ...);
int vscanf(const char *, va_list);
int vfscanf(FILE *, const char *, va_list);
int vsscanf(const char *, const char *, va_list);

int _set_printf_count_output(int);
int _get_printf_count_output(void);
int _vfprintf_p(FILE *, const char *, va_list);
int _vprintf_p(const char *, va_list);
int _fprintf_p(FILE *, const char *, ...);
int _printf_p(const char *, ...);
int _vscprintf(const char *, va_list);
int _vscprintf_p(const char *, va_list);
int _scprintf(const char *, ...);
int _scprintf_p(const char *, ...);
int _vsnprintf(char *, size_t, const char *, va_list);
int _vsnprintf_c(char *, size_t, const char *, va_list);
int _snprintf(char *, size_t, const char *, ...);
int _snprintf_c(char *, size_t, const char *, ...);
int _vsprintf_p(char *, size_t, const char *, va_list);
int _sprintf_p(char *, size_t, const char *, ...);
int _snscanf(const char *, size_t, const char *, ...);

#if !__STDC__
#define SYS_OPEN _SYS_OPEN

char *tempnam(const char *, const char *);
int   fcloseall(void);
FILE *fdopen(int, const char *);
int   fgetchar(void);
int   fileno(FILE *);
int   flushall(void);
int   fputchar(int);
int   getw(FILE *);
int   putw(int, FILE *);
int   rmtmp(void);
int   unlink(const char *);
#endif

#define _CRT_INTERNAL_PRINTF_LEGACY_VSPRINTF_NULL_TERMINATION (1ULL << 0)
#define _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR       (1ULL << 1)
#define _CRT_INTERNAL_PRINTF_LEGACY_WIDE_SPECIFIERS           (1ULL << 2)
#define _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY      (1ULL << 3)
#define _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS     (1ULL << 4)

#define _CRT_INTERNAL_SCANF_SECURECRT                   (1ULL << 0)
#define _CRT_INTERNAL_SCANF_LEGACY_WIDE_SPECIFIERS      (1ULL << 1)
#define _CRT_INTERNAL_SCANF_LEGACY_MSVCRT_COMPATIBILITY (1ULL << 2)

#include <bits/msvc/locale/stdio.h>
#include <bits/msvc/wchar/stdio.h>

#endif
