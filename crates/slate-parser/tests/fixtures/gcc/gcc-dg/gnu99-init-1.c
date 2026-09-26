/* Test for GNU extensions to C99 designated initializers */
/* Origin: Jakub Jelinek <jakub@redhat.com> */
/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

typedef __SIZE_TYPE__ size_t;
extern int            memcmp(const void *, const void *, size_t);
extern void           abort(void);
extern void           exit(int);

int a[][2][4] = {[2 ... 4][0 ... 1][2 ... 3] = 1, [2] = 2, [2][0][2] = 3};
struct E {};
struct F {
  struct E H;
};
struct G {
  int      I;
  struct E J;
  int      K;
};
struct H {
  int      I;
  struct F J;
  int      K;
};
struct G k = {.J = {}, 1};
struct H l = {.J.H = {}, 2};
struct H m = {.J = {}, 3};
struct I {
  int J;
  int K[3];
  int L;
};
struct M {
  int      N;
  struct I O[3];
  int      P;
};
struct M n[] = {[0 ... 5].O[1 ... 2].K[0 ... 1] = 4, 5, 6, 7};
struct M o[] = {
    [0 ... 5].O = {[1 ... 2].K[0 ... 1] = 4}, [5].O[2].K[2] = 5, 6, 7};
struct M p[] = {
    [0 ... 5].O[1 ... 2].K = {[0 ... 1] = 4}, [5].O[2].K[2] = 5, 6, 7};
int q[3][3] = {[0 ... 1] = {[1 ... 2] = 23}, [1][2] = 24};
int r[1]    = {[0 ... 1 - 1] = 27};

