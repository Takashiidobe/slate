/* PR tree-optimization/126476 */
/* { dg-do run { target bitint } } */

[[gnu::noipa]] int foo(unsigned _BitInt(4) n) {
  return ((1ULL << n) & (1ULL << 20)) != 0;
}

[[gnu::noipa]] int bar(unsigned _BitInt(4) n) {
  return (((1ULL << 40) >> n) & (1ULL << 20)) != 0;
}

int
main() {
  for (unsigned i = 0; i < 16; i++)
    if (foo(i) != 0 || bar(i) != 0)
      __builtin_abort();
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
// DEFAULT-NEXT:     fn %0 @foo(%1 n: u4b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<u64>(and<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), read<u4b>(%1)), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(20))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bar(%3 n: u4b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(40)), read<u4b>(%3)), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(20))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %5 i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %8: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                 let %9: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%8), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%5, read<u32>(%9));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %10: bool [synthetic];
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(u4b) -> i32>(%0, truncate<u4b, reason=arg, fits=unknown>(read<u32>(%5))), const<i32>(0))
// DEFAULT-NEXT:                     write<bool>(%10, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%10, ne<i32>(call<i32, signature=fn(u4b) -> i32>(%2, truncate<u4b, reason=arg, fits=unknown>(read<u32>(%5))), const<i32>(0)));
// DEFAULT-NEXT:                 if read<bool>(%10)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
