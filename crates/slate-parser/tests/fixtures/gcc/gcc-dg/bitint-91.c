/* PR tree-optimization/113988 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-mavx512f" { target i?86-*-* x86_64-*-* } } */

int i;

#if __BITINT_MAXWIDTH__ >= 256
void
foo (void *p, _BitInt(256) x)
{
  __builtin_memcpy (p, &x, sizeof x);
}

_BitInt(256)
bar (void *p, _BitInt(256) x)
{
  _BitInt(246) y = x + 1;
  __builtin_memcpy (p, &y, sizeof y);
  return x;
}
#endif

#if __BITINT_MAXWIDTH__ >= 512
void
baz (void *p, _BitInt(512) x)
{
  __builtin_memcpy (p, &x, sizeof x);
}

_BitInt(512)
qux (void *p, _BitInt(512) x)
{
  _BitInt(512) y = x + 1;
  __builtin_memcpy (p, &y, sizeof y);
  return x;
}
#endif

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %0 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %18 @__builtin_memcpy(%15 <unnamed>: ptr<void>, %16 <unnamed>: ptr<const void>, %17 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 p: ptr<void>, %3 x: i256b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%18, read<ptr<void>>(%2), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i256b>>(%3)), const<u64>(32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%5 p: ptr<void>, %6 x: i256b) -> i256b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 y: i246b [storage=automatic] = truncate<i246b, reason=assign, fits=unknown>(add<i256b, overflow=ub>(read<i256b>(%6), widen<i256b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%18, read<ptr<void>>(%5), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i246b>>(%7)), const<u64>(32));
// DEFAULT-NEXT:         return read<i256b>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @baz(%9 p: ptr<void>, %10 x: i512b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%18, read<ptr<void>>(%9), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i512b>>(%10)), const<u64>(64));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @qux(%12 p: ptr<void>, %13 x: i512b) -> i512b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 y: i512b [storage=automatic] = add<i512b, overflow=ub>(read<i512b>(%13), widen<i512b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%18, read<ptr<void>>(%12), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i512b>>(%14)), const<u64>(64));
// DEFAULT-NEXT:         return read<i512b>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
