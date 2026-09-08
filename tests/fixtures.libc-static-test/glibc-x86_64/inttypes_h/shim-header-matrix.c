#include <inttypes.h>

extern long slate_oracle_imaxabs(long);
extern struct imaxdiv_t slate_oracle_imaxdiv(long, long);
extern long slate_oracle_strtoimax(const char *restrict, char **restrict, int);
extern unsigned long slate_oracle_strtoumax(const char *restrict, char **restrict, int);
extern unsigned long slate_oracle_umaxabs(long);
extern long slate_oracle_wcstoimax(const int *restrict, int **restrict, int);
extern unsigned long slate_oracle_wcstoumax(const int *restrict, int **restrict, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_imaxabs), __typeof__(imaxabs)),
    "inttypes.h:imaxabs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_imaxdiv), __typeof__(imaxdiv)),
    "inttypes.h:imaxdiv declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtoimax), __typeof__(strtoimax)),
    "inttypes.h:strtoimax declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtoumax), __typeof__(strtoumax)),
    "inttypes.h:strtoumax declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_umaxabs), __typeof__(umaxabs)),
    "inttypes.h:umaxabs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wcstoimax), __typeof__(wcstoimax)),
    "inttypes.h:wcstoimax declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wcstoumax), __typeof__(wcstoumax)),
    "inttypes.h:wcstoumax declaration differs from oracle");

static __typeof__(imaxabs) *const slate_reference_imaxabs = &imaxabs;
static __typeof__(imaxdiv) *const slate_reference_imaxdiv = &imaxdiv;
static __typeof__(strtoimax) *const slate_reference_strtoimax = &strtoimax;
static __typeof__(strtoumax) *const slate_reference_strtoumax = &strtoumax;
static __typeof__(umaxabs) *const slate_reference_umaxabs = &umaxabs;
static __typeof__(wcstoimax) *const slate_reference_wcstoimax = &wcstoimax;
static __typeof__(wcstoumax) *const slate_reference_wcstoumax = &wcstoumax;

typedef struct struct imaxdiv_t slate_oracle_typedef_imaxdiv_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_imaxdiv_t, imaxdiv_t), "typedef imaxdiv_t differs from oracle");

typedef short slate_oracle_typedef_int16_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int16_t, int16_t), "typedef int16_t differs from oracle");

typedef int slate_oracle_typedef_int32_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int32_t, int32_t), "typedef int32_t differs from oracle");

typedef long slate_oracle_typedef_int64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int64_t, int64_t), "typedef int64_t differs from oracle");

typedef signed char slate_oracle_typedef_int8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int8_t, int8_t), "typedef int8_t differs from oracle");

typedef long slate_oracle_typedef_int_fast16_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int_fast16_t, int_fast16_t), "typedef int_fast16_t differs from oracle");

typedef long slate_oracle_typedef_int_fast32_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int_fast32_t, int_fast32_t), "typedef int_fast32_t differs from oracle");

typedef long slate_oracle_typedef_int_fast64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int_fast64_t, int_fast64_t), "typedef int_fast64_t differs from oracle");

typedef signed char slate_oracle_typedef_int_fast8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int_fast8_t, int_fast8_t), "typedef int_fast8_t differs from oracle");

typedef short slate_oracle_typedef_int_least16_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int_least16_t, int_least16_t), "typedef int_least16_t differs from oracle");

typedef int slate_oracle_typedef_int_least32_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int_least32_t, int_least32_t), "typedef int_least32_t differs from oracle");

typedef long slate_oracle_typedef_int_least64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int_least64_t, int_least64_t), "typedef int_least64_t differs from oracle");

typedef signed char slate_oracle_typedef_int_least8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int_least8_t, int_least8_t), "typedef int_least8_t differs from oracle");

typedef long slate_oracle_typedef_intmax_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_intmax_t, intmax_t), "typedef intmax_t differs from oracle");

typedef long slate_oracle_typedef_intptr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_intptr_t, intptr_t), "typedef intptr_t differs from oracle");

typedef unsigned short slate_oracle_typedef_uint16_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint16_t, uint16_t), "typedef uint16_t differs from oracle");

typedef unsigned int slate_oracle_typedef_uint32_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint32_t, uint32_t), "typedef uint32_t differs from oracle");

typedef unsigned long slate_oracle_typedef_uint64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint64_t, uint64_t), "typedef uint64_t differs from oracle");

typedef unsigned char slate_oracle_typedef_uint8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint8_t, uint8_t), "typedef uint8_t differs from oracle");

typedef unsigned long slate_oracle_typedef_uint_fast16_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint_fast16_t, uint_fast16_t), "typedef uint_fast16_t differs from oracle");

typedef unsigned long slate_oracle_typedef_uint_fast32_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint_fast32_t, uint_fast32_t), "typedef uint_fast32_t differs from oracle");

typedef unsigned long slate_oracle_typedef_uint_fast64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint_fast64_t, uint_fast64_t), "typedef uint_fast64_t differs from oracle");

typedef unsigned char slate_oracle_typedef_uint_fast8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint_fast8_t, uint_fast8_t), "typedef uint_fast8_t differs from oracle");

typedef unsigned short slate_oracle_typedef_uint_least16_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint_least16_t, uint_least16_t), "typedef uint_least16_t differs from oracle");

typedef unsigned int slate_oracle_typedef_uint_least32_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint_least32_t, uint_least32_t), "typedef uint_least32_t differs from oracle");

typedef unsigned long slate_oracle_typedef_uint_least64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint_least64_t, uint_least64_t), "typedef uint_least64_t differs from oracle");

