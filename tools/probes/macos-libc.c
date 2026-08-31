#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <langinfo.h>
#include <locale.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <sys/ucontext.h>
#include <time.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>
#include <xlocale.h>

struct slate_macos_stdio_locale_layouts {
  FILE      stream;
  fpos_t    position;
  mbstate_t conversion;
  struct lconv numeric;
  locale_t  locale;
  wint_t    wide;
  wctype_t  classification;
  wctrans_t transformation;
};

struct slate_macos_filesystem_layouts {
  struct stat    file_status;
  struct dirent  directory_entry;
  struct statvfs filesystem_status;
};

struct slate_macos_time_signal_layouts {
  struct timespec  precise_time;
  struct timeval   wall_time;
  struct timezone  zone;
  struct tm        calendar;
  struct itimerval interval_timer;
  struct sigevent  event;
  siginfo_t        signal_info;
  struct sigaction action;
  stack_t          alternate_stack;
  ucontext_t       context;
  jmp_buf          jump;
  sigjmp_buf       signal_jump;
};

FILE *slate_macos_open(const char *path) { return fopen(path, "r"); }

int slate_macos_errno(void) { return errno; }

FILE *slate_macos_stdin(void) { return stdin; }

int slate_macos_classify(wint_t value, locale_t locale) {
  return iswctype_l(value, wctype("alpha"), locale);
}

int slate_macos_export(void) { return LC_ALL + CODESET; }

int slate_macos_stat(const char *path, struct stat *out) {
  return stat(path, out);
}

struct dirent *slate_macos_readdir(DIR *dir) { return readdir(dir); }

int slate_macos_open_read_only(const char *path) {
  return open(path, O_RDONLY);
}

int slate_macos_clock(struct timespec *value) {
  return clock_gettime(CLOCK_MONOTONIC, value);
}

int slate_macos_set_timer(const struct itimerval *value) {
  return setitimer(ITIMER_REAL, value, 0);
}

int slate_macos_sigmask(sigset_t *mask) {
  return sigprocmask(SIG_BLOCK, mask, 0);
}

int slate_macos_altstack(stack_t *stack) { return sigaltstack(stack, 0); }
