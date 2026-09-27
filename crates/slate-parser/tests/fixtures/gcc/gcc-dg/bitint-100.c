/* PR tree-optimization/113466 */
/* { dg-do compile { target bitint575 } } */
/* { dg-options "-O2" } */

int foo (int);

__attribute__((returns_twice, noipa)) _BitInt(325)
bar (_BitInt(575) x)
{
  (void) x;
  return 0wb;
}

__attribute__((returns_twice, noipa)) _BitInt(325)
garply (_BitInt(575) x, _BitInt(575) y, _BitInt(575) z, int u, int v, _BitInt(575) w)
{
  (void) x;
  (void) y;
  (void) z;
  (void) u;
  (void) v;
  (void) w;
  return 0wb;
}

_BitInt(325)
baz (_BitInt(575) y)
{
  foo (1);
  return bar (y);
}

_BitInt(325)
qux (int x, _BitInt(575) y)
{
  if (x == 25)
    x = foo (2);
  else if (x == 42)
    x = foo (foo (3));
  return bar (y);
}

void
corge (int x, _BitInt(575) y, _BitInt(325) *z)
{
  void *q[] = { &&l1, &&l2, &&l3, &&l3 };
  if (x == 25)
    {
    l1:
      x = foo (2);
    }
  else if (x == 42)
    {
    l2:
      x = foo (foo (3));
    }
l3:
  *z = bar (y);
  if (x < 4)
    goto *q[x & 3];
}

_BitInt(325)
freddy (int x, _BitInt(575) y)
{
  bar (y);
  ++y;
  if (x == 25)
    x = foo (2);
  else if (x == 42)
    x = foo (foo (3));
  return bar (y);
}

_BitInt(325)
quux (_BitInt(575) x, _BitInt(575) y, _BitInt(575) z)
{
  _BitInt(575) w = x + y;
  foo (1);
  return garply (x, y, z, 42, 42, w);
}

_BitInt(325)
grault (int x, _BitInt(575) y, _BitInt(575) z)
{
  _BitInt(575) v = x + y;
  _BitInt(575) w = x - y;
  if (x == 25)
    x = foo (2);
  else if (x == 42)
    x = foo (foo (3));
  return garply (y, z, v, 0, 0, w);
}