typedef unsigned char slate_oracle_typedef_uint_least8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint_least8_t, uint_least8_t), "typedef uint_least8_t differs from oracle");

typedef unsigned long slate_oracle_typedef_uintmax_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uintmax_t, uintmax_t), "typedef uintmax_t differs from oracle");

typedef unsigned long slate_oracle_typedef_uintptr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uintptr_t, uintptr_t), "typedef uintptr_t differs from oracle");

#ifndef INT16_C
#error "inttypes.h:INT16_C macro is missing from libc-shim"
#endif

#ifndef INT16_MAX
#error "inttypes.h:INT16_MAX macro is missing from libc-shim"
#endif

#ifndef INT16_MIN
#error "inttypes.h:INT16_MIN macro is missing from libc-shim"
#endif

#ifndef INT16_WIDTH
#error "inttypes.h:INT16_WIDTH macro is missing from libc-shim"
#endif

#ifndef INT32_C
#error "inttypes.h:INT32_C macro is missing from libc-shim"
#endif

#ifndef INT32_MAX
#error "inttypes.h:INT32_MAX macro is missing from libc-shim"
#endif

#ifndef INT32_MIN
#error "inttypes.h:INT32_MIN macro is missing from libc-shim"
#endif

#ifndef INT32_WIDTH
#error "inttypes.h:INT32_WIDTH macro is missing from libc-shim"
#endif

#ifndef INT64_C
#error "inttypes.h:INT64_C macro is missing from libc-shim"
#endif

#ifndef INT64_MAX
#error "inttypes.h:INT64_MAX macro is missing from libc-shim"
#endif

#ifndef INT64_MIN
#error "inttypes.h:INT64_MIN macro is missing from libc-shim"
#endif

#ifndef INT64_WIDTH
#error "inttypes.h:INT64_WIDTH macro is missing from libc-shim"
#endif

#ifndef INT8_C
#error "inttypes.h:INT8_C macro is missing from libc-shim"
#endif

#ifndef INT8_MAX
#error "inttypes.h:INT8_MAX macro is missing from libc-shim"
#endif

#ifndef INT8_MIN
#error "inttypes.h:INT8_MIN macro is missing from libc-shim"
#endif

#ifndef INT8_WIDTH
#error "inttypes.h:INT8_WIDTH macro is missing from libc-shim"
#endif

#ifndef INTMAX_C
#error "inttypes.h:INTMAX_C macro is missing from libc-shim"
#endif

#ifndef INTMAX_MAX
#error "inttypes.h:INTMAX_MAX macro is missing from libc-shim"
#endif

#ifndef INTMAX_MIN
#error "inttypes.h:INTMAX_MIN macro is missing from libc-shim"
#endif

#ifndef INTMAX_WIDTH
#error "inttypes.h:INTMAX_WIDTH macro is missing from libc-shim"
#endif

#ifndef INTPTR_MAX
#error "inttypes.h:INTPTR_MAX macro is missing from libc-shim"
#endif

#ifndef INTPTR_MIN
#error "inttypes.h:INTPTR_MIN macro is missing from libc-shim"
#endif

#ifndef INTPTR_WIDTH
#error "inttypes.h:INTPTR_WIDTH macro is missing from libc-shim"
#endif

#ifndef INT_FAST16_MAX
#error "inttypes.h:INT_FAST16_MAX macro is missing from libc-shim"
#endif

#ifndef INT_FAST16_MIN
#error "inttypes.h:INT_FAST16_MIN macro is missing from libc-shim"
#endif

#ifndef INT_FAST16_WIDTH
#error "inttypes.h:INT_FAST16_WIDTH macro is missing from libc-shim"
#endif

#ifndef INT_FAST32_MAX
#error "inttypes.h:INT_FAST32_MAX macro is missing from libc-shim"
#endif

#ifndef INT_FAST32_MIN
#error "inttypes.h:INT_FAST32_MIN macro is missing from libc-shim"
#endif

#ifndef INT_FAST32_WIDTH
#error "inttypes.h:INT_FAST32_WIDTH macro is missing from libc-shim"
#endif

#ifndef INT_FAST64_MAX
#error "inttypes.h:INT_FAST64_MAX macro is missing from libc-shim"
#endif

#ifndef INT_FAST64_MIN
#error "inttypes.h:INT_FAST64_MIN macro is missing from libc-shim"
#endif

#ifndef INT_FAST64_WIDTH
#error "inttypes.h:INT_FAST64_WIDTH macro is missing from libc-shim"
#endif

#ifndef INT_FAST8_MAX
#error "inttypes.h:INT_FAST8_MAX macro is missing from libc-shim"
#endif

#ifndef INT_FAST8_MIN
#error "inttypes.h:INT_FAST8_MIN macro is missing from libc-shim"
#endif

#ifndef INT_FAST8_WIDTH
#error "inttypes.h:INT_FAST8_WIDTH macro is missing from libc-shim"
#endif

#ifndef INT_LEAST16_MAX
#error "inttypes.h:INT_LEAST16_MAX macro is missing from libc-shim"
#endif

#ifndef INT_LEAST16_MIN
#error "inttypes.h:INT_LEAST16_MIN macro is missing from libc-shim"
#endif

#ifndef INT_LEAST16_WIDTH
#error "inttypes.h:INT_LEAST16_WIDTH macro is missing from libc-shim"
#endif

#ifndef INT_LEAST32_MAX
#error "inttypes.h:INT_LEAST32_MAX macro is missing from libc-shim"
#endif

