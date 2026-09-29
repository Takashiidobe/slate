/* PR target/89434 */

#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8 && __CHAR_BIT__ == 8
long g = 0;

static inline unsigned long long foo(unsigned long long u) {
  unsigned x;
  __builtin_mul_overflow(-1, g, &x);
  u |= (unsigned)u < (unsigned short)x;
  return x - u;
}

int main() {
  unsigned long long x = foo(0x222222222ULL);
  if (x != 0xfffffffddddddddeULL)
    __builtin_abort();
  return 0;
}
#else
int main() { return 0; }
#endif


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
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_u:[0-9]+]] u: u64) -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u32 [storage=automatic];
// DEFAULT-NEXT:         overflow_mul<bool>(neg<i32, overflow=ub>(const<i32>(1)), read<i64>(%[[VALUE_g]]), deref(addr_of<ptr<u32>>(%[[VALUE_x]])));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = or<u64>(read<u64>(%[[VALUE0]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(lt<u32>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%[[VALUE_u]])), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_x]]))))))))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_u]], read<u64>(%[[VALUE1]]));
// DEFAULT-NEXT:         return sub<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_x]])), read<u64>(%[[VALUE_u]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: u64 [storage=automatic] = call<u64, signature=fn(u64) -> u64>(%[[VALUE_foo]], const<u64>(9162596898));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_x_2]]), const<u64>(18446744064546954718))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