_BitInt(325)
plugh (int x, _BitInt(575) y, _BitInt(575) z, _BitInt(575) v, _BitInt(575) w)
{
  garply (y, z, v, 1, 2, w);
  ++y;
  z += 2wb;
  v <<= 3;
  w *= 3wb;
  if (x == 25)
    x = foo (2);
  else if (x == 42)
    x = foo (foo (3));
  return garply (y, z, v, 1, 2, w);
}

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
// DEFAULT-NEXT:     fn %0 @foo(%43 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar(%2 x: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         read<i575b>(%2);
// DEFAULT-NEXT:         return widen<i325b, reason=return>(const<i2b>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @garply(%4 x: i575b, %5 y: i575b, %6 z: i575b, %7 u: i32, %8 v: i32, %9 w: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         read<i575b>(%4);
// DEFAULT-NEXT:         read<i575b>(%5);
// DEFAULT-NEXT:         read<i575b>(%6);
// DEFAULT-NEXT:         read<i32>(%7);
// DEFAULT-NEXT:         read<i32>(%8);
// DEFAULT-NEXT:         read<i575b>(%9);
// DEFAULT-NEXT:         return widen<i325b, reason=return>(const<i2b>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @baz(%11 y: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%0, const<i32>(1));
// DEFAULT-NEXT:         return call<i325b, signature=fn(i575b) -> i325b>(%1, read<i575b>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @qux(%13 x: i32, %14 y: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%13), const<i32>(25))
// DEFAULT-NEXT:             write<i32>(%13, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(2)));
// DEFAULT-NEXT:             call<i32, signature=fn(i32) -> i32>(%0, const<i32>(2));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%13), const<i32>(42))
// DEFAULT-NEXT:                 write<i32>(%13, call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3))));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3)));
// DEFAULT-NEXT:         return call<i325b, signature=fn(i575b) -> i325b>(%1, read<i575b>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @corge(%19 x: i32, %20 y: i575b, %21 z: ptr<i325b>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %22 q: array<ptr<void>, 4> [storage=automatic] [align=16] = aggregate<array<ptr<void>, 4>, zero_fill=false>(index0 = label_addr<ptr<void>>(%16), index1 = label_addr<ptr<void>>(%17), index2 = label_addr<ptr<void>>(%18), index3 = label_addr<ptr<void>>(%18));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%19), const<i32>(25))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %16 l1:
// DEFAULT-NEXT:                     write<i32>(%19, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(2)));
// DEFAULT-NEXT:                     call<i32, signature=fn(i32) -> i32>(%0, const<i32>(2));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%19), const<i32>(42))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     label %17 l2:
// DEFAULT-NEXT:                         write<i32>(%19, call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3))));
// DEFAULT-NEXT:                         call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         label %18 l3:
// DEFAULT-NEXT:             write<i325b>(deref(read<ptr<i325b>>(%21)), call<i325b, signature=fn(i575b) -> i325b>(%1, read<i575b>(%20)));
// DEFAULT-NEXT:             call<i325b, signature=fn(i575b) -> i325b>(%1, read<i575b>(%20));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%19), const<i32>(4))
// DEFAULT-NEXT:             goto *read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(4)>(%22), and<i32>(read<i32>(%19), const<i32>(3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @freddy(%24 x: i32, %25 y: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i325b, signature=fn(i575b) -> i325b>(%1, read<i575b>(%25));
// DEFAULT-NEXT:         let %44: i575b [synthetic] = read<i575b>(%25);
// DEFAULT-NEXT:         let %45: i575b [synthetic] = add<i575b, overflow=ub>(read<i575b>(%44), widen<i575b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i575b>(%25, read<i575b>(%45));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%24), const<i32>(25))
// DEFAULT-NEXT:             write<i32>(%24, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(2)));
// DEFAULT-NEXT:             call<i32, signature=fn(i32) -> i32>(%0, const<i32>(2));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%24), const<i32>(42))
// DEFAULT-NEXT:                 write<i32>(%24, call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3))));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3)));
// DEFAULT-NEXT:         return call<i325b, signature=fn(i575b) -> i325b>(%1, read<i575b>(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @quux(%27 x: i575b, %28 y: i575b, %29 z: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %30 w: i575b [storage=automatic] = add<i575b, overflow=ub>(read<i575b>(%27), read<i575b>(%28));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%0, const<i32>(1));
// DEFAULT-NEXT:         return call<i325b, signature=fn(i575b, i575b, i575b, i32, i32, i575b) -> i325b>(%3, read<i575b>(%27), read<i575b>(%28), read<i575b>(%29), const<i32>(42), const<i32>(42), read<i575b>(%30));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @grault(%32 x: i32, %33 y: i575b, %34 z: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %35 v: i575b [storage=automatic] = add<i575b, overflow=ub>(widen<i575b, reason=usual_arith>(read<i32>(%32)), read<i575b>(%33));
// DEFAULT-NEXT:         let %36 w: i575b [storage=automatic] = sub<i575b, overflow=ub>(widen<i575b, reason=usual_arith>(read<i32>(%32)), read<i575b>(%33));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%32), const<i32>(25))
// DEFAULT-NEXT:             write<i32>(%32, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(2)));
// DEFAULT-NEXT:             call<i32, signature=fn(i32) -> i32>(%0, const<i32>(2));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%32), const<i32>(42))
// DEFAULT-NEXT:                 write<i32>(%32, call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3))));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3)));
// DEFAULT-NEXT:         return call<i325b, signature=fn(i575b, i575b, i575b, i32, i32, i575b) -> i325b>(%3, read<i575b>(%33), read<i575b>(%34), read<i575b>(%35), const<i32>(0), const<i32>(0), read<i575b>(%36));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @plugh(%38 x: i32, %39 y: i575b, %40 z: i575b, %41 v: i575b, %42 w: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i325b, signature=fn(i575b, i575b, i575b, i32, i32, i575b) -> i325b>(%3, read<i575b>(%39), read<i575b>(%40), read<i575b>(%41), const<i32>(1), const<i32>(2), read<i575b>(%42));
// DEFAULT-NEXT:         let %46: i575b [synthetic] = read<i575b>(%39);
// DEFAULT-NEXT:         let %47: i575b [synthetic] = add<i575b, overflow=ub>(read<i575b>(%46), widen<i575b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i575b>(%39, read<i575b>(%47));
// DEFAULT-NEXT:         let %48: i575b [synthetic] = read<i575b>(%40);
// DEFAULT-NEXT:         let %49: i575b [synthetic] = add<i575b, overflow=ub>(read<i575b>(%48), widen<i575b, reason=usual_arith>(const<i3b>(2)));
// DEFAULT-NEXT:         write<i575b>(%40, read<i575b>(%49));
// DEFAULT-NEXT:         let %50: i575b [synthetic] = read<i575b>(%41);
// DEFAULT-NEXT:         let %51: i575b [synthetic] = shl<i575b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i575b>(%50), const<i32>(3));
// DEFAULT-NEXT:         write<i575b>(%41, read<i575b>(%51));
// DEFAULT-NEXT:         let %52: i575b [synthetic] = read<i575b>(%42);
// DEFAULT-NEXT:         let %53: i575b [synthetic] = mul<i575b, overflow=ub>(read<i575b>(%52), widen<i575b, reason=usual_arith>(const<i3b>(3)));
// DEFAULT-NEXT:         write<i575b>(%42, read<i575b>(%53));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%38), const<i32>(25))
// DEFAULT-NEXT:             write<i32>(%38, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(2)));
// DEFAULT-NEXT:             call<i32, signature=fn(i32) -> i32>(%0, const<i32>(2));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%38), const<i32>(42))
// DEFAULT-NEXT:                 write<i32>(%38, call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3))));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3)));
// DEFAULT-NEXT:         return call<i325b, signature=fn(i575b, i575b, i575b, i32, i32, i575b) -> i325b>(%3, read<i575b>(%39), read<i575b>(%40), read<i575b>(%41), const<i32>(1), const<i32>(2), read<i575b>(%42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
