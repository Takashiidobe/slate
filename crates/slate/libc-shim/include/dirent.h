#ifndef _SLATE_DIRENT_H
#define _SLATE_DIRENT_H

typedef struct __dirstream DIR;

struct dirent {
  unsigned long d_ino;
  long d_off;
  unsigned short d_reclen;
  unsigned char d_type;
  char d_name[256];
};

#define d_fileno d_ino

#define DT_UNKNOWN 0
#define DT_FIFO 1
#define DT_CHR 2
#define DT_DIR 4
#define DT_BLK 6
#define DT_REG 8
#define DT_LNK 10
#define DT_SOCK 12
#define DT_WHT 14

#define IFTODT(mode) (((mode) & 0170000) >> 12)
#define DTTOIF(type) ((type) << 12)

DIR *opendir(const char *name);
DIR *fdopendir(int fd);
int closedir(DIR *dirp);

struct dirent *readdir(DIR *dirp);
void rewinddir(DIR *dirp);
void seekdir(DIR *dirp, long pos);
long telldir(DIR *dirp);
int dirfd(DIR *dirp);

int scandir(const char *dir, struct dirent ***namelist,
            int (*filter)(const struct dirent *),
            int (*compare)(const struct dirent **, const struct dirent **));

int alphasort(const struct dirent **a, const struct dirent **b);
int versionsort(const struct dirent **a, const struct dirent **b);

#endif
