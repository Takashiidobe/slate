/* PR rtl-optimization/78546 */
/* { dg-do run { target int128 } } */

typedef unsigned __int128 u128;
u128                      b;

static inline u128 foo(u128 p1) {
  p1 += ~b;
  return -p1;
}

int
main() {
  asm volatile("" : : : "memory");
  u128 x = foo(~0x7fffffffffffffffLL);
  if (x != 0x8000000000000001ULL)
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
// DEFAULT-NEXT:     type @type0 u128 = u128;
// DEFAULT-NEXT:     global %1 b: u128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 p1: u128) -> u128 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7: u128 [synthetic] = read<u128>(%3);
// DEFAULT-NEXT:         let %8: u128 [synthetic] = add<u128, overflow=wrap>(read<u128>(%7), not<u128>(read<u128>(%1)));
// DEFAULT-NEXT:         write<u128>(%3, read<u128>(%8));
// DEFAULT-NEXT:         return neg<u128, overflow=wrap>(read<u128>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %5 x: u128 [storage=automatic] = call<u128, signature=fn(u128) -> u128>(%2, reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(not<i64>(const<i64>(9223372036854775807)))));
// DEFAULT-NEXT:         if ne<u128>(read<u128>(%5), widen<u128, reason=usual_arith>(const<u64>(9223372036854775809)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
