/* PR tree-optimization/126504 */
/* { dg-do run { target bitint } } */

typedef unsigned _BitInt(4) U;

[[gnu::noipa]] int foo(U n) { return ((1 << n) & (1 << 20)) != 0; }

[[gnu::noipa]] int bar(U n) { return ((unsigned)(1 << n) & (1u << 20)) != 0; }

int
main() {
  for (unsigned i = 0; i < 16; i++)
    if (foo(i) || bar(i))
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
// DEFAULT-NEXT:     type @type0 U = u4b;
// DEFAULT-NEXT:     fn %1 @foo(%2 n: u4b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(and<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<u4b>(%2)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(20))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 n: u4b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<u32>(and<u32>(reinterpret<u32, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<u4b>(%4))), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(20))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %6 i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: u32 [synthetic] = read<u32>(%6);
// DEFAULT-NEXT:                 let %10: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%9), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%6, read<u32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %11: bool [synthetic];
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(u4b) -> i32>(%1, truncate<u4b, reason=arg, fits=unknown>(read<u32>(%6))), const<i32>(0))
// DEFAULT-NEXT:                     write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%11, ne<i32>(call<i32, signature=fn(u4b) -> i32>(%3, truncate<u4b, reason=arg, fits=unknown>(read<u32>(%6))), const<i32>(0)));
// DEFAULT-NEXT:                 if read<bool>(%11)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
