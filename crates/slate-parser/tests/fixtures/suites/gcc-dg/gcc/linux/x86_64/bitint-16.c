/* PR c/102989 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -std=c23 -pedantic-errors" } */

_BitInt(15) a;
_BitInt(42) b;
#if __BITINT_MAXWIDTH__ >= 115
_BitInt(115) c;
#endif
#if __BITINT_MAXWIDTH__ >= 192
_BitInt(192) d;
#endif
#if __BITINT_MAXWIDTH__ >= 575
_BitInt(575) e;
#endif

int
main ()
{
  __builtin_clear_padding (&a);
  __builtin_clear_padding (&b);
#if __BITINT_MAXWIDTH__ >= 115
  __builtin_clear_padding (&c);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  __builtin_clear_padding (&d);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  __builtin_clear_padding (&e);
#endif
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i15b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i42b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i115b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i192b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         intrinsic<void, slate.clear_padding>(addr_of<ptr<i15b>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         intrinsic<void, slate.clear_padding>(addr_of<ptr<i42b>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         intrinsic<void, slate.clear_padding>(addr_of<ptr<i115b>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         intrinsic<void, slate.clear_padding>(addr_of<ptr<i192b>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         intrinsic<void, slate.clear_padding>(addr_of<ptr<i575b>>(%[[VALUE_e]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
