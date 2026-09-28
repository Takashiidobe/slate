/* PR tree-optimization/113119 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O2" } */

_BitInt(8) b;
_Bool c;
#if __BITINT_MAXWIDTH__ >= 8445
_BitInt(8445) a;

void
foo (_BitInt(4058) d)
{
  c = __builtin_add_overflow (a, 0ULL, &d);
  __builtin_add_overflow (a, 0ULL, &d);
  b = d;
}
#endif

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
// DEFAULT-NEXT:     global %0 b: i8b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 c: bool [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 a: i8445b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 d: i4058b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<bool>(%1, overflow_add<bool>(read<i8445b>(%2), const<u64>(0), deref(addr_of<ptr<i4058b>>(%4))));
// DEFAULT-NEXT:         overflow_add<bool>(read<i8445b>(%2), const<u64>(0), deref(addr_of<ptr<i4058b>>(%4)));
// DEFAULT-NEXT:         overflow_add<bool>(read<i8445b>(%2), const<u64>(0), deref(addr_of<ptr<i4058b>>(%4)));
// DEFAULT-NEXT:         write<i8b>(%0, truncate<i8b, reason=assign, fits=unknown>(read<i4058b>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
