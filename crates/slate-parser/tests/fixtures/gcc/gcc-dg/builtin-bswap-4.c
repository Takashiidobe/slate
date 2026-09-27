/* { dg-do run } */
/* { dg-require-effective-target stdint_types } */
/* { dg-options "-Wall" } */

#include <stdint.h>

#define MAKE_FUN(suffix, type)						\
  type my_bswap##suffix(type x) {					\
    type result = 0;							\
    int shift;								\
    for (shift = 0; shift < 8 * sizeof (type); shift += 8)	\
      {									\
	result <<= 8;							\
	result |= (x >> shift) & 0xff;					\
      }									\
    return result;							\
  }									\

MAKE_FUN(16, uint16_t);
MAKE_FUN(32, uint32_t);
MAKE_FUN(64, uint64_t);

extern void abort (void);

#define NUMS16					\
  {						\
    0x0000,					\
    0x1122,					\
    0xffff,					\
  }

#define NUMS32					\
  {						\
    0x00000000UL,				\
    0x11223344UL,				\
    0xffffffffUL,				\
  }

#define NUMS64					\
  {						\
    0x0000000000000000ULL,			\
    0x1122334455667788ULL,			\
    0xffffffffffffffffULL,			\
  }

uint16_t uint16_ts[] =
  NUMS16;

uint32_t uint32_ts[] =
  NUMS32;

uint64_t uint64_ts[] =
  NUMS64;

#define N(table) (sizeof (table) / sizeof (table[0]))

int
main (void)
{
  int i;

  for (i = 0; i < N(uint16_ts); i++)
    if (__builtin_bswap16 (uint16_ts[i]) != my_bswap16 (uint16_ts[i]))
      abort ();

  for (i = 0; i < N(uint32_ts); i++)
    if (__builtin_bswap32 (uint32_ts[i]) != my_bswap32 (uint32_ts[i]))
      abort ();

  for (i = 0; i < N(uint64_ts); i++)
    if (__builtin_bswap64 (uint64_ts[i]) != my_bswap64 (uint64_ts[i]))
      abort ();

  return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type0 __uint16_t = u16;
// DEFAULT-NEXT:     type @type1 __uint32_t = u32;
// DEFAULT-NEXT:     type @type2 __uint64_t = u64;
// DEFAULT-NEXT:     type @type3 uint16_t = u16;
// DEFAULT-NEXT:     type @type4 uint32_t = u32;
// DEFAULT-NEXT:     type @type5 uint64_t = u64;
// DEFAULT-NEXT:     global %19 uint16_ts: array<u16, 3> [storage=static] = aggregate<array<u16, 3>, zero_fill=false>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(4386))), index2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(65535)))) [linkage=external];
// DEFAULT-NEXT:     global %20 uint32_ts: array<u32, 3> [storage=static] = aggregate<array<u32, 3>, zero_fill=false>(index0 = truncate<u32, reason=assign, fits=always>(const<u64>(0)), index1 = truncate<u32, reason=assign, fits=always>(const<u64>(287454020)), index2 = truncate<u32, reason=assign, fits=always>(const<u64>(4294967295))) [linkage=external];
// DEFAULT-NEXT:     global %21 uint64_ts: array<u64, 3> [storage=static] [align=16] = aggregate<array<u64, 3>, zero_fill=false>(index0 = const<u64>(0), index1 = const<u64>(1234605616436508552), index2 = const<u64>(18446744073709551615)) [linkage=external];
// DEFAULT-NEXT:     fn %6 @my_bswap16(%7 x: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 result: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %9 shift: i32 [storage=automatic];
// DEFAULT-NEXT:         for %24
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%9))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(2)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), const<i32>(8));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%37));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %38: u16 [synthetic] = read<u16>(%8);
// DEFAULT-NEXT:                     let %39: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%38))), const<i32>(8))));
// DEFAULT-NEXT:                     write<u16>(%8, read<u16>(%39));
// DEFAULT-NEXT:                     let %40: u16 [synthetic] = read<u16>(%8);
// DEFAULT-NEXT:                     let %41: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%40))), and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%7))), read<i32>(%9)), const<i32>(255)))));
// DEFAULT-NEXT:                     write<u16>(%8, read<u16>(%41));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<u16>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @my_bswap32(%11 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 result: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %13 shift: i32 [storage=automatic];
// DEFAULT-NEXT:         for %25
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%13))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %42: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %43: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%42), const<i32>(8));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%43));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %44: u32 [synthetic] = read<u32>(%12);
// DEFAULT-NEXT:                     let %45: u32 [synthetic] = shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%44), const<i32>(8));
// DEFAULT-NEXT:                     write<u32>(%12, read<u32>(%45));
// DEFAULT-NEXT:                     let %46: u32 [synthetic] = read<u32>(%12);
// DEFAULT-NEXT:                     let %47: u32 [synthetic] = or<u32>(read<u32>(%46), and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%11), read<i32>(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))));
// DEFAULT-NEXT:                     write<u32>(%12, read<u32>(%47));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<u32>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @my_bswap64(%15 x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 result: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %17 shift: i32 [storage=automatic];
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%17))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), const<i32>(8));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%49));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %50: u64 [synthetic] = read<u64>(%16);
// DEFAULT-NEXT:                     let %51: u64 [synthetic] = shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%50), const<i32>(8));
// DEFAULT-NEXT:                     write<u64>(%16, read<u64>(%51));
// DEFAULT-NEXT:                     let %52: u64 [synthetic] = read<u64>(%16);
// DEFAULT-NEXT:                     let %53: u64 [synthetic] = or<u64>(read<u64>(%52), and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%15), read<i32>(%17)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:                     write<u64>(%16, read<u64>(%53));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<u64>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %29 @__builtin_bswap16(%28 <unnamed>: u16) -> u16 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %32 @__builtin_bswap32(%31 <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %35 @__builtin_bswap64(%34 <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %22 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %23 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%23, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%23))), div<u64, by_zero=ub>(const<u64>(6), const<u64>(2)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %54: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                 let %55: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%54), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%23, read<i32>(%55));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%29, read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(3)>(%19), read<i32>(%23))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%6, read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(3)>(%19), read<i32>(%23))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%23, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%23))), div<u64, by_zero=ub>(const<u64>(12), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %56: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                 let %57: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%56), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%23, read<i32>(%57));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(call<u32, signature=fn(u32) -> u32>(%32, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(3)>(%20), read<i32>(%23))))), call<u32, signature=fn(u32) -> u32>(%10, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(3)>(%20), read<i32>(%23))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:         for %33
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%23, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%23))), div<u64, by_zero=ub>(const<u64>(24), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %58: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                 let %59: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%58), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%23, read<i32>(%59));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(u64) -> u64>(%35, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(3)>(%21), read<i32>(%23))))), call<u64, signature=fn(u64) -> u64>(%14, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(3)>(%21), read<i32>(%23))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
