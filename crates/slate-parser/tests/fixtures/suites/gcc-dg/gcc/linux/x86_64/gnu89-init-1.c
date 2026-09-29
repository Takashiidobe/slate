/* Test for GNU extensions to compound literals */
/* Origin: Jakub Jelinek <jakub@redhat.com> */
/* { dg-do run } */
/* { dg-options "-std=gnu89" } */

extern void abort (void);
extern void exit (int);

struct A { int i; int j; int k[4]; };
struct B { };
struct C { int i; };
struct D { int i; struct C j; };

/* As a GNU extension, we allow initialization of objects with static storage
   duration by compound literals.  It is handled as if the object
   was initialized only with the bracket enclosed list if compound literal's
   and object types match.  If the object being initialized has array type
   of unknown size, the size is determined by compound literal's initializer
   list, not by size of the compound literal.  */

struct A a = (struct A) { .j = 6, .k[2] = 12 };
struct B b = (struct B) { };
int c[] = (int []) { [2] = 6, 7, 8 };
int d[] = (int [3]) { 1 };
int e[2] = (int []) { 1, 2 };
int f[2] = (int [2]) { 1 };
struct C g[3] = { [2] = (struct C) { 13 }, [1] = (const struct C) { 12 } };
struct D h = { .j = (struct C) { 15 }, .i = 14 };
struct D i[2] = { [1].j = (const struct C) { 17 },
		  [0] = { 0, (struct C) { 16 } } };
struct C j[2][3] = { [0 ... 1] = { [0 ... 2] = (struct C) { 26 } } };
struct C k[3][2] = { [0 ... 2][0 ... 1] = (const struct C) { 27 } };

