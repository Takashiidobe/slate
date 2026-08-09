#include <stdio.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                             \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

_Static_assert(BUFSIZ == 512, "BUFSIZ");
_Static_assert(_NFILE == 512, "_NFILE");
_Static_assert(_NSTREAM_ == 512, "_NSTREAM_");
_Static_assert(_IOB_ENTRIES == 3, "_IOB_ENTRIES");
_Static_assert(EOF == -1, "EOF");
_Static_assert(_IOFBF == 0x0000, "_IOFBF");
_Static_assert(_IOLBF == 0x0040, "_IOLBF");
_Static_assert(_IONBF == 0x0004, "_IONBF");
_Static_assert(L_tmpnam == 260, "L_tmpnam");
_Static_assert(SEEK_CUR == 1, "SEEK_CUR");
_Static_assert(SEEK_END == 2, "SEEK_END");
_Static_assert(SEEK_SET == 0, "SEEK_SET");
_Static_assert(FILENAME_MAX == 260, "FILENAME_MAX");
_Static_assert(FOPEN_MAX == 20, "FOPEN_MAX");
_Static_assert(_SYS_OPEN == 20, "_SYS_OPEN");
_Static_assert(TMP_MAX == 2147483647, "TMP_MAX");
_Static_assert(sizeof(fpos_t) == 8, "fpos_t size");
_Static_assert(_Alignof(fpos_t) == 8, "fpos_t alignment");
_Static_assert((fpos_t)-1 < 0, "fpos_t signedness");
_Static_assert(__builtin_types_compatible_p(FILE, struct _iobuf), "FILE tag");

TYPE_IS(stdin, FILE *);
TYPE_IS(stdout, FILE *);
TYPE_IS(stderr, FILE *);
TYPE_IS(&__acrt_iob_func, FILE *(*)(unsigned int));
TYPE_IS(&_get_stream_buffer_pointers,
        errno_t (*)(FILE *, char ***, char ***, int **));

TYPE_IS(&clearerr, void (*)(FILE *));
TYPE_IS(&fclose, int (*)(FILE *));
TYPE_IS(&_fcloseall, int (*)(void));
TYPE_IS(&_fdopen, FILE *(*)(int, const char *));
TYPE_IS(&feof, int (*)(FILE *));
TYPE_IS(&ferror, int (*)(FILE *));
TYPE_IS(&fflush, int (*)(FILE *));
TYPE_IS(&fgetc, int (*)(FILE *));
TYPE_IS(&_fgetchar, int (*)(void));
TYPE_IS(&fgetpos, int (*)(FILE *, fpos_t *));
TYPE_IS(&fgets, char *(*)(char *, int, FILE *));
TYPE_IS(&_fileno, int (*)(FILE *));
TYPE_IS(&_flushall, int (*)(void));
TYPE_IS(&fopen, FILE *(*)(const char *, const char *));
TYPE_IS(&fputc, int (*)(int, FILE *));
TYPE_IS(&_fputchar, int (*)(int));
TYPE_IS(&fputs, int (*)(const char *, FILE *));
TYPE_IS(&fread, size_t (*)(void *, size_t, size_t, FILE *));
TYPE_IS(&freopen, FILE *(*)(const char *, const char *, FILE *));
TYPE_IS(&_fsopen, FILE *(*)(const char *, const char *, int));
TYPE_IS(&fsetpos, int (*)(FILE *, const fpos_t *));
TYPE_IS(&fseek, int (*)(FILE *, long, int));
TYPE_IS(&_fseeki64, int (*)(FILE *, long long, int));
TYPE_IS(&ftell, long (*)(FILE *));
TYPE_IS(&_ftelli64, long long (*)(FILE *));
TYPE_IS(&fwrite, size_t (*)(const void *, size_t, size_t, FILE *));
TYPE_IS(&getc, int (*)(FILE *));
TYPE_IS(&getchar, int (*)(void));
TYPE_IS(&_getmaxstdio, int (*)(void));
TYPE_IS(&_getw, int (*)(FILE *));
TYPE_IS(&perror, void (*)(const char *));
TYPE_IS(&_pclose, int (*)(FILE *));
TYPE_IS(&_popen, FILE *(*)(const char *, const char *));
TYPE_IS(&putc, int (*)(int, FILE *));
TYPE_IS(&putchar, int (*)(int));
TYPE_IS(&puts, int (*)(const char *));
TYPE_IS(&_putw, int (*)(int, FILE *));
TYPE_IS(&remove, int (*)(const char *));
TYPE_IS(&rename, int (*)(const char *, const char *));
TYPE_IS(&_unlink, int (*)(const char *));
TYPE_IS(&rewind, void (*)(FILE *));
TYPE_IS(&_rmtmp, int (*)(void));
TYPE_IS(&setbuf, void (*)(FILE *, char *));
TYPE_IS(&_setmaxstdio, int (*)(int));
TYPE_IS(&setvbuf, int (*)(FILE *, char *, int, size_t));
TYPE_IS(&_tempnam, char *(*)(const char *, const char *));
TYPE_IS(&tmpfile, FILE *(*)(void));
TYPE_IS(&tmpnam, char *(*)(char *));
TYPE_IS(&ungetc, int (*)(int, FILE *));