#ifndef INT_LEAST32_MIN
#error "inttypes.h:INT_LEAST32_MIN macro is missing from libc-shim"
#endif

#ifndef INT_LEAST32_WIDTH
#error "inttypes.h:INT_LEAST32_WIDTH macro is missing from libc-shim"
#endif

#ifndef INT_LEAST64_MAX
#error "inttypes.h:INT_LEAST64_MAX macro is missing from libc-shim"
#endif

#ifndef INT_LEAST64_MIN
#error "inttypes.h:INT_LEAST64_MIN macro is missing from libc-shim"
#endif

#ifndef INT_LEAST64_WIDTH
#error "inttypes.h:INT_LEAST64_WIDTH macro is missing from libc-shim"
#endif

#ifndef INT_LEAST8_MAX
#error "inttypes.h:INT_LEAST8_MAX macro is missing from libc-shim"
#endif

#ifndef INT_LEAST8_MIN
#error "inttypes.h:INT_LEAST8_MIN macro is missing from libc-shim"
#endif

#ifndef INT_LEAST8_WIDTH
#error "inttypes.h:INT_LEAST8_WIDTH macro is missing from libc-shim"
#endif

#ifndef PRIB16
#error "inttypes.h:PRIB16 macro is missing from libc-shim"
#endif

#ifndef PRIB32
#error "inttypes.h:PRIB32 macro is missing from libc-shim"
#endif

#ifndef PRIB64
#error "inttypes.h:PRIB64 macro is missing from libc-shim"
#endif

#ifndef PRIB8
#error "inttypes.h:PRIB8 macro is missing from libc-shim"
#endif

#ifndef PRIBFAST16
#error "inttypes.h:PRIBFAST16 macro is missing from libc-shim"
#endif

#ifndef PRIBFAST32
#error "inttypes.h:PRIBFAST32 macro is missing from libc-shim"
#endif

#ifndef PRIBFAST64
#error "inttypes.h:PRIBFAST64 macro is missing from libc-shim"
#endif

#ifndef PRIBFAST8
#error "inttypes.h:PRIBFAST8 macro is missing from libc-shim"
#endif

#ifndef PRIBLEAST16
#error "inttypes.h:PRIBLEAST16 macro is missing from libc-shim"
#endif

#ifndef PRIBLEAST32
#error "inttypes.h:PRIBLEAST32 macro is missing from libc-shim"
#endif

#ifndef PRIBLEAST64
#error "inttypes.h:PRIBLEAST64 macro is missing from libc-shim"
#endif

#ifndef PRIBLEAST8
#error "inttypes.h:PRIBLEAST8 macro is missing from libc-shim"
#endif

#ifndef PRIBMAX
#error "inttypes.h:PRIBMAX macro is missing from libc-shim"
#endif

#ifndef PRIBPTR
#error "inttypes.h:PRIBPTR macro is missing from libc-shim"
#endif

#ifndef PRIX16
#error "inttypes.h:PRIX16 macro is missing from libc-shim"
#endif

#ifndef PRIX32
#error "inttypes.h:PRIX32 macro is missing from libc-shim"
#endif

#ifndef PRIX64
#error "inttypes.h:PRIX64 macro is missing from libc-shim"
#endif

#ifndef PRIX8
#error "inttypes.h:PRIX8 macro is missing from libc-shim"
#endif

#ifndef PRIXFAST16
#error "inttypes.h:PRIXFAST16 macro is missing from libc-shim"
#endif

#ifndef PRIXFAST32
#error "inttypes.h:PRIXFAST32 macro is missing from libc-shim"
#endif

#ifndef PRIXFAST64
#error "inttypes.h:PRIXFAST64 macro is missing from libc-shim"
#endif

#ifndef PRIXFAST8
#error "inttypes.h:PRIXFAST8 macro is missing from libc-shim"
#endif

#ifndef PRIXLEAST16
#error "inttypes.h:PRIXLEAST16 macro is missing from libc-shim"
#endif

#ifndef PRIXLEAST32
#error "inttypes.h:PRIXLEAST32 macro is missing from libc-shim"
#endif

#ifndef PRIXLEAST64
#error "inttypes.h:PRIXLEAST64 macro is missing from libc-shim"
#endif

#ifndef PRIXLEAST8
#error "inttypes.h:PRIXLEAST8 macro is missing from libc-shim"
#endif

#ifndef PRIXMAX
#error "inttypes.h:PRIXMAX macro is missing from libc-shim"
#endif

#ifndef PRIXPTR
#error "inttypes.h:PRIXPTR macro is missing from libc-shim"
#endif

#ifndef PRIb16
#error "inttypes.h:PRIb16 macro is missing from libc-shim"
#endif

#ifndef PRIb32
#error "inttypes.h:PRIb32 macro is missing from libc-shim"
#endif

#ifndef PRIb64
#error "inttypes.h:PRIb64 macro is missing from libc-shim"
#endif

#ifndef PRIb8
#error "inttypes.h:PRIb8 macro is missing from libc-shim"
#endif

#ifndef PRIbFAST16
#error "inttypes.h:PRIbFAST16 macro is missing from libc-shim"
#endif

#ifndef PRIbFAST32
#error "inttypes.h:PRIbFAST32 macro is missing from libc-shim"
#endif

#ifndef PRIbFAST64
#error "inttypes.h:PRIbFAST64 macro is missing from libc-shim"
#endif

#ifndef PRIbFAST8
#error "inttypes.h:PRIbFAST8 macro is missing from libc-shim"
#endif

