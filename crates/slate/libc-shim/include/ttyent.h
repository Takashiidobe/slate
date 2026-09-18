#ifndef _SLATE_TTYENT_H
#define _SLATE_TTYENT_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)

#define _PATH_TTYS "/etc/ttys"
#define _TTYS_OFF "off"
#define _TTYS_ON "on"
#define _TTYS_SECURE "secure"
#define _TTYS_WINDOW "window"

struct ttyent {
  char *ty_name;
  char *ty_getty;
  char *ty_type;
  int ty_status;
  char *ty_window;
  char *ty_comment;
};

__BEGIN_DECLS
struct ttyent *getttyent(void) __THROW;
struct ttyent *getttynam(const char *__tty) __THROW;
int setttyent(void) __THROW;
int endttyent(void) __THROW;
__END_DECLS

#define TTY_ON 0x01
#define TTY_SECURE 0x02

#endif

#endif
