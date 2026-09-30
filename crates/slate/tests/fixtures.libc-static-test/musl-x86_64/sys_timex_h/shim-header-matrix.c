#include <sys/timex.h>

typedef struct timeval slate_oracle_struct_ntptimeval_time;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ntptimeval *)0)->time), slate_oracle_struct_ntptimeval_time), "struct ntptimeval.time field type differs from oracle");

typedef long slate_oracle_struct_ntptimeval_maxerror;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ntptimeval *)0)->maxerror), slate_oracle_struct_ntptimeval_maxerror), "struct ntptimeval.maxerror field type differs from oracle");

typedef long slate_oracle_struct_ntptimeval_esterror;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ntptimeval *)0)->esterror), slate_oracle_struct_ntptimeval_esterror), "struct ntptimeval.esterror field type differs from oracle");

#ifndef ADJ_ESTERROR
#error "sys/timex.h:ADJ_ESTERROR macro is missing from libc-shim"
#endif

#ifndef ADJ_FREQUENCY
#error "sys/timex.h:ADJ_FREQUENCY macro is missing from libc-shim"
#endif

#ifndef ADJ_MAXERROR
#error "sys/timex.h:ADJ_MAXERROR macro is missing from libc-shim"
#endif

#ifndef ADJ_MICRO
#error "sys/timex.h:ADJ_MICRO macro is missing from libc-shim"
#endif

#ifndef ADJ_NANO
#error "sys/timex.h:ADJ_NANO macro is missing from libc-shim"
#endif

#ifndef ADJ_OFFSET
#error "sys/timex.h:ADJ_OFFSET macro is missing from libc-shim"
#endif

#ifndef ADJ_OFFSET_SINGLESHOT
#error "sys/timex.h:ADJ_OFFSET_SINGLESHOT macro is missing from libc-shim"
#endif

#ifndef ADJ_OFFSET_SS_READ
#error "sys/timex.h:ADJ_OFFSET_SS_READ macro is missing from libc-shim"
#endif

#ifndef ADJ_SETOFFSET
#error "sys/timex.h:ADJ_SETOFFSET macro is missing from libc-shim"
#endif

#ifndef ADJ_STATUS
#error "sys/timex.h:ADJ_STATUS macro is missing from libc-shim"
#endif

#ifndef ADJ_TAI
#error "sys/timex.h:ADJ_TAI macro is missing from libc-shim"
#endif

#ifndef ADJ_TICK
#error "sys/timex.h:ADJ_TICK macro is missing from libc-shim"
#endif

#ifndef ADJ_TIMECONST
#error "sys/timex.h:ADJ_TIMECONST macro is missing from libc-shim"
#endif

#ifndef MAXTC
#error "sys/timex.h:MAXTC macro is missing from libc-shim"
#endif

#ifndef MOD_CLKA
#error "sys/timex.h:MOD_CLKA macro is missing from libc-shim"
#endif

#ifndef MOD_CLKB
#error "sys/timex.h:MOD_CLKB macro is missing from libc-shim"
#endif

#ifndef MOD_ESTERROR
#error "sys/timex.h:MOD_ESTERROR macro is missing from libc-shim"
#endif

#ifndef MOD_FREQUENCY
#error "sys/timex.h:MOD_FREQUENCY macro is missing from libc-shim"
#endif

#ifndef MOD_MAXERROR
#error "sys/timex.h:MOD_MAXERROR macro is missing from libc-shim"
#endif

#ifndef MOD_MICRO
#error "sys/timex.h:MOD_MICRO macro is missing from libc-shim"
#endif

#ifndef MOD_NANO
#error "sys/timex.h:MOD_NANO macro is missing from libc-shim"
#endif

#ifndef MOD_OFFSET
#error "sys/timex.h:MOD_OFFSET macro is missing from libc-shim"
#endif

#ifndef MOD_STATUS
#error "sys/timex.h:MOD_STATUS macro is missing from libc-shim"
#endif

#ifndef MOD_TAI
#error "sys/timex.h:MOD_TAI macro is missing from libc-shim"
#endif

#ifndef MOD_TIMECONST
#error "sys/timex.h:MOD_TIMECONST macro is missing from libc-shim"
#endif

#ifndef STA_CLK
#error "sys/timex.h:STA_CLK macro is missing from libc-shim"
#endif

#ifndef STA_CLOCKERR
#error "sys/timex.h:STA_CLOCKERR macro is missing from libc-shim"
#endif

#ifndef STA_DEL
#error "sys/timex.h:STA_DEL macro is missing from libc-shim"
#endif

#ifndef STA_FLL
#error "sys/timex.h:STA_FLL macro is missing from libc-shim"
#endif

#ifndef STA_FREQHOLD
#error "sys/timex.h:STA_FREQHOLD macro is missing from libc-shim"
#endif

#ifndef STA_INS
#error "sys/timex.h:STA_INS macro is missing from libc-shim"
#endif

#ifndef STA_MODE
#error "sys/timex.h:STA_MODE macro is missing from libc-shim"
#endif

#ifndef STA_NANO
#error "sys/timex.h:STA_NANO macro is missing from libc-shim"
#endif

#ifndef STA_PLL
#error "sys/timex.h:STA_PLL macro is missing from libc-shim"
#endif

#ifndef STA_PPSERROR
#error "sys/timex.h:STA_PPSERROR macro is missing from libc-shim"
#endif

#ifndef STA_PPSFREQ
#error "sys/timex.h:STA_PPSFREQ macro is missing from libc-shim"
#endif

#ifndef STA_PPSJITTER
#error "sys/timex.h:STA_PPSJITTER macro is missing from libc-shim"
#endif

#ifndef STA_PPSSIGNAL
#error "sys/timex.h:STA_PPSSIGNAL macro is missing from libc-shim"
#endif

#ifndef STA_PPSTIME
#error "sys/timex.h:STA_PPSTIME macro is missing from libc-shim"
#endif

#ifndef STA_PPSWANDER
#error "sys/timex.h:STA_PPSWANDER macro is missing from libc-shim"
#endif

#ifndef STA_RONLY
#error "sys/timex.h:STA_RONLY macro is missing from libc-shim"
#endif

#ifndef STA_UNSYNC
#error "sys/timex.h:STA_UNSYNC macro is missing from libc-shim"
#endif

#ifndef TIME_BAD
#error "sys/timex.h:TIME_BAD macro is missing from libc-shim"
#endif

#ifndef TIME_DEL
#error "sys/timex.h:TIME_DEL macro is missing from libc-shim"
#endif

#ifndef TIME_ERROR
#error "sys/timex.h:TIME_ERROR macro is missing from libc-shim"
#endif

#ifndef TIME_INS
#error "sys/timex.h:TIME_INS macro is missing from libc-shim"
#endif

#ifndef TIME_OK
#error "sys/timex.h:TIME_OK macro is missing from libc-shim"
#endif

#ifndef TIME_OOP
#error "sys/timex.h:TIME_OOP macro is missing from libc-shim"
#endif

#ifndef TIME_WAIT
#error "sys/timex.h:TIME_WAIT macro is missing from libc-shim"
#endif

int main(void) { return 0; }
