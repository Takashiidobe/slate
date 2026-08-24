#ifndef _SLATE_BITS_MSVC_SECURE_TIME_H
#define _SLATE_BITS_MSVC_SECURE_TIME_H

#include <bits/msvc/types.h>
#include <bits/msvc/stddef.h>

#if __STDC_WANT_SECURE_LIB__
errno_t asctime_s(char *, size_t, const struct tm *);
errno_t ctime_s(char *, size_t, const time_t *);
errno_t gmtime_s(struct tm *, const time_t *);
errno_t localtime_s(struct tm *, const time_t *);
#endif

errno_t _ctime32_s(char *, size_t, const __time32_t *);
errno_t _ctime64_s(char *, size_t, const __time64_t *);
errno_t _gmtime32_s(struct tm *, const __time32_t *);
errno_t _gmtime64_s(struct tm *, const __time64_t *);
errno_t _localtime32_s(struct tm *, const __time32_t *);
errno_t _localtime64_s(struct tm *, const __time64_t *);
errno_t _strdate_s(char *, size_t);
errno_t _strtime_s(char *, size_t);

#endif