#ifndef PRIbLEAST16
#error "inttypes.h:PRIbLEAST16 macro is missing from libc-shim"
#endif

#ifndef PRIbLEAST32
#error "inttypes.h:PRIbLEAST32 macro is missing from libc-shim"
#endif

#ifndef PRIbLEAST64
#error "inttypes.h:PRIbLEAST64 macro is missing from libc-shim"
#endif

#ifndef PRIbLEAST8
#error "inttypes.h:PRIbLEAST8 macro is missing from libc-shim"
#endif

#ifndef PRIbMAX
#error "inttypes.h:PRIbMAX macro is missing from libc-shim"
#endif

#ifndef PRIbPTR
#error "inttypes.h:PRIbPTR macro is missing from libc-shim"
#endif

#ifndef PRId16
#error "inttypes.h:PRId16 macro is missing from libc-shim"
#endif

#ifndef PRId32
#error "inttypes.h:PRId32 macro is missing from libc-shim"
#endif

#ifndef PRId64
#error "inttypes.h:PRId64 macro is missing from libc-shim"
#endif

#ifndef PRId8
#error "inttypes.h:PRId8 macro is missing from libc-shim"
#endif

#ifndef PRIdFAST16
#error "inttypes.h:PRIdFAST16 macro is missing from libc-shim"
#endif

#ifndef PRIdFAST32
#error "inttypes.h:PRIdFAST32 macro is missing from libc-shim"
#endif

#ifndef PRIdFAST64
#error "inttypes.h:PRIdFAST64 macro is missing from libc-shim"
#endif

#ifndef PRIdFAST8
#error "inttypes.h:PRIdFAST8 macro is missing from libc-shim"
#endif

#ifndef PRIdLEAST16
#error "inttypes.h:PRIdLEAST16 macro is missing from libc-shim"
#endif

#ifndef PRIdLEAST32
#error "inttypes.h:PRIdLEAST32 macro is missing from libc-shim"
#endif

#ifndef PRIdLEAST64
#error "inttypes.h:PRIdLEAST64 macro is missing from libc-shim"
#endif

#ifndef PRIdLEAST8
#error "inttypes.h:PRIdLEAST8 macro is missing from libc-shim"
#endif

#ifndef PRIdMAX
#error "inttypes.h:PRIdMAX macro is missing from libc-shim"
#endif

#ifndef PRIdPTR
#error "inttypes.h:PRIdPTR macro is missing from libc-shim"
#endif

#ifndef PRIi16
#error "inttypes.h:PRIi16 macro is missing from libc-shim"
#endif

#ifndef PRIi32
#error "inttypes.h:PRIi32 macro is missing from libc-shim"
#endif

#ifndef PRIi64
#error "inttypes.h:PRIi64 macro is missing from libc-shim"
#endif

#ifndef PRIi8
#error "inttypes.h:PRIi8 macro is missing from libc-shim"
#endif

#ifndef PRIiFAST16
#error "inttypes.h:PRIiFAST16 macro is missing from libc-shim"
#endif

#ifndef PRIiFAST32
#error "inttypes.h:PRIiFAST32 macro is missing from libc-shim"
#endif

#ifndef PRIiFAST64
#error "inttypes.h:PRIiFAST64 macro is missing from libc-shim"
#endif

#ifndef PRIiFAST8
#error "inttypes.h:PRIiFAST8 macro is missing from libc-shim"
#endif

#ifndef PRIiLEAST16
#error "inttypes.h:PRIiLEAST16 macro is missing from libc-shim"
#endif

#ifndef PRIiLEAST32
#error "inttypes.h:PRIiLEAST32 macro is missing from libc-shim"
#endif

#ifndef PRIiLEAST64
#error "inttypes.h:PRIiLEAST64 macro is missing from libc-shim"
#endif

#ifndef PRIiLEAST8
#error "inttypes.h:PRIiLEAST8 macro is missing from libc-shim"
#endif

#ifndef PRIiMAX
#error "inttypes.h:PRIiMAX macro is missing from libc-shim"
#endif

#ifndef PRIiPTR
#error "inttypes.h:PRIiPTR macro is missing from libc-shim"
#endif

#ifndef PRIo16
#error "inttypes.h:PRIo16 macro is missing from libc-shim"
#endif

#ifndef PRIo32
#error "inttypes.h:PRIo32 macro is missing from libc-shim"
#endif

#ifndef PRIo64
#error "inttypes.h:PRIo64 macro is missing from libc-shim"
#endif

#ifndef PRIo8
#error "inttypes.h:PRIo8 macro is missing from libc-shim"
#endif

#ifndef PRIoFAST16
#error "inttypes.h:PRIoFAST16 macro is missing from libc-shim"
#endif

#ifndef PRIoFAST32
#error "inttypes.h:PRIoFAST32 macro is missing from libc-shim"
#endif

#ifndef PRIoFAST64
#error "inttypes.h:PRIoFAST64 macro is missing from libc-shim"
#endif

#ifndef PRIoFAST8
#error "inttypes.h:PRIoFAST8 macro is missing from libc-shim"
#endif

#ifndef PRIoLEAST16
#error "inttypes.h:PRIoLEAST16 macro is missing from libc-shim"
#endif

#ifndef PRIoLEAST32
#error "inttypes.h:PRIoLEAST32 macro is missing from libc-shim"
#endif

#ifndef PRIoLEAST64
#error "inttypes.h:PRIoLEAST64 macro is missing from libc-shim"
#endif

#ifndef PRIoLEAST8
#error "inttypes.h:PRIoLEAST8 macro is missing from libc-shim"
#endif

