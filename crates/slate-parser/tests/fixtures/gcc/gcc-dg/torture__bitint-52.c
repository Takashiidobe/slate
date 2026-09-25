/* PR middle-end/113574 */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

unsigned _BitInt(1) a;
unsigned _BitInt(8) b;

void foo(unsigned _BitInt(16) x) { a += (x << 2) | b; }

int
main() {
  foo(0xfef1uwb);
  if (a)
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
// DEFAULT-NEXT:     global %0 a: u1b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: u8b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 x: u16b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5: u1b [synthetic] = read<u1b>(%0);
// DEFAULT-NEXT:         let %6: u1b [synthetic] = truncate<u1b, reason=assign, fits=unknown>(add<u16b, overflow=wrap>(widen<u16b, reason=usual_arith>(read<u1b>(%5)), or<u16b>(shl<u16b, overflow=wrap, amount_out_of_range=ub>(read<u16b>(%3), const<i32>(2)), widen<u16b, reason=usual_arith>(read<u8b>(%1)))));
// DEFAULT-NEXT:         write<u1b>(%0, read<u1b>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u16b) -> void>(%2, const<u16b>(65265));
// DEFAULT-NEXT:         if ne<u1b>(read<u1b>(%0), const<u1b>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
