/* Test for GNU extensions to C99 designated initializers */
/* Origin: Jakub Jelinek <jakub@redhat.com> */
/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

typedef __SIZE_TYPE__ size_t;
extern int memcmp (const void *, const void *, size_t);
extern void abort (void);
extern void exit (int);

int a[][2][4] = { [2 ... 4][0 ... 1][2 ... 3] = 1, [2] = 2, [2][0][2] = 3 };
struct E {};
struct F { struct E H; };
struct G { int I; struct E J; int K; };
struct H { int I; struct F J; int K; };
struct G k = { .J = {}, 1 };
struct H l = { .J.H = {}, 2 };
struct H m = { .J = {}, 3 };
struct I { int J; int K[3]; int L; };
struct M { int N; struct I O[3]; int P; };
struct M n[] = { [0 ... 5].O[1 ... 2].K[0 ... 1] = 4, 5, 6, 7 };
struct M o[] = { [0 ... 5].O = { [1 ... 2].K[0 ... 1] = 4 },
		 [5].O[2].K[2] = 5, 6, 7 };
struct M p[] = { [0 ... 5].O[1 ... 2].K = { [0 ... 1] = 4 },
		 [5].O[2].K[2] = 5, 6, 7 };
int q[3][3] = { [0 ... 1] = { [1 ... 2] = 23 }, [1][2] = 24 };
int r[1] = { [0 ... 1 - 1] = 27 };

