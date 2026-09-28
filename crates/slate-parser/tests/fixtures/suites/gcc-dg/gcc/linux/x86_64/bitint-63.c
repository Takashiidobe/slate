/* PR tree-optimization/113297 */
/* { dg-do compile { target bitint } } */
/* { dg-require-stack-check "generic" } */
/* { dg-options "-std=c23 -O -fno-tree-fre --param=large-stack-frame=1024 -fstack-check=generic" } */

#if __BITINT_MAXWIDTH__ >= 513
typedef _BitInt(513) B;
#else
typedef int B;
#endif

static inline __attribute__((__always_inline__)) void
bar (B x)
{
  B y = x;
  if (y)
    __builtin_abort ();
}

void
foo (void)
{
  bar (0);
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
// DEFAULT-NEXT:     type @type0 B = i513b;
// DEFAULT-NEXT:     fn %5 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @bar(%2 x: i513b) -> void [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 y: i513b [storage=automatic] = read<i513b>(%2);
// DEFAULT-NEXT:         if ne<i513b>(read<i513b>(%3), const<i513b>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i513b) -> void>(%1, widen<i513b, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
