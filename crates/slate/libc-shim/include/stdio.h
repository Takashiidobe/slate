#ifndef _SLATE_STDIO_H
#define _SLATE_STDIO_H

#define __need_size_t
#define __need_NULL
#include <stddef.h>
#undef __need_size_t
#undef __need_NULL

typedef struct FILE FILE;

#define EOF (-1)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int printf(const char *restrict format, ...);
int fprintf(FILE *restrict stream, const char *restrict format, ...);
int sprintf(char *restrict s, const char *restrict format, ...);
int snprintf(char *restrict s, size_t n, const char *restrict format, ...);

int scanf(const char *restrict format, ...);
int fscanf(FILE *restrict stream, const char *restrict format, ...);
int sscanf(const char *restrict s, const char *restrict format, ...);

FILE *fopen(const char *restrict pathname, const char *restrict mode);
FILE *freopen(const char *restrict pathname, const char *restrict mode,
              FILE *restrict stream);
int fclose(FILE *stream);
int fflush(FILE *stream);

size_t fread(void *restrict ptr, size_t size, size_t nmemb,
             FILE *restrict stream);
size_t fwrite(const void *restrict ptr, size_t size, size_t nmemb,
              FILE *restrict stream);

int fgetc(FILE *stream);
char *fgets(char *restrict s, int n, FILE *restrict stream);
int fputc(int c, FILE *stream);
int fputs(const char *restrict s, FILE *restrict stream);
int ungetc(int c, FILE *stream);
int getchar(void);
int putchar(int c);
int puts(const char *s);

int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
void rewind(FILE *stream);

void clearerr(FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);
void perror(const char *s);

int remove(const char *pathname);
int rename(const char *oldpath, const char *newpath);
FILE *tmpfile(void);

#endif