int main (void)
{
  int x, y, z;

  if (a[2][0][0] != 2 || a[2][0][2] != 3)
    abort ();
  a[2][0][0] = 0;
  a[2][0][2] = 1;
  for (x = 0; x <= 4; x++)
    for (y = 0; y <= 1; y++)
      for (z = 0; z <= 3; z++)
	if (a[x][y][z] != (x >= 2 && z >= 2))
	  abort ();
  if (k.I || l.I || m.I || k.K != 1 || l.K != 2 || m.K != 3)
    abort ();
  for (x = 0; x <= 5; x++)
    {
      if (n[x].N || n[x].O[0].J || n[x].O[0].L)
	abort ();
      for (y = 0; y <= 2; y++)
	if (n[x].O[0].K[y])
	  abort ();
      for (y = 1; y <= 2; y++)
	{
	  if (n[x].O[y].J)
	    abort ();
	  if (n[x].O[y].K[0] != 4)
	    abort ();
	  if (n[x].O[y].K[1] != 4)
	    abort ();
	  if ((x < 5 || y < 2) && (n[x].O[y].K[2] || n[x].O[y].L))
	    abort ();
	}
      if (x < 5 && n[x].P)
	abort ();
    }
  if (n[5].O[2].K[2] != 5 || n[5].O[2].L != 6 || n[5].P != 7)
    abort ();
  if (memcmp (n, o, sizeof (n)) || sizeof (n) != sizeof (o))
    abort ();
  if (memcmp (n, p, sizeof (n)) || sizeof (n) != sizeof (p))
    abort ();
  if (q[0][0] || q[0][1] != 23 || q[0][2] != 23)
    abort ();
  if (q[1][0] || q[1][1] != 23 || q[1][2] != 24)
    abort ();
  if (q[2][0] || q[2][1] || q[2][2])
    abort ();
  if (r[0] != 27)
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_E:[0-9]+]] E = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_F:[0-9]+]] F = struct {
// DEFAULT-NEXT:         field0 H: @type[[TYPE_E]];
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_G:[0-9]+]] G = struct {
// DEFAULT-NEXT:         field0 I: i32;
// DEFAULT-NEXT:         field1 J: @type[[TYPE_E]];
// DEFAULT-NEXT:         field2 K: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_H:[0-9]+]] H = struct {
// DEFAULT-NEXT:         field0 I: i32;
// DEFAULT-NEXT:         field1 J: @type[[TYPE_F]];
// DEFAULT-NEXT:         field2 K: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_I:[0-9]+]] I = struct {
// DEFAULT-NEXT:         field0 J: i32;
// DEFAULT-NEXT:         field1 K: array<i32, 3>;
// DEFAULT-NEXT:         field2 L: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_M:[0-9]+]] M = struct {
// DEFAULT-NEXT:         field0 N: i32;
// DEFAULT-NEXT:         field1 O: array<@type[[TYPE_I]], 3>;
// DEFAULT-NEXT:         field2 P: i32;
// DEFAULT-NEXT:     } [size=68, align=4, offsets=[0, 4, 64]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<array<array<i32, 4>, 2>, 5> [storage=static] [align=16] = aggregate<array<array<array<i32, 4>, 2>, 5>, zero_fill=true>(index2 = aggregate<array<array<i32, 4>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 4>, zero_fill=true>(index0 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(1)), index1 = aggregate<array<i32, 4>, zero_fill=true>(index2..=3 = const<i32>(1))), index3 = aggregate<array<array<i32, 4>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 4>, zero_fill=true>(index2..=3 = const<i32>(1)), index1 = aggregate<array<i32, 4>, zero_fill=true>(index2..=3 = const<i32>(1))), index4 = aggregate<array<array<i32, 4>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 4>, zero_fill=true>(index2..=3 = const<i32>(1)), index1 = aggregate<array<i32, 4>, zero_fill=true>(index2..=3 = const<i32>(1)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: @type[[TYPE_G]] [storage=static] = aggregate<@type[[TYPE_G]], zero_fill=true>(field1 = aggregate<@type[[TYPE_E]], zero_fill=false>(), field2 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: @type[[TYPE_H]] [storage=static] = aggregate<@type[[TYPE_H]], zero_fill=true>(field1 = aggregate<@type[[TYPE_F]], zero_fill=false>(field0 = aggregate<@type[[TYPE_E]], zero_fill=false>()), field2 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: @type[[TYPE_H]] [storage=static] = aggregate<@type[[TYPE_H]], zero_fill=true>(field1 = aggregate<@type[[TYPE_F]], zero_fill=true>(), field2 = const<i32>(3)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: array<@type[[TYPE_M]], 6> [storage=static] [align=16] = aggregate<array<@type[[TYPE_M]], 6>, zero_fill=false>(index0..=4 = aggregate<@type[[TYPE_M]], zero_fill=true>(field1 = aggregate<array<@type[[TYPE_I]], 3>, zero_fill=true>(index1 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))))), index5 = aggregate<@type[[TYPE_M]], zero_fill=true>(field1 = aggregate<array<@type[[TYPE_I]], 3>, zero_fill=true>(index1 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=false>(index0..=1 = const<i32>(4), index2 = const<i32>(5)), field2 = const<i32>(6))), field2 = const<i32>(7))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: array<@type[[TYPE_M]], 6> [storage=static] [align=16] = aggregate<array<@type[[TYPE_M]], 6>, zero_fill=false>(index0..=4 = aggregate<@type[[TYPE_M]], zero_fill=true>(field1 = aggregate<array<@type[[TYPE_I]], 3>, zero_fill=true>(index1 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))))), index5 = aggregate<@type[[TYPE_M]], zero_fill=true>(field1 = aggregate<array<@type[[TYPE_I]], 3>, zero_fill=true>(index1 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=false>(index0..=1 = const<i32>(4), index2 = const<i32>(5)), field2 = const<i32>(6))), field2 = const<i32>(7))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: array<@type[[TYPE_M]], 6> [storage=static] [align=16] = aggregate<array<@type[[TYPE_M]], 6>, zero_fill=false>(index0..=4 = aggregate<@type[[TYPE_M]], zero_fill=true>(field1 = aggregate<array<@type[[TYPE_I]], 3>, zero_fill=true>(index1 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))))), index5 = aggregate<@type[[TYPE_M]], zero_fill=true>(field1 = aggregate<array<@type[[TYPE_I]], 3>, zero_fill=true>(index1 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=false>(index0..=1 = const<i32>(4), index2 = const<i32>(5)), field2 = const<i32>(6))), field2 = const<i32>(7))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: array<array<i32, 3>, 3> [storage=static] [align=16] = aggregate<array<array<i32, 3>, 3>, zero_fill=true>(index0 = aggregate<array<i32, 3>, zero_fill=true>(index1..=2 = const<i32>(23)), index1 = aggregate<array<i32, 3>, zero_fill=true>(index1 = const<i32>(23), index2 = const<i32>(24))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_r:[0-9]+]] r: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(27)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE3:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: i32 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<i32, 4>, 2>>, subtract=false, element=array<array<i32, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 2>>, length=Some(5)>(%[[VALUE_a]]), const<i32>(2)))), const<i32>(0)))), const<i32>(0)))), const<i32>(2)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<i32, 4>, 2>>, subtract=false, element=array<array<i32, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 2>>, length=Some(5)>(%[[VALUE_a]]), const<i32>(2)))), const<i32>(0)))), const<i32>(2)))), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<i32, 4>, 2>>, subtract=false, element=array<array<i32, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 2>>, length=Some(5)>(%[[VALUE_a]]), const<i32>(2)))), const<i32>(0)))), const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<i32, 4>, 2>>, subtract=false, element=array<array<i32, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 2>>, length=Some(5)>(%[[VALUE_a]]), const<i32>(2)))), const<i32>(0)))), const<i32>(2))), const<i32>(1));
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], const<i32>(0));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_x]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y]], const<i32>(0));
// DEFAULT-NEXT:                     condition: le<i32>(read<i32>(%[[VALUE_y]]), const<i32>(1))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:                         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_z]], const<i32>(0));
// DEFAULT-NEXT:                             condition: le<i32>(read<i32>(%[[VALUE_z]]), const<i32>(3))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_z]]);
// DEFAULT-NEXT:                                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_z]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<i32, 4>, 2>>, subtract=false, element=array<array<i32, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 2>>, length=Some(5)>(%[[VALUE_a]]), read<i32>(%[[VALUE_x]])))), read<i32>(%[[VALUE_y]])))), read<i32>(%[[VALUE_z]])))), from_bool<i32, reason=promotion>(logical_and<bool>(ge<i32>(read<i32>(%[[VALUE_x]]), const<i32>(2)), ge<i32>(read<i32>(%[[VALUE_z]]), const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_k]])), const<i32>(0)), ne<i32>(read<i32>(field0(%[[VALUE_l]])), const<i32>(0))), ne<i32>(read<i32>(field0(%[[VALUE_m]])), const<i32>(0))), ne<i32>(read<i32>(field2(%[[VALUE_k]])), const<i32>(1))), ne<i32>(read<i32>(field2(%[[VALUE_l]])), const<i32>(2))), ne<i32>(read<i32>(field2(%[[VALUE_m]])), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], const<i32>(0));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_x]]), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), read<i32>(%[[VALUE_x]]))))), const<i32>(0)), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_I]]>, subtract=false, element=@type[[TYPE_I]], overflow=ub>(array_decay<ptr<@type[[TYPE_I]]>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), read<i32>(%[[VALUE_x]]))))), const<i32>(0))))), const<i32>(0))), ne<i32>(read<i32>(field2(deref(ptr_offset<ptr<@type[[TYPE_I]]>, subtract=false, element=@type[[TYPE_I]], overflow=ub>(array_decay<ptr<@type[[TYPE_I]]>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), read<i32>(%[[VALUE_x]]))))), const<i32>(0))))), const<i32>(0)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     for %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_y]], const<i32>(0));
// DEFAULT-NEXT:                         condition: le<i32>(read<i32>(%[[VALUE_y]]), const<i32>(2))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE17:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:                             let %[[VALUE18:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE17]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE18]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_I]]>, subtract=false, element=@type[[TYPE_I]], overflow=ub>(array_decay<ptr<@type[[TYPE_I]]>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), read<i32>(%[[VALUE_x]]))))), const<i32>(0))))), read<i32>(%[[VALUE_y]])))), const<i32>(0))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     for %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_y]], const<i32>(1));
// DEFAULT-NEXT:                         condition: le<i32>(read<i32>(%[[VALUE_y]]), const<i32>(2))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:                             let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_I]]>, subtract=false, element=@type[[TYPE_I]], overflow=ub>(array_decay<ptr<@type[[TYPE_I]]>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), read<i32>(%[[VALUE_x]]))))), read<i32>(%[[VALUE_y]]))))), const<i32>(0))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_I]]>, subtract=false, element=@type[[TYPE_I]], overflow=ub>(array_decay<ptr<@type[[TYPE_I]]>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), read<i32>(%[[VALUE_x]]))))), read<i32>(%[[VALUE_y]]))))), const<i32>(0)))), const<i32>(4))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_I]]>, subtract=false, element=@type[[TYPE_I]], overflow=ub>(array_decay<ptr<@type[[TYPE_I]]>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), read<i32>(%[[VALUE_x]]))))), read<i32>(%[[VALUE_y]]))))), const<i32>(1)))), const<i32>(4))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 if logical_and<bool>(logical_or<bool>(lt<i32>(read<i32>(%[[VALUE_x]]), const<i32>(5)), lt<i32>(read<i32>(%[[VALUE_y]]), const<i32>(2))), logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_I]]>, subtract=false, element=@type[[TYPE_I]], overflow=ub>(array_decay<ptr<@type[[TYPE_I]]>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), read<i32>(%[[VALUE_x]]))))), read<i32>(%[[VALUE_y]]))))), const<i32>(2)))), const<i32>(0)), ne<i32>(read<i32>(field2(deref(ptr_offset<ptr<@type[[TYPE_I]]>, subtract=false, element=@type[[TYPE_I]], overflow=ub>(array_decay<ptr<@type[[TYPE_I]]>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), read<i32>(%[[VALUE_x]]))))), read<i32>(%[[VALUE_y]]))))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     if logical_and<bool>(lt<i32>(read<i32>(%[[VALUE_x]]), const<i32>(5)), ne<i32>(read<i32>(field2(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), read<i32>(%[[VALUE_x]]))))), const<i32>(0)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_I]]>, subtract=false, element=@type[[TYPE_I]], overflow=ub>(array_decay<ptr<@type[[TYPE_I]]>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), const<i32>(5))))), const<i32>(2))))), const<i32>(2)))), const<i32>(5)), ne<i32>(read<i32>(field2(deref(ptr_offset<ptr<@type[[TYPE_I]]>, subtract=false, element=@type[[TYPE_I]], overflow=ub>(array_decay<ptr<@type[[TYPE_I]]>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), const<i32>(5))))), const<i32>(2))))), const<i32>(6))), ne<i32>(read<i32>(field2(deref(ptr_offset<ptr<@type[[TYPE_M]]>, subtract=false, element=@type[[TYPE_M]], overflow=ub>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]]), const<i32>(5))))), const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_o]])), const<u64>(408)), const<i32>(0)), ne<u64>(const<u64>(408), const<u64>(408)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_n]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type[[TYPE_M]]>, length=Some(6)>(%[[VALUE_p]])), const<u64>(408)), const<i32>(0)), ne<u64>(const<u64>(408), const<u64>(408)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_q]]), const<i32>(0)))), const<i32>(0)))), const<i32>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_q]]), const<i32>(0)))), const<i32>(1)))), const<i32>(23))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_q]]), const<i32>(0)))), const<i32>(2)))), const<i32>(23)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_q]]), const<i32>(1)))), const<i32>(0)))), const<i32>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_q]]), const<i32>(1)))), const<i32>(1)))), const<i32>(23))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_q]]), const<i32>(1)))), const<i32>(2)))), const<i32>(24)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_q]]), const<i32>(2)))), const<i32>(0)))), const<i32>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_q]]), const<i32>(2)))), const<i32>(1)))), const<i32>(0))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_q]]), const<i32>(2)))), const<i32>(2)))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_r]]), const<i32>(0)))), const<i32>(27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
