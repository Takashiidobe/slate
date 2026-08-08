#ifndef _SLATE_BITS_MSVC_STDDEF_H
#define _SLATE_BITS_MSVC_STDDEF_H

#ifndef __STDC_WANT_SECURE_LIB__
#define __STDC_WANT_SECURE_LIB__ 1
#endif

#if __STDC_WANT_SECURE_LIB__
typedef size_t rsize_t;
#endif

#endif
