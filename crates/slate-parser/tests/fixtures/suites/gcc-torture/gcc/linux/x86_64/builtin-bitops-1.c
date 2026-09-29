#include <assert.h>
#include <limits.h>

#if __INT_MAX__ > 2147483647L
#if __INT_MAX__ >= 9223372036854775807L
#define BITSIZEOF_INT 64
#else
#define BITSIZEOF_INT 32
#endif
#else
#if __INT_MAX__ >= 2147483647L
#define BITSIZEOF_INT 32
#else
#define BITSIZEOF_INT 16
#endif
#endif

#if __LONG_MAX__ > 2147483647L
#if __LONG_MAX__ >= 9223372036854775807L
#define BITSIZEOF_LONG 64
#else
#define BITSIZEOF_LONG 32
#endif
#else
#define BITSIZEOF_LONG 32
#endif

#if __LONG_LONG_MAX__ > 2147483647L
#if __LONG_LONG_MAX__ >= 9223372036854775807L
#define BITSIZEOF_LONG_LONG 64
#else
#define BITSIZEOF_LONG_LONG 32
#endif
#else
#define BITSIZEOF_LONG_LONG 32
#endif

#define MAKE_FUNS(suffix, type)                                                \
  int my_ffs##suffix(type x) {                                                 \
    int i;                                                                     \
    if (x == 0)                                                                \
      return 0;                                                                \
    for (i = 0; i < CHAR_BIT * sizeof(type); i++)                              \
      if (x & ((type)1 << i))                                                  \
        break;                                                                 \
    return i + 1;                                                              \
  }                                                                            \
                                                                               \
  int my_ctz##suffix(type x) {                                                 \
    int i;                                                                     \
    for (i = 0; i < CHAR_BIT * sizeof(type); i++)                              \
      if (x & ((type)1 << i))                                                  \
        break;                                                                 \
    return i;                                                                  \
  }                                                                            \
                                                                               \
  int my_clz##suffix(type x) {                                                 \
    int i;                                                                     \
    for (i = 0; i < CHAR_BIT * sizeof(type); i++)                              \
      if (x & ((type)1 << ((CHAR_BIT * sizeof(type)) - i - 1)))                \
        break;                                                                 \
    return i;                                                                  \
  }                                                                            \
                                                                               \
  int my_clrsb##suffix(type x) {                                               \
    int i;                                                                     \
    int leading = (x >> CHAR_BIT * sizeof(type) - 1) & 1;                      \
    for (i = 1; i < CHAR_BIT * sizeof(type); i++)                              \
      if (((x >> ((CHAR_BIT * sizeof(type)) - i - 1)) & 1) != leading)         \
        break;                                                                 \
    return i - 1;                                                              \
  }                                                                            \
                                                                               \
  int my_popcount##suffix(type x) {                                            \
    int i;                                                                     \
    int count = 0;                                                             \
    for (i = 0; i < CHAR_BIT * sizeof(type); i++)                              \
      if (x & ((type)1 << i))                                                  \
        count++;                                                               \
    return count;                                                              \
  }                                                                            \
                                                                               \
  int my_parity##suffix(type x) {                                              \
    int i;                                                                     \
    int count = 0;                                                             \
    for (i = 0; i < CHAR_BIT * sizeof(type); i++)                              \
      if (x & ((type)1 << i))                                                  \
        count++;                                                               \
    return count & 1;                                                          \
  }

MAKE_FUNS(, unsigned);
MAKE_FUNS(l, unsigned long);
MAKE_FUNS(ll, unsigned long long);

extern void abort(void);
extern void exit(int);

#define NUMS16                                                                 \
  {0x0000U, 0x0001U, 0x8000U, 0x0002U, 0x4000U, 0x0100U,                       \
   0x0080U, 0xa5a5U, 0x5a5aU, 0xcafeU, 0xffffU}

#define NUMS32                                                                 \
  {0x00000000UL, 0x00000001UL, 0x80000000UL, 0x00000002UL, 0x40000000UL,       \
   0x00010000UL, 0x00008000UL, 0xa5a5a5a5UL, 0x5a5a5a5aUL, 0xcafe0000UL,       \
   0x00cafe00UL, 0x0000cafeUL, 0xffffffffUL}

