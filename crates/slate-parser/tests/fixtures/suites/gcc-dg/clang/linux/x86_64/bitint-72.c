/* PR middle-end/113410 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23" } */

#if __BITINT_MAXWIDTH__ >= 905
void bar (_BitInt(905) n, int[n]);
#else
void bar (int n, int[n]);
#endif

void
foo (int n)
{
  int buf[n];
  bar (n, buf);
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     fn %1 @bar(%5 n: i905b, %6 <unnamed>: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%3)));
// DEFAULT-NEXT:         let %4 buf: vla<i32, %7> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i905b, ptr<i32>) -> void>(%1, widen<i905b, reason=arg>(read<i32>(%3)), array_decay<ptr<i32>, length=None>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