#ifndef PRIoMAX
#error "inttypes.h:PRIoMAX macro is missing from libc-shim"
#endif

#ifndef PRIoPTR
#error "inttypes.h:PRIoPTR macro is missing from libc-shim"
#endif

#ifndef PRIu16
#error "inttypes.h:PRIu16 macro is missing from libc-shim"
#endif

#ifndef PRIu32
#error "inttypes.h:PRIu32 macro is missing from libc-shim"
#endif

#ifndef PRIu64
#error "inttypes.h:PRIu64 macro is missing from libc-shim"
#endif

#ifndef PRIu8
#error "inttypes.h:PRIu8 macro is missing from libc-shim"
#endif

#ifndef PRIuFAST16
#error "inttypes.h:PRIuFAST16 macro is missing from libc-shim"
#endif

#ifndef PRIuFAST32
#error "inttypes.h:PRIuFAST32 macro is missing from libc-shim"
#endif

#ifndef PRIuFAST64
#error "inttypes.h:PRIuFAST64 macro is missing from libc-shim"
#endif

#ifndef PRIuFAST8
#error "inttypes.h:PRIuFAST8 macro is missing from libc-shim"
#endif

#ifndef PRIuLEAST16
#error "inttypes.h:PRIuLEAST16 macro is missing from libc-shim"
#endif

#ifndef PRIuLEAST32
#error "inttypes.h:PRIuLEAST32 macro is missing from libc-shim"
#endif

#ifndef PRIuLEAST64
#error "inttypes.h:PRIuLEAST64 macro is missing from libc-shim"
#endif

#ifndef PRIuLEAST8
#error "inttypes.h:PRIuLEAST8 macro is missing from libc-shim"
#endif

#ifndef PRIuMAX
#error "inttypes.h:PRIuMAX macro is missing from libc-shim"
#endif

#ifndef PRIuPTR
#error "inttypes.h:PRIuPTR macro is missing from libc-shim"
#endif

#ifndef PRIx16
#error "inttypes.h:PRIx16 macro is missing from libc-shim"
#endif

#ifndef PRIx32
#error "inttypes.h:PRIx32 macro is missing from libc-shim"
#endif

#ifndef PRIx64
#error "inttypes.h:PRIx64 macro is missing from libc-shim"
#endif

#ifndef PRIx8
#error "inttypes.h:PRIx8 macro is missing from libc-shim"
#endif

#ifndef PRIxFAST16
#error "inttypes.h:PRIxFAST16 macro is missing from libc-shim"
#endif

#ifndef PRIxFAST32
#error "inttypes.h:PRIxFAST32 macro is missing from libc-shim"
#endif

#ifndef PRIxFAST64
#error "inttypes.h:PRIxFAST64 macro is missing from libc-shim"
#endif

#ifndef PRIxFAST8
#error "inttypes.h:PRIxFAST8 macro is missing from libc-shim"
#endif

#ifndef PRIxLEAST16
#error "inttypes.h:PRIxLEAST16 macro is missing from libc-shim"
#endif

#ifndef PRIxLEAST32
#error "inttypes.h:PRIxLEAST32 macro is missing from libc-shim"
#endif

#ifndef PRIxLEAST64
#error "inttypes.h:PRIxLEAST64 macro is missing from libc-shim"
#endif

#ifndef PRIxLEAST8
#error "inttypes.h:PRIxLEAST8 macro is missing from libc-shim"
#endif

#ifndef PRIxMAX
#error "inttypes.h:PRIxMAX macro is missing from libc-shim"
#endif

#ifndef PRIxPTR
#error "inttypes.h:PRIxPTR macro is missing from libc-shim"
#endif

#ifndef PTRDIFF_MAX
#error "inttypes.h:PTRDIFF_MAX macro is missing from libc-shim"
#endif

#ifndef PTRDIFF_MIN
#error "inttypes.h:PTRDIFF_MIN macro is missing from libc-shim"
#endif

#ifndef PTRDIFF_WIDTH
#error "inttypes.h:PTRDIFF_WIDTH macro is missing from libc-shim"
#endif

#ifndef SCNb16
#error "inttypes.h:SCNb16 macro is missing from libc-shim"
#endif

#ifndef SCNb32
#error "inttypes.h:SCNb32 macro is missing from libc-shim"
#endif

#ifndef SCNb64
#error "inttypes.h:SCNb64 macro is missing from libc-shim"
#endif

#ifndef SCNb8
#error "inttypes.h:SCNb8 macro is missing from libc-shim"
#endif

#ifndef SCNbFAST16
#error "inttypes.h:SCNbFAST16 macro is missing from libc-shim"
#endif

#ifndef SCNbFAST32
#error "inttypes.h:SCNbFAST32 macro is missing from libc-shim"
#endif

#ifndef SCNbFAST64
#error "inttypes.h:SCNbFAST64 macro is missing from libc-shim"
#endif

#ifndef SCNbFAST8
#error "inttypes.h:SCNbFAST8 macro is missing from libc-shim"
#endif

#ifndef SCNbLEAST16
#error "inttypes.h:SCNbLEAST16 macro is missing from libc-shim"
#endif

#ifndef SCNbLEAST32
#error "inttypes.h:SCNbLEAST32 macro is missing from libc-shim"
#endif

#ifndef SCNbLEAST64
#error "inttypes.h:SCNbLEAST64 macro is missing from libc-shim"
#endif

#ifndef SCNbLEAST8
#error "inttypes.h:SCNbLEAST8 macro is missing from libc-shim"
#endif