#define NUMS64                                                                 \
  {0x0000000000000000ULL, 0x0000000000000001ULL, 0x8000000000000000ULL,        \
   0x0000000000000002ULL, 0x4000000000000000ULL, 0x0000000100000000ULL,        \
   0x0000000080000000ULL, 0xa5a5a5a5a5a5a5a5ULL, 0x5a5a5a5a5a5a5a5aULL,        \
   0xcafecafe00000000ULL, 0x0000cafecafe0000ULL, 0x00000000cafecafeULL,        \
   0xffffffffffffffffULL}

unsigned int ints[] =
#if BITSIZEOF_INT == 64
    NUMS64;
#elif BITSIZEOF_INT == 32
    NUMS32;
#else
    NUMS16;
#endif

unsigned long longs[] =
#if BITSIZEOF_LONG == 64
    NUMS64;
#else
    NUMS32;
#endif

unsigned long long longlongs[] =
#if BITSIZEOF_LONG_LONG == 64
    NUMS64;
#else
    NUMS32;
#endif

#define N(table) (sizeof(table) / sizeof(table[0]))

int main(void) {
  int i;

  for (i = 0; i < N(ints); i++) {
    if (__builtin_ffs(ints[i]) != my_ffs(ints[i]))
      abort();
    if (ints[i] != 0 && __builtin_clz(ints[i]) != my_clz(ints[i]))
      abort();
    if (ints[i] != 0 && __builtin_ctz(ints[i]) != my_ctz(ints[i]))
      abort();
    if (__builtin_clrsb(ints[i]) != my_clrsb(ints[i]))
      abort();
    if (__builtin_popcount(ints[i]) != my_popcount(ints[i]))
      abort();
    if (__builtin_parity(ints[i]) != my_parity(ints[i]))
      abort();
  }

  for (i = 0; i < N(longs); i++) {
    if (__builtin_ffsl(longs[i]) != my_ffsl(longs[i]))
      abort();
    if (longs[i] != 0 && __builtin_clzl(longs[i]) != my_clzl(longs[i]))
      abort();
    if (longs[i] != 0 && __builtin_ctzl(longs[i]) != my_ctzl(longs[i]))
      abort();
    if (__builtin_clrsbl(longs[i]) != my_clrsbl(longs[i]))
      abort();
    if (__builtin_popcountl(longs[i]) != my_popcountl(longs[i]))
      abort();
    if (__builtin_parityl(longs[i]) != my_parityl(longs[i]))
      abort();
  }

  for (i = 0; i < N(longlongs); i++) {
    if (__builtin_ffsll(longlongs[i]) != my_ffsll(longlongs[i]))
      abort();
    if (longlongs[i] != 0 &&
        __builtin_clzll(longlongs[i]) != my_clzll(longlongs[i]))
      abort();
    if (longlongs[i] != 0 &&
        __builtin_ctzll(longlongs[i]) != my_ctzll(longlongs[i]))
      abort();
    if (__builtin_clrsbll(longlongs[i]) != my_clrsbll(longlongs[i]))
      abort();
    if (__builtin_popcountll(longlongs[i]) != my_popcountll(longlongs[i]))
      abort();
    if (__builtin_parityll(longlongs[i]) != my_parityll(longlongs[i]))
      abort();
  }

  /* Test constant folding.  */

#define TEST(x, suffix)                                                        \
  if (__builtin_ffs##suffix(x) != my_ffs##suffix(x))                           \
    abort();                                                                   \
  if (x != 0 && __builtin_clz##suffix(x) != my_clz##suffix(x))                 \
    abort();                                                                   \
  if (x != 0 && __builtin_ctz##suffix(x) != my_ctz##suffix(x))                 \
    abort();                                                                   \
  if (__builtin_clrsb##suffix(x) != my_clrsb##suffix(x))                       \
    abort();                                                                   \
  if (__builtin_popcount##suffix(x) != my_popcount##suffix(x))                 \
    abort();                                                                   \
  if (__builtin_parity##suffix(x) != my_parity##suffix(x))                     \
    abort();

#if BITSIZEOF_INT == 32
  TEST(0x00000000UL, );
  TEST(0x00000001UL, );
  TEST(0x80000000UL, );
  TEST(0x40000000UL, );
  TEST(0x00010000UL, );
  TEST(0x00008000UL, );
  TEST(0xa5a5a5a5UL, );
  TEST(0x5a5a5a5aUL, );
  TEST(0xcafe0000UL, );
  TEST(0x00cafe00UL, );
  TEST(0x0000cafeUL, );
  TEST(0xffffffffUL, );
#endif

#if BITSIZEOF_LONG_LONG == 64
  TEST(0x0000000000000000ULL, ll);
  TEST(0x0000000000000001ULL, ll);
  TEST(0x8000000000000000ULL, ll);
  TEST(0x0000000000000002ULL, ll);
  TEST(0x4000000000000000ULL, ll);
  TEST(0x0000000100000000ULL, ll);
  TEST(0x0000000080000000ULL, ll);
  TEST(0xa5a5a5a5a5a5a5a5ULL, ll);
  TEST(0x5a5a5a5a5a5a5a5aULL, ll);
  TEST(0xcafecafe00000000ULL, ll);
  TEST(0x0000cafecafe0000ULL, ll);
  TEST(0x00000000cafecafeULL, ll);
  TEST(0xffffffffffffffffULL, ll);
#endif

  exit(0);
}



// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_ints:[0-9]+]] ints: array<u32, 13> [storage=static] [align=16] = aggregate<array<u32, 13>, zero_fill=false>(index0 = truncate<u32, reason=assign, fits=always>(const<u64>(0)), index1 = truncate<u32, reason=assign, fits=always>(const<u64>(1)), index2 = truncate<u32, reason=assign, fits=always>(const<u64>(2147483648)), index3 = truncate<u32, reason=assign, fits=always>(const<u64>(2)), index4 = truncate<u32, reason=assign, fits=always>(const<u64>(1073741824)), index5 = truncate<u32, reason=assign, fits=always>(const<u64>(65536)), index6 = truncate<u32, reason=assign, fits=always>(const<u64>(32768)), index7 = truncate<u32, reason=assign, fits=always>(const<u64>(2779096485)), index8 = truncate<u32, reason=assign, fits=always>(const<u64>(1515870810)), index9 = truncate<u32, reason=assign, fits=always>(const<u64>(3405643776)), index10 = truncate<u32, reason=assign, fits=always>(const<u64>(13303296)), index11 = truncate<u32, reason=assign, fits=always>(const<u64>(51966)), index12 = truncate<u32, reason=assign, fits=always>(const<u64>(4294967295))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_longs:[0-9]+]] longs: array<u64, 13> [storage=static] [align=16] = aggregate<array<u64, 13>, zero_fill=false>(index0 = const<u64>(0), index1 = const<u64>(1), index2 = const<u64>(9223372036854775808), index3 = const<u64>(2), index4 = const<u64>(4611686018427387904), index5 = const<u64>(4294967296), index6 = const<u64>(2147483648), index7 = const<u64>(11936128518282651045), index8 = const<u64>(6510615555426900570), index9 = const<u64>(14627351832016453632), index10 = const<u64>(223195676147712), index11 = const<u64>(3405695742), index12 = const<u64>(18446744073709551615)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_longlongs:[0-9]+]] longlongs: array<u64, 13> [storage=static] [align=16] = aggregate<array<u64, 13>, zero_fill=false>(index0 = const<u64>(0), index1 = const<u64>(1), index2 = const<u64>(9223372036854775808), index3 = const<u64>(2), index4 = const<u64>(4611686018427387904), index5 = const<u64>(4294967296), index6 = const<u64>(2147483648), index7 = const<u64>(11936128518282651045), index8 = const<u64>(6510615555426900570), index9 = const<u64>(14627351832016453632), index10 = const<u64>(223195676147712), index11 = const<u64>(3405695742), index12 = const<u64>(18446744073709551615)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_my_ffs:[0-9]+]] @my_ffs(%[[VALUE_x:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%[[VALUE_x]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(read<u32>(%[[VALUE_x]]), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), read<i32>(%[[VALUE_i]]))), const<u32>(0))
// DEFAULT-NEXT:                     break %[[VALUE0]];
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_ctz:[0-9]+]] @my_ctz(%[[VALUE_x_2:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_2]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(read<u32>(%[[VALUE_x_2]]), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), read<i32>(%[[VALUE_i_2]]))), const<u32>(0))
// DEFAULT-NEXT:                     break %[[VALUE3]];
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_clz:[0-9]+]] @my_clz(%[[VALUE_x_3:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_3]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(read<u32>(%[[VALUE_x_3]]), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_3]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), const<u32>(0))
// DEFAULT-NEXT:                     break %[[VALUE6]];
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_clrsb:[0-9]+]] @my_clrsb(%[[VALUE_x_4:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_4:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_leading:[0-9]+]] leading: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_x_4]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_4]], const<i32>(1));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_4]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_x_4]]), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_4]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_leading]])))
// DEFAULT-NEXT:                     break %[[VALUE9]];
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%[[VALUE_i_4]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_popcount:[0-9]+]] @my_popcount(%[[VALUE_x_5:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_5:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_count:[0-9]+]] count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_5]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_5]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_5]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_5]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(read<u32>(%[[VALUE_x_5]]), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), read<i32>(%[[VALUE_i_5]]))), const<u32>(0))
// DEFAULT-NEXT:                     let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:                     let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_parity:[0-9]+]] @my_parity(%[[VALUE_x_6:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_6:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_count_2:[0-9]+]] count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_6]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_6]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_6]]);
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_6]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(read<u32>(%[[VALUE_x_6]]), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), read<i32>(%[[VALUE_i_6]]))), const<u32>(0))
// DEFAULT-NEXT:                     let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count_2]]);
// DEFAULT-NEXT:                     let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_count_2]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:         return and<i32>(read<i32>(%[[VALUE_count_2]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_ffsl:[0-9]+]] @my_ffsl(%[[VALUE_x_7:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_7:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE_x_7]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_7]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_7]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_7]]);
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_7]], read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%[[VALUE_x_7]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%[[VALUE_i_7]]))), const<u64>(0))
// DEFAULT-NEXT:                     break %[[VALUE22]];
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_i_7]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_ctzl:[0-9]+]] @my_ctzl(%[[VALUE_x_8:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_8:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_8]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_8]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_8]]);
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_8]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%[[VALUE_x_8]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%[[VALUE_i_8]]))), const<u64>(0))
// DEFAULT-NEXT:                     break %[[VALUE25]];
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i_8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_clzl:[0-9]+]] @my_clzl(%[[VALUE_x_9:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_9:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_9]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_9]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_9]]);
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE29]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_9]], read<i32>(%[[VALUE30]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%[[VALUE_x_9]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_9]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), const<u64>(0))
// DEFAULT-NEXT:                     break %[[VALUE28]];
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i_9]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_clrsbl:[0-9]+]] @my_clrsbl(%[[VALUE_x_10:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_10:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_leading_2:[0-9]+]] leading: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_x_10]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         for %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_10]], const<i32>(1));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_10]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE32:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_10]]);
// DEFAULT-NEXT:                 let %[[VALUE33:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE32]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_10]], read<i32>(%[[VALUE33]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_x_10]]), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_10]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_leading_2]]))))
// DEFAULT-NEXT:                     break %[[VALUE31]];
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%[[VALUE_i_10]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_popcountl:[0-9]+]] @my_popcountl(%[[VALUE_x_11:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_11:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_count_3:[0-9]+]] count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_11]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_11]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE35:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_11]]);
// DEFAULT-NEXT:                 let %[[VALUE36:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE35]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_11]], read<i32>(%[[VALUE36]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%[[VALUE_x_11]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%[[VALUE_i_11]]))), const<u64>(0))
// DEFAULT-NEXT:                     let %[[VALUE37:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count_3]]);
// DEFAULT-NEXT:                     let %[[VALUE38:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE37]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_count_3]], read<i32>(%[[VALUE38]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_count_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_parityl:[0-9]+]] @my_parityl(%[[VALUE_x_12:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_12:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_count_4:[0-9]+]] count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE39:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_12]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_12]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE40:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_12]]);
// DEFAULT-NEXT:                 let %[[VALUE41:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE40]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_12]], read<i32>(%[[VALUE41]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%[[VALUE_x_12]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%[[VALUE_i_12]]))), const<u64>(0))
// DEFAULT-NEXT:                     let %[[VALUE42:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count_4]]);
// DEFAULT-NEXT:                     let %[[VALUE43:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE42]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_count_4]], read<i32>(%[[VALUE43]]));
// DEFAULT-NEXT:         return and<i32>(read<i32>(%[[VALUE_count_4]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_ffsll:[0-9]+]] @my_ffsll(%[[VALUE_x_13:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_13:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE_x_13]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE44:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_13]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_13]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE45:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_13]]);
// DEFAULT-NEXT:                 let %[[VALUE46:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE45]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_13]], read<i32>(%[[VALUE46]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%[[VALUE_x_13]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%[[VALUE_i_13]]))), const<u64>(0))
// DEFAULT-NEXT:                     break %[[VALUE44]];
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_i_13]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_ctzll:[0-9]+]] @my_ctzll(%[[VALUE_x_14:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_14:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE47:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_14]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_14]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE48:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_14]]);
// DEFAULT-NEXT:                 let %[[VALUE49:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE48]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_14]], read<i32>(%[[VALUE49]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%[[VALUE_x_14]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%[[VALUE_i_14]]))), const<u64>(0))
// DEFAULT-NEXT:                     break %[[VALUE47]];
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i_14]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_clzll:[0-9]+]] @my_clzll(%[[VALUE_x_15:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_15:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE50:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_15]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_15]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE51:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_15]]);
// DEFAULT-NEXT:                 let %[[VALUE52:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE51]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_15]], read<i32>(%[[VALUE52]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%[[VALUE_x_15]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_15]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), const<u64>(0))
// DEFAULT-NEXT:                     break %[[VALUE50]];
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i_15]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_clrsbll:[0-9]+]] @my_clrsbll(%[[VALUE_x_16:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_16:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_leading_3:[0-9]+]] leading: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_x_16]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         for %[[VALUE53:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_16]], const<i32>(1));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_16]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE54:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_16]]);
// DEFAULT-NEXT:                 let %[[VALUE55:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE54]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_16]], read<i32>(%[[VALUE55]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_x_16]]), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_16]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_leading_3]]))))
// DEFAULT-NEXT:                     break %[[VALUE53]];
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%[[VALUE_i_16]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_popcountll:[0-9]+]] @my_popcountll(%[[VALUE_x_17:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_17:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_count_5:[0-9]+]] count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE56:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_17]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_17]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE57:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_17]]);
// DEFAULT-NEXT:                 let %[[VALUE58:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE57]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_17]], read<i32>(%[[VALUE58]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%[[VALUE_x_17]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%[[VALUE_i_17]]))), const<u64>(0))
// DEFAULT-NEXT:                     let %[[VALUE59:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count_5]]);
// DEFAULT-NEXT:                     let %[[VALUE60:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE59]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_count_5]], read<i32>(%[[VALUE60]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_count_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_parityll:[0-9]+]] @my_parityll(%[[VALUE_x_18:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_18:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_count_6:[0-9]+]] count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE61:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_18]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_18]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE62:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_18]]);
// DEFAULT-NEXT:                 let %[[VALUE63:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE62]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_18]], read<i32>(%[[VALUE63]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%[[VALUE_x_18]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%[[VALUE_i_18]]))), const<u64>(0))
// DEFAULT-NEXT:                     let %[[VALUE64:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count_6]]);
// DEFAULT-NEXT:                     let %[[VALUE65:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE64]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_count_6]], read<i32>(%[[VALUE65]]));
// DEFAULT-NEXT:         return and<i32>(read<i32>(%[[VALUE_count_6]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE66:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ffs:[0-9]+]] @__builtin_ffs(%[[VALUE67:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clz:[0-9]+]] @__builtin_clz(%[[VALUE68:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctz:[0-9]+]] @__builtin_ctz(%[[VALUE69:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clrsb:[0-9]+]] @__builtin_clrsb(%[[VALUE70:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcount:[0-9]+]] @__builtin_popcount(%[[VALUE71:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_parity:[0-9]+]] @__builtin_parity(%[[VALUE72:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ffsl:[0-9]+]] @__builtin_ffsl(%[[VALUE73:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clzl:[0-9]+]] @__builtin_clzl(%[[VALUE74:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctzl:[0-9]+]] @__builtin_ctzl(%[[VALUE75:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clrsbl:[0-9]+]] @__builtin_clrsbl(%[[VALUE76:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcountl:[0-9]+]] @__builtin_popcountl(%[[VALUE77:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_parityl:[0-9]+]] @__builtin_parityl(%[[VALUE78:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ffsll:[0-9]+]] @__builtin_ffsll(%[[VALUE79:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clzll:[0-9]+]] @__builtin_clzll(%[[VALUE80:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctzll:[0-9]+]] @__builtin_ctzll(%[[VALUE81:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clrsbll:[0-9]+]] @__builtin_clrsbll(%[[VALUE82:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcountll:[0-9]+]] @__builtin_popcountll(%[[VALUE83:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_parityll:[0-9]+]] @__builtin_parityll(%[[VALUE84:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i_19:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE85:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_19]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_19]]))), div<u64, by_zero=ub>(const<u64>(52), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE86:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_19]]);
// DEFAULT-NEXT:                 let %[[VALUE87:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE86]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_19]], read<i32>(%[[VALUE87]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]])))))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE88:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                     if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                         write<bool>(%[[VALUE88]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]]))))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]])))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%[[VALUE88]], const<bool>(false));
// DEFAULT-NEXT:                     if read<bool>(%[[VALUE88]])
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE89:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                     if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                         write<bool>(%[[VALUE89]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]]))))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]])))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%[[VALUE89]], const<bool>(false));
// DEFAULT-NEXT:                     if read<bool>(%[[VALUE89]])
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]])))))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]]))))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]]))))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%[[VALUE_ints]]), read<i32>(%[[VALUE_i_19]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE90:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_19]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_19]]))), div<u64, by_zero=ub>(const<u64>(104), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE91:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_19]]);
// DEFAULT-NEXT:                 let %[[VALUE92:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE91]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_19]], read<i32>(%[[VALUE92]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsl]], reinterpret<i64, reason=arg, fits=unknown>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]])))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsl]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE93:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                         write<bool>(%[[VALUE93]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzl]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]]))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzl]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]])))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%[[VALUE93]], const<bool>(false));
// DEFAULT-NEXT:                     if read<bool>(%[[VALUE93]])
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE94:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                         write<bool>(%[[VALUE94]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzl]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]]))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzl]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]])))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%[[VALUE94]], const<bool>(false));
// DEFAULT-NEXT:                     if read<bool>(%[[VALUE94]])
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbl]], reinterpret<i64, reason=arg, fits=unknown>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]])))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbl]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountl]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]]))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountl]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityl]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]]))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityl]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longs]]), read<i32>(%[[VALUE_i_19]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE95:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_19]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_19]]))), div<u64, by_zero=ub>(const<u64>(104), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE96:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_19]]);
// DEFAULT-NEXT:                 let %[[VALUE97:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE96]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_19]], read<i32>(%[[VALUE97]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=unknown>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]])))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE98:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                         write<bool>(%[[VALUE98]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]]))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]])))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%[[VALUE98]], const<bool>(false));
// DEFAULT-NEXT:                     if read<bool>(%[[VALUE98]])
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE99:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                         write<bool>(%[[VALUE99]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]]))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]])))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%[[VALUE99]], const<bool>(false));
// DEFAULT-NEXT:                     if read<bool>(%[[VALUE99]])
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=unknown>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]])))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]]))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]]))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%[[VALUE_longlongs]]), read<i32>(%[[VALUE_i_19]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(0)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], truncate<u32, reason=arg, fits=always>(const<u64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE100:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE100]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(0))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(0)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE100]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE100]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE101]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(0))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(0)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE101]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE101]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(0)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], truncate<u32, reason=arg, fits=always>(const<u64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(0))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(0))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], truncate<u32, reason=arg, fits=always>(const<u64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE102:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE102]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(1))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(1)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE102]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE102]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE103:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE103]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(1))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(1)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE103]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE103]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], truncate<u32, reason=arg, fits=always>(const<u64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(1))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(1))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(2147483648)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE104:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2147483648), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE104]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(2147483648)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE104]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE104]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE105:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2147483648), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE105]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(2147483648)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE105]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE105]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(2147483648)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1073741824)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE106:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1073741824), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE106]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(1073741824)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE106]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE106]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE107:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1073741824), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE107]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(1073741824)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE107]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE107]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1073741824)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(65536)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], truncate<u32, reason=arg, fits=always>(const<u64>(65536))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE108:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(65536), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE108]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(65536))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(65536)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE108]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE108]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE109:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(65536), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE109]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(65536))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(65536)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE109]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE109]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(65536)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], truncate<u32, reason=arg, fits=always>(const<u64>(65536))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(65536))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(65536))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(65536))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(65536))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(32768)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], truncate<u32, reason=arg, fits=always>(const<u64>(32768))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE110:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(32768), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE110]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(32768))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(32768)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE110]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE110]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE111:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(32768), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE111]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(32768))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(32768)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE111]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE111]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(32768)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], truncate<u32, reason=arg, fits=always>(const<u64>(32768))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(32768))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(32768))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(32768))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(32768))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(2779096485)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE112:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2779096485), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE112]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(2779096485)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE112]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE112]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE113:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2779096485), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE113]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(2779096485)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE113]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE113]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(2779096485)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1515870810)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE114:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1515870810), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE114]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(1515870810)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE114]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE114]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE115:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1515870810), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE115]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(1515870810)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE115]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE115]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1515870810)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(3405643776)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE116:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(3405643776), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE116]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(3405643776)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE116]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE116]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE117:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(3405643776), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE117]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(3405643776)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE117]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE117]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(3405643776)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(13303296)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], truncate<u32, reason=arg, fits=always>(const<u64>(13303296))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE118:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(13303296), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE118]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(13303296))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(13303296)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE118]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE118]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE119:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(13303296), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE119]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(13303296))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(13303296)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE119]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE119]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(13303296)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], truncate<u32, reason=arg, fits=always>(const<u64>(13303296))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(13303296))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(13303296))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(13303296))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(13303296))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(51966)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], truncate<u32, reason=arg, fits=always>(const<u64>(51966))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE120:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(51966), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE120]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(51966))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(51966)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE120]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE120]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE121:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(51966), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE121]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(51966))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(51966)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE121]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE121]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(51966)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], truncate<u32, reason=arg, fits=always>(const<u64>(51966))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(51966))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(51966))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(51966))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(51966))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(4294967295)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ffs]], truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE122:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4294967295), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE122]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clz]], truncate<u32, reason=arg, fits=always>(const<u64>(4294967295)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE122]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE122]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE123:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4294967295), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE123]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_ctz]], truncate<u32, reason=arg, fits=always>(const<u64>(4294967295)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE123]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE123]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(4294967295)))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_clrsb]], truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_popcount]], truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_my_parity]], truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(0))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE124:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE124]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(0)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(0))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE124]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE124]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE125:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE125]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(0)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(0))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE125]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE125]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(0))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(0)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(0)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(1))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE126:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE126]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(1)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE126]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE126]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE127:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE127]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(1)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE127]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE127]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(1))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(1)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(1)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(9223372036854775808))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(9223372036854775808)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE128:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(9223372036854775808), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE128]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(9223372036854775808)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(9223372036854775808))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE128]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE128]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE129:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(9223372036854775808), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE129]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(9223372036854775808)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(9223372036854775808))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE129]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE129]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(9223372036854775808))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(9223372036854775808)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(9223372036854775808)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(9223372036854775808)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(9223372036854775808)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(9223372036854775808)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(2))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE130:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE130]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(2)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(2))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE130]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE130]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE131:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE131]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(2)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(2))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE131]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE131]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(2))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(2)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(2)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(4611686018427387904))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(4611686018427387904)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE132:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4611686018427387904), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE132]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(4611686018427387904)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(4611686018427387904))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE132]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE132]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE133:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4611686018427387904), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE133]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(4611686018427387904)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(4611686018427387904))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE133]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE133]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(4611686018427387904))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(4611686018427387904)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(4611686018427387904)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(4611686018427387904)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(4611686018427387904)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(4611686018427387904)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(4294967296))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(4294967296)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE134:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4294967296), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE134]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(4294967296)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(4294967296))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE134]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE134]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE135:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4294967296), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE135]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(4294967296)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(4294967296))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE135]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE135]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(4294967296))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(4294967296)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(4294967296)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(4294967296)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(4294967296)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(4294967296)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(2147483648))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(2147483648)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE136:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2147483648), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE136]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(2147483648)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(2147483648))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE136]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE136]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE137:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2147483648), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE137]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(2147483648)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(2147483648))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE137]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE137]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(2147483648))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(2147483648)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(2147483648)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(2147483648)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(2147483648)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(2147483648)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(11936128518282651045))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(11936128518282651045)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE138:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(11936128518282651045), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE138]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(11936128518282651045)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(11936128518282651045))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE138]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE138]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE139:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(11936128518282651045), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE139]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(11936128518282651045)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(11936128518282651045))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE139]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE139]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(11936128518282651045))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(11936128518282651045)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(11936128518282651045)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(11936128518282651045)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(11936128518282651045)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(11936128518282651045)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(6510615555426900570))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(6510615555426900570)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE140:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(6510615555426900570), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE140]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(6510615555426900570)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(6510615555426900570))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE140]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE140]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE141:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(6510615555426900570), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE141]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(6510615555426900570)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(6510615555426900570))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE141]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE141]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(6510615555426900570))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(6510615555426900570)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(6510615555426900570)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(6510615555426900570)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(6510615555426900570)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(6510615555426900570)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(14627351832016453632))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(14627351832016453632)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE142:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(14627351832016453632), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE142]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(14627351832016453632)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(14627351832016453632))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE142]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE142]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE143:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(14627351832016453632), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE143]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(14627351832016453632)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(14627351832016453632))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE143]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE143]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(14627351832016453632))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(14627351832016453632)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(14627351832016453632)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(14627351832016453632)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(14627351832016453632)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(14627351832016453632)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(223195676147712))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(223195676147712)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE144:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(223195676147712), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE144]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(223195676147712)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(223195676147712))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE144]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE144]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE145:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(223195676147712), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE145]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(223195676147712)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(223195676147712))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE145]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE145]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(223195676147712))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(223195676147712)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(223195676147712)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(223195676147712)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(223195676147712)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(223195676147712)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(3405695742))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(3405695742)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE146:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(3405695742), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE146]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(3405695742)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(3405695742))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE146]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE146]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE147:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(3405695742), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE147]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(3405695742)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(3405695742))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE147]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE147]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=always>(const<u64>(3405695742))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(3405695742)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(3405695742)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(3405695742)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(3405695742)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(3405695742)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(18446744073709551615))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ffsll]], const<u64>(18446744073709551615)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE148:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE148]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(18446744073709551615)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clzll]], const<u64>(18446744073709551615))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE148]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE148]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE149:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE149]], ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(18446744073709551615)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_ctzll]], const<u64>(18446744073709551615))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE149]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE149]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(18446744073709551615))), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_clrsbll]], const<u64>(18446744073709551615)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(18446744073709551615)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_popcountll]], const<u64>(18446744073709551615)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(18446744073709551615)), call<i32, signature=fn(u64) -> i32>(%[[VALUE_my_parityll]], const<u64>(18446744073709551615)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
