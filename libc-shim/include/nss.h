#ifndef _SLATE_NSS_H
#define _SLATE_NSS_H

#include <features.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

enum nss_status {
  NSS_STATUS_TRYAGAIN = -2,
  NSS_STATUS_UNAVAIL,
  NSS_STATUS_NOTFOUND,
  NSS_STATUS_SUCCESS,
  NSS_STATUS_RETURN
};

#define NSS_DECLARE_MODULE_FUNCTIONS(module) \
  extern int _nss_##module##_endaliasent(void)

#endif
