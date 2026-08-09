#ifndef _SLATE_BITS_MSVC_TYPES_H
#define _SLATE_BITS_MSVC_TYPES_H

#ifndef __DEFINED_errno_t
typedef int errno_t;
#define __DEFINED_errno_t
#endif

#endif

#if defined(__NEED_FILE) && !defined(__DEFINED_FILE)
typedef struct _iobuf FILE;
#define __DEFINED_FILE
#endif
