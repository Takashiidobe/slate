#ifndef _SLATE_ALLOCA_H
#define _SLATE_ALLOCA_H

#include <features.h>

#define __NEED_size_t
#include <bits/types.h>

void *alloca(size_t);

#define alloca __builtin_alloca

#endif