#ifndef SCNbMAX
#error "inttypes.h:SCNbMAX macro is missing from libc-shim"
#endif

#ifndef SCNbPTR
#error "inttypes.h:SCNbPTR macro is missing from libc-shim"
#endif

#ifndef SCNd16
#error "inttypes.h:SCNd16 macro is missing from libc-shim"
#endif

#ifndef SCNd32
#error "inttypes.h:SCNd32 macro is missing from libc-shim"
#endif

#ifndef SCNd64
#error "inttypes.h:SCNd64 macro is missing from libc-shim"
#endif

#ifndef SCNd8
#error "inttypes.h:SCNd8 macro is missing from libc-shim"
#endif

#ifndef SCNdFAST16
#error "inttypes.h:SCNdFAST16 macro is missing from libc-shim"
#endif

#ifndef SCNdFAST32
#error "inttypes.h:SCNdFAST32 macro is missing from libc-shim"
#endif

#ifndef SCNdFAST64
#error "inttypes.h:SCNdFAST64 macro is missing from libc-shim"
#endif

#ifndef SCNdFAST8
#error "inttypes.h:SCNdFAST8 macro is missing from libc-shim"
#endif

#ifndef SCNdLEAST16
#error "inttypes.h:SCNdLEAST16 macro is missing from libc-shim"
#endif

#ifndef SCNdLEAST32
#error "inttypes.h:SCNdLEAST32 macro is missing from libc-shim"
#endif

#ifndef SCNdLEAST64
#error "inttypes.h:SCNdLEAST64 macro is missing from libc-shim"
#endif

#ifndef SCNdLEAST8
#error "inttypes.h:SCNdLEAST8 macro is missing from libc-shim"
#endif

#ifndef SCNdMAX
#error "inttypes.h:SCNdMAX macro is missing from libc-shim"
#endif

#ifndef SCNdPTR
#error "inttypes.h:SCNdPTR macro is missing from libc-shim"
#endif

#ifndef SCNi16
#error "inttypes.h:SCNi16 macro is missing from libc-shim"
#endif

#ifndef SCNi32
#error "inttypes.h:SCNi32 macro is missing from libc-shim"
#endif

#ifndef SCNi64
#error "inttypes.h:SCNi64 macro is missing from libc-shim"
#endif

#ifndef SCNi8
#error "inttypes.h:SCNi8 macro is missing from libc-shim"
#endif

#ifndef SCNiFAST16
#error "inttypes.h:SCNiFAST16 macro is missing from libc-shim"
#endif

#ifndef SCNiFAST32
#error "inttypes.h:SCNiFAST32 macro is missing from libc-shim"
#endif

#ifndef SCNiFAST64
#error "inttypes.h:SCNiFAST64 macro is missing from libc-shim"
#endif

#ifndef SCNiFAST8
#error "inttypes.h:SCNiFAST8 macro is missing from libc-shim"
#endif

#ifndef SCNiLEAST16
#error "inttypes.h:SCNiLEAST16 macro is missing from libc-shim"
#endif

#ifndef SCNiLEAST32
#error "inttypes.h:SCNiLEAST32 macro is missing from libc-shim"
#endif

#ifndef SCNiLEAST64
#error "inttypes.h:SCNiLEAST64 macro is missing from libc-shim"
#endif

#ifndef SCNiLEAST8
#error "inttypes.h:SCNiLEAST8 macro is missing from libc-shim"
#endif

#ifndef SCNiMAX
#error "inttypes.h:SCNiMAX macro is missing from libc-shim"
#endif

#ifndef SCNiPTR
#error "inttypes.h:SCNiPTR macro is missing from libc-shim"
#endif

#ifndef SCNo16
#error "inttypes.h:SCNo16 macro is missing from libc-shim"
#endif

#ifndef SCNo32
#error "inttypes.h:SCNo32 macro is missing from libc-shim"
#endif

#ifndef SCNo64
#error "inttypes.h:SCNo64 macro is missing from libc-shim"
#endif

#ifndef SCNo8
#error "inttypes.h:SCNo8 macro is missing from libc-shim"
#endif

#ifndef SCNoFAST16
#error "inttypes.h:SCNoFAST16 macro is missing from libc-shim"
#endif

#ifndef SCNoFAST32
#error "inttypes.h:SCNoFAST32 macro is missing from libc-shim"
#endif

#ifndef SCNoFAST64
#error "inttypes.h:SCNoFAST64 macro is missing from libc-shim"
#endif

#ifndef SCNoFAST8
#error "inttypes.h:SCNoFAST8 macro is missing from libc-shim"
#endif

#ifndef SCNoLEAST16
#error "inttypes.h:SCNoLEAST16 macro is missing from libc-shim"
#endif

#ifndef SCNoLEAST32
#error "inttypes.h:SCNoLEAST32 macro is missing from libc-shim"
#endif

#ifndef SCNoLEAST64
#error "inttypes.h:SCNoLEAST64 macro is missing from libc-shim"
#endif

#ifndef SCNoLEAST8
#error "inttypes.h:SCNoLEAST8 macro is missing from libc-shim"
#endif

#ifndef SCNoMAX
#error "inttypes.h:SCNoMAX macro is missing from libc-shim"
#endif

#ifndef SCNoPTR
#error "inttypes.h:SCNoPTR macro is missing from libc-shim"
#endif

#ifndef SCNu16
#error "inttypes.h:SCNu16 macro is missing from libc-shim"
#endif