TYPE_IS(&_lock_file, void (*)(FILE *));
TYPE_IS(&_unlock_file, void (*)(FILE *));
TYPE_IS(&_fclose_nolock, int (*)(FILE *));
TYPE_IS(&_fflush_nolock, int (*)(FILE *));
TYPE_IS(&_fgetc_nolock, int (*)(FILE *));
TYPE_IS(&_fputc_nolock, int (*)(int, FILE *));
TYPE_IS(&_fread_nolock, size_t (*)(void *, size_t, size_t, FILE *));
TYPE_IS(&_fseek_nolock, int (*)(FILE *, long, int));
TYPE_IS(&_fseeki64_nolock, int (*)(FILE *, long long, int));
TYPE_IS(&_ftell_nolock, long (*)(FILE *));
TYPE_IS(&_ftelli64_nolock, long long (*)(FILE *));
TYPE_IS(&_fwrite_nolock, size_t (*)(const void *, size_t, size_t, FILE *));
TYPE_IS(&_getc_nolock, int (*)(FILE *));
TYPE_IS(&_putc_nolock, int (*)(int, FILE *));
TYPE_IS(&_ungetc_nolock, int (*)(int, FILE *));
TYPE_IS(&__p__commode, int *(*)(void));
TYPE_IS(_commode, int);

TYPE_IS(&printf, int (*)(const char *, ...));
TYPE_IS(&fprintf, int (*)(FILE *, const char *, ...));
TYPE_IS(&sprintf, int (*)(char *, const char *, ...));
TYPE_IS(&snprintf, int (*)(char *, size_t, const char *, ...));
TYPE_IS(&vprintf, int (*)(const char *, va_list));
TYPE_IS(&vfprintf, int (*)(FILE *, const char *, va_list));
TYPE_IS(&vsprintf, int (*)(char *, const char *, va_list));
TYPE_IS(&vsnprintf, int (*)(char *, size_t, const char *, va_list));
TYPE_IS(&scanf, int (*)(const char *, ...));
TYPE_IS(&fscanf, int (*)(FILE *, const char *, ...));
TYPE_IS(&sscanf, int (*)(const char *, const char *, ...));
TYPE_IS(&vscanf, int (*)(const char *, va_list));
TYPE_IS(&vfscanf, int (*)(FILE *, const char *, va_list));
TYPE_IS(&vsscanf, int (*)(const char *, const char *, va_list));
TYPE_IS(&_set_printf_count_output, int (*)(int));
TYPE_IS(&_get_printf_count_output, int (*)(void));
TYPE_IS(&_vfprintf_p, int (*)(FILE *, const char *, va_list));
TYPE_IS(&_vprintf_p, int (*)(const char *, va_list));
TYPE_IS(&_fprintf_p, int (*)(FILE *, const char *, ...));
TYPE_IS(&_printf_p, int (*)(const char *, ...));
TYPE_IS(&_vscprintf, int (*)(const char *, va_list));
TYPE_IS(&_vscprintf_p, int (*)(const char *, va_list));
TYPE_IS(&_scprintf, int (*)(const char *, ...));
TYPE_IS(&_scprintf_p, int (*)(const char *, ...));
TYPE_IS(&_vsnprintf, int (*)(char *, size_t, const char *, va_list));
TYPE_IS(&_vsnprintf_c, int (*)(char *, size_t, const char *, va_list));
TYPE_IS(&_snprintf, int (*)(char *, size_t, const char *, ...));
TYPE_IS(&_snprintf_c, int (*)(char *, size_t, const char *, ...));
TYPE_IS(&_vsprintf_p,
        int (*)(char *, size_t, const char *, va_list));
TYPE_IS(&_sprintf_p, int (*)(char *, size_t, const char *, ...));
TYPE_IS(&_snscanf, int (*)(const char *, size_t, const char *, ...));
TYPE_IS(&tempnam, char *(*)(const char *, const char *));
TYPE_IS(&fcloseall, int (*)(void));
TYPE_IS(&fdopen, FILE *(*)(int, const char *));
TYPE_IS(&fgetchar, int (*)(void));
TYPE_IS(&fileno, int (*)(FILE *));
TYPE_IS(&flushall, int (*)(void));
TYPE_IS(&fputchar, int (*)(int));
TYPE_IS(&getw, int (*)(FILE *));
TYPE_IS(&putw, int (*)(int, FILE *));
TYPE_IS(&rmtmp, int (*)(void));
TYPE_IS(&unlink, int (*)(const char *));
_Static_assert(SYS_OPEN == 20, "SYS_OPEN");

_Static_assert(_CRT_INTERNAL_PRINTF_LEGACY_VSPRINTF_NULL_TERMINATION == 1ULL,
               "legacy vsprintf option");
_Static_assert(_CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR == 2ULL,
               "standard snprintf option");
_Static_assert(_CRT_INTERNAL_PRINTF_LEGACY_WIDE_SPECIFIERS == 4ULL,
               "legacy wide option");
_Static_assert(_CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY == 8ULL,
               "legacy msvcrt option");
_Static_assert(_CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS == 16ULL,
               "legacy exponent option");
_Static_assert(_CRT_INTERNAL_SCANF_SECURECRT == 1ULL, "secure scanf option");
_Static_assert(_CRT_INTERNAL_SCANF_LEGACY_WIDE_SPECIFIERS == 2ULL,
               "legacy scanf wide option");
_Static_assert(_CRT_INTERNAL_SCANF_LEGACY_MSVCRT_COMPATIBILITY == 4ULL,
               "legacy scanf msvcrt option");

extern int popen;
extern int pclose;
extern int fseeko;
extern int ftello;
extern int flockfile;
extern int getline;
extern int getdelim;

int main(void) { return 0; }
