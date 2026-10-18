/* { dg-do run } */
/* { dg-require-effective-target int128 } */
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

MAKE_FUN(128, __uint128_t);

extern void abort (void);

typedef union
{
  struct { uint64_t lo; uint64_t hi; } s;
  __uint128_t n;
} u;

#define NUMS128							\
  {								\
    { .s = { 0x0000000000000000ULL, 0x1122334455667788ULL } }, 	\
    { .s = { 0x1122334455667788ULL, 0xffffffffffffffffULL } },	\
    { .s = { 0xffffffffffffffffULL, 0x0000000000000000ULL } }	\
  }

u uint128_ts[] = NUMS128;

#define N(table) (sizeof (table) / sizeof (table[0]))

int
main (void)
{
  int i;

  for (i = 0; i < N(uint128_ts); i++)
    if (__builtin_bswap128 (uint128_ts[i].n) != my_bswap128 (uint128_ts[i].n))
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
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_uint64_t:[0-9]+]] uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 s: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:         field1 n: u128;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = struct {
// DEFAULT-NEXT:         field0 lo: u64;
// DEFAULT-NEXT:         field1 hi: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_uint128_ts:[0-9]+]] uint128_ts: array<@type[[TYPE0]], 3> [storage=static] = aggregate<array<@type[[TYPE0]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<u64>(0), field1 = const<u64>(1234605616436508552))), index1 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<u64>(1234605616436508552), field1 = const<u64>(18446744073709551615))), index2 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<u64>(18446744073709551615), field1 = const<u64>(0)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_my_bswap128:[0-9]+]] @my_bswap128(%[[VALUE_x:[0-9]+]] x: u128) -> u128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: u128 [storage=automatic] = reinterpret<u128, reason=assign, fits=unknown>(widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_shift:[0-9]+]] shift: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_shift]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(16)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_shift]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(8));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_shift]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: u128 [synthetic] = read<u128>(%[[VALUE_result]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: u128 [synthetic] = shl<u128, overflow=wrap, amount_out_of_range=ub>(read<u128>(%[[VALUE3]]), const<i32>(8));
// DEFAULT-NEXT:                     write<u128>(%[[VALUE_result]], read<u128>(%[[VALUE4]]));
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: u128 [synthetic] = read<u128>(%[[VALUE_result]]);
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: u128 [synthetic] = or<u128>(read<u128>(%[[VALUE5]]), and<u128>(shr<u128, amount_out_of_range=ub, fill=zero_extend>(read<u128>(%[[VALUE_x]]), read<i32>(%[[VALUE_shift]])), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:                     write<u128>(%[[VALUE_result]], read<u128>(%[[VALUE6]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<u128>(%[[VALUE_result]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap128:[0-9]+]] @__builtin_bswap128(%[[VALUE7:[0-9]+]] <unnamed>: u128) -> u128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(48), const<u64>(16)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u128>(call<u128, signature=fn(u128) -> u128>(%[[VALUE___builtin_bswap128]], read<u128>(field1(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(3)>(%[[VALUE_uint128_ts]]), read<i32>(%[[VALUE_i]])))))), call<u128, signature=fn(u128) -> u128>(%[[VALUE_my_bswap128]], read<u128>(field1(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(3)>(%[[VALUE_uint128_ts]]), read<i32>(%[[VALUE_i]])))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