#ifndef SCNu32
#error "inttypes.h:SCNu32 macro is missing from libc-shim"
#endif

#ifndef SCNu64
#error "inttypes.h:SCNu64 macro is missing from libc-shim"
#endif

#ifndef SCNu8
#error "inttypes.h:SCNu8 macro is missing from libc-shim"
#endif

#ifndef SCNuFAST16
#error "inttypes.h:SCNuFAST16 macro is missing from libc-shim"
#endif

#ifndef SCNuFAST32
#error "inttypes.h:SCNuFAST32 macro is missing from libc-shim"
#endif

#ifndef SCNuFAST64
#error "inttypes.h:SCNuFAST64 macro is missing from libc-shim"
#endif

#ifndef SCNuFAST8
#error "inttypes.h:SCNuFAST8 macro is missing from libc-shim"
#endif

#ifndef SCNuLEAST16
#error "inttypes.h:SCNuLEAST16 macro is missing from libc-shim"
#endif

#ifndef SCNuLEAST32
#error "inttypes.h:SCNuLEAST32 macro is missing from libc-shim"
#endif

#ifndef SCNuLEAST64
#error "inttypes.h:SCNuLEAST64 macro is missing from libc-shim"
#endif

#ifndef SCNuLEAST8
#error "inttypes.h:SCNuLEAST8 macro is missing from libc-shim"
#endif

#ifndef SCNuMAX
#error "inttypes.h:SCNuMAX macro is missing from libc-shim"
#endif

#ifndef SCNuPTR
#error "inttypes.h:SCNuPTR macro is missing from libc-shim"
#endif

#ifndef SCNx16
#error "inttypes.h:SCNx16 macro is missing from libc-shim"
#endif

#ifndef SCNx32
#error "inttypes.h:SCNx32 macro is missing from libc-shim"
#endif

#ifndef SCNx64
#error "inttypes.h:SCNx64 macro is missing from libc-shim"
#endif

#ifndef SCNx8
#error "inttypes.h:SCNx8 macro is missing from libc-shim"
#endif

#ifndef SCNxFAST16
#error "inttypes.h:SCNxFAST16 macro is missing from libc-shim"
#endif

#ifndef SCNxFAST32
#error "inttypes.h:SCNxFAST32 macro is missing from libc-shim"
#endif

#ifndef SCNxFAST64
#error "inttypes.h:SCNxFAST64 macro is missing from libc-shim"
#endif

#ifndef SCNxFAST8
#error "inttypes.h:SCNxFAST8 macro is missing from libc-shim"
#endif

#ifndef SCNxLEAST16
#error "inttypes.h:SCNxLEAST16 macro is missing from libc-shim"
#endif

#ifndef SCNxLEAST32
#error "inttypes.h:SCNxLEAST32 macro is missing from libc-shim"
#endif

#ifndef SCNxLEAST64
#error "inttypes.h:SCNxLEAST64 macro is missing from libc-shim"
#endif

#ifndef SCNxLEAST8
#error "inttypes.h:SCNxLEAST8 macro is missing from libc-shim"
#endif

#ifndef SCNxMAX
#error "inttypes.h:SCNxMAX macro is missing from libc-shim"
#endif

#ifndef SCNxPTR
#error "inttypes.h:SCNxPTR macro is missing from libc-shim"
#endif

#ifndef SIG_ATOMIC_MAX
#error "inttypes.h:SIG_ATOMIC_MAX macro is missing from libc-shim"
#endif

#ifndef SIG_ATOMIC_MIN
#error "inttypes.h:SIG_ATOMIC_MIN macro is missing from libc-shim"
#endif

#ifndef SIG_ATOMIC_WIDTH
#error "inttypes.h:SIG_ATOMIC_WIDTH macro is missing from libc-shim"
#endif

#ifndef SIZE_MAX
#error "inttypes.h:SIZE_MAX macro is missing from libc-shim"
#endif

#ifndef SIZE_WIDTH
#error "inttypes.h:SIZE_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINT16_C
#error "inttypes.h:UINT16_C macro is missing from libc-shim"
#endif

#ifndef UINT16_MAX
#error "inttypes.h:UINT16_MAX macro is missing from libc-shim"
#endif

#ifndef UINT16_WIDTH
#error "inttypes.h:UINT16_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINT32_C
#error "inttypes.h:UINT32_C macro is missing from libc-shim"
#endif

#ifndef UINT32_MAX
#error "inttypes.h:UINT32_MAX macro is missing from libc-shim"
#endif

#ifndef UINT32_WIDTH
#error "inttypes.h:UINT32_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINT64_C
#error "inttypes.h:UINT64_C macro is missing from libc-shim"
#endif

#ifndef UINT64_MAX
#error "inttypes.h:UINT64_MAX macro is missing from libc-shim"
#endif

#ifndef UINT64_WIDTH
#error "inttypes.h:UINT64_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINT8_C
#error "inttypes.h:UINT8_C macro is missing from libc-shim"
#endif

#ifndef UINT8_MAX
#error "inttypes.h:UINT8_MAX macro is missing from libc-shim"
#endif

#ifndef UINT8_WIDTH
#error "inttypes.h:UINT8_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINTMAX_C
#error "inttypes.h:UINTMAX_C macro is missing from libc-shim"
#endif

#ifndef UINTMAX_MAX
#error "inttypes.h:UINTMAX_MAX macro is missing from libc-shim"
#endif

#ifndef UINTMAX_WIDTH
#error "inttypes.h:UINTMAX_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINTPTR_MAX
#error "inttypes.h:UINTPTR_MAX macro is missing from libc-shim"
#endif

