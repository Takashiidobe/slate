#ifndef _SLATE_MCHECK_H
#define _SLATE_MCHECK_H

#include <features.h>

enum mcheck_status {
  MCHECK_DISABLED = -1,
  MCHECK_OK,
  MCHECK_HEAD,
  MCHECK_TAIL,
  MCHECK_FREE,
};

int  mcheck(void (*abortfn)(enum mcheck_status));
int  mcheck_pedantic(void (*abortfn)(enum mcheck_status));
void mcheck_check_all(void);

enum mcheck_status mprobe(void *ptr);

#endif
