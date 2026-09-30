/* PR tree-optimization/113783 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-mavx512f" { target i?86-*-* x86_64-*-* } } */

int i;

#if __BITINT_MAXWIDTH__ >= 246
void
foo (void *p, _BitInt(246) x)
{
  __builtin_memcpy (p, &x, sizeof x);
}

_BitInt(246)
bar (void *p, _BitInt(246) x)
{
  _BitInt(246) y = x + 1;
  __builtin_memcpy (p, &y, sizeof y);
  return x;
}
#endif

#if __BITINT_MAXWIDTH__ >= 502
void
baz (void *p, _BitInt(502) x)
{
  __builtin_memcpy (p, &x, sizeof x);
}

_BitInt(502)
qux (void *p, _BitInt(502) x)
{
  _BitInt(502) y = x + 1;
  __builtin_memcpy (p, &y, sizeof y);
  return x;
}
#endif

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
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<void>, %[[VALUE_x:[0-9]+]] x: i246b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_p]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i246b>>(%[[VALUE_x]])), const<u64>(32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p_2:[0-9]+]] p: ptr<void>, %[[VALUE_x_2:[0-9]+]] x: i246b) -> i246b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i246b [storage=automatic] = add<i246b, overflow=ub>(read<i246b>(%[[VALUE_x_2]]), widen<i246b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_p_2]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i246b>>(%[[VALUE_y]])), const<u64>(32));
// DEFAULT-NEXT:         return read<i246b>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_p_3:[0-9]+]] p: ptr<void>, %[[VALUE_x_3:[0-9]+]] x: i502b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_p_3]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i502b>>(%[[VALUE_x_3]])), const<u64>(64));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qux:[0-9]+]] @qux(%[[VALUE_p_4:[0-9]+]] p: ptr<void>, %[[VALUE_x_4:[0-9]+]] x: i502b) -> i502b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: i502b [storage=automatic] = add<i502b, overflow=ub>(read<i502b>(%[[VALUE_x_4]]), widen<i502b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_p_4]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i502b>>(%[[VALUE_y_2]])), const<u64>(64));
// DEFAULT-NEXT:         return read<i502b>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