#ifndef UINTPTR_WIDTH
#error "inttypes.h:UINTPTR_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINT_FAST16_MAX
#error "inttypes.h:UINT_FAST16_MAX macro is missing from libc-shim"
#endif

#ifndef UINT_FAST16_WIDTH
#error "inttypes.h:UINT_FAST16_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINT_FAST32_MAX
#error "inttypes.h:UINT_FAST32_MAX macro is missing from libc-shim"
#endif

#ifndef UINT_FAST32_WIDTH
#error "inttypes.h:UINT_FAST32_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINT_FAST64_MAX
#error "inttypes.h:UINT_FAST64_MAX macro is missing from libc-shim"
#endif

#ifndef UINT_FAST64_WIDTH
#error "inttypes.h:UINT_FAST64_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINT_FAST8_MAX
#error "inttypes.h:UINT_FAST8_MAX macro is missing from libc-shim"
#endif

#ifndef UINT_FAST8_WIDTH
#error "inttypes.h:UINT_FAST8_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINT_LEAST16_MAX
#error "inttypes.h:UINT_LEAST16_MAX macro is missing from libc-shim"
#endif

#ifndef UINT_LEAST16_WIDTH
#error "inttypes.h:UINT_LEAST16_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINT_LEAST32_MAX
#error "inttypes.h:UINT_LEAST32_MAX macro is missing from libc-shim"
#endif

#ifndef UINT_LEAST32_WIDTH
#error "inttypes.h:UINT_LEAST32_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINT_LEAST64_MAX
#error "inttypes.h:UINT_LEAST64_MAX macro is missing from libc-shim"
#endif

#ifndef UINT_LEAST64_WIDTH
#error "inttypes.h:UINT_LEAST64_WIDTH macro is missing from libc-shim"
#endif

#ifndef UINT_LEAST8_MAX
#error "inttypes.h:UINT_LEAST8_MAX macro is missing from libc-shim"
#endif

#ifndef UINT_LEAST8_WIDTH
#error "inttypes.h:UINT_LEAST8_WIDTH macro is missing from libc-shim"
#endif

#ifndef WCHAR_MAX
#error "inttypes.h:WCHAR_MAX macro is missing from libc-shim"
#endif

#ifndef WCHAR_MIN
#error "inttypes.h:WCHAR_MIN macro is missing from libc-shim"
#endif

#ifndef WCHAR_WIDTH
#error "inttypes.h:WCHAR_WIDTH macro is missing from libc-shim"
#endif

#ifndef WINT_MAX
#error "inttypes.h:WINT_MAX macro is missing from libc-shim"
#endif

#ifndef WINT_MIN
#error "inttypes.h:WINT_MIN macro is missing from libc-shim"
#endif

#ifndef WINT_WIDTH
#error "inttypes.h:WINT_WIDTH macro is missing from libc-shim"
#endif

#ifndef _ATFILE_SOURCE
#error "inttypes.h:_ATFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _BITS_STDINT_INTN_H
#error "inttypes.h:_BITS_STDINT_INTN_H macro is missing from libc-shim"
#endif

#ifndef _BITS_STDINT_LEAST_H
#error "inttypes.h:_BITS_STDINT_LEAST_H macro is missing from libc-shim"
#endif

#ifndef _BITS_STDINT_UINTN_H
#error "inttypes.h:_BITS_STDINT_UINTN_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TIME64_H
#error "inttypes.h:_BITS_TIME64_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPESIZES_H
#error "inttypes.h:_BITS_TYPESIZES_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPES_H
#error "inttypes.h:_BITS_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _BITS_WCHAR_H
#error "inttypes.h:_BITS_WCHAR_H macro is missing from libc-shim"
#endif

#ifndef _DEFAULT_SOURCE
#error "inttypes.h:_DEFAULT_SOURCE macro is missing from libc-shim"
#endif

#ifndef _DYNAMIC_STACK_SIZE_SOURCE
#error "inttypes.h:_DYNAMIC_STACK_SIZE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _FEATURES_H
#error "inttypes.h:_FEATURES_H macro is missing from libc-shim"
#endif

#ifndef _INTTYPES_H
#error "inttypes.h:_INTTYPES_H macro is missing from libc-shim"
#endif

#ifndef _ISOC11_SOURCE
#error "inttypes.h:_ISOC11_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC23_SOURCE
#error "inttypes.h:_ISOC23_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC2Y_SOURCE
#error "inttypes.h:_ISOC2Y_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC95_SOURCE
#error "inttypes.h:_ISOC95_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC99_SOURCE
#error "inttypes.h:_ISOC99_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE64_SOURCE
#error "inttypes.h:_LARGEFILE64_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE_SOURCE
#error "inttypes.h:_LARGEFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_C_SOURCE
#error "inttypes.h:_POSIX_C_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_SOURCE
#error "inttypes.h:_POSIX_SOURCE macro is missing from libc-shim"
#endif

#ifndef _STDC_PREDEF_H
#error "inttypes.h:_STDC_PREDEF_H macro is missing from libc-shim"
#endif

#ifndef _STDINT_H
#error "inttypes.h:_STDINT_H macro is missing from libc-shim"
#endif

#ifndef _SYS_CDEFS_H
#error "inttypes.h:_SYS_CDEFS_H macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE
#error "inttypes.h:_XOPEN_SOURCE macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE_EXTENDED
#error "inttypes.h:_XOPEN_SOURCE_EXTENDED macro is missing from libc-shim"
#endif

int main(void) { return 0; }
