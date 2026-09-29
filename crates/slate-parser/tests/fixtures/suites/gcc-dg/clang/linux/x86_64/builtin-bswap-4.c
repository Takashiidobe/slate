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
// DEFAULT-NEXT:     type @type[[TYPE___uint16_t:[0-9]+]] __uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_uint16_t:[0-9]+]] uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_uint32_t:[0-9]+]] uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_uint64_t:[0-9]+]] uint64_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_uint16_ts:[0-9]+]] uint16_ts: array<u16, 3> [storage=static] = aggregate<array<u16, 3>, zero_fill=false>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(4386))), index2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(65535)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_uint32_ts:[0-9]+]] uint32_ts: array<u32, 3> [storage=static] = aggregate<array<u32, 3>, zero_fill=false>(index0 = truncate<u32, reason=assign, fits=always>(const<u64>(0)), index1 = truncate<u32, reason=assign, fits=always>(const<u64>(287454020)), index2 = truncate<u32, reason=assign, fits=always>(const<u64>(4294967295))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_uint64_ts:[0-9]+]] uint64_ts: array<u64, 3> [storage=static] [align=16] = aggregate<array<u64, 3>, zero_fill=false>(index0 = const<u64>(0), index1 = const<u64>(1234605616436508552), index2 = const<u64>(18446744073709551615)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_my_bswap16:[0-9]+]] @my_bswap16(%[[VALUE_x:[0-9]+]] x: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_shift:[0-9]+]] shift: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_shift]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(2)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_shift]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(8));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_shift]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_result]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE3]]))), const<i32>(8))));
// DEFAULT-NEXT:                     write<u16>(%[[VALUE_result]], read<u16>(%[[VALUE4]]));
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_result]]);
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE5]]))), and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_x]]))), read<i32>(%[[VALUE_shift]])), const<i32>(255)))));
// DEFAULT-NEXT:                     write<u16>(%[[VALUE_result]], read<u16>(%[[VALUE6]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<u16>(%[[VALUE_result]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_bswap32:[0-9]+]] @my_bswap32(%[[VALUE_x_2:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_2:[0-9]+]] result: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_shift_2:[0-9]+]] shift: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_shift_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift_2]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_shift_2]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(8));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_shift_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE10:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_result_2]]);
// DEFAULT-NEXT:                     let %[[VALUE11:[0-9]+]]: u32 [synthetic] = shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE10]]), const<i32>(8));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_result_2]], read<u32>(%[[VALUE11]]));
// DEFAULT-NEXT:                     let %[[VALUE12:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_result_2]]);
// DEFAULT-NEXT:                     let %[[VALUE13:[0-9]+]]: u32 [synthetic] = or<u32>(read<u32>(%[[VALUE12]]), and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_shift_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_result_2]], read<u32>(%[[VALUE13]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_result_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_bswap64:[0-9]+]] @my_bswap64(%[[VALUE_x_3:[0-9]+]] x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_3:[0-9]+]] result: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_shift_3:[0-9]+]] shift: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_shift_3]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift_3]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_shift_3]]);
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(8));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_shift_3]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE17:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_result_3]]);
// DEFAULT-NEXT:                     let %[[VALUE18:[0-9]+]]: u64 [synthetic] = shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE17]]), const<i32>(8));
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_result_3]], read<u64>(%[[VALUE18]]));
// DEFAULT-NEXT:                     let %[[VALUE19:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_result_3]]);
// DEFAULT-NEXT:                     let %[[VALUE20:[0-9]+]]: u64 [synthetic] = or<u64>(read<u64>(%[[VALUE19]]), and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_x_3]]), read<i32>(%[[VALUE_shift_3]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_result_3]], read<u64>(%[[VALUE20]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_result_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap16:[0-9]+]] @__builtin_bswap16(%[[VALUE21:[0-9]+]] <unnamed>: u16) -> u16 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap32:[0-9]+]] @__builtin_bswap32(%[[VALUE22:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap64:[0-9]+]] @__builtin_bswap64(%[[VALUE23:[0-9]+]] <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(6), const<u64>(2)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE26:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE25]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE26]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(3)>(%[[VALUE_uint16_ts]]), read<i32>(%[[VALUE_i]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE_my_bswap16]], read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(3)>(%[[VALUE_uint16_ts]]), read<i32>(%[[VALUE_i]]))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(12), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(3)>(%[[VALUE_uint32_ts]]), read<i32>(%[[VALUE_i]]))))), call<u32, signature=fn(u32) -> u32>(%[[VALUE_my_bswap32]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(3)>(%[[VALUE_uint32_ts]]), read<i32>(%[[VALUE_i]]))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(24), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE31:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE32:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE31]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE32]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(3)>(%[[VALUE_uint64_ts]]), read<i32>(%[[VALUE_i]]))))), call<u64, signature=fn(u64) -> u64>(%[[VALUE_my_bswap64]], read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(3)>(%[[VALUE_uint64_ts]]), read<i32>(%[[VALUE_i]]))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
