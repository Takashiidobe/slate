/* PR middle-end/121828 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O2" } */

void baz (int);
#if __BITINT_MAXWIDTH__ >= 255
unsigned _BitInt(255) a;

void
foo (int x, int y)
{
  unsigned _BitInt(255) b;
  int t = __builtin_sub_overflow (y, x, &b);
  baz (t);
  a = b;
}

void
bar (int x, int y)
{
  unsigned _BitInt(255) b;
  bool t = __builtin_sub_overflow (y, x, &b);
  a = b;
  baz (t);
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
// DEFAULT-NEXT:     global %1 a: u255b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @baz(%12 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 x: i32, %4 y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 b: u255b [storage=automatic];
// DEFAULT-NEXT:         let %6 t: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_sub<bool>(read<i32>(%4), read<i32>(%3), deref(addr_of<ptr<u255b>>(%5))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, read<i32>(%6));
// DEFAULT-NEXT:         write<u255b>(%1, read<u255b>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @bar(%8 x: i32, %9 y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 b: u255b [storage=automatic];
// DEFAULT-NEXT:         let %11 t: bool [storage=automatic] = overflow_sub<bool>(read<i32>(%9), read<i32>(%8), deref(addr_of<ptr<u255b>>(%10)));
// DEFAULT-NEXT:         write<u255b>(%1, read<u255b>(%10));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, from_bool<i32, reason=arg>(read<bool>(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
