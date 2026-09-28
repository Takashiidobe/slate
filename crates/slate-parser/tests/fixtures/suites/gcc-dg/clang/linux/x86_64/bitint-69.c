/* PR tree-optimization/113330 */
/* { dg-do compile { target bitint } } */
/* { dg-require-stack-check "generic" } */
/* { dg-options "-std=c23 -O --param=large-stack-frame=131072 -fstack-check=generic --param=sccvn-max-alias-queries-per-access=0" } */

_BitInt(8) a;

static inline __attribute__((__always_inline__)) void
bar (int, int, int, int, int, int, int, int)
{
#if __BITINT_MAXWIDTH__ >= 65535
  _BitInt(65535) b = 0;
  _BitInt(383) c = 0;
#else
  _BitInt(63) b = 0;
  _BitInt(39) c = 0;
#endif
  a = b;
}

void
foo (void)
{
  bar (0, 0, 0, 0, 0, 0, 0, 0);
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
// DEFAULT-NEXT:     global %0 a: i8b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar(%5 <unnamed>: i32, %6 <unnamed>: i32, %7 <unnamed>: i32, %8 <unnamed>: i32, %9 <unnamed>: i32, %10 <unnamed>: i32, %11 <unnamed>: i32, %12 <unnamed>: i32) -> void [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 b: i65535b [storage=automatic] = widen<i65535b, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %3 c: i383b [storage=automatic] = widen<i383b, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         write<i8b>(%0, truncate<i8b, reason=assign, fits=unknown>(read<i65535b>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32) -> void>(%1, const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