int main(void) {
  int x, y, z;

  if (a[2][0][0] != 2 || a[2][0][2] != 3)
    abort();
  a[2][0][0] = 0;
  a[2][0][2] = 1;
  for (x = 0; x <= 4; x++)
    for (y = 0; y <= 1; y++)
      for (z = 0; z <= 3; z++)
        if (a[x][y][z] != (x >= 2 && z >= 2))
          abort();
  if (k.I || l.I || m.I || k.K != 1 || l.K != 2 || m.K != 3)
    abort();
  for (x = 0; x <= 5; x++) {
    if (n[x].N || n[x].O[0].J || n[x].O[0].L)
      abort();
    for (y = 0; y <= 2; y++)
      if (n[x].O[0].K[y])
        abort();
    for (y = 1; y <= 2; y++) {
      if (n[x].O[y].J)
        abort();
      if (n[x].O[y].K[0] != 4)
        abort();
      if (n[x].O[y].K[1] != 4)
        abort();
      if ((x < 5 || y < 2) && (n[x].O[y].K[2] || n[x].O[y].L))
        abort();
    }
    if (x < 5 && n[x].P)
      abort();
  }
  if (n[5].O[2].K[2] != 5 || n[5].O[2].L != 6 || n[5].P != 7)
    abort();
  if (memcmp(n, o, sizeof(n)) || sizeof(n) != sizeof(o))
    abort();
  if (memcmp(n, p, sizeof(n)) || sizeof(n) != sizeof(p))
    abort();
  if (q[0][0] || q[0][1] != 23 || q[0][2] != 23)
    abort();
  if (q[1][0] || q[1][1] != 23 || q[1][2] != 24)
    abort();
  if (q[2][0] || q[2][1] || q[2][2])
    abort();
  if (r[0] != 27)
    abort();
  exit(0);
}



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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 E = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type2 F = struct {
// DEFAULT-NEXT:         field0 H: @type1;
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type3 G = struct {
// DEFAULT-NEXT:         field0 I: i32;
// DEFAULT-NEXT:         field1 J: @type1;
// DEFAULT-NEXT:         field2 K: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 4]];
// DEFAULT-NEXT:     type @type4 H = struct {
// DEFAULT-NEXT:         field0 I: i32;
// DEFAULT-NEXT:         field1 J: @type2;
// DEFAULT-NEXT:         field2 K: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 4]];
// DEFAULT-NEXT:     type @type5 I = struct {
// DEFAULT-NEXT:         field0 J: i32;
// DEFAULT-NEXT:         field1 K: array<i32, 3>;
// DEFAULT-NEXT:         field2 L: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 16]];
// DEFAULT-NEXT:     type @type6 M = struct {
// DEFAULT-NEXT:         field0 N: i32;
// DEFAULT-NEXT:         field1 O: array<@type5, 3>;
// DEFAULT-NEXT:         field2 P: i32;
// DEFAULT-NEXT:     } [size=68, align=4, offsets=[0, 4, 64]];
// DEFAULT-NEXT:     global %4 a: array<array<array<i32, 4>, 2>, 5> [storage=static] [align=16] = aggregate<array<array<array<i32, 4>, 2>, 5>, zero_fill=true>(index2 = aggregate<array<array<i32, 4>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 4>, zero_fill=true>(index0 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(1)), index1 = aggregate<array<i32, 4>, zero_fill=true>(index2..=3 = const<i32>(1))), index3 = aggregate<array<array<i32, 4>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 4>, zero_fill=true>(index2..=3 = const<i32>(1)), index1 = aggregate<array<i32, 4>, zero_fill=true>(index2..=3 = const<i32>(1))), index4 = aggregate<array<array<i32, 4>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 4>, zero_fill=true>(index2..=3 = const<i32>(1)), index1 = aggregate<array<i32, 4>, zero_fill=true>(index2..=3 = const<i32>(1)))) [linkage=external];
// DEFAULT-NEXT:     global %9 k: @type3 [storage=static] = aggregate<@type3, zero_fill=true>(field1 = aggregate<@type1, zero_fill=false>(), field2 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %10 l: @type4 [storage=static] = aggregate<@type4, zero_fill=true>(field1 = aggregate<@type2, zero_fill=false>(field0 = aggregate<@type1, zero_fill=false>()), field2 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %11 m: @type4 [storage=static] = aggregate<@type4, zero_fill=true>(field1 = aggregate<@type2, zero_fill=true>(), field2 = const<i32>(3)) [linkage=external];
// DEFAULT-NEXT:     global %14 n: array<@type6, 6> [storage=static] [align=16] = aggregate<array<@type6, 6>, zero_fill=false>(index0..=4 = aggregate<@type6, zero_fill=true>(field1 = aggregate<array<@type5, 3>, zero_fill=true>(index1 = aggregate<@type5, zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type5, zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))))), index5 = aggregate<@type6, zero_fill=true>(field1 = aggregate<array<@type5, 3>, zero_fill=true>(index1 = aggregate<@type5, zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type5, zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=false>(index0..=1 = const<i32>(4), index2 = const<i32>(5)), field2 = const<i32>(6))), field2 = const<i32>(7))) [linkage=external];
// DEFAULT-NEXT:     global %15 o: array<@type6, 6> [storage=static] [align=16] = aggregate<array<@type6, 6>, zero_fill=false>(index0..=4 = aggregate<@type6, zero_fill=true>(field1 = aggregate<array<@type5, 3>, zero_fill=true>(index1 = aggregate<@type5, zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type5, zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))))), index5 = aggregate<@type6, zero_fill=true>(field1 = aggregate<array<@type5, 3>, zero_fill=true>(index1 = aggregate<@type5, zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type5, zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=false>(index0..=1 = const<i32>(4), index2 = const<i32>(5)), field2 = const<i32>(6))), field2 = const<i32>(7))) [linkage=external];
// DEFAULT-NEXT:     global %16 p: array<@type6, 6> [storage=static] [align=16] = aggregate<array<@type6, 6>, zero_fill=false>(index0..=4 = aggregate<@type6, zero_fill=true>(field1 = aggregate<array<@type5, 3>, zero_fill=true>(index1 = aggregate<@type5, zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type5, zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))))), index5 = aggregate<@type6, zero_fill=true>(field1 = aggregate<array<@type5, 3>, zero_fill=true>(index1 = aggregate<@type5, zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type5, zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=false>(index0..=1 = const<i32>(4), index2 = const<i32>(5)), field2 = const<i32>(6))), field2 = const<i32>(7))) [linkage=external];
// DEFAULT-NEXT:     global %17 q: array<array<i32, 3>, 3> [storage=static] [align=16] = aggregate<array<array<i32, 3>, 3>, zero_fill=true>(index0 = aggregate<array<i32, 3>, zero_fill=true>(index1..=2 = const<i32>(23)), index1 = aggregate<array<i32, 3>, zero_fill=true>(index1 = const<i32>(23), index2 = const<i32>(24))) [linkage=external];
// DEFAULT-NEXT:     global %18 r: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(27)) [linkage=external];
// DEFAULT-NEXT:     fn %1 @memcmp(%23 <unnamed>: ptr<const void>, %24 <unnamed>: ptr<const void>, %25 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @exit(%26 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %20 x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %21 y: i32 [storage=automatic];
// DEFAULT-NEXT:         let %22 z: i32 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<i32, 4>, 2>>, subtract=false, element=array<array<i32, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 2>>, length=Some(5)>(%4), const<i32>(2)))), const<i32>(0)))), const<i32>(0)))), const<i32>(2)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<i32, 4>, 2>>, subtract=false, element=array<array<i32, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 2>>, length=Some(5)>(%4), const<i32>(2)))), const<i32>(0)))), const<i32>(2)))), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<i32, 4>, 2>>, subtract=false, element=array<array<i32, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 2>>, length=Some(5)>(%4), const<i32>(2)))), const<i32>(0)))), const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<i32, 4>, 2>>, subtract=false, element=array<array<i32, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 2>>, length=Some(5)>(%4), const<i32>(2)))), const<i32>(0)))), const<i32>(2))), const<i32>(1));
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%20, const<i32>(0));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%20), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%20, read<i32>(%34));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %28
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%21, const<i32>(0));
// DEFAULT-NEXT:                     condition: le<i32>(read<i32>(%21), const<i32>(1))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %35: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                         let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%21, read<i32>(%36));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %29
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%22, const<i32>(0));
// DEFAULT-NEXT:                             condition: le<i32>(read<i32>(%22), const<i32>(3))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %37: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:                                 let %38: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%37), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%22, read<i32>(%38));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<i32, 4>, 2>>, subtract=false, element=array<array<i32, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 2>>, length=Some(5)>(%4), read<i32>(%20)))), read<i32>(%21)))), read<i32>(%22)))), from_bool<i32, reason=promotion>(logical_and<bool>(ge<i32>(read<i32>(%20), const<i32>(2)), ge<i32>(read<i32>(%22), const<i32>(2)))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%9)), const<i32>(0)), ne<i32>(read<i32>(field0(%10)), const<i32>(0))), ne<i32>(read<i32>(field0(%11)), const<i32>(0))), ne<i32>(read<i32>(field2(%9)), const<i32>(1))), ne<i32>(read<i32>(field2(%10)), const<i32>(2))), ne<i32>(read<i32>(field2(%11)), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%20, const<i32>(0));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%20), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %39: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                 let %40: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%39), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%20, read<i32>(%40));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), read<i32>(%20))))), const<i32>(0)), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(array_decay<ptr<@type5>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), read<i32>(%20))))), const<i32>(0))))), const<i32>(0))), ne<i32>(read<i32>(field2(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(array_decay<ptr<@type5>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), read<i32>(%20))))), const<i32>(0))))), const<i32>(0)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     for %31
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%21, const<i32>(0));
// DEFAULT-NEXT:                         condition: le<i32>(read<i32>(%21), const<i32>(2))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %41: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                             let %42: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%41), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%21, read<i32>(%42));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(array_decay<ptr<@type5>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), read<i32>(%20))))), const<i32>(0))))), read<i32>(%21)))), const<i32>(0))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     for %32
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%21, const<i32>(1));
// DEFAULT-NEXT:                         condition: le<i32>(read<i32>(%21), const<i32>(2))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %43: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                             let %44: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%43), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%21, read<i32>(%44));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(array_decay<ptr<@type5>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), read<i32>(%20))))), read<i32>(%21))))), const<i32>(0))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(array_decay<ptr<@type5>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), read<i32>(%20))))), read<i32>(%21))))), const<i32>(0)))), const<i32>(4))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(array_decay<ptr<@type5>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), read<i32>(%20))))), read<i32>(%21))))), const<i32>(1)))), const<i32>(4))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                                 if logical_and<bool>(logical_or<bool>(lt<i32>(read<i32>(%20), const<i32>(5)), lt<i32>(read<i32>(%21), const<i32>(2))), logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(array_decay<ptr<@type5>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), read<i32>(%20))))), read<i32>(%21))))), const<i32>(2)))), const<i32>(0)), ne<i32>(read<i32>(field2(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(array_decay<ptr<@type5>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), read<i32>(%20))))), read<i32>(%21))))), const<i32>(0))))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     if logical_and<bool>(lt<i32>(read<i32>(%20), const<i32>(5)), ne<i32>(read<i32>(field2(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), read<i32>(%20))))), const<i32>(0)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(array_decay<ptr<@type5>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), const<i32>(5))))), const<i32>(2))))), const<i32>(2)))), const<i32>(5)), ne<i32>(read<i32>(field2(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(array_decay<ptr<@type5>, length=Some(3)>(field1(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), const<i32>(5))))), const<i32>(2))))), const<i32>(6))), ne<i32>(read<i32>(field2(deref(ptr_offset<ptr<@type6>, subtract=false, element=@type6, overflow=ub>(array_decay<ptr<@type6>, length=Some(6)>(%14), const<i32>(5))))), const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(memcmp, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type6>, length=Some(6)>(%14)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type6>, length=Some(6)>(%15)), const<u64>(408)), const<i32>(0)), ne<u64>(const<u64>(408), const<u64>(408)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(memcmp, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type6>, length=Some(6)>(%14)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type6>, length=Some(6)>(%16)), const<u64>(408)), const<i32>(0)), ne<u64>(const<u64>(408), const<u64>(408)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%17), const<i32>(0)))), const<i32>(0)))), const<i32>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%17), const<i32>(0)))), const<i32>(1)))), const<i32>(23))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%17), const<i32>(0)))), const<i32>(2)))), const<i32>(23)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%17), const<i32>(1)))), const<i32>(0)))), const<i32>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%17), const<i32>(1)))), const<i32>(1)))), const<i32>(23))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%17), const<i32>(1)))), const<i32>(2)))), const<i32>(24)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%17), const<i32>(2)))), const<i32>(0)))), const<i32>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%17), const<i32>(2)))), const<i32>(1)))), const<i32>(0))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%17), const<i32>(2)))), const<i32>(2)))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%18), const<i32>(0)))), const<i32>(27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
