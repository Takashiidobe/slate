#ifndef _SLATE_SYS_IOCTL_H
#define _SLATE_SYS_IOCTL_H

// glibc version
int ioctl(int fd, unsigned long op, ...);
// musl version
// int ioctl(int fd, int op, ...);

#endif
