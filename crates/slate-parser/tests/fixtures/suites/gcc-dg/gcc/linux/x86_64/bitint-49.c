/* PR tree-optimization/112880 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O2" } */

#if __BITINT_MAXWIDTH__ >= 1024
_BitInt(1024) a, b, c, d, e, f;

void
foo (void)
{
  __builtin_add_overflow (a, b, &a);
  __builtin_sub_overflow (c, d, &c);
  __builtin_mul_overflow (e, f, &e);
}
#endif

#if __BITINT_MAXWIDTH__ >= 512
_BitInt(512) g, h, i, j, k, l;

void
bar (void)
{
  __builtin_add_overflow (g, h, &g);
  __builtin_sub_overflow (i, j, &i);
  __builtin_mul_overflow (k, l, &k);
}
#endif

_BitInt(32) m, n, o, p, q, r;

void
baz (void)
{
  __builtin_add_overflow (m, n, &m);
  __builtin_sub_overflow (o, p, &o);
  __builtin_mul_overflow (q, r, &q);
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
// DEFAULT-NEXT:     global %0 a: i1024b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i1024b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i1024b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i1024b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: i1024b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 f: i1024b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 g: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 h: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 i: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 j: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 k: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 l: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 m: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 n: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 o: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 p: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 q: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 r: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         overflow_add<bool>(read<i1024b>(%0), read<i1024b>(%1), deref(addr_of<ptr<i1024b>>(%0)));
// DEFAULT-NEXT:         overflow_sub<bool>(read<i1024b>(%2), read<i1024b>(%3), deref(addr_of<ptr<i1024b>>(%2)));
// DEFAULT-NEXT:         overflow_mul<bool>(read<i1024b>(%4), read<i1024b>(%5), deref(addr_of<ptr<i1024b>>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         overflow_add<bool>(read<i512b>(%7), read<i512b>(%8), deref(addr_of<ptr<i512b>>(%7)));
// DEFAULT-NEXT:         overflow_sub<bool>(read<i512b>(%9), read<i512b>(%10), deref(addr_of<ptr<i512b>>(%9)));
// DEFAULT-NEXT:         overflow_mul<bool>(read<i512b>(%11), read<i512b>(%12), deref(addr_of<ptr<i512b>>(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @baz() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         overflow_add<bool>(read<i32b>(%14), read<i32b>(%15), deref(addr_of<ptr<i32b>>(%14)));
// DEFAULT-NEXT:         overflow_sub<bool>(read<i32b>(%16), read<i32b>(%17), deref(addr_of<ptr<i32b>>(%16)));
// DEFAULT-NEXT:         overflow_mul<bool>(read<i32b>(%18), read<i32b>(%19), deref(addr_of<ptr<i32b>>(%18)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
