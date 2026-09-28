#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
#else
typedef unsigned uint32_t;
#endif

#define __fake_const_swab32(x)                                                 \
  ((uint32_t)((((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |               \
              (((uint32_t)(x) & (uint32_t)0x0000ff00UL) << 8) |                \
              (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 8) |                \
              (((uint32_t)(x) & (uint32_t)0x0000ff00UL)) |                     \
              (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

/* Previous version of bswap optimization would detect byte swap when none
   happen. This test aims at catching such wrong detection to avoid
   regressions.  */

__attribute__((noinline, noclone)) uint32_t fake_swap32(uint32_t in) {
  return __fake_const_swab32(in);
}

int main(void) {
  if (sizeof(uint32_t) * __CHAR_BIT__ != 32)
    return 0;
  if (fake_swap32(0x12345678UL) != 0x78567E12UL)
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
// DEFAULT-NEXT:     type @type0 uint32_t = u32;
// DEFAULT-NEXT:     fn %1 @fake_swap32(%2 in: u32) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<u32>(or<u32>(or<u32>(or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(%2), truncate<u32, reason=explicit, fits=always>(const<u64>(255))), const<i32>(24)), shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(%2), truncate<u32, reason=explicit, fits=always>(const<u64>(65280))), const<i32>(8))), shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(%2), truncate<u32, reason=explicit, fits=always>(const<u64>(255))), const<i32>(8))), and<u32>(read<u32>(%2), truncate<u32, reason=explicit, fits=always>(const<u64>(65280)))), shr<u32, amount_out_of_range=ub, fill=zero_extend>(and<u32>(read<u32>(%2), truncate<u32, reason=explicit, fits=always>(const<u64>(4278190080))), const<i32>(24)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if ne<u64>(widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%1, truncate<u32, reason=arg, fits=always>(const<u64>(305419896)))), const<u64>(2018934290))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
