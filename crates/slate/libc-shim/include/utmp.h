#ifndef _SLATE_UTMP_H
#define _SLATE_UTMP_H

#if defined(__SLATE_LIBC_GLIBC)
#include <stdint.h>
#include <sys/types.h>
#include <sys/time.h>
#else
#include <utmpx.h>
#endif

#define ACCOUNTING  9
#define UT_NAMESIZE 32
#define UT_HOSTSIZE 256
#if defined(__SLATE_LIBC_GLIBC)
#define ut_name ut_user
#define ut_time ut_tv.tv_sec
#define ut_xtime ut_tv.tv_sec
#define ut_addr ut_addr_v6[0]
#define EMPTY 0
#define RUN_LVL 1
#define BOOT_TIME 2
#define NEW_TIME 3
#define OLD_TIME 4
#define INIT_PROCESS 5
#define LOGIN_PROCESS 6
#define USER_PROCESS 7
#define DEAD_PROCESS 8
#define UT_UNKNOWN EMPTY
#endif
#define UT_LINESIZE 32

#if defined(__SLATE_LIBC_GLIBC)
struct lastlog {
#if defined(__SLATE_ARCH_AARCH64)
  time_t ll_time;
#else
  uint32_t ll_time;
#endif
  char   ll_line[UT_LINESIZE];
  char   ll_host[UT_HOSTSIZE];
};

struct exit_status {
  short e_termination;
  short e_exit;
};

struct utmp {
  short ut_type;
  pid_t ut_pid;
  char ut_line[UT_LINESIZE];
  char ut_id[4];
  char ut_user[UT_NAMESIZE];
  char ut_host[UT_HOSTSIZE];
  struct exit_status ut_exit;
  int32_t ut_session;
  struct {
    uint32_t tv_sec;
    int32_t tv_usec;
  } ut_tv;
  int32_t ut_addr_v6[4];
  char __glibc_reserved[20];
};
#else
struct lastlog {
  time_t ll_time;
  char   ll_line[UT_LINESIZE];
  char   ll_host[UT_HOSTSIZE];
};
#endif

#if !defined(__SLATE_LIBC_GLIBC)
#define ut_time ut_tv.tv_sec
#define ut_name ut_user
#define ut_addr ut_addr_v6[0]
#define utmp utmpx
#define e_exit __e_exit
#define e_termination __e_termination
#endif

void         endutent(void);
struct utmp *getutent(void);
struct utmp *getutid(const struct utmp *);
struct utmp *getutline(const struct utmp *);
struct utmp *pututline(const struct utmp *);
void         setutent(void);

void updwtmp(const char *, const struct utmp *);
int  utmpname(const char *);

int login_tty(int);

#define _PATH_UTMP "/dev/null/utmp"
#define _PATH_WTMP "/dev/null/wtmp"

#define UTMP_FILE     _PATH_UTMP
#define WTMP_FILE     _PATH_WTMP
#define UTMP_FILENAME _PATH_UTMP
#define WTMP_FILENAME _PATH_WTMP

#endif
