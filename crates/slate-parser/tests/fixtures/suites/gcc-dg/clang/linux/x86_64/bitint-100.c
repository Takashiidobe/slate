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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         read<i575b>(%[[VALUE_x]]);
// DEFAULT-NEXT:         return widen<i325b, reason=return>(const<i2b>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_garply:[0-9]+]] @garply(%[[VALUE_x_2:[0-9]+]] x: i575b, %[[VALUE_y:[0-9]+]] y: i575b, %[[VALUE_z:[0-9]+]] z: i575b, %[[VALUE_u:[0-9]+]] u: i32, %[[VALUE_v:[0-9]+]] v: i32, %[[VALUE_w:[0-9]+]] w: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         read<i575b>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:         read<i575b>(%[[VALUE_y]]);
// DEFAULT-NEXT:         read<i575b>(%[[VALUE_z]]);
// DEFAULT-NEXT:         read<i32>(%[[VALUE_u]]);
// DEFAULT-NEXT:         read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         read<i575b>(%[[VALUE_w]]);
// DEFAULT-NEXT:         return widen<i325b, reason=return>(const<i2b>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_y_2:[0-9]+]] y: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(1));
// DEFAULT-NEXT:         return call<i325b, signature=fn(i575b) -> i325b>(%[[VALUE_bar]], read<i575b>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qux:[0-9]+]] @qux(%[[VALUE_x_3:[0-9]+]] x: i32, %[[VALUE_y_3:[0-9]+]] y: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(25))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_3]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(2)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(42))
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x_3]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(3))));
// DEFAULT-NEXT:         return call<i325b, signature=fn(i575b) -> i325b>(%[[VALUE_bar]], read<i575b>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_corge:[0-9]+]] @corge(%[[VALUE_x_4:[0-9]+]] x: i32, %[[VALUE_y_4:[0-9]+]] y: i575b, %[[VALUE_z_2:[0-9]+]] z: ptr<i325b>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: array<ptr<void>, 4> [storage=automatic] [align=16] = aggregate<array<ptr<void>, 4>, zero_fill=false>(index0 = label_addr<ptr<void>>(%[[VALUE_l1:[0-9]+]]), index1 = label_addr<ptr<void>>(%[[VALUE_l2:[0-9]+]]), index2 = label_addr<ptr<void>>(%[[VALUE_l3:[0-9]+]]), index3 = label_addr<ptr<void>>(%[[VALUE_l3]]));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_x_4]]), const<i32>(25))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %[[VALUE_l1]] l1:
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_x_4]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(2)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%[[VALUE_x_4]]), const<i32>(42))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     label %[[VALUE_l2]] l2:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_x_4]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(3))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         label %[[VALUE_l3]] l3:
// DEFAULT-NEXT:             write<i325b>(deref(read<ptr<i325b>>(%[[VALUE_z_2]])), call<i325b, signature=fn(i575b) -> i325b>(%[[VALUE_bar]], read<i575b>(%[[VALUE_y_4]])));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_x_4]]), const<i32>(4))
// DEFAULT-NEXT:             goto *read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(4)>(%[[VALUE_q]]), and<i32>(read<i32>(%[[VALUE_x_4]]), const<i32>(3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_freddy:[0-9]+]] @freddy(%[[VALUE_x_5:[0-9]+]] x: i32, %[[VALUE_y_5:[0-9]+]] y: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i325b, signature=fn(i575b) -> i325b>(%[[VALUE_bar]], read<i575b>(%[[VALUE_y_5]]));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i575b [synthetic] = read<i575b>(%[[VALUE_y_5]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i575b [synthetic] = add<i575b, overflow=ub>(read<i575b>(%[[VALUE1]]), widen<i575b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i575b>(%[[VALUE_y_5]], read<i575b>(%[[VALUE2]]));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_x_5]]), const<i32>(25))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_5]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(2)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%[[VALUE_x_5]]), const<i32>(42))
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x_5]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(3))));
// DEFAULT-NEXT:         return call<i325b, signature=fn(i575b) -> i325b>(%[[VALUE_bar]], read<i575b>(%[[VALUE_y_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_quux:[0-9]+]] @quux(%[[VALUE_x_6:[0-9]+]] x: i575b, %[[VALUE_y_6:[0-9]+]] y: i575b, %[[VALUE_z_3:[0-9]+]] z: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_w_2:[0-9]+]] w: i575b [storage=automatic] = add<i575b, overflow=ub>(read<i575b>(%[[VALUE_x_6]]), read<i575b>(%[[VALUE_y_6]]));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(1));
// DEFAULT-NEXT:         return call<i325b, signature=fn(i575b, i575b, i575b, i32, i32, i575b) -> i325b>(%[[VALUE_garply]], read<i575b>(%[[VALUE_x_6]]), read<i575b>(%[[VALUE_y_6]]), read<i575b>(%[[VALUE_z_3]]), const<i32>(42), const<i32>(42), read<i575b>(%[[VALUE_w_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_grault:[0-9]+]] @grault(%[[VALUE_x_7:[0-9]+]] x: i32, %[[VALUE_y_7:[0-9]+]] y: i575b, %[[VALUE_z_4:[0-9]+]] z: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: i575b [storage=automatic] = add<i575b, overflow=ub>(widen<i575b, reason=usual_arith>(read<i32>(%[[VALUE_x_7]])), read<i575b>(%[[VALUE_y_7]]));
// DEFAULT-NEXT:         let %[[VALUE_w_3:[0-9]+]] w: i575b [storage=automatic] = sub<i575b, overflow=ub>(widen<i575b, reason=usual_arith>(read<i32>(%[[VALUE_x_7]])), read<i575b>(%[[VALUE_y_7]]));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_x_7]]), const<i32>(25))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_7]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(2)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%[[VALUE_x_7]]), const<i32>(42))
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x_7]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(3))));
// DEFAULT-NEXT:         return call<i325b, signature=fn(i575b, i575b, i575b, i32, i32, i575b) -> i325b>(%[[VALUE_garply]], read<i575b>(%[[VALUE_y_7]]), read<i575b>(%[[VALUE_z_4]]), read<i575b>(%[[VALUE_v_2]]), const<i32>(0), const<i32>(0), read<i575b>(%[[VALUE_w_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_plugh:[0-9]+]] @plugh(%[[VALUE_x_8:[0-9]+]] x: i32, %[[VALUE_y_8:[0-9]+]] y: i575b, %[[VALUE_z_5:[0-9]+]] z: i575b, %[[VALUE_v_3:[0-9]+]] v: i575b, %[[VALUE_w_4:[0-9]+]] w: i575b) -> i325b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i325b, signature=fn(i575b, i575b, i575b, i32, i32, i575b) -> i325b>(%[[VALUE_garply]], read<i575b>(%[[VALUE_y_8]]), read<i575b>(%[[VALUE_z_5]]), read<i575b>(%[[VALUE_v_3]]), const<i32>(1), const<i32>(2), read<i575b>(%[[VALUE_w_4]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i575b [synthetic] = read<i575b>(%[[VALUE_y_8]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i575b [synthetic] = add<i575b, overflow=ub>(read<i575b>(%[[VALUE3]]), widen<i575b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i575b>(%[[VALUE_y_8]], read<i575b>(%[[VALUE4]]));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i575b [synthetic] = read<i575b>(%[[VALUE_z_5]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i575b [synthetic] = add<i575b, overflow=ub>(read<i575b>(%[[VALUE5]]), widen<i575b, reason=usual_arith>(const<i3b>(2)));
// DEFAULT-NEXT:         write<i575b>(%[[VALUE_z_5]], read<i575b>(%[[VALUE6]]));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i575b [synthetic] = read<i575b>(%[[VALUE_v_3]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i575b [synthetic] = shl<i575b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i575b>(%[[VALUE7]]), const<i32>(3));
// DEFAULT-NEXT:         write<i575b>(%[[VALUE_v_3]], read<i575b>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i575b [synthetic] = read<i575b>(%[[VALUE_w_4]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i575b [synthetic] = mul<i575b, overflow=ub>(read<i575b>(%[[VALUE9]]), widen<i575b, reason=usual_arith>(const<i3b>(3)));
// DEFAULT-NEXT:         write<i575b>(%[[VALUE_w_4]], read<i575b>(%[[VALUE10]]));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_x_8]]), const<i32>(25))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_8]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(2)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%[[VALUE_x_8]]), const<i32>(42))
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x_8]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(3))));
// DEFAULT-NEXT:         return call<i325b, signature=fn(i575b, i575b, i575b, i32, i32, i575b) -> i325b>(%[[VALUE_garply]], read<i575b>(%[[VALUE_y_8]]), read<i575b>(%[[VALUE_z_5]]), read<i575b>(%[[VALUE_v_3]]), const<i32>(1), const<i32>(2), read<i575b>(%[[VALUE_w_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
