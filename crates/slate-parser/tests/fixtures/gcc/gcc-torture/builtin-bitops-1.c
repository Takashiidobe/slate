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
// DEFAULT-NEXT:     global %65 ints: array<u32, 13> [storage=static] [align=16] = aggregate<array<u32, 13>, zero_fill=false>(index0 = truncate<u32, reason=assign, fits=always>(const<u64>(0)), index1 = truncate<u32, reason=assign, fits=always>(const<u64>(1)), index2 = truncate<u32, reason=assign, fits=always>(const<u64>(2147483648)), index3 = truncate<u32, reason=assign, fits=always>(const<u64>(2)), index4 = truncate<u32, reason=assign, fits=always>(const<u64>(1073741824)), index5 = truncate<u32, reason=assign, fits=always>(const<u64>(65536)), index6 = truncate<u32, reason=assign, fits=always>(const<u64>(32768)), index7 = truncate<u32, reason=assign, fits=always>(const<u64>(2779096485)), index8 = truncate<u32, reason=assign, fits=always>(const<u64>(1515870810)), index9 = truncate<u32, reason=assign, fits=always>(const<u64>(3405643776)), index10 = truncate<u32, reason=assign, fits=always>(const<u64>(13303296)), index11 = truncate<u32, reason=assign, fits=always>(const<u64>(51966)), index12 = truncate<u32, reason=assign, fits=always>(const<u64>(4294967295))) [linkage=external];
// DEFAULT-NEXT:     global %66 longs: array<u64, 13> [storage=static] [align=16] = aggregate<array<u64, 13>, zero_fill=false>(index0 = const<u64>(0), index1 = const<u64>(1), index2 = const<u64>(9223372036854775808), index3 = const<u64>(2), index4 = const<u64>(4611686018427387904), index5 = const<u64>(4294967296), index6 = const<u64>(2147483648), index7 = const<u64>(11936128518282651045), index8 = const<u64>(6510615555426900570), index9 = const<u64>(14627351832016453632), index10 = const<u64>(223195676147712), index11 = const<u64>(3405695742), index12 = const<u64>(18446744073709551615)) [linkage=external];
// DEFAULT-NEXT:     global %67 longlongs: array<u64, 13> [storage=static] [align=16] = aggregate<array<u64, 13>, zero_fill=false>(index0 = const<u64>(0), index1 = const<u64>(1), index2 = const<u64>(9223372036854775808), index3 = const<u64>(2), index4 = const<u64>(4611686018427387904), index5 = const<u64>(4294967296), index6 = const<u64>(2147483648), index7 = const<u64>(11936128518282651045), index8 = const<u64>(6510615555426900570), index9 = const<u64>(14627351832016453632), index10 = const<u64>(223195676147712), index11 = const<u64>(3405695742), index12 = const<u64>(18446744073709551615)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @my_ffs(%1 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 i: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %70
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%2))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %128: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %129: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%128), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%129));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(read<u32>(%1), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), read<i32>(%2))), const<u32>(0))
// DEFAULT-NEXT:                     break %70;
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%2), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @my_ctz(%4 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %71
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%5))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %130: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %131: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%130), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%131));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(read<u32>(%4), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), read<i32>(%5))), const<u32>(0))
// DEFAULT-NEXT:                     break %71;
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @my_clz(%7 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %72
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%8))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %132: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %133: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%132), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%133));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(read<u32>(%7), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), const<u32>(0))
// DEFAULT-NEXT:                     break %72;
// DEFAULT-NEXT:         return read<i32>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @my_clrsb(%10 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 leading: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%10), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         for %73
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(1));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%11))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %134: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %135: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%134), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%135));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%10), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%11)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%12)))
// DEFAULT-NEXT:                     break %73;
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @my_popcount(%14 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %16 count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %74
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%15, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%15))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %136: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                 let %137: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%136), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%15, read<i32>(%137));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(read<u32>(%14), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), read<i32>(%15))), const<u32>(0))
// DEFAULT-NEXT:                     let %138: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                     let %139: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%138), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%16, read<i32>(%139));
// DEFAULT-NEXT:         return read<i32>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @my_parity(%18 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %20 count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %75
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %140: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %141: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%140), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%141));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(read<u32>(%18), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)), read<i32>(%19))), const<u32>(0))
// DEFAULT-NEXT:                     let %142: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                     let %143: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%142), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%20, read<i32>(%143));
// DEFAULT-NEXT:         return and<i32>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @my_ffsl(%22 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 i: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%22), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %76
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%23, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%23))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %144: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                 let %145: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%144), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%23, read<i32>(%145));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%22), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%23))), const<u64>(0))
// DEFAULT-NEXT:                     break %76;
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @my_ctzl(%25 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %26 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %77
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%26, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%26))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %146: i32 [synthetic] = read<i32>(%26);
// DEFAULT-NEXT:                 let %147: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%146), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%26, read<i32>(%147));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%25), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%26))), const<u64>(0))
// DEFAULT-NEXT:                     break %77;
// DEFAULT-NEXT:         return read<i32>(%26);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @my_clzl(%28 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %29 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %78
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%29, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%29))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %148: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:                 let %149: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%148), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%29, read<i32>(%149));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%28), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%29)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), const<u64>(0))
// DEFAULT-NEXT:                     break %78;
// DEFAULT-NEXT:         return read<i32>(%29);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @my_clrsbl(%31 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %32 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %33 leading: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%31), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         for %79
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%32, const<i32>(1));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%32))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %150: i32 [synthetic] = read<i32>(%32);
// DEFAULT-NEXT:                 let %151: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%150), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%32, read<i32>(%151));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%31), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%32)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%33))))
// DEFAULT-NEXT:                     break %79;
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @my_popcountl(%35 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %36 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %37 count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %80
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%36, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%36))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %152: i32 [synthetic] = read<i32>(%36);
// DEFAULT-NEXT:                 let %153: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%152), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%36, read<i32>(%153));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%35), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%36))), const<u64>(0))
// DEFAULT-NEXT:                     let %154: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:                     let %155: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%154), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%37, read<i32>(%155));
// DEFAULT-NEXT:         return read<i32>(%37);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @my_parityl(%39 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %40 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %41 count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %81
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%40, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%40))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %156: i32 [synthetic] = read<i32>(%40);
// DEFAULT-NEXT:                 let %157: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%156), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%40, read<i32>(%157));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%39), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%40))), const<u64>(0))
// DEFAULT-NEXT:                     let %158: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:                     let %159: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%158), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%41, read<i32>(%159));
// DEFAULT-NEXT:         return and<i32>(read<i32>(%41), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @my_ffsll(%43 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %44 i: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%43), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %82
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%44, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%44))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %160: i32 [synthetic] = read<i32>(%44);
// DEFAULT-NEXT:                 let %161: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%160), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%44, read<i32>(%161));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%43), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%44))), const<u64>(0))
// DEFAULT-NEXT:                     break %82;
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @my_ctzll(%46 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %47 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %83
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%47, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%47))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %162: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:                 let %163: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%162), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%47, read<i32>(%163));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%46), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%47))), const<u64>(0))
// DEFAULT-NEXT:                     break %83;
// DEFAULT-NEXT:         return read<i32>(%47);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @my_clzll(%49 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %50 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %84
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%50, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%50))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %164: i32 [synthetic] = read<i32>(%50);
// DEFAULT-NEXT:                 let %165: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%164), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%50, read<i32>(%165));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%49), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%50)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), const<u64>(0))
// DEFAULT-NEXT:                     break %84;
// DEFAULT-NEXT:         return read<i32>(%50);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @my_clrsbll(%52 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %53 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %54 leading: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%52), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         for %85
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%53, const<i32>(1));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%53))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %166: i32 [synthetic] = read<i32>(%53);
// DEFAULT-NEXT:                 let %167: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%166), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%53, read<i32>(%167));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%52), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%53)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%54))))
// DEFAULT-NEXT:                     break %85;
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%53), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @my_popcountll(%56 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %57 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %58 count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %86
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%57, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%57))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %168: i32 [synthetic] = read<i32>(%57);
// DEFAULT-NEXT:                 let %169: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%168), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%57, read<i32>(%169));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%56), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%57))), const<u64>(0))
// DEFAULT-NEXT:                     let %170: i32 [synthetic] = read<i32>(%58);
// DEFAULT-NEXT:                     let %171: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%170), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%58, read<i32>(%171));
// DEFAULT-NEXT:         return read<i32>(%58);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %59 @my_parityll(%60 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %61 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %62 count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %87
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%61, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%61))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %172: i32 [synthetic] = read<i32>(%61);
// DEFAULT-NEXT:                 let %173: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%172), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%61, read<i32>(%173));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%60), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), read<i32>(%61))), const<u64>(0))
// DEFAULT-NEXT:                     let %174: i32 [synthetic] = read<i32>(%62);
// DEFAULT-NEXT:                     let %175: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%174), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%62, read<i32>(%175));
// DEFAULT-NEXT:         return and<i32>(read<i32>(%62), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %64 @exit(%88 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %91 @__builtin_ffs(%90 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %93 @__builtin_clz(%92 <unnamed>: u32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %95 @__builtin_ctz(%94 <unnamed>: u32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %97 @__builtin_clrsb(%96 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %99 @__builtin_popcount(%98 <unnamed>: u32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %101 @__builtin_parity(%100 <unnamed>: u32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %104 @__builtin_ffsl(%103 <unnamed>: i64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %106 @__builtin_clzl(%105 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %108 @__builtin_ctzl(%107 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %110 @__builtin_clrsbl(%109 <unnamed>: i64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %112 @__builtin_popcountl(%111 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %114 @__builtin_parityl(%113 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %117 @__builtin_ffsll(%116 <unnamed>: i64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %119 @__builtin_clzll(%118 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %121 @__builtin_ctzll(%120 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %123 @__builtin_clrsbll(%122 <unnamed>: i64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %125 @__builtin_popcountll(%124 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %127 @__builtin_parityll(%126 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %68 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %69 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %89
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%69, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%69))), div<u64, by_zero=ub>(const<u64>(52), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %176: i32 [synthetic] = read<i32>(%69);
// DEFAULT-NEXT:                 let %177: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%176), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%69, read<i32>(%177));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69)))))), call<i32, signature=fn(u32) -> i32>(%0, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     let %178: bool [synthetic];
// DEFAULT-NEXT:                     if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                         write<bool>(%178, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69))))), call<i32, signature=fn(u32) -> i32>(%6, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69)))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%178, const<bool>(false));
// DEFAULT-NEXT:                     if read<bool>(%178)
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     let %179: bool [synthetic];
// DEFAULT-NEXT:                     if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                         write<bool>(%179, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69))))), call<i32, signature=fn(u32) -> i32>(%3, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69)))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%179, const<bool>(false));
// DEFAULT-NEXT:                     if read<bool>(%179)
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69)))))), call<i32, signature=fn(u32) -> i32>(%9, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69))))), call<i32, signature=fn(u32) -> i32>(%13, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69))))), call<i32, signature=fn(u32) -> i32>(%17, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(13)>(%65), read<i32>(%69))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %102
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%69, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%69))), div<u64, by_zero=ub>(const<u64>(104), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %180: i32 [synthetic] = read<i32>(%69);
// DEFAULT-NEXT:                 let %181: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%180), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%69, read<i32>(%181));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i64) -> i32>(%104, reinterpret<i64, reason=arg, fits=unknown>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69)))))), call<i32, signature=fn(u64) -> i32>(%21, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     let %182: bool [synthetic];
// DEFAULT-NEXT:                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                         write<bool>(%182, ne<i32>(call<i32, signature=fn(u64) -> i32>(%106, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69))))), call<i32, signature=fn(u64) -> i32>(%27, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69)))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%182, const<bool>(false));
// DEFAULT-NEXT:                     if read<bool>(%182)
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     let %183: bool [synthetic];
// DEFAULT-NEXT:                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                         write<bool>(%183, ne<i32>(call<i32, signature=fn(u64) -> i32>(%108, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69))))), call<i32, signature=fn(u64) -> i32>(%24, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69)))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%183, const<bool>(false));
// DEFAULT-NEXT:                     if read<bool>(%183)
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i64) -> i32>(%110, reinterpret<i64, reason=arg, fits=unknown>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69)))))), call<i32, signature=fn(u64) -> i32>(%30, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u64) -> i32>(%112, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69))))), call<i32, signature=fn(u64) -> i32>(%34, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u64) -> i32>(%114, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69))))), call<i32, signature=fn(u64) -> i32>(%38, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%66), read<i32>(%69))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %115
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%69, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%69))), div<u64, by_zero=ub>(const<u64>(104), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %184: i32 [synthetic] = read<i32>(%69);
// DEFAULT-NEXT:                 let %185: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%184), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%69, read<i32>(%185));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=unknown>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69)))))), call<i32, signature=fn(u64) -> i32>(%42, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     let %186: bool [synthetic];
// DEFAULT-NEXT:                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                         write<bool>(%186, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69))))), call<i32, signature=fn(u64) -> i32>(%48, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69)))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%186, const<bool>(false));
// DEFAULT-NEXT:                     if read<bool>(%186)
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     let %187: bool [synthetic];
// DEFAULT-NEXT:                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                         write<bool>(%187, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69))))), call<i32, signature=fn(u64) -> i32>(%45, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69)))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%187, const<bool>(false));
// DEFAULT-NEXT:                     if read<bool>(%187)
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=unknown>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69)))))), call<i32, signature=fn(u64) -> i32>(%51, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69))))), call<i32, signature=fn(u64) -> i32>(%55, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69))))), call<i32, signature=fn(u64) -> i32>(%59, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(13)>(%67), read<i32>(%69))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(0)))), call<i32, signature=fn(u32) -> i32>(%0, truncate<u32, reason=arg, fits=always>(const<u64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %188: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%188, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, truncate<u32, reason=arg, fits=always>(const<u64>(0))), call<i32, signature=fn(u32) -> i32>(%6, truncate<u32, reason=arg, fits=always>(const<u64>(0)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%188, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%188)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %189: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%189, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, truncate<u32, reason=arg, fits=always>(const<u64>(0))), call<i32, signature=fn(u32) -> i32>(%3, truncate<u32, reason=arg, fits=always>(const<u64>(0)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%189, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%189)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(0)))), call<i32, signature=fn(u32) -> i32>(%9, truncate<u32, reason=arg, fits=always>(const<u64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, truncate<u32, reason=arg, fits=always>(const<u64>(0))), call<i32, signature=fn(u32) -> i32>(%13, truncate<u32, reason=arg, fits=always>(const<u64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, truncate<u32, reason=arg, fits=always>(const<u64>(0))), call<i32, signature=fn(u32) -> i32>(%17, truncate<u32, reason=arg, fits=always>(const<u64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1)))), call<i32, signature=fn(u32) -> i32>(%0, truncate<u32, reason=arg, fits=always>(const<u64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %190: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%190, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, truncate<u32, reason=arg, fits=always>(const<u64>(1))), call<i32, signature=fn(u32) -> i32>(%6, truncate<u32, reason=arg, fits=always>(const<u64>(1)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%190, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%190)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %191: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%191, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, truncate<u32, reason=arg, fits=always>(const<u64>(1))), call<i32, signature=fn(u32) -> i32>(%3, truncate<u32, reason=arg, fits=always>(const<u64>(1)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%191, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%191)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1)))), call<i32, signature=fn(u32) -> i32>(%9, truncate<u32, reason=arg, fits=always>(const<u64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, truncate<u32, reason=arg, fits=always>(const<u64>(1))), call<i32, signature=fn(u32) -> i32>(%13, truncate<u32, reason=arg, fits=always>(const<u64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, truncate<u32, reason=arg, fits=always>(const<u64>(1))), call<i32, signature=fn(u32) -> i32>(%17, truncate<u32, reason=arg, fits=always>(const<u64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(2147483648)))), call<i32, signature=fn(u32) -> i32>(%0, truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %192: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2147483648), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%192, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))), call<i32, signature=fn(u32) -> i32>(%6, truncate<u32, reason=arg, fits=always>(const<u64>(2147483648)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%192, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%192)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %193: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2147483648), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%193, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))), call<i32, signature=fn(u32) -> i32>(%3, truncate<u32, reason=arg, fits=always>(const<u64>(2147483648)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%193, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%193)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(2147483648)))), call<i32, signature=fn(u32) -> i32>(%9, truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))), call<i32, signature=fn(u32) -> i32>(%13, truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))), call<i32, signature=fn(u32) -> i32>(%17, truncate<u32, reason=arg, fits=always>(const<u64>(2147483648))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1073741824)))), call<i32, signature=fn(u32) -> i32>(%0, truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %194: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1073741824), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%194, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))), call<i32, signature=fn(u32) -> i32>(%6, truncate<u32, reason=arg, fits=always>(const<u64>(1073741824)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%194, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%194)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %195: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1073741824), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%195, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))), call<i32, signature=fn(u32) -> i32>(%3, truncate<u32, reason=arg, fits=always>(const<u64>(1073741824)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%195, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%195)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1073741824)))), call<i32, signature=fn(u32) -> i32>(%9, truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))), call<i32, signature=fn(u32) -> i32>(%13, truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))), call<i32, signature=fn(u32) -> i32>(%17, truncate<u32, reason=arg, fits=always>(const<u64>(1073741824))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(65536)))), call<i32, signature=fn(u32) -> i32>(%0, truncate<u32, reason=arg, fits=always>(const<u64>(65536))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %196: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(65536), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%196, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, truncate<u32, reason=arg, fits=always>(const<u64>(65536))), call<i32, signature=fn(u32) -> i32>(%6, truncate<u32, reason=arg, fits=always>(const<u64>(65536)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%196, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%196)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %197: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(65536), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%197, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, truncate<u32, reason=arg, fits=always>(const<u64>(65536))), call<i32, signature=fn(u32) -> i32>(%3, truncate<u32, reason=arg, fits=always>(const<u64>(65536)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%197, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%197)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(65536)))), call<i32, signature=fn(u32) -> i32>(%9, truncate<u32, reason=arg, fits=always>(const<u64>(65536))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, truncate<u32, reason=arg, fits=always>(const<u64>(65536))), call<i32, signature=fn(u32) -> i32>(%13, truncate<u32, reason=arg, fits=always>(const<u64>(65536))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, truncate<u32, reason=arg, fits=always>(const<u64>(65536))), call<i32, signature=fn(u32) -> i32>(%17, truncate<u32, reason=arg, fits=always>(const<u64>(65536))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(32768)))), call<i32, signature=fn(u32) -> i32>(%0, truncate<u32, reason=arg, fits=always>(const<u64>(32768))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %198: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(32768), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%198, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, truncate<u32, reason=arg, fits=always>(const<u64>(32768))), call<i32, signature=fn(u32) -> i32>(%6, truncate<u32, reason=arg, fits=always>(const<u64>(32768)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%198, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%198)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %199: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(32768), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%199, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, truncate<u32, reason=arg, fits=always>(const<u64>(32768))), call<i32, signature=fn(u32) -> i32>(%3, truncate<u32, reason=arg, fits=always>(const<u64>(32768)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%199, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%199)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(32768)))), call<i32, signature=fn(u32) -> i32>(%9, truncate<u32, reason=arg, fits=always>(const<u64>(32768))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, truncate<u32, reason=arg, fits=always>(const<u64>(32768))), call<i32, signature=fn(u32) -> i32>(%13, truncate<u32, reason=arg, fits=always>(const<u64>(32768))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, truncate<u32, reason=arg, fits=always>(const<u64>(32768))), call<i32, signature=fn(u32) -> i32>(%17, truncate<u32, reason=arg, fits=always>(const<u64>(32768))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(2779096485)))), call<i32, signature=fn(u32) -> i32>(%0, truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %200: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2779096485), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%200, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))), call<i32, signature=fn(u32) -> i32>(%6, truncate<u32, reason=arg, fits=always>(const<u64>(2779096485)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%200, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%200)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %201: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2779096485), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%201, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))), call<i32, signature=fn(u32) -> i32>(%3, truncate<u32, reason=arg, fits=always>(const<u64>(2779096485)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%201, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%201)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(2779096485)))), call<i32, signature=fn(u32) -> i32>(%9, truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))), call<i32, signature=fn(u32) -> i32>(%13, truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))), call<i32, signature=fn(u32) -> i32>(%17, truncate<u32, reason=arg, fits=always>(const<u64>(2779096485))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1515870810)))), call<i32, signature=fn(u32) -> i32>(%0, truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %202: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1515870810), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%202, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))), call<i32, signature=fn(u32) -> i32>(%6, truncate<u32, reason=arg, fits=always>(const<u64>(1515870810)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%202, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%202)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %203: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1515870810), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%203, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))), call<i32, signature=fn(u32) -> i32>(%3, truncate<u32, reason=arg, fits=always>(const<u64>(1515870810)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%203, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%203)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(1515870810)))), call<i32, signature=fn(u32) -> i32>(%9, truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))), call<i32, signature=fn(u32) -> i32>(%13, truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))), call<i32, signature=fn(u32) -> i32>(%17, truncate<u32, reason=arg, fits=always>(const<u64>(1515870810))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(3405643776)))), call<i32, signature=fn(u32) -> i32>(%0, truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %204: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(3405643776), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%204, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))), call<i32, signature=fn(u32) -> i32>(%6, truncate<u32, reason=arg, fits=always>(const<u64>(3405643776)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%204, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%204)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %205: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(3405643776), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%205, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))), call<i32, signature=fn(u32) -> i32>(%3, truncate<u32, reason=arg, fits=always>(const<u64>(3405643776)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%205, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%205)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(3405643776)))), call<i32, signature=fn(u32) -> i32>(%9, truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))), call<i32, signature=fn(u32) -> i32>(%13, truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))), call<i32, signature=fn(u32) -> i32>(%17, truncate<u32, reason=arg, fits=always>(const<u64>(3405643776))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(13303296)))), call<i32, signature=fn(u32) -> i32>(%0, truncate<u32, reason=arg, fits=always>(const<u64>(13303296))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %206: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(13303296), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%206, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, truncate<u32, reason=arg, fits=always>(const<u64>(13303296))), call<i32, signature=fn(u32) -> i32>(%6, truncate<u32, reason=arg, fits=always>(const<u64>(13303296)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%206, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%206)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %207: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(13303296), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%207, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, truncate<u32, reason=arg, fits=always>(const<u64>(13303296))), call<i32, signature=fn(u32) -> i32>(%3, truncate<u32, reason=arg, fits=always>(const<u64>(13303296)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%207, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%207)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(13303296)))), call<i32, signature=fn(u32) -> i32>(%9, truncate<u32, reason=arg, fits=always>(const<u64>(13303296))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, truncate<u32, reason=arg, fits=always>(const<u64>(13303296))), call<i32, signature=fn(u32) -> i32>(%13, truncate<u32, reason=arg, fits=always>(const<u64>(13303296))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, truncate<u32, reason=arg, fits=always>(const<u64>(13303296))), call<i32, signature=fn(u32) -> i32>(%17, truncate<u32, reason=arg, fits=always>(const<u64>(13303296))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(51966)))), call<i32, signature=fn(u32) -> i32>(%0, truncate<u32, reason=arg, fits=always>(const<u64>(51966))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %208: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(51966), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%208, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, truncate<u32, reason=arg, fits=always>(const<u64>(51966))), call<i32, signature=fn(u32) -> i32>(%6, truncate<u32, reason=arg, fits=always>(const<u64>(51966)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%208, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%208)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %209: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(51966), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%209, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, truncate<u32, reason=arg, fits=always>(const<u64>(51966))), call<i32, signature=fn(u32) -> i32>(%3, truncate<u32, reason=arg, fits=always>(const<u64>(51966)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%209, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%209)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(51966)))), call<i32, signature=fn(u32) -> i32>(%9, truncate<u32, reason=arg, fits=always>(const<u64>(51966))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, truncate<u32, reason=arg, fits=always>(const<u64>(51966))), call<i32, signature=fn(u32) -> i32>(%13, truncate<u32, reason=arg, fits=always>(const<u64>(51966))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, truncate<u32, reason=arg, fits=always>(const<u64>(51966))), call<i32, signature=fn(u32) -> i32>(%17, truncate<u32, reason=arg, fits=always>(const<u64>(51966))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%91, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(4294967295)))), call<i32, signature=fn(u32) -> i32>(%0, truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %210: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4294967295), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%210, ne<i32>(call<i32, signature=fn(u32) -> i32>(%93, truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))), call<i32, signature=fn(u32) -> i32>(%6, truncate<u32, reason=arg, fits=always>(const<u64>(4294967295)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%210, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%210)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %211: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4294967295), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%211, ne<i32>(call<i32, signature=fn(u32) -> i32>(%95, truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))), call<i32, signature=fn(u32) -> i32>(%3, truncate<u32, reason=arg, fits=always>(const<u64>(4294967295)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%211, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%211)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%97, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(4294967295)))), call<i32, signature=fn(u32) -> i32>(%9, truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%99, truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))), call<i32, signature=fn(u32) -> i32>(%13, truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%101, truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))), call<i32, signature=fn(u32) -> i32>(%17, truncate<u32, reason=arg, fits=always>(const<u64>(4294967295))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=always>(const<u64>(0))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %212: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%212, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(0)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(0))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%212, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%212)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %213: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%213, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(0)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(0))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%213, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%213)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=always>(const<u64>(0))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(0)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(0)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=always>(const<u64>(1))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %214: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%214, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(1)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%214, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%214)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %215: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%215, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(1)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%215, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%215)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=always>(const<u64>(1))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(1)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(1)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=unknown>(const<u64>(9223372036854775808))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(9223372036854775808)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %216: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(9223372036854775808), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%216, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(9223372036854775808)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(9223372036854775808))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%216, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%216)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %217: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(9223372036854775808), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%217, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(9223372036854775808)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(9223372036854775808))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%217, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%217)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=unknown>(const<u64>(9223372036854775808))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(9223372036854775808)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(9223372036854775808)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(9223372036854775808)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(9223372036854775808)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(9223372036854775808)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=always>(const<u64>(2))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %218: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%218, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(2)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(2))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%218, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%218)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %219: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%219, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(2)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(2))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%219, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%219)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=always>(const<u64>(2))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(2)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(2)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=always>(const<u64>(4611686018427387904))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(4611686018427387904)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %220: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4611686018427387904), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%220, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(4611686018427387904)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(4611686018427387904))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%220, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%220)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %221: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4611686018427387904), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%221, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(4611686018427387904)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(4611686018427387904))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%221, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%221)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=always>(const<u64>(4611686018427387904))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(4611686018427387904)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(4611686018427387904)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(4611686018427387904)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(4611686018427387904)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(4611686018427387904)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=always>(const<u64>(4294967296))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(4294967296)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %222: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4294967296), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%222, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(4294967296)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(4294967296))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%222, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%222)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %223: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4294967296), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%223, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(4294967296)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(4294967296))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%223, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%223)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=always>(const<u64>(4294967296))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(4294967296)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(4294967296)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(4294967296)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(4294967296)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(4294967296)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=always>(const<u64>(2147483648))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(2147483648)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %224: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2147483648), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%224, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(2147483648)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(2147483648))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%224, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%224)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %225: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2147483648), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%225, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(2147483648)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(2147483648))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%225, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%225)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=always>(const<u64>(2147483648))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(2147483648)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(2147483648)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(2147483648)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(2147483648)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(2147483648)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=unknown>(const<u64>(11936128518282651045))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(11936128518282651045)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %226: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(11936128518282651045), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%226, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(11936128518282651045)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(11936128518282651045))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%226, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%226)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %227: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(11936128518282651045), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%227, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(11936128518282651045)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(11936128518282651045))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%227, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%227)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=unknown>(const<u64>(11936128518282651045))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(11936128518282651045)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(11936128518282651045)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(11936128518282651045)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(11936128518282651045)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(11936128518282651045)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=always>(const<u64>(6510615555426900570))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(6510615555426900570)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %228: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(6510615555426900570), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%228, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(6510615555426900570)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(6510615555426900570))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%228, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%228)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %229: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(6510615555426900570), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%229, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(6510615555426900570)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(6510615555426900570))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%229, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%229)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=always>(const<u64>(6510615555426900570))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(6510615555426900570)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(6510615555426900570)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(6510615555426900570)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(6510615555426900570)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(6510615555426900570)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=unknown>(const<u64>(14627351832016453632))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(14627351832016453632)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %230: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(14627351832016453632), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%230, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(14627351832016453632)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(14627351832016453632))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%230, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%230)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %231: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(14627351832016453632), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%231, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(14627351832016453632)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(14627351832016453632))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%231, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%231)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=unknown>(const<u64>(14627351832016453632))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(14627351832016453632)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(14627351832016453632)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(14627351832016453632)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(14627351832016453632)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(14627351832016453632)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=always>(const<u64>(223195676147712))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(223195676147712)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %232: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(223195676147712), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%232, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(223195676147712)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(223195676147712))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%232, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%232)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %233: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(223195676147712), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%233, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(223195676147712)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(223195676147712))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%233, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%233)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=always>(const<u64>(223195676147712))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(223195676147712)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(223195676147712)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(223195676147712)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(223195676147712)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(223195676147712)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=always>(const<u64>(3405695742))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(3405695742)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %234: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(3405695742), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%234, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(3405695742)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(3405695742))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%234, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%234)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %235: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(3405695742), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%235, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(3405695742)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(3405695742))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%235, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%235)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=always>(const<u64>(3405695742))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(3405695742)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(3405695742)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(3405695742)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(3405695742)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(3405695742)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%117, reinterpret<i64, reason=arg, fits=unknown>(const<u64>(18446744073709551615))), call<i32, signature=fn(u64) -> i32>(%42, const<u64>(18446744073709551615)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %236: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%236, ne<i32>(call<i32, signature=fn(u64) -> i32>(%119, const<u64>(18446744073709551615)), call<i32, signature=fn(u64) -> i32>(%48, const<u64>(18446744073709551615))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%236, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%236)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         let %237: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%237, ne<i32>(call<i32, signature=fn(u64) -> i32>(%121, const<u64>(18446744073709551615)), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(18446744073709551615))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%237, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%237)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%123, reinterpret<i64, reason=arg, fits=unknown>(const<u64>(18446744073709551615))), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(18446744073709551615)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%125, const<u64>(18446744073709551615)), call<i32, signature=fn(u64) -> i32>(%55, const<u64>(18446744073709551615)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%127, const<u64>(18446744073709551615)), call<i32, signature=fn(u64) -> i32>(%59, const<u64>(18446744073709551615)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%64, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
