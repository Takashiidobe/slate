#ifndef _SLATE_STDIO_H
#define _SLATE_STDIO_H

#define __need_size_t
#define __need_NULL
#include <stddef.h>
#undef __need_size_t
#undef __need_NULL

typedef struct {
  int __count;
  union {
    int __wch;
    char __wchb[4];
  } __value;
} __mbstate_t;

typedef long __off_t;

typedef struct __fpos_t {
  __off_t __pos;
  __mbstate_t __state;
} __fpos_t;

typedef struct FILE FILE;
typedef __fpos_t fpos_t;

typedef long off_t;

#define EOF (-1)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

void clearerr(FILE *);
char *ctermid(char *);
char *cuserid(char *);
int fclose(FILE *);
FILE *fdopen(int, const char *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int fgetpos(FILE *, fpos_t *);
char *fgets(char *, int, FILE *);
int fileno(FILE *);
void flockfile(FILE *);
FILE *fopen(const char *, const char *);
int fprintf(FILE *, const char *, ...);
int fputc(int, FILE *);
int fputs(const char *, FILE *);
size_t fread(void *, size_t, size_t, FILE *);
FILE *freopen(const char *, const char *, FILE *);
int fscanf(FILE *, const char *, ...);
int fseek(FILE *, long int, int);
int fseeko(FILE *, off_t, int);
int fsetpos(FILE *, const fpos_t *);
long ftell(FILE *);
off_t ftello(FILE *);
int ftrylockfile(FILE *);
void funlockfile(FILE *);
size_t fwrite(const void *, size_t, size_t, FILE *);
int getc(FILE *);
int getchar(void);
int getc_unlocked(FILE *);
int getchar_unlocked(void);
int getopt(int, char *const[], const char);
char *gets(char *);
int getw(FILE *);
int pclose(FILE *);
void perror(const char *);
FILE *popen(const char *, const char *);
int printf(const char *, ...);
int putc(int, FILE *);
int putchar(int);
int putc_unlocked(int, FILE *);
int putchar_unlocked(int);
int puts(const char *);
int putw(int, FILE *);
int remove(const char *);
int rename(const char *, const char *);
void rewind(FILE *);
int scanf(const char *, ...);
void setbuf(FILE *, char *);
int setvbuf(FILE *, char *, int, size_t);
int snprintf(char *, size_t, const char *, ...);
int sprintf(char *, const char *, ...);
int sscanf(const char *, const char *, int, ...);
char *tempnam(const char *, const char *);
FILE *tmpfile(void);
char *tmpnam(char *);
int ungetc(int, FILE *);
int vfprintf(FILE *, const char *, ...);
int vprintf(const char *, ...);
int vsnprintf(char *, size_t, const char *, ...);
int vsprintf(char *, const char *, ...);

#endif
