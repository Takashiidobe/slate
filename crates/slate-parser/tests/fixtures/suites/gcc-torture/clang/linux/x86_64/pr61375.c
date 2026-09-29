#ifdef __UINT64_TYPE__
typedef __UINT64_TYPE__ uint64_t;
#else
typedef unsigned long long uint64_t;
#endif

#ifndef __SIZEOF_INT128__
#define __int128 long long
#endif

/* Some version of bswap optimization would ICE when analyzing a mask constant
   too big for an uint64_t variable (PR210931).  */

__attribute__((noinline, noclone)) uint64_t
uint128_central_bitsi_ior(unsigned __int128 in1, uint64_t in2) {
  __int128 mask = (__int128)0xffff << 56;
  return ((in1 & mask) >> 56) | in2;
}

int main(int argc, char **argv) {
  __int128 in = 1;
#ifdef __SIZEOF_INT128__
  in <<= 64;
#endif
  if (sizeof(uint64_t) * __CHAR_BIT__ != 64)
    return 0;
  if (sizeof(unsigned __int128) * __CHAR_BIT__ != 128)
    return 0;
  if (uint128_central_bitsi_ior(in, 2) != 0x102)
    __builtin_abort();
  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_uint64_t:[0-9]+]] uint64_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE_uint128_central_bitsi_ior:[0-9]+]] @uint128_central_bitsi_ior(%[[VALUE_in1:[0-9]+]] in1: u128, %[[VALUE_in2:[0-9]+]] in2: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_mask:[0-9]+]] mask: i128 [storage=automatic] = shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(65535)), const<i32>(56));
// DEFAULT-NEXT:         return truncate<u64, reason=return, fits=unknown>(or<u128>(shr<u128, amount_out_of_range=ub, fill=zero_extend>(and<u128>(read<u128>(%[[VALUE_in1]]), reinterpret<u128, reason=usual_arith, fits=unknown>(read<i128>(%[[VALUE_mask]]))), const<i32>(56)), widen<u128, reason=usual_arith>(read<u64>(%[[VALUE_in2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_in:[0-9]+]] in: i128 [storage=automatic] = widen<i128, reason=assign>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_in]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i128 [synthetic] = shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i128>(%[[VALUE0]]), const<i32>(64));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_in]], read<i128>(%[[VALUE1]]));
// DEFAULT-NEXT:         if ne<u64>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(64))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if ne<u64>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(128))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u128, u64) -> u64>(%[[VALUE_uint128_central_bitsi_ior]], reinterpret<u128, reason=arg, fits=unknown>(read<i128>(%[[VALUE_in]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(258))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
