#ifndef _SLATE_CRYPT_H
#define _SLATE_CRYPT_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)

#define CRYPT_OUTPUT_SIZE                        384
#define CRYPT_MAX_PASSPHRASE_SIZE                 512
#define CRYPT_GENSALT_OUTPUT_SIZE                 192
#define CRYPT_DATA_RESERVED_SIZE                  767
#define CRYPT_DATA_INTERNAL_SIZE                  30720
#define CRYPT_SALT_OK                             0
#define CRYPT_SALT_INVALID                        1
#define CRYPT_SALT_METHOD_DISABLED                2
#define CRYPT_SALT_METHOD_LEGACY                  3
#define CRYPT_SALT_TOO_CHEAP                      4
#define CRYPT_GENSALT_IMPLEMENTS_DEFAULT_PREFIX   1
#define CRYPT_GENSALT_IMPLEMENTS_AUTO_ENTROPY     1
#define CRYPT_CHECKSALT_AVAILABLE                 1
#define CRYPT_PREFERRED_METHOD_AVAILABLE          1
#define XCRYPT_VERSION_MAJOR                      4
#define XCRYPT_VERSION_MINOR                      5
#define XCRYPT_VERSION_NUM \
  ((XCRYPT_VERSION_MAJOR << 16) | XCRYPT_VERSION_MINOR)
#define XCRYPT_VERSION_STR "4.5.2"

struct crypt_data {
  char output[CRYPT_OUTPUT_SIZE];
  char setting[CRYPT_OUTPUT_SIZE];
  char input[CRYPT_MAX_PASSPHRASE_SIZE];
  char reserved[CRYPT_DATA_RESERVED_SIZE];
  char initialized;
  char internal[CRYPT_DATA_INTERNAL_SIZE];
};

#else

struct crypt_data {
  int  initialized;
  char __buf[256];
};

#endif

char *crypt(const char *, const char *);
char *crypt_r(const char *, const char *, struct crypt_data *);

#endif