int main (void)
{
  if (a.i || a.j != 6 || a.k[0] || a.k[1] || a.k[2] != 12 || a.k[3])
    abort ();
  if (c[0] || c[1] || c[2] != 6 || c[3] != 7 || c[4] != 8)
    abort ();
  if (sizeof (c) != 5 * sizeof (int))
    abort ();
  if (d[0] != 1 || d[1] || d[2])
    abort ();
  if (sizeof (d) != 3 * sizeof (int))
    abort ();
  if (e[0] != 1 || e[1] != 2)
    abort ();
  if (sizeof (e) != 2 * sizeof (int))
    abort ();
  if (f[0] != 1 || f[1])
    abort ();
  if (sizeof (f) != 2 * sizeof (int))
    abort ();
  if (g[0].i || g[1].i != 12 || g[2].i != 13)
    abort ();
  if (h.i != 14 || h.j.i != 15)
    abort ();
  if (i[0].i || i[0].j.i != 16 || i[1].i || i[1].j.i != 17)
    abort ();
  if (j[0][0].i != 26 || j[0][1].i != 26 || j[0][2].i != 26)
    abort ();
  if (j[1][0].i != 26 || j[1][1].i != 26 || j[1][2].i != 26)
    abort ();
  if (k[0][0].i != 27 || k[0][1].i != 27 || k[1][0].i != 27)
    abort ();
  if (k[1][1].i != 27 || k[2][0].i != 27 || k[2][1].i != 27)
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT gnu89
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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:         field2 k: array<i32, 4>;
// DEFAULT-NEXT:     } [size=24, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type2 C = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 D = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: @type2;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %6 a: @type0 [storage=static] = copy<@type0, reason=assign>(read<@type0>(compound_literal %19 [storage=static] = aggregate<@type0, zero_fill=true>(field1 = const<i32>(6), field2 = aggregate<array<i32, 4>, zero_fill=true>(index2 = const<i32>(12))))) [linkage=external];
// DEFAULT-NEXT:     global %7 b: @type1 [storage=static] = copy<@type1, reason=assign>(read<@type1>(compound_literal %20 [storage=static] = aggregate<@type1, zero_fill=false>())) [linkage=external];
// DEFAULT-NEXT:     global %8 c: array<i32, 5> [storage=static] [align=16] = aggregate<array<i32, 5>, zero_fill=true>(index2 = const<i32>(6), index3 = const<i32>(7), index4 = const<i32>(8)) [linkage=external];
// DEFAULT-NEXT:     global %9 d: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=true>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %10 e: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %11 f: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=true>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %12 g: array<@type2, 3> [storage=static] = aggregate<array<@type2, 3>, zero_fill=true>(index1 = copy<@type2, reason=assign>(read<@type2>(compound_literal %22 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(12)))), index2 = copy<@type2, reason=assign>(read<@type2>(compound_literal %21 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(13))))) [linkage=external];
// DEFAULT-NEXT:     global %13 h: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = const<i32>(14), field1 = copy<@type2, reason=assign>(read<@type2>(compound_literal %23 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(15))))) [linkage=external];
// DEFAULT-NEXT:     global %14 i: array<@type3, 2> [storage=static] [align=16] = aggregate<array<@type3, 2>, zero_fill=false>(index0 = aggregate<@type3, zero_fill=false>(field0 = const<i32>(0), field1 = copy<@type2, reason=assign>(read<@type2>(compound_literal %25 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(16))))), index1 = aggregate<@type3, zero_fill=true>(field1 = copy<@type2, reason=assign>(read<@type2>(compound_literal %24 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(17)))))) [linkage=external];
// DEFAULT-NEXT:     global %15 j: array<array<@type2, 3>, 2> [storage=static] [align=16] = aggregate<array<array<@type2, 3>, 2>, zero_fill=false>(index0..=1 = aggregate<array<@type2, 3>, zero_fill=false>(index0..=2 = copy<@type2, reason=assign>(read<@type2>(compound_literal %26 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(26)))))) [linkage=external];
// DEFAULT-NEXT:     global %16 k: array<array<@type2, 2>, 3> [storage=static] [align=16] = aggregate<array<array<@type2, 2>, 3>, zero_fill=false>(index0..=1 = aggregate<array<@type2, 2>, zero_fill=false>(index0..=1 = copy<@type2, reason=assign>(read<@type2>(compound_literal %27 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(27))))), index2 = aggregate<array<@type2, 2>, zero_fill=false>(index0..=1 = copy<@type2, reason=assign>(read<@type2>(compound_literal %28 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(27)))))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%18 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %17 @main() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%6)), const<i32>(0)), ne<i32>(read<i32>(field1(%6)), const<i32>(6))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field2(%6)), const<i32>(0)))), const<i32>(0))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field2(%6)), const<i32>(1)))), const<i32>(0))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field2(%6)), const<i32>(2)))), const<i32>(12))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field2(%6)), const<i32>(3)))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%8), const<i32>(0)))), const<i32>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%8), const<i32>(1)))), const<i32>(0))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%8), const<i32>(2)))), const<i32>(6))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%8), const<i32>(3)))), const<i32>(7))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%8), const<i32>(4)))), const<i32>(8)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(20), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%9), const<i32>(0)))), const<i32>(1)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%9), const<i32>(1)))), const<i32>(0))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%9), const<i32>(2)))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(12), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%10), const<i32>(0)))), const<i32>(1)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%10), const<i32>(1)))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(1)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%11), const<i32>(1)))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(0))))), const<i32>(0)), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(1))))), const<i32>(12))), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(2))))), const<i32>(13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(%13)), const<i32>(14)), ne<i32>(read<i32>(field0(field1(%13))), const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(2)>(%14), const<i32>(0))))), const<i32>(0)), ne<i32>(read<i32>(field0(field1(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(2)>(%14), const<i32>(0)))))), const<i32>(16))), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(2)>(%14), const<i32>(1))))), const<i32>(0))), ne<i32>(read<i32>(field0(field1(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(2)>(%14), const<i32>(1)))))), const<i32>(17)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(deref(ptr_offset<ptr<array<@type2, 3>>, subtract=false, element=array<@type2, 3>, overflow=ub>(array_decay<ptr<array<@type2, 3>>, length=Some(2)>(%15), const<i32>(0)))), const<i32>(0))))), const<i32>(26)), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(deref(ptr_offset<ptr<array<@type2, 3>>, subtract=false, element=array<@type2, 3>, overflow=ub>(array_decay<ptr<array<@type2, 3>>, length=Some(2)>(%15), const<i32>(0)))), const<i32>(1))))), const<i32>(26))), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(deref(ptr_offset<ptr<array<@type2, 3>>, subtract=false, element=array<@type2, 3>, overflow=ub>(array_decay<ptr<array<@type2, 3>>, length=Some(2)>(%15), const<i32>(0)))), const<i32>(2))))), const<i32>(26)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(deref(ptr_offset<ptr<array<@type2, 3>>, subtract=false, element=array<@type2, 3>, overflow=ub>(array_decay<ptr<array<@type2, 3>>, length=Some(2)>(%15), const<i32>(1)))), const<i32>(0))))), const<i32>(26)), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(deref(ptr_offset<ptr<array<@type2, 3>>, subtract=false, element=array<@type2, 3>, overflow=ub>(array_decay<ptr<array<@type2, 3>>, length=Some(2)>(%15), const<i32>(1)))), const<i32>(1))))), const<i32>(26))), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(deref(ptr_offset<ptr<array<@type2, 3>>, subtract=false, element=array<@type2, 3>, overflow=ub>(array_decay<ptr<array<@type2, 3>>, length=Some(2)>(%15), const<i32>(1)))), const<i32>(2))))), const<i32>(26)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(deref(ptr_offset<ptr<array<@type2, 2>>, subtract=false, element=array<@type2, 2>, overflow=ub>(array_decay<ptr<array<@type2, 2>>, length=Some(3)>(%16), const<i32>(0)))), const<i32>(0))))), const<i32>(27)), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(deref(ptr_offset<ptr<array<@type2, 2>>, subtract=false, element=array<@type2, 2>, overflow=ub>(array_decay<ptr<array<@type2, 2>>, length=Some(3)>(%16), const<i32>(0)))), const<i32>(1))))), const<i32>(27))), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(deref(ptr_offset<ptr<array<@type2, 2>>, subtract=false, element=array<@type2, 2>, overflow=ub>(array_decay<ptr<array<@type2, 2>>, length=Some(3)>(%16), const<i32>(1)))), const<i32>(0))))), const<i32>(27)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(deref(ptr_offset<ptr<array<@type2, 2>>, subtract=false, element=array<@type2, 2>, overflow=ub>(array_decay<ptr<array<@type2, 2>>, length=Some(3)>(%16), const<i32>(1)))), const<i32>(1))))), const<i32>(27)), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(deref(ptr_offset<ptr<array<@type2, 2>>, subtract=false, element=array<@type2, 2>, overflow=ub>(array_decay<ptr<array<@type2, 2>>, length=Some(3)>(%16), const<i32>(2)))), const<i32>(0))))), const<i32>(27))), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(deref(ptr_offset<ptr<array<@type2, 2>>, subtract=false, element=array<@type2, 2>, overflow=ub>(array_decay<ptr<array<@type2, 2>>, length=Some(3)>(%16), const<i32>(2)))), const<i32>(1))))), const<i32>(27)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
