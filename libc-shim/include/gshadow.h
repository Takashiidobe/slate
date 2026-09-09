#ifndef _SLATE_GSHADOW_H
#define _SLATE_GSHADOW_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)

#include <paths.h>

#define __NEED_FILE
#define __NEED_size_t
#include <bits/types.h>

#define GSHADOW _PATH_GSHADOW

struct sgrp {
  char *sg_namp;
  char *sg_passwd;
  char **sg_adm;
  char **sg_mem;
};

void setsgent(void);
void endsgent(void);
struct sgrp *getsgent(void);
struct sgrp *getsgnam(const char *);
struct sgrp *sgetsgent(const char *);
struct sgrp *fgetsgent(FILE *);
int putsgent(const struct sgrp *, FILE *);

#if defined(_DEFAULT_SOURCE) || defined(_GNU_SOURCE)
int getsgent_r(struct sgrp *, char *, size_t, struct sgrp **);
int getsgnam_r(const char *, struct sgrp *, char *, size_t, struct sgrp **);
int sgetsgent_r(const char *, struct sgrp *, char *, size_t, struct sgrp **);
int fgetsgent_r(FILE *, struct sgrp *, char *, size_t, struct sgrp **);
#endif

#endif

#endif
